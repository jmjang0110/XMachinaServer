#include "pch.h"
#include "ResourceManager.h"

#include "FileIO.h"
#include "GameObject.h"
#include "Collider.h"


namespace {
	const std::string kTerrainDataPath = "Contents/Resource/Terrain.bin";
	const std::string kModelDataPath = "Contents/Resource/Models/";
	const std::string kSceneDataPath = "Contents/Resource/ServerScene.bin";
}

DEFINE_SINGLETON(ResourceManager);


void Model::SetChild(SPtr<Model> child)
{
	if (mChild) {
		if (mChild->mSibling) {
			SPtr<Model> tail = mChild->mSibling;
			while (tail->mSibling) {
				tail = tail->mSibling;
			}
			tail->mSibling = child;
		}
		else {
			child->mSibling = nullptr;
			mChild->mSibling = child;
		}
	}
	else {
		mChild = child;
	}
}


void BattleScene::Load()
{
	std::ifstream file = FileIO::OpenBinFile(kSceneDataPath);

	std::string token{};

	FileIO::ReadString(file, token); // <GameObjects>:
	int objectCnt = FileIO::ReadVal<int>(file);
	mBuildings.reserve(objectCnt);
	mEnemies.reserve(objectCnt);

	int sameObjectCnt = 0;
	sptr<Model> model{};
	ObjectTag objectTag{};
	GameObjectInfo::Type objectType{};

	for (int i = 0; i < objectCnt; ++i) {
		if (sameObjectCnt <= 0) {
			FileIO::ReadString(file, token); // <Tag>:
			std::string tag = FileIO::ReadString(file);

			FileIO::ReadString(file, token); // <FileName>:
			std::string modelName = FileIO::ReadString(file);
			if (tag == "Enemy") {
				objectTag = ObjectTag::Enemy;
				if (modelName == "Onyscidus")
					objectType = GameObjectInfo::Type::Monster_Onyscidus;
				else if (modelName == "Ursacetus;")
					objectType = GameObjectInfo::Type::Monster_Ursacetus;
				else if (modelName == "AdvancedCombat_5")
					objectType = GameObjectInfo::Type::Monster_AdvancedCombat_5;
			}
			else if(tag == "Building" || tag == "Dissolve_Building") {
				objectTag = ObjectTag::Building;
				objectType = GameObjectInfo::Type::Building;
			}
			else {
				objectTag = ObjectTag::None;
				LOG_MGR->Cout("[MDH_Warning] untagged(", tag, ") : ", modelName, "\n");
			}

			if (objectTag != ObjectTag::None) {
				model = RESOURCE_MGR->GetModel(modelName);
			}
			else {
				model = nullptr;
			}

			FileIO::ReadString(file, token); // <Transforms>:
			sameObjectCnt = FileIO::ReadVal<int>(file);
		}

		if (sameObjectCnt > 0) {
			SPtr<GameObject> object = std::make_shared<GameObject>();
			object->SetType(objectType);
			object->SetID(i);
			
			Matrix transform = FileIO::ReadVal<Matrix>(file);

			object->AddComponent<Transform>(ComponentInfo::Type::Transform)->SetLocalTransform(transform);
			const auto& collider = object->AddComponent<Collider>(ComponentInfo::Type::Collider);

			if (model) {
				object->SetName(model->mName);
				model->mTransform = transform;
				

				collider->SetBoundingSphereList(model->mBSList);
				collider->SetBoundingBoxList(model->mBoxList);
			}
			
			switch (objectTag) {
			case ObjectTag::Building:
				mBuildings.push_back(object);
				break;
			case ObjectTag::Enemy:
				mEnemies.push_back(object);
				break;
			default:
				break;
			}

			--sameObjectCnt;
		}
	}
}



ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	mTileMap      = nullptr;
	mHeightMapImg = nullptr;
	mBattleScene  = nullptr;
}

void ResourceManager::Init()
{
	LoadTerrain();
	LoadModels();

	mBattleScene = std::make_shared<BattleScene>();
	mBattleScene->Load();
}

/// +----------------------------------------------------
///	¡å Height Map Image Init ( Terrain.bin )
/// ----------------------------------------------------+
void ResourceManager::LoadTerrain()
{
	mHeightMapImg = std::make_shared<HeightMapImage>();
	mHeightMapImg->Init(kTerrainDataPath);
}

/// +----------------------------------------------------
///	¡å Model Init (Models)
/// ----------------------------------------------------+
void ResourceManager::LoadModels()
{
	for (const auto& modelFile : std::filesystem::directory_iterator(kModelDataPath)) {
		const std::string fileName = modelFile.path().filename().string();
		const std::string modelName = FileIO::RemoveExtension(fileName);

		sptr<Model> model = FileIO::LoadGeometryFromFile(kModelDataPath + fileName);
		mModels[modelName] = model;
	}
}