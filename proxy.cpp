#include "proxy.h"
#include <iostream>

using namespace std;

proxy_sym_linked_file::proxy_sym_linked_file(AbstractFile* f, int* num) : file(f), parent(nullptr), num_count(num) {
}

proxy_sym_linked_file::proxy_sym_linked_file(proxy_sym_linked_file* f) : file(f->file), parent(nullptr), num_count(f->num_count) {
	*num_count += 1;
}

proxy_sym_linked_file::~proxy_sym_linked_file() {
	// delete the file pointer and integer pointer if there is no more proxy object
	// pointing to that file.
	*num_count -= 1;
	if (*num_count == 0) {
		delete num_count;
		delete file;
	}
}

unsigned int proxy_sym_linked_file::getSize() {
	return file->getSize();
}

string proxy_sym_linked_file::getName() {
	return file->getName();
}

vector<char> proxy_sym_linked_file::read() {
	return file->read();
}

void proxy_sym_linked_file::accept(AbstractFileVisitor* fv) {
	file->accept(fv);
}

int proxy_sym_linked_file::write(vector<char> data) {
	return file->write(data);
}

int proxy_sym_linked_file::append(vector<char> data) {
	return file->append(data);
}

int proxy_sym_linked_file::addChild(AbstractFile*) {
	return notacomposite;
}

int proxy_sym_linked_file::removeChild(string name) {
	return notacomposite;
}

AbstractFile* proxy_sym_linked_file::getChild(std::string name) {
	return nullptr;
}

AbstractFile* proxy_sym_linked_file::getParent() {
	return parent;
}

void proxy_sym_linked_file::setParent(AbstractFile* p) {
	parent = p;
}

AbstractFile* proxy_sym_linked_file::clone() {
	return file->clone();
}
