#pragma once

#include "IDBConn.h"

extern "C"
{
#include "mysql.h"
#include "mysqld_error.h"
#include "errmsg.h"
};
// #pragma comment(lib, "..\\DB\\lib\\mysql\\opt\\libmySQL.lib")

class DBMySqlField;
class DBMySqlConn;

class DBMySqlRecordset : public DBRecordset
{
public:
	virtual ~DBMySqlRecordset();
	DBMySqlRecordset(HDBRES hRes, int iFieldCount);

public:
	virtual bool Begin();
	virtual void Next();
//	virtual bool Eof() { return m_rowindex == GetRowCount(); }
	virtual bool IsEnd() { return m_row == NULL; }
	virtual bool End();
	virtual bool ReadRow(const char *pchFormat, ...);

	virtual int GetFieldCount() const { return m_iFieldCount;}

protected:

	virtual void Initialize(HDBRES hRes);
	virtual void DbFree();
	virtual int GetRowCount() { return m_Recordset ? (int)mysql_num_rows(m_Recordset) : -1; }
    virtual inline HDBDATA GetFieldData(int iIndex);
	void GetFieldInfo(DBMySqlField *pField);
	inline void SetFieldCount(int iCount) { m_iFieldCount = iCount; }

	inline void SetFieldLen();

	MYSQL_RES *m_Recordset;
	MYSQL_ROW m_row;

private:
	int m_rowindex;
	int m_iFieldCount;

friend DBMySqlField;
friend DBMySqlConn;

};

class DBMySqlField : public  DBField
{
public:
	DBMySqlField(const DBRecordset* pRes, unsigned int iIndex);
	virtual ~DBMySqlField();
	virtual void Init(const DBRecordset* pRes, unsigned int iIndex);

public:
	virtual bool IsNull();
	virtual bool AsBool();
	virtual char AsChar();
	virtual unsigned char AsUChar();
	virtual short AsShort();
	virtual unsigned short AsUShort() ;
	virtual int AsInt();
	virtual unsigned int AsUInt();
	virtual long AsLong();
	virtual unsigned long AsULong();
	virtual float AsFloat();
	virtual double AsDouble();
	virtual __int64 AsInt64();
	virtual unsigned __int64 AsUInt64();
	virtual std::string AsString();
	virtual BYTE * AsBlob(BYTE * pDest, int iLen);

public:
	virtual std::string GetFieldName();
	virtual unsigned int GetLength();
	virtual unsigned int GetIndex();

protected:
	inline void ResetFieldInfo(MYSQL_FIELD* mysqlfield);
	inline void SetLength(int len);

    virtual HDBDATA Value(); 

	DBMySqlRecordset* m_pRes;
	friend DBMySqlRecordset;
};


class DBMySqlConn : public IDBConn
{
public:
	DBMySqlConn();
	virtual ~DBMySqlConn();

public:
	virtual bool IsValid();
    
	virtual bool ConnectToDb();
	virtual void DisconnectFromDb();

    virtual void DbClearConnState(){}
    void ForceSetBlobData(int iCount, BYTE const * const * ppByte, unsigned int const *pLen);
public:
	virtual int DbInsert(const char * table, const char * value);
	virtual int DbInsert(const char * table, const char * field, const char * value, const char * incfield = NULL);
	virtual int DbUpdate(const char * table, const char * value, const char * condition);
	virtual int DbDelete(const char * table, const char * condition);
	virtual DBRecordset * DbSelect(const char * table, const char * columns, const char * condition, 
		const char * clause, int offset = 0, int count = __MAXCOUNT__, bool bErrLog = true);
	virtual int DbExec(const char * execbuf, bool isinsert = false, const char * incfield = NULL);
	virtual DBRecordset * DbQuery(const char * querybuf, int offset = 0, int count = __MAXCOUNT__, bool bErrLog = true);
	virtual bool DbFreeRecordset(DBRecordset * pRes);
	virtual char *DbBlob2Str(const char * blob, unsigned int len);
	virtual bool DbFreeBlobStr(char * blobstr);

	virtual int DbBeginTransact();
	virtual int DbEndTransact();
	virtual int DbRollBack();

	virtual unsigned __int64 DbGetAffectedCount();
	virtual unsigned __int64 DbGetInsertId();

protected:
	virtual int OutputErrorInfo(int nErrorCode, const char *msg);

protected:
	MYSQL *m_pConn;
}; 

#ifdef _MYSQL
void DBAppendValueEscapeMysql(std::string & strSrc, const char *value, bool bAddComma = true);
void DBAppendExpressionEscapeMysql(std::string & strSrc, const char *szField, const char *szOpCode,
	const char *szConnCode, const char *value, bool bAddComma = true);
#endif // _MYSQL