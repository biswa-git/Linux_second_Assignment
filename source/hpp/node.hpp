#pragma once
#include<essential>
#include<point.hpp>
namespace mesh
{
    class node: public point
    {
    public:
        node(const size_t& = 0, const size_t& = 0);
        node(const double&, const double&, const double&,const size_t& = 0, const size_t& = 0);
        ~node();
        void SetI(const size_t&);
        size_t GetI();
        void SetJ(const size_t&);
        size_t GetJ();
        void operator=(const point&);
    private:
        size_t i,j;
    };
}
