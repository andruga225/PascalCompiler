#pragma once
#include <fstream>
#include "CToken.h"
#include <vector>

class lexErrors
{
private:
	enum lexErrorsCodes{bannedSymbol, wrongId, maxLenghtId, CommentNotOpen,CommentNotClose, maxLenghtInteger};
	std::vector<std::string> programText;
	int pos;
	int errorCode;
public:
	lexErrors(int errorCode, int pos);
	void addNewString(std::string);
	void show();
};

class IOmodule
{
private:
	enum States{ START,ID,OPERATION,CONST, ASSIGN, COMMENT };
	std::string buf;
	std::ifstream fin;
	int curSymbol=-1;
	int bufSize=0;
	char getNextSymbol();
	std::vector<lexErrors*> errorses;
public:
	IOmodule(std::string);
	CToken* getNextToken();
};

