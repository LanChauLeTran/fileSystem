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

void User::removeGroup(const string& g)
{
    if(groups.size() == 1){
        cout << "user must have at least one group" << endl;
        return;
    }
    for(auto& i: groups)
    {
        if(i.getName() == g)
        {
            int index = (&i - &groups[0]);
            groups.erase(groups.begin() + index);
        }
    }
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

string User::topGroup() const
{
    return groups[0].getName();
}