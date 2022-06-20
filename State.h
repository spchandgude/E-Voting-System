#pragma once

#include <map>
#include <vector>
#include "Assembly.h"

class State
{
private:
    int stateId;
    std::string stateName;
    map<std::string, int> assemblyNameAssemblyId;

public:
    map<int, Assembly *> assemblyList; //map of AssemblyId and Assembly *

    State(int, string,

          int);
    ~State();
    int getStateId();
    void setStateId(int);
    std::string getStateName();
    void addNewAssembly(Assembly &);
    map<std::string, int> getAssemblyNameAssemblyId();
    void setAssmblyNameAssemblyId(std::string, int);
    void showAssemblyDetails();
    void showAssemblyVoterList();
};
