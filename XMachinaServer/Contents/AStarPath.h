#pragma once


struct PQNode {
	bool operator<(const PQNode& rhs) const { return F < rhs.F; }
	bool operator>(const PQNode& rhs) const { return F > rhs.F; }

	int			F;
	int			G;
	Path::Pos	Pos;
};
class AStarPath
{
private:
	std::priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode>> pq;

	std::stack<Vec3>* mPath;
	Path::Pos		  mStart;
	Path::Pos		  mDest;


public:

};

