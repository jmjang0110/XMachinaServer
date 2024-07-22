#include "pch.h"
#include "BTTask.h"
#include "Transform.h"
#include "GameObject.h"
#include "BTNode.h"
#include "Script_Player.h"
#include "ResourceManager.h"
#include "Script_AdvancedCombatDroid_5.h"
#include "Script_Onyscidus.h"
#include "Script_Ursacetus.h"
/// +-------------------------------------------------------------------------
///	> ������ Task Path Planning A*  
/// __________________________________________________________________________


MonsterTask::PathPlanning_AStar::PathPlanning_AStar(SPtr_GameObject owner, BTTaskType type, std::function<void()> callback)
	: BTTask(owner, type, callback)

{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);

	if (owner->GetType() == GameObjectInfo::Type::Monster_AdvancedCombat_5)
		mStat = GetOwner()->GetScript<Script_AdvancedCombatDroid_5>(ScriptInfo::Type::AdvancedCombatDroid_5);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Onyscidus)
		mStat = GetOwner()->GetScript<Script_Onyscidus>(ScriptInfo::Type::Onyscidus);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Ursacetus)
		mStat = GetOwner()->GetScript<Script_Ursacetus>(ScriptInfo::Type::Ursacetus);

	MonsterTask::PathPlanning_AStar::mPath = mEnemyController->GetPaths();

}

MonsterTask::PathPlanning_AStar::PathPlanning_AStar(SPtr_GameObject owner, std::function<void()> callback)
	: BTTask(owner, BTTaskType::MonT_PathPlanningASatr, callback)
{
	mEnemyController = GetOwner()->GetScript<Script_EnemyController>(ScriptInfo::Type::EnemyController);

	if (owner->GetType() == GameObjectInfo::Type::Monster_AdvancedCombat_5)
		mStat = GetOwner()->GetScript<Script_AdvancedCombatDroid_5>(ScriptInfo::Type::AdvancedCombatDroid_5);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Onyscidus)
		mStat = GetOwner()->GetScript<Script_Onyscidus>(ScriptInfo::Type::Onyscidus);
	else if (owner->GetType() == GameObjectInfo::Type::Monster_Ursacetus)
		mStat = GetOwner()->GetScript<Script_Ursacetus>(ScriptInfo::Type::Ursacetus);

	MonsterTask::PathPlanning_AStar::mPath = mEnemyController->GetPaths();

}

MonsterTask::PathPlanning_AStar::~PathPlanning_AStar()
{
	mEnemyController = nullptr;

}


BTNodeState MonsterTask::PathPlanning_AStar::Evaluate()
{
	return BTNodeState();
}


bool MonsterTask::PathPlanning_AStar::PathPlanningAStar(Path::Pos start, Path::Pos dest)
{
	// �ʱ� ��ġ Ȥ�� ���� ������ Static�̶�� bfs�� ����� �ֺ� None ���� ȹ��
	if (RESOURCE_MGR->GetTileMap()->GetTileFromUniqueIndex(start) == TileMapInfo::TileType::Static)
		start = FindNoneTileFromBfs(start);
	if (RESOURCE_MGR->GetTileMap()->GetTileFromUniqueIndex(dest) == TileMapInfo::TileType::Static)
		dest = FindNoneTileFromBfs(dest);

	// �� �ʱ�ȭ 
	std::map<Path::Pos, Path::Pos>	mParent;
	std::map<Path::Pos, int>	mDistance;
	std::map<Path::Pos, bool>	mVisited;

	// f = g + h
	int g = 0;
	int h = (abs(dest.Z - start.Z) + abs(dest.X - start.X)) * mkWeight;
	pq.push({ g + h, g, start });
	mDistance[start] = g + h;
	mParent[start] = start;

	// AStar ����
	Path::Pos prevDir;
	while (!pq.empty()) {
		PQNode curNode = pq.top();
		prevDir = curNode.Pos - mParent[curNode.Pos];
		pq.pop();

		// ��ã�� ���� �� ������ ���� ���� ���� �������� ����
		if (mVisited.size() > mkMaxVisited) {
			dest = pq.top().Pos;
		}

		// �湮���� ���� ���鸸 �湮
		if (mVisited.contains(curNode.Pos))
			continue;

		if (mDistance[curNode.Pos] < curNode.F)
			continue;

		mVisited[curNode.Pos] = true;

		// �ش� ������ �������� ��� ����
		if (curNode.Pos == dest)
			break;

		// 8�������� Ž��
		for (int dir = 0; dir < Path::DirCount; ++dir) {
			Path::Pos nextPos = curNode.Pos + Path::gkFront[dir];

			// ���� ��ġ�� Ÿ���� ���� ������ ��� ��� continue
			if (abs(start.X - nextPos.X) > TileMap::mTileRows || abs(start.Z - nextPos.Z) > TileMap::mTileCols)
				continue;

			// ���� ���� ����� ���°� static�̶�� continue
			if (RESOURCE_MGR->GetTileMap()->GetTileFromUniqueIndex(nextPos) == TileMapInfo::TileType::Static)
				continue;

			// �̹� �湮�� ���̸� continue
			if (mVisited.contains(nextPos))
				continue;

			// ���� �Ÿ� ������ ���ٸ� �Ÿ� ����� �ִ����� ����
			if (!mDistance.contains(nextPos))
				mDistance[nextPos] = INT32_MAX;

			// ��� ��� ������ 1 / 2
			int addCost{};
			if (prevDir != Path::gkFront[dir])
				addCost = Path::kCost[0] / 2;

			int g = curNode.G + Path::kCost[dir] + addCost;
			int h = (abs(dest.Z - nextPos.Z) + abs(dest.X - nextPos.X)) * mkWeight;

			if (mDistance[nextPos] <= g + h)
				continue;

			mDistance[nextPos] = g + h;
			pq.push({ g + h, g, nextPos });
			mParent[nextPos] = curNode.Pos;
		}
	}

	Path::Pos pos = dest;
	prevDir = { 0, 0 };

	while (!mEnemyController->GetPaths()->empty())
		mEnemyController->GetPaths()->pop();

	// �θ� ��θ� ���� ���ÿ� �־��ش�. top�� first path�̴�.
	while (true) {
		Path::Pos dir = mParent[pos] - pos;

		if (prevDir != dir) {
			mPath->push(RESOURCE_MGR->GetTileMap()->GetTilePosFromUniqueIndex(pos));
		}

		if (pos == mParent[pos])
			break;

		pos = mParent[pos];
		prevDir = dir;
	}

	// �ڿ������� �������� ���� ù ��° ��δ� ��	��
	if (!mPath->empty()) {
		mPath->pop();
	}

	if (mPath->empty()) {
		return false;
	}

	return true;
}

Path::Pos  MonsterTask::PathPlanning_AStar::FindNoneTileFromBfs(const Path::Pos& pos)
{
	std::queue<Path::Pos> q;
	std::map<Path::Pos, bool> visited;
	q.push(pos);

	Path::Pos curPos{};
	while (!q.empty()) {
		curPos = q.front();
		q.pop();

		if (RESOURCE_MGR->GetTileMap()->GetTileFromUniqueIndex(curPos) == TileMapInfo::TileType::None)
			return curPos;

		if (visited[curPos])
			continue;

		visited[curPos] = true;

		for (int dir = 0; dir < 4; ++dir) {
			Path::Pos nextPos = curPos + Path::gkFront[dir];
			q.push(nextPos);
		}
	}

	return curPos;
}

