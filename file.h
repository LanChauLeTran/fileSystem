#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <ctime>
#include "user.h"
#include "group.h"

using namespace std;

class File{
	private:
		string name;
		string permissions;
		string timeStamp;
		string user;
		string group;
		int fileSize;
	public:
		File();
		File(const string& fileName,  const string& user, const string& group);

		string getName() const;
		string getPerm() const;
		string getTime() const;
		string getUser() const;
		string getGroup() const;

		void setUser(const string& n);

		bool isOwner(const string& n) const;
		bool isGroup(const vector<Group>& g) const;

		int getSize() const;

		void updateTime();
		void setPerm(const string& newPerm);
};

#endif
