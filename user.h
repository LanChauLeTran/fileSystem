#ifndef USER_H
#define USER_H

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <list>
#include "group.h"

using namespace std;

class User{
    private:
        string name;
        vector<Group> groups;

    public:
        User();
        User(const string& uName, const Group& gName);
        string getName() const {return name;} 
        void printGroups() const;
        void addGroup(const string& n);
        bool groupExists(const string& g) const;
};

#endif