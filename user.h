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
        void removeGroup(const string& g);
        bool groupExists(const string& g) const;
};

#endif

/*
else if(parsed[i] == "userdel"){
    if(inputSize != 4){
        cout << "HELLO" << endl;
        cout << "Usage: userdel -G [group] [username]" << endl;
    }
    else if(inputSize == 4 && parsed[1] == "-G" &&
            allUsers.find(parsed[3]) != allUsers.end()){
        if(allUsers.find(parsed[3])->second.groupExists(parsed[2])){

        }
        else{
            cout << "userdel: group '" << parsed[2] 
                    << "' does not have user '" << parsed[3]
                    << "'" << endl;
        }
    }
    else if(inputSize == 4){
        cout << "Usage: userdel -G [group] [username]" << endl;
    }
}*/