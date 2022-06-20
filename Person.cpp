//file contains person details
#include <iostream>
#include <string.h>
#include "Person.h"
#include "AadharDetails.h"
#include "sqlite3.h"
#include <sstream>
using namespace std;

extern sqlite3 *db;
extern int personCount;
int callback(void *data, int argc, char **argv, char **azColName);

Person ::Person() {}
Person::Person(int i, std::string n, int a, string g, string qual, string db, string addr, long adharno)
{

	id = i;
	name = n;
	age = a;
	gender = g;
	qualification = qual;
	ptrDb = db;
	ptrAddr = addr;
	aadharNo = adharno;
	personCount = i + 1;
}

int Person ::getPersonId()
{
	return id;
}

bool Person::acceptPersonDetails()
{
	std::cout << "\n$$$$ Enter Personal Details $$$$$" << endl;
	this->id = personCount++;
	std::cout << "Enter Name :" << std::endl;
	cin >> this->name;
	std::cout << "Enter Age :" << std::endl;
	std::cin >> this->age;
	std::cout << "Enter Gender: " << std::endl;
	std::cin >> this->gender;
	std::cout << "Enter Date Of Birth(DD/MM/YYYY): " << std::endl;
	std::cin >> this->ptrDb;
	std::cout << "Enter Full Address: " << std::endl;
	cin >> this->ptrAddr;
	std::cout << "Please Enter Qualification: " << std::endl;
	std::cin >> qualification;
	std::cout << "Please Enter AadharId: " << std::endl;
	std::cin >> this->aadharNo;

	cout << "Details accepted" << endl;

	if (age < 18)
		return false;

	char *zErrMsg = 0;
	int rc;
	const char *sql;
	const char *data = "Callback function called";
	stringstream ss, ss2;
	string query, query2;

	ss << "INSERT INTO person "
		"VALUES (null,' "
	   << name << "','" << gender << "','" << age << "','" << ptrDb << "','" << ptrAddr << "','" << qualification << "')";
	query = ss.str();
	sql = query.c_str();

	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		fprintf(stdout, "Person Record inserted successfully\n");
	}
	return true;
}

void Person::displayPersonalDetails()
{
	std::string datePtr;
	std::string addrPtr;
	std::cout << "Name :" << getName() << std::endl;
	// std::cout<<"ID :"<<getId()<<std::endl;
	std::cout << "Aadhar :" << getAadharId() << std::endl;
	std::cout << "Age : " << getAge() << std::endl;
	std::cout << "Gender : " << getGender() << std::endl;
	datePtr = getDb();
	addrPtr = getAddr();
	std::cout << "Date of Birth : " << datePtr << std::endl;
	std::cout << "std::string : " << addrPtr << std::endl;
	std::cout << "Qualification: " << this->getQualification() << std::endl;
	std::cout << "Aadhar ID: " << this->getAadharId() << std::endl;
}

/*int Person::getId(){
	return this->id;
}

void Person::setId(int id){
	this->id = id;
}*/

std::string Person::getName()
{
	return name;
}

void Person::setName(std::string n)
{
	name = n;
}

int Person::getAge()
{
	return age;
}

void Person::setAge(int a)
{
	age = a;
}

std::string Person::getGender()
{
	return gender;
}

void Person::setGender(std::string g)
{
	gender = g;
}

std::string Person::getDb()
{
	return ptrDb;
}

void Person::setDb(std::string db)
{
	ptrDb = db; //Need to overload '=' operator here
}

void Person::setAddr(std::string addr)
{
	ptrAddr = addr; //Need to Overload '=' operator
}

std::string Person::getAddr()
{
	return ptrAddr;
}

std::string Person::getQualification()
{
	return qualification;
}

void Person::setQualification(std::string qual)
{
	qualification = qual;
}

void Person ::setAadharId(long id)
{
	aadharNo = id;
}

long Person::getAadharId()
{
	return aadharNo;
}

// int main(){
// 	std::string date(1, 10, 2014);
// 	std::string addr(1, "road", "temple", "city", "state", 411021);
// 	Person p1;
// 	AadharDetails a;
// 	// p1->setId(1234);
// 	p1.setName("Rahul Shinde");
// 	p1.setAssemblyId(101);
// 	p1.setQualification("BE");
// 	p1.setAge(21);
// 	p1.setGender('M');
// 	p1.setDb(date);
// 	p1.setAddr(addr);
// 	a.verify(1);
// 	// p1.getPersonDetails();
// 	p1.displayPersonalDetails();
// }
