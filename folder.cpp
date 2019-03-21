#include "folder.h"
#include <algorithm>
#include <map>
#define BOLDBLUE "\033[1m\033[34m"  
#define RESET "\033[0m"

Folder::Folder(const string& folderName){
	name = folderName;
	permissions = "rwxrwxrwx";
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

//param: string for file name
//Make sure name doesnt already exist as either a file or fodler
//before creating new file.
//if it exist, update timestamp, else create a new object file
void Folder:: touch(const string& name){
	bool fileExist = false;
	bool folderExist = false;
	for (auto &i: files){
		if(i.getName() == name){
			fileExist = true;
			folderExist = true;
			i.updateTime();
		}
	}
	for (const auto i: folders){
		if(i->getName() == name){
			folderExist = true;
			fileExist = true;
			i->updateTime();
		}
	}
	if(!fileExist){
		folderExist = true;
		files.push_back(File(name));
	}
	if(!folderExist)
	{
		this->mkdir(name);
	}
}

//param: string for directory name
//Make sure name doesnt already exist as either a file or folder
//if exist, let user know, else create new object folder
void Folder::mkdir(const string& dirName){
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
void Folder::ls() const{
	for (auto i: files){
		cout << i.getName() << '\t';
	}
	for (auto i: folders){
		cout << BOLDBLUE << i->getName() << RESET << '\t';
	}
	cout << endl;
}

//run through vector of files then folders
//print member variables via getters
void Folder::lsl() const{
	for (auto i: files){
		cout << "-" << i.getPerm() 
			 << '\t' << i.getSize()
			 << '\t' << i.getTime()
			 << '\t' << i.getName()
			 << endl;
	}
	for (auto i: folders){
		cout << "d" << i->getPerm()
			 << '\t' << i->getSize()
			 << '\t' << i->getTime()
			 << '\t' << BOLDBLUE << i->getName() << RESET 
			 << endl;
	}
}

//param: string for for the name of the folder trying to reach
//check that the folder exist , if it does, return the pointer
//to that folder
Folder* Folder::cd(const string& name){
	bool exist = false;
	Folder* found;
	for (auto i: folders){
		if(i->getName() == name){
			exist = true;
			found = i;
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
void Folder::rm(const string& target){
	bool exist = false;
	for (unsigned int i = 0; i < files.size(); i++){
		if(files[i].getName() == target){
			files.erase(files.begin() + i);
			exist = true;
		}
	}
	if(!exist){
		cout << "rm: cannot remove '" << target << "': No such file" << endl;
	}
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
		files[indexOfFile].updateTime();
	}
	else if(folderExist){
		folders[indexOfFolder]->setPerm(newPerm);
		folders[indexOfFolder]->updateTime();
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