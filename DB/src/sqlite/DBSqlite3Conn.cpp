#include ".\sqlite3\DBSqlite3Conn.h"

bool LoadParam(unsigned short usType) {

	return true;
}

DBSqlite3Recordset::DBSqlite3Recordset(HDBRES hRes) {

	m_bIsEnd = false;
	m_pStmt = (sqlite3_stmt*)hRes;
	Initialize(hRes);
}

DBSqlite3Recordset:: ~DBSqlite3Recordset() {

	DbFree();
}

bool DBSqlite3Recordset::Begin() {

	return true;
}

void DBSqlite3Recordset::Next() {

	int iRet;
	iRet = sqlite3_step(m_pStmt);
	if (iRet != SQLITE_ROW) {
		m_bIsEnd = true;
	}
}

bool DBSqlite3Recordset::IsEnd() {

	if (m_bIsEnd || m_iRowCount == 0) {
		return true;
	}

	return false;
}

bool DBSqlite3Recordset::End() {

	return false;
}

bool DBSqlite3Recordset::ReadRow(const char *pchFormat, ...) {

	va_list arglst;
	va_start(arglst, pchFormat);
	int i = 0;
	do {
		if (*pchFormat != ftNull && SQLITE_NULL == sqlite3_column_type(m_pStmt, i)) {
			va_arg(arglst, void*);
			continue;
		}

		switch (*pchFormat)
		{
		case ftBool: {
			*va_arg(arglst, bool*) = GetField(i)->AsBool(); // need testing
			break;
		}
		case ftChar: {
			*va_arg(arglst, char*) = GetField(i)->AsChar();
			break;
		}
		case ftUChar: {
			*va_arg(arglst, unsigned char*) = GetField(i)->AsUChar();
			break;
		}
		case ftShort: {
			*va_arg(arglst, short*) = GetField(i)->AsShort();
			break;
		}
		case ftUShort: {
			*va_arg(arglst, unsigned short*) = GetField(i)->AsUShort();
			break;
		}
		case ftInt: {
			*va_arg(arglst, int*) = GetField(i)->AsInt();
			break;
		}
		case ftUInt: {
			*va_arg(arglst, unsigned int *) = GetField(i)->AsUInt();
			break;
		}
		case ftLong: {
			*va_arg(arglst, long*) = GetField(i)->AsLong();
			break;
		}
		case ftULong: {
			*va_arg(arglst, unsigned long*) = GetField(i)->AsULong();
			break;
		}
		case ftFloat: {
			*va_arg(arglst, float*) = GetField(i)->AsFloat();
			break;
		}
		case ftDouble: {
			*va_arg(arglst, double*) = GetField(i)->AsDouble();
			break;
		}
		case ftInt64: {
			*va_arg(arglst, __int64*) = GetField(i)->AsInt64();
			break;
		}
		case ftUInt64: {
			*va_arg(arglst, unsigned __int64*) = GetField(i)->AsUInt64();
			break;
		}
		case ftString: {
			*va_arg(arglst, std::string*) = GetField(i)->AsString();
			break;
		}
		case ftBlob: {
// 			HZByteArray *bytearr = va_arg(arglst, HZByteArray *);
// 			bytearr->Init();
// 			bytearr->RemoveAll();
// 
// 			int len = GetField(i)->GetLength();
// 			if (len < 0)
// 				continue;
// 
// 			BYTE *pResult = new BYTE[len];
// 
// 			GetField(i)->AsBlob(pResult, len);
// 			bytearr->SetData(pResult, len);
// 
// 			delete[] pResult;
 			break;
		}
		case ftNull: {
			va_arg(arglst, void *);
			i--;
			break;
		}
		default:
			return false;
		}

	} while (i++, *++pchFormat != 0);

	va_end(arglst);
	return true;
}

void DBSqlite3Recordset::Initialize(HDBRES hRes) {

	m_iRowCount = sqlite3_data_count((sqlite3_stmt*)hRes);
	m_iFieldCount = sqlite3_column_count((sqlite3_stmt*)hRes);
	m_Field = (DBField**) new BYTE[m_iFieldCount *sizeof(DBField *)];
	for (int i = 0; i < m_iFieldCount; i++) { // 准备好 Field 
		m_Field[i] = new DBSqlite3Field(this, i);
	}
}

void DBSqlite3Recordset::DbFree() {

	if (NULL != m_Field) {
		for (int i = 0; i < GetFieldCount(); i++) {
			if (NULL != m_Field[i]) {
				SAFE_DELETE(m_Field[i]);
			}
		}

		if (NULL != m_Field) {
			delete[](BYTE*)m_Field;
			m_Field = NULL;
		}
	}

	// sqlite3_stmt *m_pStmt
	sqlite3_finalize(m_pStmt);
}

DBSqlite3Field::DBSqlite3Field(const DBRecordset* pRes, unsigned int iIndex) : DBField(pRes, iIndex) {

	Init(pRes, iIndex);
}

DBSqlite3Field::~DBSqlite3Field()
{

}

void DBSqlite3Field::Init(const DBRecordset* pRes, unsigned int iIndex)
{
	m_pRes = (DBSqlite3Recordset*)pRes;
	m_iIndex = iIndex;
	m_sFieldName = sqlite3_column_name(m_pRes->m_pStmt, iIndex);
}

bool DBSqlite3Field::IsNull()
{
	return sqlite3_column_type(m_pRes->m_pStmt, m_iIndex) == SQLITE_NULL;
}

bool DBSqlite3Field::AsBool()
{
	unsigned char ch = *sqlite3_column_text(m_pRes->m_pStmt, m_iIndex);
	return (bool)(ch != 0);
}

char DBSqlite3Field::AsChar()
{
	unsigned char ch = *sqlite3_column_text(m_pRes->m_pStmt, m_iIndex);
	return (char)ch;
}

unsigned char DBSqlite3Field::AsUChar()
{
	unsigned char usCH = *sqlite3_column_text(m_pRes->m_pStmt, m_iIndex);
	return usCH;
}

short DBSqlite3Field::AsShort()
{
	int sqlValue = sqlite3_column_int(m_pRes->m_pStmt, m_iIndex);
	return (short)sqlValue;
}

unsigned short DBSqlite3Field::AsUShort()
{
	int sqlValue = sqlite3_column_int(m_pRes->m_pStmt, m_iIndex);
	return (unsigned short)sqlValue;
}

int DBSqlite3Field::AsInt()
{
	return sqlite3_column_int(m_pRes->m_pStmt, m_iIndex);
}

unsigned int DBSqlite3Field::AsUInt()
{
	return (unsigned int)sqlite3_column_int(m_pRes->m_pStmt, m_iIndex);
}

long DBSqlite3Field::AsLong()
{
	long sqlValue = (long)sqlite3_column_int(m_pRes->m_pStmt, m_iIndex);
	return sqlValue;
}

unsigned long DBSqlite3Field::AsULong()
{
	unsigned long sqlValue = (unsigned long)sqlite3_column_int(m_pRes->m_pStmt, m_iIndex);
	return (unsigned long)sqlValue;
}

float DBSqlite3Field::AsFloat()
{
	double sqlValue = sqlite3_column_double(m_pRes->m_pStmt, m_iIndex);
	return (float)sqlValue;
}

double DBSqlite3Field::AsDouble()
{
	return sqlite3_column_double(m_pRes->m_pStmt, m_iIndex);
}

__int64 DBSqlite3Field::AsInt64()
{
	return sqlite3_column_int64(m_pRes->m_pStmt, m_iIndex);
}

unsigned __int64 DBSqlite3Field::AsUInt64()
{
	return (unsigned __int64)sqlite3_column_int64(m_pRes->m_pStmt, m_iIndex);
}

std::string DBSqlite3Field::AsString()
{
	return std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_pRes->m_pStmt, m_iIndex)));
}

BYTE *DBSqlite3Field::AsBlob(BYTE *pDest, int iLen) {

	const void *pchBlobStart = sqlite3_column_blob(m_pRes->m_pStmt, m_iIndex);
	int iBlobLen = sqlite3_column_bytes(m_pRes->m_pStmt, m_iIndex);
	if (iLen < iBlobLen) {
		return NULL;
	}

	memcpy(pDest, pchBlobStart, iBlobLen);

	return pDest;
}

std::string DBSqlite3Field::GetFieldName()
{
	return m_sFieldName;
}

unsigned int DBSqlite3Field::GetLength()
{
	return (unsigned int)sqlite3_column_bytes(m_pRes->m_pStmt, m_iIndex);
}

unsigned int DBSqlite3Field::GetIndex()
{
	return m_iIndex;
}

HDBDATA DBSqlite3Field::GetData()
{
	return Value();
}

HDBDATA DBSqlite3Field::Value()
{
	return NULL;
}
//////////////////////////////////////////////////////////////////////////

DBSqlite3Conn::DBSqlite3Conn() {

	m_ppv = NULL;
	m_nBolbCount = NULL;
	m_pStmt = NULL;
	m_pConn = NULL;
}

DBSqlite3Conn::~DBSqlite3Conn() {

	DbClearConnState();
	if (IsValid()) sqlite3_close(m_pConn);
}

bool DBSqlite3Conn::IsValid() {

	return (NULL != m_pConn);
}

bool DBSqlite3Conn::ConnectToDb() {

	if (IsValid()) {
		return false;
	}

	int ret = sqlite3_open(GetDbConnParam()->GetDBName().c_str(), &m_pConn);
	if (SQLITE_OK != ret) {
		if (NULL != m_pConn) {
			OutputErrorInfo(ret, sqlite3_errmsg(m_pConn));
		}
		else {
			OutputErrorInfo(1000, "ConnectToDb Fail\n");
		}

		return false;
	}

#ifdef SQLITE_HAS_CODEC
	size_t len = GetDbConnParam()->GetPasswd().length();
	ret = sqlite3_key(m_pConn, (const void*)GetDbConnParam()->GetPasswd().c_str(), (int)len);
	if (SQLITE_OK != ret) {
		DisconnectFromDb();
		return false;
	}
#endif // SQLITE_HAS_CODEC

	return true;
}

void DBSqlite3Conn::DisconnectFromDb() {

	DbClearConnState();

	if (IsValid()) {
		sqlite3_close(m_pConn);
	}

	m_pConn = NULL;
}

void DBSqlite3Conn::DbClearConnState() {

	if (NULL != m_ppv) {
		for (int i = 0; i < MAX_BLOBNUM; i++) {
			SAFE_ARRAY_DELETE(m_ppv[i]);
		}
		SAFE_ARRAY_DELETE(m_ppv);
	}

	m_nBolbCount = 0;

	if (NULL != m_pStmt) {
		sqlite3_finalize(m_pStmt);
		m_pStmt = NULL;
	}
}

int DBSqlite3Conn::DbInsert(const char *table, const char *value) {

	int iRet;
	if (NULL == table || NULL == value) return -1;
	if (!IsValid()) return -2;

	size_t iEscaps = GetEscapWordsCount(value);

	char *szInput = (char *) new char[MINSTRLEN + strlen(table) + strlen(value) + iEscaps];
	if (NULL == szInput) return -3;

	if (0 == iEscaps) {
		sprintf(szInput, "INSERT INTO %s VALUES (%s)\0 ", table, value);
	}
	else {
		SprintfWithLocalEscapWords(szInput, "INSERT INTO %s VALUES (%s)\0 ", table, value);
	}

	do {
		if (HasBlobValue(szInput)) {
			iRet = PrepareStmt(szInput, strlen(szInput));
			if (SQLITE_OK != iRet) break;

			iRet = BandingBolbs(szInput);
			if (SQLITE_OK != iRet) break;

			iRet = DoSqlStmt();
		}
		else {
			iRet = QuicklySQL(szInput);
		}
	} while (false);

	if (SQLITE_DONE == iRet) iRet = SQLITE_OK;

	if (SQLITE_OK == iRet && NULL != GetSink()) {
		GetSink()->OnDbAffectedOperation(KKDBSQL_INSERT, NULL, dtSQLITE3, szInput, 0, NULL, NULL);
	}

	SAFE_ARRAY_DELETE(szInput);

	m_nBolbCount = 0;
	if (NULL != m_pStmt) {
		sqlite3_finalize(m_pStmt);
		m_pStmt = NULL;
	}

	return iRet;
}

int DBSqlite3Conn::DbInsert(const char *table, const char *field, const char *value, const char *) {

	if (NULL == table || NULL == field || NULL == value) return -1;
	if (!IsValid()) return -2;

	size_t iEscaps = GetEscapWordsCount(value);

	char *szInput = (char *) new char[MINSTRLEN + strlen(table) + strlen(field) + strlen(value) + iEscaps];
	if (NULL == szInput) return -3;

	if (0 == iEscaps) {
		sprintf(szInput, "INSERT INTO %s (%s) VALUES (%s) ", table, field, value);
	}
	else {
		SprintfWithLocalEscapWords(szInput, "INSERT INTO %s (%s) VALUES (%s)  ", table, field, value);
	}

	int iRet;
	do {
		if (HasBlobValue(szInput)) {
			iRet = PrepareStmt(szInput, strlen(szInput));
			if (SQLITE_OK != iRet) break;

			iRet = BandingBolbs(szInput);
			if (SQLITE_OK != iRet) break;

			iRet = DoSqlStmt();
		}
		else {
			iRet = QuicklySQL(szInput);
		}
	} while (false);

	if (SQLITE_DONE == iRet) iRet = SQLITE_OK;

	if (SQLITE_OK == iRet && NULL != GetSink()) {
		GetSink()->OnDbAffectedOperation(KKDBSQL_INSERT, NULL, dtSQLITE3, szInput, 0, NULL, NULL);
	}

	SAFE_ARRAY_DELETE(szInput);

	m_nBolbCount = 0;
	if (NULL != m_pStmt) {
		sqlite3_finalize(m_pStmt);
		m_pStmt = NULL;
	}

	return iRet;
}

int DBSqlite3Conn::DbUpdate(const char *table, const char *value, const char *condition) {

	if (NULL == table || NULL == value) return -1;

	size_t iLenValue = GetEscapWordsCount(value);
	size_t iLenCondition = GetEscapWordsCount(condition);

	char *szUpdate = NULL;
	if (NULL != condition) {
		szUpdate = (char*) new char[MINSTRLEN + strlen(table) + strlen(condition) + strlen(value) + iLenValue + iLenCondition];
		if (NULL == szUpdate) return -3;

		if (strlen(condition) > 0)
			sprintf(szUpdate, "UPDATE %s SET %s WHERE %s", table, value, condition);
		else
			sprintf(szUpdate, "UPDATE %s SET %s", table, value);
	}
	else {
		szUpdate = (char*) new char[MINSTRLEN + strlen(table) + strlen(value) + iLenValue];
		if (NULL == szUpdate) return -3;

		sprintf(szUpdate, "UPDATE %s SET %s", table, value);
	}

	if (iLenValue + iLenCondition > 0) {
		SprintfWithLocalEscapWords(szUpdate, "%s", szUpdate);
	}

	int iRet;
	do {
		if (HasBlobValue(szUpdate)) {
			iRet = PrepareStmt(szUpdate, strlen(szUpdate));
			if (SQLITE_OK != iRet) break;
			iRet = BandingBolbs(szUpdate);
			if (SQLITE_OK != iRet)  break;

			iRet = DoSqlStmt();
		}
		else {
			iRet = QuicklySQL(szUpdate);
		}
	} while (0);

	if (SQLITE_DONE == iRet) iRet = SQLITE_OK;

	if (SQLITE_OK == iRet && NULL != GetSink()) {
		GetSink()->OnDbAffectedOperation(KKDBSQL_UPDATE, NULL, dtSQLITE3, szUpdate, 0, NULL, NULL);
	}

	SAFE_ARRAY_DELETE(szUpdate);

	m_nBolbCount = 0;
	if (NULL != m_pStmt) {
		sqlite3_finalize(m_pStmt);
		m_pStmt = NULL;
	}

	return iRet;
}

int DBSqlite3Conn::DbDelete(const char *table, const char *condition) {

	if (NULL == table) return -1;

	size_t iLenDel = GetEscapWordsCount(condition);

	char *szDel = NULL;
	if (NULL != condition) {
		szDel = (char*) new char[MINSTRLEN + strlen(table) + strlen(condition) + iLenDel];
		if (NULL == szDel) return -3;

		if (strlen(condition) > 0) {
			sprintf(szDel, "DELETE FROM %s WHERE %s", table, condition);
		}
		else {
			sprintf(szDel, "DELETE FROM %s WHERE 1", table);
		}
	}
	else {
		szDel = (char*) new char[MINSTRLEN + strlen(table)];
		if (NULL == szDel) return -3;

		sprintf(szDel, "DELETE FROM %s WHERE 1", table);
	}

	if (iLenDel > 0) {
		SprintfWithLocalEscapWords(szDel, "%s", szDel);
	}

	int nRetcode = QuicklySQL(szDel);
	if (SQLITE_DONE == nRetcode) {
		nRetcode = 0;
	}

	if (SQLITE_OK == nRetcode && NULL != GetSink()) {
		GetSink()->OnDbAffectedOperation(KKDBSQL_UPDATE, NULL, dtSQLITE3, szDel, 0, NULL, NULL);
	}

	SAFE_ARRAY_DELETE(szDel);

	return nRetcode;
}

int DBSqlite3Conn::DbExec(const char *execbuf, bool isinsert, const char *incfield) {

	size_t iLenExec = GetEscapWordsCount(execbuf);

	char *tmpbufLink = NULL;
	char *tmpbuf = NULL;
	if (iLenExec > 0) {
		tmpbuf = new char[strlen(execbuf) + iLenExec * 2];
		if (NULL == iLenExec) return -3;

		SprintfWithLocalEscapWords(tmpbuf, "%s", execbuf);
		tmpbufLink = tmpbuf;
	}
	else {
		tmpbufLink = (char*)execbuf;
	}

	int iRet;
	do {
		if (HasBlobValue(tmpbufLink)) {
			iRet = PrepareStmt(tmpbufLink, strlen(tmpbufLink));
			if (SQLITE_OK != iRet) break;
			iRet = BandingBolbs(tmpbufLink);
			if (SQLITE_OK != iRet) break;

			iRet = DoSqlStmt();
		}
		else {
			iRet = QuicklySQL(tmpbufLink);
		}
	} while (false);

	if (SQLITE_DONE == iRet) iRet = SQLITE_OK;

	SAFE_ARRAY_DELETE(tmpbuf);

	m_nBolbCount = 0;
	if (m_pStmt) {
		sqlite3_finalize(m_pStmt);
		m_pStmt = NULL;
	}

	return iRet;
}

DBRecordset *DBSqlite3Conn::DbSelect(const char *table, const char *columns,
	const char *condition, const char *clause, int offset, int count, bool bErrLog) {

	if (NULL == table || NULL == columns) return NULL;

	size_t nSQLLen = 0;
	if (NULL != condition) nSQLLen += strlen(condition);
	if (NULL != clause) nSQLLen += strlen(clause);

	size_t iLenSel = GetEscapWordsCount(condition);

	char *szQuery = (char*) new char[MINSTRLEN + strlen(table) + strlen(columns) + nSQLLen + iLenSel];
	if (NULL == szQuery) return NULL;

	sprintf(szQuery, "SELECT %s FROM %s", columns, table);
	if (NULL != condition && strlen(condition) > 0) {
		sprintf(szQuery, "%s WHERE %s", szQuery, condition);
	}

	if (NULL != clause && strlen(clause) > 0) {
		sprintf(szQuery, "%s %s", szQuery, clause);
	}

	if (count != __MAXCOUNT__ || offset != 0) {
		sprintf(szQuery, "%s LIMIT %d, %d\0", szQuery, offset, count);
	}
	else {
		sprintf(szQuery, "%s\0", szQuery);
	}

	if (iLenSel > 0) {
		SprintfWithLocalEscapWords(szQuery, "%s", szQuery);
	}

	int iRet;
	iRet = PrepareStmt(szQuery, strlen(szQuery));
	if (SQLITE_OK != iRet) {
		SAFE_ARRAY_DELETE(szQuery);
		return NULL;
	}

	iRet = DoSqlStmt();

	SAFE_ARRAY_DELETE(szQuery);

	if (iRet == SQLITE_DONE || iRet == SQLITE_ROW) {
		DBRecordset* lp = new DBSqlite3Recordset(HDBRES(this->m_pStmt));
		m_pStmt = NULL;
		return lp;
	}

	if (NULL != m_pStmt) {
		sqlite3_finalize(m_pStmt);
	}

	return NULL;
}

DBRecordset *DBSqlite3Conn::DbQuery(const char *querybuf, int offset, 
	int count, bool bErrLog) {

	Assert(querybuf);
	if (!IsValid() || NULL == querybuf) return NULL;

	size_t iLenQ = GetEscapWordsCount(querybuf);
	char *szQuery = (char*) new char[MINSTRLEN + strlen(querybuf) + iLenQ];
	if (NULL == szQuery) return NULL;

	if (count != __MAXCOUNT__ || offset != 0) {
		sprintf(szQuery, "%s LIMIT %d, %d\0", querybuf, offset, count);
	}
	else {
		sprintf(szQuery, "%s\0", querybuf);
	}

	if (iLenQ > 0) {
		SprintfWithLocalEscapWords(szQuery, "%s", szQuery);
	}

	int iRet;
	iRet = PrepareStmt(szQuery, strlen(szQuery));

	if (SQLITE_OK != iRet) {
		SAFE_ARRAY_DELETE(szQuery);
		return NULL;
	}

	iRet = DoSqlStmt();

	SAFE_ARRAY_DELETE(szQuery);

	if (iRet == SQLITE_DONE || iRet == SQLITE_ROW) {
		DBRecordset* lp = new DBSqlite3Recordset(HDBRES(this->m_pStmt));
		m_pStmt = NULL;
		return lp;
	}

	if (NULL != m_pStmt) {
		sqlite3_finalize(m_pStmt);
	}

	return NULL;
}

bool DBSqlite3Conn::DbFreeRecordset(DBRecordset *pRes) {

	if (NULL == pRes)
		return false;

	SAFE_DELETE(pRes);

	return true;
}

char *DBSqlite3Conn::DbBlob2Str(const char *blob, unsigned int len) {

	if (!IsValid()) return NULL;

	if (NULL == m_ppv) {
		m_ppv = (BYTE**)new BYTE[MAX_BLOBNUM *sizeof(BYTE*)];
		for (int i = 0; i < MAX_BLOBNUM; i++)
			m_ppv[i] = new BYTE[MAX_BLOBNUM];
	}

	if (m_nBolbCount < MAX_BLOBNUM) {
		try {
			memcpy(m_ppv[m_nBolbCount], blob, len);
			m_nBolbLen[m_nBolbCount] = len;

			char *pchBlobStrID = new char[5];
			sprintf(pchBlobStrID, "?%03d\0", m_nBolbCount + 1);

			m_nBolbCount++;

			return pchBlobStrID;
		}
		catch (...) {
			return NULL;
		}
	}

	return NULL;
}

bool DBSqlite3Conn::DbFreeBlobStr(char *blobstr) {

	if (blobstr) {
		delete[] blobstr;
		return true;
	}

	return false;
}


int DBSqlite3Conn::DbBeginTransact() {
	return sqlite3_exec(m_pConn, "begin;", 0, 0, NULL);
	//return sqlite3_prepare_v2(m_pConn, "begin;", strlen("begin;"), &m_pStmt, NULL);
}

int DBSqlite3Conn::DbEndTransact() {
	return sqlite3_exec(m_pConn, "commit;", 0, 0, NULL);
	//return sqlite3_prepare_v2(m_pConn, "commit;", strlen("commit;"), &m_pStmt, NULL);
}

int DBSqlite3Conn::DbRollBack() {
	return sqlite3_exec(m_pConn, "rollback;", 0, 0, NULL);
	//return sqlite3_prepare_v2(m_pConn, "rollback;", strlen("rollback;"), &m_pStmt, NULL);
}

unsigned __int64 DBSqlite3Conn::DbGetAffectedCount() {
	// 功能:获取最近执行的sqlite3_exec影响的行数.
	return sqlite3_changes(m_pConn);
}

unsigned __int64 DBSqlite3Conn::DbGetInsertId() {
	return (unsigned __int64)sqlite3_last_insert_rowid(m_pConn);
}

int DBSqlite3Conn::OutputErrorInfo(int nErrorCode, const char *msg) {

	std::string errorCode = "";
	switch (nErrorCode)
	{
	case SQLITE_OK:
		errorCode = "/* 成功 | Successful result"; break;
	case  SQLITE_ERROR:
		errorCode = "SQL错误 或 丢失数据库 | SQL error or missing database"; break;
	case SQLITE_INTERNAL:
		errorCode = "SQLite 内部逻辑错误 | Internal logic error in SQLite"; break;
	case SQLITE_PERM:
		errorCode = "拒绝访问 | Access permission denied"; break;
	case SQLITE_ABORT:
		errorCode = "回调函数请求取消操作 | Callback routine requested an abort"; break;
	case SQLITE_BUSY:
		errorCode = "数据库文件被锁定 | The database file is locked"; break;
	case SQLITE_LOCKED:
		errorCode = "数据库中的一个表被锁定 | A table in the database is locked"; break;
	case SQLITE_NOMEM:
		errorCode = "某次 malloc() 函数调用失败 | A malloc() failed"; break;
	case SQLITE_READONLY:
		errorCode = "尝试写入一个只读数据库 | Attempt to write a readonly database"; break;
	case SQLITE_INTERRUPT:
		errorCode = "操作被 sqlite3_interupt() 函数中断 | Operation terminated by sqlite3_interrupt()"; break;
	case SQLITE_IOERR:
		errorCode = "发生某些磁盘 I/O 错误 | Some kind of disk I/O error occurred"; break;
	case SQLITE_CORRUPT:
		errorCode = "数据库磁盘映像不正确 | The database disk image is malformed"; break;
	case SQLITE_NOTFOUND:
		errorCode = "sqlite3_file_control() 中出现未知操作数 | Unknown opcode in sqlite3_file_control()"; break;
	case SQLITE_FULL:
		errorCode = "因为数据库满导致插入失败 | Insertion failed because database is full"; break;
	case SQLITE_CANTOPEN:
		errorCode = "无法打开数据库文件 | Unable to open the database file"; break;
	case SQLITE_PROTOCOL:
		errorCode = "数据库锁定协议错误 | Database lock protocol error"; break;
	case SQLITE_EMPTY:
		errorCode = "数据库为空 | Database is empty"; break;
	case SQLITE_SCHEMA:
		errorCode = "数据结构发生改变 | The database schema changed"; break;
	case SQLITE_TOOBIG:
		errorCode = "字符串或二进制数据超过大小限制 | String or BLOB exceeds size limit"; break;
	case SQLITE_CONSTRAINT:
		errorCode = "由于约束违例而取消 | Abort due to constraint violation"; break;
	case SQLITE_MISMATCH:
		errorCode = "数据类型不匹配 | Data type mismatch"; break;
	case SQLITE_MISUSE:
		errorCode = "不正确的库使用 | Library used incorrectly"; break;
	case SQLITE_NOLFS:
		errorCode = "使用了操作系统不支持的功能 | Uses OS features not supported on host"; break;
	case SQLITE_AUTH:
		errorCode = "授权失败 | Authorization denied"; break;
	case SQLITE_FORMAT:
		errorCode = "附加数据库格式错误 | Auxiliary database format error"; break;
	case SQLITE_RANGE:
		errorCode = "传递给sqlite3_bind()的第二个参数超出范围 | 2nd parameter to sqlite3_bind out of range"; break;
	case SQLITE_NOTADB:
		errorCode = "被打开的文件不是一个数据库文件 | File opened that is not a database file"; break;
	case SQLITE_ROW:
		errorCode = "sqlite3_step() 已经产生一个行结果 | sqlite3_step() has another row ready"; break;
	case SQLITE_DONE:
		errorCode = "sqlite3_step() 完成执行操作 | sqlite3_step() has finished executing"; break;
	default:
		break;
	}

	//printf("errorCode %s.\r\n", errorCode.c_str());

	return 0;
}

bool DBSqlite3Conn::HasBlobValue(const char *pchSql) {

	if (NULL == pchSql)  return false;
	size_t slen = strlen(pchSql);

	bool bIns = false;
	bool bInm = false;
	int start_pos = 0;
	char *tmp = (char*)pchSql + start_pos;
	while (tmp < (pchSql + (slen - 2))) {
		switch (*tmp)
		{
		case '\\': 
		{ 
			tmp++; 
			break; 
		}
		case '?':
		{
			if (!(bIns || bInm))
			{
				return true;
			}
			tmp++;
			break;
		}
		case '\'':
		{
			if (!bInm)	bIns = !bIns;
			tmp++;
			break;
		}
		case '"':
		{
			if (!bIns)	bInm = !bInm;
			tmp++;
			break;
		}

		default: tmp++; break;
		}
	}

	return false;
}

int DBSqlite3Conn::PrepareStmt(const char *pchSql, size_t nBytes) {

	int iResult = sqlite3_prepare(m_pConn, pchSql, (int)nBytes, &m_pStmt, NULL);
	if (SQLITE_OK != iResult) {
		// printf("sql=%s.\r\n", pchSql);
		OutputErrorInfo(iResult, sqlite3_errmsg(m_pConn));
	}

	return iResult;
}

int DBSqlite3Conn::BandingBolbs(const char *pchSql) {

	int iTmpBlobID;
	int iBindRet;
	for (size_t i = 0; i < m_nBolbCount;) {
		if ((iTmpBlobID = FindBolbID(pchSql, i)) >= 0) {
			iBindRet = sqlite3_bind_blob(m_pStmt, iTmpBlobID + 1, m_ppv[iTmpBlobID], m_nBolbLen[iTmpBlobID], NULL);
			if (SQLITE_OK != iBindRet) {
				OutputErrorInfo(iBindRet, sqlite3_errmsg(m_pConn));
				return iBindRet;
			}
		}
	}

	return iBindRet;
}

int DBSqlite3Conn::FindBolbID(const char *pchSql, size_t &nStart) {

	if (NULL == pchSql) return -1;
	size_t len = strlen(pchSql);
	if (nStart > len - 2) return -1;

	const char *tmp = pchSql + nStart;

	bool bIns = false;
	bool bInm = false;

	char chBlobStrName[4] = { 0 };
	while (tmp < pchSql + len - 2) {
		switch (*tmp)
		{
		case '\\': { tmp++; break; }
		case '?':
		{
			if (!(bInm || bIns))
			{
				if (*(tmp + 1) < '0' || *(tmp + 1) > '9') break;
				chBlobStrName[0] = *(tmp + 1);

				if (*(tmp + 2) < '0' || *(tmp + 2) > '9') break;
				chBlobStrName[1] = *(tmp + 2);

				if (*(tmp + 3) < '0' || *(tmp + 3) > '9') break;
				chBlobStrName[2] = *(tmp + 3);

				chBlobStrName[3] = 0;
				//  compute id and nStart
				int iBlobID = atoi(chBlobStrName) - 1;
				nStart = tmp - pchSql + 4;
				return iBlobID;
			}
			break;
		}
		case '\'':
		{
			if (!bInm)	bIns = !bIns;
			break;
		}
		case '"':
		{
			if (!bIns)	bInm = !bInm;
			break;
		}
		}

		tmp++;
	}

	nStart = tmp - pchSql;

	return -1;
}

int DBSqlite3Conn::DoSqlStmt() {

	return sqlite3_step(m_pStmt);
}

int DBSqlite3Conn::QuicklySQL(const char *pchSql) {

	int iResult = sqlite3_exec(m_pConn, pchSql, NULL, NULL, NULL);
	if (SQLITE_OK != iResult) {
		// printf("%s.\r\n", pchSql);

		OutputErrorInfo(iResult, sqlite3_errmsg(m_pConn));
	}
	return iResult;
}

//////////////////////////////////////////////////////////////////////////
void DBAppendValueEscapeSqlite(std::string &strSrc, const char *value, bool bAddComma) {

	if (strSrc.length() > 0) strSrc += ", ";

	char *Comma = bAddComma ? "'" : "";

	strSrc += Comma;

	if (strlen(value) > 0) {
		char *tmpbuf = sqlite3_mprintf("%q", value);
		strSrc += tmpbuf;
		sqlite3_free(tmpbuf);
	}

	strSrc += Comma;
}

void DBAppendExpressionEscapeSqlite(std::string &strSrc, const char *szField, 
	const char *szOpCode, const char *szConnCode, const char *value, bool bAddComma) {

	if (strSrc.length() > 0) strSrc += szConnCode;

	char *Comma = bAddComma ? "'" : "";

	strSrc += szField;
	strSrc += " ";
	strSrc += szOpCode;
	strSrc += Comma;

	if (strlen(value) > 0) {
		char *tmpbuf = sqlite3_mprintf("%q", value);
		strSrc += tmpbuf;
		sqlite3_free(tmpbuf);
	}

	strSrc += Comma;
}