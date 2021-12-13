#include "ErrorManager.h"
#include "IOmodule.h"

void ErrorManager::addError(ErrorCodes er, int pos)
{
	err.push_back(std::make_pair(er, pos));
}

void ErrorManager::ShowErrors()
{
	int strNum = 0;
	int symbolsSum = 0;
	for(int i=0;i<err.size();++i)
	{
		while(true)
		{
			if (err[i].second > symbolsSum + programText[strNum].length()) {
				std::cout << programText[strNum];
				symbolsSum += programText[strNum].length();
				strNum++;
			}
			else
			{
				err[i].second -= symbolsSum;
				break;
			}
		}

		std::cout << programText[strNum];

		for (int j = 0; j < err[i].second; ++j)
			std::cout << " ";
		std::cout << "^\n";

		switch (err[i].first)
		{
		case SimpleType_error:
			std::cout << "Error: error in simple type\n";
			break;
		case WaitName:
			std::cout << "Error: wait name\n";
			break;
		case PROGRAM_missed:
			std::cout << "Error: program missed\n";
			break;
		case Rightpar_missed:
			std::cout << "Error: ) missed\n";
			break;
		case Colon_missed:
			std::cout << "Error: : missed\n";
			break;
		case Point_missed:
			std::cout << "Error: . missed\n";
			break;
		case Type_error:
			std::cout << "Error: error in type\n";
			break;
		case Wait_end:
			std::cout << "Error: wait end token\n";
			break;
		case Semicolon_missed:
			std::cout << "Error: ; missed\n";
			break;
		case Equal_missed:
			std::cout << "Error: = missed\n";
			break;
		case Wait_begin:
			std::cout << "Error: wait begin token\n";
			break;
		case Error_in_const:
			std::cout << "Error: error in const\n";
			break;
		case Assign_missed:
			std::cout << "Error: assign missed\n";
			break;
		case Then_missed:
			std::cout << "Error: then missed\n";
			break;
		case Do_missed:
			std::cout << "Error: do missed\n";
			break;
		}
	}
	strNum++;

	for (strNum; strNum < programText.size(); ++strNum)
		std::cout << programText[strNum];
}

