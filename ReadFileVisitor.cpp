/*
Author: Jon Shidal
Purpose: CSE 332 lab 5
*/

#include "ReadFileVisitor.h"
#include "TextFile.h"
#include "ImageFile.h"
#include "DirectoryFile.h"
#include "ReadMetadataFileVisitor.h"

#include<iostream>
#include<vector>
using namespace std;

void ReadFileVisitor::visit(TextFile * file) {
	vector<char> contents = file->read();
	for (size_t i = 0; i < contents.size(); ++i) {
		cout << contents[i];
	}
	cout << endl;
}

void ReadFileVisitor::visit(ImageFile* file) {
	vector<char> contents = file->read();
	if (!dash_d) {
		// compiler warning here is fine. I want to be able to be negative
		for (int y = file->dimensions - 1; y >= 0; --y) {
			for (size_t x = 0; x < file->dimensions; ++x) {
				size_t index = file->coordsToIndex(x, y);
				cout << contents[index];
			}
			cout << endl;
		}
		cout << endl;
	}
	else {
		for (size_t i = 0; i < file->getSize(); ++i) {
			cout << contents[i];
  		}
		cout << endl;
	}
}

void ReadFileVisitor::visit(DirectoryFile* file) {
	if (dash_l) {
		ReadMetadataFileVisitor reader;
		vector<AbstractFile*> files = file->read_detail();
		for (AbstractFile* file : files) {
			file->accept(&reader);
		}
		return;
	}
	vector<char> contents = file->read();
	for (size_t i = 0; i < contents.size(); ++i) {
		cout << contents[i];
	}
	cout << endl;
}

void ReadFileVisitor::set_dash_l(bool b) {
	dash_l = b;
}

void ReadFileVisitor::set_dash_d(bool b) {
	dash_d = b;
}