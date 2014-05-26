#include "mini_xfileToken.h"
#include "exception.h"
#include <cctype>


using namespace std;
using namespace VTF;
using namespace mini;


XFileToken XFileToken::NextToken(istream& s, bool skipComments)
{
	if (skipComments)
	{
		if (!SkipComments(s))
			return XFileToken();
	}
	else if (!SkipWS(s))
		return XFileToken();
	auto c = s.peek();
	if (isalpha(c) || c == '_')
		return ReadIdentifier(s);
	if (isdigit(c) || c == '-' || c == '+')
		return ReadNumber(s);
	if (c == '<')
		return ReadUUID(s);
	if (c == '\"')
		return ReadString(s);
	if (c == '/' || c == '#')
		return ReadComment(s);
	XFileToken result;
	switch (c)
	{
	case '{':
		result.m_type = LeftBrace;
		break;
	case '}':
		result.m_type = RightBrace;
		break;
	case '[':
		result.m_type = LeftBracket;
		break;
	case ']':
		result.m_type = RightBracket;
		break;
	case ',':
		result.m_type = Comma;
		break;
	case ';':
		result.m_type = Semicolon;
		break;
	}
	s.ignore();
	return result;
}

XFileToken XFileToken::ReadIdentifier(istream& s)
{
	XFileToken result;
	result.m_type = Identifier;
	char c;
	while (s.get(c))
	{
		if (!isalnum(c) && c != '_')
		{
			s.putback(c);
			return result;
		}
		result.m_content.push_back(c);
	}
	return result;
}

XFileToken XFileToken::ReadComment(istream& s)
{
	XFileToken result;
	result.m_type = Comment;
	char c;
	s.get(c);
	if (c == '/')
	{
        if (!s.get(c))
            THROW_EXCEPTION_T("Parsing error", ParsingException);
        if (c != '/')
            THROW_EXCEPTION_T("Parsing error", ParsingException);
	}
	while (s.get(c))
	{
		if (c == '\n')
			return result;
		result.m_content.push_back(c);
	}
	return result;
}

XFileToken XFileToken::ReadString(istream& s)
{
	XFileToken result;
	result.m_type = String;
	char c;
	s.ignore(); // ignore the first "
	while (s.get(c))
	{
		if (c == '\"')
			return result;
		result.m_content.push_back(c);
    }
    THROW_EXCEPTION_T("Parsing error", ParsingException);
}

XFileToken XFileToken::ReadUUID(istream& s)
{
	XFileToken result;
	result.m_type = String;
	char c;
	s.ignore(); // ignore the first <
	while (s.get(c))
	{
		if (c == '>')
			return result;
		result.m_content.push_back(c);
    }
    THROW_EXCEPTION_T("Parsing error", ParsingException);
}

XFileToken XFileToken::ReadNumber(istream& s)
{
	XFileToken result;
	result.m_type = Integer;
	char c;
	s.get(c);
	result.m_content.push_back(c);
	while (s.get(c))
	{
		if (c == '.')
		{
			if (result.m_type == Integer)
				result.m_type = Float;
			else
			{
				s.putback(c);
				return result;
			}
		}
		else if (!isdigit(c))
		{
			s.putback(c);
			return result;
		}
		result.m_content.push_back(c);
	}
	return result;
}

bool XFileToken::SkipWS(istream& s)
{
	while (isspace(s.peek()))
		s.ignore();
	return !s.eof();
}

bool XFileToken::SkipComments(istream& s)
{
	char c;
	SkipWS(s);
	while (s.get(c))
	{
		if (c == '/')
		{
            if (!s.get(c))
                THROW_EXCEPTION_T("Parsing error", ParsingException);
            if (c != '/')
                THROW_EXCEPTION_T("Parsing error", ParsingException);
		}
		else if (c != '#')
		{
			s.putback(c);
			return true;
		}
		while (s.get(c))
		{
			if (c == '\n')
				break;
		}
		SkipWS(s);
	}
	return false;
}
