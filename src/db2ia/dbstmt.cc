/* The SousqlReturnCodee code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */
 
#include "dbstmt.h"

using namespace Napi;

Napi::FunctionReference DbStmt::constructor;
SQLHENV DbStmt::envh;

DbStmt::DbStmt(const Napi::CallbackInfo& info) : Napi::ObjectWrap<DbStmt>(info) {
  Napi::Env env = info.Env();
  SQLRETURN sqlReturnCode;

  //TODO: Validate that info[0] is really DbConn Object
  CHECK(!info[0].IsObject(), INVALID_PARAM_TYPE, "Expected Dbconn Object as a parameter", env);

  DbConn* conn = Napi::ObjectWrap<DbConn>::Unwrap(info[0].As<Napi::Object>());

  if (conn->connected) {
    sqlReturnCode = SQLAllocStmt(conn->connh, &stmth);
  } else {
    return;
  }

  if (sqlReturnCode != SQL_SUCCESS) {
    SQLFreeStmt( stmth, SQL_CLOSE );
    // dbStatementObject->throwErrMsg(SQL_HANDLE_DBC, env);
    return;
  }

  isDebug = conn->isDebug;
  envh = conn->envh;
  connh = conn->connh;
  con = conn;
  stmtAllocated = true;  // Any SQL Statement Handler processing can not be allowed before this.
}  

//DbStmt::~DbStmt() {}

Napi::Object DbStmt::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function constructorFunc = DefineClass(env, "dbstmt", {
      InstanceMethod("setStmtAttr", &DbStmt::SetStmtAttr),
      InstanceMethod("getStmtAttr", &DbStmt::GetStmtAttr),

      InstanceMethod("exec", &DbStmt::Exec),
      InstanceMethod("execSync", &DbStmt::ExecSync),

      InstanceMethod("prepare", &DbStmt::Prepare),
      InstanceMethod("prepareSync", &DbStmt::PrepareSync),

      InstanceMethod("bindParam", &DbStmt::BindParam),
      InstanceMethod("bindParamSync", &DbStmt::BindParamSync),

      InstanceMethod("execute", &DbStmt::Execute),
      InstanceMethod("executeSync", &DbStmt::ExecuteSync),

      InstanceMethod("fetch", &DbStmt::Fetch),
      InstanceMethod("fetchSync", &DbStmt::FetchSync),

      InstanceMethod("fetchAll", &DbStmt::FetchAll),
      InstanceMethod("fetchAllSync", &DbStmt::FetchAllSync),

      InstanceMethod("nextResult", &DbStmt::NextResult),
      InstanceMethod("closeCursor", &DbStmt::CloseCursor),
      InstanceMethod("reset", &DbStmt::Reset),

      InstanceMethod("commit", &DbStmt::Commit),
      InstanceMethod("rollback", &DbStmt::Rollback),

      InstanceMethod("numFields", &DbStmt::NumFields),
      InstanceMethod("numRows", &DbStmt::NumRows),

      InstanceMethod("fieldType", &DbStmt::FieldType),
      InstanceMethod("fieldWidth", &DbStmt::FieldWidth),
      InstanceMethod("fieldName", &DbStmt::FieldName),
      InstanceMethod("fieldPrecise", &DbStmt::FieldPrecise),
      InstanceMethod("fieldScale", &DbStmt::FieldScale),
      InstanceMethod("fieldNullable", &DbStmt::FieldNullable),

      InstanceMethod("stmtError", &DbStmt::StmtError),
      InstanceMethod("close", &DbStmt::Close),
  });

  constructor = Napi::Persistent(constructorFunc);
  constructor.SuppressDestruct();

  exports.Set("dbstmt" , constructorFunc);
  return exports;
}

/*
 *  DbStmt::NewInstance
 *    Description: TODO
 *    Parameters: none
 */
/*
Napi::Object DbStmt::NewInstance(Napi::Value arg) {
  Napi::Object dbStatementObject = constructor.New({ arg });
  return dbStatementObject;
}
*/

/*
 *  DbStmt::SetStmtAttr
 *    Description: Set an attribute of a specific statement handle. To set an
 *                 option for all statement handles associated with a
 *                 connection handle, the application can call setConnAttr().
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes two arguments, stored on the info object:
 *          info[0] (Number): Attribute is the statement attribute to set.
 *                            Refer to the attribute table for more details.
 *          info[1] (Number/String): Depending on the Attribute, this can be an
 *                                   integer value, or a character string.
 */
void DbStmt::SetStmtAttr(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  CHECK(info.Length() != 2, INVALID_PARAM_NUM, "Expected Two Parameters for SetStmtAttr", env)
  CHECK(!info[0].IsNumber(), INVALID_PARAM_TYPE, "Number Expected For first Parameter of SetStmtAttr", env)
  CHECK(!(info[1].IsNumber() || info[1].IsString()), INVALID_PARAM_TYPE, "Number || String Expected For Second Parameter of SetStmtAttr", env)
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", env);

  SQLINTEGER attr = Napi::Number(env, info[0]).Int32Value();
  char* cValue;
  SQLINTEGER sLen = 0;
  SQLRETURN sqlReturnCode = -1;

  if(info[1].IsNumber()) {
    int param = Napi::Number(env, info[1]).Int32Value();
    sqlReturnCode = SQLSetStmtAttr(this->stmth, attr, &param, 0);
    DEBUG(this, "SetStmtAttr() attr = %d, value = %d, sqlReturnCode = %d\n", (int)attr, param, (int)sqlReturnCode);
  } else { // info[1].IsString()
    std::string arg1 = Napi::String(env , info[1]).Utf8Value();
    std::vector<char> arg1Vec(arg1.begin(), arg1.end());
    arg1Vec.push_back('\0');
    cValue = &arg1Vec[0];
    sLen = strlen(cValue);
    sqlReturnCode = SQLSetStmtAttr(this->stmth, attr, cValue, sLen);
    DEBUG(this, "SetStmtAttr() attr = %d, value = %s, sqlReturnCode = %d\n", (int)attr, cValue, (int)sqlReturnCode);
  }

  if(sqlReturnCode != SQL_SUCCESS){
    this->throwErrMsg(SQL_HANDLE_STMT, env);
  }

}

/*
 *  DbStmt::GetStmtAttr
 *    Description: Returns the current settings for the specified connection option
 *    Parameters: 
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes two arguments, stored on the info object:
 *          info[0] (Number): Attribute is the connection attribute to set.
 *                            Refer to the attribute table for more details.
 *    Return: The attribute option in the format of a Number or a String.
 * 
 */
Napi::Value DbStmt::GetStmtAttr(const Napi::CallbackInfo& info) {
  
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN((info.Length() != 1), INVALID_PARAM_NUM, "Expected One Parameter for GetStmtAttr", env, env.Null());
  CHECK_WITH_RETURN((!info[0].IsNumber()), INVALID_PARAM_TYPE, "Expected first Parameter of GetStmtAttr to a Number", env, env.Null()); 
  CHECK_WITH_RETURN((this->stmtAllocated == false), STMT_NOT_READY, "The SQL Statment handler is not initialized.", env, env.Null());

  SQLINTEGER attr = Napi::Number(env, info[0]).Int32Value();
  char buf[SQL_MAX_MESSAGE_LENGTH];
  int retVal = 0;
  SQLINTEGER sLen = 0;
  void* pValue = (char*)&buf;
  SQLRETURN sqlReturnCode = SQLGetStmtAttr(this->stmth, attr, pValue, sizeof(buf), &sLen);

  if (sqlReturnCode == SQL_SUCCESS) {
    if(!sLen) {  //If the returned value is a number.
      pValue = &retVal;
      sqlReturnCode = SQLGetStmtAttr(this->stmth, attr, pValue, 0, &sLen);
      DEBUG(this, "GetStmtAttr() attr = %d, value = %d, sqlReturnCode = %d\n", (int)attr, *(int*)pValue, (int)sqlReturnCode)
      return Napi::Number::New(env , *(int*)pValue);
    } else { //If the returned value is a string.
      DEBUG(this, "GetStmtAttr() attr = %d, value = %s, sqlReturnCode = %d\n", (int)attr, (char*)pValue, (int)sqlReturnCode)
      return Napi::String::New(env , buf);
    }
  }

  // sqlReturnCode != SQL_SUCCESS
  this->throwErrMsg(SQL_HANDLE_STMT, env);
  return env.Null();
    
}

/******************************************************************************
************************************ EXEC *************************************
******************************************************************************/

/*
 *  ExecAsyncWorker
 *    Description:
 *      Asynchronous worker class that runs the 'Exec' workflow. Runs
 *      asynchronous code in the Execute() function, and then compiles the
 *      results, calls the callback function, and cleans up memory in OnOk().
 */
class ExecAsyncWorker : public Napi::AsyncWorker {

  public:
    ExecAsyncWorker(DbStmt *dbStatementObject, int originalArgumentLength, SQLCHAR *sqlStatement, Napi::Function& callback) : Napi::AsyncWorker(callback),
      dbStatementObject(dbStatementObject),
      originalArgumentLength(originalArgumentLength),
      sqlStatement(sqlStatement) {}
    ~ExecAsyncWorker() {}

    // Executed inside the worker-thread.
    void Execute () {

      memset(dbStatementObject->msg, 0, sizeof(dbStatementObject->msg));
  
      SQLRETURN sqlReturnCode = SQLExecDirect(dbStatementObject->stmth, sqlStatement, SQL_NTS);
      DEBUG(dbStatementObject, "SQLExecDirect(%d): %s\n", sqlReturnCode, sqlStatement);  
      //LOG(sqlReturnCode != SQL_SUCCESS && sqlReturnCode != SQL_NO_DATA_FOUND)
      
      SQLNumResultCols(dbStatementObject->stmth, &dbStatementObject->colCount);
      
      if (dbStatementObject->colCount == 0) /* statement is not a select statement */
        return;
        
      dbStatementObject->resultSetAvailable = true;
      
      if(dbStatementObject->bindColData(NULL) < 0) return;
      
      // Grabs data from SQL and puts it in DbStmt result array. Converted to Napi values in ExecAsyncAfter (need environment var)
      dbStatementObject->fetchData();
    } 

    // Executed when the async work is complete
    void OnOK() {
      Napi::Env env = Env(); // not sure this is right... old one just gets the Isolate out of thin air
      Napi::HandleScope scope(Env());

      // if 2 arguments were passed, 2nd is a callback and need to return values
      if (originalArgumentLength == 2)
      {
        std::vector<napi_value> callbackArguments;
        Napi::Array results = Napi::Array::New(env); 

        if (dbStatementObject->fetchColData(env, &results) < 0) return;

        if (strlen(dbStatementObject->msg) > 0) {
          callbackArguments.push_back(Napi::String::New(env, dbStatementObject->msg));
        } else {
          callbackArguments.push_back(Env().Null());
        }
        callbackArguments.push_back(results);

        Callback().Call(callbackArguments);
      }
    }

  private:
    DbStmt* dbStatementObject;
    int originalArgumentLength;
    SQLCHAR* sqlStatement;
};

/*
 *  DbStmt::Exec
 *    Description:
 *      Runs the "Exec" workflow asynchronously. Handles passed parameters
 *      passed in data before calling the ExecAsyncWorker, which returns to
 *      the JavaScript environment through a callback.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call. Contains 1
 *        2 parameters, with the 2nd being a callback function that can be
 *        used to check for errors and access data returned by the query.
 */
void DbStmt::Exec(const Napi::CallbackInfo& info) {

  Napi::Env env = info.Env();
  //Napi::HandleScope scope(env); // This wasn't done in original

  std::string arg0 = Napi::String(env , info[0]).Utf8Value();
  std::vector<char> arg0Vec(arg0.begin(), arg0.end());
  arg0Vec.push_back('\0');

  SQLCHAR* tmpSqlSt = &arg0Vec[0];
  Napi::Function callback = info[1].As<Napi::Function>();

  // send off to the worker
  ExecAsyncWorker *worker = new ExecAsyncWorker(this, info.Length(), strdup(tmpSqlSt), callback);
  worker->Queue(); 
}

/*
 *  DbStmt::ExecSync
 *    Description:
 *      Runs the "Exec" workflow synchronously, blocking the Node.js event
 *      loop.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by the N-API from the JavaScript call.
 *        Contains 1 or 2 parameters, with the 2nd being a callback function
 *        that can be used to check for errors and access data returned by the
 *        query.
 */
void DbStmt::ExecSync(const Napi::CallbackInfo& info) {

  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  DEBUG(this, "ExecSync().\n");
  
  // check arguments
  CHECK(info.Length() != 1 && info.Length() != 2, INVALID_PARAM_NUM, "The execSync() method accepts only one or two parameter.", env);
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", env);

  std::string arg0 = Napi::String(env , info[0]).Utf8Value();
  std::vector<char> arg0Vec(arg0.begin(), arg0.end());
  arg0Vec.push_back('\0');

  SQLCHAR* tmpSqlSt = &arg0Vec[0];

  SQLRETURN sqlReturnCode = SQLExecDirect(this->stmth, tmpSqlSt, SQL_NTS);
  DEBUG(this,"SQLExecDirect(%d): %s\n", sqlReturnCode, tmpSqlSt);  
  if(sqlReturnCode != SQL_SUCCESS) {
    if(sqlReturnCode == SQL_NO_DATA_FOUND) {
      //args.GetReturnValue().SetUndefined()
      //return env.Undefined();// info.SetData(env.Undefined()); //SQL statement is a SeasqlReturnCodehed UPDATE/DELETE and no rows satisfy the seasqlReturnCodeh condition
      return;
    }
    else {
      this->throwErrMsg(SQL_HANDLE_STMT, env);
      return;
    }
  }

  SQLNumResultCols(this->stmth, &this->colCount);

  Napi::Array results = Array::New(env);
  
  /* determine statement type */
  if (this->colCount > 0) { /* statement is a select statement */
    this->resultSetAvailable = true;
    if(this->bindColData(env) < 0){
      return;
      }
      this->fetchData();
    if(this->fetchColData(env, &results) < 0) {
      return;
    }
  }

  if (info.Length() >= 2) {
    Napi::Function cb = info[ info.Length() -1 ].As<Napi::Function>();
    std::vector<napi_value> args;
    args.push_back(Napi::Value(Env().Null()));
    args.push_back(results);
    cb.MakeCallback(env.Global(), args);
  }

  this->freeColumns();
}

/******************************************************************************
********************************** PREPARE ************************************
******************************************************************************/

/*
 *  PrepareAsyncWorker
 *    Description:
 *      Asynchronous worker class that runs the 'Prepare' workflow. Runs
 *      asynchronous code in the Execute() function, and then compiles the
 *      results, calls the callback function, and cleans up memory in OnOk().
 */
class PrepareAsyncWorker : public Napi::AsyncWorker {

  public:
    PrepareAsyncWorker(DbStmt *dbStatementObject, SQLCHAR *sqlStatement, Napi::Function& callback) : Napi::AsyncWorker(callback),
      dbStatementObject(dbStatementObject),
      sqlStatement(sqlStatement) {}
    ~PrepareAsyncWorker() {}

    // Executed inside the worker-thread.
    void Execute () {

      SQLRETURN sqlReturnCode = SQLPrepare(dbStatementObject->stmth, sqlStatement, strlen(sqlStatement));
      DEBUG(dbStatementObject, "SQLPrepare(%d): %s\n", sqlReturnCode, sqlStatement);
      //LOG(sqlReturnCode != SQL_SUCCESS);
    } 

    // Executed when the async work is complete
    void OnOK() {
      Napi::Env env = Env();
      Napi::HandleScope scope(env);

      std::vector<napi_value> callbackArguments;

      // check if there were any errors
      // TODO: check why its given back garabage values
      if(strlen(dbStatementObject->msg) > 0) {
        callbackArguments.push_back(Napi::String::New(env, dbStatementObject->msg));
      } else {
        callbackArguments.push_back(env.Null());
      }
      
      Callback().Call(callbackArguments);
    }

  private:
    DbStmt* dbStatementObject;
    SQLCHAR* sqlStatement;
};

/*
 *  DbStmt::Prepare
 *    Description:
 *      The entry point for running the "Prepare" workflow asynchronously.
 *      Handles passed in data before calling the PrepareAsyncWorker,
 *      which returns to the JavaScript environment through a callback.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes one or two arguments, stored on the info object:
 *          info[0]: [String]: The statement that is being prepared before
 *                   running "Bind" and/or "Execute" workflows.
 *          info[1]: [Function] (Optional): The callback function, with
 *                   arguments passed to it in the format function(error):
 *                      error:  A string representation of the error that was
 *                              encountered, or null if no error.
 */
void DbStmt::Prepare(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();

  DEBUG(this, "Prepare().\n");
  CHECK(length != 2, INVALID_PARAM_NUM, "The prepare() method accepts only two parameters.", env)
  CHECK(!info[0].IsString(), INVALID_PARAM_TYPE, "Expected Parameter 1 to be a String", env)
  CHECK(!info[1].IsFunction(), INVALID_PARAM_TYPE, "Expected Parameter 2 to be a Function", env)
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", env)
  
  std::string arg0 = Napi::String(env , info[0]).Utf8Value();
  std::vector<char> arg0Vec(arg0.begin(), arg0.end());
  arg0Vec.push_back('\0');

  SQLCHAR* tmpSqlSt = &arg0Vec[0];
  
  Napi::Function callback = info[1].As<Napi::Function>();
  PrepareAsyncWorker *worker = new PrepareAsyncWorker(this, strdup(tmpSqlSt), callback);
  worker->Queue();
}

/*
 *  DbStmt::PrepareSync
 *    Description:
 *      Runs the "Prepare" workflow synchronously, blocking the Node.js event
 *      loop.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes one or two arguments, stored on the info object:
 *          info[0] (String):   The statement that is being prepared before
 *                              running "Bind" and/or "Execute" workflows.
 *          info[1] (Function): The callback function, with
 *                              arguments passed to it in the format
 *                              function(error).
 */
void DbStmt::PrepareSync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  DEBUG(this, "Prepare().\n");

  CHECK(info.Length() != 2, INVALID_PARAM_NUM, "Expected 2 Parameters for prepare()", env)
  CHECK(!info[0].IsString(), INVALID_PARAM_TYPE, "Expected Parameter 1 to be a String", env)
  CHECK(!info[1].IsFunction(), INVALID_PARAM_TYPE, "Expected Parameter 2 to be a Function", env)
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", env);

  std::string sql = info[0].As<Napi::String>().Utf8Value();

  std::vector<char> sqlVec(sql.begin(), sql.end());
  sqlVec.push_back('\0');
  char* cValue = &sqlVec[0];

  SQLRETURN sqlReturnCode = SQLPrepare(this->stmth, cValue, strlen(cValue));

  DEBUG(this, "SQLPrepare(%d): %s\n", sqlReturnCode, cValue);
  CHECK(sqlReturnCode != SQL_SUCCESS, SQL_ERROR, "SQLPrepare() failed.", env);

  std::vector<napi_value> callbackArguments;

  // check if there were any errors
  if(strlen(this->msg) > 0) {
    callbackArguments.push_back(Napi::String::New(env, this->msg));
  } else {
    callbackArguments.push_back(Env().Null());
  }


  Napi::Function cb = info[1].As<Napi::Function>();
  cb.MakeCallback(env.Global(),{env.Null()} );

}

/******************************************************************************
******************************** BIND PARAM ***********************************
******************************************************************************/

/*
 *  BindParamAsyncWorker
 *    Description:
 *      Asynchronous worker class that runs the 'BindParam' workflow. Runs
 *      asynchronous code in the Execute() function, and then compiles the
 *      results, calls the callback function, and cleans up memory in OnOk().
 */
class BindParamAsyncWorker : public Napi::AsyncWorker {

  public:
    BindParamAsyncWorker(DbStmt *dbStatementObject, Napi::Array& parametersToBind, Napi::Function& callback) : Napi::AsyncWorker(callback),
      dbStatementObject(dbStatementObject),
      parametersToBind(Napi::Persistent(parametersToBind)) {}
    ~BindParamAsyncWorker() {}

    // Executed inside the worker-thread.
    void Execute () {
      // no-op
    }

    // Executed when the async work is complete
    void OnOK() {
      Napi::Env env = Env();
      Napi::HandleScope scope(Env());

      Napi::Array array = this->parametersToBind.Value();
      dbStatementObject->bindParams(env, &array);
      parametersToBind.Reset(); // unsure if needed, might auto refcount to 0

      std::vector<napi_value> callbackArguments;

      // check if there were any errors
      if(strlen(dbStatementObject->msg) > 0) {
        callbackArguments.push_back(Napi::String::New(env, dbStatementObject->msg));
      } else {
        callbackArguments.push_back(Env().Null());
      }

      Callback().Call(callbackArguments);
    }

  private:
    DbStmt* dbStatementObject;
    Napi::Reference<Napi::Array> parametersToBind;
};

/*
 *  DbStmt::BindParam
 *    Description:
 *      The entry point for running the "BindParam" workflow asynchronously.
 *      Run after a statement is prepared with the "Prepare" workflow.
 *      Handles passed in data before calling the BindParamAsyncWorker,
 *      which returns to the JavaScript environment through a callback.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call. Contains
 *        2 parameters, with the 2nd being a callback function that can be
 *        used to check for errors and access data returned by the query.
 */
void DbStmt::BindParam(const Napi::CallbackInfo& info) {

  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  DEBUG(this, "BindParamAsync().\n");
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "Function bindParam() must be called before binding parameters.", env)
  CHECK(info.Length() != 2, INVALID_PARAM_NUM, "The bindParam() method accepts only two parameters.", env)
  CHECK(!info[0].IsArray(), INVALID_PARAM_TYPE, "Expected Parameter 1 to be a String", env);
  CHECK(!info[1].IsFunction(), INVALID_PARAM_TYPE, "Expected Parameter 2 to be a Function", env);

  Napi::Array parametersToBind = info[0].As<Napi::Array>();
  Napi::Function callback = info[1].As<Napi::Function>();
  
  BindParamAsyncWorker *worker = new BindParamAsyncWorker(this, parametersToBind, callback);
  worker->Queue(); 
}

/*
 *  DbStmt::BindParam
 *    Description:
 *      Runs the "BindParam" workflow synchronously, blocking the Node.js event
 *      loop.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes one or two arguments, stored on the info object:
 *          info[0]: [Array]: An array of arrays, the inner arrays containing
 *                   the data to bind to the prepared statement.
 *          info[1]: [Function] (Optional): The callback function, with
 *                   arguments passed to it in the format function(error):
 *                      error:  A string representation of the error that was
 *                              encountered, or null if no error.
 */
void DbStmt::BindParamSync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env); // This wasn't done in original
  int length = info.Length();
  
  DEBUG(this, "BindParam().\n");

  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "Function bindParamSync() must be called before binding parameters.", env)
  CHECK(length != 1 && length != 2, INVALID_PARAM_NUM, "The bindParamSync() method accepts only two parameters.", env)
  CHECK(!info[0].IsArray(), INVALID_PARAM_TYPE, "Function bindParamSync(): Bad parameters.", env)
  CHECK(!info[1].IsFunction(), INVALID_PARAM_TYPE, "Function bindParamSync(): Bad parameters.", env)

  Napi::Array parametersToBind = info[0].As<Napi::Array>();
  
  this->bindParams(env, &parametersToBind);

  std::vector<napi_value> callbackArguments;

  // check if there were any errors
  if(strlen(this->msg) > 0) {
    callbackArguments.push_back(Napi::String::New(env, this->msg));
  } else {
    callbackArguments.push_back(Env().Null());
  }

  Napi::Function callback = info[1].As<Napi::Function>();
  callback.Call(callbackArguments);
}

/******************************************************************************
*********************************** EXECUTE ***********************************
******************************************************************************/

/*
 *  ExecuteAsyncWorker
 *    Description:
 *      Asynchronous worker class that runs the 'Execute' workflow. Runs
 *      asynchronous code in the Execute() function, and then compiles the
 *      results, calls the callback function, and cleans up memory in OnOk().
 */
class ExecuteAsyncWorker : public Napi::AsyncWorker {

  public:
    ExecuteAsyncWorker(DbStmt *dbStatementObject, int originalArgumentsLength, Napi::Function& callback) : Napi::AsyncWorker(callback),
      dbStatementObject(dbStatementObject),
      originalArgumentsLength(originalArgumentsLength) {}
    ~ExecuteAsyncWorker() {}

    // Executed inside the worker-thread.
    void Execute () {

      memset(dbStatementObject->msg, 0, sizeof(dbStatementObject->msg));

      SQLRETURN sqlReturnCode = SQLExecute(dbStatementObject->stmth);

      DEBUG(dbStatementObject, "SQLExecute(%d):\n", sqlReturnCode);
      //LOG(dbStatementObject, sqlReturnCode != SQL_SUCCESS && sqlReturnCode != SQL_SUCCESS_WITH_INFO);

      SQLNumResultCols(dbStatementObject->stmth, &dbStatementObject->colCount);

      /* determine statement type */
      if(dbStatementObject->colCount == 0) /* statement is not a select statement */
        return;
        
      dbStatementObject->resultSetAvailable = true;
      if(dbStatementObject->bindColData(NULL) < 0) return;
    } 

    // Executed when the async work is complete
    void OnOK() {
      Napi::Env env = Env();
      Napi::HandleScope scope(env);

      std::vector<napi_value> callbackArguments;

      // TODO: Check the logic for this function
      if(dbStatementObject->param && dbStatementObject->paramCount > 0 ) {  // executeAsync(function(array){...})

        Napi::Array results = Napi::Array::New(env); 
        dbStatementObject->fetchSp(env, &results);

        if(strlen(dbStatementObject->msg) > 0) {
          callbackArguments.push_back(Napi::String::New(env, dbStatementObject->msg));
        } else {
          callbackArguments.push_back(Env().Null());
        }

        callbackArguments.push_back(results);

      } else {
        if(strlen(dbStatementObject->msg) > 0) {
          callbackArguments.push_back(Napi::String::New(env, dbStatementObject->msg));
        } else {
          callbackArguments.push_back(Env().Null());
        }
      }

      Callback().Call(callbackArguments);

    }

  private:
    DbStmt* dbStatementObject;
    int originalArgumentsLength;
};

/*
 *  DbStmt::ExecuteSync
 *    Description:
 *      Runs the "Execute" workflow asynchronously. Takes a statement prepared
 *      with "Prepare" and possibly bound with "BindParam" and executes it,
 *      returning the results. Handles passed in data before calling the
 *      ExecuteAsyncWorker, which returns to the JavaScript environment through
 *      a callback.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes zero or one arguments, stored on the info object:
 *          info[0]: [Function] (Optional): The callback function, with up to
 *                   two arguments passed to it:
 *                      arg1: A string representation of the error that was
 *                            encountered, or null if no error.
 *                      arg2: Any results of the executed statement, returned
 *                            in JSON format.
 */
void DbStmt::Execute(const Napi::CallbackInfo& info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  DEBUG(this, "Execute().\n");
  CHECK(info.Length() != 1, INVALID_PARAM_NUM, "The execute() method accepts only one parameter.", env)
  CHECK(!info[0].IsFunction(), INVALID_PARAM_TYPE, "Expected the first parameter to be a Function.", env);
  
  Napi::Function callback = info[0].As<Napi::Function>();

  ExecuteAsyncWorker *worker = new ExecuteAsyncWorker(this, info.Length(), callback);
  worker->Queue(); 
}

/*
 *  DbStmt::ExecuteSync
 *    Description:
 *      Runs the "Execute" workflow synchronously, blocking the Node.js event
 *      loop. Takes a statement prepared with "Prepare" and possibly bound
 *      with "BindParam" and executes it, returning the results.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes zero or one arguments, stored on the info object:
 *          info[0]: [Function] (Optional): The callback function, with up to
 *                   two arguments passed to it:
 *                      arg1: A string representation of the error that was
 *                            encountered, or null if no error.
 *                      arg2: Any results of the executed statement, returned
 *                            in JSON format.
 */
void DbStmt::ExecuteSync(const Napi::CallbackInfo& info) {

  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  DEBUG(this, "execute().\n");
  CHECK(info.Length() != 1, INVALID_PARAM_NUM, "The executeSync() method accepts only one parameter.", env)
  CHECK(!info[0].IsFunction(), INVALID_PARAM_TYPE, "Expected the first parameter to be a Function.", env);

  SQLRETURN sqlReturnCode = SQLExecute(this->stmth);
  DEBUG(this, "SQLExecute(%d):\n", sqlReturnCode);
  if(sqlReturnCode != SQL_SUCCESS && sqlReturnCode != SQL_SUCCESS_WITH_INFO) {
    this->throwErrMsg(SQL_HANDLE_STMT, env);
    return;
  }
    
  SQLNumResultCols(this->stmth, &this->colCount);
  /* determine statement type */
  if (this->colCount > 0) {/* statement is a select statement */
    this->resultSetAvailable = true;
    if(this->bindColData(env) < 0) return;
  }
  /* execute(()=>{fetchAll(...)}) or */ 
  /* execute(out=>{console.log(out)}) or */
  /* execute(out=>{console.log(out); fetchAll(...)}) */
  if (info.Length() == 1 && info[0].IsFunction()) {

    Napi::Array array = Napi::Array::New(env);
    Napi::Function cb = info[ info.Length() -1 ].As<Napi::Function>();

    // TODO: What does fetchSp actually do?
    this->fetchSp(env, &array);
    std::vector<napi_value> args;
    //first callback param is error set it to null
    args.push_back(env.Null());
    args.push_back(array);

    // cb.Call(args);
    cb.MakeCallback(env.Global(), args);
  }
}

/******************************************************************************
*********************************** FETCH *************************************
******************************************************************************/

/*
 *  FetchAsyncWorker
 *    Description:
 *      Asynchronous worker class that runs the 'Fetch' workflow. Runs
 *      asynchronous code in the Execute() function, and then compiles the
 *      results, calls the callback function, and cleans up memory in OnOk().
 */
class FetchAsyncWorker : public Napi::AsyncWorker {

  public:
    FetchAsyncWorker(DbStmt *dbStatementObject, int originalArgumentsLength, int orient, int offset, Napi::Function& callback) : Napi::AsyncWorker(callback),
      dbStatementObject(dbStatementObject),
      originalArgumentsLength(originalArgumentsLength),
      orient(orient),
      offset(offset) {}
    ~FetchAsyncWorker() {}

    // Executed inside the worker-thread.
    void Execute () {

      SQLRETURN sqlReturnCode;

      if (originalArgumentsLength > 2) { 
        int retVal = 0;
        sqlReturnCode = SQLGetStmtAttr(dbStatementObject->stmth, SQL_ATTR_CURSOR_SCROLLABLE, &retVal, 0, 0);

        if(retVal == SQL_TRUE) {
          sqlReturnCode = SQLFetchScroll(dbStatementObject->stmth, orient, offset);
          DEBUG(dbStatementObject, "SQLFetchScroll(%d) orient = %d, offset = %d.\n", sqlReturnCode, orient, offset);
        } else {
          DEBUG(dbStatementObject, "Cursor is not scrollable.\n");
        }
      } else {
        sqlReturnCode = SQLFetch(dbStatementObject->stmth);
        DEBUG(dbStatementObject, "SQLFetch(%d).\n", sqlReturnCode);
      }
      
      // TODO
      //LOG(sqlReturnCode != SQL_SUCCESS && sqlReturnCode != SQL_NO_DATA_FOUND)
    } 

    // Executed when the async work is complete
    void OnOK() {

      Napi::Env env = Env();
      Napi::HandleScope scope(Env());

      std::vector<napi_value> callbackArguments;

      Napi::Object row = Napi::Object::New(env);

      dbStatementObject->fetch(env, &row);

      if(strlen(dbStatementObject->msg) > 0){
        callbackArguments.push_back(Napi::String::New(env , dbStatementObject->msg));
      } else {
        callbackArguments.push_back(env.Null());
      }
      //push the row to the callback result
      callbackArguments.push_back(row);

      Callback().Call(callbackArguments);
    }

  private:
    DbStmt* dbStatementObject;
    int originalArgumentsLength;
    int orient;
    int offset;
};

//AM
void DbStmt::Fetch(const Napi::CallbackInfo& info) {

  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  
  DEBUG(this, "fetch().\n");
  CHECK(length != 1 && length != 3 , INVALID_PARAM_NUM, "The fetch() method accept only one or three parameters.", env)
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", env)
  CHECK(this->resultSetAvailable == false, RSSET_NOT_READY, "There is no result set to be queried. Please execute a SQL command first.", env);

  int orient = 0, offset = 0;
  if (length > 2 && info[0].IsNumber() && info[1].IsNumber()) { 
    orient = Napi::Number(env, info[0]).Int32Value();
    offset = Napi::Number(env, info[1]).Int32Value();
  }

  Napi::Function callback = info[length-1].As<Napi::Function>();

  FetchAsyncWorker *worker = new FetchAsyncWorker(this, length, orient, offset, callback);
  worker->Queue();

}

//AM
void DbStmt::FetchSync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  
  DEBUG(this, "fetchSync().\n");
  CHECK(length != 1 && length != 3 , INVALID_PARAM_NUM, "The fetch() method accept only one or three parameters.", env)
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", env)
  CHECK(this->resultSetAvailable == false, RSSET_NOT_READY, "There is no result set to be queried. Please execute a SQL command first.", env);

  SQLRETURN sqlReturnCode;
  if (info.Length() > 1 && info[0].IsNumber() && info[1].IsNumber()) { 
    int orient = Napi::Number(env, info[0]).Int32Value();
    int offset = Napi::Number(env, info[1]).Int32Value();
    int retVal = 0;
    sqlReturnCode = SQLGetStmtAttr(this->stmth, SQL_ATTR_CURSOR_SCROLLABLE, &retVal, 0, 0);
    if(retVal == SQL_TRUE) {
      sqlReturnCode = SQLFetchScroll(this->stmth, orient, offset);
      DEBUG(this, "SQLFetchScroll(%d) orient = %d, offset = %d.\n", sqlReturnCode, orient, offset);
    }
    else{
      this->throwErrMsg(SQL_ERROR, "Cursor is not scrollable.", env);
    }
      
  }
  else {
    sqlReturnCode = SQLFetch(this->stmth);
    DEBUG(this, "SQLFetch(%d).\n", sqlReturnCode);
  }
  if(sqlReturnCode == SQL_SUCCESS)
  {
    Napi::Object row = Napi::Object::New(env);
    
    this->fetch(env, &row);
    
    if(length == 1 || length == 3) {  // Run call back to handle the fetched row.
        Napi::Function cb = info[length-1].As<Napi::Function>();
        std::vector<napi_value> args;
        args.push_back(row);
        cb.MakeCallback(env.Global(), args);
    }
    else{
      //TODO: need to return the row
      //RETURN(row)
    }
  }
  if(sqlReturnCode == SQL_ERROR){
    this->throwErrMsg(SQL_HANDLE_STMT, env);
  }
  //TODO: Figure out what is meant by this return
  //RETURN(Number::New(isolate, sqlReturnCode))  // SQL_NO_DATA_FOUND indicate the end of the result set. This is not an error.

}

/******************************************************************************
********************************** FETCH ALL **********************************
******************************************************************************/

/*
 *  FetchAllAsyncWorker
 *    Description:
 *      Asynchronous worker class that runs the 'FetchAll' workflow. Runs
 *      asynchronous code in the Execute() function, and then compiles the
 *      results, calls the callback function, and cleans up memory in OnOk().
 */
class FetchAllAsyncWorker : public Napi::AsyncWorker {

  public:
    FetchAllAsyncWorker(DbStmt *dbStatementObject, Napi::Function& callback) : Napi::AsyncWorker(callback),
      dbStatementObject(dbStatementObject) {}
    ~FetchAllAsyncWorker() {}

    // Executed inside the worker-thread.
    void Execute () {
      dbStatementObject->fetchData();
    } 

    // Executed when the async work is complete
    void OnOK() {
      Napi::Env env = Env(); // not sure this is right... old one just gets the Isolate out of thin air
      Napi::HandleScope scope(Env());

      std::vector<napi_value> callbackArguments;
      Napi::Array results = Napi::Array::New(env); 

      if (dbStatementObject->fetchColData(env, &results) < 0) {
        return;
      } 
      callbackArguments.push_back(results);
      Callback().Call(callbackArguments);
    }

  private:
    DbStmt* dbStatementObject;
};

//AM
void DbStmt::FetchAll(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  DEBUG(this, "FetchAllAsync().\n");
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", env)
  CHECK(this->resultSetAvailable == false, RSSET_NOT_READY, "There is no result set to be queried. Please execute a SQL command first.", env);
  
  Napi::Function callback = info[0].As<Napi::Function>();
  FetchAllAsyncWorker *worker = new FetchAllAsyncWorker(this, callback);
  worker->Queue();
}

//AM
void DbStmt::FetchAllSync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  DEBUG(this, "FetchAll().\n");

  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", env)
  CHECK(this->resultSetAvailable == false, RSSET_NOT_READY, "There is no result set to be queried. Please execute a SQL command first.", env);

  Napi::Array results = Array::New(env);
  //Error Occured fetch Col Data returns -1
  this->fetchData();
  if(this->fetchColData(env, &results) < 0) {
    return;
  } 
  
  if (info.Length() == 1) {
    Napi::Function cb = info[ info.Length() -1 ].As<Napi::Function>();
    std::vector<napi_value> args;
    args.push_back(results);
    cb.MakeCallback(env.Global(), args);
  }


}

/******************************************************************************
 **************************** END MAIN FUNCTIONS ******************************
 *****************************************************************************/

/*
 *  DbStmt::NextResult
 *    Description: Determines whether there is more information available on
 *                 the statement handle that has been associated with a stored
 *                 procedure that is returning result sets.
 *    Parameters: Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        commit() function takes no arguments.
 *    Return: void // TODO: No longer true
 */
void DbStmt::NextResult(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  SQLRETURN sqlReturnCode = SQLMoreResults(this->stmth);
  CHECK(sqlReturnCode != SQL_SUCCESS, SQL_ERROR, "SQLMoreResults() failed.", env)
  CHECK(sqlReturnCode == SQL_NO_DATA_FOUND, SQL_ERROR, "No more result set available.", env)
  
  sqlReturnCode = SQLNumResultCols(this->stmth, &this->colCount);
  CHECK(sqlReturnCode != SQL_SUCCESS, SQL_ERROR, "SQLNumResultCols() failed.", env)

  if (this->colCount == 0) { // not a SELECT statement
    //need to find way to return as output param in NAPI
    // args.GetReturnValue().SetUndefined();  /* User can issue numRows() to the get affected rows number. */
    info.Env().Null();
  } else { // SELECT statement
    this->resultSetAvailable = true;
    if(this->bindColData(env) < 0) {
      return;
    }
  }
}

/*
 *  DbStmt::Commit
 *    Description: Commit all changes to the database that have been made on
 *                 the connection since connect time or the previous call to
 *                 commit().
 *    Parameters: Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        commit() function takes no arguments.
 *    Return: void
 */
void DbStmt::Commit(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", env)
  SQLRETURN sqlReturnCode = SQLTransact(envh, this->connh, SQL_COMMIT);
  CHECK(sqlReturnCode != SQL_SUCCESS, SQL_ERROR, "SQLTransact(Commit) failed.", env)
}

/*
 *  DbStmt::Rollback
 *    Description: Rollback all changes to the database that have been made on
 *                 the connection since connect time or the previous call to
 *                 commit().
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        rollback() function takes no arguments.
 *    Return: void
 */
void DbStmt::Rollback(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", env)
  SQLRETURN sqlReturnCode = SQLTransact(envh, this->connh, SQL_ROLLBACK);
  CHECK(sqlReturnCode != SQL_SUCCESS, SQL_ERROR, "SQLTransact(Rollback) failed.", env)
}

/*
 *  DbStmt::CloseCursor
 *    Description: Calling closeCursor() closes any cursor associated with the
 *                 dbstmt object and discards any pending results. If no open
 *                 cursor is associated with the dbstmt object, the function
 *                 has no effect. If the dbstmt object references a stored
 *                 procedure that has multiple result sets, the closeCursor()
 *                 closes only the current result set. Any additional result
 *                 sets remain open and usable. If you want to reuse the dbstmt
 *                 object, please call closeCursor() before calling exec() or
 *                 prepare() for another SQL statement. Otherwise, you just
 *                 need to call delete to completely remove it.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        closeCursor() function takes no arguments.
 *    Return: void
 */
void DbStmt::CloseCursor(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  DEBUG(this, "SQLCloseCursor: stmth %d\n", this->stmth)
  SQLCloseCursor(this->stmth);
}

/*
 *  DbStmt::Reset
 *    Description: TODO
 *    Parameters: none
 */
void DbStmt::Reset(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  DEBUG(this, "SQLFreeStmt: stmth %d [SQL_RESET_PARAMS]\n", this->stmth)
  if(this->stmtAllocated){
    SQLFreeStmt(this->stmth, SQL_RESET_PARAMS); //Release all params set by SQLBindParam()
  }
}

/*
 *  DbStmt::Close
 *    Description: DB2 for i resources associated with the statement object are
 *                 freed. The open cursor, if any, is closed and all pending
 *                 results are discarded.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        close() function takes no arguments.
 *    Return: void
 */
void DbStmt::Close(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  this->freeColumns();
  DEBUG(this, "SQLFreeStmt: stmth %d [SQL_DROP]\n", this->stmth)
  if(this->stmtAllocated) {
    SQLFreeStmt(this->stmth, SQL_DROP);  //Free the statement handle here. No further processing allowed.
    this->stmtAllocated = false;  // Any SQL Statement Handler processing can not be allowed after this.
    this->resultSetAvailable = false;
  }
}

/*
 *  DbStmt::NumFields
 *    Description: Returns the number of fields contained in a result set.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        numFields() function takes no arguments.
 *    Return: Number value indicating number of fields in the result set.
 */
Napi::Value DbStmt::NumFields(const Napi::CallbackInfo& info) {

  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN((this-> stmtAllocated == false),STMT_NOT_READY,  "The SQL Statement handler is not initialized.", env, env.Null());
  CHECK_WITH_RETURN((this->resultSetAvailable == false), RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", env, env.Null());

  SQLRETURN sqlReturnCode = SQLNumResultCols(this->stmth, &colCount);
  
  if(sqlReturnCode != SQL_SUCCESS) {
    this->throwErrMsg(SQL_ERROR, "SQLNumResultCols() failed.", env);
    return env.Null();
  }

  return Napi::Number::New(env, colCount); 
}

/*
 *  DbStmt::NumRows
 *    Description: Returns the number of rows in a table affected by an UPDATE,
 *                 INSERT, MERGE, SELECT from INSERT, or DELETE statement
 *                 processed against the table.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        numRows() function takes no arguments.
 *    Return: Number value indicating number of rows affected by the operation.
 */
Napi::Value DbStmt::NumRows(const Napi::CallbackInfo& info) {

  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  CHECK_WITH_RETURN((this-> stmtAllocated == false),STMT_NOT_READY,  "The SQL Statement handler is not initialized.", env, env.Null());

  SQLINTEGER rowCount;
  if(SQLRowCount (this->stmth, &rowCount) != SQL_SUCCESS) {
    this->throwErrMsg(SQL_ERROR, "SQLRowCount() failed.", env);
    return env.Null();
  }
  return Napi::Number::New(env , rowCount);
}

/*
 *  DbStmt::FieldType
 *    Description: Returns the data type of the indicated column in a result
 *                 set.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        fieldType() function takes one argument, stored on the info object.
 *          info[0]: Number : the column number in a result set, ordered
 *                            sequentially left to right, starting at 0.
 *    Return: Number value indicating the data type of the specified column in
 *            the result set.
 */
Napi::Value DbStmt::FieldType(const Napi::CallbackInfo& info) {

  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  
  CHECK_WITH_RETURN((info.Length() != 1), INVALID_PARAM_NUM, "The fieldType() method only accepts one parameter.", env, env.Null());
  CHECK_WITH_RETURN((!info[0].IsNumber()), INVALID_PARAM_TYPE, "Expected the first parameter to be a Number.", env, env.Null()); 
  CHECK_WITH_RETURN((this->resultSetAvailable == false || this->colDescAllocated == false), RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", env, env.Null());

  SQLSMALLINT i = Napi::Number(env,info[0]).Int32Value();
  CHECK_WITH_RETURN((i >= this->colCount || i < 0), INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", env, env.Null());
  
  return Napi::Number::New(env , this->dbColumn[i].sqlType);
}

/*
 *  DbStmt::FieldWidth
 *    Description: Returns the width of the indicated column in a result set.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        fieldWidth() function takes one argument, stored on the info object.
 *          info[0]: Number : the column number in a result set, ordered
 *                            sequentially left to right, starting at 0.
 *    Return: Number value indicating the width of the specified column in the
 *            result set.
 */
Napi::Value DbStmt::FieldWidth(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN((info.Length() != 1), INVALID_PARAM_NUM, "The fieldWidth() method only accepts one parameter.", env, env.Null());
  CHECK_WITH_RETURN((!info[0].IsNumber()), INVALID_PARAM_TYPE, "Expected the first parameter to be a Number.", env, env.Null()); 
  CHECK_WITH_RETURN((this->resultSetAvailable == false || this->colDescAllocated == false), RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", env, env.Null());

  SQLSMALLINT i = Napi::Number(env,info[0]).Int32Value();
  CHECK_WITH_RETURN((i >= this->colCount || i < 0), INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", env, env.Null());

  return Napi::Number::New(env , this->dbColumn[i].nameLength);
}

/*
 *  DbStmt::FieldName
 *    Description: Returns the name of the indicated column in a result set.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        fieldName() function takes one argument, stored on the info object.
 *          info[0]: Number : the column number in a result set, ordered
 *                            sequentially left to right, starting at 0.
 *    Return: String representing the name of the specified column in the
 *            result set.
 */
Napi::Value DbStmt::FieldName(const Napi::CallbackInfo& info) {

  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN((info.Length() != 1), INVALID_PARAM_NUM, "The fieldName() method only accepts one parameter.", env, env.Null());
  CHECK_WITH_RETURN((!info[0].IsNumber()), INVALID_PARAM_TYPE, "Expected the first parameter to be a Number.", env, env.Null()); 
  CHECK_WITH_RETURN((this->resultSetAvailable == false || this->colDescAllocated == false), RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", env, env.Null());

  SQLSMALLINT i = Napi::Number(env,info[0]).Int32Value();
  CHECK_WITH_RETURN((i >= this->colCount || i < 0), INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", env, env.Null());

  return Napi::String::New(env, this->dbColumn[i].name );
}

/*
 *  DbStmt::FieldPrecise
 *    Description: Returns the precision of the indicated column in a result
 *                 set.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        fieldPrecise() function takes one argument, stored on the info
 *        object.
 *          info[0]: Number : the column number in a result set, ordered
 *                            sequentially left to right, starting at 0.
 *    Return: Number value indicating the precision of the specified column in
 *            the result set.
 */
Napi::Value DbStmt::FieldPrecise(const Napi::CallbackInfo& info) {

  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN((info.Length() != 1), INVALID_PARAM_NUM, "The fieldPrecise() method only accepts one parameter.", env, env.Null());
  CHECK_WITH_RETURN((!info[0].IsNumber()), INVALID_PARAM_TYPE, "Expected the first parameter to be a Number.", env, env.Null()); 
  CHECK_WITH_RETURN((this->resultSetAvailable == false || this->colDescAllocated == false), RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", env, env.Null());

  SQLSMALLINT i = Napi::Number(env,info[0]).Int32Value();
  CHECK_WITH_RETURN((i >= this->colCount || i < 0), INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", env, env.Null());

  return Napi::Number::New(env ,this->dbColumn[i].colPrecise);
}

/*
 *  DbStmt::FieldScale
 *    Description: Returns the scale of the indicated column in a result set.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        fieldScale() function takes one argument, stored on the info object.
 *          info[0]: Number : the column number in a result set, ordered
 *                            sequentially left to right, starting at 0.
 *    Return: Number indicating the scale of the specified
 *            column in the result set.
 */
Napi::Value DbStmt::FieldScale(const Napi::CallbackInfo& info) {

  Napi::Env env = info.Env(); 
  HandleScope scope(env);

  CHECK_WITH_RETURN((info.Length() != 1), INVALID_PARAM_NUM, "The fieldScale() method only accepts one parameter.", env, env.Null());
  CHECK_WITH_RETURN((!info[0].IsNumber()), INVALID_PARAM_TYPE, "Expected the first parameter to be a Number.", env, env.Null()); 
  CHECK_WITH_RETURN((this->resultSetAvailable == false || this->colDescAllocated == false), RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", env, env.Null());
  SQLSMALLINT i = info[0].ToNumber().Int32Value();
  CHECK_WITH_RETURN((i >= this->colCount || i < 0), INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", env, env.Null());
  
  return Napi::Number::New(env, this->dbColumn[i].colScale);
}

/*
 *  DbStmt::FieldNullable
 *    Description:
 *      Returns if the indicated column in a result set can be NULL.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        fieldNullable() function takes one argument, stored on the info
 *        object.
 *          info[0]: Number : the column number in a result set, ordered
 *                            sequentially left to right, starting at 0.
 *    Return: Boolean indicating if the passed column can be NULL.
 */
Napi::Value DbStmt::FieldNullable(const Napi::CallbackInfo& info) {

  Napi::Env env = info.Env(); 
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN((info.Length() != 1), INVALID_PARAM_NUM,"The fieldNullable() method only accepts one parameter.", env, env.Null());
  CHECK_WITH_RETURN((!info[0].IsNumber()), INVALID_PARAM_TYPE, "Expected the first parameter to be a Number.", env, env.Null()); 
  CHECK_WITH_RETURN((this->resultSetAvailable == false || this->colDescAllocated == false), RSSET_NOT_READY, "The Result set is unavailable. Try querying something first.", env, env.Null());
  
  SQLSMALLINT i = info[0].ToNumber().Int32Value();
  CHECK_WITH_RETURN((i >= this->colCount || i < 0), INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", env, env.Null());
  
  return Napi::Boolean::New(env, this->dbColumn[i].colNull == SQL_NULLABLE);
}

/*
 *  DbStmt::StmtError
 *    Description:
 *      Returns the diagnostic information associated with the most recently
 *      called function for a particular statement, connection, or environment
 *      handler.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the stmtError
 *        function takes three arguments, stored on the info object:
 *          info[0]: Number : hType, indicates the handler type of diagnostic
 *                            information. Accepted values include:
 *                              SQL_HANDLE_ENV: environment info
 *                              SQL_HANDLE_DBC: connection info
 *                              SQL_HANDLE_STMT: statement info
 *          info[1]: Number : recNo, indicates which error should be retrieved.
 *                            The first errror record is number 1.
 *          info[2]: Function : The callback function, with one argument
 *                              passed to it:
 *                                arg1: A string representation of the error
 *                                      that was returned from the call to
 *                                       SQLGetDiagRec
 */
void DbStmt::StmtError(const Napi::CallbackInfo& info) {

  Napi::Env env = info.Env(); 
  HandleScope scope(env);

  int argumentLength = info.Length();

  // check arguments
  CHECK((this->stmtAllocated == false), STMT_NOT_READY, "The SQL Statement handler is not initialized.", env);
  CHECK((argumentLength != 3), INVALID_PARAM_NUM, "The stmtError() method accepts three parameters.", env);
  CHECK(!info[0].IsNumber(), INVALID_PARAM_TYPE, "Expected a number as the first parameter", env);
  CHECK(!info[1].IsNumber(), INVALID_PARAM_TYPE, "Expected a number as the second parameter", env);
  CHECK(!info[2].IsFunction(), INVALID_PARAM_TYPE, "Expected a function as the third parameter", env);

  SQLSMALLINT hType = info[0].ToNumber().Int32Value();
  SQLSMALLINT recno = info[1].ToNumber().Int32Value();
  Napi::Function callback = info[2].As<Napi::Function>();
  std::vector<napi_value> callbackArguments;

  /*
   * Prepare data for use with SQLGetDiagRec, which returns the current values
   * of multiple fields of a diagnostic record that contains error, warning,
   * and status information. Unlike SQLGetDiagField(), which returns one
   * diagnostic field per call, SQLGetDiagRec() returns several commonly used
   * fields of a diagnostic record: the SQLSTATE, native error code, and error
   * message text.
   */

  SQLINTEGER handle;
  
  switch(hType)
  {
    case SQL_HANDLE_ENV: // Retrieve the environment diagnostic information
      handle = envh;
      break;
    case SQL_HANDLE_DBC: // Retrieve the connection diagnostic information
      handle = this->connh;
      break;
    case SQL_HANDLE_STMT: // Retrieve the statement diagnostic information
      handle = this->stmth;
      break;
    default: // hType value not recognized
      return;
  }

  SQLCHAR msg[SQL_MAX_MESSAGE_LENGTH + 1];
  SQLCHAR sqlstate[SQL_SQLSTATE_SIZE + 1];
  char errorMessage[SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10];

  SQLINTEGER sqlcode = 0;
  SQLSMALLINT length = 0;
  SQLCHAR *p = NULL;

  // MI: Do we really need to set every value in each buffer to '\0'?
  // memset(msg, '\0', SQL_MAX_MESSAGE_LENGTH + 1);
  // memset(sqlstate, '\0', SQL_SQLSTATE_SIZE + 1);
  // memset(errorMessage, '\0', SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10);

  SQLRETURN sqlReturnCode = SQLGetDiagRec(hType, handle, recno, sqlstate, &sqlcode, msg, SQL_MAX_MESSAGE_LENGTH + 1, &length);  
  
  // handle all possible return codes
  switch(sqlReturnCode) {
    case SQL_SUCCESS:
      if (msg[length-1] == '\n') {
        p = &msg[length-1];
        *p = '\0';
      }
      break;
    case SQL_SUCCESS_WITH_INFO: // TODO
    case SQL_ERROR:             // TODO
    case SQL_INVALID_HANDLE:    // TODO
      break;
  }

  sprintf(errorMessage, "SQLSTATE=%s SQLCODE=%d : %s", sqlstate, (int)sqlcode, msg);
  callbackArguments.push_back(Napi::String::New(env, errorMessage));
  callback.Call(callbackArguments);
}

/****** UNEXPOSED HELPER FUNCTIONS ******/

int DbStmt::populateColumnDescriptions(Napi::Env env) {
    if(colDescAllocated == true) {
      freeColumnDescriptions();
    }
    
    SQLRETURN sqlReturnCode;
    dbColumn = (db2_column*)calloc(colCount, sizeof(db2_column));
    
    if(isDebug)
      printf("SQLDescribeCol(%d).\n", colCount);
    
    for(int i = 0; i < colCount; i++) {
      dbColumn[i].name = (SQLCHAR*)calloc(MAX_COLNAME_WIDTH, sizeof(SQLCHAR));
      sqlReturnCode = SQLDescribeCol(
        stmth, i + 1, //Column index starts from 1.
        dbColumn[i].name,  //the buffer to store the Column Name
        MAX_COLNAME_WIDTH, //the size of the store buffer 
        &dbColumn[i].nameLength,  //the accurate length of the Column Name
        &dbColumn[i].sqlType,  //the SQL type of the Column
        &dbColumn[i].colPrecise, 
        &dbColumn[i].colScale,
        &dbColumn[i].colNull);
      if(isDebug)      
        printf("[%d]sqlType[%d]\tcolScale[%d]\tcolPrecise[%d]\n", i, dbColumn[i].sqlType, dbColumn[i].colScale, dbColumn[i].colPrecise);
      if (sqlReturnCode != SQL_SUCCESS) {
        freeColumnDescriptions(); // this never gets run. colDescAllocated isnt set to true until after this call
        if(env != NULL)
          throwErrMsg(SQL_ERROR, "SQLDescribeCol() failed.", env);
        else printErrorToLog();
        return -1;
      }
    }
    colDescAllocated = true;
    return 0;
  }
  
  void DbStmt::freeColumnDescriptions() {
    if(colDescAllocated == true) { 
      for(int i = 0; i < colCount && dbColumn[i].name; i++)
        free(dbColumn[i].name); 
      free(dbColumn); 
      colDescAllocated = false; 
    }
  }
  
  int DbStmt::bindColData(Napi::Env env) {
    if(colDataAllocated == true) 
      freeColumnData();
    
    if(colDescAllocated != true)
      if(populateColumnDescriptions(env) < 0)
        return -1;
    
    SQLRETURN sqlReturnCode;
    SQLINTEGER maxColLen = 0;
    rowData = (SQLCHAR**)calloc(colCount, sizeof(SQLCHAR*)); 
    
    if (isDebug) {
      printf("SQLBindCol(%d).\n", colCount);
    }

    for(int i = 0; i < colCount; i++) {
      switch(dbColumn[i].sqlType) {
        case SQL_DECIMAL :
        case SQL_NUMERIC :
        {
          maxColLen = dbColumn[i].colPrecise * 256 + dbColumn[i].colScale;
          rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
          sqlReturnCode = SQLBindCol(stmth, i + 1, SQL_C_CHAR, (SQLPOINTER)rowData[i], maxColLen, &dbColumn[i].rlength);
        }
        break;
        case SQL_VARBINARY :
        case SQL_BINARY :
        {
          maxColLen = dbColumn[i].colPrecise;
          rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
          sqlReturnCode = SQLBindCol(stmth, i + 1, SQL_C_BINARY, (SQLPOINTER)rowData[i], maxColLen, &dbColumn[i].rlength);
        }
        break;
        case SQL_BLOB :
        {
          // maxColLen = dbColumn[i].colPrecise;
          // rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
          // int* length = (int*)malloc(sizeof(int));
          // struct blob_data {
            // int    length;
            // SQLCHAR* data;
          // };
          // blob_data* blob = (blob_data*)malloc(sizeof(blob_data));;
          // sqlReturnCode = SQLBindCol(stmth, i + 1, SQL_C_BLOB, blob, sizeof(blob_data), &blob->length);
        }
        break;
        case SQL_WCHAR :
        case SQL_WVARCHAR :
        {
          maxColLen = dbColumn[i].colPrecise << 2;
          rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
          sqlReturnCode = SQLBindCol(stmth, i + 1, SQL_C_CHAR, (SQLPOINTER)rowData[i], maxColLen, &dbColumn[i].rlength);
        }
        break;
        default :
        {
          maxColLen = dbColumn[i].colPrecise + 1;
          rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
          sqlReturnCode = SQLBindCol(stmth, i + 1, SQL_C_CHAR, (SQLPOINTER)rowData[i], maxColLen, &dbColumn[i].rlength);
        }
        break;
      }
      if (sqlReturnCode != SQL_SUCCESS) {
        freeColumnData();
        if(env != NULL)
          throwErrMsg(SQL_ERROR, "SQLBindCol() failed.", env);
        else printErrorToLog();
        return -1;
      }
    } 
    colDataAllocated = true;
    return 0;
  }
  
  void DbStmt::fetchData() {

    SQLRETURN sqlReturnCode;

    while(( sqlReturnCode = SQLFetch(stmth)) == SQL_SUCCESS) 
    {
      result_item* row = (result_item*)calloc(colCount, sizeof(result_item)); 
      for(int i = 0; i < colCount; i++)
      {
        int colLen = 0;
        if(dbColumn[i].rlength == SQL_NTS) {
          colLen = strlen(rowData[i]);
          row[i].data = (SQLCHAR*)calloc(colLen + 1, sizeof(SQLCHAR));
          memcpy(row[i].data, rowData[i], colLen * sizeof(SQLCHAR));
          row[i].rlength = SQL_NTS;
        }
        else if(dbColumn[i].rlength == SQL_NULL_DATA) {
          row[i].data = NULL;
          row[i].rlength = SQL_NULL_DATA;
        }
        else {
          colLen = dbColumn[i].rlength;
          row[i].data = (SQLCHAR*)calloc(colLen, sizeof(SQLCHAR));
          memcpy(row[i].data, rowData[i], colLen * sizeof(SQLCHAR));
          row[i].rlength = colLen;
        }
      }
      result.push_back(row);
    }

    //TODO: Not sure this is right
    LOG(sqlReturnCode != SQL_NO_DATA_FOUND)
  }

  int DbStmt::fetchColData(Napi::Env env, Napi::Array *array) {  
    for(std::size_t i = 0; i < result.size(); i++)
    {
      Napi::Object row = Object::New(env);
      for(int j = 0; j < colCount; j++)
      {
        Napi::Value value;
        if(result[i][j].rlength == SQL_NULL_DATA)
          value = env.Null();
        else {
          switch(dbColumn[j].sqlType) {
            case SQL_VARBINARY :
            case SQL_BINARY : // TODO
            //   printf("\n\ntrying to fetch binary data\n\n");
              //value = Local<Value>::New(isolate, node::Buffer::New(isolate, result[i][j].data, result[i][j].rlength).ToLocalChecked());
              // break;
            case SQL_BLOB : //TODO
              // value = Local<Value>::New(isolate, node::Buffer::New(isolate, rowData[i] + sizeof(int), *(int*)rowData[i]).ToLocalChecked());
              break;
            default : 
              value = Napi::String::New(env, result[i][j].data);
              break;
          }
        }
        row.Set(Napi::String::New(env, (char const*)dbColumn[j].name), value);
        if(result[i][j].data)
          free(result[i][j].data);
      }
      array->Set(i, row);  //Build the JSON data
      free(result[i]);
    }
    result.clear();
    return 0;
  }
  
  void DbStmt::freeColumns() {
    freeColumnDescriptions();
    freeColumnData();
  }

  void DbStmt::freeColumnData() {
    if(colDataAllocated == true) { 
      for(int i = 0; i < colCount && rowData[i]; i++)
        free(rowData[i]); 
      free(rowData); 
      colDataAllocated = false; 
    }
  }
  
  void DbStmt::freeSp() {
    if(param) {
      for(int i = 0; i < paramCount; i++)
        free(param[i].buf);
      free(param);
      param = NULL;
    }
  }

  void DbStmt::bindParams(Napi::Env env, Napi::Array *params) {

    Napi::Array object;
    int bindIndicator;
    SQLRETURN sqlReturnCode;
    freeSp();
    
    paramCount = params->Length();
    param = (db2_param*)calloc(paramCount, sizeof(db2_param));

    for(SQLSMALLINT i = 0; i < paramCount; i++) {

      object = params->Get(i).As<Napi::Array>();  //Get a  ? parameter from the array.
      param[i].io = object.Get(1).ToNumber().Int32Value();  //Get the parameter In/Out type. // MI these were all -> before
      bindIndicator = object.Get(2).ToNumber().Int32Value();  //Get the indicator(str/int). // MI these were all -> before
      
      sqlReturnCode = SQLDescribeParam(stmth, i + 1, 
                          &param[i].paramType, 
                          &param[i].paramSize, 
                          &param[i].decDigits, 
                          &param[i].nullable);
      if (sqlReturnCode != SQL_SUCCESS)
        throwErrMsg(SQL_ERROR, "SQLDescribeParam() failed.", env);

      Napi::Value value = object.Get((uint32_t)0); // have to cast otherwise it complains about ambiguity
      
      if(bindIndicator == 0 || bindIndicator == 1) { //Parameter is string 
        //String::Utf8Value string(value);
        const char* string = value.ToString().Utf8Value().c_str();
        param[i].valueType = SQL_C_CHAR;
        if(param[i].io == SQL_PARAM_INPUT) {
          param[i].buf = strdup(string);
          if(bindIndicator == 0) //CLOB
            param[i].ind = strlen(string);
          else if(bindIndicator == 1) //NTS
            param[i].ind = SQL_NTS;
        }
        else if(param[i].io == SQL_PARAM_OUTPUT) {
          param[i].buf = (char*)calloc(param[i].paramSize + 1, sizeof(char));
          param[i].ind = param[i].paramSize;
        }
        else if(param[i].io == SQL_PARAM_INPUT_OUTPUT) {
          param[i].buf = (char*)calloc(param[i].paramSize + 1, sizeof(char));
          strcpy((char*)param[i].buf, string);
          if(bindIndicator == 0) //CLOB
            param[i].ind = strlen(string);
          else if(bindIndicator == 1) //NTS
            param[i].ind = SQL_NTS;
        }
      }
      else if(bindIndicator == 2) { //Parameter is Integer
        int64_t *number = (int64_t*)malloc(sizeof(int64_t));
        *number = value.ToNumber().Int32Value();
        param[i].valueType = SQL_C_BIGINT;
        param[i].buf = number;
        param[i].ind = 0;
      }
      else if(bindIndicator == 3) { //Parameter is NULL
        param[i].valueType = SQL_C_DEFAULT;
        param[i].ind = SQL_NULL_DATA;
      }
      else if(value.IsNumber() || bindIndicator == 4) { //Parameter is Decimal
        double *number = (double*)malloc(sizeof(double));
        *number = value.ToNumber();
        param[i].valueType = SQL_C_DOUBLE;
        param[i].buf = number;
        param[i].ind = sizeof(double);
        param[i].decDigits = 7;
        param[i].paramSize = sizeof(double);
      }
      else if(value.IsBoolean() || bindIndicator == 5) { //Parameter is Boolean
        bool *boolean = (bool*)malloc(sizeof(bool));
        *boolean = value.ToBoolean();
        param[i].valueType = SQL_C_BIT;
        param[i].buf = boolean;
        param[i].ind = 0;
      }
  
      sqlReturnCode = SQLBindParameter(stmth, i + 1, 
              param[i].io, 
              param[i].valueType, 
              param[i].paramType, 
              param[i].paramSize, 
              param[i].decDigits, 
              param[i].buf, 0, 
              &param[i].ind);
      if(isDebug)
        printf("SQLBindParameter(%d) TYPE[%2d] SIZE[%3d] DIGI[%d] IO[%d] IND[%3d]\n", sqlReturnCode, param[i].paramType, param[i].paramSize, param[i].decDigits, param[i].io, param[i].ind);
    }
  }
  
  int DbStmt::fetchSp(Napi::Env env, Napi::Array *array) {
    for(int i = 0, j = 0; i < paramCount; i++) {
      db2_param* p = &param[i];
      if(p->io != SQL_PARAM_INPUT) {
        if(p->valueType == SQL_C_BIGINT)  // Integer
          array->Set(j, Napi::Number::New(env, *(int64_t*)p->buf).Int32Value());
        else if(p->valueType == SQL_C_DOUBLE)  // Decimal
          array->Set(j, Napi::Number::New(env, *(double*)p->buf));
        else if(p->valueType == SQL_C_BIT)  // Boolean
          array->Set(j, Napi::Boolean::New(env, *(bool*)p->buf));
        else
          array->Set(j, Napi::String::New(env, (char*)p->buf));
        j++;
      }
    }
    freeSp();
    return 0;
  }
  
  
  int DbStmt::fetch(Napi::Env env, Napi::Object* row) {
    for(int i = 0; i < colCount; i++)
    {
      Napi::Value value;
      if(dbColumn[i].rlength == SQL_NULL_DATA){
        value = env.Null();
      }
      else {
        switch(dbColumn[i].sqlType) {
          case SQL_VARBINARY :
          case SQL_BINARY : //TODO Solve Buffer to Allow SQL_VARBINARY & SQL_BINARY
            // value = Napi::Buffer<Napi::Value>::New(env, rowData[i], dbColumn[i].rlength);
            break;
          case SQL_BLOB : //TODO
            // value = Local<Value>::New(isolate, node::Buffer::New(isolate, rowData[i] + sizeof(int), *(int*)rowData[i]).ToLocalChecked());
            break;
          default : 
            value = Napi::String::New(env,rowData[i]);
            break;
        }
      }
      row->Set(Napi::String::New(env , (char const*)dbColumn[i].name), value );
    }
    return 0;
  }

  void DbStmt::printError(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt)
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
  
  void DbStmt::printErrorToLog()
  {
    SQLCHAR buffer[SQL_MAX_MESSAGE_LENGTH + 1];
    SQLCHAR sqlstate[SQL_SQLSTATE_SIZE + 1];
    SQLINTEGER sqlcode;
    SQLSMALLINT length;

    SQLRETURN sqlReturnCode = SQLGetDiagRec(SQL_HANDLE_STMT, stmth, 1, sqlstate, &sqlcode, buffer, SQL_MAX_MESSAGE_LENGTH + 1, &length); 
    if (sqlReturnCode == SQL_SUCCESS) {
      if(buffer[length-1] == '\n') {
        SQLCHAR *p = &buffer[length-1];
        *p = '\0';
      }
      sprintf((char*)msg, "SQLSTATE=%s SQLCODE=%d %s", sqlstate, (int)sqlcode, buffer);
    }
  }
  
  void DbStmt::throwErrMsg(int handleType, Napi::Env env) 
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
    SQLRETURN sqlReturnCode = -1;
    
    if(handleType == SQL_HANDLE_STMT && stmtAllocated == true) {
      sqlReturnCode = SQLGetDiagRec(SQL_HANDLE_STMT, stmth, 1, sqlstate, &sqlcode, msg, SQL_MAX_MESSAGE_LENGTH + 1, &length); 
      printError(envh, connh, stmth);
    }
    else
      return;
    if  (sqlReturnCode == SQL_SUCCESS) {
      if (msg[length-1] == '\n') {
        p = &msg[length-1];
        *p = '\0';
      }
      sprintf((char *)errMsg, "SQLSTATE=%s SQLCODE=%d %s", sqlstate, (int)sqlcode, msg);
    }
    // isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, errMsg)));
    Napi::Error::New(env, String::New(env, errMsg)).ThrowAsJavaScriptException();
    return;
  }
  
  void DbStmt::throwErrMsg(int code, const char* msg, Napi::Env env)
  {
    SQLCHAR errMsg[SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10];
    sprintf((char *)errMsg, "SQLSTATE=PAERR SQLCODE=%d %s", code, msg);
    Napi::Error::New(env, String::New(env, errMsg)).ThrowAsJavaScriptException();
    return;
  }