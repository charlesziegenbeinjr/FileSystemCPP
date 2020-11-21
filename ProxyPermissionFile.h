#pragma once
#include "AbstractFile.h"

class proxy_permission_file : public AbstractFile {
	friend class AbstractFile;
public:
	proxy_permission_file(AbstractFile*);
	// functions regarding files functionality
	virtual std::vector<char> read() override;
	virtual int write(std::vector<char>) override; // re-writes file
	virtual int append(std::vector<char>) override; // appends to current contents
	virtual unsigned int getSize() override;
	virtual std::string getName() override;

	virtual AbstractFile* clone() override;

	virtual ~proxy_permission_file() = default;

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
};