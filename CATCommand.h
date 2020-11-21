#pragma once
#include "AbstractCommand.h"


class CATCommand : public AbstractCommand {
public:
	CATCommand(AbstractFileSystem* fileSys);

	virtual int execute(std::string& CWD, std::string options) override;
	virtual void displayInfo() override;
	virtual ~CATCommand() = default;
private:
	AbstractFileSystem* fs;
	std::string cat_prompt();
	void cat_prompt_msg();
};