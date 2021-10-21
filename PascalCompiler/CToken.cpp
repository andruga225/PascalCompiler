#include "CToken.h"

CToken::CToken(TokenType tt, EOperationKeyWords ew)
{
	this->tt = tt;
	this->oper = ew;
}

CToken::CToken(TokenType tt, CVariant* c)
{
	this->tt = tt;
	this->constVal = c;
}

CToken::CToken(TokenType tt, std::string ident)
{
	this->tt = tt;
	this->ident = ident;
}

void CToken::Show()
{
	if(tt==ttIdent)
	{
		std::cout << "ttIdent " << ident << std::endl;
	}

	if(tt==ttOperation)
	{
		std::cout << "ttOperation " << OperKeyWords[oper] << std::endl;
	}

	if(tt==ttConst)
	{
		std::cout << "ttConst ";
		//std::string type=constVal->getType();
		if(constVal->getType()==0)
		{
			CIntVariant* i =dynamic_cast<CIntVariant*>(constVal);
			std::cout << i->getValue() << std::endl;
		}
		if(constVal->getType()==1)
		{
			CFloVariant* f = dynamic_cast<CFloVariant*>(constVal);
			std::cout << f->getValue() << std::endl;
		}
		if(constVal->getType()==2)
		{
			CStrVariant* s = dynamic_cast<CStrVariant*>(constVal);
			std::cout << s->getValue() << std::endl;
		}
	}
}