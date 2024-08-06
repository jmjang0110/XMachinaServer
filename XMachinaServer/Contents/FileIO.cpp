#include "pch.h"
#include "FileIO.h"

#include "ResourceManager.h"
#include "Animation.h"

namespace {
    sptr<Model> LoadFrame(std::ifstream& file)
    {
        sptr<Model> model = std::make_shared<Model>();

        std::string token;
        FileIO::ReadString(file, token); // <Name>:
        FileIO::ReadString(file, model->mName);

        FileIO::ReadString(file, token); // <Matrix>:
        FileIO::ReadVal(file, model->mTransform);

        FileIO::ReadString(file, token); // <BoundingSphere>:
        FileIO::ReadVal(file, model->mBS.Center);
        FileIO::ReadVal(file, model->mBS.Radius);

        FileIO::ReadString(file, token); // <BoundingBoxes>:
        int boxCnt = FileIO::ReadVal<int>(file);

        if (boxCnt > 0)
        {
            std::vector<Vec3> centers;
            std::vector<Vec3> extents;
            FileIO::ReadRange(file, centers, boxCnt);
            FileIO::ReadRange(file, extents, boxCnt);

            MyBoundingOrientedBox box{};
            for (int i = 0; i < boxCnt; ++i) {
                box.SetOrigin(centers[i]);
                box.Center = centers[i];
                box.Extents = extents[i];

                model->mBoxList.push_back(box);
            }
        }

        return model;
    }
}


sptr<Model> FileIO::LoadGeometryFromFile(const std::string& filePath)
{
    std::ifstream file = OpenBinFile(filePath);

    sptr<Model> model{};
    std::string controller;
    std::string token;

    bool isEOF{ false };
    while (!isEOF) {
        FileIO::ReadString(file, token); // <Frame>:

        switch (Hash(token)) {
        case Hash("<Frame>:"):
            model = ::LoadFrame(file);
            break;
        case Hash("<Controller>:"):
            controller = FileIO::ReadString(file);
            break;
        case Hash("</Frame>"):
            isEOF = true;
            break;
        default:
            assert(0);
            break;
        }
    }

    model->mAnimatorController = controller;

    return model;
}



// animation
namespace {
    sptr<AnimatorMotion> LoadAnimatorBlendTree(std::ifstream& file, AnimatorMotionInfo& motionInfo)
    {
        std::string name;
        FileIO::ReadString(file, name);

        int motionCnt = FileIO::ReadVal<int>(file);

        sptr<const AnimationClip> clip{};
        for (int i = 0; i < motionCnt; ++i) {
            clip = FileIO::ReadAnimationClip(file);
            FileIO::ReadVal<Vec2>(file);
        }
        motionInfo.Clip = clip;

        return std::make_shared<AnimatorMotion>(motionInfo);
    }

    sptr<AnimatorMotion> LoadAnimatorState(std::ifstream& file, AnimatorMotionInfo& motionInfo)
    {
        motionInfo.Clip = FileIO::ReadAnimationClip(file);

        return std::make_shared<AnimatorMotion>(motionInfo);
    }

    sptr<AnimatorStateMachine> LoadAnimatorStateMachine(std::ifstream& file)
    {
        std::string token;

        ///// Default State Machine Info /////
        std::string stateMachineName = FileIO::ReadString(file);

        // Entry //
        FileIO::ReadString(file, token);	// <Entry>:
        std::vector<sptr<const AnimatorTransition>> entryTransitions = FileIO::LoadTransitions(file);

        sptr<AnimatorStateMachine> stateMachine = std::make_shared<AnimatorStateMachine>(stateMachineName, entryTransitions);
        //////////////////////////////////


        // Motions //
        FileIO::ReadString(file, token);	// <Motions>:
        int stateSize = FileIO::ReadVal<int>(file);

        AnimatorMotionInfo motionInfo{};

        for (int i = 0; i < stateSize; ++i) {

            sptr<AnimatorMotion> motion{};

            FileIO::ReadVal<float>(file, motionInfo.Speed);
            FileIO::LoadStateTransitions(file);

            std::string motionType = FileIO::ReadString(file);

            switch (Hash(motionType)) {
            case Hash("<BlendTree>:"):
                motion = LoadAnimatorBlendTree(file, motionInfo);
                break;
            case Hash("<State>:"):
                motion = LoadAnimatorState(file, motionInfo);
                break;
            default:
                assert(0);
                break;
            }

            stateMachine->AddState(motion);
        }

        // Sub State Machines //
        FileIO::ReadString(file, token);	// <SubStateMachines>:
        int layerSize = FileIO::ReadVal<int>(file);
        for (int i = 0; i < layerSize; ++i) {
            stateMachine->AddStateMachine(LoadAnimatorStateMachine(file));
        }

        return stateMachine;
    }
}


sptr<AnimationClip> FileIO::LoadAnimationClip(const std::string& filePath)
{
    std::ifstream file = OpenBinFile(filePath);

    std::string clipName = FileIO::ReadString(file); //Animation Set Name

    const float length = FileIO::ReadVal<float>(file);
    const int frameRate = FileIO::ReadVal<int>(file);
    const int boneCnt = FileIO::ReadVal<int>(file);
    const int keyFrameCnt = static_cast<int>(ceil(length * frameRate));

    sptr<AnimationClip> clip = std::make_shared<AnimationClip>(clipName, length);

    clip->mKeyFrameTimes.resize(keyFrameCnt);
    for (int i = 0; i < keyFrameCnt; ++i) {
        FileIO::ReadVal(file, clip->mKeyFrameTimes[i]);

        for (int j = 0; j < boneCnt; ++j) {
            FileIO::ReadString(file);
            Matrix m = FileIO::ReadVal<Matrix>(file);
        }
    }


    return clip;
}

sptr<AnimatorController> FileIO::LoadAnimatorController(const std::string& filePath)
{
    std::string token{};
    std::ifstream file = OpenBinFile(filePath);

    // Params //
    std::unordered_map<std::string, AnimatorParameter> params{};

    FileIO::ReadString(file, token);	// <Params>:
    int paramSize = FileIO::ReadVal<int>(file);

    for (int i = 0; i < paramSize; ++i) {
        AnimatorParameter param{};
        std::string paramName = FileIO::ReadString(file);

        // Set param type and default value
        std::string paramType = FileIO::ReadString(file);
        switch (Hash(paramType)) {
        case Hash("Float"):
            param.type = AnimatorParameter::Type::Float;
            FileIO::ReadVal(file, param.val.f);
            break;
        case Hash("Int"):
            param.type = AnimatorParameter::Type::Int;
            FileIO::ReadVal(file, param.val.i);
            break;
        case Hash("Bool"):
            param.type = AnimatorParameter::Type::Bool;
            FileIO::ReadVal(file, param.val.b);
            break;
        default:
            assert(0);
            break;
        }

        params.insert(std::make_pair(paramName, param));
    }

    // Layer //
    std::vector<sptr<AnimatorLayer>> layers;

    FileIO::ReadString(file, token);	// <Layers>:
    int layerCnt = FileIO::ReadVal<int>(file);
    layers.resize(layerCnt);
    for (auto& layer : layers) {
        layer = FileIO::LoadAnimatorLayer(file);
    }

    return std::make_shared<AnimatorController>(params, layers);
}

sptr<AnimatorLayer> FileIO::LoadAnimatorLayer(std::ifstream& file)
{
    std::string token;

    std::string layerName;
    FileIO::ReadString(file, layerName);

    FileIO::ReadString(file, token);	// <AvatarMask>: or <StateMachines>:
    if (token == "<AvatarMask>:") {
        LoadAvatarMask(file);

        FileIO::ReadString(file, token);	// <StateMachines>:
    }

    sptr<AnimatorStateMachine> rootStateMachine = LoadAnimatorStateMachine(file);

    return std::make_shared<AnimatorLayer>(layerName, rootStateMachine);
}


std::vector<sptr<const AnimatorTransition>> FileIO::LoadTransitions(std::ifstream& file)
{
    std::vector<sptr<const AnimatorTransition>> transitions{};

    int transitionSize = FileIO::ReadVal<int>(file);
    transitions.resize(transitionSize);

    for (int i = 0; i < transitionSize; ++i) {
        transitions[i] = FileIO::LoadTransition(file);
    }

    return transitions;
}

void FileIO::LoadStateTransitions(std::ifstream& file)
{
    int transitionSize = FileIO::ReadVal<int>(file);

    for (int i = 0; i < transitionSize; ++i) {
        FileIO::LoadTransition(file);
        FileIO::ReadVal<float>(file);
    }
}

sptr<const AnimatorTransition> FileIO::LoadTransition(std::ifstream& file)
{
    sptr<AnimatorTransition> transition = std::make_shared<AnimatorTransition>();

    FileIO::ReadString(file, transition->Destination);
    int conditionSize = FileIO::ReadVal<int>(file);
    transition->Conditions.resize(conditionSize);

    for (auto& condition : transition->Conditions) {
        FileIO::ReadString(file, condition.mode);
        FileIO::ReadString(file, condition.paramName);

        switch (Hash(condition.mode)) {
        case Hash("If"):
        case Hash("IfNot"):
            break;
        default:
            FileIO::ReadVal(file, condition.threshold);
            break;
        }
    }

    return transition;
}


sptr<const AnimationClip> FileIO::ReadAnimationClip(std::ifstream& file)
{
    std::string clipFolder = FileIO::ReadString(file);
    std::string clipName = FileIO::ReadString(file);
    return RESOURCE_MGR->GetAnimationClip(clipFolder + '/' + clipName);
}

void FileIO::LoadAvatarMask(std::ifstream& file)
{
    FileIO::ReadVal<bool>(file);
    FileIO::ReadVal<bool>(file);
    FileIO::ReadVal<bool>(file);
    FileIO::ReadVal<bool>(file);
    FileIO::ReadVal<bool>(file);
    FileIO::ReadVal<bool>(file);
    FileIO::ReadVal<bool>(file);
    FileIO::ReadVal<bool>(file);
    FileIO::ReadVal<bool>(file);
}