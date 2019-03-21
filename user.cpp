#include "user.h"

User::User(const string& uName, const Group& gName)
{
    name = uName;
    Group nGroup(gName);
    groups.push_back(nGroup);
}

void User::printGroups() const
{
    for(const auto i: groups)
    {
        cout << i.getName() << ' ';
    }
    cout << endl;
}

void User::addGroup(const string& n)
{
    Group g(n);
    groups.push_back(g);
}

bool User::groupExists(const string& g) const
{
    for(auto &i: groups)
    {
        if(i.getName() == g){
            return true;
        }
    }
    return false;
}