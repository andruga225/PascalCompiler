#pragma once
#include <string>
class CVariant
{
public:
	virtual std::string getType()=0;
	virtual ~CVariant()=default;
};

class CIntVariant:public CVariant
{
private:
	int value;
public:
	CIntVariant(int v);
	int getValue();
	std::string getType() override;
	~CIntVariant() override;
};

class CFloVariant:public CVariant
{
private:
	double value;
public:
	CFloVariant(double v);
	double getValue();
	std::string getType() override;
	~CFloVariant() override;
};

class CStrVariant:public CVariant
{
private:
	std::string value;
public:
	CStrVariant(std::string v);
	std::string getValue();
	std::string getType() override;
	~CStrVariant() override;
};