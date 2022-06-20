//file contains election commission details
#include "ElectionCommision.h"
#include "sqlite3.h"
#include <sstream>
#include <iostream>
using namespace std;

extern int voterCount;
extern int candidateCount;
extern int electionCount;

extern sqlite3 *db;

int callback(void *data, int argc, char **argv, char **azColName);
ElectionCommision::ElectionCommision()
{
	char *zErrMsg = 0;
	int rc;
	const char *sql;
	const char *data = "Callback function called";

	stringstream ss, ss2;
	string query, query2;

	sqlite3_stmt *stmt;
	ss << "select electionid,electionname from election group by electionid";
	query = ss.str();
	sql = query.c_str();

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		// fprintf("error: ", sqlite3_errmsg(sqlite3->db));
	}
	Election *electionObj;

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		int electionId = sqlite3_column_int(stmt, 0);
		string electionName = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
		electionObj = new Election(electionId, electionName);
		electionsList.insert({electionObj->getElectionId(), electionObj});
	}
}
Voter *ElectionCommision::registerNewVoter()
{
	Voter *voterObj;
	Person *personObj = new Person();
	if (!personObj->acceptPersonDetails())
		return NULL;

	std::string username;
	std::string password;
	pair<int, int> pairStateIdAsmId;

	std::cout << "Enter Username" << endl;
	std::cin >> username;
	std::cout << "Enter Password" << endl;
	std::cin >> password;
	/* Generate Voter_ID and Assembly_ID if the candidate is eligible based on the std::string after verification*/
	assignStateId(personObj, pairStateIdAsmId);

	int voterId = voterCount;
	int personId = personObj->getPersonId();

	//int stateId = pairStateIdAsmId.first;
	//int assemblyId = pairStateIdAsmId.second;
	int stateId, assemblyId;
	std::cout << "Enter State ID" << endl;
	std::cin >> stateId;
	std::cout << "Enter Assembly Id" << endl;
	std::cin >> assemblyId;
	voterObj = new Voter(voterCount, assemblyId, stateId, personId, username, password, 0);

	states[stateId]->assemblyList[assemblyId]->setVoterList(voterId, voterObj);
	// states[stateId]->assemblyList[assemblyId]->getVoterList()[1]->displayVoterDetails();

	char *zErrMsg = 0;
	int rc;
	const char *sql;
	const char *data = "Callback function called";
	stringstream ss, ss2;
	string query, query2;

	ss << "INSERT INTO voter "
		"VALUES (null,'"
	   << username << "','" << password << "','" << assemblyId << "','" << stateId << "','" << personId << "','" << 0 << "')";
	query = ss.str();
	sql = query.c_str();

	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return NULL;
	}
	else
	{
		fprintf(stdout, "Person Record inserted successfully\n");
		std::cout << "{{{{{{{{     Voter Registered       }}}}}}}" << endl;
	}

	return voterObj;
}

void ElectionCommision::registerNewCandidate(Person *personObj)
{
	Candidate *candidateObj;
	int personid = personObj->getPersonId();
	int stateId, asmId;
	string username, password, party;

	std::cout << "\nElection Id\t\tElection Name" << endl;
	for (auto x : electionsList)
	{
		std::cout << x.first << "\t\t" << x.second->getElectionName() << endl;
	}

	std::cout << "Enter Election Id of Which You want to see Details" << endl;
	int inputElectionId;
	std::cin >> inputElectionId;

	std::cout << "\nHere is List of States in which this Election is going on" << endl;
	std::cout << "State Id \t\t State Name" << endl;
	for (auto x : electionsList[inputElectionId]->getstateIds())
	{
		std::cout << x << "\t\t" << states[x]->getStateName() << endl;
	}

	cin >> stateId;
	std::cout << "\nAssembly List. Enter Assembly Id In Which You Want To Contest :" << endl;
	std::cout << "AssemblyId "
		    << "Assembly Name" << endl;
	for (auto x : states[stateId]->assemblyList)
	{
		std::cout << x.first << "\t" << x.second->getAssemblyName() << endl;
	}
	std::cout << "INPUT :";
	std::cin >> asmId;

	std::cout << "Enter Party Name" << endl;
	std::cin >> party;
	std::cout << "Enter Username" << endl;
	std::cin >> username;
	std::cout << "Enter Password" << endl;
	std::cin >> password;

	int candidateId = candidateCount;

	candidateObj = new Candidate(candidateId, asmId, stateId, inputElectionId, personid, party, username, password);

	states[stateId]->assemblyList[asmId]->setCandidateList(candidateId, candidateObj);
	states[stateId]->assemblyList[asmId]->setCandidateVotesList(candidateId, 0);
	states[stateId]->assemblyList[asmId]->getCandidateList()[1]->displayCandidateDetails();

	char *zErrMsg = 0;
	int rc;
	const char *sql;
	const char *data = "Callback function called";
	stringstream ss, ss2;
	string query, query2;

	ss2 << "INSERT INTO candidate "
		 "VALUES (null,' "
	    << username << "','" << password << "','" << asmId << "','" << stateId << "','" << personid << "','" << inputElectionId << "','" << party << "')";
	query = ss2.str();
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
		std::cout << "{{{{{{{{     Candidate Registered       }}}}}}}" << std::endl;
	}
}

void ElectionCommision::createElection()
{
	Election *electionObj = new Election();
	electionObj->setElectionId(electionCount);
	electionObj->acceptElectionDetails();

	electionsList.insert({electionObj->getElectionId(), electionObj});
}

void ElectionCommision::showOngoingElectionDetails()
{
	int inputElectionId, inputStateId, inputAssemblyId;
	std::cout << "In ShowOngoingElectionDetails Function " << std::endl;
	//for (auto x : states[1]->assemblyList[1]->getVoterList())
	//{
	//	std::cout << x.first << "\t\t" << x.second->getVoterPersonPair().second->getName() << "\t\t"<<std::endl;
	//	//std::cout << states[inputStateId]->assemblyList[inputAssemblyId]->getCandidateVotesList()[x.first] << endl;
	//}
	std::cout << "\n****************Here is List of All Ongoing Elections***************" << endl;
	std::cout << "\nElection Id\t\tElection Name" << endl;
	for (auto x : electionsList)
	{
		std::cout << x.first << "\t\t" << x.second->getElectionName() << endl;
	}

	std::cout << "\nEnter Election Id of Which You want to see Details" << endl;
	std::cin >> inputElectionId;

	std::cout << "\nHere is List of States in which this Election is going on :-" << endl;
	std::cout << "State Id \t\t State Name" << endl;
	for (auto x : electionsList[inputElectionId]->getstateIds())
	{
		std::cout << x << "\t\t" << states[x]->getStateName() << endl;
	}

	std::cout << "\nEnter State Id of which you want to see Details" << endl;
	std::cin >> inputStateId;

	std::cout << "\nHere is List of Asselmblies in This State" << endl;
	for (auto x : states[inputStateId]->assemblyList)
	{
		std::cout << x.first << "\t\t" << x.second->getAssemblyName() << endl;
	}
	std::cout << "\nEnter Assembly Id of which you want to see Details" << endl;
	std::cin >> inputAssemblyId;
	std::cout << "\nHere List of Candidates in this Assembly :- " << endl;
	std::cout << "\nCandidate ID \t\tCandidate Name\t\tCandidate Votes" << endl;
	for (auto x : states[inputStateId]->assemblyList[inputAssemblyId]->getCandidateList())
	{
		std::cout << x.first << "\t\t\t" << x.second->getCandidatePersonPair().second->getName() << "\t\t";
		//std::cout << states[inputStateId]->assemblyList[inputAssemblyId]->getCandidateVotesList()[x.first] << endl;
		std::cout << (*(states[inputStateId]->assemblyList[inputAssemblyId]->getCandidateVotesList()))[x.first] << std::endl;
	}
}

void ElectionCommision::addNewState(int i, string stateName, int no)
{
	State *stateObj = new State(i, stateName, no);
	if (states.find(i) == states.end())
	{
		states.insert({stateObj->getStateId(), stateObj});
	}
	else
	{
		std::cout << "State ID alredy exist in Record" << endl;
	}
}

void ElectionCommision::showStates()
{
	std::cout << "**********STATES DETAILS************" << endl;
	for (auto x : states)
	{
		std::cout << "State ID :" << x.first << endl;
		std::cout << "State Name :" << (x.second)->getStateName();
		(x.second)->showAssemblyDetails();
	}
}

void ElectionCommision::assignStateId(Person *personObj, pair<int, int> &pairStateIdAsmId)
{

	map<std::string, int> mapStateNameStateID;
	mapStateNameStateID.insert({"Maharashtra", 0});
	mapStateNameStateID.insert({"Madhya Pradesh", 1});
	mapStateNameStateID.insert({"Kerala", 2});
	mapStateNameStateID.insert({"Manipur", 4});

	//map<std::string,map<std::string,int>> mapCityAssembly;
	map<std::string, int> assemblyNameAssemblyId;

	assemblyNameAssemblyId.insert({"Kasba Peth", 0});
	assemblyNameAssemblyId.insert({"Kothrud", 1});
	assemblyNameAssemblyId.insert({"Baramati", 2});

	//mapCityAssembly.insert({"Pune",assemblyNameAssemblyId});
	pairStateIdAsmId.first = mapStateNameStateID["Maharashtra"];    //states[personObj->getAddr().state];
	pairStateIdAsmId.second = assemblyNameAssemblyId["Kasba Peth"]; //mapAsmCity[personObj->getAddr().city]["Kothrud"];
											    //states[pairStateIdAsmId.first]->getAssemblyNameAssemblyId()["Kasba Peth"];
}

map<std::string, int> ElectionCommision::getMapStateNameStateId()
{
	return mapStateNameStateID;
}

void ElectionCommision::setMapStateNameStateId(std::string, int stateId)
{
}

ElectionCommision::~ElectionCommision()
{
	for (auto x : electionsList)
		delete x.second;
	for (auto x : states)
		delete x.second;
}
void ElectionCommision::endElection()
{
	int inputElectionId;
	std::cout << "\nElection Id\t\tElection Name" << endl;
	for (auto x : electionsList)
	{
		std::cout << x.first << "\t\t" << x.second->getElectionName() << endl;
	}
	cout << "Enter Id to stop: ";
	cin >> inputElectionId;

	for (auto x : electionsList[inputElectionId]->getstateIds())
	{
		for (auto y : states[x]->assemblyList)
		{
			std::cout << y.second->findWinner() << endl;
		}
	}
}