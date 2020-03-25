#include ".\mysql\DBMysqlConn.h"
#include ".\mysql\DBSetValue.h"

DBMySqlRecordset::DBMySqlRecordset(HDBRES hRes, int iFieldCount)
{
	m_Recordset = NULL;
	m_row = NULL;
	m_Field = NULL;
	m_rowindex = 0;
	SetFieldCount(iFieldCount);
	Initialize(hRes);
}

DBMySqlRecordset::~DBMySqlRecordset() { DbFree(); }

void DBMySqlRecordset::Initialize(HDBRES hRes)
{
	assert(hRes);

	DbFree();

	m_Field = (DBField**) new BYTE[m_iFieldCount * sizeof(DBField *)];

	int i = 0;
	for(i = 0; i < GetFieldCount(); i++)
	{
		m_Field[i] = NULL;
	}

	m_Recordset = (MYSQL_RES *)hRes;
	for(i = 0; i < m_iFieldCount; i++)
	{
		m_Field[i] = new DBMySqlField(this, i);
	}
	Begin();
}

bool DBMySqlRecordset::ReadRow(const char * pchFormat, ...)
{
	va_list arglst;
 
	va_start(arglst, pchFormat);
	int i = 0;
	do
	{
		if(*pchFormat != ftNull && m_row[i] == NULL && GetField(i)->GetLength() == 0) // need testing
		{
			va_arg(arglst, void*);
			continue;
		}

		switch(*pchFormat)
		{
			case ftBool:
			{	
   				*va_arg(arglst, bool*) = *(char *)m_row[i] != '0'; // need testing
     			break;
			}
			case ftChar:
			{
				*va_arg(arglst, char*) = *(char *)m_row[i];
				break;
			}
			case ftUChar:
			{
				*va_arg(arglst, unsigned char*) = *(unsigned char *)m_row[i];
				break;
			}
			case ftShort:
			{
				DBSetValue(*va_arg(arglst, short*), (char *)m_row[i]);				
				break;
			}
			case ftUShort:
			{
				DBSetValue(*va_arg(arglst, unsigned short*), (char *)m_row[i]);	
				break;
			}
			case ftInt:
			{
				DBSetValue(*va_arg(arglst, int*), (char *)m_row[i]);
				break;
			}
			case ftUInt:
			{
				DBSetValue(*va_arg(arglst, unsigned int *), (char *)m_row[i]);
				break;
			}
			case ftLong:
			{
				DBSetValue(*va_arg(arglst, long*), (char *)m_row[i]);
				break;
			}
			case ftULong:
			{
				DBSetValue(*va_arg(arglst, unsigned long*), (char *)m_row[i]);
				break;
			}
			case ftFloat:
			{
				DBSetValue(*va_arg(arglst, float*), (char *)m_row[i]);
				break;
			}
			case ftDouble:
			{
				DBSetValue(*va_arg(arglst, double*), (char *)m_row[i]);
				break;
			}
			case ftInt64:
			{
				DBSetValue(*va_arg(arglst, __int64*), (char *)m_row[i]);
				break;
			}
			case ftUInt64:
			{
				DBSetValue(*va_arg(arglst, unsigned __int64*), (char *)m_row[i]);
				break;
			}
			case ftString:
			{
				DBSetValue(*va_arg(arglst, std::string*), (char *)m_row[i]);
// 				*va_arg(arglst, std:string*) = (char *)m_row[i];
				break;
			}
			case ftBlob:
			{
// 				HZByteArray * bytearr = va_arg(arglst, KK_ByteArray *);
// 				bytearr->RemoveAll();
// 
// 				int len = GetField(i)->GetLength();
// 				if(len < 0)
// 					continue;
// 
// 				BYTE * pResult = new BYTE[len];
// 
// 				memcpy(pResult, m_row[i], len);
// 				bytearr->SetData(pResult, len);
// 
// 				delete[] pResult;
				break;
			}
			case ftNull:
			{
				va_arg(arglst, void *);
				i--;
				break;
			}
			default:
				return false;
		}
			
	}while(i++, *++pchFormat != 0);

	va_end(arglst);
    return true;
}

void DBMySqlRecordset::DbFree()
{
	m_row = NULL;	
	if(NULL != m_Field)
	{
		for(int i = 0; i < GetFieldCount(); i++)
		{
			if( m_Field[i] != NULL)
			{
				delete m_Field[i];
				m_Field[i] = NULL;
			}
		}
		delete [](BYTE*)m_Field;
		m_Field = NULL;
	}
	if(m_Recordset)
		mysql_free_result(m_Recordset);
}

bool DBMySqlRecordset::Begin()
{
	assert(m_Recordset);
	if(mysql_num_rows(m_Recordset) > 0 )
	{
		mysql_data_seek(m_Recordset, 0);
		m_row = mysql_fetch_row(m_Recordset);
		m_rowindex = 0;
//		SetFieldLen();
		return true;
	}
	return false;
}

void DBMySqlRecordset::Next()
{
	assert(m_Recordset);

	m_row = mysql_fetch_row(m_Recordset);
//	SetFieldLen();
	++m_rowindex;
}

bool DBMySqlRecordset::End()
{
	assert(m_Recordset);
	unsigned int iRow = GetRowCount();
	if (iRow > 0 ) 
	{
		mysql_data_seek(m_Recordset, iRow - 1) ;
		m_row = mysql_fetch_row(m_Recordset);
//		SetFieldLen();
		m_rowindex = iRow-1;
		return true;
	}
	return false;
}

void DBMySqlRecordset::SetFieldLen()
{
	unsigned long * len = mysql_fetch_lengths(m_Recordset);
	if(len == NULL)
		return;
	for(int i = 0; i < m_iFieldCount; i++)
		((DBMySqlField *)GetField(i))->SetLength(len[i]);
}

HDBDATA DBMySqlRecordset::GetFieldData(int iIndex)
{
	assert(m_row != NULL);
    return (HDBDATA) m_row[iIndex];
}

void DBMySqlRecordset::GetFieldInfo(DBMySqlField *pField)
{
	assert(pField->GetIndex() >= 0);
	assert((int)pField->GetIndex() < m_iFieldCount);

	mysql_field_seek(m_Recordset, pField->GetIndex());
	MYSQL_FIELD * tmpField = mysql_fetch_field(m_Recordset);
    if(NULL == tmpField)
		assert(false);

	pField->ResetFieldInfo(tmpField);
}

DBMySqlField::DBMySqlField(const DBRecordset* pRes, unsigned int iIndex) : DBField(pRes,iIndex)
{
	Init(pRes,iIndex);
}

DBMySqlField::~DBMySqlField() {}

void DBMySqlField::Init(const DBRecordset* pRes, unsigned int iIndex)
{
	assert(pRes);
	m_pRes = (DBMySqlRecordset*) pRes;
	assert((int)iIndex < m_pRes->m_iFieldCount);
	m_iIndex = iIndex;
	m_pRes->GetFieldInfo(this);
}

void DBMySqlField::ResetFieldInfo(MYSQL_FIELD* mysqlfield)
{
	m_sFieldName = mysqlfield->name;
}

HDBDATA DBMySqlField::Value()
{
	assert(m_pRes);
	return (HDBDATA) (m_pRes->GetFieldData(m_iIndex));
}

bool DBMySqlField::IsNull()
{
	return (m_pRes->GetFieldData( m_iIndex) == NULL && m_pRes->GetField(m_iIndex)->GetLength() == 0); // need testing
}

bool DBMySqlField::AsBool()
{
	assert(m_pRes);
	assert(!IsNull());
    return  (*(char *) m_pRes->GetFieldData(m_iIndex)) != '0';
}

char DBMySqlField::AsChar()
{
	assert(m_pRes);
	assert(!IsNull());
	return *(char *) m_pRes->GetFieldData(m_iIndex);
}

unsigned char DBMySqlField::AsUChar()
{
	assert(m_pRes);
	assert(!IsNull());
	//unsigned char ucRecord;
	//DBSetValue(ucRecord, (char *) m_pRes->GetFieldData(m_iIndex));
    return *(unsigned char*)m_pRes->GetFieldData(m_iIndex);
	//return ucRecord;
}

short DBMySqlField::AsShort()
{
	assert(m_pRes);
	assert(!IsNull());
	short sRecord;
	DBSetValue(sRecord, (char *) m_pRes->GetFieldData(m_iIndex));
	return sRecord;
}

unsigned short DBMySqlField::AsUShort() 
{
	assert(m_pRes);
	assert(!IsNull());
	unsigned short usRecord;
	DBSetValue(usRecord, (char *)m_pRes->GetFieldData(m_iIndex));
	return usRecord;
}

int DBMySqlField::AsInt()
{
	assert(m_pRes);
	assert(!IsNull());
	int iReslut;
	DBSetValue(iReslut, (char *)m_pRes->GetFieldData(m_iIndex));
	return iReslut;
}

unsigned int DBMySqlField::AsUInt()
{
	assert(m_pRes);
	assert(!IsNull());
	unsigned int uiRecord;
	DBSetValue(uiRecord, (char *)m_pRes->GetFieldData(m_iIndex));
	return uiRecord;
}

long DBMySqlField::AsLong()
{
	assert(m_pRes);
	assert(!IsNull());
	long lResult;
	DBSetValue(lResult, (char *)m_pRes->GetFieldData(m_iIndex));
	return lResult;
}

unsigned long DBMySqlField::AsULong()
{
	assert(m_pRes);
	assert(!IsNull());
	unsigned long ulResult;
	DBSetValue(ulResult, (char *)m_pRes->GetFieldData(m_iIndex));
	return ulResult;
}

float DBMySqlField::AsFloat()
{
	assert(m_pRes);
	assert(!IsNull());
	float fResult;
	DBSetValue(fResult, (char *)m_pRes->GetFieldData(m_iIndex));
	return fResult;
}

double DBMySqlField::AsDouble()
{
	assert(m_pRes);
	assert(!IsNull());
	double dResult;
	DBSetValue(dResult, (char *)m_pRes->GetFieldData(m_iIndex));
	return dResult;
}

__int64 DBMySqlField::AsInt64()
{
	assert(m_pRes);
	assert(!IsNull());
	__int64 iResult;
	DBSetValue(iResult, (char *)m_pRes->GetFieldData(m_iIndex));
	return iResult;
}

unsigned __int64 DBMySqlField::AsUInt64()
{
	assert(m_pRes);
	assert(!IsNull());
	unsigned __int64 uiResult;
	DBSetValue(uiResult, (char *)m_pRes->GetFieldData(m_iIndex));
	return uiResult;
}

std::string DBMySqlField::AsString()
{
	assert(m_pRes);
	assert(!IsNull());
	return (char *)m_pRes->GetFieldData(m_iIndex);
}


BYTE * DBMySqlField::AsBlob(BYTE * pDest, int iLen)
{
	assert(m_pRes);
	assert(!IsNull());
	assert(pDest);
	int len = GetLength();
	if(iLen <= len || len ==0)
		return NULL;
	BYTE * pResult = (BYTE *) m_pRes->GetFieldData(m_iIndex);
	memcpy(pDest, pResult, len);
	return pDest;
}

std::string DBMySqlField::GetFieldName()
{
	return m_sFieldName;
}

unsigned int DBMySqlField::GetLength()
{
	unsigned long * len = mysql_fetch_lengths(m_pRes->m_Recordset);
	if (len == NULL)
	{
		return -1;
	}

	m_length = len[m_iIndex];
	return m_length;
}

unsigned int DBMySqlField::GetIndex()
{
	return m_iIndex;
}

void DBMySqlField::SetLength(int len)
{
	m_length = len;
}

DBMySqlConn::DBMySqlConn() :IDBConn()
{
	m_pConn = NULL;
}

DBMySqlConn::~DBMySqlConn()
{
	DisconnectFromDb();	
}

bool DBMySqlConn::IsValid()
{
	 return (m_pConn != NULL); 
}

bool DBMySqlConn::ConnectToDb()
{
	if (IsValid()) return false;

	if ((m_pConn = mysql_init(NULL)) == NULL) {
		OutputErrorInfo(1000, "mysql_init failed\n");
		DisconnectFromDb();
		return false;
	}

	if ((mysql_real_connect(m_pConn, GetDbConnParam()->GetHost().c_str(), GetDbConnParam()->GetUser().c_str(),
		GetDbConnParam()->GetPasswd().c_str(), GetDbConnParam()->GetDBName().c_str(), GetDbConnParam()->GetPort(), NULL, 0)) == NULL) {
		OutputErrorInfo(1001, "mysql_real_connect failed\n");
		DisconnectFromDb();
		return false;
	}

	return true;
}

void DBMySqlConn::DisconnectFromDb()
{
	if (m_pConn) {
		mysql_close(m_pConn);
		m_pConn = NULL;
	}
}

int DBMySqlConn::DbInsert(const char * table, const char * value)
{
	Assert(table || value);
	if (!IsValid() || NULL == table || NULL == value) return 1;
    
	char * querybuf = (char*) new BYTE[256 + strlen(table) + strlen(value)];
   
    if (querybuf == NULL)
    {
        OutputErrorInfo(1002, "DbInsert new buf null.");
        return -1;
    }

	sprintf(querybuf, "insert into %s values ( %s )", table, value);
	int ret = mysql_query(m_pConn, querybuf);
	if (ret) ret = OutputErrorInfo(ret, querybuf);

	if (ret == CR_SERVER_LOST || ret == CR_SERVER_GONE_ERROR)
	{
		DisconnectFromDb();
		if(ConnectToDb())
		{
			ret = mysql_query(m_pConn, querybuf);
		}
        if (ret) OutputErrorInfo(ret, querybuf);
	}
    if (0 == ret && NULL != GetSink() && DbGetAffectedCount() > 0)
		GetSink()->OnDbAffectedOperation(KKDBSQL_INSERT, GetDbConnParam()->GetDBName().c_str(),
                                            GetDbConnParam()->GetDBType(),
                                            querybuf,
                                            0, NULL, NULL);
	delete[] (BYTE*)querybuf;

	return ret;
}

int DBMySqlConn::DbInsert(const char * table, const char * field, const char * value, const char * incfield)
{
	Assert(table || field || value);
    if (!IsValid() || NULL == table || NULL == field || NULL == value) return 1;

	char * querybuf = (char*)new BYTE[256 + strlen(table) + strlen(field) + strlen(value)];
   
    if (querybuf == NULL)
    {
        OutputErrorInfo(1004, "DbInsert new buf null.");
        return -1;
    }

	sprintf(querybuf, "insert into %s ( %s ) values ( %s )", table, field, value);
	int ret = mysql_query(m_pConn, querybuf);

	if (ret) ret = OutputErrorInfo(ret, querybuf);
	
	if (ret == CR_SERVER_LOST || ret == CR_SERVER_GONE_ERROR)
	{
		DisconnectFromDb();
		if(ConnectToDb())
		{
			ret = mysql_query(m_pConn, querybuf);
		}
		if (ret) OutputErrorInfo(ret, querybuf);
	}

	if (0 == ret && NULL != GetSink() && DbGetAffectedCount() > 0)
		GetSink()->OnDbAffectedOperation(KKDBSQL_INSERT, GetDbConnParam()->GetDBName().c_str(),
                                        GetDbConnParam()->GetDBType(),
                                        querybuf,
                                        0, NULL, NULL);
	delete[] (BYTE*)querybuf;
	return ret;
}

int DBMySqlConn::DbUpdate(const char * table, const char * value, const char * condition)
{
    Assert(table||value);
	if (!IsValid() || NULL == table || NULL == value) return 1;

	char * querybuf;
	if(condition != NULL)
		querybuf = (char*)new BYTE[256+strlen(table)+strlen(value)+strlen(condition)];
	else
		querybuf = (char*)new BYTE[256+strlen(table)+strlen(value)];

    if (querybuf == NULL)
    {
        OutputErrorInfo(1003, "DbUpdate new buf null.");
        return -1;
    }

	if (condition == NULL || strlen(condition) == 0)
		sprintf(querybuf, "update %s set %s", table, value);
	else
		sprintf(querybuf, "update %s set %s where %s", table, value, condition);
	
	int ret = mysql_query(m_pConn, querybuf);
    if (ret) ret = mysql_errno(m_pConn);
	
	if (ret == CR_SERVER_LOST || ret == CR_SERVER_GONE_ERROR)
	{
		DisconnectFromDb();
		if(ConnectToDb())
		{
			ret = mysql_query(m_pConn, querybuf);
		}
	}
	
	if (ret) ret = OutputErrorInfo(ret, querybuf);

	if (0 == ret && NULL != GetSink() && DbGetAffectedCount() > 0)
		GetSink()->OnDbAffectedOperation(KKDBSQL_UPDATE, GetDbConnParam()->GetDBName().c_str(),
                                        GetDbConnParam()->GetDBType(),
                                        querybuf,
                                        0, NULL, NULL);

	delete[] (BYTE*)querybuf;
	return ret;
}

int DBMySqlConn::DbDelete(const char * table, const char * condition)
{
    Assert(table);
	if (!IsValid() || NULL == table) return 1;
    char * querybuf = NULL;
    if (condition)
    	querybuf = (char*)new BYTE[(256+strlen(table)+strlen(condition))];
    else
        querybuf = (char*)new BYTE[(256+strlen(table)/*+strlen(condition)*/)];

    if (querybuf == NULL)
    {
        OutputErrorInfo(1003, "DbDelete new buf null.");
        return -1;
    }

	if (condition == NULL || strlen(condition) == 0)
		sprintf(querybuf, "delete from %s", table);
	else
		sprintf(querybuf, "delete from %s where %s", table, condition);

	int ret = mysql_query(m_pConn, querybuf);
	if (ret) ret = mysql_errno(m_pConn);

	if (ret == CR_SERVER_LOST || ret == CR_SERVER_GONE_ERROR)
	{
		DisconnectFromDb();
		if(ConnectToDb())
		{
			ret = mysql_query(m_pConn, querybuf);
		}
	}
	if (ret) ret = OutputErrorInfo(ret, querybuf);
	

	if (0 == ret && NULL != GetSink() && DbGetAffectedCount() > 0)
		GetSink()->OnDbAffectedOperation(KKDBSQL_UPDATE, GetDbConnParam()->GetDBName().c_str(),
		GetDbConnParam()->GetDBType(),
                                        querybuf,
                                        0, NULL, NULL);
    delete[] (BYTE*)querybuf;
	return ret;
}

DBRecordset * DBMySqlConn::DbSelect(const char * table, const char * columns, const char * condition, const char * clause, int offset, int count, bool bErrLog)
{
    Assert(table || columns);
	if (!IsValid() || NULL == table || NULL == columns) return NULL;

	size_t lCon = condition == NULL ? 0 : strlen(condition);
	
	size_t lCla = clause == NULL ? 0 : strlen(clause);

	char * querybuf = (char*)new BYTE[(256+strlen(table)+strlen(columns)+lCon+lCla)];
    if (querybuf == NULL)
    {
        OutputErrorInfo(1003, "DbSelect new buf null.");
        return NULL;
    }

	std::ostringstream querystr;
	if (lCon == 0)
		sprintf(querybuf, "select %s from %s", columns, table);
	else
		sprintf(querybuf, "select %s from %s where %s", columns, table, condition);

	if (lCla > 0)	//sprintf(querybuf, "%s %s", querybuf, clause);
    {
        strcat(querybuf, " ");
        strcat(querybuf, clause);
    }

	if (count == __MAXCOUNT__ && offset == 0)
		querystr << querybuf;
	else
		querystr << querybuf << " limit " << offset << " , " << count;

	int ret = mysql_query(m_pConn, querystr.str().c_str());
	if (ret) ret = mysql_errno(m_pConn);

	if (ret == CR_SERVER_LOST || ret == CR_SERVER_GONE_ERROR)
	{
		DisconnectFromDb();
		if(ConnectToDb())
		{
			ret = mysql_query(m_pConn, querystr.str().c_str());
		}
	}
	
	if (ret)
	{
		if (bErrLog)
		{
			OutputErrorInfo(ret, querystr.str().c_str());
		}
		delete[] (BYTE*)querybuf;
		return NULL;
	}
	MYSQL_RES * pRes = mysql_store_result(m_pConn);
	if (pRes == NULL)
	{
		OutputErrorInfo(ret, querystr.str().c_str());
		delete[] (BYTE*)querybuf;
		return NULL;
	}

    delete[] (BYTE*)querybuf;

	return new DBMySqlRecordset((HDBRES)pRes, mysql_field_count(m_pConn));
	
}

int DBMySqlConn::DbExec(const char * execbuf, bool isinsert, const char * incfield)
{
    Assert(execbuf);
	if (!IsValid() || NULL == execbuf) return 1;
	int ret = mysql_query(m_pConn, execbuf);
	if (ret) ret = mysql_errno(m_pConn);

	if (ret == CR_SERVER_LOST || ret == CR_SERVER_GONE_ERROR)
	{
		DisconnectFromDb();
		if(ConnectToDb())
		{
			ret = mysql_query(m_pConn, execbuf);
		}
	}
	if (ret) return OutputErrorInfo(ret, execbuf);

	if (0 == ret && NULL != GetSink() && DbGetAffectedCount() > 0)
		GetSink()->OnDbAffectedOperation(KKDBSQL_UPDATE, GetDbConnParam()->GetDBName().c_str(),
		GetDbConnParam()->GetDBType(),
                                            execbuf,
                                            0, NULL, NULL);

	return ret;
}

DBRecordset * DBMySqlConn::DbQuery(const char * querybuf, int offset, int count, bool bErrLog)
{
    Assert(querybuf);
	if (!IsValid() || NULL == querybuf) return NULL;
	
	std::ostringstream querystr;
	if(count == __MAXCOUNT__ && offset == 0)
		querystr << querybuf;
	else
		querystr << querybuf << " limit " << offset << " , " << count;

	int ret = mysql_query(m_pConn, querystr.str().c_str());
	if (ret) ret = mysql_errno(m_pConn);

	if (ret == CR_SERVER_LOST || ret == CR_SERVER_GONE_ERROR)
	{
		DisconnectFromDb();
		if(ConnectToDb())
		{
			ret = mysql_query(m_pConn, querystr.str().c_str());
		}
	}
	if (ret)
	{
		if (bErrLog)
		{
			OutputErrorInfo(ret, querystr.str().c_str());
		}
		return NULL;
	}
	MYSQL_RES * pRes = mysql_store_result(m_pConn);
	if (pRes == NULL)
	{
		OutputErrorInfo(ret, querystr.str().c_str());
		return NULL;
	}

	return new DBMySqlRecordset((HDBRES)pRes, mysql_field_count(m_pConn));
}

bool DBMySqlConn::DbFreeRecordset(DBRecordset * pRes)
{
	if (NULL == pRes)
		return false;
	
	delete pRes;
	return true;
}

char *DBMySqlConn::DbBlob2Str(const char * blob, unsigned int len)
{
	assert(blob);
	assert(len > 0);
	char * blobstr = new char[2 * len + 3];
    if (blobstr == NULL)
    {
		OutputErrorInfo(1006, "DbBlob2Str new buf null.");
        return NULL;
    }
	blobstr[0] = '\'';

	int blen = mysql_escape_string(blobstr + 1, blob, len);
	if(blen < 0)
	{
		delete[] blobstr;
		return NULL;
	}
	blobstr[blen + 1] = '\'';
	blobstr[blen + 2] = 0;
	return blobstr;
}

bool DBMySqlConn::DbFreeBlobStr(char * blobstr)
{
	if(blobstr == NULL)
		return false;
	delete[] blobstr;
	return true;
}

int DBMySqlConn::DbBeginTransact()
{
	if (!IsValid()) return 1;

	int ret = mysql_query(m_pConn, "START TRANSACTION");
    if (ret) ret = mysql_errno(m_pConn);
    if (ret == CR_SERVER_LOST || ret == CR_SERVER_GONE_ERROR)
	{
		DisconnectFromDb();
		if(ConnectToDb())
		{
			ret = mysql_query(m_pConn, "START TRANSACTION");
		}
	}
	if (ret) return OutputErrorInfo(ret, "START TRANSACTION");

	if (0 == ret && NULL != GetSink() && DbGetAffectedCount() > 0)
		GetSink()->OnDbAffectedOperation(KKDBSQL_UPDATE, GetDbConnParam()->GetDBName().c_str(),
		GetDbConnParam()->GetDBType(),
                                            "START TRANSACTION",
                                            0, NULL, NULL);

	return ret;
}

int DBMySqlConn::DbEndTransact()
{
	if (!IsValid()) return 1;

	int ret = mysql_query(m_pConn, "COMMIT");
	if (ret) return OutputErrorInfo(ret, "COMMIT");
	if (0 == ret && NULL != GetSink() && DbGetAffectedCount() > 0)
		GetSink()->OnDbAffectedOperation(KKDBSQL_UPDATE, GetDbConnParam()->GetDBName().c_str(),
		GetDbConnParam()->GetDBType(),
                                            "COMMIT",
                                            0, NULL, NULL);
	return ret;
}

int DBMySqlConn::DbRollBack()
{
	if (!IsValid()) return 1;

	int ret = mysql_query(m_pConn, "ROLLBACK");
	if (ret) return OutputErrorInfo(ret, "ROLLBACK");
	if (0 == ret && NULL != GetSink() && DbGetAffectedCount() > 0)
		GetSink()->OnDbAffectedOperation(KKDBSQL_UPDATE, GetDbConnParam()->GetDBName().c_str(),
		GetDbConnParam()->GetDBType(),
                                            "ROLLBACK",
                                            0, NULL, NULL);
	return ret;
}

unsigned __int64  DBMySqlConn::DbGetAffectedCount()
{
	return m_pConn ? mysql_affected_rows(m_pConn) : -1; 
}

unsigned __int64  DBMySqlConn::DbGetInsertId()
{
	return m_pConn ? mysql_insert_id(m_pConn) : -1; 
}

int DBMySqlConn::OutputErrorInfo(int nErrorCode, const char *msg)
{
    if(!IsValid()) return 1;
    
    int err_no = mysql_errno(m_pConn);
    const char *err_msg = mysql_error(m_pConn);
    
    if (err_no == ER_ERROR_ON_WRITE) return err_no;
    
    if (msg != NULL && strlen(msg) > 0)
    {
//         OutputDebugPrintA("%s, %d", msg, ::GetCurrentThreadId());
//         OutputDebugLogA("%s : %s, %d\n", dtNow.GetKKStringA(), msg, ::GetCurrentThreadId());
    }
    
//     OutputDebugPrintA("err_no = %d, err_msg = %s, %d", err_no, err_msg != NULL ? err_msg : "null", ::GetCurrentThreadId());
//     OutputDebugLogA("%s : err_no = %d, err_msg = %s, %d\n", dtNow.GetKKStringA(), err_no, err_msg != NULL ? err_msg : "null", ::GetCurrentThreadId());
    
    return err_no;
}


void EscapWords(const char *inValue, char* &outValue, size_t nLen){

	size_t i = 0;
	size_t j = 0;
	while (i < nLen) {
		switch (inValue[i])
		{
		case '\'':
		case '\"':
		case '\\':
			outValue[j++] = '\\';
			outValue[j++] = inValue[i++];
			break;
		default:
			outValue[j++] = inValue[i++];
			break;
		}
	}

	outValue[j] = '\0';
}

void  DBAppendExpressionEscapeMysql(std::string & strSrc, const char *szField,
	const char *szOpCode, const char *szConnCode, const char *value, bool bAddComma) {

	if (strSrc.length() > 0) strSrc += szConnCode;

	size_t len = strlen(value);
	if (len > 0) {
		char *bufValue = (char*)_alloca(len * 2 + 1);
		strSrc += szField;
		strSrc += " ";
		strSrc += szOpCode;
		strSrc += bAddComma ? " '" : "";

		EscapWords(value, bufValue, len);

		strSrc += bufValue;
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

void DBAppendValueEscapeMysql(std::string & strSrc, const char *value, bool bAddComma){

	if (strSrc.length() > 0) strSrc += ", ";
	char *Comma = bAddComma ? "'" : "";
	size_t len = strlen(value);
	if (len > 0) {
		char *bufValue = (char*)_alloca(len * 2 + 1);

		EscapWords(value, bufValue, len);

		strSrc += Comma;
		strSrc += bufValue;
		strSrc += Comma;
	}
	else {
		strSrc += Comma;
		strSrc += Comma;
	}
}