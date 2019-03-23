#ifndef FOLDER_H
#define FOLDER_H

#include "file.h"
#include "user.h"
#include "group.h"

class Folder{
	private:
		Folder* parent;
		string name;
		string permissions;
		string timeStamp;
		string user;
		string group;
		int folderSize;
		vector<File> files;
		vector<Folder* > folders;
	
	public:
		Folder(const string& folderName, const string& u, const string& g);
		Folder(): parent(nullptr), name("home"), user("user"), group("Users") {}
		~Folder();

		string getName() const;
		string getPerm() const;
		string getTime() const;
		string getUser() const {return user;}
		string getGroup() const {return group;}

		bool isOwner(const string& n) const;

		int getSize() const;
		Folder* getParent();
		
		void setPerm(const string& newPerm);
		void updateTime();

		void touch(const string& fileName, const User& u);
		void mkdir(const string& dirName, const string& u, const string& g);
		void ls(const User& user) const;
		void lsl(const User& user) const;
		Folder* cd(const string& name, const User& u);
		void pwd();
		void rmdir(const string& dir);
		void rm(const string& target);
		void chmod(const string& obj, const string& perm);
		bool fileExists(const string& fileName) const;
		void xFile(const string& f, const User& u) const;
};

#endif