#pragma once
#include <iostream>
#include <map>
#include "Person.h"
using namespace std;
class Candidate
{
private:
	int candidateId;
	std::string partyName;
	pair<int, Person *> personalDetails;
	int assemblyId;
	int electionId;
	int stateId;
	int personId;
	std::string username;
	std::string password;

public:
	Candidate();
	Candidate(int a, int b, int c, int d, int e, std::string f, std::string, std::string);
	void regist();
	pair<int, Person *> getCandidatePersonPair();
	void setCandidatePersonPair(int, Person *);
	int getStateId();
	void displayCandidateDetails();
	void setStateId(int);
	int getAssemblyId();
	void setAssemblyId(int asmId);
	int getCandidateId();
	int getElectionId();
	void setElectionId(int electionId);
	std::string getUSername();
	void setUsername(std::string);
	std::string getPassword();
	void setPassword(std::string);
	std::string getPartyName();
	~Candidate();
};
