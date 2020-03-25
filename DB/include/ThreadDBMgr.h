#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\threaddb\ThreadDBPool.h"

class ThreadDBMgr
{
public:
	ThreadDBMgr();
	virtual ~ThreadDBMgr();

	// ����һ�����ݿ����ӳأ������ǲ�ͬ���ݿ����ͣ�Ҳ������ͬһ�����ݿ����ͣ�����DBConnParam��pool name����
	bool AddThreadDbPool(DBConnParam &dbConnParam, IDBConnSink *lpSink = NULL);

	// �������ݿ⣨��������
	bool CreateDatabase(const std::string &strPoolName);
	// �������ݿ��
	bool CreateTables(const std::string &strPoolName, char *szCreateTables[], int iCount);

	// �������ӳ�����ȡ��ǰ�̵߳�һ��Ψһ����
	IDBConn *GetThreadDbConn(const std::string &strPoolName, DWORD dwWait = DB_GETCONNECTION_TIMEOUT);
	void ReleaseCurrentThreadDbConn(const std::string &strPoolName);
	void ReleaseAllThreadDbPool();

	void PrintfInfo();

private:
	DBLckMap<std::string, ThreadDBPool *> m_mapName2DbPool;
};

