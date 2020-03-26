#include<mesh.hpp>

mesh::mesh::mesh(): sizeX(0), sizeY(0), meshData(nullptr)
{
}

mesh::mesh::~mesh()
{
    if(meshData != nullptr)
    {
        for(size_t i=0; i<sizeX; ++i)
        {
            if(meshData[i] != nullptr) delete[] meshData[i];
        }
        delete meshData;
    }
}

void mesh::mesh::Read(const std::string& fileName)
{
    std::ifstream fileIn(fileName.c_str());
    if (!fileIn.is_open()) std::cout << "not open" << std::endl;

    double coord[3]{ 0,0,0 };

    fileIn >> sizeX >> sizeY;

    meshData = new node*[sizeY];

    for(size_t j=0; j<sizeY; ++j)
    {
        meshData[j] = new node[sizeX];
        for(size_t i=0; i<sizeX; ++i)
        {
            fileIn >> coord[0] >> coord[1] >> coord[2];
            meshData[j][i].SetX(coord[0]);
            meshData[j][i].SetY(coord[1]);
            meshData[j][i].SetZ(coord[2]);
            meshData[j][i].SetI(i);
            meshData[j][i].SetJ(j);
        }
    }
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
    size_t size = (sizeX-2)*(sizeY-2); // NUMBER OF UNKNOWN
    Eigen::SparseMatrix<double,Eigen::RowMajor> A(static_cast<int>(size),static_cast<int>(size));

    for(size_t j=1; j<(sizeY-1); ++j)
    {
        for(size_t i=1; i<(sizeX-1); ++i)
        {
            int index = static_cast<int>((j-1)*(sizeX-2)+(i-1));

            A.coeffRef(index,index) = -4.0;
            if(i!=1)         A.coeffRef(index,index-1) = 1.0;
            if(i!=(sizeX-2)) A.coeffRef(index,index+1) = 1.0;
            if(j!=1)         A.coeffRef(index,index-static_cast<int>(sizeX-2)) = 1.0;
            if(j!=(sizeY-2)) A.coeffRef(index,index+static_cast<int>(sizeX-2)) = 1.0;
        }
    }

    Eigen::VectorXd bx(size), by(size), bz(size);
    bx.setZero();
    by.setZero();
    bz.setZero();

    for(size_t j=1; j<(sizeY-1); ++j)
    {
        for(size_t i=1; i<(sizeX-1); ++i)
        {
            int index = static_cast<int>((j-1)*(sizeX-2)+(i-1));
            if(i==1)
            {
                bx(index) -= meshData[j][i-1].GetX();
                by(index) -= meshData[j][i-1].GetY();
                bz(index) -= meshData[j][i-1].GetZ();
            }
            if(i==(sizeX-2))
            {
                bx(index) -= meshData[j][i+1].GetX();
                by(index) -= meshData[j][i+1].GetY();
                bz(index) -= meshData[j][i+1].GetZ();
            }
            if(j==1)
            {
                bx(index) -= meshData[j-1][i].GetX();
                by(index) -= meshData[j-1][i].GetY();
                bz(index) -= meshData[j-1][i].GetZ();
            }
            if(j==(sizeY-2))
            {
                bx(index) -= meshData[j+1][i].GetX();
                by(index) -= meshData[j+1][i].GetY();
                bz(index) -= meshData[j+1][i].GetZ();
            }
        }
    }

    Eigen::BiCGSTAB<Eigen::SparseMatrix<double,Eigen::RowMajor>, Eigen::IncompleteLUT<double> > solver;
    solver.setTolerance(1e-10);
    solver.setMaxIterations(10000);
    solver.compute(A);
    Eigen::VectorXd x = solver.solve(bx);
    Eigen::VectorXd y = solver.solve(by);
    Eigen::VectorXd z = solver.solve(bz);

    for(size_t j=1; j<(sizeY-1); ++j)
    {
        for(size_t i=1; i<(sizeX-1); ++i)
        {
            int index = static_cast<int>((j-1)*(sizeX-2)+(i-1));

            auto xVal=x[index];
            auto yVal=y[index];
            auto zVal=z[index];

            meshData[j][i].SetX(xVal);
            meshData[j][i].SetY(yVal);
            meshData[j][i].SetZ(zVal);
        }
    }
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
