#ifndef USER_H
#define USER_H

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include "group.h"

using namespace std;

class User{
    private:
        string name;
        vector<Group> groups;

    public:
        User();
        User(const string& uName, const Group& gName);
        string getName() {return name;}
        void printGroups() const;
};

#endif