#pragma once

#include "IOmodule.h"

enum EType { et_integer, et_float, et_string, et_boolean };

class CType
{
protected:
	EType myType;
	bool isConst;
public:
	virtual bool isDerivedTo(CType*) = 0;
	virtual CType* derivedTo(CType*, CType*) = 0;
	virtual bool isDerivedFrom(CType*) = 0;
	EType  getType();
	void setType(EType);
	virtual ~CType() = 0 {}
};

class CIntType : public CType
{
	int value;
public:
	bool isDerivedTo(CType*) override;
	CType* derivedTo(CType*, CType*) override;
	bool isDerivedFrom(CType*) override;
	CIntType();
	CIntType(int,bool);
	~CIntType() override;
};

class CFloatType : public CType
{
	double value;
public:
	bool isDerivedTo(CType*) override;
	CType* derivedTo(CType*, CType*) override;
	bool isDerivedFrom(CType*) override;
	CFloatType();
	CFloatType(double,bool);
	~CFloatType() override;
};

class CStrType : public CType
{
	std::string value;
public:
	bool isDerivedTo(CType*) override;
	CType* derivedTo(CType*,CType*) override;
	bool isDerivedFrom(CType*) override;
	CStrType();
	CStrType(std::string, bool);
	~CStrType() override;
};

class CBoolType : public CType
{
	bool value;
public:
	bool isDerivedTo(CType*) override;
	CType* derivedTo(CType*, CType*) override;
	bool isDerivedFrom(CType*) override;
	CBoolType();
	CBoolType(bool,bool);
	~CBoolType() override;
};

inline std::map<std::string, CType*> aviableTypes;

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
	void types(std::vector<std::string>);
	void simpleType(std::vector<std::string>);
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
	CType* expression();
	CType* simpleExpression();
	CType* term();
	CType* factor();
	void listOfElements();
	void element();

	void whileStatment();

public:
	Syntax(std::string);
};


