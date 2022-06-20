//file contains voter functions
#include <iostream>
#include "Voter.h"
#include "ElectionCommision.h"
#include "sqlite3.h"
#include <sstream>
using namespace std;

extern ElectionCommision e;
extern sqlite3 *db;
extern int voterCount;

int callback(void *data, int argc, char **argv, char **azColName);

Voter::Voter() {}
Voter ::Voter(int voterId, int assemblyId, int stateId, int personId, std::string username, std::string password, int verified)
{
    char *zErrMsg = 0;
    int rc;
    const char *sql;
    const char *data = "Callback function called";
    int callback(void *data, int argc, char **argv, char **azColName);
    voterCount = voterId + 1;
    this->stateId = stateId;
    this->assemblyId = assemblyId;
    this->voterId = voterId;
    this->username = username;
    this->password = password;
    this->verified = verified;

    stringstream ss, ss2;
    string query, query2;

    sqlite3_stmt *stmt;

    ss << "select * from person where personid = " << personId;
    query = ss.str();
    sql = query.c_str();

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        // fprintf("error: ", sqlite3_errmsg(sqlite3->db));
    }

    Person *personObj;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int personid = sqlite3_column_int(stmt, 0);
        string personname = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
        string gender = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
        int age = sqlite3_column_int(stmt, 3);
        string dob = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)));
        string address = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5)));
        string qualification = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6)));
        int aadharid = sqlite3_column_int(stmt, 7);

        personObj = new Person(personid, personname, age, gender, qualification, dob, address, aadharid);
        personalDetails.first = personObj->getPersonId();
        personalDetails.second = personObj;
        //e.getMapStateNameStateId()[stateName]->assemblyNameAssemblyId[stateObj->getName()] = i;
    }
    sqlite3_finalize(stmt);
}

int Voter::getVoterId()
{
    return voterId;
}

pair<int, Person *> Voter::getVoterPersonPair()
{
    return personalDetails;
}

void Voter::setVoterPersonPair(int voterid, Person *p)
{
    personalDetails.first = voterid;
    personalDetails.second = p;
}

void Voter::vote()
{
    int voteNum;
    cout << "\n********Select a Candidate to Vote :**********" << endl;
    cout << "Candidate Id.\tName OF Candidate\tParty Name " << endl;
    int stateId = this->stateId;
    int assemblyId = this->assemblyId;
    for (auto x : e.states[stateId]->assemblyList[assemblyId]->getCandidateList())
    {
        cout << x.first << "\t\t" << x.second->getCandidatePersonPair().second->getName() << "\t\t\t"
             << "Congress" << endl;
    }
    cin >> voteNum;

    (*(e.states[stateId]->assemblyList[assemblyId]->getCandidateVotesList()))[voteNum]++;

    char *zErrMsg = 0;
    int rc;
    const char *sql;
    const char *data = "Callback function called";
    stringstream ss, ss2;
    string query, query2;

    ss2 << "update candidate set votes = votes + 1 where candidateid= " << voteNum;
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
        fprintf(stdout, "Vote Added successfully \t\tPleast Collect Ur Receipt \n");
    }

    e.states[stateId]->assemblyList[assemblyId]->showCandidateVotes();
}

void Voter::displayVoterDetails()
{
    cout << "\n**************Voter Details **************" << endl;
    cout << "Voter ID:" << voterId << endl;
    personalDetails.second->displayPersonalDetails();
}

void Voter::setStateId(int stateId)
{
    this->stateId = stateId;
}

int Voter::getStateId()
{
    return stateId;
}

void Voter::setAssemblyId(int assemblyId)
{
    this->assemblyId = assemblyId;
}

int Voter::getAssemblyId()
{
    return assemblyId;
}

std::string Voter::getUSername()
{
    return username;
}
void Voter::setUsername(std::string username)
{
    this->username = username;
}

std::string Voter::getPassword()
{
    return password;
}
void Voter::setPassword(std::string password)
{
    this->password = password;
}
void Voter::registerAsCandidate()
{
    e.registerNewCandidate(this->personalDetails.second);
}

Voter::~Voter()
{

    delete personalDetails.second;
}