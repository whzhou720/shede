#pragma once

#include "Includes_DB.h"

bool LoadParam(unsigned short usType);

#define __MAXCOUNT__	0x0FFFFFFF
#define MAX_BLOBNUM		5
#define MINSTRLEN		256

typedef struct _DbRes{} *HDBRES;
typedef struct _DbField{} *HDBFIELD;
typedef struct _DbData{} *HDBDATA;

enum DATABASE_TYPES
{
	dtMYSQL,
	dtSQLITE3,
};

enum Field_Type
{
	ftBool = 'b',

	ftChar = 'c',
	ftUChar = 'C',

	ftShort = 's',
	ftUShort = 'S',

	ftInt = 'i',
	ftUInt = 'I',

	ftLong = 'l',
	ftULong = 'L',

	ftFloat = 'f',
	ftDouble = 'd',

	ftInt64 = 'e',
	ftUInt64 = 'E',

	ftString = 'Z',
	ftBlob = 'B',

	ftNull = '0'
};

template <class T>
void convertFromString(T &value, const std::string &s) {
	std::stringstream ss(s);
	ss >> value;
}

template <class T>
std::string ConvertToString(T value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

class DBRecordset;
class DBField
{
public:
	DBField(const DBRecordset* pRes, unsigned int iIndex) { }
	virtual ~DBField() { }

	virtual void Init(const DBRecordset* pRes, unsigned int iIndex) = 0;

public:
	virtual bool IsNull() = 0;
	virtual bool AsBool() = 0;
	virtual char AsChar() = 0;
	virtual unsigned char AsUChar() = 0;
	virtual short AsShort() = 0;
	virtual unsigned short AsUShort() = 0;
	virtual int AsInt() = 0;
	virtual unsigned int AsUInt() = 0;
	virtual long AsLong() = 0;
	virtual unsigned long AsULong() = 0;
	virtual float AsFloat() = 0;
	virtual double AsDouble() = 0;
	virtual __int64 AsInt64() = 0;
	virtual unsigned __int64 AsUInt64() = 0;
	virtual std::string AsString() = 0;
	virtual BYTE* AsBlob(BYTE *pDest, int iLen) = 0;

public:
	virtual std::string GetFieldName() = 0;
	virtual unsigned int GetLength() = 0;
	virtual unsigned int GetIndex() = 0;

protected:
	virtual HDBDATA Value() = 0;
	// move from public model to protected model

protected:
	std::string m_sFieldName;
	unsigned int m_length;  // this datavalue must not be used in mysql , use GetLength instead!
	unsigned int m_iIndex;
	DBRecordset* pRes;
};

class DBRecordset
{
public:
	virtual ~DBRecordset() {}

	virtual bool Begin() = 0;
	virtual void Next() = 0;
	virtual bool End() = 0;
	virtual bool IsEnd() = 0;
	virtual bool ReadRow(const char *pchFormat, ...) = 0;

	virtual int GetFieldCount() const = 0;

protected:
	virtual void Initialize(HDBRES hRes) = 0;
	virtual inline HDBDATA GetFieldData(int iIndex) = 0;

protected:
	DBField ** m_Field;

protected:
	friend DBField;

public:
	DBField* GetField(int iIndex) const
	{
		if (iIndex < 0) return NULL;
		if (iIndex >= GetFieldCount()) return NULL;
		return m_Field[iIndex];
	}

	DBField* GetField(std::string sFieldName) const
	{
		//std::transform(sFieldName.begin(), sFieldName.end(), sFieldName.begin(), tolower);

		for (int i = 0; i < GetFieldCount(); i++)
		{
			if (!m_Field[i]->GetFieldName().compare(sFieldName))
				return m_Field[i];
		}
		return NULL;
	}

	DBField* GetField(const char *lpszFieldName) const
	{
		for (int i = 0; i < GetFieldCount(); i++)
		{
			std::string strTmpField = m_Field[i]->GetFieldName();

			//std::transform(strTmpField.begin(), strTmpField.end(), strTmpField.begin(), tolower);

			if (!strTmpField.compare(lpszFieldName))
			{
				return m_Field[i];
			}
		}
		return NULL;
	}
};

class DBConnParam
{
public:
	DBConnParam & operator = (const DBConnParam& dbConnParam)
	{
		if (&dbConnParam == this)
		{
			return *this;
		}

		m_strPoolName = dbConnParam.m_strPoolName;
		m_enDBType = dbConnParam.m_enDBType;
		m_strHost = dbConnParam.m_strHost;
		m_usPort = dbConnParam.m_usPort;
		m_strUser = dbConnParam.m_strUser;
		m_strPasswd = dbConnParam.m_strPasswd;
		m_strDBName = dbConnParam.m_strDBName;
		m_usMinConn = dbConnParam.m_usMinConn;
		m_usMaxConn = dbConnParam.m_usMaxConn;

		return *this;
	}

	inline std::string GetPoolName() const { return m_strPoolName; }
	inline void SetPoolName(const std::string &strPoolName) { m_strPoolName = strPoolName; }

	inline DATABASE_TYPES GetDBType() const { return m_enDBType; }
	inline void SetDBType(const DATABASE_TYPES &enDBType) { m_enDBType = enDBType; }

	inline std::string GetHost() const { return m_strHost; }
	inline void SetHost(const std::string &strHost) { m_strHost = strHost; }

	inline std::string GetUser() const { return m_strUser; }
	inline void SetUser(const std::string &strUser) { m_strUser = strUser; }

	inline std::string GetPasswd() const { return m_strPasswd; }
	inline void SetPasswd(const std::string &strPasswd) { m_strPasswd = strPasswd; }

	inline std::string GetDBName() const { return m_strDBName; }
	inline void SetDBName(const std::string &strDB) { m_strDBName = strDB; }

	inline unsigned short GetPort() const { return m_usPort; }
	inline void SetPort(unsigned short usPort) { m_usPort = usPort; }

	inline unsigned short GetMinConn() const { return m_usMinConn; }
	inline void SetMinConn(const unsigned short &usMinConn) { m_usMinConn = usMinConn; }

	inline unsigned short GetMaxConn() const { return m_usMaxConn; }
	inline void SetMaxConn(const unsigned short &usMaxConn) { m_usMaxConn = usMaxConn; }

private:
	std::string		m_strPoolName;
	DATABASE_TYPES	m_enDBType;
	std::string		m_strHost;
	unsigned short	m_usPort;
	std::string		m_strUser;
	std::string		m_strPasswd;
	std::string		m_strDBName;
	unsigned short	m_usMinConn;
	unsigned short	m_usMaxConn;
};

enum SQL_OPERTAION
{
	KKDBSQL_SELECT,
	KKDBSQL_UPDATE,
	KKDBSQL_INSERT,
	KKDBSQL_DELETE,
	KKDBSQL_EXEC,
	KKDBSQL_BEGINTRANS,
	KKDBSQL_ENDTRANS,
	KKDBSQL_ROLLBACK
};

class IDBConnSink
{
public:
	virtual void OnDbAffectedOperation(SQL_OPERTAION SqlOperation, char const *pDb,
		DATABASE_TYPES DBType, char const *sql, int iBlobCount,
		BYTE const *const *const ppBlobs, long const *lBlobLens) = 0;
};

class IDBConn
{
public:
	IDBConn() { m_pConnSink = NULL; }
	virtual ~IDBConn() {}

public:
	virtual bool IsValid() = 0;
	virtual bool ConnectToDb() = 0;
	virtual void DisconnectFromDb() = 0;
	virtual void DbClearConnState() = 0;

public:
	virtual int DbInsert(const char *table, const char *value) = 0;
	virtual int DbInsert(const char *table, const char *field,
		const char *value, const char *incfield = NULL) = 0;
	virtual int DbUpdate(const char *table, const char *value, const char *condition) = 0;
	virtual int DbDelete(const char *table, const char *condition) = 0;
	virtual int DbExec(const char *execbuf, bool isinsert = false,
		const char *incfield = NULL) = 0;

	virtual DBRecordset *DbSelect(const char *table, const char *columns,
		const char *condition, const char *clause, int offset = 0,
		int count = __MAXCOUNT__, bool bErrLog = true) = 0;

	virtual DBRecordset *DbQuery(const char *querybuf, int offset = 0,
		int count = __MAXCOUNT__, bool bErrLog = true) = 0;
	virtual bool DbFreeRecordset(DBRecordset *pRes) = 0;

	virtual char *DbBlob2Str(const char *blob, unsigned int len) = 0;
	virtual bool DbFreeBlobStr(char *blobstr) = 0;

	virtual int DbBeginTransact() = 0;
	virtual int DbEndTransact() = 0;
	virtual int DbRollBack() = 0;

	virtual unsigned __int64 DbGetAffectedCount() = 0;
	virtual unsigned __int64 DbGetInsertId() = 0;

public:
	inline void SetSink(IDBConnSink *obj) { m_pConnSink = obj; }
	inline IDBConnSink *GetSink() { return m_pConnSink; }

	virtual void SetDbConnParam(const DBConnParam& dbConnParam) { m_dbConnParam = dbConnParam; };
	const DBConnParam* GetDbConnParam() { return &m_dbConnParam; }

	// should reWrite for force set db blob data
	void ForceSetBlobData(int iCount, BYTE const *const *ppByte, unsigned int const *pLen) { Assert(0); }

protected:
	virtual int OutputErrorInfo(int nErrorCode, const char *msg) = 0;

private:
	IDBConnSink *m_pConnSink;
	DBConnParam m_dbConnParam;

public:
	static bool CheckConn(IDBConn *lpConn) {

		if (NULL == lpConn) {
			return false;
		}

		if (!lpConn->IsValid()) {
			lpConn->DisconnectFromDb();
			return lpConn->ConnectToDb();
		}

		return true;
	}

	static bool IsExistField(std::map<std::string, bool> mapMustField, std::string strField) {

		return (mapMustField.find(strField) != mapMustField.end());
	}

	static bool GetItemSum(IDBConn *lpConn, const std::string &tabName, unsigned long &ulCount)
	{
		std::string strQuery = "select count(*) from ";
		strQuery += tabName;

		bool ret = false;
		DBRecordset* pRes = lpConn->DbQuery(strQuery.c_str());
		if (NULL == pRes) {
			return ret;
		}

		pRes->Begin();
		if (!pRes->IsEnd()) {
			DBField *pField = pRes->GetField(0);
			unsigned long long ullA = lpConn->DbGetAffectedCount();
			if (NULL != pField
				//&& lpConn->DbGetAffectedCount() != 0
				&& !pField->IsNull()) {
				ulCount = pField->AsULong();
				ret = true;
			}
		}

		SAFE_DELETE(pRes);
		return ret;
	}

protected:
	size_t GetEscapWordsCount(const char *value) {

		if (NULL == value) {
			return 0;
		}

		size_t iCount = 0;
		size_t iLen = strlen(value);
		size_t i = 0;
		while (i < iLen) {
			if ('\\' == value[i]) {
				++iCount;
				i += 2;
			}
			else{
				i++;
			}
		}

		return iCount;
	}

protected:
	void SprintfWithLocalEscapWords(char *szInput, const char *expression, 
		const char *value) {

		size_t len = strlen(value);
		char *bufValue = (char*)_alloca(len * 2);
		EscapWords(value, bufValue, len);

		sprintf(szInput, expression, bufValue);
	}

	void SprintfWithLocalEscapWords(char *szInput, const char *expression, 
		const char *table, const char *value) {

		size_t len = strlen(value);
		char *bufValue = (char*)_alloca(len * 2);
		EscapWords(value, bufValue, len);

		sprintf(szInput, expression, table, bufValue);
	}

	void SprintfWithLocalEscapWords(char *szInput, const char *expression,
		const char *table, const char *field, const char *value) {

		size_t len = strlen(value);
		char *bufValue = (char*)_alloca(len * 2);
		EscapWords(value, bufValue, len);
		len = strlen(bufValue);
		sprintf(szInput, expression, table, field, bufValue);
	}

	void EscapWords(const char *inValue, char* &outValue, size_t nLen){

		size_t i = 0;
		size_t j = 0;
		while (i < nLen) {
			if ('\\' == inValue[i]) {
				outValue[j++] = '\'';
				outValue[j++] = inValue[++i];
			}
			else {
				outValue[j++] = inValue[i++];
			}
		}

		outValue[j] = '\0';
	}
};