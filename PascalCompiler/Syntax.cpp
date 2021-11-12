#include "Syntax.h"

Syntax::Syntax(std::string fileName)
{
	CIO = new IOmodule(fileName);
	curToken = CIO->getNextToken();
}

void Syntax::getNext()
{
	curToken = CIO->getNextToken();
}

void Syntax::accept(EOperationKeyWords op)
{
	if(curToken==nullptr||curToken->getTokenType()!=ttOperation||curToken->getOperation()!= op)
	{
		std::cout << "Expected operation\n";
		return;
	}
	getNext();
}

void Syntax::accept(TokenType ident)
{
	if(curToken==nullptr||curToken->getTokenType()!=ident)
	{
		std::cout << "Expected identificator\n";
		return;
	}
	getNext();
}

void Syntax::programme()
{
	accept(programSy);
	accept(ttIdent);
	accept(semicolon);
	//block();
	accept(point);
}

