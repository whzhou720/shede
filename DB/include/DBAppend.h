#pragma once

template<class TYPE>
void DBAppendExpressionEx(std::string & strSrc, const char *szField, 
	const char *szOpCode, const char *szConnCode, TYPE value)
{
	if (strSrc.length() > 0) strSrc += szConnCode;

	std::stringstream ss;
	ss << szField;
	ss << " ";
	ss << szOpCode;
	ss << " ";
	ss << value;

	strSrc += ss.str();
}

inline void DBAppendExpression(std::string & strSrc, const char *szField, 
	const char *szOpCode, const char *szConnCode, char value)
{
	DBAppendExpressionEx(strSrc, szField, szOpCode, szConnCode, value);
}
inline void DBAppendExpression(std::string & strSrc, const char *szField, 
	const char *szOpCode, const char *szConnCode, short value)
{
	DBAppendExpressionEx(strSrc, szField, szOpCode, szConnCode, value);
}
inline void DBAppendExpression(std::string & strSrc, const char *szField, 
	const char *szOpCode, const char *szConnCode, int value)
{
	DBAppendExpressionEx(strSrc, szField, szOpCode, szConnCode, value);
}
inline void DBAppendExpression(std::string & strSrc, const char *szField, 
	const char *szOpCode, const char *szConnCode, long value)
{
	DBAppendExpressionEx(strSrc, szField, szOpCode, szConnCode, value);
}
inline void DBAppendExpression(std::string & strSrc, const char *szField, 
	const char *szOpCode, const char *szConnCode, __int64 value)
{
	DBAppendExpressionEx(strSrc, szField, szOpCode, szConnCode, value);
}

inline void DBAppendExpression(std::string & strSrc, const char *szField, 
	const char *szOpCode, const char *szConnCode, unsigned char value)
{
	DBAppendExpressionEx(strSrc, szField, szOpCode, szConnCode, value);
}
inline void DBAppendExpression(std::string & strSrc, const char *szField, 
	const char *szOpCode, const char *szConnCode, unsigned short value)
{
	DBAppendExpressionEx(strSrc, szField, szOpCode, szConnCode, value);
}
inline void DBAppendExpression(std::string & strSrc, const char *szField, 
	const char *szOpCode, const char *szConnCode, unsigned int value)
{
	DBAppendExpressionEx(strSrc, szField, szOpCode, szConnCode, value);
}
inline void DBAppendExpression(std::string & strSrc, const char *szField, 
	const char *szOpCode, const char *szConnCode, unsigned long value)
{
	DBAppendExpressionEx(strSrc, szField, szOpCode, szConnCode, value);
}
inline void DBAppendExpression(std::string & strSrc, const char *szField, 
	const char *szOpCode, const char *szConnCode, unsigned __int64 value)
{
	DBAppendExpressionEx(strSrc, szField, szOpCode, szConnCode, value);
}
inline void DBAppendExpression(std::string & strSrc, const char *szField, 
	const char *szOpCode, const char *szConnCode, double value)
{
	DBAppendExpressionEx(strSrc, szField, szOpCode, szConnCode, value);
}

void DBAppendExpression(std::string & strSrc, const char *szField, const char *szOpCode, 
	const char *szConnCode, const char *value, bool bAddComma = true);

template<class TYPE>
void DBAppendValueEx(std::string & strSrc, TYPE value)
{
	if (strSrc.length() > 0) strSrc += ", ";

	std::stringstream ss;
	ss << value;
	strSrc += ss.str();
}

inline void DBAppendValue(std::string & strSrc, char value)
{
	DBAppendValueEx(strSrc, value);
}
inline void DBAppendValue(std::string & strSrc, short value)
{
	DBAppendValueEx(strSrc, value);
}
inline void DBAppendValue(std::string & strSrc, int value)
{
	DBAppendValueEx(strSrc, value);
}
inline void DBAppendValue(std::string & strSrc, long value)
{
	DBAppendValueEx(strSrc, value);
}
inline void DBAppendValue(std::string & strSrc, __int64 value)
{
	DBAppendValueEx(strSrc, value);
}

inline void DBAppendValue(std::string & strSrc, unsigned char value)
{
	DBAppendValueEx(strSrc, value);
}
inline void DBAppendValue(std::string & strSrc, unsigned short value)
{
	DBAppendValueEx(strSrc, value);
}
inline void DBAppendValue(std::string & strSrc, unsigned int value)
{
	DBAppendValueEx(strSrc, value);
}
inline void DBAppendValue(std::string & strSrc, unsigned long value)
{
	DBAppendValueEx(strSrc, value);
}
inline void DBAppendValue(std::string & strSrc, unsigned __int64 value)
{
	DBAppendValueEx(strSrc, value);
}
inline void DBAppendValue(std::string & strSrc, double value)
{
	DBAppendValueEx(strSrc, value);
}

void DBAppendValue(std::string & strSrc, const char *value, bool bAddComma = true);

