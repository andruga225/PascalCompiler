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

//void Syntax::accept(CVariant*)
//{
//	if(curToken==nullptr||curToken->getOperation()!=ttConst)
//}


void Syntax::programme()
{
	accept(programSy);
	accept(ttIdent);
	accept(semicolon);
	block();
	accept(point);
}

void Syntax::block()
{
	constDeclarationPart();
	typesDeclarationPart();
	varDeclarationPart();
	operatorDeclarationPart();
}

void Syntax::constDeclarationPart() //тут подумать
{
	if (curToken->getTokenType() == ttOperation && curToken->getOperation() == constSy) {

		accept(constSy);

		getNext();

		while (curToken->getTokenType()==ttIdent)
		{
			/*
			 * у нас идентификатор, после равно, после число, после точка с запятой
			 */
			accept(equal);
			accept(ttConst); //fix? че я вообще написал?
			accept(semicolon);
		}
	}
}

void Syntax::varDeclarationPart()
{
	if(curToken->getTokenType()==ttOperation&&curToken->getOperation()==varSy)
	{
		accept(varSy);

		do
		{
			varDeclaration();
			accept(semicolon);
		} while (curToken->getTokenType() == ttIdent);
	}
}

void Syntax::varDeclaration()
{
	accept(ttIdent);
	while(curToken->getTokenType()==ttOperation&&curToken->getOperation()==comma)
	{
		getNext();
		accept(ttIdent);
	}

	accept(colon);

	types();
}

void Syntax::typesDeclarationPart()
{
	accept(typeSy);

	do
	{
		accept(ttIdent);
		accept(equal);
		types();
		accept(semicolon);

	} while (curToken->getTokenType() == ttIdent); //сложно, что типы данных не ключевые слова
}

void Syntax::types()
{
	/*"Семантический анализатор поможет выбрать альтернативу"
	 * Что ж, хочется верить
	 */
	simpleType();
	compositeType();
	referenceType();
}

void Syntax::simpleType()
{
	countingType();
	limitedType();

	accept(ttIdent);//третья альтернатива <имя типа>
}

void Syntax::countingType()
{
	accept(leftpar);
	accept(ttIdent);

	while(curToken->getTokenType()==ttOperation&&curToken->getOperation()==comma)
	{
		accept(comma);
		accept(ttIdent);
	}

	accept(rightpar);
}

void Syntax::limitedType()
{
	//ttConst значит константы, здесь нужно подумать
	accept(ttConst);
	accept(twopoints);
	accept(ttConst);
}

void Syntax::compositeType()
{
	if (curToken->getTokenType() == ttOperation && curToken->getOperation() == packedSy)
	{
		accept(packedSy);
	}

	unpackedCompositeType();
}

void Syntax::unpackedCompositeType()
{
	regularType();
	combinedType();
	pluralType();
	fileType();
}

void Syntax::regularType()
{
	accept(arraySy);
	accept(lbracket);
	simpleType();
	while(curToken->getTokenType()==ttOperation&&curToken->getOperation()==comma)
	{
		getNext();
		simpleType();
	}
	accept(rbracket);
	accept(ofSy);
	types();
}

void Syntax::combinedType()
{
	accept(recordSy);
	listOfFields();
	accept(endSy);
}

void Syntax::listOfFields()
{
	//Тут выбор тоже непонятный
	fixedPart();
	variantPart();
}

void Syntax::fixedPart()
{
	recordSection();

	while(curToken->getTokenType()==ttOperation&&curToken->getOperation()==semicolon)
	{
		recordSection();
	}
}

void Syntax::recordSection()
{
	accept(ttIdent);

	while (curToken->getTokenType() == ttOperation && curToken->getOperation() == colon)
	{
		getNext();
		accept(ttIdent);
	}
	accept(colon);
	types();//или пусто..
}

void Syntax::variantPart()
{
	accept(caseSy);

	tagField();
	accept(ttIdent);
	accept(ofSy);
	variant();

	do
	{
		accept(semicolon);
		variant();
	} while (curToken->getTokenType() == ttOperation && curToken->getOperation() == semicolon);
}

void Syntax::tagField()
{
	accept(ttIdent);
	accept(colon);//или пусто, я так понимаю, тут надо каким-то хитрым образом пусто обрабатывать везде
}

void Syntax::variant()
{
	variantLabelsList();
	accept(colon);
	accept(leftpar);
	listOfFields();
	accept(rightpar);//или пусто
}

void Syntax::variantLabelsList()
{
	accept(ttConst);//какая-то константа, с этим, как я уже говорил, надо разобраться

	while(curToken->getTokenType()==ttOperation&&curToken->getOperation()==comma)
	{
		accept(comma);
		accept(ttConst);
	}
}

void Syntax::pluralType()
{
	accept(setSy);
	accept(ofSy);
	simpleType();
}

void Syntax::fileType()
{
	accept(fileSy);
	accept(ofSy);
	types();
}

void Syntax::operatorDeclarationPart()
{
	accept(beginSy);
	_operator();

	while(curToken->getTokenType()==ttOperation&&curToken->getOperation()==semicolon)
	{
		accept(semicolon);
		_operator();
	}

	accept(endSy);
}

void Syntax::_operator()
{
	//в нашем случае меток нет, так что сразу простой и сложный
	simpleOperator();
	complexOperator();//выбор тут
}

void Syntax::simpleOperator()
{
	assignOperator();
}

void Syntax::assignOperator()
{
	accept(ttIdent);
	accept(assign);
	expression();
}

void Syntax::expression()
{
	simpleExpression();
	//or
	simpleExpression();
	//accept операторы отношения
	simpleExpression();
}

void Syntax::simpleExpression()
{
	term();
	//while(+|-|or)
	//accept(oper)
	term();
}

void Syntax::term()
{
	factor();
	//while(*|/|div|mod|and)
	//accept(oper)
	factor();
}

void Syntax::factor()
{
	
}





























