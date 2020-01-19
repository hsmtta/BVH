//
// (c) 2020 Takahiro Hashimoto
//

#include "BVH.h"
#include "Observer.h"
#include <iostream>
#include <fstream>

template <typename T>
BVH<T>::BVH()
	: T_tri(1.0), T_aabb(1.0)
{
	nodes.resize(1);
}

template <typename T>
BVH<T>::~BVH()
{
}

template <typename T>
void BVH<T>::ConstructBVH(std::vector<T*>& objects)
{
	ConstructBVHInternal(objects, 0);
}

template <typename T>
void BVH<T>::SaveAsVtk(const std::string& fname) const
{
	using namespace std;

	const int n = (int)nodes.size();

	cout << "[Entering] BVH::SaveAsVtk ..." << endl;

	ofstream ofs;
	ofs.open(fname);

	// write header
	ofs << "# vtk DataFile Version 2.0" << endl;
	ofs << "3D model of boudary volume hierarchy" << endl;
	ofs << "ASCII" << endl;
	ofs << "DATASET UNSTRUCTURED_GRID" << endl;
	ofs << endl;
	ofs << "POINTS " << 8 * nodes.size() << " float" << endl;
	for (const BVHNode<T>& node : nodes)
	{
		const VecVec3 varr = node.CornerPoints();
		for (const Vec3& v : varr)
		{
			ofs << " " 
				<< v.x() << " "	
				<< v.y() << " "	
				<< v.z() << endl;
		}
	}

	ofs << "CELLS " << nodes.size() << " " << 9 * nodes.size() << endl;
	for (int i = 0; i < (int)nodes.size(); i++)
	{
		ofs << " 8";
		for (int j = 0; j < 8; j++)
		{
			ofs << " " << 8 * i + j;
		}
		ofs << endl;
	}

	ofs << "CELL_TYPES " << nodes.size() << endl;
	for (int i = 0; i < (int)nodes.size(); i++)
	{
		ofs << " 11" << endl;
	}

	// write properties
	ofs << "CELL_DATA " << (int)nodes.size() << endl;
	ofs << "FIELD FieldData 4" << endl;
	ofs << "BoxID 1 " << (int)nodes.size() << " int" << endl;
	for (int i = 0; i < (int)nodes.size(); i++)
	{
		ofs << " " << i << endl;
	}
	ofs << "Child0 1 " << (int)nodes.size() << " int" << endl;
	for (int i = 0; i < (int)nodes.size(); i++)
	{
		ofs << " " << nodes[i].children[0] << endl;
	}
	ofs << "Child1 1 " << (int)nodes.size() << " int" << endl;
	for (int i = 0; i < (int)nodes.size(); i++)
	{
		ofs << " " << nodes[i].children[1] << endl;
	}
	ofs << "NumFaces 1 " << (int)nodes.size() << " int" << endl;
	for (int i = 0; i < (int)nodes.size(); i++)
	{
		ofs << " " << nodes[i].objects.size() << endl;
	}

	ofs.close();

	cout << "saved " << nodes.size() << " boxes as '" << fname << "'" << endl;
	cout << "[Leaving] BVH::SaveAsVtk" << endl;
}

template <typename T>
void BVH<T>::ConstructBVHInternal(std::vector<T*>& objects, const int nodeIdx)
{
	using namespace std;

	BVHNode<T>& node = nodes[nodeIdx];
	
	CreateAABBfromFaces(objects, node.bbox);

	double bestCost = T_tri * objects.size();

	int bestAxis = -1;
	int bestSplitIndex = -1;
	const double SA_root = SurfaceArea(node.bbox);

	for (int axis = 0; axis < 3; axis++)
	{
		sort(objects.begin(), objects.end(), [axis](const T* a, const T* b) {
			return a->center[axis] < b->center[axis];
			});
	
		// For each separation, caluculate surface areas for the V1 region
		Mat23 v1bbox;
		BVHNode<T>::EmptyAABB(v1bbox);
		vector<double> v1SA(objects.size() + 1, numeric_limits<double>::max());
		v1SA[0] = SurfaceArea(v1bbox);
		for (int i = 0; i < (int)objects.size(); i++) // i is number of polygons in V1
		{
			const T* const f = objects[i];
			MergeAABB(v1bbox, f->bbox, v1bbox); // update bbox from left to right
			v1SA[i] = SurfaceArea(v1bbox);
		}

		// For each separation, caluculate surface areas for the V2 region
		Mat23 v2bbox;
		BVHNode<T>::EmptyAABB(v2bbox);
		vector<double> v2SA(objects.size() + 1, numeric_limits<double>::max());
		v2SA[objects.size()] = SurfaceArea(v2bbox);
		for (int i = (int)objects.size()-1; i >= 0; i--) // i is number of polygons in V1
		{
			const T* const f = objects[i];
			MergeAABB(v2bbox, f->bbox, v2bbox);
			v2SA[i] = SurfaceArea(v2bbox);
		}

		// Calculate optimum cost
		for (int i = 1; i < (int)objects.size()-1; i++)
		{
			const int j = (int)objects.size() - i;
			// SAH-based cost
			const double cost =
				2 * T_aabb + (i * v1SA[i] + j * v2SA[i] ) * T_tri / SA_root;

			if (cost < bestCost)
			{
				// update the best cost
				bestCost = cost;
				bestAxis = axis;
				bestSplitIndex = i;
			}
		}
	}

	if (bestAxis == -1) // Best axis is not updated from the initial value
	{
		// Register the current node as a leaf node
		MakeLeaf(objects, node);
	}
	else
	{
		// Separete region into left and right based on the best axis
		sort(objects.begin(), objects.end(), [bestAxis](const T* a, const T* b) {
			return a->center[bestAxis] < b->center[bestAxis];
			});

		// Create child nodes
		nodes[nodeIdx].children[0] = (int)nodes.size();
		nodes.push_back(BVHNode<T>());
		nodes[nodeIdx].children[1] = (int)nodes.size();
		nodes.push_back(BVHNode<T>());

		vector<T*> left(objects.begin(), objects.begin() + bestSplitIndex);
		vector<T*> right(objects.begin() + bestSplitIndex, objects.end());

		ConstructBVHInternal(left, nodes[nodeIdx].children[0]);
		ConstructBVHInternal(right, nodes[nodeIdx].children[1]);
	}
}

template <typename T>
void BVH<T>::CreateAABBfromFaces(const std::vector<T*>& objects, Mat23& bbox)
{
	for (const T* obj : objects)
	{
		MergeAABB(obj->bbox, bbox, bbox);
	}
}

template <typename T>
void BVH<T>::MergeAABB(const Mat23& bbox1, const Mat23& bbox2, Mat23& result)
{
	for (int i = 0; i < 3; i++)
	{
		result(0, i) = std::min(bbox1(0, i), bbox2(0, i));
		result(1, i) = std::max(bbox1(1, i), bbox2(1, i));
	}
}

template <typename T>
double BVH<T>::SurfaceArea(const Mat23& bbox)
{
	if (bbox(1, 0) <= bbox(0, 0) ||
		bbox(1, 1) <= bbox(0, 1) ||
		bbox(1, 2) <= bbox(0, 2))
	{
		return 0.0;
	}
	else // dx, dy, dz > 0
	{
		const double dx = bbox(1, 0) - bbox(0, 0);
		const double dy = bbox(1, 1) - bbox(0, 1);
		const double dz = bbox(1, 2) - bbox(0, 2);
		return 2.0 * (dx * dy + dy * dz + dz * dx);
	}
}

template <typename T>
void BVH<T>::MakeLeaf(const std::vector<T*>& objects, BVHNode<T>& node)
{
	node.children[0] = node.children[1] = -1;
	node.objects = std::vector<T*>(objects.begin(), objects.end());
}

template class BVH<Face>;
template class BVH<Observer>;