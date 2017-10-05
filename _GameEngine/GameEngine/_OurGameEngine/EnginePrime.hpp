//All Objects and Components must inherit from it.
//Contains the most vital functions all components need.
//Abstract Class
class EnginePrime
{

	//gets called once before game starts
	virtual Start();

	//gets called every program iteration "Frame"
	virtual Update();

	//Virtual destructor to insure class is abstract
	virtual ~EnginePrime() = 0;

};