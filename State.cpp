//file contains state functions

#include "State.h"
#include "ElectionCommision.h"
#include "sqlite3.h"
#include <sstream>

extern ElectionCommision e;

extern sqlite3 *db;

int callback(void *data, int argc, char **argv, char **azColName);

State::State(int stateID, string name, int no)
{
	char *zErrMsg = 0;
	int rc;
	const char *sql;
	const char *data = "Callback function called";

	this->stateId = stateID;
	this->stateName = name;

	e.getMapStateNameStateId().insert({stateName, stateID});
	stringstream ss, ss2;
	string query, query2;

	sqlite3_stmt *stmt;
	ss << "select * from assembly where stateid = " << (stateID);
	query = ss.str();
	sql = query.c_str();

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		// fprintf("error: ", sqlite3_errmsg(sqlite3->db));
	}
	Assembly *assemblyObj;

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		int asmId = sqlite3_column_int(stmt, 0);
		string assemblyName = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
		int population = sqlite3_column_int(stmt, 2);
		int eligibleVoterCount = sqlite3_column_int(stmt, 3);
		int currentCandidateid = sqlite3_column_int(stmt, 4);
		int stateid = sqlite3_column_int(stmt, 5);
		assemblyObj = new Assembly(asmId, assemblyName, population, eligibleVoterCount, currentCandidateid, stateid);
		assemblyList.insert({assemblyObj->getAssemblyNo(), assemblyObj});
	}
	if (rc != SQLITE_DONE)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	}
	sqlite3_finalize(stmt);
}

int State::getStateId()
{
	return stateId;
}

void State::setStateId(int p)
{
	stateId = p;
}

std::string State::getStateName()
{
	return this->stateName;
}

// void State::addNewAssembly(Assembly &assemblyObj)
// {
// 	int tempAssemblyNo;
// 	cout<<"Enter Assembly No\t";
// 	cin>>tempAssemblyNo;
// 	assemblyObj.setAssemblyNo(tempAssemblyNo);

// 	if(assemblyList.find(tempAssemblyNo) == assemblyList.end())
// 	{
// 		assemblyObj.acceptAssemblyDetails();
// 		assemblyList.insert({assemblyObj.getAssemblyNo(),&assemblyObj});
// 	}
// 	else
// 	{
// 		cout<<"Assembly NO already Exist"<<endl;
// 	}

// }
void State::showAssemblyDetails()
{
	for (auto x : assemblyList)
	{
		(x.second)->showAssemblyDetails();
	}
}
/*
map<std::string, int> State::getAssemblyNameAssemblyId()
{
	return 0;
}
*/
void State::setAssmblyNameAssemblyId(std::string, int)
{
}

State::~State()
{
	for (auto x : assemblyList)
		delete x.second;
}