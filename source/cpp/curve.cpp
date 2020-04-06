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
	Parameterize();
}

void mesh::curve::InsertPoint(mesh::point& pointData)
{
    curveData.emplace_back(pointData);
}

void mesh::curve::Parameterize()
{
    if(isParameterized) return;
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

double mesh::curve::Intersect(mesh::curve& c)
{
	for (size_t i = 0; i < this->curveData.size() - 1; i++)
	{
		for (size_t j = 0; j < c.curveData.size() - 1; j++)
		{
			auto line1Start = this->curveData[i];
			auto line1End = this->curveData[i + 1];
			auto line2Start = c.curveData[j];
			auto line2End = c.curveData[j + 1];
			auto dir1 = mesh::CrossProd(line1Start, line2Start, line1End).Unit();
			auto dir2 = mesh::CrossProd(line1End, line2End, line1Start).Unit();
			auto dir3 = mesh::CrossProd(line2Start, line1Start, line2End).Unit();
			auto dir4 = mesh::CrossProd(line2End, line1End, line2Start).Unit();

			if (dir1.Abs() < EPS || dir2.Abs() < EPS || dir3.Abs() < EPS || dir4.Abs() < EPS || ((dir1 == dir2) && (dir3 == dir4)))// if true then two lines intersects
			{
				vector p2_p1(line1Start, line2Start);
				vector line1(line1Start, line1End);
				vector line2(line2Start, line2End);
				vector v1 = line1.Unit();
				vector v2 = line2.Unit();

				auto a = (p2_p1 ^ v2).Abs()/ (v1 ^ v2).Abs();
				return this->parameterizeData[i] + (this->parameterizeData[i + 1] - this->parameterizeData[i]) * a / line1.Abs();
				/*
				point intersectionPoint(
					line1Start.GetX() + a * v1.GetDx(),
					line1Start.GetY() + a * v1.GetDy(),
					line1Start.GetZ() + a * v1.GetDz());
				return intersectionPoint;
				*/

			}
		}
	}
	std::cerr << "No intersection found. Exiting..." << std::endl;
	exit(EXIT_FAILURE);
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
