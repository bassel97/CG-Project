#include "_OurGameEngine\EnginePrime.hpp"

class Transform :public EnginePrime
{
public:
	Transform();
	~Transform();

	//if the position is defined by a parent
	Transform* parent;

	void Start();
	void Update();

private:

};