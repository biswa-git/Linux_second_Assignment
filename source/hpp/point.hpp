#pragma once
#include<essential>
namespace mesh
{
    class point
    {
    public:
        point();
        point(const double&, const double&, const double&);
        ~point();

        void SetX(const double&);
        double GetX() const;
        void SetY(const double&);
        double GetY() const;
        void SetZ(const double&);
        double GetZ() const;

        point operator+(const point&);
        point operator-(const point&);
        point operator*(const double&);

    protected:
        double x,y,z;
    };
}
