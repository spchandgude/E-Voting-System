//main driver file
#include <iostream>
#include "ElectionCommision.h"
#include "sqlite3.h"
#include "Voter.h"
#include <sstream>
#include <thread>
#include <mutex>
using namespace std;

sqlite3 *db;

int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char *)data);

    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}
void adminFuntions();
void voterFunctions();
void candidateFunctions();

Voter *voterLogin(string username, string password);
Candidate *candidateLogin(string username, string password);

int rc = sqlite3_open("test.db", &db);
ElectionCommision e;
std::mutex m;
void threadFun()
{
    m.lock();
    cout << "Intialized thread For Intializing Maps" << this_thread::get_id() << endl;
    char *zErrMsg = 0;
    std::string sq;
    char *sql;
    char ans; // Used during do while for menu.
    const char *data = "Callback function called";

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }
    sqlite3_stmt *stmt;

    sq = "SELECT * FROM state";
    sql = (char *)sq.c_str();

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        // fprintf("error: ", sqlite3_errmsg(sqlite3->db));
    }
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int stateId = sqlite3_column_int(stmt, 0);
        string name = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
        int asmId = sqlite3_column_int(stmt, 2);
        //cout << "StateId" << stateId << endl;
        //cout << "Statename" << name << endl;
        //cout << "AssemblydID" << asmId << endl;
        e.addNewState(stateId, name, asmId);
    }
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
    }
    sqlite3_finalize(stmt);
    cout << "Stopped thread! All Maps are Intialized" << this_thread::get_id() << endl;
    m.unlock();
}

int main()
{

    thread t1(threadFun); //created thread for intializing maps
    // thread t2(threadFun); //thread to show mutex lock

    char ans; // Used during do while for menu.

    // cout << "Intialized thread For Main Menu " << this_thread::get_id() << endl; //thread for main menu

    int ch;

    cout << "\n\n************************Online Voting System*******************************" << endl;
    t1.join();
    // t2.join();

    do
    {
        cout << "\nWho are you ?\n1.Admin\n2.Voter\n3.Candidate\n"
             << endl;
        cin >> ch;

        switch (ch)
        {
        case 1:
            adminFuntions();
            break;
        case 2:
            voterFunctions();
            break;
        case 3:
            candidateFunctions();
            break;
        case 4:
            return 0;
        default:
            break;
        }
        cout << "Do You Want To Continue ? ";
        cin >> ans;
    } while (ans == 'y' || ans == 'Y');
    return 0;
}

void adminFuntions()
{
    int ch;
    cout << "\n1.Create Election \n2.Show Election Details\n3 Show All States Details\n4.End Election and find Winner\n 5.Return to previous menu" << endl;
    cin >> ch;
    switch (ch)
    {
    case 1:
        e.createElection();
        break;
    case 2:
        e.showOngoingElectionDetails();
        break;
    case 3:
        e.showStates();
        break;
    case 4:
        return e.endElection();
        break;
    default:
        cout << "\nInvalid Choice" << endl;
        break;
    }
}

void voterFunctions()
{
    int ch, ch2;
    cout << "\n1.New Voter \n2.Already Registered\n3.Return To Previous Menu" << endl;
    cin >> ch;
    Voter *v1;
    string username, password;
    switch (ch)
    {
    case 1:
        v1 = e.registerNewVoter();
        break;
    case 2:
        cout << "\nEnter Username" << endl;
        cin >> username;
        cout << "\nEnter Password" << endl;
        cin >> password;
        v1 = voterLogin(username, password);
        if (v1 != NULL)
        {
            cout << "**************" << endl;
            cout << "\n1.Vote For Election\n2.Register as New Candidate" << endl;
            cin >> ch2;
            switch (ch2)
            {
            case 1:
                v1->vote();
                break;
            case 2:
                v1->registerAsCandidate();
                break;
            default:
                cout << "Invalid Choice" << endl;
            }
        }
        else
        {
            cout << "\nInvalid Credentials" << endl;
        }

        break;
    case 3:
        return;
    default:
        cout << "\nInvalid Choice" << endl;
        break;
    }
}

void candidateFunctions()
{

    Candidate *c;
    std::string username;
    std::string password;

    cout << "\nEnter Username" << endl;
    cin >> username;
    cout << "\nEnter Password" << endl;
    cin >> password;
    c = candidateLogin(username, password);
    if (c != NULL)
    {
        std::cout << "\n**********Candidate Details************" << std::endl;
        std::cout << "Welcome !\nThese Are Your Details :-" << std::endl;
        std::cout << "Role\t\t: Candidate" << std::endl;
        std::cout << "Candidate Id \t:   " << c->getCandidateId() << std::endl;
        std::cout << "Assembly Id \t:   " << c->getAssemblyId() << std::endl;
        std::cout << "State Id \t:   " << c->getStateId() << std::endl;
        std::cout << "Election ID \t:   " << c->getElectionId() << std::endl;
        std::cout << "Party Name \t:   " << c->getPartyName() << std::endl;
        //std::cout<<"Election \t:   "<<(e.getElectionsList()[c->getElectionId()])->getElectionName()<<std::endl;
        //Implement getElectionsList() for this.
        std::cout << "****************************************\n"
                  << std::endl;
    }
}

Voter *voterLogin(string username, string password)
{
    char *zErrMsg = 0;
    const char *sql;
    int rc;
    const char *data = "Callback function called";
    int callback(void *data, int argc, char **argv, char **azColName);

    stringstream ss, ss2;
    string query, query2;

    sqlite3_stmt *stmt;

    // ss2 << "select * from voter where username =   " << username;
    ss2 << "select * from voter where username = 'saurabh' and password = 'pass'";

    query = ss2.str();

    std::string sql2 = "select * from voter where username ='" + username + "' and password = '" + password + "'";
    sql = sql2.c_str();
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        // fprintf("error: ", sqlite3_errmsg(sqlite3->db));
    }
    Voter *voterObj = NULL;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int voterid = sqlite3_column_int(stmt, 0);
        int assemblyid = sqlite3_column_int(stmt, 3);
        int stateid = sqlite3_column_int(stmt, 4);
        cout << "Voter id and assembly id" << voterid;
        voterObj = e.states[stateid]->assemblyList[assemblyid]->getVoterList()[voterid];

        //e.getMapStateNameStateId()[stateName]->assemblyNameAssemblyId[stateObj->getName()] = i;
    }

    sqlite3_finalize(stmt);

    return voterObj;
}

Candidate *candidateLogin(string username, string password)
{
    char *zErrMsg = 0;
    const char *sql;
    int rc;
    const char *data = "Callback function called";
    int callback(void *data, int argc, char **argv, char **azColName);

    stringstream ss, ss2;
    string query, query2;

    sqlite3_stmt *stmt;

    // ss2 << "select * from voter where username =   " << username;
    //ss2 << "select * from candidate where username = 'saurabh' and password = 'pass'";

    //query = ss2.str();

    std::string sql2 = "select * from candidate where username ='" + username + "' and password = '" + password + "'";
    sql = sql2.c_str();
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        // fprintf("error: ", sqlite3_errmsg(sqlite3->db));
    }
    Candidate *candidateObj = NULL;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        std::cout << "Inside While" << std::endl;
        int candidateid = sqlite3_column_int(stmt, 0);
        int assemblyid = sqlite3_column_int(stmt, 3);
        int stateid = sqlite3_column_int(stmt, 4);
        //cout << "Voter id and assembly id" << candidateid;
        candidateObj = e.states[stateid]->assemblyList[assemblyid]->getCandidateList()[candidateid];
    }

    sqlite3_finalize(stmt);
    std::cout << "After while" << std::endl;
    return candidateObj;
}
