#pragma once


struct PQNode {
	int			F;
	int			G;
	Path::Pos	Pos;

	bool operator<(const PQNode& rhs) const { return F < rhs.F; }
	bool operator>(const PQNode& rhs) const { return F > rhs.F; }

};

class AStarPath
{
private:
	std::priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode>> pq;

	std::stack<Vec3>* mPaths                = {};
	Path::Pos		  mStart               = {};
	Path::Pos		  mDest                = {};

	int				  mWeight              = 10;
	int				  mMaxVisited          = 2000;

public:
	bool PathPlanningAStar(Path::Pos start, Path::Pos dest);
	Path::Pos FindNoneTileFromBfs(const Path::Pos& pos);
};

