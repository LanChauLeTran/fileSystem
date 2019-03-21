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
        cout << i.getName() << '\t';
    }
    cout << endl;
}