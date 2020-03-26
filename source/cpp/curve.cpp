#include<curve.hpp>

mesh::curve::curve():isRead(false), isParameterized(false)
{
}

mesh::curve::~curve()
{
}

void mesh::curve::Read(const std::string& fileName)
{
    std::ifstream fileIn(fileName.c_str());
    if (!fileIn.is_open()) std::cout << "not open" << std::endl;

    std::string line;
    size_t curveSize;
    double coord[3]{ 0,0,0 };

    fileIn >> curveSize;
    for(size_t i = 0; i < curveSize; ++i)
    {
        fileIn >> coord[0] >> coord[1] >> coord[2];
        point pointData(coord[0],coord[1],coord[2]);
        curveData.emplace_back(pointData);
    }
    isRead = true;
}

void mesh::curve::InsertPoint(mesh::point& pointData)
{
    curveData.emplace_back(pointData);
}

void mesh::curve::Parameterize()
{
    double length = 0.0;
    parameterizeData.push_back(length);
    for(size_t i = 1; i < curveData.size(); ++i)
    {
        auto dx = curveData[i].GetX() - curveData[i-1].GetX();
        auto dy = curveData[i].GetY() - curveData[i-1].GetY();
        auto dz = curveData[i].GetZ() - curveData[i-1].GetZ();
        length += sqrt(dx*dx + dy*dy + dz*dz);
        parameterizeData.push_back(length);
    }
    for(size_t i = 0; i < curveData.size(); ++i)
    {
        parameterizeData[i]/=length;
    }
    isParameterized = true;
}


double mesh::curve::GetParameterAt(const size_t& i)
{
    return parameterizeData[i];
}

mesh::point mesh::curve::PointAt(const double& p)
{
    size_t leftPos = 0;
    size_t rightPos = parameterizeData.size()-1;

    //BINARY SEARCH nLog(n)
    while((rightPos-leftPos) != 1)
    {
        auto currentPos = leftPos + (rightPos-leftPos)/2;
        if(parameterizeData[currentPos] <= p)
        {
            leftPos = currentPos;
        }
        else
        {
            rightPos = currentPos;
        }
    }
    auto fraction = (p-parameterizeData[leftPos])/(parameterizeData[rightPos]-parameterizeData[leftPos]);

    return curveData[leftPos]*(1.0-fraction) + curveData[rightPos]*fraction;

}
