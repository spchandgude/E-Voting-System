//file contains candidate details
#include <iostream>
#include "ElectionCommision.h"
#include "Candidate.h"
#include "sqlite3.h"
#include <sstream>
extern ElectionCommision e;
extern sqlite3 *db;
extern int candidateCount;
Candidate::Candidate(int a, int b, int c, int d, int e, std::string f, std::string username, std::string password)
{
    char *zErrMsg = 0;
    int rc;
    const char *sql;
    const char *data = "Callback function called";
    int callback(void *data, int argc, char **argv, char **azColName);
    candidateCount = candidateId + 1;
    candidateId = a;
    assemblyId = b;
    stateId = c;
    electionId = d;
    personId = e;
    partyName = f;
    this->username = username;
    this->password = password;

    candidateCount = a + 1;
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
void Candidate::regist()
{
}

pair<int, Person *> Candidate::getCandidatePersonPair()
{
    return personalDetails;
}
void Candidate::setCandidatePersonPair(int candidateid, Person *p)
{
    personalDetails.first = candidateid;
    personalDetails.second = p;
}

void Candidate::displayCandidateDetails()
{
    cout << "\n*************Candidate Details**************" << endl;
    cout << "Candidate ID:" << candidateId << endl;
    personalDetails.second->displayPersonalDetails();
}

int Candidate::getStateId()

{

    return this->stateId;
}

int Candidate ::getCandidateId()
{
    return candidateId;
}

void Candidate::setStateId(int stateId)

{

    this->stateId = stateId;
}

int Candidate::getAssemblyId()

{

    return this->assemblyId;
}

std::string Candidate::getPartyName()
{
    return this->partyName;
}

/*


bool Candidate::setAssemblyId(int asmId)

{

    if(assemblyId[0] == -1){

        assemblyId[0] = asmId;

    }else if(assemblyId[1] == -1){

        assemblyId[1] = asmId;

    }else{

        return false;

    }

    return true;

}

*/

std::string Candidate::getUSername()
{
    return username;
}
void Candidate::setUsername(std::string username)
{
    this->username = username;
}

std::string Candidate::getPassword()
{
    return password;
}
void Candidate::setPassword(std::string password)
{
    this->password = password;
}
void Candidate::setAssemblyId(int p)
{

    assemblyId = p;
}

int Candidate::getElectionId()

{

    return this->electionId;
}

void Candidate::setElectionId(int electionId)

{

    this->electionId = electionId;
}

Candidate::~Candidate()
{
    delete personalDetails.second;
}
