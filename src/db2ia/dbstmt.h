/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets, irrespective of what has been     */
/* deposited with the U.S. Copyright Office.                        */

#pragma once

#include "dbconn.h"
#include <vector>


#define LOG(a) if((a)) { this->printErrorToLog(); return; }

using namespace Napi;

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
  SQLINTEGER  ind;
  void*       buf;
};

struct result_item {
  SQLCHAR*    data;
  SQLINTEGER  rlength;
};

class DbStmt : public Napi::ObjectWrap<DbStmt> {

  // classes that do the async versions of the workflows
  friend class ExecAsyncWorker;
  friend class PrepareAsyncWorker;
  friend class BindParamAsyncWorker;
  friend class ExecuteAsyncWorker;
  friend class FetchAsyncWorker;
  friend class FetchAllAsyncWorker;

  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    DbStmt(const Napi::CallbackInfo& info);

  private:
    
    static Napi::FunctionReference constructor;

    Napi::Value SetStmtAttr(const Napi::CallbackInfo& info);
    Napi::Value GetStmtAttr(const Napi::CallbackInfo& info);
    
    void Exec(const Napi::CallbackInfo& info);
    Napi::Value ExecSync(const Napi::CallbackInfo& info);
    
    void Prepare(const Napi::CallbackInfo& info);
    void PrepareSync(const Napi::CallbackInfo& info);
    
    void BindParam(const Napi::CallbackInfo& info);
    void BindParamSync(const Napi::CallbackInfo& info);
    
    void Execute(const Napi::CallbackInfo& info);
    Napi::Value ExecuteSync(const Napi::CallbackInfo& info);
    
    void NextResult(const Napi::CallbackInfo& info);

    void Fetch(const Napi::CallbackInfo& info);
    Napi::Value FetchSync(const Napi::CallbackInfo& info);
    
    void FetchAll(const Napi::CallbackInfo& info);
    Napi::Value FetchAllSync(const Napi::CallbackInfo& info);
    
    Napi::Value CloseCursor(const Napi::CallbackInfo& info);
    void Reset(const Napi::CallbackInfo& info);
    Napi::Value Commit(const Napi::CallbackInfo& info);
    Napi::Value Rollback(const Napi::CallbackInfo& info);
    Napi::Value Close(const Napi::CallbackInfo& info);
    
    Napi::Value NumFields(const Napi::CallbackInfo& info);
    Napi::Value NumRows(const Napi::CallbackInfo& info);
    Napi::Value FieldType(const Napi::CallbackInfo& info);
    Napi::Value FieldWidth(const Napi::CallbackInfo& info);
    Napi::Value FieldName(const Napi::CallbackInfo& info);
    Napi::Value FieldPrecise(const Napi::CallbackInfo& info);
    Napi::Value FieldScale(const Napi::CallbackInfo& info);
    Napi::Value FieldNullable(const Napi::CallbackInfo& info);

    
    void StmtError(const Napi::CallbackInfo& info);

    // unexposed helper functions
    int populateColumnDescriptions(Napi::Env env);
    void freeColumnDescriptions();
    int bindColData(Napi::Env env);
    int fetchData();
    //int fetchColData(Napi::Env env, Napi::Array array);
    int fetchColData(Napi::Env env, Napi::Array *array);
    void freeColumnData();
    void freeColumns();
    void freeSp();
    void bindParams(Napi::Env env, Napi::Array *params);
    int fetchSp(Napi::Env env, Napi::Array *array);
    int fetch(Napi::Env env, Napi::Object* row);
    void printError(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt);
    void printErrorToLog();
    void throwErrMsg(int handleType, Napi::Env env);
    void throwErrMsg(int code, const char* msg, Napi::Env env);
    std::string returnErrMsg(int handleType);

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