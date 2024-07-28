#include "pch.h"
#include "Animation.h"

#include "ResourceManager.h"
#include "ServerLib/ThreadManager.h"
#include "GameObject.h"

Animation::Animation()
{
}

Animation::Animation(const Animation& other)
    : Component(other)
{

}

Animation::Animation(SPtr<GameObject> owner, ComponentInfo::Type Type)
    : Component(owner, Type, static_cast<UINT32>(Type))
{
}

Animation::~Animation()
{
}

void Animation::Clone(SPtr<Component> other)
{
    Component::Clone(other);
	SPtr<Animation> other_AnimationComponent = std::static_pointer_cast<Animation>(other);

	mController = MEMORY->Make_Shared<AnimatorController>(*(other_AnimationComponent->GetController().get()));
	mController->SetAnimOwner(this);

}

bool Animation::WakeUp()
{
    return false;
}

bool Animation::Start()
{
    return false;
}

bool Animation::Update()
{
	Component::Update();

    return true;
}

bool Animation::LateUpdate()
{
	Component::LateUpdate();

    return false;
}

bool Animation::Animate()
{
	mController->Animate();

	return true;
}

void Animation::Load(const std::string& controller)
{
	mController = std::make_shared<AnimatorController>(*RESOURCE_MGR->GetAnimatorController(controller));
	mController->SetName(controller);
}

AnimatorController::AnimatorController(const std::unordered_map<std::string, AnimatorParameter>& parameters, std::vector<sptr<AnimatorLayer>> layers)
	:
	mParameters(parameters),
	mLayers(layers)
{
}

AnimatorController::AnimatorController(const AnimatorController& other)
	:
	mParameters(other.mParameters),
	mName(other.mName)
{
	// layers 복사 하기
	mLayers.reserve(other.mLayers.size());
	for (auto& layer : other.mLayers) {
		mLayers.push_back(std::make_shared<AnimatorLayer>(*layer));
	}

	CheckTransition();
}

void AnimatorController::InitLayers()
{
}

void AnimatorController::Animate()
{
	CheckTransition();

	//LOG_MGR->Cout("Animate : ", mName, " ");
	for (auto& layer : mLayers) {
		layer->Animate();
	}

}

void AnimatorController::CheckTransition() const
{
    for (auto& layer : mLayers) {
        layer->CheckTransition(this);
    }
}

sptr<AnimatorMotion> AnimatorController::FindMotionByName(const std::string& motionName, const std::string& layerName) const
{
	const auto& result = FindLayerByName(layerName)->FindMotionByName(motionName);
	if (!result) {
		LOG_MGR->Cout("[WARNING] Couldn't Find Motion : ", motionName, " (", mName, ")\n");
	}

	return result;
}

sptr<AnimatorLayer> AnimatorController::FindLayerByName(const std::string& layerName) const
{
	for (auto& layer : mLayers) {
		if (layer->GetName() == layerName) {
			return layer;
		}
	}

	throw std::runtime_error("there's no layer name in controller");
}

AnimatorMotion::AnimatorMotion(const AnimatorMotionInfo& info)
	:
	mClip(info.Clip),
	mName(info.Clip->mName),
	mOriginSpeed(info.Speed),
	mSpeed(info.Speed),
	mCrntLength(0),
	mMaxLength(info.Clip->mLength)
{
}

AnimatorMotion::AnimatorMotion(const AnimatorMotion& other)
	:
	mClip(other.mClip),
	mName(other.mName),
	mOriginSpeed(other.mOriginSpeed),
	mSpeed(other.mSpeed),
	mCrntLength(other.mCrntLength),
	mMaxLength(other.mMaxLength)
{
}

bool AnimatorMotion::Animate()
{
	// LOG_MGR->Cout("[Animation : ", mName, "] (", DELTA_TIME, ")");
	mCrntLength += mSpeed * mAnimOwner->GetOwner()->GetDeltaTime();

	for (auto& [time, callback] : mCallbacks | std::ranges::views::reverse) {
		if (mCrntLength >= time) {
			if (!callback.Called) {
				callback.Callback();
				callback.Called = true;
			}
			break;
		}
	}
	if (IsEndAnimation()) {
		ResetLength();
		return true;
	}

	return false;
}

void AnimatorMotion::Reset()
{
	mCrntLength = 0.f;
	mSpeed = mOriginSpeed;

	ResetCallbacks();
}

void AnimatorMotion::ResetLength()
{
	mCrntLength = 0.f;

	ResetCallbacks();
}

void AnimatorMotion::ResetCallbacks()
{
	for (auto& callback : mCallbacks) {
		callback.second.Reset();
	}
}

bool AnimatorMotion::IsEndAnimation() const
{
	return (mCrntLength >= mMaxLength) || (mCrntLength <= 0);
}

void AnimatorMotion::AddStartCallback(const std::function<void()>& callback)
{
	AddCallback(callback, 0);

}

void AnimatorMotion::AddEndCallback(const std::function<void()>& callback)
{
	AddCallback(callback, mClip->GetMaxFrameRate());

}

void AnimatorMotion::AddStopCallback(const std::function<void()>& callback)
{
	mCallbackStop = std::make_shared<MotionCallback>(callback);

}

void AnimatorMotion::DelStopCallback()
{
	mCallbackStop = nullptr;

}

void AnimatorMotion::AddChangeCallback(const std::function<void()>& callback)
{
	mCallbackChange = std::make_shared<MotionCallback>(callback);

}

void AnimatorMotion::DelChabgeCallback()
{
	mCallbackChange = nullptr;

}

void AnimatorMotion::AddCallback(const std::function<void()>& callback, int frame)
{
	mCallbacks.insert(std::make_pair(GetFrameTime(frame), MotionCallback{ callback }));
}

void AnimatorMotion::DelCallback(int frame)
{
	mCallbacks.erase(GetFrameTime(frame));
}

float AnimatorMotion::GetFrameTime(int frame)
{
	return mClip->GetFrameTime(frame);
}

AnimatorLayer::AnimatorLayer(const std::string& name, sptr<AnimatorStateMachine> rootStateMachine)
	:
	mName(name),
	mRootStateMachine(rootStateMachine)
{
	mCrntState = mRootStateMachine->Entry();
}

AnimatorLayer::AnimatorLayer(const AnimatorLayer& other)
{
	mName = other.mName;
	mRootStateMachine = std::make_shared<AnimatorStateMachine>(*other.mRootStateMachine);
	mCrntState = mRootStateMachine->Entry();
}

sptr<AnimatorMotion> AnimatorLayer::CheckTransition(const AnimatorController* controller)
{
	const auto& nextState = mRootStateMachine->CheckTransition(controller);
	if (!nextState) {
		return nullptr;
	}

	ChangeState(nextState);

	return nullptr;
}

void AnimatorLayer::ChangeState(rsptr<AnimatorMotion> state)
{
	if (!state) {
		return;
	}
	if (mCrntState == state) {
		return;
	}

	mCrntState->Reset();
	mCrntState = state;
}

sptr<AnimatorMotion> AnimatorLayer::FindMotionByName(const std::string& motionName) const
{
	return mRootStateMachine->FindMotionByName(motionName);
}

void AnimatorLayer::Animate()
{
	mCrntState->Animate();

}

AnimatorStateMachine::AnimatorStateMachine(const std::string& name, const std::vector<sptr<const AnimatorTransition>>& entryTransitions)
	:
	mName(name),
	mEntryTransitions(entryTransitions)
{
}

AnimatorStateMachine::AnimatorStateMachine(const AnimatorStateMachine& other)
{
	mName = other.mName;
	mEntryTransitions = other.mEntryTransitions;

	for (const auto& [name, state] : other.mStates) {
		AddState(std::make_shared<AnimatorMotion>(*state));
	}

	for (const auto& [name, stateMachine] : other.mStateMachines) {
		AddStateMachine(std::make_shared<AnimatorStateMachine>(*stateMachine));
	}
}

sptr<AnimatorMotion> AnimatorStateMachine::GetState(const std::string& name) const
{
	if (mStates.contains(name)) {
		return mStates.at(name);
	}

	return nullptr;
}

sptr<AnimatorStateMachine> AnimatorStateMachine::GetStateMachine(const std::string& name) const
{
	if (mStateMachines.contains(name)) {
		return mStateMachines.at(name);
	}

	return nullptr;
}

sptr<AnimatorMotion> AnimatorStateMachine::Entry() const
{
	if (mStates.contains(mEntryTransitions.front()->Destination)) {
		return mStates.at(mEntryTransitions.front()->Destination);
	}
	else if (mStateMachines.contains(mEntryTransitions.front()->Destination)) {
		return mStateMachines.at(mEntryTransitions.front()->Destination)->Entry();
	}

	return nullptr;
}

sptr<AnimatorMotion> AnimatorStateMachine::CheckTransition(const AnimatorController* controller) const
{
	for (const auto& transition : mEntryTransitions) {
		std::string destination = transition->CheckTransition(controller);
		if (destination != "") {
			if (sptr<AnimatorMotion> state = GetState(destination)) {
				return state;
			}

			if (sptr<AnimatorStateMachine> subStateMachine = GetStateMachine(destination)) {
				return subStateMachine->CheckTransition(controller);
			}
		}
	}

	return nullptr;
}

void AnimatorStateMachine::AddState(rsptr<AnimatorMotion> state)
{
	mStates.insert(std::make_pair(state->GetName(), state));
}

void AnimatorStateMachine::AddStateMachine(rsptr<AnimatorStateMachine> stateMachine)
{
	mStateMachines.insert(std::make_pair(stateMachine->GetName(), stateMachine));
}

sptr<AnimatorMotion> AnimatorStateMachine::FindMotionByName(const std::string& motionName) const
{
	for (auto& state : mStates) {
		auto& motion = state.second;
		if (motion->GetName() == motionName) {
			return motion;
		}
	}

	for (auto& stateMachine : mStateMachines) {
		if (auto motion = stateMachine.second->FindMotionByName(motionName)) {
			return motion;
		}
	}

	return nullptr;
}

std::string AnimatorTransition::CheckTransition(const AnimatorController* controller) const
{
	if (Conditions.empty()) {
		return Destination;
	}

	for (const auto& condition : Conditions) {
		const AnimatorParameter* param = controller->GetParam(condition.paramName);
		if (!param) {
			continue;
		}

		switch (Hash(condition.mode)) {
		case Hash("If"):	// == true
		{
			bool value = param->val.b;
			if (value != true) {
				return "";
			}
		}

		break;
		case Hash("IfNot"):	// == false
		{
			bool value = param->val.b;
			if (value != false) {
				return "";
			}
		}

		break;
		case Hash("Equals"):	// == Math::IsEqual
		{
			switch (param->type) {
			case AnimatorParameter::Type::Int:
			{
				int value = param->val.i;
				if (value != static_cast<int>(round(condition.threshold))) {
					return "";
				}
			}

			break;
			case AnimatorParameter::Type::Float:
			{
				float value = param->val.f;
				if (!Math::IsEqual(value, condition.threshold)) {
					return "";
				}
			}

			break;
			default:
				assert(0);
				break;
			}
		}

		break;
		case Hash("NotEqual"):	// == !Math::IsEqual
		{
			switch (param->type) {
			case AnimatorParameter::Type::Int:
			{
				int value = param->val.i;
				if (value == static_cast<int>(round(condition.threshold))) {
					return "";
				}
			}

			break;
			case AnimatorParameter::Type::Float:
			{
				float value = param->val.f;
				if (Math::IsEqual(value, condition.threshold)) {
					return "";
				}
			}

			break;
			default:
				assert(0);
				break;
			}
		}

		break;
		case Hash("Greater"):
		{
			switch (param->type) {
			case AnimatorParameter::Type::Int:
			{
				int value = param->val.i;
				if (value <= static_cast<int>(round(condition.threshold))) {
					return "";
				}
			}

			break;
			case AnimatorParameter::Type::Float:
			{
				float value = param->val.f;
				if (value <= condition.threshold) {
					return "";
				}
			}

			break;
			default:
				assert(0);
				break;
			}
		}

		break;
		case Hash("Less"):
		{
			switch (param->type) {
			case AnimatorParameter::Type::Int:
			{
				int value = param->val.i;
				if (value >= static_cast<int>(round(condition.threshold))) {
					return "";
				}
			}

			break;
			case AnimatorParameter::Type::Float:
			{
				float value = param->val.f;
				if (value >= condition.threshold) {
					return "";
				}
			}

			break;
			default:
				assert(0);
				break;
			}
		}

		break;
		default:
			assert(0);
			break;
		}
	}

	return Destination;
}
