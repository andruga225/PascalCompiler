#include "IOmodule.h"

char IOmodule::getNextSymbol()
{
	curSymbol++;
	return buf[curSymbol];
}

int isKeyWord(std::string str)
{
	for(auto i: OperKeyWords)
	{
		if (i.second == str) return i.first;
	}

	return -1;
}


IOmodule::IOmodule(std::string input)
{
	this->fin.open(input);
	while(!fin.eof())
	{
		std::string s;
		std::getline(fin, s);
		buf += s;
	}

}

CToken* IOmodule::getNextToken()
{
	States state = START;
	char c = getNextSymbol();
	if (curSymbol == buf.size())
		return NULL;
	std::string token="";

	while (true) {
		switch (state)
		{
		case START:
		{
			while (c == ' ' || c == '\t' || c == '\n')
				c = getNextSymbol();
			token.push_back(c);

			if(c=='?'||c=='&'||c=='%'||(c>='À'&&c<='ß')||(c>='à'&&c<='ÿ'))
			{
				errorses.push_back(new lexErrors(0, curSymbol));
				while (c != ' ' || c != '\t' || c != '\n' || curSymbol != bufSize)
					c = getNextSymbol();
				token = "";
			}
					
			if (c >= '0' && c <= '9' || c == '.') {
				state = CONST;
			}
			else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_') {
				state = ID;
			}
			else if (c == ':')
			{
				state = ASSIGN;
			}
			else if(c=='{')
			{
				state = COMMENT;
			}else if(c=='}')
			{
				errorses.push_back(new lexErrors(3, curSymbol));
				c = getNextSymbol();
			}else
			{
				state = OPERATION;
			}
			break;
		}
		case ID:
		{
			char c = getNextSymbol();
			while ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || (c >= '0' && c <= '9'))
			{
				token.push_back(c);
				c = getNextSymbol();
			}
			if (c == '?' || c == '&' || c == '%' || (c >= 'À' && c <= 'ß') || (c >= 'à' && c <= 'ÿ'))
			{
				errorses.push_back(new lexErrors(0, curSymbol));
				while (c != ' ' || c != '\t' || c != '\n' || curSymbol != bufSize)
					c = getNextSymbol();
				token = "";
			}
			std::string k;
			k.push_back(c);
				if(isKeyWord(k)!=-1)
				{
					errorses.push_back(new lexErrors(1, curSymbol));
				}
			curSymbol--;
			int o = isKeyWord(token);

			if (o != -1)
			{
				//EOperationKeyWords op = static_cast<EOperationKeyWords>(o);
				return new CToken(ttOperation, static_cast<EOperationKeyWords>(o));
			}

				return new CToken(ttIdent, token);

		}
		case OPERATION:
		{
			//token.push_back(c);
			if (c == '<' || c == '>')
			{
				char r = getNextSymbol();
				if (r == '=')
				{
					if (c == '<') return new CToken(ttOperation, laterequal);
					return new CToken(ttOperation, greaterequal);
				}
				if (r == '>') return new CToken(ttOperation, latergreater);
				curSymbol--;
			}

			while (c >= 'a' && c <= 'z')
			{
				c = getNextSymbol();
				token.push_back(c);
			}

			int o = isKeyWord(token);

				if(token.length()!=1)
					curSymbol--;

			return new CToken(ttOperation,static_cast<EOperationKeyWords>(o));
		}
		case CONST:
		{
			c = getNextSymbol();
			bool fl = false;
			while (c >= '0' && c <= '9' || c == '.') {
				if (c == '.') fl = true;
				token.push_back(c);
				c = getNextSymbol();
			}

			if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
			{
				errorses.push_back(new lexErrors(1, curSymbol));
			}
			curSymbol--;
			if (fl)
				return new CToken(ttConst, new CFloVariant(atof(token.c_str())));

			int a = atoi(token.c_str());

			if (a > 32767 || a < -32768)
			{
				errorses.push_back(new lexErrors(5, curSymbol));
			}
			else
			return new CToken(ttConst, new CIntVariant(a));

		}
		case ASSIGN:
		{
			c = getNextSymbol();
			if (c == '=')
				return new CToken(ttOperation, assign);
		}
		case COMMENT:
		{
			c = getNextSymbol();
			while (c != '}'&&curSymbol!=bufSize)
				c = getNextSymbol();

			if (curSymbol == bufSize) {
				errorses.push_back(new lexErrors(4, curSymbol));
			}
		}
		}
	}
}

void lexErrors::addNewString(std::string str)
{
	programText.push_back(str);
}

lexErrors::lexErrors(int errorCode, int pos)
{
	this->errorCode = errorCode;
	this->pos = pos;
}


