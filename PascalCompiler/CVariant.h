#pragma once
#include <string>
class CVariant
{
protected:
	enum Type{CIntVariant, CFloVariant, CStrVariant};
public:
	virtual int getType()=0;
	virtual ~CVariant()=default;
};

class CIntVariant:public CVariant
{
private:
	int value;
public:
	CIntVariant(int v);
	int getValue();
	int getType() override;
	~CIntVariant() override;
};

class CFloVariant:public CVariant
{
private:
	double value;
public:
	CFloVariant(double v);
	double getValue();
	int getType() override;
	~CFloVariant() override;
};

class CStrVariant:public CVariant
{
private:
	std::string value;
public:
	CStrVariant(std::string v);
	std::string getValue();
	int getType() override;
	~CStrVariant() override;
};