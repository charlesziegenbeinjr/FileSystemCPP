#include "ProxyPermissionFile.h"

using namespace std;

proxy_permission_file::proxy_permission_file(AbstractFile* f) : file(f), parent(nullptr) {}

unsigned int proxy_permission_file::getSize() {
	return file->getSize();
}

string proxy_permission_file::getName() {
	return file->getName();
}

vector<char> proxy_permission_file::read() {
	return file->read();
}

void proxy_permission_file::accept(AbstractFileVisitor* fv) {
	file->accept(fv);
}

int proxy_permission_file::write(vector<char> data) {
	return unabletowritetoreadonlyfile;
}

int proxy_permission_file::append(vector<char> data) {
	return unabletowritetoreadonlyfile;
}

int proxy_permission_file::addChild(AbstractFile*) {
	return notacomposite;
}

int proxy_permission_file::removeChild(string name) {
	return notacomposite;
}

AbstractFile* proxy_permission_file::getChild(std::string name) {
	return nullptr;
}

AbstractFile* proxy_permission_file::getParent() {
	return parent;
}

void proxy_permission_file::setParent(AbstractFile* p) {
	parent = p;
}

AbstractFile* proxy_permission_file::clone() {
	return file->clone();
}
