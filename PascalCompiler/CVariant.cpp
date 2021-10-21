#include "CVariant.h"


CIntVariant::CIntVariant(int v)
{
	this->value = v;
}
int CIntVariant::getValue()
{
	return value;
}

int CIntVariant::getType()
{
	return Type::CIntVariant;
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

int CFloVariant::getType()
{
	return Type::CFloVariant;
}

CStrVariant::CStrVariant(std::string v)
{
	this->value = v;
}

std::string CStrVariant::getValue()
{
	return value;
}

int CStrVariant::getType()
{
	return Type::CStrVariant;
}

CStrVariant::~CStrVariant() = default;

