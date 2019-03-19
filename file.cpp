#include "file.h"

File::File(const string& fileName){
	permissions = "rwxrwxrwx";
	time_t now = time(0);
	char* temp = ctime(&now);
	temp[strlen(temp)-1] = '\0';
	timeStamp = temp;

	name = fileName;
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