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
#include "Script_PheroDropper.h"
#include "Script_BehaviorTrees.h"

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

/* Item */
#include "Script_Crate.h"
#include "Script_WeaponBurnout.h"
#include "Script_WeaponDBMS.h"
#include "Script_WeaponMineLauncher.h"
#include "Script_WeaponPipeLine.h"
#include "Script_WeaponSkyLine.h"
namespace {
	const std::string kTerrainDataPath            = "Contents/Resource/Terrain.bin";
	const std::string kModelDataPath              = "Contents/Resource/Models/";
	const std::string kSceneDataPath              = "Contents/Resource/ServerScene.bin";
	const std::string kAnimationClipDataPath      = "Contents/Resource/AnimationClips/";
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

	for (int i = 0; i < objectCnt; ++i) {
		std::string modelName = {};

		if (sameObjectCnt <= 0) {
			/// -------------------- 1. <Tag> --------------------
			FileIO::ReadString(file, token); // <Tag>:
			std::string tag = FileIO::ReadString(file);
			objectTag = Load_SettingObjectTag(tag);

			/// -------------------- 2. <FileName> --------------------
			FileIO::ReadString(file, token); // <FileName>:
			modelName = FileIO::ReadString(file);
			model = RESOURCE_MGR->GetModel(modelName);

			/// -------------------- 3. <Transforms> --------------------
			FileIO::ReadString(file, token); // <Transforms>:
			sameObjectCnt = FileIO::ReadVal<int>(file);
		}

		if (sameObjectCnt > 0) {
			/// -------------------- 4. <GameObject> --------------------
			SPtr<GameObject> object = std::make_shared<GameObject>();
			object->SetName(model->mName);
			if (objectTag == ObjectTag::Enemy) {
				SPtr<Animation> animation = object->AddComponent<Animation>(Component::Type::Animation);
				animation->Load(model->mAnimatorController);
			}

			/// +---------------------------------------------------
			///	¡å Component : Tarnsform 
			/// ---------------------------------------------------+
			{
				FileIO::ReadString(file, token);	// <Transform>:
				Matrix transformMatrix = FileIO::ReadVal<Matrix>(file);
				model->mTransform      = transformMatrix;

				SPtr<Transform> transform = object->AddComponent<Transform>(Component::Type::Transform);
				transform->SetLocalTransform(transformMatrix);
				transform->SetPosition(Transform::GetPosition(transformMatrix));

				FileIO::ReadString(file, token);	// </Transform>: or <ScriptExporter>:
				if (Hash(token) == Hash("<ScriptExporter>:"))
				{
					LoadScriptExporter(file, object);
					FileIO::ReadString(file); // </Transform>:
				}
			}

			if (object->GetComponent<Collider>(Component::Type::Collider)) {
				AddBuilding(object);
			}
			else {
				const auto& collider = object->AddComponent<Collider>(Component::Type::Collider);
				collider->SetBS(model->mBS);
				if (objectTag == ObjectTag::Enemy)
				{
					AddMonster(object, model->mName);
				}
				else if(objectTag == ObjectTag::Building) {
					collider->SetBoundingBoxList(model->mBoxList);
					AddBuilding(object);
				}
			}

			--sameObjectCnt;
		}
	}

	UpdateTiles();
}

ObjectTag BattleScene::Load_SettingObjectTag(std::string loadname)
{
	ObjectTag tag = ObjectTag::Untagged;
	if (loadname == "Enemy") {
		tag = ObjectTag::Enemy;
		
	}
	else if (loadname == "Building" || loadname == "Dissolve_Building" || loadname == "Bound") {
		tag = ObjectTag::Building;
	}
	else if (loadname == "Crate") {
		tag = ObjectTag::Crate;
	}
	else {
		assert(0);
	}
	return tag;
}

void BattleScene::AddMonster(SPtr<GameObject> object, std::string modelName)
{
	UINT32 ID = static_cast<UINT32>(mEnemies.size() + 1);
	object->SetID(ID);

	/// +---------------------------------------------------
	///	¡å Script  
	/// ---------------------------------------------------+
	object->AddScript<Script_EnemyController>();
	object->AddScript<Script_PheroDropper>();

	if (modelName == "Onyscidus") {
		object->SetScriptEntity<Script_Onyscidus>();
		object->AddScript<Script_MindControlledEnemyBT>();

	}
	else if (modelName == "Ursacetus") {
		object->SetScriptEntity<Script_Ursacetus>();
		object->AddScript<Script_MindControlledEnemyBT>();
	}
	else if (modelName == "AdvancedCombatDroid") {
		object->SetScriptEntity<Script_AdvancedCombatDroid_5>();
		object->AddScript<Script_MindControlledEnemyBT>();
	}
	else if (modelName == "Anglerox") {
		object->SetScriptEntity<Script_Anglerox>();
		object->AddScript<Script_MindControlledEnemyBT>();
	}
	else if (modelName == "Arack") {
		object->SetScriptEntity<Script_Arack>();
		object->AddScript<Script_MindControlledEnemyBT>();
	}
	else if (modelName == "Ceratoferox") {
		object->SetScriptEntity<Script_Ceratoferox>();
		object->AddScript<Script_MindControlledEnemyBT>();
	}
	else if (modelName == "Gobbler") {
		object->SetScriptEntity<Script_Gobbler>();
		object->AddScript<Script_MindControlledEnemyBT>();
	}
	else if (modelName == "LightBipedMech") {
		object->SetScriptEntity<Script_LightBipedMech>();
		object->AddScript<Script_MindControlledEnemyBT>();
	}
	else if (modelName == "MiningMech") {
		object->SetScriptEntity<Script_MiningMech>();
		object->AddScript<Script_MindControlledEnemyBT>();
	}
	else if (modelName == "Rapax") {
		object->SetScriptEntity<Script_Rapax>();
		object->AddScript<Script_MindControlledEnemyBT>();
	}
	else if (modelName == "Aranobot") {
		object->SetScriptEntity<Script_Aranobot>();
		object->AddScript<Script_MindControlledEnemyBT>();
	}
	else if (modelName == "Deus_Phase_1") {
		object->SetScriptEntity<Script_Deus_Phase_1>();
		object->AddScript<Script_DeusPhase1BT>();
	}
	else if (modelName == "Deus_Phase_2") {
		object->SetScriptEntity<Script_Deus_Phase_2>();
		object->AddScript<Script_DeusPhase1BT>();

	}
	else
		assert(0);

	mEnemies.push_back(object);
}

void BattleScene::AddBuilding(SPtr<GameObject> object)
{
	UINT32 ID = static_cast<UINT32>(mBuildings.size());
	object->SetID(ID);

	const auto& BuildingScript = object->SetScriptEntity<Script_Building>();
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
		SPtr<GameObject> crate = object;
		std::string weaponName{};
		int id;
		exporter.GetData("ID", id);
		exporter.GetData("Name", weaponName);
		LOG_MGR->Cout(weaponName, '\n');
			
		/// > ------------------ Crate ------------------
		crate->SetID(id);
		crate->SetName(weaponName);
		auto crate_entity = crate->SetScriptEntity<Script_Crate>();
		
		/// > ------------------ Weapon ------------------
		SPtr<GameObject> weapon = std::make_shared<GameObject>();
		weapon->SetID(id + 1);
		
		weapon->AddComponent<Transform>(Component::Type::Transform);
		weapon->GetTransform()->SetLocalTransform(crate->GetTransform()->GetLocalTransform());
		weapon->GetTransform()->SetPosition(crate->GetTransform()->GetPosition());

		SetScriptEntityByName(weapon, weaponName);

		crate_entity->SetItem(weapon); // Crate <----- Incrate ---- Weapon 

		mStaticItems.push_back(crate);
		mDynamicItems.push_back(weapon);
	}
		break;
	case Hash("Bound"):
	{
		const auto& collider = object->AddComponent<Collider>(Component::Type::Collider);

		// sphere
		{
			MyBoundingSphere bs;
			exporter.GetData("rad", bs.Radius);
			collider->SetBS(bs);
		}

		// box list
		{
			int boundingCnt{};
			exporter.GetData("Size", boundingCnt);
			std::vector<MyBoundingOrientedBox> boxList{};
			for (int i = 0; i < boundingCnt; ++i) {
				const std::string& idx = std::to_string(i);
				MyBoundingOrientedBox box;
				exporter.GetData(idx + "x", box.Extents.x);
				exporter.GetData(idx + "y", box.Extents.y);
				exporter.GetData(idx + "z", box.Extents.z);
				boxList.push_back(box);
			}
			collider->SetBoundingBoxList(boxList);
		}
		collider->UpdateTransform();
		collider->UpdateColliderSnapShot();
	}
		break;
	default:
		assert(0);
		break;
	}
}

void BattleScene::SetScriptEntityByName(SPtr<GameObject> object, std::string itemName)
{
	if (itemName == "SkyLine") {
		object->SetScriptEntity<Script_WeaponSkyLine>();
	}
	else if (itemName == "DBMS") {
		object->SetScriptEntity<Script_WeaponDBMS>();
	}
	else if (itemName == "PipeLine") {
		object->SetScriptEntity<Script_WeaponPipeLine>();
	}
	else if (itemName == "Burnout") {
		object->SetScriptEntity<Script_WeaponBurnout>();
	}
	else if (itemName == "MineLauncher") {
		object->SetScriptEntity<Script_WeaponMineLauncher>();
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
