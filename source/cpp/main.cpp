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

//---------------------------------//
    m.LaplaceSmoother();
//---------------------------------//
    auto meshData1=m.GetMeshData();
    std::ofstream fileOut("../output/testTI.dat");
    for (int j = 0; j < 100; ++j)
    {
        for (int i = 0; i < 100; ++i)
        {
            fileOut <<meshData1[j][i].GetX()<<" "<<meshData1[j][i].GetY()<<" "<<meshData1[j][i].GetZ() <<std::endl;
        }
    }
    fileOut.close();

//---------------------------------//
    m.TransFiniteInterpolator();
//---------------------------------//

    auto meshData2=m.GetMeshData();
    fileOut.open("../output/testTI.dat");
    for (int j = 0; j < 100; ++j)
    {
        for (int i = 0; i < 100; ++i)
        {
            fileOut <<meshData2[j][i].GetX()<<" "<<meshData2[j][i].GetY()<<" "<<meshData2[j][i].GetZ() <<std::endl;
        }
    }

    return 0;
}
