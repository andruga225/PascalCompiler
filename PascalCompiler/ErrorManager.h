#pragma once
#include <vector>

enum ErrorCodes{SimpleType_error,WaitName,PROGRAM_missed,Rightpar_missed,
	Colon_missed,Point_missed,Type_error,Wait_end,Semicolon_missed,Equal_missed,
	Wait_begin,Error_in_const, Assign_missed, Then_missed,Do_missed};
class ErrorManager
{
	std::vector<std::pair<ErrorCodes,int>> err;
public:
	void addError(ErrorCodes, int);
	void ShowErrors();
};

