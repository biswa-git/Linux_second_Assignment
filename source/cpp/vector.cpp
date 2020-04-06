#include "vector.hpp"

mesh::vector::vector(const point& start, const point& end)
{
	dx = end.GetX() - start.GetX();
	dy = end.GetY() - start.GetY();
	dz = end.GetZ() - start.GetZ();
}

mesh::vector::vector(const double& dx, const double& dy, const double& dz) :dx(dx), dy(dy), dz(dz)
{
	//std::cout << "vector created" << std::endl;
}

mesh::vector::~vector()
{
	//std::cout << "vector deleted" << std::endl;
}

mesh::vector mesh::vector::operator+(const vector& V)
{
	return vector(this->dx + V.dx, this->dy + V.dy, this->dz + V.dz);
}

mesh::vector mesh::vector::operator-(const vector& V)
{
	return vector(this->dx - V.dx, this->dy - V.dy, this->dz - V.dz);
}

double mesh::vector::operator*(const vector& V)
{
	return (this->dx * V.dx + this->dy * V.dy + this->dz * V.dz);
}

//3d implementatin needed
mesh::vector mesh::vector::operator^(const vector& V)
{
	vector crossProd;
	crossProd.SetDx((this->dy * V.dz) - (this->dz * V.dy));
	crossProd.SetDy((this->dz * V.dx) - (this->dx * V.dz));
	crossProd.SetDz((this->dx * V.dy) - (this->dy * V.dx));
	return crossProd;
}

double mesh::vector::Abs()
{
	return sqrt(dx * dx + dy * dy + dz * dz);
}

void mesh::vector::SetDx(const double& dx)
{
	this->dx = dx;
}

void mesh::vector::SetDy(const double& dy)
{
	this->dy = dy;
}

void mesh::vector::SetDz(const double& dz)
{
	this->dz = dz;
}

double mesh::vector::GetDx() const
{
	return dx;
}

double mesh::vector::GetDy() const
{
	return dy;
}

double mesh::vector::GetDz() const
{
	return dz;
}

mesh::vector mesh::vector::Unit()
{
	auto abs = this->Abs();
	if (abs < EPS)
	{
		vector unit(0,0,0);
		return unit;
	}
	else
	{
		auto uDx = dx / abs;
		auto uDy = dy / abs;
		auto uDz = dz / abs;
		vector unit(uDx, uDy, uDz);
		return unit;
	}
}

void mesh::vector::Reassign(const point& start, const point& end)
{
	dx = end.GetX() - start.GetX();
	dy = end.GetY() - start.GetY();
	dz = end.GetZ() - start.GetZ();
}

void mesh::vector::Reassign(const double& dx, const double& dy, const double& dz)
{
	this->dx = dx;
	this->dy = dy;
	this->dz = dz;
}

void mesh::vector::Print() const
{
	std::cout << "[" << dx << "," << dy << "," << dz << "]" << std::endl;
}


bool mesh::vector::operator==(const vector& V)
{
	if ((fabs(this->dx - V.dx) < EPS) && (fabs(this->dy - V.dy) < EPS) && (fabs(this->dz - V.dz) < EPS)) return true;
	return false;
}

mesh::vector mesh::CrossProd(const point& p1, const point& p2, const point& p3)
{
	vector v1(p1, p2), v2(p2, p3);
	return (v1 ^ v2);
}
