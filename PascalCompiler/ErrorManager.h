#pragma once
#include <vector>

enum ErrorCodes{WaitName,PROGRAM_missed,WaitSemicolon};
class ErrorManager
{
	std::vector<std::pair<ErrorCodes,int>> err;
public:
	void addError(ErrorCodes, int);
	void ShowErrors();
};

