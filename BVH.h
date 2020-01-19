//
// (c) 2020 Takahiro Hashimoto
//
#pragma once

#include "BVHNode.h"
#include <list>

template <typename T>
class BVH
{
public:
	BVH();
	~BVH();

	const double T_tri;
	const double T_aabb;

	std::vector<BVHNode<T>> nodes;

	void ConstructBVH(std::vector<T*>& objects);
	void SaveAsVtk(const std::string& fname) const; // for visualization

private:
	void ConstructBVHInternal(std::vector<T*>& objects, const int nodeIdx);

	static void CreateAABBfromFaces(const std::vector<T*>& objects, Mat23& bbox);
	static void MergeAABB(const Mat23& bbox1, const Mat23& bbox2, Mat23& result);
	static double SurfaceArea(const Mat23& bbox);
	static void MakeLeaf(const std::vector<T*>& objects, BVHNode<T>& node);
};