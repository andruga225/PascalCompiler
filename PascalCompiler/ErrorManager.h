#pragma once
#include <vector>

enum ErrorCodes{WaitName,PROGRAM_missed,Point_missed,Semicolon_missed,Equal_missed};
class ErrorManager
{
	std::vector<std::pair<ErrorCodes,int>> err;
public:
	void addError(ErrorCodes, int);
	void ShowErrors();
};

