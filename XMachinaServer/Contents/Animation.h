#pragma once

#include "Component.h"

class Animation;

class AnimationClip {

public:
	std::string mName;

	float mLength{};
	std::vector<float> mKeyFrameTimes{};

public:
	AnimationClip(const std::string& name, float length) { mName = name; mLength = length; }

public:
	float GetLength() const { return mLength; }
	float GetFrameTime(int frame) const { return mKeyFrameTimes[frame]; }
	int GetMaxFrameRate() const { return static_cast<int>(mKeyFrameTimes.size()) - 1; }
};

struct MotionCallback {
	std::function<void()> Callback;
	bool Called;

	void Reset()
	{
		Called = false;
	}
};


struct AnimatorMotionInfo {
	SPtr<const AnimationClip> Clip{};
	float Speed{};
};

class AnimatorStateMachine;
class AnimatorMotion {

private:
	Animation* mAnimOwner;

private:
	sptr<const AnimationClip> mClip{};

	float 	mSpeed{};
	float 	mOriginSpeed{};
	float 	mCrntLength{};
	float	mMaxLength{};

	std::string mName{};
	std::map<float, MotionCallback> mCallbacks;
	sptr<MotionCallback> mCallbackStop;
	sptr<MotionCallback> mCallbackChange;
	sptr<MotionCallback> mCallbackAnimate;

public:
	AnimatorMotion(const AnimatorMotionInfo& info);
	AnimatorMotion(const AnimatorMotion& other);

	void SetAnimOwner(Animation* animOwner) { mAnimOwner = animOwner; }
	const std::string& GetName() const { return mName; }

public:
	bool Animate();

	void Reset();
	void ResetLength();
	void ResetCallbacks();
	void ResetOriginSpeed(float speed) { mSpeed = speed; mOriginSpeed = speed; }

	bool IsEndAnimation() const;

	void AddStartCallback(const std::function<void()>& callback);
	void AddEndCallback(const std::function<void()>& callback);
	void AddStopCallback(const std::function<void()>& callback);
	void DelStopCallback();
	void AddChangeCallback(const std::function<void()>& callback);
	void DelChabgeCallback();
	void AddAnimateCallback(const std::function<void()>& callback);
	void DelAnimateCallback();

	void AddCallback(const std::function<void()>& callback, int frame);

	void DelCallback(int frame);

protected:
	virtual float GetFrameTime(int frame);
};


//
struct AnimationCondition {
	std::string mode{};
	std::string paramName{};
	float threshold{};
};

class AnimatorController;
class AnimatorLayer;
struct AnimatorTransition {
	std::string Destination{};
	std::vector<AnimationCondition> Conditions{};

	std::string CheckTransition(const AnimatorController* controller) const;
};

class AnimatorStateMachine {
	using StateMap = std::unordered_map<std::string, sptr<AnimatorMotion>>;
	using StateMachineMap = std::unordered_map<std::string, sptr<AnimatorStateMachine>>;

private:
	std::string mName;
	SPtr<AnimatorLayer> mLayer{};

	StateMap mStates{};
	StateMachineMap mStateMachines{};

	std::vector<sptr<const AnimatorTransition>> mEntryTransitions{};

public:
	AnimatorStateMachine(const std::string& name, const std::vector<sptr<const AnimatorTransition>>& entryTransitions);
	AnimatorStateMachine(const AnimatorStateMachine& other);
	void SetAnimOwner(Animation* animOwner) {
		for (auto& iter : mStates) {
			iter.second->SetAnimOwner(animOwner);
		}
	}
public:
	const std::string& GetName() const { return mName; }
	sptr<AnimatorMotion> GetState(const std::string& name) const;
	sptr<AnimatorStateMachine> GetStateMachine(const std::string& name) const;
	const AnimatorLayer* GetLayer() const { return mLayer.get(); }


public:
	sptr<AnimatorMotion> Entry() const;
	sptr<AnimatorMotion> CheckTransition(const AnimatorController* controller) const;

	void AddState(rsptr<AnimatorMotion> state);
	void AddStateMachine(rsptr<AnimatorStateMachine> stateMachine);

	sptr<AnimatorMotion> FindMotionByName(const std::string& motionName) const;

};
//



//
class AnimatorController;
class AnimatorMotion;
class AnimatorStateMachine;
class AnimatorLayer {
private:
	std::string mName{};

	sptr<AnimatorStateMachine>	mRootStateMachine{};
	sptr<AnimatorMotion>		mCrntState{};

public:
	AnimatorLayer(const std::string& name, sptr<AnimatorStateMachine> rootStateMachine);
	AnimatorLayer(const AnimatorLayer& other);

public:
	sptr<AnimatorMotion> CheckTransition(const AnimatorController* controller, bool isChangeImmed);
	void ChangeState(rsptr<AnimatorMotion> state);

	sptr<AnimatorMotion> FindMotionByName(const std::string& motionName) const;
	sptr<AnimatorMotion> GetCrntMotion() const { return mCrntState; }

	void SetAnimOwner(Animation* animOwner) { 
		mRootStateMachine->SetAnimOwner(animOwner);
	}

	std::string GetName() { return mName; }
	void Animate();

};
//


//
struct AnimatorParameter {
	enum class Type : BYTE {
		Float,
		Int,
		Bool,
	};

	Type type{};
	union value {
		bool b;
		int i;
		float f;
	} val{};
};

class AnimatorController {
	template <typename T>
	static constexpr bool is_valid_param_type = (std::is_same<T, bool>::value || std::is_same<T, int>::value || std::is_same<T, float>::value);
	
private:
	std::string mName{};
	std::unordered_map<std::string, AnimatorParameter> mParameters{};

	std::vector<sptr<AnimatorLayer>> mLayers;

public:
	AnimatorController(const std::unordered_map<std::string, AnimatorParameter>& parameters, std::vector<sptr<AnimatorLayer>> layers);
	AnimatorController(const AnimatorController& other);

public:
	void SetAnimOwner(Animation* animOwner) { for (auto& layer : mLayers) layer->SetAnimOwner(animOwner); }

	const AnimatorParameter* GetParam(const std::string& paramName) const { return &mParameters.at(paramName); }
	bool HasParam(const std::string paramName) const { return mParameters.contains(paramName); }

	template<class T, typename std::enable_if<is_valid_param_type<T>>::type* = nullptr>
	bool SetValueOnly(const std::string& paramName, T value)
	{
		if (!HasParam(paramName)) {
			return false;
		}

		AnimatorParameter::value val{};

		auto& param = mParameters[paramName];
		switch (param.type) {
		case AnimatorParameter::Type::Bool:
			val.b = static_cast<bool>(value);
			if (param.val.b == val.b) {
				return true;
			}
			break;
		case AnimatorParameter::Type::Int:
			val.i = static_cast<int>(value);
			if (param.val.i == val.i) {
				return true;
			}
			break;
		case AnimatorParameter::Type::Float:
			val.f = static_cast<float>(value);
			if (Math::IsEqual(param.val.f, val.f)) {
				return true;
			}
			break;
		default:
			assert(0);
			break;
		}

		param.val = val;
		return true;
	}

	// isChangeImmed == true : transition animation state without waiting & blending
	template<class T, typename std::enable_if<is_valid_param_type<T>>::type* = nullptr>
	void SetValue(const std::string& paramName, T value, bool isChangeImmed = false)
	{
		if (!SetValueOnly(paramName, value)) {
			return;
		}

		if (isChangeImmed) {
			CheckTransition(isChangeImmed);
		}
	}

	void SetName(const std::string& name) { mName = name; }
	void Animate();

public:
	void InitLayers();
	void CheckTransition(bool isChangedImmed = false) const;

	sptr<AnimatorMotion> FindMotionByName(const std::string& motionName, const std::string& layerName = "Base Layer") const;
	sptr<AnimatorLayer>  FindLayerByName(const std::string& layerName) const;

};
//




class Animation : public Component {
private:
	SPtr<AnimatorController> mController{};

public:
	Animation();
	Animation(const Animation& other);
	Animation(SPtr<GameObject> owner, ComponentInfo::Type Type);
	~Animation();

	virtual void Clone(SPtr<Component> other);

	virtual bool WakeUp() override;
	virtual bool Start() override;
	virtual bool Update() override;
	virtual bool Animate() override;
	virtual bool LateUpdate() override;

public:
	void Load(const std::string& controller);
	SPtr<AnimatorController> GetController() { return mController; }

};

