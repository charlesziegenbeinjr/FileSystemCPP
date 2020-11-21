/*
Author: Jon Shidal
Purpose: CSE 332 lab 5
*/

#include "RemoveCommand.h"
#include<iostream>

using namespace std;

RemoveCommand::RemoveCommand(AbstractFileSystem* fileSys) : fs(fileSys) {}

// removes a file from the current working directory
int RemoveCommand::execute(std::string& CWD, std::string options) {
	string fullPath = CWD;
	fullPath += '/';
	fullPath += options;
	int result = fs->deleteFile(fullPath);
	if (result == AbstractFileSystem::filedoesnotexist) {
		cout << "File does not exist" << endl;
		return filedoesnotexist;
	}
	else if (result == AbstractFileSystem::fileisopen) {
		cout << "File is currently in use" << endl;
		return fileinuse;
	}
	return success;
}
void RemoveCommand::displayInfo() {
	cout << "touch removes a file from the file system and then deletes the file" << endl;
	cout << "Usage: rm <filename> [-r]" << endl;
}