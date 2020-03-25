#pragma once

#include "IDBConn.h"

// #define SQLITE_HAS_CODEC 1

extern "C"
{
#include "sqlite3.h"
};

// #pragma comment(lib, "..\\DB\\lib\\sqlite3\\sqlite3.lib")

class DBSqlite3Field;
class DBSqlite3Conn;
class DBSqlite3Recordset : public DBRecordset
{
public:
	DBSqlite3Recordset(HDBRES hRes);
	virtual ~DBSqlite3Recordset();

public:
	virtual bool Begin();
	virtual void Next();
	virtual bool IsEnd();
	virtual bool End();
	virtual bool ReadRow(const char *pchFormat, ...);

	virtual int GetFieldCount() const { return m_iFieldCount; }
	virtual int GetRowCount() const { return m_iRowCount; }

protected:
	virtual void Initialize(HDBRES hRes);
	virtual void DbFree();

	virtual inline HDBDATA GetFieldData(int iIndex) { return NULL; }

private:
	int m_rowindex;
	int m_iRowCount;
	int m_iFieldCount;
	sqlite3_stmt *m_pStmt;
	bool m_bIsEnd;

private:
	friend DBSqlite3Field;
	friend DBSqlite3Conn;
};

class DBSqlite3Field : public DBField
{
public:
	DBSqlite3Field(const DBRecordset* pRes, unsigned int iIndex);
	virtual ~DBSqlite3Field();
	virtual void Init(const DBRecordset* pRes, unsigned int iIndex);

public:
	virtual bool IsNull();
	virtual bool AsBool();
	virtual char AsChar();
	virtual unsigned char AsUChar();
	virtual short AsShort();
	virtual unsigned short AsUShort();
	virtual int AsInt();
	virtual unsigned int AsUInt();
	virtual long AsLong();
	virtual unsigned long AsULong();
	virtual float AsFloat();
	virtual double AsDouble();
	virtual __int64 AsInt64();
	virtual unsigned __int64 AsUInt64();
	virtual std::string AsString();
	virtual BYTE *AsBlob(BYTE *pDest, int iLen);

public:
	virtual std::string GetFieldName();
	virtual unsigned int GetLength();
	virtual unsigned int GetIndex();
	HDBDATA GetData();

protected:
	virtual HDBDATA Value();

protected:
	DBSqlite3Recordset* m_pRes;

protected:
	friend DBSqlite3Recordset;
};

class DBSqlite3Conn : public IDBConn
{
public:
	DBSqlite3Conn();
	~DBSqlite3Conn();

public:
	virtual bool IsValid();
	virtual bool ConnectToDb();
	virtual void DisconnectFromDb();
	virtual void DbClearConnState();

public:
	virtual int DbInsert(const char *table, const char *value);
	virtual int DbInsert(const char *table, const char *field, const char *value, const char *incfield = NULL);
	virtual int DbUpdate(const char *table, const char *value, const char *condition);
	virtual int DbDelete(const char *table, const char *condition);
	virtual DBRecordset *DbSelect(const char *table, const char *columns, const char *condition,
		const char *clause, int offset = 0, int count = __MAXCOUNT__, bool bErrLog = true);
	virtual int DbExec(const char *execbuf, bool isinsert = false, const char *incfield = NULL);
	virtual DBRecordset *DbQuery(const char *querybuf, int offset = 0, int count = __MAXCOUNT__, bool bErrLog = true);
	virtual bool DbFreeRecordset(DBRecordset *pRes);
	virtual char *DbBlob2Str(const char *blob, unsigned int len);
	virtual bool DbFreeBlobStr(char *blobstr);

	virtual int DbBeginTransact();
	virtual int DbEndTransact();
	virtual int DbRollBack();

	virtual unsigned __int64 DbGetAffectedCount();
	virtual unsigned __int64 DbGetInsertId();

protected:
	virtual int OutputErrorInfo(int nErrorCode, const char *msg);

protected:
	bool HasBlobValue(const char *pchSql);
	int PrepareStmt(const char *pchSql, size_t nBytes);
	int BandingBolbs(const char *pchSql);
	int FindBolbID(const char *pchSql, size_t &nStart);
	int DoSqlStmt();
	int QuicklySQL(const char *pchSql);

protected:
	sqlite3 *m_pConn;
	sqlite3_stmt *m_pStmt;
	BYTE** m_ppv;

	long m_nBolbLen[MAX_BLOBNUM];
	size_t m_nBolbCount;
};

#ifdef _SQLITE
void DBAppendValueEscapeSqlite(std::string &strSrc, const char *value, bool bAddComma = true);
void DBAppendExpressionEscapeSqlite(std::string &strSrc, const char *szField, const char *szOpCode,
	const char *szConnCode, const char *value, bool bAddComma = true);
#endif // _SQLITE

