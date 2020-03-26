#include<node.hpp>

mesh::node::node(const size_t& i, const size_t& j): i(i), j(j)
{

}

mesh::node::node(const double& _x, const double& _y, const double& _z, const size_t& i, const size_t& j): mesh::point(_x,_y,_z), i(i), j(j)
{

}

mesh::node::~node()
{

}

void mesh::node::SetI(const size_t& _i)
{
    i = _i;
}

size_t mesh::node::GetI()
{
    return i;
}
void mesh::node::SetJ(const size_t& _j)
{
    j = _j;
}

size_t mesh::node::GetJ()
{
    return j;
}

void mesh::node::operator=(const point& p)
{
    this->x = p.GetX();
    this->y = p.GetY();
    this->z = p.GetZ();
}
