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
		s += '\n';
		programText.push_back(s);
		buf += s;
	}

	this->fin.close();

}

CToken* IOmodule::getNextToken()
{
	States state = START;
	char c = getNextSymbol();
	std::string token="";

	while (true) {
		switch (state)
		{
		case START:
		{
			if (curSymbol == buf.size()-1)
				return NULL;

			while (c == ' ' || c == '\t' || c == '\n')
				c = getNextSymbol();
			token.push_back(c);

			if(c=='?'||c=='&'||c=='%'||(c>='À'&&c<='ß')||(c>='à'&&c<='ÿ'))
			{
				error=new lexError(lexError::bannedSymbol, curSymbol);
				state = ERROR;
			}
					
			if (c >= '0' && c <= '9' || c == '.'||c==39) {
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
				error=new lexError(lexError::CommentNotOpen, curSymbol);
				state = ERROR;

			}else
			{
				state = OPERATION;
			}
			break;
		}
		case ID:
		{
			c = getNextSymbol();
			while ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || (c >= '0' && c <= '9'))
			{
				token.push_back(c);
				c = getNextSymbol();
			}
			if (c == '?' || c == '&' || c == '%' || (c >= 'À' && c <= 'ß') || (c >= 'à' && c <= 'ÿ'))
			{
				error=new lexError(lexError::bannedSymbol, curSymbol);
				c = getNextSymbol();
				while ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || (c >= '0' && c <= '9'))
				{
					c = getNextSymbol();
				}
				curSymbol--;
				state = ERROR;
				break;
			}

			curSymbol--;
			int o = isKeyWord(token);

			if (o != -1)
			{
				return new CToken(ttOperation, static_cast<EOperationKeyWords>(o));
			}

			if(token.length()>126)
			{
				error = new lexError(lexError::maxLenghtId, curSymbol);
				state = ERROR;
				break;
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

			if (c == '?' || c == '&' || c == '%' || (c >= 'À' && c <= 'ß') || (c >= 'à' && c <= 'ÿ'))
			{
				error=new lexError(lexError::bannedSymbol, curSymbol);
				state = ERROR;
				break;
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
			if (c == '.')
			{
				c = getNextSymbol();
				if (c == '.')
					return new CToken(ttOperation, twopoints);

				curSymbol--;
				return new CToken(ttOperation, point);
			}

			if (c == 39)
			{
				token = "";
				c = getNextSymbol();
				while (c != 39 && curSymbol < buf.size() - 1)
				{
					token.push_back(c);
					c = getNextSymbol();
				}

				if (c == 39)
					return new CToken(ttConst, new CStrVariant(token));

				if (curSymbol == buf.size() - 1) {
					curSymbol--;
					error = new lexError(lexError::StrNotEnd, curSymbol - token.length());
					state = ERROR;
					break;
				}
			}
			c = getNextSymbol();
			bool fl = false;
			while (c >= '0' && c <= '9' || c == '.') {
				if (c == '.')
				{
					c = getNextSymbol();
					if (c == '.')
					{
						curSymbol--;
						break;
					}

					curSymbol-=2;
					c = getNextSymbol();
					fl = true;
				}
				token.push_back(c);
				c = getNextSymbol();
			}

			if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')||c=='_')
			{
				error=new lexError(lexError::wrongId, curSymbol);
				state = ERROR;
				c = getNextSymbol();

				while ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')|| c == '_' || (c >= '0' && c <= '9'))
					c = getNextSymbol();

				curSymbol--;

				break;
			}

			curSymbol--;
			if (fl)
				return new CToken(ttConst, new CFloVariant(atof(token.c_str())));

			int a = atoi(token.c_str());

			if (a > 32767 || a < -32768)
			{
				error=new lexError(lexError::maxLenghtInteger, curSymbol);
				state = ERROR;
				break;
			}
			
			return new CToken(ttConst, new CIntVariant(a));

		}
		case ASSIGN:
		{
			c = getNextSymbol();
			if (c == '=')
				return new CToken(ttOperation, assign);

			if (c == '?' || c == '&' || c == '%' || (c >= 'À' && c <= 'ß') || (c >= 'à' && c <= 'ÿ'))
			{
				error=new lexError(lexError::bannedSymbol, curSymbol);
				state = ERROR;
				break;
			}
			curSymbol--;
			return new CToken(ttOperation, colon);
		}
		case COMMENT:
		{
			token = "";
			int errorPos = curSymbol;
			c = getNextSymbol();
			while (c != '}'&&curSymbol<buf.size()-1)
				c = getNextSymbol();

			if (curSymbol == buf.size()-1) {
				curSymbol--;
				error=new lexError(lexError::CommentNotClose, errorPos);
				state = ERROR;
				break;
			}

			state = START;
			c = getNextSymbol();
			break;
		}
		case ERROR:
		{
			//while (c == ' ' || c == '\t' || c == '\n'||curSymbol!=buf.size())
				c = getNextSymbol();
			error->show();
			token = "";
			delete error;
			state = START;
		}
		}
	}
}

lexError::lexError(lexErrorsCodes errorCode, int pos)
{
	this->errorCode = errorCode;
	this->pos = pos;
}

void lexError::show()
{
	int rowPos=0;
	for (int i=0;i<programText.size();++i)
	{
		if(pos>programText[i].length())
		{
			pos -= programText[i].length();
			rowPos = i+1;
		}else
		{
			break;
		}
	}

	std::cout << programText[rowPos];
	for (int i = 0; i < pos; ++i)
		std::cout << " ";
	std::cout << "^" << std::endl;

	switch (errorCode)
	{
	case bannedSymbol:
		std::cout << "Error: Banned symbol!" << std::endl;
		break;
	case wrongId:
		std::cout << "Error: Violation of the rules of the name of the identifier!" << std::endl;
		break;
	case maxLenghtId:
		std::cout << "Error: Maximum identifier length exceeded!" << std::endl;
		break;
	case CommentNotOpen:
		std::cout << "Error: Comment not open!" << std::endl;
		break;
	case CommentNotClose:
		std::cout << "Error: Comment not close!" << std::endl;
		break;
	case maxLenghtInteger:
		std::cout << "Error: Out of range!" << std::endl;
		break;
	case StrNotEnd:
		std::cout << "Error: String const not end!" << std::endl;
		break;
	}
	
}

