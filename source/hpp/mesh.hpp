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
        mesh();
        ~mesh();
        void Read(const std::string&);
        node** AdjNodes(node*,const size_t&);
        void LaplaceSmoother();
        void TransFiniteInterpolator();
        node** GetMeshData();
        node* AdjNodes();
    private:
        size_t sizeX, sizeY;
        node** meshData;
    };
}
