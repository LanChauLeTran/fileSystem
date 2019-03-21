#ifndef GROUP_H
#define GROUP_H

#include <iostream>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

class Group{
    private:
        string name;

    public:
        Group();
        Group(const string& uName): name(uName) {}
        string getName() const {return name;}
};

#endif