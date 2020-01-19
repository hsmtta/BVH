# BVH

The code generate bounding volume hierarchy (BVH) of arbitrary class T, which is assigned by the template argument.

# Sample code

    void main()
    {
        // Class "Object" must have member variables: 
        // "Mat23 bbox" as a bounding box, and "Vec3 center" as a gravity center of bounding box
        std::vector<Object*> objects;

        BVH<Object> bvh;
        bvh.ConstructBVH(objects);
        bvh.SaveAsVtk("BVH.vtk"); // The result is saved as VTK (ParaView) format
    }
    
# Reference

The code is based on the article (in Japanese): https://qiita.com/omochi64/items/9336f57118ba918f82ec
