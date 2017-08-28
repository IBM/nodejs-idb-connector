/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/* ---------------------------------------------------------------- */
/* IBM Confidential                                                 */
/*                                                                  */
/* OCO Source Materials                                             */
/*                                                                  */
/* Product(s):                                                      */
/*     5733-OPS                                                     */
/*                                                                  */
/* (C)Copyright IBM Corp.  2014, 2016                               */
/*                                                                  */
/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */
/*  --------------------------------------------------------------- */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */
/********************************************************************/
/* HISTORY:                                                         */
/*                                                                  */ 
/*                                                                  */
/* Change Activity:                                                 */
/*                                                                  */
/* CFD List:                                                        */
/*                                                                  */
/* $A0=TTTTTTTT RRRRRRRRRR  YYDDMM  Userid     Comments             */
/* $A1=FW629781 v1r1m0.dgo  141023  mengxumx:  creation             */
/* $B1=FW640460 v1r1m0f.dgo 150707  mengxumx:  v0.12 upgrade        */
/* End CFD List.                                                    */
/*                                                                  */
/*  Additional notes about the Change Activity                      */
/*                                                                  */
/* End Change Activity.                                             */
/********************************************************************/

#pragma once

#include "dbconn.h"
#include <vector>

#define LOG(a) if((a)) { obj->printErrorToLog(); return; }
#define INITASYNC \
  CallBackData* cbd = reinterpret_cast<CallBackData*>(req->data); \
  DbStmt* obj = cbd->obj; \
  memset(obj->msg, 0, sizeof(obj->msg));

using namespace v8;

struct db2_column {
  SQLCHAR*    name;
  SQLSMALLINT  nameLength;
  SQLSMALLINT  sqlType;
  SQLINTEGER  colPrecise;
  SQLSMALLINT  colScale;
  SQLSMALLINT  colNull;
};

class DbStmt : public node::ObjectWrap {
  public:
    static void Init();
    static void NewInstance(const ARGUMENTS& args);
    
  private:
    explicit DbStmt(DbConn* conn);
    ~DbStmt();
    
    void allocColRow() {
      if(colRowAllocated == true) 
        freeColRow(); 
      dbColumn = (db2_column*)calloc(colCount, sizeof(db2_column)); 
      rowData = (SQLCHAR**)calloc(colCount, sizeof(SQLCHAR*)); 
      for(int i = 0; i < colCount; i++) { 
        dbColumn[i].name = (SQLCHAR*)calloc(MAX_COLNAME_WIDTH, sizeof(SQLCHAR)); 
        rowData[i] = (SQLCHAR*)calloc(MAX_COL_WIDTH, sizeof(SQLCHAR)); 
      } 
      colRowAllocated = true; 
    }
    
    void freeColRow() {
      if(colRowAllocated == true) { 
        for(int i = 0; i < colCount && dbColumn[i].name && rowData[i]; i++) { 
          free(dbColumn[i].name); 
          free(rowData[i]); 
        } 
        free(dbColumn); 
        free(rowData); 
        colRowAllocated = false; 
      } 
    }
    
    void freeSp() {
      if(spInCount > 0) { 
        for(int i = 0; i < spInCount; i++) 
          free(spIn[i]); 
        spInCount = 0;
      } 
      if(spOutCount > 0) { 
        for(int i = 0; i < spOutCount; i++) 
          free(spOut[i]); 
        spOutCount = 0;
      } 
      spInNumCount = 0;
      spOutNumCount = 0;
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

    static void nop(uv_work_t* req) {    }
    static void after_work(uv_work_t* req, int status) {    }
    
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
    bool colRowAllocated = false;
    
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
    std::vector<SQLCHAR**> result;
    char msg[SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10];
};