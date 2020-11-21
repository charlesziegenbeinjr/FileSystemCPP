#include "DisplayCommand.h"
#include "ReadFileVisitor.h"
#include "DirectoryFile.h"
#include <iostream>

using namespace std;

DisplayCommand::DisplayCommand(AbstractFileSystem* fileSys) : fs(fileSys) {}


int DisplayCommand::execute(std::string& CWD, std::string options) {
	string fullPath = CWD;
	fullPath += '/';
	fullPath += options;
	AbstractFile* file = fs->openFile(fullPath);

	// check if the file doesn't exist
	if (file == nullptr) {
		cout << "File does not exist" << endl;
		return filedoesnotexist;
	}

	// displaying a directory file is not allowed
	if (dynamic_cast<DirectoryFile*>(file) != nullptr) {
		cout << "cannot display a directory file" << endl;
		fs->closeFile(file);
		return directorycannotbedisplayed;
	}

	ReadFileVisitor reader;
	size_t endIndex = fullPath.find_first_of(' ', 0);

	// extract the command options, if there is -d involved, set the flag to true, otherwise, set to false
	if (endIndex != string::npos) {
		string d = fullPath.substr(endIndex + 1, string::npos);
		if (d.compare("-d") == 0) {
			reader.set_dash_d(true);
		}
		else {
			reader.set_dash_d(false);
		}
	}
	else {
		reader.set_dash_d(false);
	}

	// use the visitor to print out the content of a file
	file->accept(&reader);

	fs->closeFile(file);

	return success;
}
void DisplayCommand::displayInfo() {
	cout << "Display contents of the file" << endl;
	cout << "Usage: ds <filename> [-d]" << endl;
}
