#include<mesh.hpp>

mesh::mesh::mesh(size_t sizeX, size_t sizeY): sizeX(sizeX), sizeY(sizeY), meshData(nullptr)
{
    meshData = new node*[sizeY];

    for(size_t j=0; j<sizeY; ++j)
    {
        meshData[j] = new node[sizeX];
        for(size_t i=0; i<sizeX; ++i)
        {
            meshData[j][i].SetX(0);
            meshData[j][i].SetY(0);
            meshData[j][i].SetZ(0);
            meshData[j][i].SetI(i);
            meshData[j][i].SetJ(j);
        }
    }
}

mesh::mesh::~mesh()
{
    if(meshData != nullptr)
    {
        for(size_t j=0; j<sizeY; ++j)
        {
            if(meshData[j] != nullptr) delete[] meshData[j];
        }
        delete meshData;
    }
}

void mesh::mesh::SetBoundingCurve(curve& c1,curve& c2,curve& c3,curve& c4)
{
    c1.Parameterize();
    c2.Parameterize();
    c3.Parameterize();
    c4.Parameterize();

    if(c1.Intersect(c2) && c1.Intersect(c4) && c3.Intersect(c2) && c1.Intersect(c4))
    {
        double dx = 1.0/(sizeX-1);
        double dy = 1.0/(sizeY-1);
        for(size_t i=0; i<sizeX; ++i)
        {
            meshData[0][i] = c1.PointAt(i*dx);
            meshData[sizeY-1][i] = c3.PointAt(i*dx);
        }
        for(size_t j=0; j<sizeY; ++j)
        {
            meshData[j][0] = c2.PointAt(j*dy);
            meshData[j][sizeX-1] = c4.PointAt(j*dy);
        }
    }
}
size_t mesh::mesh::GetSizeX()
{
    return sizeX;
}

size_t mesh::mesh::GetSizeY()
{
    return sizeY;
}

mesh::node** mesh::mesh::AdjNodes(node* n,const size_t& size)
{
    node** adjNode = new node*[size];
    for(size_t j=0; j<size; ++j)
    {
        adjNode[j] = new node[size];
    }

    int iBegin = static_cast<int>(n->GetI()-size/2);
    int jBegin = static_cast<int>(n->GetJ()-size/2);

    for(size_t j=0; j<size; ++j)
    {
        for(size_t i=0; i<size; ++i)
        {
            adjNode[j][i] = meshData[static_cast<size_t>(jBegin)+j][static_cast<size_t>(iBegin)+i];
        }
    }

    return adjNode;
}

void mesh::mesh::LaplaceSmoother()
{
    double tolerance = 10e-10;
    double error = 10*tolerance;
    size_t it = 0;
    while (error>tolerance)
    {
		//Gauss Seidel with over relaxation
        error = 0;
        for(size_t j=1; j<(sizeY-1); ++j)
        {
            for(size_t i=1; i<(sizeX-1); ++i)
            {
                auto RelaxationFactor = 1.96;

                auto tempX = meshData[j][i].GetX();
                auto tempY = meshData[j][i].GetY();
                auto tempZ = meshData[j][i].GetZ();

                auto incrX = 0.25*(meshData[j][i+1].GetX()+meshData[j][i-1].GetX()+meshData[j+1][i].GetX()+meshData[j-1][i].GetX()) - tempX;
                auto incrY = 0.25*(meshData[j][i+1].GetY()+meshData[j][i-1].GetY()+meshData[j+1][i].GetY()+meshData[j-1][i].GetY()) - tempY;
                auto incrZ = 0.25*(meshData[j][i+1].GetZ()+meshData[j][i-1].GetZ()+meshData[j+1][i].GetZ()+meshData[j-1][i].GetZ()) - tempZ;
                
				meshData[j][i].SetX(tempX+RelaxationFactor*incrX);
                meshData[j][i].SetY(tempY+RelaxationFactor*incrY);
                meshData[j][i].SetZ(tempZ+RelaxationFactor*incrZ);

				//I'm calculating absolute error. Relative error calculation is better option
                error += (tempX - meshData[j][i].GetX())*(tempX - meshData[j][i].GetX());
                error += (tempY - meshData[j][i].GetY())*(tempY - meshData[j][i].GetY());
                error += (tempZ - meshData[j][i].GetZ())*(tempZ - meshData[j][i].GetZ());
            }
        }
        error = sqrt(error);
        ++it;

        if(it%1000 == 0) std::cout << "Iteration number: " << it << "; Error: " << error <<std::endl;
    }
    std::cout << "Iteration number: " << it << "; Error: " << error <<std::endl;

}

void mesh::mesh::TransFiniteInterpolator()
{
    curve c1, c2, c3, c4;
    for(size_t i=0; i<sizeX; ++i)
    {
        point p1 = meshData[0][i];
        point p3 = meshData[sizeY-1][i];
        c1.InsertPoint(p1);
        c3.InsertPoint(p3);
    }

    for(size_t j=0; j<sizeY; ++j)
    {
        point p2 = meshData[j][0];
        point p4 = meshData[j][sizeX-1];
        c2.InsertPoint(p2);
        c4.InsertPoint(p4);
    }

    c1.Parameterize();
    c2.Parameterize();
    c3.Parameterize();
    c4.Parameterize();

    point p12 = meshData[0][0];
    point p32 = meshData[sizeY-1][0];
    point p34 = meshData[sizeY-1][sizeX-1];
    point p14 = meshData[0][sizeX-1];

    double dx = 1.0/(sizeX-1);
    double dy = 1.0/(sizeY-1);

    for(size_t j=0; j<sizeY; ++j)
    {
        for(size_t i=0; i<sizeX; ++i)
        {
            double u = c1.GetParameterAt(i)*(1-(dy*j)) + c3.GetParameterAt(i)*(dy*j);
            double v = c2.GetParameterAt(j)*(1-(dx*i)) + c4.GetParameterAt(j)*(dx*i);

            meshData[j][i] = (c1.PointAt(u) * (1 - v)) + (c3.PointAt(u) * v) + (c2.PointAt(v) * (1 - u)) + (c4.PointAt(v) * u)
                           - ((p12 * ((1 - u) * (1 - v))) + (p34 * (u * v)) + (p14 * (u * (1 - v))) + (p32 * ((1 - u) * v)));

        }
     }
}

mesh::node** mesh::mesh::GetMeshData()
{
    return meshData;
}
