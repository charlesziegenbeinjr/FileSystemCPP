#include "ChmodCommand.h"
#include "DirectoryFile.h"
#include "ProxyPermissionFile.h"
#include<iostream>

using namespace std;

ChmodCommand::ChmodCommand(AbstractFileSystem* fileSys) : fs(fileSys) {}

int ChmodCommand::execute(std::string& CWD, std::string options) {
	string fullPath = CWD;
	fullPath += '/';
	fullPath += options;
	AbstractFile* file = fs->openFile(fullPath);

	if (file == nullptr) {
		cout << "File does not exist" << endl;
		return filedoesnotexist;
	}

	if (dynamic_cast<DirectoryFile *>(file)) {
		fs->closeFile(file);
		cout << "cannot give permission on a directory" << endl;
		return permissioncannotbegiven;
	}

	proxy_permission_file* permission_file = dynamic_cast<proxy_permission_file*>(file);
	if (permission_file == nullptr) {
		fullPath = CWD + '/' + file->getName();

		permission_file = new proxy_permission_file(file->clone());
		fs->closeFile(file);
		fs->deleteFile(fullPath);

		int result = fs->addFile(fullPath, permission_file);
		if (result != success) {
			cout << "failed to add the permission file to file system, deleting file" << endl;
			delete permission_file;
			return failedtoadd;
		}
		fs->closeFile(permission_file);
	}
	else {
		fs->closeFile(file);
	}

	return success;
}
void ChmodCommand::displayInfo() {
	cout << "changes the permission of a file to read only" << endl;
	cout << "Usage: chmod <filename>" << endl;
}