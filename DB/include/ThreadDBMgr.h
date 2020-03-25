#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\threaddb\ThreadDBPool.h"

class ThreadDBMgr
{
public:
	ThreadDBMgr();
	virtual ~ThreadDBMgr();

	// 增加一个数据库连接池（可以是不同数据库类型，也可以是同一个数据库类型，根据DBConnParam的pool name区别）
	bool AddThreadDbPool(DBConnParam &dbConnParam, IDBConnSink *lpSink = NULL);

	// 创建数据库（不创建表）
	bool CreateDatabase(const std::string &strPoolName);
	// 创建数据库表
	bool CreateTables(const std::string &strPoolName, char *szCreateTables[], int iCount);

	// 根据连接池名获取当前线程的一个唯一连接
	IDBConn *GetThreadDbConn(const std::string &strPoolName, DWORD dwWait = DB_GETCONNECTION_TIMEOUT);
	void ReleaseCurrentThreadDbConn(const std::string &strPoolName);
	void ReleaseAllThreadDbPool();

	void PrintfInfo();

private:
	DBLckMap<std::string, ThreadDBPool *> m_mapName2DbPool;
};

