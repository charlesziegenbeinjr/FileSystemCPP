/*
Author: Jon Shidal
Purpose: CSE 332 lab 5
*/

#include "LSCommand.h"
#include "ReadFileVisitor.h"
#include<iostream>

using namespace std;

LSCommand::LSCommand(AbstractFileSystem * fileSys) : fs(fileSys) {}

int LSCommand::execute(std::string& CWD, std::string options) {
	AbstractFile* f = fs->openFile(CWD);
	if (f == nullptr) { // file creation failed
		cout << "failed to open directory" << endl;
		return directorydoesnotexist;
	}

	ReadFileVisitor reader;
	// check if having the command option -l
	// if so, set the flag to true, otherwise, set to false
	if (options.compare("-l") == 0) {
		reader.set_dash_l(true);
	}
	else {
		reader.set_dash_l(false);
	}

	// print out all files of the directory using visitor
	f->accept(&reader);
	fs->closeFile(f);
	return success;
}
void LSCommand::displayInfo() {
	cout << "lists the contents of the current directory" << endl;
	cout << "Usage: ls [-l]" << endl;
}