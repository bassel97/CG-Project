//All Objects and Components must inherit from it.
//Contains the most vital functions all components need.
//Abstract Class
class EnginePrime
{
public:

	//gets called once before game starts
	virtual void Start() = 0;

	//gets called every program iteration "Frame"
	virtual void Update() = 0;


};