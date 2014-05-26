#pragma once

#include <string>
#include <iostream>
#include <fstream>

namespace mini
{

	class XFileToken
	{
	public:

		enum Type
		{
			None, //no more tokens
			Identifier, //can contain alphanumeric characters and underscore, can't start with a digit
			Integer, //integer number
			Float, //floating point number
			LeftBrace, //left brace '{'
			RightBrace, //right brace '}'
			Semicolon, //semicolon ';'
			Comma, //comma ','
			LeftBracket, //left bracket '['
			RightBracket, //right bracket ']'
			UUID, //universally unique identifier
			String, //character string
			Comment // comment
		};

		XFileToken() : m_type(None) { }

		Type m_type;
		std::string m_content;

		/*********************************************************************
		Returns the next .x file token from the stream
		*********************************************************************/
		static XFileToken NextToken(std::istream& s, bool skipComments = true);

		/*********************************************************************
		Skips  any number  of  white  space   characters  from  the  stream
		until a non-whitespace  character is encountered or  stream runs out
		of  data.  Returns false  if  end-of-file  was  encountered or  true
		otherwise.
		*********************************************************************/
		static bool SkipWS(std::istream& s);

	private:
		static XFileToken ReadUUID(std::istream& s);
		static XFileToken ReadIdentifier(std::istream& s);
		static XFileToken ReadString(std::istream& s);
		static XFileToken ReadNumber(std::istream& s);
		static XFileToken ReadComment(std::istream& s);
		static bool SkipComments(std::istream& s);
	};
}
