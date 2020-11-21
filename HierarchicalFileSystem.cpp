/*
Author: Jon Shidal
Purpose: CSE 332 lab5 - defines the hierarchical file system class
*/

#include "HierarchicalFileSystem.h"
#include<iostream>

using namespace std;

HierarchicalFileSystem::HierarchicalFileSystem() : rootDirectory(nullptr){
	rootDirectory = new DirectoryFile("root");
	rootDirectory->setParent(rootDirectory);  // root, so parent is itself
}

int HierarchicalFileSystem::addFile(std::string path, AbstractFile* fp) {
	size_t endOfDirectories = path.find_last_of('/');
	if (endOfDirectories == string::npos) { // not '/' so not a valid path
		return invalidpath;
	}
	string filename = path.substr(endOfDirectories + 1, string::npos);
	string directoryPath = path.substr(0, endOfDirectories);
	AbstractFile* toAddTo = parsePath(directoryPath); 
	if (toAddTo == nullptr) {  // directory doesn't exist
		return locationdoesnotexist;
	}
	if (fp == nullptr) {
		return nullfile;
	}
	else {
		if (fp->getParent() != nullptr) {   // file already exist elsewhere in the filesystem
			return filealreadyadded;
		}
		int result = toAddTo->addChild(fp);
		if ( result == AbstractFile::notacomposite || result == AbstractFile::filealreadyexists) { // unable to addChild, must not be a directoryFile
			return result;
		}
		fp->setParent(toAddTo);
		return success;
	}
}

int HierarchicalFileSystem::deleteFile(std::string path) {
	AbstractFile* fileToDelete = parsePath(path);

	if (fileToDelete == nullptr) {
		return filedoesnotexist; // unable to find file
	}
	if (openFiles.find(fileToDelete) != openFiles.end()) {
		return fileisopen; // file in use
	}
	AbstractFile* parent = fileToDelete->getParent();

	if (parent == fileToDelete) {  // in case it is the root directory
		return cantdeleteroot;
	}
	/* TODO: What if the file is a directory? 
	We shoud only remove it if it is empty, or if the user specifies we should delete the file recursively. 
	This will be fixed in lab 5
	*/

	// if the file we want to delete is a directory file, the directory file is not empty, and the -r is not involved
	// we won't be able to delete this file.
	if (dynamic_cast<DirectoryFile*>(fileToDelete) && fileToDelete->getSize() != 0 && !dash_r) {
		cerr << "Directory cannot be removed. Not Empty" << endl;
		return directorynotempty;
	}
	else {
		// if the file we want to delete is a directory file, and the -r is involved, we want to call the remove_directory() helper
		// function to remove the directory anyway
		if (dynamic_cast<DirectoryFile*>(fileToDelete) && dash_r) {
			remove_directory(fileToDelete);
			parent->removeChild(fileToDelete->getName());
			delete fileToDelete;
		}
		// if the file we want to delete is not a directory, just remove the file.
		else {
			parent->removeChild(fileToDelete->getName()); // remove from parent, this rewrites parent's contents also
			delete fileToDelete;
		}
	}
	return success;
}

AbstractFile* HierarchicalFileSystem::openFile(std::string path) {
	AbstractFile* fileToOpen = parsePath(path);
	if (fileToOpen == nullptr) {
		return nullptr;
	}
	if (openFiles.find(fileToOpen) == openFiles.end()) {
		openFiles.insert(fileToOpen);
		return fileToOpen;
	}
	else {
		return nullptr;
	}
}

int HierarchicalFileSystem::closeFile(AbstractFile* file) {
	auto it = openFiles.find(file);
	if (it != openFiles.end()) { // file is currently open
		openFiles.erase(it);
		return success;
	}
	else { // file not open
		return filenotopen;
	}
}

AbstractFile* HierarchicalFileSystem::parsePath(string path) {
	size_t startIndex = 0;
	size_t endIndex = path.find_first_of('/', startIndex);
	size_t endIndex2 = path.find_first_of(' ', startIndex);
	endIndex = endIndex < endIndex2 ? endIndex : endIndex2;

	AbstractFile* current;
	string directory;
	if (endIndex != string::npos) {
		directory = path.substr(startIndex, endIndex - startIndex);
		if (directory == "root") {
			current = rootDirectory;
			startIndex = endIndex + 1;
		}
		else {
			return nullptr;
		}
	}
	else {
		if (path.substr(startIndex, string::npos) == "root") {
			return rootDirectory;
		}
		else {
			return nullptr;
		}
	}
	// root handled, traverse the rest of the path
	bool end_of_name = false;
	while (startIndex != string::npos) {
		endIndex = path.find_first_of('/',startIndex);
		endIndex2 = path.find_first_of(' ', startIndex);
		if (endIndex > endIndex2) {
			endIndex = endIndex2;
			end_of_name = true;
		}

		if (endIndex == string::npos) {
			directory = path.substr(startIndex, string::npos);
			startIndex = string::npos;
		}
		else {
			directory = path.substr(startIndex, endIndex - startIndex);
			startIndex = endIndex + 1;
		}
		current = current->getChild(directory);
		if (current == nullptr) {
			return nullptr;
		}
		if (end_of_name) {
			break;
		}
	}

	// check if the command has a -r option while making a deletion
	if (endIndex2 != string::npos) {
		string d = path.substr(endIndex2+1, string::npos);
		if (d.compare("-r") == 0) {
			dash_r = true;
		}
		else {
			dash_r = false;
		}
	}

	return current;
}

HierarchicalFileSystem::~HierarchicalFileSystem() {
	for (auto it = rootDirectory->children.begin(); it != rootDirectory->children.end(); ++it) {
		delete it->second;
	}
}

// recursively deleting files from the directory, after all files are removed, remove the directory as well.
void HierarchicalFileSystem::remove_directory(AbstractFile *file) {
	DirectoryFile* df = dynamic_cast<DirectoryFile*>(file);
	// check if the current file we are deleting is a directoroy file
	if (df != nullptr) {
		// if the file we want to delete is an empty file, just removoe the directory (base case 1)
		if (file->getSize() == 0) {
			AbstractFile* parent = file->getParent();
			parent->removeChild(file->getName()); // remove from parent, this rewrites parent's contents also
			delete file;
			return;
		}
		// oherwise, recursively removing every files in the current direcory (recursive case)
		else {
			map<string, AbstractFile*> children = df->children;

			for (auto f = children.begin(); f != children.end(); ++f) {
				remove_directory(f->second);
			}
		}
	}
	// if the file we want to delete is not a directory file, just remove the file (base case 2).
	else {
		AbstractFile* parent = file->getParent();
		parent->removeChild(file->getName()); // remove from parent, this rewrites parent's contents also
		delete file;
	}
}