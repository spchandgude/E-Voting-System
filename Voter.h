#pragma once

#include "Person.h"
#include "Assembly.h"

class Assembly;
class Voter
{
	int voterId;
	int assemblyId;
	int stateId;
	std::string username;
	std::string password;
	int verified;
	pair<int, Person *> personalDetails;

public:
	Voter();
	Voter(int v, int a, int s, int p, string, string, int);
	pair<int, Person *> getVoterPersonPair();
	int getVoterId();
	void setVoterPersonPair(int, Person *);
	void displayVoterDetails();
	void setStateId(int stateId);
	int getStateId();
	void setAssemblyId(int AssemblyId);
	void registerAsCandidate();
	int getAssemblyId();
	std::string getUSername();
	void setUsername(std::string);

	std::string getPassword();
	void setPassword(std::string);
	void vote();
	~Voter();
};
