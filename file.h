#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

class File{
	private:
		string name;
		string permissions;
		string timeStamp;
		int fileSize;
	public:
		File();
		File(const string& fileName);

		string getName() const;
		string getPerm() const;
		string getTime() const;
		int getSize() const;

		void updateTime();
		void setPerm(const string& newPerm);
};

#endif
