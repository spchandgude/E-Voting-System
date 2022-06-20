#pragma once

#include"Person.h"
#include<map>

class AadharDetails
{
	std :: map<long,Person *> aadharDetailsList;
	public:
		AadharDetails();
		bool verify(long);
};