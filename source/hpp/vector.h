#pragma once
#include<point.hpp>
#include<essential>
namespace mesh
{
	class vector
	{
	public:

		vector(const point&, const point&);
		vector(const double& = 0.0, const double& = 0.0, const double& = 0.0);
		virtual ~vector();

		vector operator+(const vector&);
		vector operator-(const vector&);
		double operator*(const vector&); // DOT PRODUCT
		vector operator^(const vector&); // CROSS PRODUCT
		double Abs();

		double GetDx() const;
		double GetDy() const;
		double GetDz() const;

		void Reassign(const point&, const point&);
		void Reassign(const double&, const double&, const double& = 0.0);

		void Print() const;

	private:
		double dx, dy, dz;
		void SetDx(const double&);
		void SetDy(const double&);
		void SetDz(const double&);

	};
}