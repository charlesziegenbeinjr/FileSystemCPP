#include "CopyCommand.h"
#include "DirectoryFile.h"
#include "ReadFileVisitor.h"
#include<iostream>

using namespace std;

CopyCommand::CopyCommand(AbstractFileSystem* fileSys) : fs(fileSys) {}

int CopyCommand::execute(std::string& CWD, std::string options) {
	string fullPath = CWD;
	fullPath += '/';
	fullPath += options;
	AbstractFile* file = fs->openFile(fullPath);

	// if the file does not exist
	if (file == nullptr) {
		cout << "File does not exist" << endl;
		return filedoesnotexist;
	}

	// cloning a directory file is not allowed
	if (dynamic_cast<DirectoryFile*>(file) != nullptr) {
		cout << "cannot clone a directory file" << endl;
		fs->closeFile(file);
		return directorycannotbecloned;
	}
	
	// clone the file
	AbstractFile* clone_file = file->clone();

	size_t endIndex1 = options.find_first_of(' ', 0);

	string copy_to_path = options.substr(endIndex1 + 1, string::npos);
	AbstractFile* check_dir = fs->openFile(copy_to_path);

	// copying into a directory that doesn't exist is not allowed
	if (check_dir == nullptr) {
		cout << "The directory copying into does not exist" << endl;
		fs->closeFile(file);
		return directorydoesnotexist;
	}

	// clone into a non directory file is not allowed
	if (dynamic_cast<DirectoryFile*>(check_dir) == nullptr) {
		cout << "cannot clone a file to a non directory file" << endl;
		fs->closeFile(file);
		fs->closeFile(check_dir);
		delete clone_file;
		return cannotcopytoanondirectoryfile;
	}
	fs->closeFile(check_dir);

	copy_to_path += '/';
	copy_to_path += clone_file->getName();

	// add the clone file to the clone destination path
	int result = fs->addFile(copy_to_path, clone_file);
	
	if (result != success) {
		cout << "failed to add the cloned file to file system, deleting file" << endl;
		fs->closeFile(file);
		delete clone_file;
		return failedtoadd;
	}

	fs->closeFile(clone_file);
	fs->closeFile(file);
	return success;
}
void CopyCommand::displayInfo() {
	cout << "copy a file to the destination" << endl;
	cout << "Usage: cp <file_to_copy> <full_path_to_destination>" << endl;
}