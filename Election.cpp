//file contains election functions
#include "Election.h"
#include "ElectionCommision.h"
#include <iostream>
#include "sqlite3.h"
#include <sstream>
using namespace std;

extern sqlite3 *db;
int electionCount = 1;
int callback(void *data, int argc, char **argv, char **azColName);

extern ElectionCommision e;

Election::Election() {}
Election::Election(int id, string name)
{
	this->electionId = id;
	this->electionName = name;
	this->electionStatus = false;

	electionCount = id + 1;
	sqlite3_stmt *stmt;
	char *zErrMsg = 0;
	int rc, rc2;
	const char *sql, *sql2;
	const char *data = "Callback function called";
	int callback(void *data, int argc, char **argv, char **azColName);
	stringstream ss;
	string query;

	ss << "select stateid  from election where electionid = " << electionId;
	query = ss.str();
	sql = query.c_str();

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		// fprintf("error: ", sqlite3_errmsg(sqlite3->db));
	}
	int count = 0;
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		int stateId = sqlite3_column_int(stmt, 0);
		this->stateIds.push_back(stateId);
	}
	if (rc != SQLITE_DONE)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		// fprintf("error: ", sqlite3_errmsg(db));
	}
	sqlite3_finalize(stmt);
}

void Election::acceptElectionDetails()
{

	int inputStateId;

	cout << "\n**************Accepting Election Details*************" << endl;
	int no;
	cout << "Enter Election Name" << endl;
	cin >> electionName;
	cout << "Enter No of States in which you want to conduct Election" << endl;
	cin >> no;
	
	for (int i = 0; i < no; i++)
	{
		char *zErrMsg = 0;
		int rc;
		const char *sql;
		const char *data = "Callback function called";
		stringstream ss, ss2;
		string query, query2;
		cout << "Enter State Id of State" << endl;
		cin >> inputStateId;
		stateIds.push_back(inputStateId);

		ss << "INSERT INTO election "
			  "VALUES (null,' "
		    <<electionCount<< "','" << electionName << "','" << inputStateId << "','" << 0 <<  "')";

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
			fprintf(stdout, "Election Record inserted successfully\n");
		}
	}
}

void Election::showElectionDetails()
{
	cout << "\n********Election Details***********" << endl;
	cout << "Election Name" << endl;
	cout << electionName;

	for (auto x : stateIds)
	{
		cout << x;
	}
}
string Election::getElectionName()
{
	return electionName;
}
vector<int> Election::getstateIds()
{
	return stateIds;
}

void Election::setElectionId(int no)
{
	electionId = no;
}
int Election::getElectionId()
{
	return electionId;
}
