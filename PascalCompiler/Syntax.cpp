#include "Syntax.h"

Syntax::Syntax(std::string fileName)
{
	CIO = new IOmodule(fileName);
	curToken = CIO->getNextToken();
	programme();
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
		std::cout << "Expected another token type\n";
		return;
	}
	getNext();
}

bool Syntax::isOper(std::vector<EOperationKeyWords> oper)
{
	for(int i=0;i<oper.size();++i)
	{
		if (curToken->getOperation() == oper[i])
			return true;
	}

	return false;
		
}



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
			accept(ttConst);
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
	if (curToken->getTokenType() == ttOperation && curToken->getOperation() == typeSy) {
		accept(typeSy);

		do
		{
			accept(ttIdent);
			accept(equal);
			types();
			accept(semicolon);

		} while (curToken->getTokenType() == ttIdent); //сложно, что типы данных не ключевые слова
	}
}

void Syntax::types()
{
	/*"Семантический анализатор поможет выбрать альтернативу"
	 * Что ж, хочется верить
	 *
	 * upd: я сам семантический анализатор ААААААААА
	 */
	if(curToken->getTokenType()==ttIdent) //тип переменной не ключевое слово
		simpleType();
	else if (curToken->getTokenType()==ttOperation&&(curToken->getOperation()==packedSy||isOper({arraySy, recordSy,setSy, fileSy})))
		compositeType();
	else
	referenceType();
}

void Syntax::simpleType()
{
	if(curToken->getTokenType()==ttOperation&&curToken->getOperation()==leftpar)
		countingType();
	else if(curToken->getTokenType()==ttConst)
		limitedType();
	else
		accept(ttIdent);
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
	if(curToken->getOperation()==arraySy)
		regularType();
	else if(curToken->getOperation()==recordSy)
		combinedType();
	else if(curToken->getOperation()==setSy)
		pluralType();
	else
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
	if(curToken->getTokenType()==ttOperation&&curToken->getOperation()==caseSy)
		variantPart();
	else {
		fixedPart();//а тут просто <fixedPart>
		if (curToken->getTokenType() == ttOperation && curToken->getOperation() == caseSy)
			variantPart();//тут получается альтернатива <fixedPart><variantPart>
	}
	
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
	if (curToken->getTokenType() == ttIdent) {
		accept(ttIdent);

		while (curToken->getTokenType() == ttOperation && curToken->getOperation() == colon)
		{
			getNext();
			accept(ttIdent);
		}
		accept(colon);
		types();
	}
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
	if (curToken->getTokenType() == ttIdent) {
		accept(ttIdent);
		accept(colon);
	}
}

void Syntax::variant()
{
	if (curToken->getTokenType() == ttConst) {
		variantLabelsList();
		accept(colon);
		accept(leftpar);
		listOfFields();
		accept(rightpar);
	}
}

void Syntax::variantLabelsList()
{
	accept(ttConst);

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

void Syntax::referenceType()
{
	accept(arrow);
	accept(ttIdent);//type name
}


void Syntax::operatorDeclarationPart()
{
	compoundOperator();
}

void Syntax::compoundOperator()
{
	accept(beginSy);
	_operator();

	while (curToken->getTokenType() == ttOperation && curToken->getOperation() == semicolon)
	{
		accept(semicolon);
		_operator();
	}

	accept(endSy);
}


void Syntax::_operator()
{
	//в нашем случае меток нет, так что сразу простой и сложный
	//проще выбрать сложный, в других случаях будет простой, вроде нет пересечений с ключевыми словами
	if (curToken->getTokenType() == ttOperation && isOper({beginSy, ifSy, whileSy, withSy}))
		complexOperator();
	else
		simpleOperator();
}

void Syntax::simpleOperator()
{
	//упрощенный вариант, у нас только присваивание или пустой оператор
	if (curToken->getTokenType()==ttIdent)
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

	if(curToken->getTokenType()==ttOperation&&isOper({equal,latergreater,later,laterequal,greater,greaterequal,inSy}))
	{
		//Придумать обработку операторов отношения, по факту от типа simpleExpression зависит
		//Семантика уже?

		//пока так для проверки
		switch (curToken->getOperation())
		{
		case equal:
			accept(equal);
			break;
		case latergreater:
			accept(latergreater);
			break;
		case later:
			accept(later);
			break;
		case laterequal:
			accept(laterequal);
			break;
		case greater:
			accept(greater);
			break;
		case greaterequal:
			accept(greaterequal);
			break;
		case inSy:
			accept(inSy);
			break;
		}

		simpleExpression();
	}
	
}

void Syntax::simpleExpression()
{
	term();
	while (curToken->getTokenType() == ttOperation && isOper({ plus,minus,orSy }))
	{
		//пока так, для проверки работы в целом анализатора
		switch(curToken->getOperation())
		{
		case plus:
			accept(plus);
			break;
		case minus:
			accept(minus);
			break;
		case orSy:
			accept(orSy);
			break;
		}
		term();
	}
}

void Syntax::term()
{
	factor();
	while (curToken->getTokenType() == ttOperation && isOper({ star,slash,divSy,modSy,andSy })) {
		switch (curToken->getOperation())
		{
		case star:
			accept(star);
			break;
		case slash:
			accept(slash);
			break;
		case divSy:
			accept(divSy);
			break;
		case modSy:
			accept(modSy);
			break;
		case andSy:
			accept(andSy);
			break;
		}
		factor();
	}
}

void Syntax::factor()
{
	if (curToken->getTokenType() == ttIdent)
		accept(ttIdent);
	else if (curToken->getTokenType() == ttConst)
		accept(ttConst);
	else if(curToken->getTokenType()==ttOperation&&curToken->getOperation()==leftpar)
	{
		accept(leftpar);
		expression();
		accept(rightpar);
	}else if(curToken->getTokenType()==ttOperation&&curToken->getOperation()==lbracket)
	{
		accept(lbracket);
		listOfElements();
		accept(rbracket);
	}else
	{
		accept(notSy);
		factor();
	}
}

void Syntax::listOfElements()
{
	//супер сложная альтернатива, но попробуем
	if (curToken->getTokenType()==ttIdent||curToken->getTokenType()==ttConst||
		(curToken->getTokenType()==ttOperation&&isOper({leftpar,lbracket,nilSy}))) {
		element();

		while (curToken->getTokenType() == ttOperation && curToken->getOperation() == comma)
		{
			accept(comma);
			element();
		}
	}
}

void Syntax::element()
{
	expression();

	if(curToken->getTokenType()==ttOperation&&curToken->getOperation()==twopoints)
	{
		accept(twopoints);
		expression();
	}
}

void Syntax::complexOperator()
{
	if (curToken->getOperation() == beginSy)
		compoundOperator();
	else if (curToken->getOperation() == ifSy||curToken->getOperation()==caseSy)
		choosingOperator();
	else if (isOper({whileSy,repeatSy,forSy}))
		loopOperator();
	else
		joinOperator();
}

void Syntax::choosingOperator()
{
	if (curToken->getOperation() == ifSy)
		ifStatement();
	//else caseStatement ???
}

void Syntax::ifStatement()
{
	accept(ifSy);
	expression();
	accept(thenSy);
	_operator();
	if (curToken->getTokenType() == ttOperation && curToken->getOperation() == elseSy)
	{
		accept(elseSy);
		_operator();
	}
}

void Syntax::loopOperator()
{
	if (curToken->getOperation() == whileSy)
		whileStatment();
	/*
	 * else if(curToken->getOperation()==repeatSy)
	 *     repeatStatement();
	 * else
	 *     forStatement();
	 */
}

void Syntax::whileStatment()
{
	accept(whileSy);
	expression();
	accept(doSy);
	_operator();
}

void Syntax::joinOperator()
{
	//просто так
}




































