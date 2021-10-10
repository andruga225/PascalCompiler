#pragma once
#include <fstream>
#include "CToken.h"
class IOmodule
{
private:
	enum States{ START,ID,OPERATION,CONST, ASSIGN };
	std::string buf;
	std::ifstream fin;
	int curSymbol=-1;
	int bufSize=0;
	char getNextSymbol();
public:
	IOmodule(std::string);
	CToken* getNextToken();
};

