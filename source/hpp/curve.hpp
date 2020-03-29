#pragma once
#include<essential>
#include<point.hpp>

namespace mesh
{
    class curve
    {
    public:
        curve();
        ~curve();
        void Read(const std::string&);
        void InsertPoint(point&);
        void Parameterize();
        bool Intersect(curve&);
        double GetParameterAt(const size_t&);
        point PointAt(const double&);
    private:
        bool isRead, isParameterized;
        std::vector<point> curveData;
        std::vector<double> parameterizeData;

    };
}
