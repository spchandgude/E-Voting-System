/*  Personal Details Class. It's object will be created in the Voter and Candidate classes.

	Case 		: Camel Case
*/
#pragma once

#include <iostream>

class Person
{
private:
	int id;
	std::string name;
	std::string gender;
	int age;
	std::string ptrDb;
	std::string ptrAddr;
	std::string qualification;
	long aadharNo;

public:
	Person();
	Person(int i, std::string n, int a, std::string g, std::string qual, std::string d, std::string addr, long adhar);
	/*
		: id(i), name(n), gender(g), age(a), ptrDb(d), ptrAddr(addr), qualification(qual), aadharNo(adhar), assemblyId(aId)
		{
		    std::cout<<"Person";
		}*/
	// int getId();
	//void setId(int);
	std::string getName();
	void setName(std::string);
	int getAge();
	void setAge(int);
	std::string getGender();
	void setGender(std::string);
	std::string getDb();
	void setDb(std::string);
	std::string getAddr();
	void setAddr(std::string);
	std::string getQualification();
	void setQualification(std::string);
	long getAadharId();
	void setAadharId(long);
	bool acceptPersonDetails();
	void displayPersonalDetails();
	int getPersonId();
};
