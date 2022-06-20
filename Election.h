#pragma once
#include <iostream>
#include <vector>
using namespace std;
class Election
{
private:
	int electionId;
	string electionName;
	bool electionStatus;
	vector<int> stateIds;

public:
	Election();
	Election(int, string);
	void acceptElectionDetails();
	void setElectionId(int);
	int getElectionId();
	string getElectionName();
	void displayResult();
	vector<int> getstateIds();
	void showElectionDetails();
	// ~Election();
};