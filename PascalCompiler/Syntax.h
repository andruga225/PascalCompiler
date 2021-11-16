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
	void accept(CVariant*);
	void programme();
	void block();
	void constDeclarationPart();

	void typesDeclarationPart();
	void typesDeclaration();
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
	void _operator();
	void simpleOperator();
	void assignOperator();
	void complexOperator();
	void compositeOperator();
	void choosingOperator();
	void loopOperator();
	void conditionalOperator();
	void expression();
	void simpleExpression();
	void term();
	void factor();

	void whileStatment();
public:
	Syntax(std::string);
};

