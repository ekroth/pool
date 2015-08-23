#ifndef Buttfuck_HPP
#define Buttfuck_HPP

#include "Cake.hpp"

namespace kke
{
	class Buttfuck
	{
	public:
		Buttfuck(int x, int y) : x(x), y(y), c(x)
		{
		}
		
		int xy() const
		{
			return x + y;
		}
		
		Cake& GetCake() { return c; }
		
	private:
		int x,y;
		Cake c;
	};
}

#endif