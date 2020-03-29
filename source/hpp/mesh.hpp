#pragma once
#include<essential>
#include<node.hpp>
#include<curve.hpp>
#include<Eigen/Sparse>

namespace mesh
{
    class mesh
    {
    public:
        mesh(size_t = 0, size_t = 0);
        ~mesh();
        void SetBoundingCurve(curve&, curve&, curve&, curve&);
        size_t GetSizeX();
        size_t GetSizeY();
        node** AdjNodes(node*,const size_t&);
        void LaplaceSmoother();
        void TransFiniteInterpolator();
        node** GetMeshData();
        node* AdjNodes();
    private:
        size_t sizeX, sizeY;
        node** meshData;
        std::vector<curve> boundingCurve;
    };
}
