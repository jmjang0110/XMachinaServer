#include "pch.h"
#include "BTTaskM_PathPlanning_AStar.h"
#include "BTNode.h"

#include "ResourceManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "Animation.h"
#include "Rigidbody.h"
#include "Collider.h"

#include "Script_Player.h"
#include "Script_Enemy.h"
#include "Script_EnemyController.h"
/// +-------------------------------------------------------------------------
///	> ������ Task Path Planning A*  
/// __________________________________________________________________________


MonsterTask::PathPlanning_AStar::PathPlanning_AStar(SPtr<GameObject> owner, BTTaskType type, std::function<void()> callback)
	: MonsterBTTask(owner, type, callback)

{


	MonsterTask::PathPlanning_AStar::mPaths = mEnemyController->GetPaths();

}

MonsterTask::PathPlanning_AStar::PathPlanning_AStar(SPtr<GameObject> owner, std::function<void()> callback)
	: MonsterBTTask(owner, BTTaskType::MonT_PathPlanningASatr, callback)
{


	MonsterTask::PathPlanning_AStar::mPaths = mEnemyController->GetPaths();

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
			mParent.insert(std::make_pair(nextPos, curNode.Pos));
			//mParent[nextPos] = curNode.Pos;

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
			mPaths->push(RESOURCE_MGR->GetTileMap()->GetTilePosFromUniqueIndex(pos));
		}

		if (pos == mParent[pos])
			break;

		pos = mParent[pos];
		prevDir = dir;
	}

	// �ڿ������� �������� ���� ù ��° ��δ� ��	��
	if (!mPaths->empty()) {
		mPaths->pop();
	}

	if (mPaths->empty()) {
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

