#include<curve.hpp>
#include<mesh.hpp>
int main()//int argc, char const *argv[]
{
    mesh::curve c;
    c.Read("../input/test1.dat");
    c.Parameterize();
    auto k = c.PointAt(.65);
    mesh::mesh m;
    m.Read("../input/test3.dat");
    m.LaplaceSmoother();

    m.TransFiniteInterpolator();
    auto meshData=m.GetMeshData();
    std::ofstream fileOut("../output/testTI.dat");

    for (int j = 0; j < 100; ++j)
    {
        for (int i = 0; i < 100; ++i)
        {
            fileOut <<meshData[j][i].GetX()<<" "<<meshData[j][i].GetY()<<" "<<meshData[j][i].GetZ() <<std::endl;
        }
    }

    m.TransFiniteInterpolator();
    return 0;
}
