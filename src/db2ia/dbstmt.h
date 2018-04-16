/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */

#pragma once

#include "dbconn.h"
#include <vector>
#include "node_buffer.h"

#define LOG(a) if((a)) { obj->printErrorToLog(); return; }
#define INITASYNC \
  CallBackData* cbd = reinterpret_cast<CallBackData*>(req->data); \
  DbStmt* obj = cbd->obj; \
  memset(obj->msg, 0, sizeof(obj->msg));

using namespace v8;

struct db2_column {
  SQLCHAR*    name;
  SQLSMALLINT nameLength;
  SQLSMALLINT sqlType;
  SQLINTEGER  colPrecise;
  SQLSMALLINT colScale;
  SQLSMALLINT colNull;
  SQLINTEGER  rlength;
};

struct db2_param {
  SQLSMALLINT valueType;
  SQLSMALLINT paramType;
  SQLINTEGER  paramSize;
  SQLSMALLINT decDigits;
  SQLSMALLINT nullable;
  int         io;
  int         ind;
  void*       buf;
};

struct result_item {
  SQLCHAR*    data;
  SQLINTEGER  rlength;
};

class DbStmt : public node::ObjectWrap {
  public:
    static void Init();
    static void NewInstance(const ARGUMENTS& args);
    
  private:
    explicit DbStmt(DbConn* conn);
    ~DbStmt();

    int getColDesc(Isolate* isolate) {
      if(colDescAllocated == true) 
        freeColDesc();
      
      SQLRETURN rc;
      dbColumn = (db2_column*)calloc(colCount, sizeof(db2_column));
      
      if(isDebug)
        printf("SQLDescribeCol(%d).\n", colCount);
      
      for(int i = 0; i < colCount; i++) {
        dbColumn[i].name = (SQLCHAR*)calloc(MAX_COLNAME_WIDTH, sizeof(SQLCHAR));
        rc = SQLDescribeCol(stmth, i + 1, //Column index starts from 1.
              dbColumn[i].name,  //the buffer to store the Column Name
              MAX_COLNAME_WIDTH, //the size of the store buffer 
              &dbColumn[i].nameLength,  //the accurate length of the Column Name
              &dbColumn[i].sqlType,  //the SQL type of the Column
              &dbColumn[i].colPrecise, &dbColumn[i].colScale, &dbColumn[i].colNull);
        if(isDebug)      
          printf("[%d]sqlType[%d]\tcolScale[%d]\tcolPrecise[%d]\n", i, dbColumn[i].sqlType, dbColumn[i].colScale, dbColumn[i].colPrecise);
        if(rc != SQL_SUCCESS) {
          freeColDesc();
          if(isolate != NULL)
            throwErrMsg(SQL_ERROR, "SQLDescribeCol() failed.", isolate);
          else printErrorToLog();
          return -1;
        }
      }
      colDescAllocated = true;
      return 0;
    }
    
    void freeColDesc() {
      if(colDescAllocated == true) { 
        for(int i = 0; i < colCount && dbColumn[i].name; i++)
          free(dbColumn[i].name); 
        free(dbColumn); 
        colDescAllocated = false; 
      }
    }
    
    int bindColData(Isolate* isolate) {
      if(colDataAllocated == true) 
        freeColData();
      
      if(colDescAllocated != true)
        if(getColDesc(isolate) < 0)
          return -1;
      
      SQLRETURN rc;
      SQLINTEGER rlength = 0;
      SQLINTEGER maxColLen = 0;
      int ctype = SQL_C_CHAR;
      rowData = (SQLCHAR**)calloc(colCount, sizeof(SQLCHAR*)); 
      
      if(isDebug)
        printf("SQLBindCol(%d).\n", colCount);
      
      // SQL_C_BIGINT
      // SQL_C_BINARY
      // SQL_C_BLOB
      // SQL_C_BLOB_LOCATOR
      // SQL_C_CHAR
      // SQL_C_CLOB
      // SQL_C_CLOB_LOCATOR
      // SQL_C_DATE
      // SQL_TYPE_DATE
      // SQL_C_DATETIME
      // SQL_C_DBCHAR
      // SQL_C_DBCLOB
      // SQL_C_DBCLOB_LOCATOR
      // SQL_C_DECFLOAT128
      // SQL_C_DECFLOAT64
      // SQL_C_DECFLOAT32
      // SQL_C_DOUBLE
      // SQL_C_FLOAT
      // SQL_C_LONG
      // SQL_C_SLONG
      // SQL_C_REAL
      // SQL_C_SHORT
      // SQL_C_TIME
      // SQL_C_TIMESTAMP
      // SQL_C_STINYINT
      // SQL_C_UTINYINT
      // SQL_TYPE_TIME
      // SQL_TYPE_TIMESTAMP
      // SQL_C_WCHAR

      for(int i = 0; i < colCount; i++) {
        switch(dbColumn[i].sqlType) {
          case SQL_DECIMAL :
          case SQL_NUMERIC :
          {
            maxColLen = dbColumn[i].colPrecise * 256 + dbColumn[i].colScale;
            rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
            rc = SQLBindCol(stmth, i + 1, SQL_C_CHAR, (SQLPOINTER)rowData[i], maxColLen, &dbColumn[i].rlength);
          }
          break;
          case SQL_VARBINARY :
          case SQL_BINARY :
          {
            maxColLen = dbColumn[i].colPrecise;
            rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
            rc = SQLBindCol(stmth, i + 1, SQL_C_BINARY, (SQLPOINTER)rowData[i], maxColLen, &dbColumn[i].rlength);
          }
          break;
          case SQL_WCHAR :
          case SQL_WVARCHAR :
          {
            maxColLen = dbColumn[i].colPrecise << 2;
            rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
            rc = SQLBindCol(stmth, i + 1, SQL_C_CHAR, (SQLPOINTER)rowData[i], maxColLen, &dbColumn[i].rlength);
          }
          break;
          default :
          {
            maxColLen = dbColumn[i].colPrecise + 1;
            rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
            rc = SQLBindCol(stmth, i + 1, SQL_C_CHAR, (SQLPOINTER)rowData[i], maxColLen, &dbColumn[i].rlength);
          }
          break;
        }
        if(rc != SQL_SUCCESS) {
          freeColData();
          if(isolate != NULL)
            throwErrMsg(SQL_ERROR, "SQLBindCol() failed.", isolate);
          else printErrorToLog();
          return -1;
        }
      } 
      colDataAllocated = true;
      return 0;
    }
    
    void freeColData() {
      if(colDataAllocated == true) { 
        for(int i = 0; i < colCount && rowData[i]; i++)
          free(rowData[i]); 
        free(rowData); 
        colDataAllocated = false; 
      }
    }
    
    void freeCol() {
      freeColDesc();
      freeColData();
    }
    
    void freeSp() {
      if(param) {
        for(int i = 0; i < paramCount; i++)
          free(param[i].buf);
        free(param);
        param = NULL;
      }
    }
  
    void printParam() {
      for(int i = 0; i < paramCount; i++) {
        printf("TYPE[%2d] SIZE[%3d] DIGI[%d] IO[%d] IND[%3d] BUF",
            param[i].paramType, param[i].paramSize, param[i].decDigits, param[i].io, param[i].ind);
        if(param[i].valueType = SQL_C_CHAR)  // String
          printf("[%s]\n", (char*)param[i].buf);
        else if(param[i].valueType = SQL_C_BIGINT)  // Integer
          printf("[%d]\n", *(int64_t*)param[i].buf);
        else if(param[i].valueType = SQL_C_DOUBLE)  // Decimal
          printf("[%f]\n", *(double*)param[i].buf);
        else if(param[i].valueType = SQL_C_BIT)  // Decimal
          printf("[%d]\n", *(bool*)param[i].buf);
      }
    }
    
    void printError(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt)
    {
      if(isDebug == true) 
      { 
        SQLCHAR buffer[SQL_MAX_MESSAGE_LENGTH + 1];
        SQLCHAR sqlstate[SQL_SQLSTATE_SIZE + 1];
        SQLINTEGER sqlcode;
        SQLSMALLINT length;
        while( SQLError(henv, hdbc, hstmt, sqlstate, &sqlcode, buffer, SQL_MAX_MESSAGE_LENGTH + 1, &length) == SQL_SUCCESS )
        {
          printf("\n **** ERROR *****\n");
          printf("SQLSTATE: %s\n", sqlstate);
          printf("Native Error Code: %ld\n", sqlcode);
          printf("%s \n", buffer);
        }
      }
    }
    
    void printErrorToLog()
    {
      SQLCHAR buffer[SQL_MAX_MESSAGE_LENGTH + 1];
      SQLCHAR sqlstate[SQL_SQLSTATE_SIZE + 1];
      SQLINTEGER sqlcode;
      SQLSMALLINT length;

      SQLRETURN rc = SQLGetDiagRec(SQL_HANDLE_STMT, stmth, 1, sqlstate, &sqlcode, buffer, SQL_MAX_MESSAGE_LENGTH + 1, &length); 
      if(rc == SQL_SUCCESS) {
        if(buffer[length-1] == '\n') {
          SQLCHAR *p = &buffer[length-1];
          *p = '\0';
        }
        sprintf((char*)msg, "SQLSTATE=%s SQLCODE=%d %s", sqlstate, (int)sqlcode, buffer);
      }
    }
    
    void throwErrMsg(int handleType, Isolate* isolate) 
    {
      SQLCHAR msg[SQL_MAX_MESSAGE_LENGTH + 1];
      SQLCHAR sqlstate[SQL_SQLSTATE_SIZE + 1];
      SQLCHAR errMsg[SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10];
      SQLINTEGER sqlcode = 0;
      SQLSMALLINT length = 0;
      SQLCHAR *p = NULL;

      memset(msg, '\0', SQL_MAX_MESSAGE_LENGTH + 1);
      memset(sqlstate, '\0', SQL_SQLSTATE_SIZE + 1);
      memset(errMsg, '\0', SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10);
      SQLRETURN rc = -1;
      
      if(handleType == SQL_HANDLE_STMT && stmtAllocated == true) {
        rc = SQLGetDiagRec(SQL_HANDLE_STMT, stmth, 1, sqlstate, &sqlcode, msg, SQL_MAX_MESSAGE_LENGTH + 1, &length); 
        printError(envh, connh, stmth);
      }
      else
        return;
      if (rc == SQL_SUCCESS) {
        if (msg[length-1] == '\n') {
          p = &msg[length-1];
          *p = '\0';
        }
        sprintf((char *)errMsg, "SQLSTATE=%s SQLCODE=%d %s", sqlstate, (int)sqlcode, msg);
      }
      isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, errMsg)));
    }
    
    void throwErrMsg(int code, const char* msg, Isolate* isolate)
    {
      SQLCHAR errMsg[SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10];
      sprintf((char *)errMsg, "SQLSTATE=PAERR SQLCODE=%d %s", code, msg);
      isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, errMsg)));
    }

    static void nop(uv_work_t* req) { }
    static void after_work(uv_work_t* req, int status) { }
    
    static void New(const ARGUMENTS& args);
    static Persistent<Function> constructor;
    
    static void SetStmtAttr(const ARGUMENTS& args);
    static void GetStmtAttr(const ARGUMENTS& args);
    
    static void Exec(const ARGUMENTS& args);
    static void ExecAsync(const ARGUMENTS& args);
    static void ExecAsyncRun(uv_work_t *req);
    static void ExecAsyncAfter(uv_work_t *req, int status);
    
    static void CloseCursor(const ARGUMENTS& args);
    
    static void Prepare(const ARGUMENTS& args);
    static void PrepareAsync(const ARGUMENTS& args);
    static void PrepareAsyncRun(uv_work_t *req);
    static void PrepareAsyncAfter(uv_work_t *req, int status);
    
    static void BindParam(const ARGUMENTS& args);
    static void BindParamAsync(const ARGUMENTS& args);
    static void BindParamAsyncRun(uv_work_t *req);
    static void BindParamAsyncAfter(uv_work_t *req, int status);
    
    static void Execute(const ARGUMENTS& args);
    static void ExecuteAsync(const ARGUMENTS& args);
    static void ExecuteAsyncRun(uv_work_t *req);
    static void ExecuteAsyncAfter(uv_work_t *req, int status);
    
    static void NextResult(const ARGUMENTS& args);

    static void Fetch(const ARGUMENTS& args);
    static void FetchAsync(const ARGUMENTS& args);
    static void FetchAsyncRun(uv_work_t *req);
    static void FetchAsyncAfter(uv_work_t *req, int status);
    
    static void FetchAll(const ARGUMENTS& args);
    static void FetchAllAsync(const ARGUMENTS& args);
    static void FetchAllAsyncRun(uv_work_t *req);
    static void FetchAllAsyncAfter(uv_work_t *req, int status);
    
    static void Commit(const ARGUMENTS& args);
    static void Rollback(const ARGUMENTS& args);
    
    static void NumFields(const ARGUMENTS& args);
    static void NumRows(const ARGUMENTS& args);
    
    static void FieldType(const ARGUMENTS& args);
    static void FieldWidth(const ARGUMENTS& args);
    static void FieldName(const ARGUMENTS& args);
    static void FieldPrecise(const ARGUMENTS& args);
    static void FieldScale(const ARGUMENTS& args);
    static void FieldNullable(const ARGUMENTS& args);
    
    static void StmtError(const ARGUMENTS& args);
    
    static void Close(const ARGUMENTS& args);
    
    uv_work_t request;

    bool stmtAllocated = false;
    bool resultSetAvailable = false;
    bool colDescAllocated = false;
    bool colDataAllocated = false;
    
    bool isDebug = false;
    
    static SQLHENV envh;
    SQLHDBC connh;
    SQLHSTMT stmth;
    DbConn* con;
    
    char* xmlOut;
    char* spIn[SP_PARAM_MAX];
    char* spOut[SP_PARAM_MAX];
    int spInNum[SP_PARAM_MAX];
    int spOutNum[SP_PARAM_MAX];
    SQLINTEGER indicator[SP_PARAM_MAX];
    int spInCount = 0;
    int spOutCount = 0;
    int spInNumCount;
    int spOutNumCount;
    
    SQLSMALLINT colCount = 0;
    db2_column* dbColumn;
    SQLCHAR** rowData;
    std::vector<result_item*> result;
    
    db2_param* param = NULL;
    int paramCount = 0;
    
    char msg[SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10];
};