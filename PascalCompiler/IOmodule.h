#pragma once
#include <fstream>
#include "CToken.h"
#include <vector>

inline std::vector<std::string> programText;//как же это ужасно, но лучше я пока не придумал

class lexError
{
public:
	enum lexErrorsCodes { bannedSymbol, wrongId, maxLenghtId, CommentNotOpen, CommentNotClose, maxLenghtInteger, StrNotEnd };
private:
	int pos;
	lexErrorsCodes errorCode;
public:
	lexError(lexErrorsCodes errorCode, int pos);
	void show();
};

class IOmodule
{
private:
	enum States{ START,ID,OPERATION,CONST, ASSIGN, COMMENT, ERROR };
	std::string buf;
	std::ifstream fin;
	int curSymbol=-1;
	char getNextSymbol();
	lexError* error;
public:
	IOmodule(std::string);
	CToken* getNextToken();
};

