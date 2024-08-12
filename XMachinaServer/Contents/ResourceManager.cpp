#include "pch.h"
#include "ResourceManager.h"

#include "FileIO.h"
#include "GameObject.h"
#include "GameItem.h"
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
#include "Script_Deus_Phase_1.h"
#include "Script_Deus_Phase_2.h"

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
				else if (modelName == "Deus_Phase_1")
					objectType = GameObjectInfo::Type::Montser_Deus_Phase_1;
				else if (modelName == "Deus_Phase_2")
					objectType = GameObjectInfo::Type::Monster_Deus_Phase_2;

				else
					assert(0);
			}
			else if(tag == "Building" || tag == "Dissolve_Building") {
				objectTag = ObjectTag::Building;
				objectType = GameObjectInfo::Type::Building;
			}
			else if (tag == "Crate") {
				objectTag = ObjectTag::Item;
				objectType = GameObjectInfo::Type::Crate;
			}
			else {
				objectTag = ObjectTag::None;
				objectType = GameObjectInfo::Type::None;
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
			assert(model);

			bool isGameMonster{};
			switch (objectType) {
				case GameObjectInfo::Type::Monster_Onyscidus:
				case GameObjectInfo::Type::Monster_Ursacetus:
				case GameObjectInfo::Type::Monster_AdvancedCombat_5:
				case GameObjectInfo::Type::Monster_Anglerox:
				case GameObjectInfo::Type::Monster_Arack:
				case GameObjectInfo::Type::Monster_Ceratoferox:
				case GameObjectInfo::Type::Monster_Gobbler:
				case GameObjectInfo::Type::Monster_LightBipedMech:
				case GameObjectInfo::Type::Monster_MiningMech:
				case GameObjectInfo::Type::Monster_Rapax:
				case GameObjectInfo::Type::Monster_Aranobot:
				case GameObjectInfo::Type::Montser_Deus_Phase_1:
				case GameObjectInfo::Type::Monster_Deus_Phase_2:
					
					isGameMonster = true;
					break;
			}

			SPtr<GameObject> object;
			if (isGameMonster) {
				object = std::make_shared<GameMonster>();
				object->SetAnimation(model->mAnimatorController);
			}
			else {
				object = std::make_shared<GameObject>();
			}

			object->SetType(objectType);
			object->SetName(model->mName);

			/// +---------------------------------------------------
			///	¡å Component : Tarnsform 
			/// ---------------------------------------------------+
			{
				FileIO::ReadString(file, token);	// <Transform>:
				Matrix transformMatrix = FileIO::ReadVal<Matrix>(file);
				model->mTransform = transformMatrix;

				FileIO::ReadString(file, token);	// </Transform>: or <ScriptExporter>:
				if (Hash(token) == Hash("<ScriptExporter>:"))
				{
					LoadScriptExporter(file, object);
					FileIO::ReadString(file); // </Transform>:
				}
					
				SPtr<Transform> transform = object->AddComponent<Transform>(ComponentInfo::Type::Transform);
				transform->SetLocalTransform(transformMatrix);
				transform->SetPosition(Transform::GetPosition(transformMatrix));
			}


			/// +---------------------------------------------------
			///	¡å Component : Collider 
			/// ---------------------------------------------------+
			const auto& collider = object->AddComponent<Collider>(ComponentInfo::Type::Collider);
			collider->SetBS(model->mBS);

			/// +---------------------------------------------------
			///	¢º Game Monster 
			/// ---------------------------------------------------+
			if (isGameMonster)
			{
				AddMonster(object);
			}
			else if(objectType == GameObjectInfo::Type::Building){
				AddBuilding(object);
			}

			--sameObjectCnt;
		}
	}

	UpdateTiles();
}

void BattleScene::AddMonster(SPtr<GameObject> object)
{
	sptr<GameMonster> monsterObject = std::static_pointer_cast<GameMonster>(object);
	UINT32 ID = static_cast<UINT32>(mEnemies.size() + 1);
	monsterObject->SetID(ID);
	monsterObject->SetMonsterID(ID);

	/// +---------------------------------------------------
	///	¡å Script  
	/// ---------------------------------------------------+
	monsterObject->AddScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);
	switch (object->GetType())
	{
	case GameObjectInfo::Type::Monster_Ursacetus: {
		monsterObject->SetMonsterType(FBProtocol::MONSTER_TYPE_URSACETUS);
		monsterObject->AddScript<Script_Ursacetus>(ScriptInfo::Type::Stat);

	}
	break;
	case GameObjectInfo::Type::Monster_Onyscidus: {
		monsterObject->SetMonsterType(FBProtocol::MONSTER_TYPE_ONYSCIDUS);
		monsterObject->AddScript<Script_Onyscidus>(ScriptInfo::Type::Stat);

	}
	break;
	case GameObjectInfo::Type::Monster_AdvancedCombat_5: {
		monsterObject->SetMonsterType(FBProtocol::MONSTER_TYPE_ADVANCED_COMBAT_DROIR_5);
		monsterObject->AddScript<Script_AdvancedCombatDroid_5>(ScriptInfo::Type::Stat);

	}
	break;
	case GameObjectInfo::Type::Monster_Anglerox: {
		monsterObject->SetMonsterType(FBProtocol::MONSTER_TYPE_ANGLEROX);
		monsterObject->AddScript<Script_Anglerox>(ScriptInfo::Type::Stat);

	}
	break;
	case GameObjectInfo::Type::Monster_Arack: {
		monsterObject->SetMonsterType(FBProtocol::MONSTER_TYPE_ARACK);
		monsterObject->AddScript<Script_Arack>(ScriptInfo::Type::Stat);

	}
	break;
	case GameObjectInfo::Type::Monster_Ceratoferox: {
		monsterObject->SetMonsterType(FBProtocol::MONSTER_TYPE_CERATOFEROX);
		monsterObject->AddScript<Script_Ceratoferox>(ScriptInfo::Type::Stat);

	}
	break;
	case GameObjectInfo::Type::Monster_Gobbler: {
		monsterObject->SetMonsterType(FBProtocol::MONSTER_TYPE_GOBBLER);
		monsterObject->AddScript<Script_Gobbler>(ScriptInfo::Type::Stat);

	}
	break;
	case GameObjectInfo::Type::Monster_LightBipedMech: {
		monsterObject->SetMonsterType(FBProtocol::MONSTER_TYPE_LIGHTBIPEDMECH);
		monsterObject->AddScript<Script_LightBipedMech>(ScriptInfo::Type::Stat);

	}
	break;
	case GameObjectInfo::Type::Monster_MiningMech: {
		monsterObject->SetMonsterType(FBProtocol::MONSTER_TYPE_MININGMECH);
		monsterObject->AddScript<Script_MiningMech>(ScriptInfo::Type::Stat);

	}
	break;
	case GameObjectInfo::Type::Monster_Rapax: {
		monsterObject->SetMonsterType(FBProtocol::MONSTER_TYPE_RAPAX);
		monsterObject->AddScript<Script_Rapax>(ScriptInfo::Type::Stat);

	}
	break;
	case GameObjectInfo::Type::Monster_Aranobot: {
		monsterObject->SetMonsterType(FBProtocol::MONSTER_TYPE_ARANOBOT);
		monsterObject->AddScript<Script_Aranobot>(ScriptInfo::Type::Stat);

	}
	break;
	case GameObjectInfo::Type::Montser_Deus_Phase_1: {
		monsterObject->SetMonsterType(FBProtocol::MONSTER_TYPE_DEUS_PHASE_1);
		monsterObject->AddScript<Script_Deus_Phase_1>(ScriptInfo::Type::Stat);

	}
	break;
	case GameObjectInfo::Type::Monster_Deus_Phase_2: {
		monsterObject->SetMonsterType(FBProtocol::MONSTER_TYPE_DEUS_PHASE_2);
		monsterObject->AddScript<Script_Deus_Phase_2>(ScriptInfo::Type::Stat);

	}
	break;


	default:
		assert(0);
		break;
	}

	object->AddScript<Script_DefaultEnemyBT>(ScriptInfo::Type::DefaultEnemyBT);
	const auto& pherodropper = object->AddScript<Script_PheroDropper>(ScriptInfo::Type::PheroDropper);
	pherodropper->Init();

	mEnemies.push_back(monsterObject);
}

void BattleScene::AddBuilding(SPtr<GameObject> object)
{
	UINT32 ID = static_cast<UINT32>(mBuildings.size());
	object->SetID(ID);

	const auto& BuildingScript = object->AddScript<Script_Building>(ScriptInfo::Type::Building);
	Coordinate SectorIdx = SectorController::GetSectorIdxByPosition(object->GetTransform()->GetPosition());
	mBuildings.push_back(object);
}

void BattleScene::UpdateTiles() const
{
	for (const auto& building : mBuildings) {
		RESOURCE_MGR->GetTileMap()->UpdateTiles(TileMapInfo::TileType::Static, building.get());
	}
}

void BattleScene::LoadScriptExporter(std::ifstream& file, SPtr<GameObject> object)
{
	ScriptExporter exporter;
	exporter.Load(file);

	switch (Hash(exporter.GetName())) {
	case Hash("WeaponCrate"):
	{
		std::string weaponName{};
		int id;
		exporter.GetData("ID", id);
		exporter.GetData("Name", weaponName);

		object->SetID(id);

		SPtr<GameItem> crateObject = std::static_pointer_cast<GameItem>(object);
		SPtr<GameItem> weaponObject = std::make_shared<GameItem>();
		weaponObject->SetID(id + 1);
		crateObject->SetChildItem(weaponObject.get());

		mItems.push_back(crateObject);
		mItems.push_back(weaponObject);
	}
		break;
	default:
		assert(0);
		break;
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

void ScriptExporter::Load(std::ifstream& file)
{
	std::string token{};

	FileIO::ReadString(file, token);	// <ScriptName>:
	FileIO::ReadString(file, mName);

	FileIO::ReadString(file, token);	// <ScriptInfo>:
	int cnt = FileIO::ReadVal<int>(file);

	for (int i = 0; i < cnt; ++i) {
		std::string key = FileIO::ReadString(file);
		char type = key.back();
		key.pop_back();

		ScriptParameter param;
		switch (type) {
		case 'i':
			param.Type = ScriptParameter::Type::Int;
			FileIO::ReadVal(file, param.Val.i);
			break;
		case 'f':
			param.Type = ScriptParameter::Type::Float;
			FileIO::ReadVal(file, param.Val.f);
			break;
		case 'b':
			param.Type = ScriptParameter::Type::Bool;
			FileIO::ReadVal(file, param.Val.b);
			break;
		case 's':
			param.Type = ScriptParameter::Type::String;
			FileIO::ReadString(file, param.Str);
			break;
		default:
			assert(0);
			break;
		}

		mData[key] = param;
	}
}
