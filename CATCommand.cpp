#include "CATCommand.h"
#include "ProxyPermissionFile.h"
#include "ReadFileVisitor.h"
#include "DirectoryFile.h"
#include <iostream>

using namespace std;

CATCommand::CATCommand(AbstractFileSystem* fileSys) : fs(fileSys) {}


int CATCommand::execute(std::string& CWD, std::string options) {
	string fullPath = CWD;
	fullPath += '/';
	fullPath += options;
	AbstractFile* file = fs->openFile(fullPath);
	// if the file does not exist, just print out the message and return a non-zero value
	if (file == nullptr) {
		cout << "File does not exist" << endl;
		return filedoesnotexist;
	}

	// writing a directory file is not allowed
	if (dynamic_cast<DirectoryFile*>(file)) {
		fs->closeFile(file);
		cout << "Cannot write to a directory" << endl;
		return filecannotbewritten;
	}

	cout << fullPath << endl;
	// writing a read only file is not allowed
	if (dynamic_cast<proxy_permission_file*>(file)) {
		fs->closeFile(file);
		cout << "This file is read only" << endl;
		return filecannotbewritten;
	}

	size_t endIndex = fullPath.find_first_of(' ', 0);
	bool dash_a = false;
	if (endIndex != string::npos) {
		string d = fullPath.substr(endIndex + 1, string::npos);
		if (d.compare("-a") == 0) {
			dash_a = true;
		}
	}

	cat_prompt_msg();
	
	vector<char> cat_content;

	// keep taking user inputs unless user stops 
	while (1) {
		string cat_input = cat_prompt();

		// quit command
		if (cat_input == ":q") {
			cat_content.clear();
			break;
		}
		// quit and save command
		// if has -a option, append the content to the file
		// otherwise, just rewrite the file
		else if (cat_input == ":wq") {
			if (dash_a) {
				file->append(cat_content);
			}
			else {
				file->write(cat_content);
			}
			break;
		}
		// pushing the content of the file to the vector
		else {
			for (size_t i = 0; i < cat_input.size(); ++i) {
				cat_content.push_back(cat_input[i]);
			}
		}
	}
	fs->closeFile(file);

	return success;
}
void CATCommand::displayInfo() {
	cout << "write contents to a file" << endl;
	cout << "Usage: cat <filename> [-a]" << endl;
}

string CATCommand::cat_prompt() {
	string input;
	getline(cin, input);
	return input;
}

void CATCommand::cat_prompt_msg() {
	cout << "Enter data you would like to write to the file. ";
	cout << "Enter :wq to save the file and exit, ";
	cout << "enter :q to exit without saving" << endl;
}