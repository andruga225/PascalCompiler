#include "CVariant.h"


CIntVariant::CIntVariant(int v)
{
	this->value = v;
}
int CIntVariant::getValue()
{
	return value;
}

std::string CIntVariant::getType()
{
	return "CIntVariant";
}

CIntVariant::~CIntVariant() = default;

CFloVariant::CFloVariant(double v)
{
	this->value = v;
}

double CFloVariant::getValue()
{
	return value;
}

CFloVariant::~CFloVariant() = default;

std::string CFloVariant::getType()
{
	return "CFloVariant";
}

CStrVariant::CStrVariant(std::string v)
{
	this->value = v;
}

std::string CStrVariant::getValue()
{
	return value;
}

std::string CStrVariant::getType()
{
	return "CStrVariant";
}

CStrVariant::~CStrVariant() = default;

