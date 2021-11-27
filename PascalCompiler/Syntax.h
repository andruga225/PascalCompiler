#pragma once

#include "IOmodule.h"
class Syntax
{
private:
	IOmodule* CIO;
	CToken* curToken;
	//Help functions
	void getNext();
	void accept(EOperationKeyWords);
	void accept(TokenType);
	bool isOper(std::vector<EOperationKeyWords>);

	//¡Õ‘
	void programme();
	void block();
	void constDeclarationPart();

	void typesDeclarationPart();
	void types();
	void simpleType();
	void compositeType();
	void referenceType();
	void countingType();
	void limitedType();
	void unpackedCompositeType();
	void regularType();
	void combinedType();
	void listOfFields();
	void fixedPart();
	void variantPart();
	void recordSection();
	void tagField();
	void variant();
	void variantLabelsList();
	void pluralType();
	void fileType();

	void varDeclarationPart();
	void varDeclaration();

	//void constDeclaration();

	void operatorDeclarationPart();
	void compoundOperator();
	void _operator();
	void simpleOperator();
	void assignOperator();
	void complexOperator();
	void choosingOperator();
	void ifStatement();
	void loopOperator();
	void joinOperator();
	void expression();
	void simpleExpression();
	void term();
	void factor();
	void listOfElements();
	void element();

	void whileStatment();

public:
	Syntax(std::string);
};

enum EType{et_integer, et_float, et_string, et_boolean};

class CType
{
protected:
	EType myType;
public:
	virtual bool isDerivedTo(CType);
	virtual CType derivedTo(CType, CType);
	EType getType();
	void setType(CType);
};

class CIntType: public CType
{
	int value;
public:
	bool isDerivedTo(CType) override;
	CType derivedTo(CType, CType) override;
	CIntType();
};

class CFloatType: public CType
{
	double value;
public:
	bool isDerivedTo(CType) override;
	CType derivedTo(CType, CType) override;
	CFloatType();
};

class CStrType: public CType
{
	std::string value;
public:
	bool isDerivedTo(CType) override;
	CType derivedTo(CType, CType) override;
	CStrType();
};

class CBoolType: public CType
{
	bool value;
public:
	bool isDerivedTo(CType) override;
	CType derivedTo(CType, CType) override;
	CBoolType();
};

std::map<std::string, CType> aviableTypes;

