#include "Includes_DB.h"
#include "DBAppend.h"

void DBAppendExpression(std::string & strSrc, const char *szField, const char *szOpCode, 
	const char *szConnCode, const char *value, bool bAddComma)
{
	if (strSrc.length() > 0) strSrc += szConnCode;

	size_t len = strlen(value);
	if (len > 0) {
		strSrc += szField;
		strSrc += " ";
		strSrc += szOpCode;
		strSrc += bAddComma ? " '" : "";
		strSrc += value;
		strSrc += bAddComma ? "'" : "";
	}
	else {
		strSrc += szField;
		strSrc += " ";
		strSrc += szOpCode;
		strSrc += bAddComma ? "'" : "";
		strSrc += bAddComma ? "'" : "";
	}
}

void DBAppendValue(std::string & strSrc, const char *value, bool bAddComma)
{
	if (strSrc.length() > 0) strSrc += ", ";
	char *Comma = bAddComma ? "'" : "";
	size_t len = strlen(value);
	if (len > 0) {
		//-------
		strSrc += Comma;
		strSrc += value;
		strSrc += Comma;
	}
	else {
		strSrc += Comma;
		strSrc += Comma;
	}
}
