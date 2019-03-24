#include "file.h"

File::File(const string& fileName, const string& u, const string& g){
	permissions = "rw-r--r--";
	time_t now = time(0);
	char* temp = ctime(&now);
	temp[strlen(temp)-1] = '\0';
	timeStamp = temp;

	name = fileName;
	user = u;
	group = g;
	fileSize = 1024;
}

string File::getName() const{
	return name;
}

string File::getPerm() const{
	return permissions;
}

string File::getTime() const{
	return timeStamp;
}

string File::getUser() const{
	return user;
}

string File::getGroup() const{
	return group;
}

bool File::isOwner(const string& n) const{
	if(n == user){
		return true;
	}
	else
	{
		return false;
	}
	
}

bool File::isGroup(const vector<Group>& g) const{
	for(auto& i: g){
		if(group == i.getName()){
			return true;
		}
	}
	return false;
}

int File::getSize() const{
	return fileSize;
}

void File::updateTime(){
	time_t now = time(0);
	char* temp = ctime(&now);
	temp[strlen(temp)-1] = '\0';
	timeStamp = temp;
}

void File::setPerm(const string& newPerm){
	permissions = newPerm;
}