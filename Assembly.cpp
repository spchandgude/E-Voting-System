//file contains assembly functions
#include <list>
#include <map>
#include "Assembly.h"
#include "ElectionCommision.h"
#include "sqlite3.h"
#include <sstream>
extern ElectionCommision e;
int voterCount = 1;
int candidateCount = 1;
int personCount = 1;
extern sqlite3 *db;

Assembly::Assembly(int assemblyId, std::string assemblyName, int population, int voterCount, int currentCandidateId, int stateId)
{
	char *zErrMsg = 0;
	int rc;
	const char *sql;
	const char *data = "Callback function called";
	int callback(void *data, int argc, char **argv, char **azColName);

	stringstream ss, ss2;
	string query, query2;

	sqlite3_stmt *stmt;

	ss << "select * from candidate where stateid = " << stateId << " and assemblyid = " << assemblyId;
	query = ss.str();
	sql = query.c_str();

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	Candidate *candidateObj;
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		int candidateid = sqlite3_column_int(stmt, 0);
		string username = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
		string password = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
		int assemblyid = sqlite3_column_int(stmt, 3);
		int stateid = sqlite3_column_int(stmt, 4);
		int personid = sqlite3_column_int(stmt, 5);
		int electionid = sqlite3_column_int(stmt, 6);
		string partyname = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7)));
		int votes = sqlite3_column_int(stmt, 8);

		candidateObj = new Candidate(candidateid, assemblyid, stateid, electionid, personid, partyname, username, password);

		candidateList.insert({candidateObj->getCandidateId(), candidateObj});
		candidateVotes.insert({candidateObj->getCandidateId(), votes});
		//e.getMapStateNameStateId()[stateName]->assemblyNameAssemblyId[stateObj->getName()] = i;
	}
	if (rc != SQLITE_DONE)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		// fprintf("error: ", sqlite3_errmsg(db));
	}
	sqlite3_finalize(stmt);

	ss2 << "select * from voter where stateid = " << stateId << " and assemblyid = " << assemblyId;

	query = ss2.str();
	sql = query.c_str();

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		// fprintf("error: ", sqlite3_errmsg(sqlite3->db));
	}
	Voter *voterObj;

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		int voterid = sqlite3_column_int(stmt, 0);
		string username = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
		string password = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
		int assemblyid = sqlite3_column_int(stmt, 3);
		int stateid = sqlite3_column_int(stmt, 4);
		int personid = sqlite3_column_int(stmt, 5);
		int verified = sqlite3_column_int(stmt, 6);

		voterObj = new Voter(voterid, assemblyid, stateid, personid, username, password, verified);
		voterList.insert({voterObj->getVoterId(), voterObj});
		//e.getMapStateNameStateId()[stateName]->assemblyNameAssemblyId[stateObj->getName()] = i;
	}

	sqlite3_finalize(stmt);

	this->assemblyNo = assemblyId;
	this->assemblyName = assemblyName;
	this->population = population;
	this->eligibleVotersCount = voterCount;
	//this->currentCandidateId = currentCandidateId;
	this->stateId = stateId;
}
int Assembly::getAssemblyNo()
{
	return assemblyNo;
}

string Assembly::getAssemblyName()
{
	return assemblyName;
}
int Assembly::getPoulation()
{
	return population;
}
int Assembly::getEligibleVoters()
{
	return eligibleVotersCount;
}

void Assembly::displayVoters()
{

	for (int i = 0; i < getEligibleVoters(); ++i)
	{
		//std::cout<<voterList[i]->getPersonDetails(); // Person Class Should have getPersonDetails()
	}
}

void Assembly::addVote(int candidateId)
{
	this->candidateVotes[candidateId]++;
}

void Assembly::showCandidateVotes()
{
	cout << "Candidates Id\tTotal Votes" << endl;
	for (auto x : candidateVotes)
	{
		cout << x.first << "\t\t" << x.second << endl;
	}
}

/* 	ElectionClass will contain display Election results.
	It will call all assemblies findWinner();
	findWinner() will return winner's candidate ids.
	ElectionClass can store these candidate ids and then print the candidate details over there */

int Assembly::findWinner()
{
	pair<int, int> temp = make_pair(0, 0);
	map<int, int>::iterator iter;
	for (iter = candidateVotes.begin(); iter != candidateVotes.end(); iter++)
	{
		if (iter->second > temp.second)
		{
			temp = make_pair(iter->first, iter->second);
		}
	}
	if (temp.first != 0)
	{
		auto winner_details = candidateList.find(temp.first)->second;
		cout << "The winner is " << winner_details->getUSername() << " and has won by " << temp.second << " votes." << endl;
	}
	else
		return -1;
}

void Assembly::showAssemblyDetails()
{
	cout << "\nBelow are Assembly Details\n\t";

	cout << "\n\tAssembly Name:\t\t\t\t" << getAssemblyName();
	cout << "\n\tAssembly Population:\t\t\t" << getPoulation();
	cout << "\n\tNumber of Eiligible Voters:\t\t" << getEligibleVoters();
	for (int i = 0; i < getEligibleVoters(); i++)
	{
		//std::cout<<voterList[i]->getPersonDetails()<<std::endl; // Person Class Should have getPersonDetails()
	}
	cout << endl;
}

map<int, int> *Assembly::getCandidateVotesList()
{

	return &candidateVotes;
}

map<int, Voter *> Assembly::getVoterList()
{
	return voterList;
}

map<int, Candidate *> Assembly::getCandidateList()
{
	return candidateList;
}

void Assembly::setCandidateList(int candidateId, Candidate *candidateObj)
{
	candidateList[candidateId] = candidateObj;
}

void Assembly::setVoterList(int voterId, Voter *voterObj)
{
	voterList[voterId] = voterObj;
}

void Assembly::setCandidateVotesList(int candidateId, int votes)
{
	candidateVotes[candidateId] = votes;
}

Assembly::~Assembly()
{
	for (auto x : voterList)
		delete x.second;

	for (auto x : candidateList)
		delete x.second;
}