#include<point.hpp>

mesh::point::point():x(0),y(0),z(0)
{

}

mesh::point::point(const double& x, const double& y, const double& z):x(x),y(y),z(z)
{

}

void mesh::point::SetX(const double& _x)
{
    x = _x;
}

double mesh::point::GetX() const
{
    return x;
}
void mesh::point::SetY(const double& _y)
{
    y = _y;
}

double mesh::point::GetY() const
{
    return y;
}
void mesh::point::SetZ(const double& _z)
{
    z = _z;
}

double mesh::point::GetZ() const
{
    return z;
}

mesh::point mesh::point::operator+(const point& p)
{
    point result;
    result.SetX(this->x+p.x);
    result.SetY(this->y+p.y);
    result.SetZ(this->z+p.z);
    return result;
}

mesh::point mesh::point::operator-(const point& p)
{
    point result;
    result.SetX(this->x-p.x);
    result.SetY(this->y-p.y);
    result.SetZ(this->z-p.z);
    return result;
}

mesh::point mesh::point::operator*(const double& multiplier)
{
    point result;
    result.SetX(multiplier*this->x);
    result.SetY(multiplier*this->y);
    result.SetZ(multiplier*this->z);
    return result;
}

bool mesh::point::operator==(const point& p)
{
    if( fabs(x-p.x)<1e-12 && fabs(y-p.y)<1e-12 && fabs(z-p.z)<1e-12 )
    {
        return true;
    }
    else
    {
        return  false;
    }
}
