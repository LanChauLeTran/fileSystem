//PROGRAMMER: LANCHAU LETRAN

#include <sstream>
#include "folder.h"
#include <cctype>
#include <stdio.h> 
#include <stdlib.h> 
#include <algorithm>
#include <map>
#define BOLDBLUE "\033[1m\033[34m"  
#define BOLDGREEN   "\033[1m\033[32m"
#define RESET "\033[0m"

using namespace std;

int main(){
	map<string, Group> allGroups;
	map<string, User> allUsers;
	Group defaultG("users");

	allGroups.insert(pair<string, Group>(defaultG.getName(), defaultG));

	Folder root;
	Folder* curDir = &root;

	User base("user", defaultG);
	allUsers.insert(pair<string, User>(base.getName(), base));

	User* curUser = &(allUsers.find("user")->second);
	  

	string input;
	bool cont = true;
	vector<string> parsed;
	string token;


	while(cont){ //read in commands until exit or quit
		parsed.clear();
		cout << BOLDGREEN << curUser->getName() << RESET << ":"
			 << BOLDBLUE << curDir->getName() << RESET << "$ ";

		getline(cin, input);
		istringstream ss(input);
				 
		while(getline(ss, token, ' ')){
			parsed.push_back(token);
		}

		int inputSize = parsed.size();

		int i = 0;
		if(inputSize == 0){ //decision branching for each command
			//if no input, do nothing.
		}
		else if(parsed[i] == "ls"){
			if(inputSize == 1){
				curDir->ls();
			}
			else if(inputSize == 2 && parsed[1] == "-l"){
				curDir->lsl();
			}
			else{ //invalid inputs handling
				cout << "ls: no such file or directory" << endl;
			}
		}
		else if(parsed[i] == "cd"){
			if(inputSize == 2){ 
				//parse cd.. vs cd .. to run the correct command
				if(parsed[1] != ".." && parsed[1] != "../"){ 
					curDir = curDir->cd(parsed[1]);
				}
				else if(parsed[1] == ".." || parsed[1] == "../"){
					if(!(curDir->getName() == "/")){
						curDir = curDir->getParent();
					}
				}
			}
			else if(inputSize == 1){ //addition funcitonality
				//go to root if only cd is entered
				curDir = &root;
			}
		}
		else if(parsed[i] == "pwd"){
			curDir->pwd();
		}
		else if(parsed[i] == "rmdir"){
			if(inputSize < 2) { //error handling if not enough parameters
				cout << "rmdir: missing operand" << endl;
			}
			else if(inputSize > 2){
				cout << "rmdir: too many operands" << endl;
			}
			else{
				curDir->rmdir(parsed[1]);
			}
		}
		else if(parsed[i] == "mkdir"){
			if(inputSize < 2){
				cout << "mkdr: missing operand" << endl;
			}
			else if(inputSize > 2){
				cout << "mkdr: too many operands" << endl;
			}
			else{
				curDir->mkdir(parsed[1]);
			}
		}
		else if(parsed[i] == "rm"){
			if(inputSize < 2){
				cout << "rm: missing operand" << endl;
			}
			else if(inputSize > 2){
				cout << "rm: too many operands" << endl;
			}
			else{
				curDir->rm(parsed[1]);
			}
		}
		else if(parsed[i] == "chmod"){
			int permissionSize = parsed[1].size();
			if(inputSize != 3){ //error handling for not enough parameters
				cout << "chmod: missing operand" << endl;
			}
			else if(permissionSize != 3)
			{
				cout << "chmod: invalid code" << endl;
			}
			else if(inputSize == 3 && permissionSize == 3){
				curDir->chmod(parsed[2], parsed[1]);
			}
			else if(inputSize == 3 && parsed[1].size() > 3){
				cout << "chmod: invalid code: '" << parsed[1] << "'" << endl;
			}
		}
		else if(parsed[i] == "touch"){
			if(inputSize < 2){
				cout << "touch: missing file operand" << endl;
			}
			else{
				for(i = 1; i < inputSize; i++){
					curDir->touch(parsed[i]);
				}
			}
		}
		else if(parsed[i] == "groupadd"){
			if(inputSize != 2){
				cout << "Usage: groupadd [group]" << endl;
			}
			else if(inputSize == 2 && 
					allGroups.find(parsed[1]) == allGroups.end()){
				string gName = parsed[i+1];
				Group newGroup(gName);
				allGroups.insert(pair<string, Group>(gName, newGroup));
			}
			else{
				cout << "groupadd: group '" << parsed[1] 
					 << "' already exists" << endl;
			}
		}
		else if(parsed[i] == "groups"){
			if(inputSize == 1){
				curUser->printGroups();
			}
			else if(inputSize == 2){
				if(allUsers.find(parsed[1]) == allUsers.end()){
					cout << "groups: '" << parsed[1] 
						 << "': does not exist" << endl;
				}
				else{
					allUsers.find(parsed[1])->second.printGroups();
				}
			}
		}
		else if(parsed[i] == "whoami"){
			if(inputSize == 1){
				cout << curUser->getName() << endl;
			}
			else{
				cout << "whoami: extra operands" << endl;
			}
		}
		else if(parsed[i] == "useradd"){
			if(inputSize != 2){
				cout << "Usage: useradd [username]" << endl;
			}
			else if (inputSize == 2 && 
					 allUsers.find(parsed[1]) == allUsers.end()){
				string uName = parsed[i+1];
				User newUser(uName, defaultG);
				allUsers.insert(pair<string, User>(uName, newUser));
			}
			else{
				cout << "useradd: user '" << parsed[1]
					 << "' already exist" << endl;
			}
		}
		else if(parsed[i] == "userdel"){
			if(inputSize != 2){
				cout << "Usage: userdel [username]" << endl;
			}
			else if (inputSize == 2 && 
					 allUsers.find(parsed[1]) != allUsers.end()){
				if(curUser->getName() == parsed[1]){
					cout << "userdel: cannot remove current user" << endl;
				}
				else{
					allUsers.erase(parsed[1]);
				}
			}
			else if (inputSize == 2 &&
					 allUsers.find(parsed[1]) == allUsers.end()){
				cout << "userdel: user '" << parsed[1]
					 << "' does not exist" << endl;
			}
		}
		else if(parsed[i] == "switchto"){
			if(inputSize != 2){
				cout << "Usage: switchto [username]" << endl;
			}
			else if(inputSize == 2 &&
					allUsers.find(parsed[1]) != allUsers.end()){
				curUser = &(allUsers.find(parsed[1])->second);
			}
			else if(inputSize == 2 &&
					allUsers.find(parsed[1]) == allUsers.end()){
				cout << "switchto: user '" << parsed[1]
					 << "' does not exist" << endl;
			}
		}
		else if(parsed[i] == "usermod"){
			if(inputSize != 4){
				cout << "Usage: usermod -a -G [group]" << endl;
			}
			else if(inputSize == 4 && parsed[1] == "-a" && parsed[2] == "-G" && 
					allGroups.find(parsed[3]) == allGroups.end()){
				cout << "usermod: group '" << parsed[3]
					 << "' does not exist" << endl;
			}
			else if(inputSize == 4 && parsed[1] == "-a" && parsed[2] == "-G" && 
					allGroups.find(parsed[3]) != allGroups.end()){
				if(!curUser->groupExists(parsed[3])){
					curUser->addGroup(parsed[3]);
				}
				else if(curUser->groupExists(parsed[3])){
					cout << "usermod: " << curUser->getName()
						 << " already in '"
						 << parsed[3] << "' " << endl;
				}
			}
			else if(inputSize == 4){
				cout << "Usage: usermod -a -G [group]" << endl;
			}
		}
		else if(input == "exit" || input == "quit"){
			cont = false;
		}
		else{
			cout << parsed[i] << ": command not found" << endl;
		}
	}
	
	return 0;
}
