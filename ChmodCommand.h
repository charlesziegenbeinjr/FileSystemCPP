#pragma once
#include "AbstractCommand.h"

class ChmodCommand : public AbstractCommand {
public:
	ChmodCommand(AbstractFileSystem* fileSys);

	virtual int execute(std::string& CWD, std::string options) override;
	virtual void displayInfo() override;
	virtual ~ChmodCommand() = default;
private:
	AbstractFileSystem* fs;
};