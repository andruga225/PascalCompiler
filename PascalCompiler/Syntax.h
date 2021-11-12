#pragma once

#include "IOmodule.h"
class Syntax
{
private:
	IOmodule* CIO;
	CToken* curToken;
	void getNext();
	void accept(EOperationKeyWords);
	void accept(TokenType);
	void programme();
	void block();
	void constDeclarationPart();
	void typesDeclarationPart();
	void varDeclarationPart();
	void constDeclaration();
	void typesDeclaration();
	void types();
	void simpleType();


public:
	Syntax(std::string);
};

