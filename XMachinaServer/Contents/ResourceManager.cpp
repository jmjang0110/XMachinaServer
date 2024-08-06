#include "pch.h"
#include "ResourceManager.h"

#include "FileIO.h"
#include "GameObject.h"
#include "Collider.h"
#include "Transform.h"
#include "SectorController.h"

#include "Script_Building.h"

#include "Script_Enemy.h"
#include "Script_EnemyController.h"

#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Ursacetus.h"
#include "Script_Onyscidus.h"
#include "Script_Arack.h"
#include "Script_Aranobot.h"
#include "Script_Ceratoferox.h"
#include "Script_Gobbler.h"
#include "Script_LightBipedMech.h"
#include "Script_Rapax.h"
#include "Script_Anglerox.h"
#include "Script_MiningMech.h"

#include "Script_BehaviorTree.h"
#include "Script_DefaultEnemyBT.h"
#include "Script_PheroDropper.h"




namespace {
	const std::string kTerrainDataPath = "Contents/Resource/Terrain.bin";
	const std::string kModelDataPath = "Contents/Resource/Models/";
	const std::string kSceneDataPath = "Contents/Resource/ServerScene.bin";
	const std::string kAnimationClipDataPath = "Contents/Resource/AnimationClips/";
	const std::string kAnimatorControllerDataPath = "Contents/Resource/AnimatorControllers/";
}

DEFINE_SINGLETON(ResourceManager);


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
				else if (modelName == "Ursacetus")
					objectType = GameObjectInfo::Type::Monster_Ursacetus;
				else if (modelName == "AdvancedCombatDroid")
					objectType = GameObjectInfo::Type::Monster_AdvancedCombat_5;
				else if (modelName == "Anglerox")
					objectType = GameObjectInfo::Type::Monster_Anglerox;
				else if (modelName == "Arack")
					objectType = GameObjectInfo::Type::Monster_Arack;
				else if (modelName == "Ceratoferox")
					objectType = GameObjectInfo::Type::Monster_Ceratoferox;
				else if (modelName == "Gobbler")
					objectType = GameObjectInfo::Type::Monster_Gobbler;
				else if (modelName == "LightBipedMech")
					objectType = GameObjectInfo::Type::Monster_LightBipedMech;
				else if (modelName == "MiningMech")
					objectType = GameObjectInfo::Type::Monster_MiningMech;
				else if (modelName == "Rapax")
					objectType = GameObjectInfo::Type::Monster_Rapax;
				else if (modelName == "Aranobot")
					objectType = GameObjectInfo::Type::Monster_Aranobot;
				else
					assert(0);



			}
			else if(tag == "Building" || tag == "Dissolve_Building") {
				objectTag = ObjectTag::Building;
				objectType = GameObjectInfo::Type::Building;
			}
			else {
				objectTag = ObjectTag::None;
				objectType = GameObjectInfo::Type::None;
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
			/// +---------------------------------------------------
			///	¢º Game Monster 
			/// ---------------------------------------------------+
			if (objectType == GameObjectInfo::Type::Monster_Onyscidus ||
				objectType == GameObjectInfo::Type::Monster_Ursacetus ||
				objectType == GameObjectInfo::Type::Monster_AdvancedCombat_5 ||
				objectType == GameObjectInfo::Type::Monster_Anglerox ||
				objectType == GameObjectInfo::Type::Monster_Arack ||
				objectType == GameObjectInfo::Type::Monster_Ceratoferox || 
				objectType == GameObjectInfo::Type::Monster_Gobbler ||
				objectType == GameObjectInfo::Type::Monster_LightBipedMech ||
				objectType == GameObjectInfo::Type::Monster_MiningMech ||
				objectType == GameObjectInfo::Type::Monster_Rapax ||
				objectType == GameObjectInfo::Type::Monster_Aranobot )
			{
				SPtr<GameMonster> object = std::make_shared<GameMonster>();
				object->SetType(objectType);

				if (model) {
					object->SetName(model->mName);
					Matrix transformMatrix = FileIO::ReadVal<Matrix>(file);

					/// +---------------------------------------------------
					///	¡å Component : Tarnsform 
					/// ---------------------------------------------------+
					SPtr<Transform> transform = object->AddComponent<Transform>(ComponentInfo::Type::Transform);
					transform->SetWorldTransform(transformMatrix);
					Vec3 Rot = Quaternion::ToEuler(transform->GetLocalRotation());

					/// +---------------------------------------------------
					///	¡å Component : Collider 
					/// ---------------------------------------------------+
					const auto& collider = object->AddComponent<Collider>(ComponentInfo::Type::Collider);
					collider->SetBS(model->mBS);

					object->SetAnimation(model->mAnimatorController);

					UINT32 ID = static_cast<UINT32>(mEnemies.size() + 1);
					object->SetID(ID);
					object->SetMonsterID(ID);


					/// +---------------------------------------------------
					///	¡å Script  
					/// ---------------------------------------------------+
					object->AddScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
					switch (objectType)
					{
					case GameObjectInfo::Type::Monster_Ursacetus: {
						object->SetMonsterType(FBProtocol::MONSTER_TYPE_URSACETUS);
						object->AddScript<Script_Ursacetus>(ScriptInfo::Type::Ursacetus);

					}
						break;
					case GameObjectInfo::Type::Monster_Onyscidus: {
						object->SetMonsterType(FBProtocol::MONSTER_TYPE_ONYSCIDUS);
						object->AddScript<Script_Onyscidus>(ScriptInfo::Type::Onyscidus);

					}
						break;
					case GameObjectInfo::Type::Monster_AdvancedCombat_5: {
						object->SetMonsterType(FBProtocol::MONSTER_TYPE_ADVANCED_COMBAT_DROIR_5);
						object->AddScript<Script_AdvancedCombatDroid_5>(ScriptInfo::Type::AdvancedCombatDroid_5);

					}
						break;
					case GameObjectInfo::Type::Monster_Anglerox: {
						object->SetMonsterType(FBProtocol::MONSTER_TYPE_ANGLEROX);
						object->AddScript<Script_Anglerox>(ScriptInfo::Type::Anglerox);

					}
						break;
					case GameObjectInfo::Type::Monster_Arack: {
						object->SetMonsterType(FBProtocol::MONSTER_TYPE_ARACK);
						object->AddScript<Script_Arack>(ScriptInfo::Type::Arack);

					}
						break;
					case GameObjectInfo::Type::Monster_Ceratoferox: {
						object->SetMonsterType(FBProtocol::MONSTER_TYPE_CERATOFEROX);
						object->AddScript<Script_Ceratoferox>(ScriptInfo::Type::Ceratoferox);

					}
						break;
					case GameObjectInfo::Type::Monster_Gobbler: {
						object->SetMonsterType(FBProtocol::MONSTER_TYPE_GOBBLER);
						object->AddScript<Script_Gobbler>(ScriptInfo::Type::Gobbler);

					}
						break;
					case GameObjectInfo::Type::Monster_LightBipedMech: {
						object->SetMonsterType(FBProtocol::MONSTER_TYPE_LIGHTBIPEDMECH);
						object->AddScript<Script_LightBipedMech>(ScriptInfo::Type::LightBipedMech);

					}
						break;
					case GameObjectInfo::Type::Monster_MiningMech: {
						object->SetMonsterType(FBProtocol::MONSTER_TYPE_MININGMECH);
						object->AddScript<Script_MiningMech>(ScriptInfo::Type::MiningMech);

					}
						break;
					case GameObjectInfo::Type::Monster_Rapax: {
						object->SetMonsterType(FBProtocol::MONSTER_TYPE_RAPAX);
						object->AddScript<Script_Rapax>(ScriptInfo::Type::Rapax);

					}
						break;
					case GameObjectInfo::Type::Monster_Aranobot: {
						object->SetMonsterType(FBProtocol::MONSTER_TYPE_ARANOBOT);
						object->AddScript<Script_Aranobot>(ScriptInfo::Type::Aranabot);

					}
						break;
					}


					object->AddScript<Script_DefaultEnemyBT>(ScriptInfo::Type::DefaultEnemyBT);
					const auto& pherodropper = object->AddScript<Script_PheroDropper>(ScriptInfo::Type::PheroDropper);
					pherodropper->Init();

					mEnemies.push_back(object);

				}
			}
			else if(objectType == GameObjectInfo::Type::Building){

				SPtr<GameObject> object = std::make_shared<GameObject>();
				object->SetType(objectType);

				if (model) {
					object->SetName(model->mName);
					Matrix transformMatrix = FileIO::ReadVal<Matrix>(file);
					model->mTransform = transformMatrix;

					/// +---------------------------------------------------
					///	¡å Component : Tarnsform 
					/// ---------------------------------------------------+
					SPtr<Transform> transform = object->AddComponent<Transform>(ComponentInfo::Type::Transform);
					transform->SetLocalTransform(transformMatrix);
					transform->SetPosition(Transform::GetPosition(transformMatrix));

					/// +---------------------------------------------------
					///	¡å Component : Collider 
					/// ---------------------------------------------------+
					const auto& collider = object->AddComponent<Collider>(ComponentInfo::Type::Collider);
					collider->SetBS(model->mBS);

					UINT32 ID = static_cast<UINT32>(mBuildings.size());
					object->SetID(ID);

					const auto& BuildingScript = object->AddScript<Script_Building>(ScriptInfo::Type::Building);
					Coordinate SectorIdx       = SectorController::GetSectorIdxByPosition(Transform::GetPosition(transformMatrix));
					mBuildings.push_back(object);

					RESOURCE_MGR->GetTileMap()->UpdateTiles(TileMapInfo::TileType::Static, mBuildings[i].get());

				}
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
	mTileMap = std::make_shared<TileMap>();

	LoadTerrain();
	LoadModels();
	LoadAnimationClips();
	LoadAnimatorControllers();

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

void ResourceManager::LoadAnimationClips()
{
	for (const auto& clipFolder : std::filesystem::directory_iterator(kAnimationClipDataPath)) {
		std::string clipFolderName = clipFolder.path().filename().string();

		for (const auto& file : std::filesystem::directory_iterator(kAnimationClipDataPath + clipFolderName + '/')) {
			std::string fileName = file.path().filename().string();
			sptr<AnimationClip> clip = FileIO::LoadAnimationClip(clipFolder.path().string() + '/' + fileName);

			FileIO::RemoveExtension(fileName);
			const std::string clipName = clipFolderName + '/' + fileName;
			mAnimationClips[clipName] = clip;
		}
	}
}

void ResourceManager::LoadAnimatorControllers()
{
	for (const auto& file : std::filesystem::directory_iterator(kAnimatorControllerDataPath)) {
		const std::string fileName = file.path().filename().string();
		mAnimatorControllers[FileIO::RemoveExtension(fileName)] = FileIO::LoadAnimatorController(kAnimatorControllerDataPath + fileName);
	}
}
