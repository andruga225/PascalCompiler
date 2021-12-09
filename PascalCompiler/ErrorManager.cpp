#include "ErrorManager.h"

void ErrorManager::addError(ErrorCodes er, int pos)
{
	err.push_back(std::make_pair(er, pos));
}
