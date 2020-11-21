#pragma once
#include "AbstractFile.h"

class proxy_sym_linked_file : public AbstractFile {
	friend class AbstractFile;
	friend class TextFile;
public:
	proxy_sym_linked_file(AbstractFile*, int* num_count);
	proxy_sym_linked_file(proxy_sym_linked_file*);
	// functions regarding files functionality
	virtual std::vector<char> read() override;
	virtual int write(std::vector<char>) override; // re-writes file
	virtual int append(std::vector<char>) override; // appends to current contents
	virtual unsigned int getSize() override;
	virtual std::string getName() override;

	virtual AbstractFile* clone() override;

	virtual ~proxy_sym_linked_file() override;

	// visitor pattern function
	virtual void accept(AbstractFileVisitor*) override;

protected:
	// composite pattern functions, should only be used by file system objects or command objects which should be friended
	virtual int addChild(AbstractFile*) override;
	virtual int removeChild(std::string name) override;
	virtual void setParent(AbstractFile*) override;
	virtual AbstractFile* getChild(std::string name) override;
	virtual AbstractFile* getParent() override;
private:
	AbstractFile* file;
	AbstractFile* parent;
	int* num_count;
};