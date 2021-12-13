#include "Syntax.h"

Syntax::Syntax(std::string fileName)
{
	er = new ErrorManager();
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
		throw std::invalid_argument("Expected operation\n");
	}
	getNext();
}

void Syntax::accept(TokenType ident)
{
	if(curToken==nullptr||curToken->getTokenType()!=ident)
	{
		throw std::invalid_argument("Expected another token type\n");
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

void Syntax::skipTo(std::vector<CToken*> skip)
{
	while(curToken!=nullptr)
	{
		if(curToken->getTokenType()==ttIdent)
		{
			for(int i=0;i<skip.size();++i)
			{
				if(skip[i]->getTokenType()==ttIdent)
				{
					return;
				}

			}
		}else if(curToken->getTokenType()==ttConst)
		{
			for (int i = 0; i < skip.size(); ++i)
			{
				if (skip[i]->getTokenType() == ttConst)
				{
					return;
				}

			}
		}else if(curToken->getTokenType()==ttOperation)
		{
			for(int i=0;i<skip.size();++i)
			{
				if(skip[i]->getTokenType()==ttOperation&&skip[i]->getOperation()==curToken->getOperation())
					return;
			}
		}

		getNext();
	}

	if(curToken==nullptr)
	{
		er->ShowErrors();
		exit(0);
	}
}



void Syntax::programme()
{
	try {
		accept(programSy);
	}catch(std:: exception)
	{
		er->addError(PROGRAM_missed, CIO->getPos());
		skipTo({ new CToken(ttOperation,constSy),new CToken(ttOperation,typeSy),new CToken(ttOperation,varSy),new CToken(ttOperation,beginSy) });
		block();
		try {
			accept(point);
			er->ShowErrors();
			return;
		}catch (std::exception)
		{
			er->addError(Point_missed, CIO->getPos());
			er->ShowErrors();
			return;
		}
	}
	try {
		accept(ttIdent);
	}catch (std::exception)
	{
		er->addError(WaitName, CIO->getPos());
		skipTo({ new CToken(ttOperation,constSy),new CToken(ttOperation,typeSy),new CToken(ttOperation,varSy),new CToken(ttOperation,beginSy), new CToken(ttOperation,semicolon) });
		block();
		try {
			accept(point);
			er->ShowErrors();
			return;
		}
		catch (std::exception)
		{
			er->addError(Point_missed, CIO->getPos());
			er->ShowErrors();
			return;
		}
	}
	try {
		accept(semicolon);
	}catch (std::exception)
	{
		er->addError(Semicolon_missed, CIO->getPos());
		skipTo({ new CToken(ttOperation,constSy),new CToken(ttOperation,typeSy),new CToken(ttOperation,varSy),new CToken(ttOperation,beginSy) });
		block();
		try {
			accept(point);
			er->ShowErrors();
			return;
		}
		catch (std::exception)
		{
			er->addError(Point_missed, CIO->getPos());
			er->ShowErrors();
			return;
		}
	}
	block();
	try {
		accept(point);
	}catch (std::exception)
	{
		er->addError(Point_missed, CIO->getPos());
		er->ShowErrors();
		return;
	}

	er->ShowErrors();
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

		if (curToken->getTokenType() != ttIdent)
		{
			er->addError(WaitName, CIO->getPos());
			skipTo({new CToken(ttIdent, ""), new CToken(ttOperation, typeSy), new CToken(ttOperation, varSy), new CToken(ttOperation, beginSy)});
		}
		while (curToken->getTokenType()==ttIdent)
		{
			/*
			 * у нас идентификатор, после равно, после число, после точка с запятой
			 */
			std::string variable=curToken->getIdent();
			accept(ttIdent);

			try {
				accept(equal);
			}catch (std:: exception)
			{
				er->addError(Equal_missed, CIO->getPos());
				skipTo({ new CToken(ttOperation,semicolon), new CToken(ttOperation, typeSy), new CToken(ttOperation, varSy), new CToken(ttOperation, beginSy) });
			}

			if (curToken->getTokenType() == ttConst) {
				if (curToken->getConstVal()->getType() == 0)
				{
					CIntVariant* i = dynamic_cast<CIntVariant*>(curToken->getConstVal());
					aviableTypes[variable]=new CIntType(i->getValue(), true);
				}

				if (curToken->getConstVal()->getType() == 1)
				{
					CFloVariant* f = dynamic_cast<CFloVariant*>(curToken->getConstVal());
					aviableTypes[variable]=new CFloatType(f->getValue(), true);
				}

				if (curToken->getConstVal()->getType() == 2)
				{
					CStrVariant* s = dynamic_cast<CStrVariant*>(curToken->getConstVal());
					aviableTypes[variable]=new CStrType(s->getValue(), true);
				}
				accept(ttConst);
			}else
			{

				er->addError(Error_in_const, CIO->getPos());
				skipTo({ new CToken(ttOperation,semicolon), new CToken(ttOperation, typeSy), new CToken(ttOperation, varSy), new CToken(ttOperation, beginSy), new CToken(ttIdent,"") });
			}
			try {
				accept(semicolon);
			}catch (std::exception)
			{
				er->addError(Semicolon_missed, CIO->getPos());
				skipTo({ new CToken(ttIdent, ""), new CToken(ttOperation, typeSy), new CToken(ttOperation, varSy), new CToken(ttOperation, beginSy) });
			}
			
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

			try {
				accept(semicolon);
			}catch (std::exception)
			{
				er->addError(Semicolon_missed, CIO->getPos());
				skipTo({ new CToken(ttIdent, ""),new CToken(ttOperation, beginSy) });
			}

		} while (curToken->getTokenType() == ttIdent);
	}
}

void Syntax::varDeclaration()
{
	std::vector<std::string> variablesWithoutTypes;

	if(curToken->getTokenType()==ttIdent){
		variablesWithoutTypes.push_back(curToken->getIdent());
		accept(ttIdent);
	}else
	{
		er->addError(WaitName, CIO->getPos());
		skipTo({ new CToken(ttOperation,comma),new CToken(ttOperation,semicolon), new CToken(ttOperation,beginSy) });
	}
	while(curToken->getTokenType()==ttOperation&&curToken->getOperation()==comma)
	{
		getNext();
		try {
			variablesWithoutTypes.push_back(curToken->getIdent());
			accept(ttIdent);
		}catch (std::exception)
		{
			er->addError(WaitName, CIO->getPos());
			skipTo({ new CToken(ttOperation,colon),new CToken(ttOperation,comma) });
		}
	}

	try {
		accept(colon);
	}catch (std::exception)
	{
		er->addError(Colon_missed, CIO->getPos());
		skipTo({ new CToken(ttOperation,semicolon),new CToken(ttIdent,"") });
		if (curToken->getTokenType() != ttIdent)
			return;
	}

	types(variablesWithoutTypes);
}

void Syntax::typesDeclarationPart()
{
	if (curToken->getTokenType() == ttOperation && curToken->getOperation() == typeSy) {
		accept(typeSy);

		do
		{
			accept(ttIdent);
			accept(equal);
			types({});
			accept(semicolon);

		} while (curToken->getTokenType() == ttIdent); //сложно, что типы данных не ключевые слова
	}
}

void Syntax::types(std::vector<std::string> variablesWithoutType)
{
	/*"Семантический анализатор поможет выбрать альтернативу"
	 * Что ж, хочется верить
	 *
	 * upd: я сам семантический анализатор ААААААААА
	 */
	if(curToken->getTokenType()==ttIdent||(curToken->getTokenType()==ttOperation&&curToken->getOperation()==leftpar)||curToken->getTokenType()==ttConst) //тип переменной не ключевое слово
		simpleType(variablesWithoutType);
	else if (curToken->getTokenType()==ttOperation&&(curToken->getOperation()==packedSy||isOper({arraySy, recordSy,setSy, fileSy})))
		compositeType();
	else if(curToken->getTokenType()==ttOperation&&curToken->getOperation()==arrow)
		referenceType();
}

void Syntax::simpleType(std::vector<std::string> variablesWithoutType)
{
	if(curToken->getTokenType()==ttOperation&&curToken->getOperation()==leftpar)
		countingType();
	else if(curToken->getTokenType()==ttConst)
		limitedType();
	else
	{
		if(curToken->getTokenType()==ttIdent)
		{
			if (curToken->getIdent() == "integer")
				for (int i = 0; i < variablesWithoutType.size(); ++i)
					aviableTypes[variablesWithoutType[i]] = new CIntType();
			else if(curToken->getIdent()=="real")
				for (int i = 0; i < variablesWithoutType.size(); ++i)
					aviableTypes[variablesWithoutType[i]] = new CFloatType();
			else if (curToken->getIdent() == "string")
				for (int i = 0; i < variablesWithoutType.size(); ++i)
					aviableTypes[variablesWithoutType[i]] = new CStrType();
			else if (curToken->getIdent() == "boolean")
				for (int i = 0; i < variablesWithoutType.size(); ++i)
					aviableTypes[variablesWithoutType[i]] = new CBoolType();
			else
			{
				er->addError(Type_error, CIO->getPos());
				accept(ttIdent);
				skipTo({ new CToken(ttOperation,semicolon),new CToken(ttOperation,beginSy), });
				return;

			}
			accept(ttIdent);
		}else
		{
			er->addError(SimpleType_error, CIO->getPos());
			skipTo({ new CToken(ttOperation,semicolon),new CToken(ttOperation,beginSy), });
		}
	}
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
	simpleType({});
	while(curToken->getTokenType()==ttOperation&&curToken->getOperation()==comma)
	{
		getNext();
		simpleType({});
	}
	accept(rbracket);
	accept(ofSy);
	types({});
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
		types({});
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
	simpleType({});
}

void Syntax::fileType()
{
	accept(fileSy);
	accept(ofSy);
	types({});
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
	//Тут погонять преколы, что-то тут немного не то
	try {
		accept(beginSy);
	}
	catch (std::exception)
	{
		er->addError(Wait_begin, CIO->getPos());
		skipTo({ new CToken(ttOperation,ifSy),new CToken(ttOperation,whileSy), new CToken(ttIdent,""),new CToken(ttOperation,endSy) });
		//_operator();
	}

	_operator();

	//if(curToken->getTokenType() == ttOperation && curToken->getOperation() != semicolon|| curToken->getTokenType() != ttOperation){
	//	er->addError(Semicolon_missed, CIO->getPos());
	//	getNext();
	//	}
	while (curToken->getTokenType() == ttOperation && curToken->getOperation() == semicolon)
	{
		accept(semicolon);
		if(curToken->getTokenType()==ttOperation&&curToken->getOperation()==endSy)
			break;
		_operator();
	}

	try {
		accept(endSy);
	}catch (std::exception)
	{
		er->addError(Wait_end,CIO->getPos());		
	}
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
	std::string variable = "";
	if (curToken->getTokenType() == ttIdent) {

		variable = curToken->getIdent();
		accept(ttIdent);
	}
	try {
		accept(assign);
	}catch (std::exception)
	{
		er->addError(Assign_missed, CIO->getPos());
		skipTo({new CToken(ttIdent,""),new CToken(ttConst,nullptr),new CToken(ttOperation,semicolon),new CToken(ttOperation,leftpar)});
	}

	auto exp=expression();

	CType* left=nullptr;
	if (aviableTypes.find(variable) != aviableTypes.end())
		left = aviableTypes[variable];


	//if(left==nullptr)
	//{
	//	//semntic error
	//	std::cout << "error\n";
	//}
	//else if(left->isDerivedFrom(exp))
	//{
	//	left=left->derivedTo(left,exp);
	//}else
	//{
	//	//semantic error
	//	std::cout << "Types error\n";
	//}
}

CType* Syntax::expression()
{
	auto left=simpleExpression();

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

		auto right=simpleExpression();

		//Вроде можно сравнивать только приводимые типы друг с другом
		//if(left->isDerivedFrom(right))
		//{
		//	left = left->derivedTo(left, right);
		//}
		//else
		//{
		//	std::cout << "error\n";
		//}

		return new CBoolType();
	}

	return left;
	
}

CType* Syntax::simpleExpression()
{
	CType* left=term();

	while (curToken->getTokenType() == ttOperation && isOper({ plus,minus,orSy }))
	{
		//пока так, для проверки работы в целом анализатора
		switch (curToken->getOperation())
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

		CType* right = term();

		//if(left->isDerivedTo(right))
		//{
		//	left=left->derivedTo(left,right);
		//}else
		//{
		//	std::cout << "Types error\n";
		//}
	}

	return left;
}

CType* Syntax::term()
{
	auto left=factor();
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
		auto right=factor();

		//if(left->isDerivedTo(right))
		//{
		//	left=left->derivedTo(left,right);
		//}
		//else {
		//	std::cout << "types error\n";
		//}
	}

	return left;
}

CType* Syntax::factor()
{
	if (curToken->getTokenType() == ttIdent)
	{
		//временно так пропишем!
			accept(ttIdent);
			return new CIntType();

		if (aviableTypes.find(curToken->getIdent()) != aviableTypes.end()) {
			auto right=aviableTypes.find(curToken->getIdent())->second;
			accept(ttIdent);
			return right;
		}

		if(curToken->getIdent()=="true")
		{
			auto right=new CBoolType(true,false);
			accept(ttIdent);
			return right;
		}

		if(curToken->getIdent()=="false")
		{
			auto right=new CBoolType(false,false);
			accept(ttIdent);
			return right;
		}

		//Тут уже семантика даже, про неописанный идентификатор...
		er->addError(WaitName, CIO->getPos());
		skipTo({ new CToken(ttOperation,semicolon),new CToken(ttOperation,doSy),new CToken(ttOperation,thenSy),new CToken(ttOperation,endSy) });
	}
	else if (curToken->getTokenType() == ttConst) {
		if(curToken->getConstVal()->getType()==0)
		{
			CIntVariant* i = dynamic_cast<CIntVariant*>(curToken->getConstVal());
			accept(ttConst);
			return new CIntType(i->getValue(),false);
		}

		if(curToken->getConstVal()->getType()==1)
		{
			CFloVariant* f = dynamic_cast<CFloVariant*>(curToken->getConstVal());
			accept(ttConst);
			return new CFloatType(f->getValue(),false);
		}

		if(curToken->getConstVal()->getType()==2)
		{
			CStrVariant* s = dynamic_cast<CStrVariant*>(curToken->getConstVal());
			accept(ttConst);
			return new CStrType(s->getValue(),false);
		}
			
	}
	else if(curToken->getTokenType()==ttOperation&&curToken->getOperation()==leftpar)
	{
		accept(leftpar);
		auto exp=expression();

		try {
			accept(rightpar);
		}catch (std::exception)
		{
			er->addError(Rightpar_missed, CIO->getPos());
			getNext();
		}
		return exp;
	}else if(curToken->getTokenType()==ttOperation&&curToken->getOperation()==lbracket)
	{
		accept(lbracket);
		listOfElements();
		accept(rbracket);
	}else if(curToken->getOperation()==ttOperation&&curToken->getOperation()==notSy)
	{
		accept(notSy);
		auto t=factor();
		return t;
	}else
	{
		er->addError(WaitName, CIO->getPos());
		skipTo({ new CToken(ttOperation,semicolon),new CToken(ttOperation,doSy),new CToken(ttOperation,thenSy),new CToken(ttOperation,endSy) });
		return nullptr;
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
	auto exp=expression();
	//if(exp->isDerivedTo(new CBoolType()))
	//{
	//	exp=exp->derivedTo(exp,new CBoolType());
	//}else
	//{
	//	std::cout << "error\n";
	//}

	try {
		accept(thenSy);
	}catch(std::exception)
	{
		er->addError(Then_missed, CIO->getPos());
		skipTo({ new CToken(ttIdent,""),new CToken(ttOperation,beginSy),new CToken(ttOperation,whileSy), new CToken(ttOperation,ifSy) });
	}

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
	auto left=expression();
	if(left->isDerivedTo(new CBoolType()))
	{
		left = left->derivedTo(left, new CBoolType());
	}
	else
	{
		std::cout << "error\n";
	}

	try {
		accept(doSy);
	}catch (std::exception)
	{
		er->addError(Do_missed, CIO->getPos());
		skipTo({ new CToken(ttIdent,""),new CToken(ttOperation,beginSy),new CToken(ttOperation,whileSy), new CToken(ttOperation,ifSy) });
	}
	_operator();
}

void Syntax::joinOperator()
{
	//просто так
}




CIntType::CIntType()
{
	myType = et_integer;
	value = 0; //по умолчанию 0 пропишем
	isConst = false;
}

CIntType::CIntType(int value,bool isConst)
{
	myType = et_integer;
	this->value = value;
	this->isConst = isConst;
}

CIntType::~CIntType() = default;

bool CIntType::isDerivedTo(CType* curType)
{
	/*
	 * Целочисленное можно привести к целочисленному и вещественному
	 */
	if (curType->getType() == et_integer || curType->getType() == et_float)
		return true;
	return false;
}

CType* CIntType::derivedTo(CType* left, CType* right)
{
	left->setType(right->getType());
	return left;
}

bool CIntType::isDerivedFrom(CType* curType)
{
	if (curType->getType() == et_integer)
		return true;
	return false;
}


CFloatType::CFloatType()
{
	myType = et_float;
	value = 0;
	isConst = false;
}

CFloatType::CFloatType(double value, bool isConst)
{
	myType = et_float;
	this->value = value;
	this->isConst = isConst;
}

CFloatType::~CFloatType() = default;

bool CFloatType::isDerivedTo(CType* curType)
{
	/*
	 * Вроде можно привести только к вещественному
	 */
	if (curType->getType() == et_float)
		return true;
	return false;
}

CType* CFloatType::derivedTo(CType* left,CType* right)
{
	left->setType(right->getType());
	return left;
}

bool CFloatType::isDerivedFrom(CType* curToken)
{
	if (curToken->getType() == et_integer || curToken->getType() == et_float)
		return true;
	return false;
}


CStrType::CStrType()
{
	myType = et_string;
	value = "";
	isConst = false;
}

CStrType::CStrType(std::string value, bool isConst)
{
	myType = et_string;
	this->value = value;
	this->isConst = isConst;
}

CStrType::~CStrType() = default;

bool CStrType::isDerivedTo(CType* curType)
{
	/*
	 * Только к строке
	 */
	if (curType->getType() == et_string)
		return true;
	return false;
}

CType* CStrType::derivedTo(CType* left, CType* right)
{
	left->setType(right->getType());
	return left;
}

bool CStrType::isDerivedFrom(CType* curToken)
{
	if (curToken->getType() == et_string)
		return true;
	return false;
}


CBoolType::CBoolType()
{
	value = false;
	myType = et_boolean;
	isConst = false;
}

CBoolType::CBoolType(bool value, bool isConst)
{
	myType = et_boolean;
	this->value = value;
	this->isConst = isConst;
}

CBoolType::~CBoolType() = default;

bool CBoolType::isDerivedTo(CType* curType)
{
	if (curType->getType() == et_boolean)
		return true;
	return false;
}

CType* CBoolType::derivedTo(CType* left, CType* right)
{
	left->setType(right->getType());
	return left;
}

bool CBoolType::isDerivedFrom(CType* curToken)
{
	if (curToken->getType() == et_boolean)
		return true;
	return false;
}

EType CType::getType()
{
	return myType;
}

void CType::setType(EType curToken)
{
	myType = curToken;
}

void CType::setError()
{
	this->isError = true;
}
