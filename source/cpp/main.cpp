#include<curve.hpp>
#include<mesh.hpp>
#include<vector.hpp>
int main()//int argc, char const *argv[]
{
	mesh::curve c1, c2, c3, c4, c5;
	
/*
        c3
    ----->-------
    |           |
 c2 ^           ^ c4,c5
    |           |
    ------>------
         c1
*/
    c1.Read("../input/curve1.dat");
    c2.Read("../input/curve2.dat");
    c3.Read("../input/curve3.dat");
    c4.Read("../input/curve4.dat");
	c5.Read("../input/curve5.dat");

	mesh::mesh m(80,80);
    m.SetBoundingCurve(c1,c2,c3,c4);

//---------------------------------//
    m.LaplaceSmoother();
//---------------------------------//
    auto meshData1=m.GetMeshData();
    std::ofstream fileOut("../output/LaplaceSmoother1.dat");
	//tecPlot format
	//fileOut << "TITLE = \"Example: Multi-Zone\""<<std::endl;
	//fileOut << "VARIABLES = \"X\", \"Y\", \"Z\""<<std::endl;
	//fileOut << "ZONE I = "<<m.GetSizeX()<<", J = "<<m.GetSizeY()<<", K = 1" <<std::endl;
    fileOut << m.GetSizeX() << " " << m.GetSizeY() <<std::endl;
    for (int j = 0; j < m.GetSizeY(); ++j)
    {
        for (int i = 0; i < m.GetSizeX(); ++i)
        {
            fileOut <<meshData1[j][i].GetX()<<" "<<meshData1[j][i].GetY()<<" "<<meshData1[j][i].GetZ() <<std::endl;
        }
    }
    fileOut.close();

    m.SetBoundingCurve(c1,c2,c3,c5);
//---------------------------------//
    m.TransFiniteInterpolator();
//---------------------------------//

    auto meshData2=m.GetMeshData();
    fileOut.open("../output/TransfiniteInterpolator.dat");
	//fileOut << "TITLE = \"Example: Multi-Zone\""<<std::endl;
	//fileOut << "VARIABLES = \"X\", \"Y\", \"Z\""<<std::endl;
	//fileOut << "ZONE I = "<<m.GetSizeX()<<", J = "<<m.GetSizeY()<<", K = 1" <<std::endl;
	fileOut << m.GetSizeX() << " " << m.GetSizeY() << std::endl;
    for (int j = 0; j < m.GetSizeY(); ++j)
    {
        for (int i = 0; i < m.GetSizeX(); ++i)
        {
            fileOut <<meshData2[j][i].GetX()<<" "<<meshData2[j][i].GetY()<<" "<<meshData2[j][i].GetZ() <<std::endl;
        }
    }
    fileOut.close();
    //---------------------------------//
        m.LaplaceSmoother();
    //---------------------------------//
        auto meshData3=m.GetMeshData();
        fileOut.open("../output/LaplaceSmoother2.dat");
		//fileOut << "TITLE = \"Example: Multi-Zone\""<<std::endl;
		//fileOut << "VARIABLES = \"X\", \"Y\", \"Z\""<<std::endl;
		//fileOut << "ZONE I = "<<m.GetSizeX()<<", J = "<<m.GetSizeY()<<", K = 1" <<std::endl;
		fileOut << m.GetSizeX() << " " << m.GetSizeY() << std::endl;
        for (int j = 0; j < m.GetSizeY(); ++j)
        {
            for (int i = 0; i < m.GetSizeX(); ++i)
            {
                fileOut <<meshData3[j][i].GetX()<<" "<<meshData3[j][i].GetY()<<" "<<meshData3[j][i].GetZ() <<std::endl;
            }
        }
        fileOut.close();

    return 0;
}
