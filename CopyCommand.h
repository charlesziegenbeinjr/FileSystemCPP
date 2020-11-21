#pragma once

#include "AbstractCommand.h"

class CopyCommand : public AbstractCommand {
public:
	CopyCommand(AbstractFileSystem* fileSys);
	virtual int execute(std::string& CWD, std::string options) override;
	virtual void displayInfo() override;
	virtual ~CopyCommand() = default;
private:
	AbstractFileSystem* fs;
};
