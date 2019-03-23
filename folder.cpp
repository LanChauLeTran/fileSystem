#include "folder.h"
#include <algorithm>
#include <map>
#define BOLDBLUE "\033[1m\033[34m"  
#define RESET "\033[0m"

Folder::Folder(const string& folderName, const string& u, const string& g){
	permissions = "rwxrwxrwx";
	name = folderName;
	user = u;
	group = g;
	parent = this;
	time_t now = time(0);
	char* temp = ctime(&now);
	temp[strlen(temp)-1] = '\0';
	timeStamp = temp;
	folderSize = 1024;
}

Folder::~Folder(){
	for (auto i: folders){
		delete i;
	}
}

string Folder::getName() const{
	return name;
}

string Folder::getPerm() const{
	return permissions;
}

string Folder::getTime() const{
	return timeStamp;
}

int Folder::getSize() const{
	return folderSize;
}

Folder* Folder::getParent(){
	if(parent != nullptr){
		return parent;
	}
	else{
		return nullptr;
	}
}

void Folder::xFile(const string& f, const User& u) const{
	if(fileExists(f)){
		for(auto& i: files){
			if(i.getName() == f){
				if( (i.getPerm()[8] == 'x') ||
					(i.getPerm()[5] == 'x' && u.groupExists(i.getGroup())) ||
					(i.getPerm()[2] == 'x' && i.isOwner(u.getName())) ){
					cout << "bash: file '" << f << "' ran/executed" << endl;
				}
				else{
					cout << "bash: permission denied" << endl;
				}
			}
		}
	}
	else{
		cout << "bash: ./" << f
				<< ": no such file or directory" << endl;
	}
}

//param: string for file name
//Make sure name doesnt already exist as either a file or fodler
//before creating new file.
//if it exist, update timestamp, else create a new object file
void Folder:: touch(const string& name, const User& u){
	bool fileExist = false;
	bool folderExist = false;
	for (auto &i: files){
		if(i.getName() == name){
			fileExist = true;
			folderExist = true;
			if( (i.getPerm()[7] == 'w') ||
				(i.getPerm()[4] == 'w' && u.groupExists(i.getGroup())) ||
				(i.getPerm()[1] == 'w' && i.isOwner(u.getName())) ){
				i.updateTime();
			}
			else{
				cout << "touch: permission denied" << endl;
			}
		}
	}
	for (const auto i: folders){
		if(i->getName() == name){
			folderExist = true;
			fileExist = true;
			if( (i->getPerm()[7] == 'w') ||
				(i->getPerm()[4] == 'w' && u.groupExists(i->getGroup())) ||
				(i->getPerm()[1] == 'w' && i->isOwner(u.getName())) ){
				i->updateTime();
			}
			else{
				cout << "touch: permission denied" << endl;
			}
		}
	}
	if(!fileExist){
		folderExist = true;
		files.push_back(File(name, u.getName(), u.topGroup()));
	}
	if(!folderExist)
	{
		this->mkdir(name, u.getName(), u.topGroup());
	}
}

//param: string for directory name
//Make sure name doesnt already exist as either a file or folder
//if exist, let user know, else create new object folder
void Folder::mkdir(const string& dirName, const string&u, const string& g){
	bool exist = false;
	for (const auto i: folders){
		if(i->getName() == dirName){
			exist = true;
		}
	}
	for (const auto i: files){
		if(i.getName() == dirName){
			exist = true;
		}
	}
	if(exist){
		cout << "mkdir: '" << dirName << "' already exists." << endl;
	}
	else{
		auto newFolder = new Folder();
		newFolder->parent = this;
		newFolder->name = dirName;
		newFolder->user = u;
		newFolder->group = g;
		newFolder->permissions = "rwxrwxrwx";
		time_t now = time(0);
		char* temp = ctime(&now);
		temp[strlen(temp)-1] = '\0';
		newFolder->timeStamp = temp;
		newFolder->folderSize = 1024;
		folders.push_back(newFolder);
	}
}

//no params
//walk thorugh vector of files and output their names
void Folder::ls(const User& u) const{
	for (auto& i: files){
		if( (i.getPerm()[3] == 'r' && u.groupExists(i.getGroup())) ||
			(i.isOwner(u.getName()) && i.getPerm()[0] == 'r') ||
			(i.getPerm()[6] == 'r') ){
			cout << i.getName() << '\t';
		}
	}
	for (auto& i: folders){
		if( (i->permissions[6] == 'r') ||
			(i->permissions[3] == 'r' && u.groupExists(i->getGroup())) ||
			(i->permissions[0] == 'r' && isOwner(u.getName())) ){
			cout << BOLDBLUE << i->getName() << RESET << '\t';
		}
	}
	cout << endl;
}

bool Folder::isOwner(const string& n) const{
	if(n == user){
		return true;
	}
	else{
		return false;
	}
}

//run through vector of files then folders
//print member variables via getters
void Folder::lsl(const User& u) const{
	for (auto& i: files){
		if( (i.getPerm()[3] == 'r' && u.groupExists(i.getGroup())) ||
			(i.isOwner(u.getName()) && i.getPerm()[0] == 'r') ||
			(i.getPerm()[6] == 'r') ){
			cout << "-" << i.getPerm() 
				<< '\t' << i.getUser()
				<< "\t\t" << i.getGroup()
				<< "\t\t" << i.getSize()
				<< '\t' << i.getTime()
				<< '\t' << i.getName()
				<< endl;
		}
	}
	for (auto& i: folders){
		if( (i->permissions[6] == 'r') ||
			(i->permissions[3] == 'r' && u.groupExists(i->getGroup())) ||
			(i->permissions[0] == 'r' && isOwner(u.getName())) ){
			cout << "d" << i->getPerm()
				<< '\t' << i->getUser()
				<< "\t\t" << i->getGroup()
				<< "\t\t" << i->getSize()
				<< '\t' << i->getTime()
				<< '\t' << BOLDBLUE << i->getName() << RESET 
				<< endl;
		}
	}
}

//param: string for for the name of the folder trying to reach
//check that the folder exist , if it does, return the pointer
//to that folder
Folder* Folder::cd(const string& name, const User& u){
	bool exist = false;
	Folder* found;
	for (auto& i: folders){
		if(i->getName() == name){
			exist = true;
			if( (i->getPerm()[8] == 'x') ||
				(i->getPerm()[5] == 'x' && u.groupExists(i->getGroup())) ||
				(i->getPerm()[2] == 'x' && isOwner(u.getName())) ){
				found = i;
			}
			else{
				cout << "cd: permission denied" << endl;
				return this;
			}
		}
	}
	if(exist){
		return found;
	}
	else{
		cout << "bash: cd: " << name << ": no such directory" << endl;
		return this;
	}
}

//check that parent member variabl is not null
//if it is not, add the path name to the string
void Folder::pwd(){
	string path = "";
	Folder* cur = this;
	while(cur->parent != nullptr){
		path = cur->name + "/" +  path;
		cur = cur->parent;
	}
	path = "/" + path;
	cout << path << endl;
}

//param: string with directory name to delete
//walk folder vector to make sure the folder exists
//if does not exist, print error message
//if exist, delete pointer and  erase folder 
void Folder::rmdir(const string& dir){
	bool exist = false;
	for (unsigned int i = 0; i < folders.size(); i++){
		if(folders[i]->getName() == dir){
			exist = true;
			delete folders[i];
			folders.erase(folders.begin() + i);
		}
	}
	if(!exist){
		cout << "rmdir: directory does not exist" << endl;
	}
}

//param: string with  file name to delete
//walk file vector to make sure the file exist
//if does not exist, print error message
//if exist delete file object
void Folder::rm(const string& target, const User& u){
	if(fileExists(target)){
		for(unsigned int i = 0; i < files.size(); i++) {
			if(files[i].getName() == target){
				if( (files[i].getPerm()[7] == 'w') ||
					(files[i].getPerm()[4] == 'w' &&  
					 u.groupExists(files[i].getGroup())) ||
					(files[i].getPerm()[1] == 'w' && 
					 files[i].isOwner(u.getName())) ){
					files.erase(files.begin() +i);
				}
				else{
					cout << "rm: permission denied" << endl;
				}
			}
		}
	}
	else{
		cout << "rm: cannot remove '" << target << "': No such file" << endl;
	}

	/*
	bool exist = false;
	for (unsigned int i = 0; i < files.size(); i++){
		if(files[i].getName() == target){
			files.erase(files.begin() + i);
			exist = true;
		}
	}
	if(!exist){
		cout << "rm: cannot remove '" << target << "': No such file" << endl;
	}*/
}

//param: string for the object to change permission
//param: string for the permission code
//make sure permission code is valid (3 digit number < 777)
//make sure the object to change permission for exist
//either a file or folder
//set new permission and update time if exist.
void Folder::chmod(const string& obj, const string& perm){
	map<string, string> permValues;
	permValues.insert(pair<string, string>("0", "---"));
	permValues.insert(pair<string, string>("1", "--x"));
	permValues.insert(pair<string, string>("2", "-w-"));
	permValues.insert(pair<string, string>("3", "-wx"));
	permValues.insert(pair<string, string>("4", "r--"));
	permValues.insert(pair<string, string>("5", "r-x"));
	permValues.insert(pair<string, string>("6", "rw-"));
	permValues.insert(pair<string, string>("7", "rwx"));	

	string newPerm = "";
	bool fileExist = false;
	bool folderExist = false;
	int indexOfFile;
	int indexOfFolder;

	for (unsigned int i = 0; i < perm.size(); i++){
		if(perm[i] < '0' || perm[i] > '7'){
			cout << "chmod: invalid mode: " << perm << endl;
			return;
		}
		else{
			string find = "";
			find = perm[i];
			newPerm = newPerm + permValues.find(find)->second;
		}
	}

	for (unsigned int i =  0; i < files.size(); i++){
		if(files[i].getName() == obj){
			fileExist = true;
			indexOfFile = i;
		}
	}
	for (unsigned int i = 0; i < folders.size(); i++){
		if(folders[i]->getName() == obj){
			folderExist = true;
			indexOfFolder = i;
		}
	}

	if(!fileExist && !folderExist){
		cout << "chmod: invalid mode: " << obj << endl;
		return;
	}
	else if(fileExist){
		files[indexOfFile].setPerm(newPerm);
		//files[indexOfFile].updateTime();
	}
	else if(folderExist){
		folders[indexOfFolder]->setPerm(newPerm);
		//folders[indexOfFolder]->updateTime();
	}
}

void Folder::setPerm(const string& newPerm){
	permissions = newPerm;
}

void Folder::updateTime(){
	time_t now = time(0);
	char* temp = ctime(&now);
	temp[strlen(temp)-1] = '\0';
	timeStamp = temp;
}

bool Folder::fileExists(const string& name) const{
	for(auto& i: files){
		if(i.getName() == name){
			return true;
		}
	}
	return false;
}