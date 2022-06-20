#pragma once

#include <iostream>
#include <vector>
#include <map>

using namespace std;
class Voter;
class Candidate;

class Assembly
{
	int assemblyNo;
	string assemblyName;
	int population;
	int eligibleVotersCount;
	int currentCandidateId;
	int stateId;
	map<int, int> candidateVotes;		 // map<CandidateId,CandidateVotes>
	map<int, Voter *> voterList;		 //Map Of VoterId and PersonDetails
	map<int, Candidate *> candidateList; //Map of CandidateId and PersonDetails.
public:
	Assembly();
	~Assembly();
	Assembly(int, std::string, int, int, int, int); //AssemblyId,AssemblyName,Population,VoterCount will be given by State Class while initialising its Assemblylist in its instance.
	int getAssemblyNo();
	string getAssemblyName();
	int getPoulation();
	int getEligibleVoters();
	void showAssemblyDetails();
	void addVote(int candidateId);
	int findWinner();
	void displayVoters();
	void showCandidateVotes();
	map<int, int> *getCandidateVotesList();
	map<int, Voter *> getVoterList();
	map<int, Candidate *> getCandidateList();
	void setCandidateList(int candidateId, Candidate *candidateObj);
	void setVoterList(int voterId, Voter *voterObj);
	void setCandidateVotesList(int candidateId, int candidateVotes);
};
