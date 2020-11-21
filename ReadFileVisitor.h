/*
Author: Jon Shidal
Purpose: CSE 332 lab 5
*/
#pragma once
#include "AbstractFileVisitor.h"

class ReadFileVisitor : public AbstractFileVisitor {
public:
	virtual void visit(TextFile*) override;
	virtual void visit(ImageFile*) override;
	virtual void visit(DirectoryFile*) override;
	virtual void set_dash_l(bool);
	virtual void set_dash_d(bool);

private:
	bool dash_l;
	bool dash_d;
};