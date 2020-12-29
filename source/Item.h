#pragma once
#include "Objeto.h"
class Item : public Objeto
{

public:
	//construtores
	Item();

	//destrutor
	~Item();

	void setAliveTrue(Objeto *obj);

};

