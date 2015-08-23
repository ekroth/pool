#ifndef Cake_HPP
#define Cake_HPP

namespace kke
{
	class Cake
	{
	public:
		Cake(int bajs) :bajs(bajs)
		{
		}
		
		int omega() const { return bajs * bajs; }
		
	private:
		int bajs;
	};
}

#endif