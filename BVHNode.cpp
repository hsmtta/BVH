//
// (c) 2020 Takahiro Hashimoto
//
#include "BVHNode.h"
#include "Observer.h"

template<typename T>
BVHNode<T>::BVHNode()
{
	EmptyAABB(bbox);
}

template<typename T>
BVHNode<T>::~BVHNode()
{
}

template<typename T>
VecVec3 BVHNode<T>::CornerPoints() const
{
	VecVec3 varr(8);
	varr[0] = { bbox(0, 0), bbox(0, 1), bbox(0, 2) };
	varr[1] = { bbox(1, 0), bbox(0, 1), bbox(0, 2) };
	varr[2] = { bbox(0, 0), bbox(1, 1), bbox(0, 2) };
	varr[3] = { bbox(1, 0), bbox(1, 1), bbox(0, 2) };
	varr[4] = { bbox(0, 0), bbox(0, 1), bbox(1, 2) };
	varr[5] = { bbox(1, 0), bbox(0, 1), bbox(1, 2) };
	varr[6] = { bbox(0, 0), bbox(1, 1), bbox(1, 2) };
	varr[7] = { bbox(1, 0), bbox(1, 1), bbox(1, 2) };

	return varr;
}

template<typename T>
void BVHNode<T>::EmptyAABB(Mat23& bbox)
{
	for (int axis = 0; axis < 3; axis++)
	{
		bbox(0, axis) = std::numeric_limits<double>::max();
		bbox(1, axis) = -std::numeric_limits<double>::max();
	}
}

template class BVHNode<Face>;
template class BVHNode<Observer>;