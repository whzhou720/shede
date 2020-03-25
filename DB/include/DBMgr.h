#pragma once

#include "IDBConn.h"

#ifdef _SQLITE
#include ".\sqlite3\DBSqlite3Conn.h"
#endif // _SQLITE
#ifdef _MYSQL
#include ".\mysql\DBMysqlConn.h"
#endif // _MYSQL

#include "ThreadDBMgr.h"
#define  DB_POOL_NAME std::string("DBMgr")

class DBMgr
{
public:
	DBMgr();
	~DBMgr();

public:
	static DBMgr *m_lpThis;
	static DBMgr *GetThis() { return m_lpThis; }

public:
	inline void SetSink(IDBConnSink *obj) { if (NULL != m_lpDbConn) m_lpDbConn->SetSink(obj); }

public:
	bool InitDB(const std::string &strDBName, bool bUsePW = false);
	void DestroyDB();
	bool IsValid();
	bool InitTables(std::string strVer);
	IDBConn* GetDbConn();

	bool InitThreadDB(const std::string &strDBName);
	void DestroyThreadDB();
	IDBConn* GetThreadDbConn();
	void ReleaseThreadDbConn();

	bool DelDBAutoIncrement(std::string strTabName, std::string strColumns);
	bool UpdateDBFieldType(std::string strTabName, std::string strColumns, std::string strNewType);
	bool AddDBIndex(std::string strTabName, std::string strColumn, std::string strIdxName);
	bool AddDBField(std::string strTabName, std::string strColumn, std::string strType, std::string strAfter);

	void PrintfInfo();
protected:
	IDBConn *m_lpDbConn;

	ThreadDBMgr m_ThreadDbMgr;
};
