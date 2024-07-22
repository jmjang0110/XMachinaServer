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
///	> ▶▶▶ Task Path Planning A*  
/// __________________________________________________________________________


MonsterTask::PathPlanning_AStar::PathPlanning_AStar(SPtr_GameObject owner, BTTaskType type, std::function<void()> callback)
	: BTTask(owner, type, callback)

{

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
	// 초기 위치 혹은 도착 지점이 Static이라면 bfs를 사용해 주변 None 지점 획득
	if (RESOURCE_MGR->GetTileMap()->GetTileFromUniqueIndex(start) == TileMapInfo::TileType::Static)
		start = FindNoneTileFromBfs(start);
	if (RESOURCE_MGR->GetTileMap()->GetTileFromUniqueIndex(dest) == TileMapInfo::TileType::Static)
		dest = FindNoneTileFromBfs(dest);

	// 값 초기화 
	std::map<Path::Pos, Path::Pos>	mParent;
	std::map<Path::Pos, int>	mDistance;
	std::map<Path::Pos, bool>	mVisited;

	// f = g + h
	int g = 0;
	int h = (abs(dest.Z - start.Z) + abs(dest.X - start.X)) * mkWeight;
	pq.push({ g + h, g, start });
	mDistance[start] = g + h;
	mParent[start] = start;

	// AStar 실행
	Path::Pos prevDir;
	while (!pq.empty()) {
		PQNode curNode = pq.top();
		prevDir = curNode.Pos - mParent[curNode.Pos];
		pq.pop();

		// 길찾기 실패 시 점수가 가장 높은 곳을 도착지로 설정
		if (mVisited.size() > mkMaxVisited) {
			dest = pq.top().Pos;
		}

		// 방문하지 않은 노드들만 방문
		if (mVisited.contains(curNode.Pos))
			continue;

		if (mDistance[curNode.Pos] < curNode.F)
			continue;

		mVisited[curNode.Pos] = true;

		// 해당 지점이 목적지인 경우 종료
		if (curNode.Pos == dest)
			break;

		// 8방향으로 탐색
		for (int dir = 0; dir < Path::DirCount; ++dir) {
			Path::Pos nextPos = curNode.Pos + Path::gkFront[dir];

			// 다음 위치의 타일이 일정 범위를 벗어난 경우 continue
			if (abs(start.X - nextPos.X) > TileMap::mTileRows || abs(start.Z - nextPos.Z) > TileMap::mTileCols)
				continue;

			// 다음 방향 노드의 상태가 static이라면 continue
			if (RESOURCE_MGR->GetTileMap()->GetTileFromUniqueIndex(nextPos) == TileMapInfo::TileType::Static)
				continue;

			// 이미 방문한 곳이면 continue
			if (mVisited.contains(nextPos))
				continue;

			// 현재 거리 정보가 없다면 거리 비용을 최댓값으로 설정
			if (!mDistance.contains(nextPos))
				mDistance[nextPos] = INT32_MAX;

			// 비용 계산 보통의 1 / 2
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

	// 부모 경로를 따라가 스택에 넣어준다. top이 first path이다.
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

	// 자연스러운 움직임을 위해 첫 번째 경로는 삭	제
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

