#include "SymCommand.h"
#include "proxy.h"
#include "DirectoryFile.h"
#include <iostream>

using namespace std;

using namespace std;

SymCommand::SymCommand(AbstractFileSystem* fileSys) : fs(fileSys) {}

int SymCommand::execute(std::string& CWD, std::string options) {
	string fullPath = CWD;
	fullPath += '/';
	fullPath += options;
	AbstractFile* file = fs->openFile(fullPath);

	// if the file doesn't exist
	if (file == nullptr) {
		cout << "File does not exist" << endl;
		return filedoesnotexist;
	}

	// linking a directory file is not allowed
	if (dynamic_cast<DirectoryFile*>(file) != nullptr) {
		cout << "cannot link a directory file" << endl;
		fs->closeFile(file);
		return directorycannotbedisplayed;
	}

	size_t endIndex1 = options.find_first_of(' ', 0);
	string sym_to_path = options.substr(endIndex1 + 1, string::npos);

	AbstractFile* dir = fs->openFile(sym_to_path);
	// the destionation path has to be a directory file
	if (dynamic_cast<DirectoryFile*>(dir) == nullptr) {
		cout << "The destination path is not a directory file" << endl;
		fs->closeFile(dir);
		delete dir;
		fs->closeFile(file);
		return notadirectory;
	}

	fs->closeFile(dir);

	proxy_sym_linked_file* proxy_file = dynamic_cast<proxy_sym_linked_file*>(file);

	proxy_sym_linked_file* proxy_sym_to_file;
	int sym_from_result = success;
	
	// if the proxy file is not a sym linked file, then make a copy of that file, and create 
	// two new proxy objects pointng to the same copy of that file, and delete the original file
	if (proxy_file == nullptr) {
		fullPath = CWD + '/' + file->getName();
		proxy_file = new proxy_sym_linked_file(file->clone(), new int(1));

		fs->closeFile(file);
		fs->deleteFile(fullPath);

		proxy_sym_to_file = new proxy_sym_linked_file(proxy_file);
		sym_from_result = fs->addFile(fullPath, proxy_file);
	}
	// otherwise, just create another sym linked file, the copy constructor will link them to the
	// same AbstractFile pointer within the proxy sym file object.
	else {
		proxy_sym_to_file = new proxy_sym_linked_file(proxy_file);
	}

	sym_to_path += '/' + proxy_file->getName();

	int result = fs->addFile(sym_to_path, proxy_sym_to_file);

	// if either file is created unsuccessfully, cancel the whole process
	if (result != success && sym_from_result != success) {
		cout << "failed to add the symbolic linked file to file system, deleting file" << endl;
		delete proxy_file;
		delete proxy_sym_to_file;
		return failedtoadd;
	}
	
	fs->closeFile(proxy_sym_to_file);
	fs->closeFile(proxy_file);
	return success;
}

void SymCommand::displayInfo() {
	cout << "Create symbolic links between two files" << endl;
	cout << "Usage: sym <filename> <file_path_to_destination>" << endl;
}