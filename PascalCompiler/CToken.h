#pragma once
#include <map>
#include <iostream>
#include "CVariant.h"

enum TokenType{ttIdent, ttOperation, ttConst};
enum EOperationKeyWords
{
	star,
	slash,
	equal,
	comma,
	semicolon,
	colon,
	point,
	arrow,
	leftpar,
	rightpar,
	lbracket,
	rbracket,
	flpar,
	frpar,
	later,
	greater,
	laterequal,
	greaterequal,
	latergreater,
	plus,
	minus,
	assign,
	twopoints,

	ifSy,
	doSy,
	ofSy,
	orSy,
	inSy,
	toSy,
	endSy,
	varSy,
	divSy,
	andSy,
	notSy,
	forSy,
	modSy,
	nilSy,
	setSy,
	theSy,
	elseSy,
	caseSy,
	fileSy,
	gotoSy,
	typeSy,
	withSy,
	beginSy,
	whileSy,
	arraySy,
	constSy,
	labelSy,
	untilSy,
	downtoSy,
	packedSy,
	recordSy,
	repeatSy,
	programSy,
	functionSy,
	procedureSy,
};

inline std::map<EOperationKeyWords, std::string> OperKeyWords{
	{star,"*"},
	{slash,"/"},
	{equal, "="},
	{comma, ","},
	{semicolon,";"},
	{colon,":"},
	{point,"."},
	{arrow,"^"},
	{leftpar,"("},
	{rightpar,")"},
	{lbracket, "["},
	{rbracket,"]"},
	{flpar,"{"},
	{frpar,"}"},
	{later,"<"},
	{greater, ">"},
	{laterequal,"<="},
	{greaterequal,">="},
	{latergreater,"<>"},
	{plus,"+"},
	{minus,"-"},
	{assign,":="},
	{twopoints,".."},
	{ifSy,"if"},
	{doSy,"do"},
	{ofSy,"of"},
	{orSy,"or"},
	{inSy,"in"},
	{toSy,"to"},
	{endSy,"end"},
	{varSy,"var"},
	{divSy,"div"},
	{andSy,"and"},
	{notSy,"not"},
	{forSy,"for"},
	{modSy,"mod"},
	{nilSy,"nil"},
	{setSy, "set"},
	{theSy,"the"},
	{elseSy,"else"},
	{caseSy,"case"},
	{fileSy,"fyle"},
	{gotoSy,"goto"},
	{typeSy,"type"},
	{withSy,"with"},
	{beginSy,"begin"},
	{whileSy,"while"},
	{arraySy,"array"},
	{constSy,"const"},
	{labelSy,"label"},
	{untilSy,"until"},
	{downtoSy,"downto"},
	{packedSy,"packed"},
	{recordSy,"record"},
	{repeatSy,"repeat"},
	{programSy,"program"},
	{functionSy,"function"},
	{procedureSy,"procedure"} };

class CToken
{
private:
	TokenType tt;
	EOperationKeyWords oper;
	std::string ident;
	CVariant* constVal;
public:
	CToken(TokenType tt, EOperationKeyWords ew);
	CToken(TokenType tt, std::string ident);
	CToken(TokenType tt, CVariant* c);
	void Show();
};

