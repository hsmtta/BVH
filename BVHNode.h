//
// (c) 2020 Takahiro Hashimoto
//
#pragma once

#include "Face.h"

template <typename T>
class BVHNode
{
public:
	BVHNode();
	~BVHNode();

	Mat23 bbox; // bounding box
	Idx2 children; // child nodes

	std::vector<T*> objects; // objects in leaf box

	VecVec3 CornerPoints() const;

	static void EmptyAABB(Mat23& bbox);

private:

};