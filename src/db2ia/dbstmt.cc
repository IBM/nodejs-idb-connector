/*  The Source code for this program is not published or otherwise  
 *  divested of its trade secrets, irrespective of what has been
 *  deposited with the U.S. Copyright Office.                        
*/

#include "dbstmt.h"

Napi::FunctionReference DbStmt::constructor;
SQLHENV DbStmt::envh;

DbStmt::DbStmt(const Napi::CallbackInfo &info) : Napi::ObjectWrap<DbStmt>(info)
{
  Napi::Env env = info.Env();
  SQLRETURN sqlReturnCode;

  //TODO: Validate that info[0] is really DbConn Object
  CHECK(!info[0].IsObject(), INVALID_PARAM_TYPE, "Expected Dbconn Object as a parameter", env);
  DbConn *conn = Napi::ObjectWrap<DbConn>::Unwrap(info[0].As<Napi::Object>());
  CHECK(!conn->connected, STMT_NOT_READY, "The Dbconn Object is not connected", env);

  //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnastmt.htm
  sqlReturnCode = SQLAllocStmt(conn->connh, //SQLHDBC hdbc -Connection Handle
                               &stmth);     //SQLHSTMT* phstmt -Pointer to Statement handle

  if (sqlReturnCode != SQL_SUCCESS)
  {
    SQLFreeStmt(stmth, SQL_CLOSE);
    throwErrMsg(SQL_HANDLE_STMT, stmth, env);
    return;
  }

  isDebug = conn->isDebug;
  envh = conn->envh;
  connh = conn->connh;
  myDbConn = conn;
  stmtAllocated = true; // Any SQL Statement Handler processing can not be allowed before this.
}

//DbStmt::~DbStmt() {}

Napi::Object DbStmt::Init(Napi::Env env, Napi::Object exports)
{
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

    InstanceMethod("bindParameters", &DbStmt::BindParameters),
    InstanceMethod("bindParametersSync", &DbStmt::BindParametersSync),

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
    InstanceMethod("fieldInfo", &DbStmt::FieldInfo),

    InstanceMethod("stmtError", &DbStmt::StmtError),
    InstanceMethod("getStmtDiag", &DbStmt::StmtError),
    InstanceMethod("close", &DbStmt::Close),

    InstanceMethod("asNumber", &DbStmt::AsNumber),
  });

  constructor = Napi::Persistent(constructorFunc);
  constructor.SuppressDestruct();

  exports.Set("dbstmt", constructorFunc);
  return exports;
}

/*
 *  DbStmt::AsNumber
 *    Description: Turn on or off automatic numberic data convertion:
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes 0 or 1 argument.
 *        info[0] (Boolean): true for ON false for OFF.
 *    Returns: boolean true/false indicating the state of the debug switch.
 * 
 */
Napi::Value DbStmt::AsNumber(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  //validation
  CHECK_WITH_RETURN(length != 0 && length != 1, INVALID_PARAM_NUM, "asNumber() Expected 0 or 1 Parameter", env, env.Null())
  if (length == 1)
  {
    CHECK_WITH_RETURN(!info[0].IsBoolean(), INVALID_PARAM_TYPE, "asNumber() Expected 1st Parameter to be a Boolean ", env, env.Null())
    this->asNumber = Napi::Boolean(env, info[0]).Value();
  }
  return Napi::Boolean::New(env, this->asNumber);
}

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
 *    Return: Boolean True if no errors occured. Otherwise error is thrown.
 */
Napi::Value DbStmt::SetStmtAttr(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN(info.Length() != 2, INVALID_PARAM_NUM, "Expected Two Parameters for SetStmtAttr", env, env.Null())
  CHECK_WITH_RETURN(!info[0].IsNumber(), INVALID_PARAM_TYPE, "Number Expected For first Parameter of SetStmtAttr", env, env.Null())
  CHECK_WITH_RETURN(!(info[1].IsNumber() || info[1].IsString()), INVALID_PARAM_TYPE, "Number || String Expected For Second Parameter of SetStmtAttr", env, env.Null())
  CHECK_WITH_RETURN(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", env, env.Null());

  SQLINTEGER attr = Napi::Number(env, info[0]).Int32Value();
  char *cValue;
  SQLINTEGER sLen = 0;
  SQLRETURN sqlReturnCode = -1;

  if (info[1].IsNumber())
  {
    int param = Napi::Number(env, info[1]).Int32Value();
    //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnsstma.htm
    sqlReturnCode = SQLSetStmtAttr(this->stmth, //SQLHSTMT hstmt -Statement handle
                                   attr,        //SQLINTEGER fAttr -Attribute to set
                                   &param,      //SQLPOINTER vParam -Value for fAttr
                                   0);          //SQLINTEGER sLen -Length of data (if string)
    DEBUG(this, "SetStmtAttr() attr = %d, value = %d, sqlReturnCode = %d\n", (int)attr, param, (int)sqlReturnCode);
  }
  else
  { // info[1].IsString()
    std::string arg1 = Napi::String(env, info[1]).Utf8Value();
    std::vector<char> arg1Vec(arg1.begin(), arg1.end());
    arg1Vec.push_back('\0');
    cValue = &arg1Vec[0];
    sLen = strlen(cValue);
    sqlReturnCode = SQLSetStmtAttr(this->stmth, attr, cValue, sLen);
    DEBUG(this, "SetStmtAttr() attr = %d, value = %s, sqlReturnCode = %d\n", (int)attr, cValue, (int)sqlReturnCode);
  }

  if (sqlReturnCode != SQL_SUCCESS)
  {
    throwErrMsg(SQL_HANDLE_STMT, stmth, env);
  }
  return Napi::Boolean::New(env, 1);
}

/*
 *  DbStmt::GetStmtAttr
 *    Description: Returns the current settings for the specified statement option
 *    Parameters: 
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes two arguments, stored on the info object:
 *          info[0] (Number): Attribute is the statement attribute to set.
 *                            Refer to the attribute table for more details.
 *    Return: The attribute option in the format of a Number or a String.
 * 
 */
Napi::Value DbStmt::GetStmtAttr(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN((info.Length() != 1), INVALID_PARAM_NUM, "Expected One Parameter for GetStmtAttr", env, env.Null());
  CHECK_WITH_RETURN((!info[0].IsNumber()), INVALID_PARAM_TYPE, "Expected first Parameter of GetStmtAttr to a Number", env, env.Null());
  CHECK_WITH_RETURN((this->stmtAllocated == false), STMT_NOT_READY, "The SQL Statment handler is not initialized.", env, env.Null());

  SQLINTEGER attr = Napi::Number(env, info[0]).Int32Value();
  char buf[SQL_MAX_MESSAGE_LENGTH];
  int retVal = 0;
  SQLINTEGER sLen = 0;
  void *pValue = (char *)&buf;
  //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfngstma.htm
  SQLRETURN sqlReturnCode = SQLGetStmtAttr(this->stmth, //SQLHSTMT hstmt -Statement handle
                                           attr,        //SQLINTEGER fAttr -Attribute to retrieve
                                           pValue,      //SQLPOINTER pvParam -Pointer to req attr
                                           sizeof(buf), //SQLINTEGER bLen -Max # bytes to store pvParam
                                           &sLen);      //SQLINTEGER sLen -Length of output data (if string)

  if (sqlReturnCode == SQL_SUCCESS)
  {
    if (!sLen)
    { //If the returned value is a number.
      pValue = &retVal;
      sqlReturnCode = SQLGetStmtAttr(this->stmth, attr, pValue, 0, &sLen);
      DEBUG(this, "GetStmtAttr() attr = %d, value = %d, sqlReturnCode = %d\n", (int)attr, *(int *)pValue, (int)sqlReturnCode)
      return Napi::Number::New(env, *(int *)pValue);
    }
    else
    { //If the returned value is a string.
      DEBUG(this, "GetStmtAttr() attr = %d, value = %s, sqlReturnCode = %d\n", (int)attr, (char *)pValue, (int)sqlReturnCode)
      return Napi::String::New(env, buf);
    }
  }

  // sqlReturnCode != SQL_SUCCESS
  throwErrMsg(SQL_HANDLE_STMT, stmth, env);
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
class ExecAsyncWorker : public Napi::AsyncWorker
{
public:
  ExecAsyncWorker(DbStmt *dbStatementObject,
                  SQLCHAR *sqlStatement,
                  Napi::Function &callback) : Napi::AsyncWorker(callback),
                                              dbStatementObject(dbStatementObject),
                                              sqlStatement(sqlStatement) {}
  ~ExecAsyncWorker() {}

  // Executed inside the worker-thread.
  void Execute()
  {
    SQLRETURN sqlReturnCode;

    //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnexecd.htm
    sqlReturnCode = SQLExecDirect(dbStatementObject->stmth, //SQLHSTMT hstmt -Statement handle
                                  sqlStatement,             //SQLCHAR* szSQLStr -SQL statement string
                                  SQL_NTS);                 //SQLINTEGER cbSQlStr -Length of szSQLStr
    DEBUG(dbStatementObject, "SQLExecDirect(%d): %s\n", sqlReturnCode, sqlStatement);
    // Return codes:
    //  - SQL_SUCCESS
    //  - SQL_SUCCESS_WITH_INFO
    //  - SQL_ERROR
    //  - SQL_INVALID_HANDLE
    //  - SQL_NO_DATA_FOUND
    // SQL_NO_DATA_FOUND is returned if the SQL statement is a Searched UPDATE 
    // or Searched DELETE and no rows satisfy the search condition.
    if (sqlReturnCode == SQL_SUCCESS_WITH_INFO)
    {
      sqlError errObj = returnErrObj(SQL_HANDLE_STMT, dbStatementObject->stmth);
      DEBUG(dbStatementObject, "SQLExecDirect SUCCESS_WITH_INFO (%d) %s\n", errObj.sqlCode, errObj.sqlState);
      if (!strcmp(errObj.sqlState, "0100C") && errObj.sqlCode == 466)
      {
        // Stored Procedures with Result Sets
        dbStatementObject->resultSetAvailable = true;
      }
    }
    else if (sqlReturnCode != SQL_SUCCESS && sqlReturnCode != SQL_NO_DATA_FOUND)
    {
      std::string errorMessage = returnErrMsg(SQL_HANDLE_STMT, dbStatementObject->stmth);
      SetError(errorMessage);
      return;
    }
    //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnnrcol.htm
    sqlReturnCode = SQLNumResultCols(dbStatementObject->stmth,      //SQLHSTMT hstmt -Statement handle
                                     &dbStatementObject->colCount); //SQLSMALLINT* pccol -# of Coluumns (Output param)

    DEBUG(dbStatementObject, "SQLNUMRESULTSCOLS(%d) Column Count = %d\n", sqlReturnCode, dbStatementObject->colCount)
    if (dbStatementObject->colCount == 0)
    { /* statement is not a select statement */
      return;
    }

    dbStatementObject->resultSetAvailable = true;
    if (dbStatementObject->bindColData(NULL) < 0)
    {
      DEBUG(dbStatementObject, "bindColData is < 0 \n")
      SetError("Error: During bindColData()");
      return;
    }
    // Grabs data from SQL and puts it in DbStmt result array. Converted to Napi values in ExecAsyncAfter (need environment var)
    dbStatementObject->fetchData();
  }

  void OnError(const Napi::Error &e)
  {
    // callback signature function(result, error)
    Callback().MakeCallback(Receiver().Value(), std::initializer_list<napi_value>{e.Env().Null(), e.Value()});
  }

  // Executed when the async work is complete
  void OnOK()
  {
    Napi::Env env = Env();
    Napi::HandleScope scope(Env());

    std::vector<napi_value> callbackArguments;
    Napi::Array results = Napi::Array::New(env);

    if (dbStatementObject->colCount == 0)
    { /* statement is not a select statement (No Result Set)*/
      DEBUG(dbStatementObject, "NO RESULTS: SQLExecDirect() call")
      //callback signature function(result, error)
      callbackArguments.push_back(Env().Null());
      callbackArguments.push_back(Env().Null());
      Callback().Call(callbackArguments);
      return;
    }
    //TODO: Handle if an Error Occurs from buildJsObject
    //Currently buildJsObject() will always return 0
    dbStatementObject->buildJsObject(env, &results);

    //callback signature function(result, error)
    callbackArguments.push_back(results);
    callbackArguments.push_back(Env().Null());
    Callback().Call(callbackArguments);
  }

private:
  DbStmt *dbStatementObject;
  SQLCHAR *sqlStatement;
};

/*
 *  DbStmt::Exec
 *    Syntex exec(string SQL, function Callback(result, error))
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
void DbStmt::Exec(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  // check arguments
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", env);
  CHECK(info.Length() != 2, INVALID_PARAM_NUM, "The exec() method accepts only two parameters.", env);
  CHECK(!info[0].IsString(), INVALID_PARAM_TYPE, "Argument 1 Must be a String", env)
  CHECK(!info[1].IsFunction(), INVALID_PARAM_TYPE, "Argument 2 Must be a Function", env)

  std::string sqlString = Napi::String(env, info[0]).Utf8Value();
  std::vector<char> sqlStringVec(sqlString.begin(), sqlString.end());
  sqlStringVec.push_back('\0');

  SQLCHAR *sqlStringPtr = &sqlStringVec[0];
  Napi::Function callback = info[1].As<Napi::Function>();

  // send off to the worker
  ExecAsyncWorker *worker = new ExecAsyncWorker(this, strdup(sqlStringPtr), callback);
  worker->Queue();
}

/*
 *  DbStmt::ExecSync
 *    Syntex: execSync(string SQL) or execSync(string SQL, function Callback(result))
 *    Description:
 *      Runs the "Exec" workflow synchronously, blocking the Node.js event
 *      loop.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by the N-API from the JavaScript call.
 *        Contains 1 or 2 parameters, with the 2nd being a callback function
 *        that can be used to check for errors and access data returned by the
 *        query.
 *    Returns: array of objects results if available & callback is not specified.
 */
Napi::Value DbStmt::ExecSync(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  std::string sqlString;
  Napi::Function cb;
  std::vector<napi_value> callbackArguments;
  DEBUG(this, "ExecSync().\n");

  // check arguments
  CHECK_WITH_RETURN(length != 1 && info.Length() != 2, INVALID_PARAM_NUM, "The execSync() method accepts only one or two parameters.", env, env.Null());
  CHECK_WITH_RETURN(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", env, env.Null());
  CHECK_WITH_RETURN(!info[0].IsString(), INVALID_PARAM_TYPE, "Argument 1 Must be a String", env, env.Null())
  if (length == 2)
  {
    CHECK_WITH_RETURN(!info[1].IsFunction(), INVALID_PARAM_TYPE, "Argument 2 Must be a Function", env, env.Null())
    cb = info[1].As<Napi::Function>();
  }
  sqlString = Napi::String(env, info[0]).Utf8Value();
  std::vector<char> sqlStringVec(sqlString.begin(), sqlString.end());
  sqlStringVec.push_back('\0');
  SQLCHAR *tmpSqlSt = &sqlStringVec[0];

  SQLRETURN sqlReturnCode = SQLExecDirect(this->stmth, tmpSqlSt, SQL_NTS);
  DEBUG(this, "SQLExecDirect(%d): %s\n", sqlReturnCode, tmpSqlSt);
  //check if an error occured
  if (sqlReturnCode == SQL_SUCCESS_WITH_INFO)
  {
    sqlError errObj = returnErrObj(SQL_HANDLE_STMT, stmth);
    DEBUG(this, "SQLExecDirect SUCCESS_WITH_INFO (%d) %s\n", errObj.sqlCode, errObj.sqlState);
    if (!strcmp(errObj.sqlState, "0100C") && errObj.sqlCode == 466)
    {
      // Stored Procedures with Result Sets
      this->resultSetAvailable = true;
    }
  }
  else if (sqlReturnCode != SQL_SUCCESS && sqlReturnCode != SQL_NO_DATA_FOUND)
  {
    if (length == 2)
    { // callback signature function(result , error)
      std::string errorMessage = returnErrMsg(SQL_HANDLE_STMT, stmth);
      Napi::Error error = Napi::Error::New(env, errorMessage);
      callbackArguments.push_back(Napi::Value(Env().Null()));
      callbackArguments.push_back(error.Value());
      cb.MakeCallback(env.Global(), callbackArguments);
      return env.Null();
    }
    else
    { //no callback provided throw the error
      throwErrMsg(SQL_HANDLE_STMT, stmth, env);
      return env.Null();
    }
  }
  //Check if result set is available
  SQLNumResultCols(this->stmth, &this->colCount);
  Napi::Array results = Napi::Array::New(env);

  /* determine statement type */
  if (this->colCount == 0)
  { /* statement is not a select statement (No Result Set) */
    DEBUG(this, "NO RESULTS: SQLExecDirect() call for (%s)\n", tmpSqlSt)
    if (length == 2)
    { //callback signature function (result, error)
      callbackArguments.push_back(env.Null());
      callbackArguments.push_back(env.Null());
      cb.MakeCallback(env.Global(), callbackArguments);
      return env.Null();
    }
    //no callback provided return the result == null in this case
    return env.Null();
  }

  DEBUG(this, "SQLExecDirect() call for (%s) Has Results\n", tmpSqlSt)
  this->resultSetAvailable = true;
  //bindColData() will throw error if it occurs
  if (this->bindColData(env) < 0)
  {
    return env.Null();
  }
  this->fetchData();
  //TODO: Handle if an Error Occurs from buildJsObject
  //Currently buildJsObject() will always return 0
  this->buildJsObject(env, &results);

  //callback signature function(results, error)
  if (length == 2)
  {
    callbackArguments.push_back(results);
    callbackArguments.push_back(env.Null());
    cb.MakeCallback(env.Global(), callbackArguments);
    this->freeColumns();
    return env.Null();
  }

  this->freeColumns();
  //no callback passed && sync method so return the results
  return results;
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
class PrepareAsyncWorker : public Napi::AsyncWorker
{
public:
  PrepareAsyncWorker(DbStmt *dbStatementObject,
                     Napi::Function &callback,
                     SQLCHAR *sqlStatement) : Napi::AsyncWorker(callback),
                                              dbStatementObject(dbStatementObject),
                                              sqlStatement(sqlStatement) {}
  ~PrepareAsyncWorker() {}

  // Executed inside the worker-thread.
  void Execute()
  {
    //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnprep.htm
    sqlReturnCode = SQLPrepare(dbStatementObject->stmth, //SQLHSTMT hstmt -Statement handle
                               sqlStatement,             //SQLCHAR* szSqlStr -SQL statement string
                               SQL_NTS);                 //SQLINTEGER cbSQLStr -Length of szSQLStr
    DEBUG(dbStatementObject, "SQLPrepare(%d): %s\n", sqlReturnCode, sqlStatement);

    if (sqlReturnCode != SQL_SUCCESS)
    {
      std::string errorMessage = returnErrMsg(SQL_HANDLE_STMT, dbStatementObject->stmth);
      if (errorMessage.length() != 0)
      {
        SetError(errorMessage);
        return;
      }
    }
  }

  void OnError(const Napi::Error &e)
  {
    //callback signature function(error)
    Callback().MakeCallback(Receiver().Value(), std::initializer_list<napi_value>{e.Value()});
  }

  // Executed when the async work is complete
  void OnOK()
  {
    Napi::Env env = Env();
    Napi::HandleScope scope(env);
    std::vector<napi_value> callbackArguments;
    //callback signature function(error)
    callbackArguments.push_back(env.Null());
    Callback().Call(callbackArguments);
  }

private:
  DbStmt *dbStatementObject;
  SQLCHAR *sqlStatement;
  SQLRETURN sqlReturnCode;
};

/*
 *  DbStmt::Prepare
 *    Syntex: prepare(string SQL, function Callback())
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
 *          info[1]: [Function]: The callback function, with
 *                   arguments passed to it in the format function(error):
 *                      error:   the error that was
 *                              encountered, or null if no error.
 */
void DbStmt::Prepare(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();

  DEBUG(this, "Prepare().\n");
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", env)
  CHECK(length != 2, INVALID_PARAM_NUM, "The prepare() method accepts only two parameters.", env)
  CHECK(!info[0].IsString(), INVALID_PARAM_TYPE, "Expected Parameter 1 to be a String", env)
  CHECK(!info[1].IsFunction(), INVALID_PARAM_TYPE, "Expected Parameter 2 to be a Function", env)

  std::string arg0 = Napi::String(env, info[0]).Utf8Value();
  std::vector<char> arg0Vec(arg0.begin(), arg0.end());
  arg0Vec.push_back('\0');

  SQLCHAR *sqlStringPtr = &arg0Vec[0];

  Napi::Function callback = info[1].As<Napi::Function>();
  PrepareAsyncWorker *worker = new PrepareAsyncWorker(this, callback, strdup(sqlStringPtr));
  worker->Queue();
}

/*
 *  DbStmt::PrepareSync
 *    Syntex: prepareSync(string SQL) , prepareSync(string SQL, function Callback())
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
 *          info[1] (Function) (Optional): The callback function, with
 *                              arguments passed to it in the format
 *                              function(error).
 */
void DbStmt::PrepareSync(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  DEBUG(this, "PrepareSync().\n");
  //Validation
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", env);
  CHECK(length != 1 && info.Length() != 2, INVALID_PARAM_NUM, "Expected 1 or 2 Parameters for prepare()", env)
  CHECK(!info[0].IsString(), INVALID_PARAM_TYPE, "Expected Parameter 1 to be a String", env)
  if (length == 2)
  {
    CHECK(!info[1].IsFunction(), INVALID_PARAM_TYPE, "Expected Parameter 2 to be a Function", env)
  }

  std::string sql = info[0].As<Napi::String>().Utf8Value();
  std::vector<char> sqlVec(sql.begin(), sql.end());
  sqlVec.push_back('\0');
  char *cValue = &sqlVec[0];

  SQLRETURN sqlReturnCode = SQLPrepare(this->stmth, cValue, SQL_NTS);
  DEBUG(this, "SQLPrepare(%d): %s\n", sqlReturnCode, cValue);
  //check if error occured
  if (sqlReturnCode != SQL_SUCCESS)
  {
    DEBUG(this, "PrepareSync() Failed.\n");
    std::string errorMessage = returnErrMsg(SQL_HANDLE_STMT, stmth);
    Napi::Error error = Napi::Error::New(env, errorMessage);
    Napi::Value errorValue = error.Value();

    //no callback was provided
    if (length != 2)
    {
      CHECK(sqlReturnCode != SQL_SUCCESS, SQL_ERROR, "PrepareSync() failed.", env);
    }
    else
    { //pass the error to the callback function , let the caller handle the error
      Napi::Function cb = info[1].As<Napi::Function>();
      cb.MakeCallback(env.Global(), {errorValue});
      return;
    }
  }
  //No Error Occured
  //callback provided Pass null back As the error
  if (info.Length() == 2)
  {
    Napi::Function cb = info[1].As<Napi::Function>();
    cb.MakeCallback(env.Global(), {env.Null()});
  }
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
class BindParamAsyncWorker : public Napi::AsyncWorker
{
public:
  BindParamAsyncWorker(DbStmt *dbStatementObject,
                       Napi::Array &parametersToBind,
                       Napi::Function &callback) : Napi::AsyncWorker(callback),
                                                   dbStatementObject(dbStatementObject),
                                                   parametersToBind(Napi::Persistent(parametersToBind)) {}
  ~BindParamAsyncWorker() {}

  // Executed inside the worker-thread.
  void Execute()
  {
    // no-op
  }

  // Executed when the async work is complete
  void OnOK()
  {
    Napi::Env env = Env();
    Napi::HandleScope scope(Env());
    Napi::Array array = this->parametersToBind.Value();
    std::vector<napi_value> callbackArguments;
    std::string error;

    int returnCode = dbStatementObject->bindParams(env, &array, error);
    //check if errors Occured.
    if (returnCode != 0)
    {
      //callback signature function(error)
      callbackArguments.push_back(Napi::Error::New(env, error).Value());
      Callback().Call(callbackArguments);
      return;
    }
    parametersToBind.Reset(); // unsure if needed, might auto refcount to 0
    //callback signature function(error)
    callbackArguments.push_back(Env().Null());
    Callback().Call(callbackArguments);
  }

private:
  DbStmt *dbStatementObject;
  Napi::Reference<Napi::Array> parametersToBind;
};

/*
 *  DbStmt::BindParam
 *    Syntex: bindParam(array ParamList, function Callback(error))
 *    Description:
 *      The entry point for running the "BindParam" workflow asynchronously.
 *      Run after a statement is prepared with the "Prepare" workflow.
 *      Handles passed in data before calling the BindParamAsyncWorker,
 *      which returns to the JavaScript environment through a callback.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call. Contains 2 parameters,
 * 
 *        info[0]: [Array]: An array of arrays, the inner arrays containing
 *                         the data to bind to the prepared statement.
 *        info[1]: [Function]: The callback function, with
 *                 arguments passed to it in the format function(error):
 *                 error:  the error that was encountered, or null if no error.
 */
void DbStmt::BindParam(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  DEBUG(this, "BindParamAsync().\n");
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", env)
  CHECK(info.Length() != 2, INVALID_PARAM_NUM, "The bindParam() method accepts only two parameters.", env)
  CHECK(!info[0].IsArray(), INVALID_PARAM_TYPE, "Expected Parameter 1 to be a Array", env);
  CHECK(!info[1].IsFunction(), INVALID_PARAM_TYPE, "Expected Parameter 2 to be a Function", env);

  Napi::Array parametersToBind = info[0].As<Napi::Array>();
  Napi::Function callback = info[1].As<Napi::Function>();

  BindParamAsyncWorker *worker = new BindParamAsyncWorker(this, parametersToBind, callback);
  worker->Queue();
}

/*
 *  DbStmt::BindParamSync
 *    Syntex: bindParamSync(array ParamList), bindParamSync(array ParamList, function Callback(error))
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
 *                      error:  the error that was encountered, or null if no error.
 */
void DbStmt::BindParamSync(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env); // This wasn't done in original
  int length = info.Length();
  Napi::Function cb;
  std::vector<napi_value> callbackArguments;
  std::string errorMsg;

  DEBUG(this, "BindParamSync().\n");

  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", env)
  CHECK(length != 1 && length != 2, INVALID_PARAM_NUM, "The bindParamSync() method accepts only one or two parameters.", env)
  CHECK(!info[0].IsArray(), INVALID_PARAM_TYPE, "Parameter 1 Must be an Array", env)
  if (length == 2)
  {
    CHECK(!info[1].IsFunction(), INVALID_PARAM_TYPE, "Parameter 2 must be a Function", env)
    cb = info[1].As<Napi::Function>();
  }

  Napi::Array parametersToBind = info[0].As<Napi::Array>();
  // Errors are checked for and thrown in this->bindParams
  int returnCode = this->bindParams(env, &parametersToBind, errorMsg);
  DEBUG(this, "bindParams(%d)\n", returnCode);
  //check if an error occured while binding
  if (returnCode != 0)
  {
    Napi::Error error = Napi::Error::New(env, errorMsg);
    if (length == 2)
    {
      //callback signature function(error)
      callbackArguments.push_back(error.Value());
      cb.Call(callbackArguments);
      return;
    }
    error.ThrowAsJavaScriptException();
  }
  //if callback was defined make the callback
  if (length == 2)
  {
    //callback signature function(error)
    callbackArguments.push_back(Env().Null());
    cb.Call(callbackArguments);
    return;
  }
}

/*
 *  BindParametersAsyncWorker
 *    Description:
 *      Asynchronous worker class that runs the 'BindParameters' workflow. Runs
 *      asynchronous code in the Execute() function, and then compiles the
 *      results, calls the callback function, and cleans up memory in OnOk().
 */
class BindParametersAsyncWorker : public Napi::AsyncWorker
{
public:
  BindParametersAsyncWorker(DbStmt *dbStatementObject,
                       Napi::Array &parametersToBind,
                       Napi::Function &callback) : Napi::AsyncWorker(callback),
                                                   dbStatementObject(dbStatementObject),
                                                   parametersToBind(Napi::Persistent(parametersToBind)) {}
  ~BindParametersAsyncWorker() {}

  // Executed inside the worker-thread.
  void Execute()
  {
    // no-op
  }

  // Executed when the async work is complete
  void OnOK()
  {
    Napi::Env env = Env();
    Napi::HandleScope scope(Env());
    Napi::Array array = this->parametersToBind.Value();
    std::vector<napi_value> callbackArguments;
    std::string error;

    int returnCode = dbStatementObject->bindParams(env, &array, error);
    //check if errors Occured.
    if (returnCode != 0)
    {
      //callback signature function(error)
      callbackArguments.push_back(Napi::Error::New(env, error).Value());
      Callback().Call(callbackArguments);
      return;
    }
    parametersToBind.Reset(); // unsure if needed, might auto refcount to 0
    //callback signature function(error)
    callbackArguments.push_back(Env().Null());
    Callback().Call(callbackArguments);
  }

private:
  DbStmt *dbStatementObject;
  Napi::Reference<Napi::Array> parametersToBind;
};

/*
 *  DbStmt::BindParameters
 *    Syntex: bindParameters(array ParamList, function Callback(error))
 *    Description:
 *      The entry point for running the "BindParameters" workflow asynchronously.
 *      Run after a statement is prepared with the "Prepare" workflow.
 *      Handles passed in data before calling the BindParametersAsyncWorker,
 *      which returns to the JavaScript environment through a callback.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call. Contains 2 parameters,
 * 
 *        info[0]: [Array]: An array of the data to bind to the prepared statement.
 *        info[1]: [Function]: The callback function, with
 *                 arguments passed to it in the format function(error):
 *                 error:  the error that was encountered, or null if no error.
 */
void DbStmt::BindParameters(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  DEBUG(this, "BindParameters().\n");
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", env)
  CHECK(info.Length() != 2, INVALID_PARAM_NUM, "The bindParameters() method accepts only two parameters.", env)
  CHECK(!info[0].IsArray(), INVALID_PARAM_TYPE, "Expected Parameter 1 to be a Array", env);
  CHECK(!info[1].IsFunction(), INVALID_PARAM_TYPE, "Expected Parameter 2 to be a Function", env);

  Napi::Array parametersToBind = info[0].As<Napi::Array>();
  Napi::Function callback = info[1].As<Napi::Function>();

  BindParametersAsyncWorker *worker = new BindParametersAsyncWorker(this, parametersToBind, callback);
  worker->Queue();
}

/*
 *  DbStmt::BindParametersSync
 *    Syntex: bindParametersSync(array ParamList), bindParametersSync(array ParamList, function Callback(error))
 *    Description:
 *      Runs the "BindParameters" workflow synchronously, blocking the Node.js event
 *      loop.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes one or two arguments, stored on the info object:
 *          info[0]: [Array]: An array of the data to bind to the prepared statement.
 *          info[1]: [Function] (Optional): The callback function, with
 *                   arguments passed to it in the format function(error):
 *                      error:  the error that was encountered, or null if no error.
 */
void DbStmt::BindParametersSync(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env); // This wasn't done in original
  int length = info.Length();
  Napi::Function cb;
  std::vector<napi_value> callbackArguments;
  std::string errorMsg;

  DEBUG(this, "BindParametersSync().\n");

  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", env)
  CHECK(length != 1 && length != 2, INVALID_PARAM_NUM, "The bindParametersSync() method accepts only one or two parameters.", env)
  CHECK(!info[0].IsArray(), INVALID_PARAM_TYPE, "Parameter 1 Must be an Array", env)
  if (length == 2)
  {
    CHECK(!info[1].IsFunction(), INVALID_PARAM_TYPE, "Parameter 2 must be a Function", env)
    cb = info[1].As<Napi::Function>();
  }

  Napi::Array parametersToBind = info[0].As<Napi::Array>();
  // Errors are checked for and thrown in this->bindParams
  int returnCode = this->bindParams(env, &parametersToBind, errorMsg);
  DEBUG(this, "bindParams(%d)\n", returnCode);
  //check if an error occured while binding
  if (returnCode != 0)
  {
    Napi::Error error = Napi::Error::New(env, errorMsg);
    if (length == 2)
    {
      //callback signature function(error)
      callbackArguments.push_back(error.Value());
      cb.Call(callbackArguments);
      return;
    }
    error.ThrowAsJavaScriptException();
  }
  //if callback was defined make the callback
  if (length == 2)
  {
    //callback signature function(error)
    callbackArguments.push_back(Env().Null());
    cb.Call(callbackArguments);
    return;
  }
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
class ExecuteAsyncWorker : public Napi::AsyncWorker
{
public:
  ExecuteAsyncWorker(DbStmt *dbStatementObject,
                     Napi::Function &callback) : Napi::AsyncWorker(callback),
                                                 dbStatementObject(dbStatementObject) {}
  ~ExecuteAsyncWorker() {}

  // Executed inside the worker-thread.
  void Execute()
  {
    //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnexec.htm
    sqlReturnCode = SQLExecute(dbStatementObject->stmth); //SQLHSTMT hstmt -Statement Handle
    DEBUG(dbStatementObject, "SQLExecuteAsync(%d):\n", sqlReturnCode);
    // Return codes
    //  - SQL_SUCCESS
    //  - SQL_SUCCESS_WITH_INFO
    //  - SQL_ERROR
    //  - SQL_INVALID_HANDLE
    //  - SQL_NO_DATA_FOUND
    //  - SQL_NEED_DATA
    // SQL_NO_DATA_FOUND is returned if the SQL statement is a Searched UPDATE
    // or Searched DELETE and no rows satisfy the search condition.
    if (sqlReturnCode == SQL_SUCCESS_WITH_INFO)
    {
      sqlError errObj = returnErrObj(SQL_HANDLE_STMT, dbStatementObject->stmth);
      if (!strcmp(errObj.sqlState, "0100C") && errObj.sqlCode == 466)
      {
        // Stored Procedures with Result Sets
        dbStatementObject->resultSetAvailable = true;
      }
    }
    else if (sqlReturnCode != SQL_SUCCESS && sqlReturnCode != SQL_NO_DATA_FOUND)
    {
      std::string errorMessage = returnErrMsg(SQL_HANDLE_STMT, dbStatementObject->stmth);
      if (errorMessage.length() != 0)
      {
        SetError(errorMessage);
        return;
      }
    }
    sqlReturnCode = SQLNumResultCols(dbStatementObject->stmth, &dbStatementObject->colCount);
    DEBUG(dbStatementObject, "SQLNUMRESULTSCOLS(%d) Column Count = %d\n", sqlReturnCode, dbStatementObject->colCount)

    /* determine statement type */
    if (dbStatementObject->colCount == 0)
    { /* statement is not a select statement */
      DEBUG(dbStatementObject, "NO RESULTS :SQLExecuteAsync()\n");
      return;
    }

    dbStatementObject->resultSetAvailable = true;
    if (dbStatementObject->bindColData(NULL) < 0)
    {
      DEBUG(dbStatementObject, "bindColData < 0 \n");
      return;
    }
  }

  //executed when SetError() is called
  void OnError(const Napi::Error &e)
  {
    //callback signature function(result, error)
    Callback().MakeCallback(Receiver().Value(), std::initializer_list<napi_value>{e.Env().Null(), e.Value()});
  }

  // Executed when the async work is complete
  void OnOK()
  {
    Napi::Env env = Env();
    Napi::HandleScope scope(env);
    std::vector<napi_value> callbackArguments;

    // param && paramCount are only set during bindParams
    // What this is doing is checking if parameters were bound
    // And get any output Params from the Stored Procedures if available.
    if (dbStatementObject->param && dbStatementObject->paramCount > 0)
    { // executeAsync(function(array) {...})
      Napi::Array results = Napi::Array::New(env);
      dbStatementObject->fetchSp(env, &results);

      if (results.Length() > 0)
      { // return output params
        callbackArguments.push_back(results);
      }
      else
      { // no output params to return
        callbackArguments.push_back(env.Null());
      }
    }
    else
    { //Paramters were not bound
      callbackArguments.push_back(env.Null());
    }
    callbackArguments.push_back(env.Null());
    Callback().Call(callbackArguments);
  }

private:
  DbStmt *dbStatementObject;
  SQLRETURN sqlReturnCode;
};

/*
 *  DbStmt::Execute
 *    Syntex: execute(function(OutputParameters, error))
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
 *          info[0]: [Function]: The callback function, with up to
 *                   two arguments passed to it:
 *                      arg1: Any output parameters of the executed statement, returned
 *                            in JSON format.
 *                      arg2: error object that was encountered, or null if no error.
 */
void DbStmt::Execute(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  DEBUG(this, "Execute().\n");
  CHECK(info.Length() != 1, INVALID_PARAM_NUM, "The execute() method accepts only one parameter.", env)
  CHECK(!info[0].IsFunction(), INVALID_PARAM_TYPE, "Expected the first parameter to be a Function.", env);

  Napi::Function callback = info[0].As<Napi::Function>();

  ExecuteAsyncWorker *worker = new ExecuteAsyncWorker(this, callback);
  worker->Queue();
}

/*
 *  DbStmt::ExecuteSync
 *    Syntex: executeSync(), executeSync(function(OutputParameters, error)) 
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
 *                      arg1: Any results of the executed statement, returned
 *                            in JSON format.
 *                      arg2: the error that was
 *                            encountered, or null if no error.
 */
Napi::Value DbStmt::ExecuteSync(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  Napi::Function cb;
  std::vector<napi_value> callbackArguments;
  SQLRETURN sqlReturnCode;

  DEBUG(this, "ExecuteSync().\n");
  //Validation
  CHECK_WITH_RETURN(length != 0 && length != 1, INVALID_PARAM_NUM, "The executeSync() method accepts 0 or 1 parameter.", env, env.Null())
  if (length == 1)
  {
    CHECK_WITH_RETURN(!info[0].IsFunction(), INVALID_PARAM_TYPE, "Expected the first parameter to be a Function.", env, env.Null());
    cb = info[0].As<Napi::Function>();
  }
  sqlReturnCode = SQLExecute(this->stmth);
  DEBUG(this, "SQLExecute(%d):\n", sqlReturnCode);
  //Check if errors occured
  if (sqlReturnCode == SQL_SUCCESS_WITH_INFO)
  {
    sqlError errObj = returnErrObj(SQL_HANDLE_STMT, stmth);
    if (!strcmp(errObj.sqlState, "0100C") && errObj.sqlCode == 466)
    {
      // Stored Procedures with Result Sets
      this->resultSetAvailable = true;
    }
  }
  else if (sqlReturnCode != SQL_SUCCESS)
  {
    if (length == 1)
    { // callback signature function(result , error)
      std::string errorMessage = returnErrMsg(SQL_HANDLE_STMT, stmth);
      Napi::Error error = Napi::Error::New(env, errorMessage);
      callbackArguments.push_back(Napi::Value(Env().Null()));
      callbackArguments.push_back(error.Value());
      cb.MakeCallback(env.Global(), callbackArguments);
      return env.Null();
    }
    else
    { //no callback provided throw the error
      throwErrMsg(SQL_HANDLE_STMT, stmth, env);
      return env.Null();
    }
  }

  sqlReturnCode = SQLNumResultCols(this->stmth, &this->colCount);
  DEBUG(this, "SQLNumResultsCols(%d) Has Results\n", sqlReturnCode)
  if (sqlReturnCode != SQL_SUCCESS)
  {
    throwErrMsg(SQL_HANDLE_STMT, stmth, env);
  }
  if (this->colCount > 0)
  { /* there is a result set*/
    DEBUG(this, "SQLExecuteSync() Has Results\n")
    this->resultSetAvailable = true;
    int returnCode = this->bindColData(env);
    DEBUG(this, "bindColData(%d)\n", returnCode)
    if (returnCode < 0)
    {
      Napi::Error::New(env, "Error While BindingColData").ThrowAsJavaScriptException();
    }
  }

  // Parameters were bound
  if (this->param && this->paramCount > 0)
  {
    Napi::Array array = Napi::Array::New(env);
    // FetchSp gets back output params from Stored Procedures.
    this->fetchSp(env, &array);
    if (length == 1)
    { //callback was defined
      if (array.Length() > 0)
      {
        //callback signature function(outParams, error)
        callbackArguments.push_back(array);
        callbackArguments.push_back(env.Null());
      }
      else
      {
        //callback signature function(outParams, error)
        callbackArguments.push_back(env.Null());
        callbackArguments.push_back(env.Null());
      }

      cb.MakeCallback(env.Global(), callbackArguments);
      return env.Null();
    }
    else
    { //callback was not defined
      if (array.Length() > 0)
      {
        return array;
      }
      else
      {
        return env.Null();
      }
    }
  }
  //there was no paramters bound
  if (length == 1)
  { //callback was defined
    //callback signature function(outParams, error)
    callbackArguments.push_back(env.Null());
    callbackArguments.push_back(env.Null());
    cb.MakeCallback(env.Global(), callbackArguments);
  }
  //callback was not defined
  // there were no output params to return
  return env.Null();
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
class FetchAsyncWorker : public Napi::AsyncWorker
{
public:
  FetchAsyncWorker(DbStmt *dbStatementObject,
                   int originalArgumentsLength,
                   int orient,
                   int offset,
                   Napi::Function &callback) : Napi::AsyncWorker(callback),
                                               dbStatementObject(dbStatementObject),
                                               originalArgumentsLength(originalArgumentsLength),
                                               orient(orient),
                                               offset(offset) {}
  ~FetchAsyncWorker() {}

  // Executed inside the worker-thread.
  void Execute()
  {
    if (originalArgumentsLength == 3)
    {
      int retVal = 0;
      sqlReturnCode = SQLGetStmtAttr(dbStatementObject->stmth, SQL_ATTR_CURSOR_SCROLLABLE, &retVal, 0, 0);
      DEBUG(dbStatementObject, "SQLGetStmtAttr(%d) Scrollable = %d.\n", sqlReturnCode, retVal);

      if (retVal == SQL_TRUE)
      {
        //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfetchsc.htm
        sqlReturnCode = SQLFetchScroll(dbStatementObject->stmth, //SQLHSTMT hstmt -Statement handle
                                       orient,                   //SQLSMALLINT fOrient -Fetch Orientation
                                       offset);                  //SQLINTEGER fOffset -Row offset
        DEBUG(dbStatementObject, "SQLFetchScroll(%d) orient = %d, offset = %d.\n", sqlReturnCode, orient, offset);
        //handle if an error occured
        if (sqlReturnCode == SQL_ERROR)
        {
          std::string errorMessage = returnErrMsg(SQL_HANDLE_STMT, dbStatementObject->stmth);
          SetError(errorMessage);
          return;
        }
      }
      DEBUG(dbStatementObject, "Cursor is not scrollable.\n");
    }
    //Perform Fetch
    //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnfetch.htm
    sqlReturnCode = SQLFetch(dbStatementObject->stmth);
    DEBUG(dbStatementObject, "SQLFetch(%d).\n", sqlReturnCode);
    //handle if an error occured
    if (sqlReturnCode == SQL_ERROR)
    {
      std::string errorMessage = returnErrMsg(SQL_HANDLE_STMT, dbStatementObject->stmth);
      SetError(errorMessage);
      return;
    }
  }

  //executed when SetError() is called , when sqlReturnCode == SQL_ERROR
  void OnError(const Napi::Error &e)
  {
    //callback signature function(row, error)
    Callback().MakeCallback(Receiver().Value(), std::initializer_list<napi_value>{e.Env().Null(), e.Value()});
  }

  // Executed when the async work is complete
  void OnOK()
  {
    Napi::Env env = Env();
    Napi::HandleScope scope(Env());
    std::vector<napi_value> callbackArguments;
    Napi::Object row = Napi::Object::New(env);

    //set the data to the row object.
    dbStatementObject->fetch(env, &row);
    //callback signature function(row, error)
    callbackArguments.push_back(row);
    callbackArguments.push_back(Napi::Number::New(env, sqlReturnCode));

    Callback().Call(callbackArguments);
  }

private:
  DbStmt *dbStatementObject;
  int originalArgumentsLength;
  int orient;
  int offset;
  SQLRETURN sqlReturnCode;
};

/*
 *  DbStmt::Fetch
 *    Syntex 1: fetch(function Callback(Row, ReturnCode/error))
 *    Syntex 2: fetch(int Orient, int Offset, function Callback(Row, ReturnCode/error))
 *    Description:
 *      Advances the cursor to the next row of the result set, and retrieves any bound columns. 
 *      Or positions the cursor based on the requested orientation.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes 1 or 3 arguments depending on the syntex, stored on the info object:
 *          info[0] or info[2]: [Function]: The callback function, with up to
 *                   two arguments passed to it:
 *                      arg1: One Row from the executed statement, returned
 *                            in JSON format.
 *                      arg2: the error that was encountered, indicating sql return code.
 */

void DbStmt::Fetch(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  int orient = 0, offset = 0;

  DEBUG(this, "fetchAsync().\n");
  //Validate Arguments
  CHECK(length != 1 && length != 3, INVALID_PARAM_NUM, "The fetch() method accept only one or three parameters.", env)
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", env)
  CHECK(this->resultSetAvailable == false, RSSET_NOT_READY, "There is no result set to be queried. Please execute a SQL command first.", env);
  CHECK(!info[length - 1].IsFunction(), INVALID_PARAM_TYPE, "Final Argument Must be a Function", env)
  Napi::Function callback = info[length - 1].As<Napi::Function>();

  if (length == 3)
  {
    CHECK(!info[0].IsNumber(), INVALID_PARAM_TYPE, "Argument 1 Must be a Number", env)
    CHECK(!info[1].IsNumber(), INVALID_PARAM_TYPE, "Argument 2 Must be a Number", env)
    orient = Napi::Number(env, info[0]).Int32Value();
    offset = Napi::Number(env, info[1]).Int32Value();
  }

  FetchAsyncWorker *worker = new FetchAsyncWorker(this, length, orient, offset, callback);
  worker->Queue();
}

/*
 * Syntex: fetchSync(), fetchSync(function(Row, Return Code)),
 *          fetchSync(int Orient, int Offset, function(Row, ReturnCode))
 *
 *  DbStmt::FetchSync
 *    Description:
 *      Runs the "Fetch" workflow synchronously, blocking the Node.js event
 *      loop. Should only be called after statment has been prepared & executed
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes 1 or 3 arguments depending on the syntex, stored on the info object:
 *          info[0]: [Function] (Optional): The callback function, with up to
 *                   two arguments passed to it:
 *                      arg1: The row (Object) that was fetched, if an error did not occur
 *                      arg2: An Error Object indicating sql return code.
 *    Returns: row (Object) if a callback function was not specified.
 */
Napi::Value DbStmt::FetchSync(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length(),
      orient = 0,
      offset = 0;
  Napi::Function cb;
  std::vector<napi_value> callbackArguments;
  SQLRETURN sqlReturnCode;

  DEBUG(this, "fetchSync().\n");
  //Validate Arguments
  CHECK_WITH_RETURN(length != 0 && length != 1 && length != 3, INVALID_PARAM_NUM, "The fetch() method accept only zero , one or three parameters.", env, env.Null())
  CHECK_WITH_RETURN(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", env, env.Null())
  CHECK_WITH_RETURN(this->resultSetAvailable == false, RSSET_NOT_READY, "There is no result set to be queried. Please execute a SQL command first.", env, env.Null());
  if (length == 1 || length == 3)
  {
    CHECK_WITH_RETURN(!info[length - 1].IsFunction(), INVALID_PARAM_TYPE, "Final Argument Must be a Function", env, env.Null())
    cb = info[length - 1].As<Napi::Function>();
  }
  if (length == 3)
  {
    CHECK_WITH_RETURN(!info[0].IsNumber(), INVALID_PARAM_TYPE, "Argument 1 Must be a Number", env, env.Null())
    CHECK_WITH_RETURN(!info[1].IsNumber(), INVALID_PARAM_TYPE, "Argument 2 Must be a Number", env, env.Null())
    orient = Napi::Number(env, info[0]).Int32Value();
    offset = Napi::Number(env, info[1]).Int32Value();

    int retVal = 0;
    sqlReturnCode = SQLGetStmtAttr(this->stmth, SQL_ATTR_CURSOR_SCROLLABLE, &retVal, 0, 0);
    DEBUG(this, "SQLGetStmtAttr(%d) Scrollable = %d.\n", sqlReturnCode, retVal);

    if (retVal == SQL_TRUE)
    {
      sqlReturnCode = SQLFetchScroll(this->stmth, orient, offset);
      DEBUG(this, "SQLFetchScroll(%d) orient = %d, offset = %d.\n", sqlReturnCode, orient, offset);
      if (sqlReturnCode == SQL_ERROR)
      {
        throwErrMsg(SQL_HANDLE_STMT, stmth, env);
      }
    }
    DEBUG(this, "Cursor is not Scrollable\n");
  }

  //Perform Fetch
  sqlReturnCode = SQLFetch(this->stmth);
  DEBUG(this, "SQLFetch(%d).\n", sqlReturnCode);
  //handle if an error occured
  if (sqlReturnCode == SQL_ERROR)
  {
    //SQL_ERROR or SQL_SUCCESS DID not occur
    if (length == 1 || length == 3)
    { //run the callback signature function(row, returnCode)
      callbackArguments.push_back(env.Null());
      callbackArguments.push_back(Napi::Number::New(env, sqlReturnCode));
      cb.MakeCallback(env.Global(), callbackArguments);
    }
    //No callback was specified return the Return Code
    return Napi::Number::New(env, sqlReturnCode); // SQL_NO_DATA_FOUND indicate the end of the result set.
  }
  else
  {
    Napi::Object row = Napi::Object::New(env);
    this->fetch(env, &row);
    if (length == 1 || length == 3)
    { // Run call back signature function(row, returnCode)
      callbackArguments.push_back(row);
      callbackArguments.push_back(Napi::Number::New(env, sqlReturnCode));
      cb.MakeCallback(env.Global(), callbackArguments);
    }
    else
    { //callback was not given return row object
      return row;
    }
  }
  return env.Null();
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
class FetchAllAsyncWorker : public Napi::AsyncWorker
{
public:
  FetchAllAsyncWorker(DbStmt *dbStatementObject,
                      Napi::Function &callback) : Napi::AsyncWorker(callback),
                                                  dbStatementObject(dbStatementObject) {}
  ~FetchAllAsyncWorker() {}

  // Executed inside the worker-thread.
  void Execute()
  {
    returnCode = dbStatementObject->fetchData();
    if (returnCode == SQL_ERROR)
    {
      std::string errorMessage = returnErrMsg(SQL_HANDLE_STMT, dbStatementObject->stmth);
      SetError(errorMessage);
      return;
    }
  }

  //executed when SetError() is called , when sqlReturnCode == SQL_ERROR
  void OnError(const Napi::Error &e)
  {
    //callback signature function(result, error)
    Callback().MakeCallback(Receiver().Value(), std::initializer_list<napi_value>{e.Env().Null(), e.Value()});
  }

  // Executed when the async work is complete
  void OnOK()
  {
    Napi::Env env = Env(); // not sure this is right... old one just gets the Isolate out of thin air
    Napi::HandleScope scope(Env());
    std::vector<napi_value> callbackArguments;
    Napi::Array results = Napi::Array::New(env);

    //load up the array with data
    dbStatementObject->buildJsObject(env, &results);
    //callback signature function(result, error)
    callbackArguments.push_back(results);
    callbackArguments.push_back(env.Null());

    Callback().Call(callbackArguments);
  }

private:
  DbStmt *dbStatementObject;
  SQLRETURN returnCode;
};

/*
 * Syntex: fetchAll(function(result, error))
 *  DbStmt::FetchAll
 *    Description:
 *      Runs the "FetchAll" workflow asynchronously,
 *      Should only be called after statment has been prepared & executed
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes 1 argument stored on the info object:
 *          info[0]: [Function]: The callback function, with up to
 *                   two arguments passed to it:
 *                      arg1: The results (array of Objects) that was fetched, if an error did not occur
 *                      arg2: An Error Object indicating sql return code.
 */
void DbStmt::FetchAll(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  DEBUG(this, "FetchAllAsync().\n");
  //Validation
  CHECK(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", env)
  CHECK(this->resultSetAvailable == false, RSSET_NOT_READY, "There is no result set to be queried. Please execute a SQL command first.", env);
  CHECK(info.Length() != 1, INVALID_PARAM_NUM, "Expected Only 1 Argument For FetchAllAsync()", env)
  CHECK(!info[0].IsFunction(), INVALID_PARAM_TYPE, "Expected Argument 1 to be a Function", env)

  Napi::Function callback = info[0].As<Napi::Function>();
  FetchAllAsyncWorker *worker = new FetchAllAsyncWorker(this, callback);
  worker->Queue();
}

/*
 * Syntex: fetchAllSync() , fetchAllSync(function(result, error))
 *  DbStmt::FetchAllSync
 *    Description:
 *      Runs the "FetchAll" workflow asynchronously,
 *      Should only be called after statment has been prepared & executed
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes 0 or 1 argument depending on the syntex stored on the info object:
 *          info[0]: [Function] (Optional): The callback function, with up to
 *                   two arguments passed to it:
 *                      arg1: The results (array of Objects) that was fetched, if an error did not occur
 *                      arg2: An Error Object indicating sql return code.
 *    Returns: results (array of Objects) if a callback was not specified.
 */
Napi::Value DbStmt::FetchAllSync(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  Napi::Function cb;
  std::vector<napi_value> callbackArguments;

  DEBUG(this, "FetchAllSync().\n");
  //Validation
  CHECK_WITH_RETURN(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", env, env.Null())
  CHECK_WITH_RETURN(this->resultSetAvailable == false, RSSET_NOT_READY, "There is no result set to be queried. Please execute a SQL command first.", env, env.Null())
  CHECK_WITH_RETURN(length != 0 && length != 1, INVALID_PARAM_NUM, "Expected Only 1 Argument For FetchAllSync()", env, env.Null())
  if (length == 1)
  {
    CHECK_WITH_RETURN(!info[0].IsFunction(), INVALID_PARAM_TYPE, "Expected Argument 1 to be a Function", env, env.Null())
    cb = info[0].As<Napi::Function>();
  }

  Napi::Array results = Napi::Array::New(env);
  //check if error occured
  if (this->fetchData() < 0)
  {
    if (length == 1)
    {
      std::string errorMessage = returnErrMsg(SQL_HANDLE_STMT, stmth);
      callbackArguments.push_back(env.Null());
      callbackArguments.push_back(Napi::Error::New(env, errorMessage).Value());
      return env.Null();
    }
    throwErrMsg(SQL_HANDLE_STMT, stmth, env);
  }

  if (this->buildJsObject(env, &results) < 0)
  {
    DEBUG(this, "buildJsObject < 0\n")
    return env.Null();
  }
  //callback signature function(row)
  if (length == 1)
  {
    callbackArguments.push_back(results);
    callbackArguments.push_back(env.Null());
    cb.MakeCallback(env.Global(), callbackArguments);
    return env.Null();
  }
  //no callback provided return the results
  return results;
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
 *    Return: boolean true if successful otherwise throw an error.
 */
Napi::Value DbStmt::NextResult(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  SQLRETURN sqlReturnCode = SQLMoreResults(this->stmth);
  CHECK_WITH_RETURN(sqlReturnCode != SQL_SUCCESS, SQL_ERROR, "SQLMoreResults() failed.", env, env.Null())
  CHECK_WITH_RETURN(sqlReturnCode == SQL_NO_DATA_FOUND, SQL_ERROR, "No data for result set available.", env, env.Null())

  sqlReturnCode = SQLNumResultCols(this->stmth, &this->colCount);
  CHECK_WITH_RETURN(sqlReturnCode != SQL_SUCCESS, SQL_ERROR, "SQLNumResultCols() failed.", env, env.Null())

  if (this->colCount > 0)
  { // is a SELECT statement
    this->resultSetAvailable = true;
    //Some error occured if < 0
    if (this->bindColData(env) < 0)
    {
      Napi::Error::New(env, "bindColData failed During NextResult()").ThrowAsJavaScriptException();
      return env.Null();
    }
    //no errors occured
    return Napi::Boolean::New(env, 1);
  }

  //not results return
  return env.Null();
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
 *    Return: true if Rollback was succesful , otherwise throws an error.
 */
Napi::Value DbStmt::Commit(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", env, env.Null())
  SQLRETURN sqlReturnCode = SQLTransact(envh, this->connh, SQL_COMMIT);
  DEBUG(this, "SQLTransact(%d)\n", sqlReturnCode)
  CHECK_WITH_RETURN(sqlReturnCode != SQL_SUCCESS, SQL_ERROR, "SQLTransact(Commit) failed.", env, env.Null())
  return Napi::Boolean::New(env, 1);
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
 *    Return: true if Rollback was succesful , otherwise throws an error.
 */
Napi::Value DbStmt::Rollback(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN(this->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", env, env.Null())
  SQLRETURN sqlReturnCode = SQLTransact(envh, this->connh, SQL_ROLLBACK);
  DEBUG(this, "SQLTransact(%d)\n", sqlReturnCode)
  CHECK_WITH_RETURN(sqlReturnCode != SQL_SUCCESS, SQL_ERROR, "SQLTransact(Rollback) failed.", env, env.Null())
  return Napi::Boolean::New(env, 1);
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
 *    Return: true if successful, othherwise throws an error.
 */
Napi::Value DbStmt::CloseCursor(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  DEBUG(this, "SQLCloseCursor: stmth %d\n", this->stmth)
  SQLRETURN sqlReturnCode = SQLCloseCursor(this->stmth);
  DEBUG(this, "SQLCloseCursor(%d)\n", sqlReturnCode)
  CHECK_WITH_RETURN(sqlReturnCode != SQL_SUCCESS, SQL_ERROR, "SQLCloseCursor() failed.", env, env.Null())
  return Napi::Boolean::New(env, 1);
}

/*
 *  DbStmt::Reset
 *    Description: All the parameters set by previous SQLBindParam() calls on this statement handle are released.
 *    Parameters: const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        reset() function takes no arguments.
 */
void DbStmt::Reset(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  DEBUG(this, "SQLFreeStmt: stmth %d [SQL_RESET_PARAMS]\n", this->stmth)
  if (this->stmtAllocated)
  {
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
 *    Return: boolean true if successful otherwise throws an error.
 */
Napi::Value DbStmt::Close(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  this->freeColumns();
  DEBUG(this, "SQLFreeStmt: stmth %d [SQL_DROP]\n", this->stmth)
  if (this->stmtAllocated)
  {
    //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnfstmt.htm
    //Free the statement handle here. No further processing allowed.
    SQLRETURN sqlReturnCode = SQLFreeStmt(this->stmth, //SQLHSTMT hstmt -Statement Handle
                                          SQL_DROP);   //SQLSMALLINT fOption -Option the manner of freeing the stmt
    DEBUG(this, "SQLFreeStmt(%d)\n", sqlReturnCode)
    CHECK_WITH_RETURN(sqlReturnCode != SQL_SUCCESS, SQL_ERROR, "SQLFreeStmt failed.", env, env.Null())
    this->stmtAllocated = false; // Any SQL Statement Handler processing can not be allowed after this.
    this->resultSetAvailable = false;
  }
  return Napi::Boolean::New(env, 1);
}

/*
 *  DbStmt::NumFields
 *    Description: Returns the number of columns contained in a result set.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        numFields() function takes no arguments.
 *    Return: Number value indicating number of columns in the result set.
 */
Napi::Value DbStmt::NumFields(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN((this->stmtAllocated == false), STMT_NOT_READY, "The SQL Statement handler is not initialized.", env, env.Null());
  CHECK_WITH_RETURN((this->resultSetAvailable == false), RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", env, env.Null());
  //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnnrcol.htm
  SQLRETURN sqlReturnCode = SQLNumResultCols(this->stmth, //SQLHSTMT hstmt -Statement Handle
                                             &colCount);  //SQLSMALLINT* pccol -Number of columns in result (OUTPUT)
  if (sqlReturnCode != SQL_SUCCESS)
  {
    throwCustomMsg(SQL_ERROR, "SQLNumResultCols() failed.", env);
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
Napi::Value DbStmt::NumRows(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  CHECK_WITH_RETURN((this->stmtAllocated == false), STMT_NOT_READY, "The SQL Statement handler is not initialized.", env, env.Null());

  SQLINTEGER rowCount;
  //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnrowc.htm
  if (SQLRowCount(this->stmth, &rowCount) != SQL_SUCCESS)
  {
    throwCustomMsg(SQL_ERROR, "SQLRowCount() failed.", env);
    return env.Null();
  }
  return Napi::Number::New(env, rowCount);
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
Napi::Value DbStmt::FieldType(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN((info.Length() != 1), INVALID_PARAM_NUM, "The fieldType() method only accepts one parameter.", env, env.Null());
  CHECK_WITH_RETURN((!info[0].IsNumber()), INVALID_PARAM_TYPE, "Expected the first parameter to be a Number.", env, env.Null());
  CHECK_WITH_RETURN((this->resultSetAvailable == false || this->colDescAllocated == false), RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", env, env.Null());

  SQLSMALLINT i = Napi::Number(env, info[0]).Int32Value();
  CHECK_WITH_RETURN((i >= this->colCount || i < 0), INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", env, env.Null());

  return Napi::Number::New(env, this->dbColumn[i].sqlType);
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
Napi::Value DbStmt::FieldWidth(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN((info.Length() != 1), INVALID_PARAM_NUM, "The fieldWidth() method only accepts one parameter.", env, env.Null());
  CHECK_WITH_RETURN((!info[0].IsNumber()), INVALID_PARAM_TYPE, "Expected the first parameter to be a Number.", env, env.Null());
  CHECK_WITH_RETURN((this->resultSetAvailable == false || this->colDescAllocated == false), RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", env, env.Null());

  SQLSMALLINT i = Napi::Number(env, info[0]).Int32Value();
  CHECK_WITH_RETURN((i >= this->colCount || i < 0), INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", env, env.Null());

  return Napi::Number::New(env, this->dbColumn[i].nameLength);
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
Napi::Value DbStmt::FieldName(const Napi::CallbackInfo &info)
{

  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN((info.Length() != 1), INVALID_PARAM_NUM, "The fieldName() method only accepts one parameter.", env, env.Null());
  CHECK_WITH_RETURN((!info[0].IsNumber()), INVALID_PARAM_TYPE, "Expected the first parameter to be a Number.", env, env.Null());
  CHECK_WITH_RETURN((this->resultSetAvailable == false || this->colDescAllocated == false), RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", env, env.Null());

  SQLSMALLINT i = Napi::Number(env, info[0]).Int32Value();
  CHECK_WITH_RETURN((i >= this->colCount || i < 0), INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", env, env.Null());

  return Napi::String::New(env, this->dbColumn[i].name);
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
Napi::Value DbStmt::FieldPrecise(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN((info.Length() != 1), INVALID_PARAM_NUM, "The fieldPrecise() method only accepts one parameter.", env, env.Null());
  CHECK_WITH_RETURN((!info[0].IsNumber()), INVALID_PARAM_TYPE, "Expected the first parameter to be a Number.", env, env.Null());
  CHECK_WITH_RETURN((this->resultSetAvailable == false || this->colDescAllocated == false), RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", env, env.Null());

  SQLSMALLINT i = Napi::Number(env, info[0]).Int32Value();
  CHECK_WITH_RETURN((i >= this->colCount || i < 0), INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", env, env.Null());

  return Napi::Number::New(env, this->dbColumn[i].colPrecise);
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
Napi::Value DbStmt::FieldScale(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

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
Napi::Value DbStmt::FieldNullable(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN((info.Length() != 1), INVALID_PARAM_NUM, "The fieldNullable() method only accepts one parameter.", env, env.Null());
  CHECK_WITH_RETURN((!info[0].IsNumber()), INVALID_PARAM_TYPE, "Expected the first parameter to be a Number.", env, env.Null());
  CHECK_WITH_RETURN((this->resultSetAvailable == false || this->colDescAllocated == false), RSSET_NOT_READY, "The Result set is unavailable. Try querying something first.", env, env.Null());

  SQLSMALLINT i = info[0].ToNumber().Int32Value();
  CHECK_WITH_RETURN((i >= this->colCount || i < 0), INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", env, env.Null());

  return Napi::Boolean::New(env, this->dbColumn[i].colNull == SQL_NULLABLE);
}

/*
 *  DbStmt::FieldInfo
 *    Description:
 *      Returns all the informaton of the indicated column.
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the
 *        fieldInfo() function takes one argument, stored on the info
 *        object.
 *          info[0]: Number : the column number in a result set, ordered
 *                            sequentially left to right, starting at 0.
 *    Return: Object containing the information of the column.
 */
Napi::Value DbStmt::FieldInfo(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  CHECK_WITH_RETURN((info.Length() != 1), INVALID_PARAM_NUM, "The fieldNullable() method only accepts one parameter.", env, env.Null());
  CHECK_WITH_RETURN((!info[0].IsNumber()), INVALID_PARAM_TYPE, "Expected the first parameter to be a Number.", env, env.Null());
  CHECK_WITH_RETURN((this->resultSetAvailable == false || this->colDescAllocated == false), RSSET_NOT_READY, "The Result set is unavailable. Try querying something first.", env, env.Null());

  SQLSMALLINT i = info[0].ToNumber().Int32Value();
  CHECK_WITH_RETURN((i >= this->colCount || i < 0), INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", env, env.Null());

  Napi::Object fieldInfo = Napi::Object::New(env);
  fieldInfo.Set(Napi::String::New(env, "Name"), Napi::String::New(env, dbColumn[i].name));
  fieldInfo.Set(Napi::String::New(env, "Type"), Napi::Number::New(env, dbColumn[i].sqlType));
  fieldInfo.Set(Napi::String::New(env, "TypeName"), Napi::String::New(env, getSQLType(dbColumn[i].sqlType)));
  fieldInfo.Set(Napi::String::New(env, "Width"), Napi::Number::New(env, dbColumn[i].nameLength));
  fieldInfo.Set(Napi::String::New(env, "Precise"), Napi::Number::New(env, dbColumn[i].colPrecise));
  fieldInfo.Set(Napi::String::New(env, "Scale"), Napi::Number::New(env, dbColumn[i].colScale));
  fieldInfo.Set(Napi::String::New(env, "Nullable"), Napi::Boolean::New(env, dbColumn[i].colNull == SQL_NULLABLE));
  return fieldInfo;
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
 *                                arg1: the error that was returned from the call to SQLGetDiagRec
 */
void DbStmt::StmtError(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

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

  switch (hType)
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

  sqlError errObj;
  char errorMessage[SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10];

  SQLINTEGER sqlReturnCode = SQL_SUCCESS;
  SQLSMALLINT length = 0;

  sqlReturnCode = SQLGetDiagRec(hType,                      //SQLSMALLINT gtype -Handle Type
                                handle,                     //SQLINTEGER handle -hadnle for info is wanted
                                recno,                      //SQLSMALLINT recNUM -Indicates which error record to return (if multiple)
                                errObj.sqlState,            //SQLCHAR* szSQLSTATE -SQLSTATE as a string of 5 characters terminated by a null character. (Output)
                                &errObj.sqlCode,            //SQLINTEGER* pfNativeError -Error Code (Output)
                                errObj.message,             //SQLCHAR* szErrorMsg -Pointer to buffer msg text (Output)
                                SQL_MAX_MESSAGE_LENGTH + 1, //SQLSMALLINT cbErorMsgMax -Max length of the buffer szErrorMsg
                                &length);                   //SQLSMALLINT* pcbErrorMsg -Pointer total # bytes to return to szErrorMsg (Output)

  // handle all possible return codes
  switch (sqlReturnCode)
  {
  case SQL_SUCCESS:
    if (errObj.message[length - 1] == '\n')
      errObj.message[length - 1] = '\0';
    break;
  case SQL_SUCCESS_WITH_INFO: // TODO
  case SQL_ERROR:             // TODO
  case SQL_INVALID_HANDLE:    // TODO
    break;
  }

  sprintf(errorMessage, "SQLSTATE=%s SQLCODE=%d : %s", errObj.sqlState, (int)errObj.sqlCode, errObj.message);
  callbackArguments.push_back(Napi::String::New(env, errorMessage));
  callback.Call(callbackArguments);
}

/****** UNEXPOSED HELPER FUNCTIONS ******/

int DbStmt::populateColumnDescriptions(Napi::Env env)
{
  if (colDescAllocated == true)
  {
    freeColumnDescriptions();
  }

  SQLRETURN sqlReturnCode;
  dbColumn = (db2ColumnDescription *)calloc(colCount, sizeof(db2ColumnDescription));

  for (int col = 0; col < colCount; col++)
  {
    dbColumn[col].name = (SQLCHAR *)calloc(MAX_COLNAME_WIDTH, sizeof(SQLCHAR));
    //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfndecol.htm
    sqlReturnCode = SQLDescribeCol(stmth,                     //SQLHSTMT hstmt -Statement handle
                                   col + 1,                   //SQLSMALLINT icol -Column # to be described
                                   dbColumn[col].name,        //SQLCHAR* szColName -Pointer to column name buffer
                                   MAX_COLNAME_WIDTH,         //SQLSMALLINT cbColNameMax -Size of szColName buffer
                                   &dbColumn[col].nameLength, //SQLSMALLINT* pcbColName -Bytes avail to return for szColName (Output)
                                   &dbColumn[col].sqlType,    //SQLSMALLINT* pfSqlType -SQL data type of column (Output)
                                   &dbColumn[col].colPrecise, //SQLINTEGER* pcbColDef -Precision of column as defined in db2 (Output)
                                   &dbColumn[col].colScale,   //SQLSMALLINT* pibScale -Scale of column as defined in db2 (Output)
                                   &dbColumn[col].colNull);   //SQLSMALLINT* pfNullable -Indactes whether null is allowed (Output)

    DEBUG(this, "DescCol(%d)\tindex[%d]\tType[%s]\tScale[%d]\tPrecise[%d]\n",
          sqlReturnCode, col, getSQLType(dbColumn[col].sqlType), dbColumn[col].colScale, dbColumn[col].colPrecise)

    if (sqlReturnCode != SQL_SUCCESS)
    {
      freeColumnDescriptions();
      if (env != NULL)
        throwCustomMsg(SQL_ERROR, "SQLDescribeCol() failed.", env);
      else
        printErrorToLog(stmth, msg);
      return -1;
    }
  }
  colDescAllocated = true;
  return 0;
}

void DbStmt::freeColumnDescriptions()
{
  for (int col = 0; col < colCount && dbColumn[col].name; col++)
    free(dbColumn[col].name);
  free(dbColumn);
  colDescAllocated = false;
}

int DbStmt::bindColData(Napi::Env env)
{
  if (bindingRowAllocated == true)
    freeBindingRow();

  if (colDescAllocated != true)
    if (populateColumnDescriptions(env) < 0)
      return -1;

  SQLRETURN sqlReturnCode;
  SQLINTEGER maxColLen = 0;
  /* 'bindingRowInC' is defined in dbstmt.h as type 'SQLCHAR**' */
  bindingRowInC = (SQLCHAR **)calloc(colCount, sizeof(SQLCHAR *));
  for (int col = 0; col < colCount; col++)
  {
    switch (dbColumn[col].sqlType)
    {
    case SQL_SMALLINT:  // -32768 to +32767
    {
      maxColLen = 7;
      bindingRowInC[col] = (SQLCHAR *)calloc(maxColLen, sizeof(SQLCHAR));
      //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnbindc.htm
      sqlReturnCode = SQLBindCol(stmth,                          //SQLHSTMT hstmt -Statement Handle
                                 col + 1,                        //SQLSMALLINT icol -# identifying the column
                                 SQL_C_CHAR,                     //SQLSMALLINT fCtype -App Data type for icol
                                 (SQLPOINTER)bindingRowInC[col], //SQLPOINTER rgbValue -Pointer to buffer to store col data (Output)
                                 maxColLen,                      //SQLINTEGER cbValueMax -Size of rgbValue buffer in bytes avail to store col data
                                 &dbColumn[col].rlength);        //SQLINTEGER* pcbValue -Pointer to value for # bytes avail to return in rgbValue buffer (Output)
    }
    break;
    case SQL_INTEGER:  // -2147483648 to +2147483647
    {
      maxColLen = 13;
      bindingRowInC[col] = (SQLCHAR *)calloc(maxColLen, sizeof(SQLCHAR));
      sqlReturnCode = SQLBindCol(stmth, col + 1, SQL_C_CHAR, (SQLPOINTER)bindingRowInC[col], maxColLen, &dbColumn[col].rlength);
    }
    break;
    case SQL_BIGINT:  // -9223372036854775808 to +9223372036854775807
    {
      maxColLen = 21;
      bindingRowInC[col] = (SQLCHAR *)calloc(maxColLen, sizeof(SQLCHAR));
      sqlReturnCode = SQLBindCol(stmth, col + 1, SQL_C_CHAR, (SQLPOINTER)bindingRowInC[col], maxColLen, &dbColumn[col].rlength);
    }
    break;
    case SQL_DECIMAL:  // -(10^n) + 1 to +(10^n) - 1
    case SQL_NUMERIC:
    {
      maxColLen = dbColumn[col].colPrecise + 4;
      bindingRowInC[col] = (SQLCHAR *)calloc(maxColLen, sizeof(SQLCHAR));
      sqlReturnCode = SQLBindCol(stmth, col + 1, SQL_C_CHAR, (SQLPOINTER)bindingRowInC[col], maxColLen, &dbColumn[col].rlength);
    }
    break;
    case SQL_REAL:     // FLOAT(24) Precision = 7
    case SQL_FLOAT:    // FLOAT(24) Precision = 7 or FLOAT(53) Precision = 15
    case SQL_DOUBLE:   // Precision = 31
    case SQL_DECFLOAT: // Precision = 34
    {
      maxColLen = 50;
      bindingRowInC[col] = (SQLCHAR *)calloc(maxColLen, sizeof(SQLCHAR));
      sqlReturnCode = SQLBindCol(stmth, col + 1, SQL_C_CHAR, (SQLPOINTER)bindingRowInC[col], maxColLen, &dbColumn[col].rlength);
    }
    break;
    case SQL_VARBINARY:
    case SQL_BINARY:
    {
      maxColLen = dbColumn[col].colPrecise;
      bindingRowInC[col] = (SQLCHAR *)malloc(maxColLen * sizeof(SQLCHAR));
      sqlReturnCode = SQLBindCol(stmth, col + 1, SQL_C_BINARY, (SQLPOINTER)bindingRowInC[col], maxColLen, &dbColumn[col].rlength);
    }
    break;
    case SQL_BLOB:
    {
      maxColLen = dbColumn[col].colPrecise;
      bindingRowInC[col] = (SQLCHAR *)malloc(maxColLen * sizeof(SQLCHAR));
      sqlReturnCode = SQLBindCol(stmth, col + 1, SQL_C_BLOB, (SQLPOINTER)bindingRowInC[col], maxColLen, &dbColumn[col].rlength);
    }
    break;
    case SQL_CLOB:
    {
      sqlReturnCode = SQLBindCol(stmth, col + 1, SQL_C_CLOB_LOCATOR, &dbColumn[col].clobLoc, 0, &dbColumn[col].rlength);
    }
    break;
    case SQL_WCHAR:
    case SQL_WVARCHAR:
    {
      maxColLen = dbColumn[col].colPrecise * 4 + 1;
      bindingRowInC[col] = (SQLCHAR *)calloc(maxColLen, sizeof(SQLCHAR));
      sqlReturnCode = SQLBindCol(stmth, col + 1, SQL_C_CHAR, (SQLPOINTER)bindingRowInC[col], maxColLen, &dbColumn[col].rlength);
    }
    break;
    default: // SQL_CHAR / SQL_VARCHAR
    {
      maxColLen = dbColumn[col].colPrecise * 4 + 1;
      bindingRowInC[col] = (SQLCHAR *)calloc(maxColLen, sizeof(SQLCHAR));
      sqlReturnCode = SQLBindCol(stmth, col + 1, SQL_C_CHAR, (SQLPOINTER)bindingRowInC[col], maxColLen, &dbColumn[col].rlength);
    }
    break;
    }
    if (sqlReturnCode != SQL_SUCCESS)
    {
      freeBindingRow();
      if (env != NULL)
        throwCustomMsg(SQL_ERROR, "SQLBindCol() failed.", env);
      else
        printErrorToLog(stmth, msg);
      return -1;
    }
  }
  bindingRowAllocated = true;
  return 0;
}

int DbStmt::fetchData()
{
  SQLRETURN sqlReturnCode = SQL_SUCCESS;
  // Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnfetch.htm
  while (true)
  {
    SQLRETURN returnCode = SQLFetch(stmth);

    if (returnCode != SQL_SUCCESS) // may be warning messages
      sqlReturnCode = returnCode;

    if (returnCode != SQL_SUCCESS && returnCode != SQL_SUCCESS_WITH_INFO) // error occurred!
      break;

    resultSetItem *rowOfResultSetInC = (resultSetItem *)calloc(colCount, sizeof(resultSetItem));
    for (int col = 0; col < colCount; col++)
    {
      int colLen = 0;
      int ind = 0;
      if (dbColumn[col].sqlType == SQL_CLOB)
      { // TODO: check return code
        SQLHANDLE stmtLoc;

        returnCode = SQLAllocStmt(connh, &stmtLoc);

        returnCode = SQLGetLength(stmtLoc, SQL_C_CLOB_LOCATOR, dbColumn[col].clobLoc, &colLen, &ind);

        rowOfResultSetInC[col].data = (SQLCHAR *)calloc(colLen + 1, sizeof(SQLCHAR));
        rowOfResultSetInC[col].rlength = colLen;
        returnCode = SQLGetCol(stmth, col + 1, SQL_C_CLOB, rowOfResultSetInC[col].data, colLen, &ind);

        returnCode = SQLFreeStmt(stmtLoc, SQL_DROP);

        // TODO: This could be done more efficiently since SQL_ATTR_FREE_LOCATOR
        //       accepts an array of locators to free.
        int rc2 = SQLSetConnectAttr(connh, SQL_ATTR_FREE_LOCATORS, &dbColumn[col].clobLoc, 1);
        if(rc2 != SQL_SUCCESS)
        {
          DEBUG(this, "SQLSetConnectAttr(%d, %d, %d, 1) returned %d\n", connh,
                SQL_ATTR_FREE_LOCATORS, dbColumn[col].clobLoc);
        }
      }
      else if (dbColumn[col].rlength == SQL_NTS)
      { // SQL_NTS = -3
        colLen = strlen(bindingRowInC[col]);
        rowOfResultSetInC[col].data = (SQLCHAR *)calloc(colLen + 1, sizeof(SQLCHAR));
        memcpy(rowOfResultSetInC[col].data, bindingRowInC[col], colLen * sizeof(SQLCHAR));
        rowOfResultSetInC[col].rlength = SQL_NTS;
      }
      else if (dbColumn[col].rlength == SQL_NULL_DATA)
      { // SQL_NULL_DATA = -1
        rowOfResultSetInC[col].data = NULL;
        rowOfResultSetInC[col].rlength = SQL_NULL_DATA;
      }
      else if (dbColumn[col].rlength < 0)
      { // inconsistent data
        rowOfResultSetInC[col].data = strdup("-");
        rowOfResultSetInC[col].rlength = SQL_NTS;
      }
      else
      {
        colLen = dbColumn[col].rlength;
        rowOfResultSetInC[col].data = (SQLCHAR *)calloc(colLen, sizeof(SQLCHAR));
        memcpy(rowOfResultSetInC[col].data, bindingRowInC[col], colLen * sizeof(SQLCHAR));
        rowOfResultSetInC[col].rlength = colLen;
      }
    }
    resultSetInC.push_back(rowOfResultSetInC);
  }
  return sqlReturnCode;
}

int DbStmt::buildJsObject(Napi::Env env, Napi::Array *array)
{
  for (std::size_t row = 0; row < resultSetInC.size(); row++)
  {
    Napi::Object rowInJs = Napi::Object::New(env);
    for (int col = 0; col < colCount; col++)
    {
      Napi::Value value;
      Napi::Value nvalue;
      if (resultSetInC[row][col].rlength == SQL_NULL_DATA)
        value = env.Null();
      else
      {
        switch (dbColumn[col].sqlType)
        {
        case SQL_VARBINARY:
        case SQL_BINARY:
        case SQL_BLOB:
        {
          SQLCHAR *binaryData = new SQLCHAR[resultSetInC[row][col].rlength]; // have to save the data on the heap
          memcpy((SQLCHAR *)binaryData, resultSetInC[row][col].data, resultSetInC[row][col].rlength);
          value = Napi::Buffer<SQLCHAR>::New(env, binaryData, resultSetInC[row][col].rlength, [](Napi::Env env, void *finalizeData) {
            delete[](SQLCHAR *) finalizeData;
          });
          break;
        }
        case SQL_SMALLINT: // -32768 to +32767
        case SQL_INTEGER:  // -2147483648 to +2147483647
        // case SQL_BIGINT:   // -9223372036854775808 to +9223372036854775807
        case SQL_REAL:     // FLOAT(24) Precision = 7
        case SQL_FLOAT:    // FLOAT(24) Precision = 7 / FLOAT(53) Precision = 15
        case SQL_DOUBLE:   // Precision = 31
        // case SQL_DECFLOAT: // Precision = 34
          if (asNumber == true)
          {
            nvalue = Napi::String::New(env, resultSetInC[row][col].data);
            value = Napi::Number::New(env, nvalue.ToNumber());
            break;
          }
        case SQL_DECIMAL:  // -(10^n) + 1 to +(10^n) - 1
        case SQL_NUMERIC:
          if (asNumber == true && dbColumn[col].colPrecise <= 15)
          {
            nvalue = Napi::String::New(env, resultSetInC[row][col].data);
            value = Napi::Number::New(env, nvalue.ToNumber());
            break;
          }
        default:
          if (resultSetInC[row][col].rlength > 0)
            value = Napi::String::New(env, resultSetInC[row][col].data, resultSetInC[row][col].rlength);
          else
            value = Napi::String::New(env, resultSetInC[row][col].data);
          break;
        }
      }
      rowInJs.Set(Napi::String::New(env, (char const *)dbColumn[col].name), value); //Build a JS row
      free(resultSetInC[row][col].data);                                            //Free current column field of the row
    }
    array->Set(row, rowInJs); //Build the JS object of the result set
    free(resultSetInC[row]);  //Free current row of the C array of the result set
  }
  resultSetInC.clear(); //Free the C array of the result set
  return 0;
}

void DbStmt::freeColumns()
{
  if (colDescAllocated == true)
    freeColumnDescriptions();
  if (bindingRowAllocated == true)
    freeBindingRow();
}

void DbStmt::freeBindingRow()
{
  for (int col = 0; col < colCount && bindingRowInC[col]; col++)
    free(bindingRowInC[col]);
  free(bindingRowInC);
  bindingRowAllocated = false;
}

void DbStmt::freeSp()
{
  if (param)
  {
    for (int i = 0; i < paramCount; i++)
      free(param[i].buf);
    free(param);
    param = NULL;
  }
}

int DbStmt::bindParams(Napi::Env env, Napi::Array *params, std::string &error)
{
  Napi::Array object;
  int bindIndicator, io;
  SQLRETURN sqlReturnCode;
  Napi::Value value, ioValue, bindValue;
  freeSp();

  paramCount = params->Length();
  param = (db2ParameterDescription *)calloc(paramCount, sizeof(db2ParameterDescription));

  for (SQLSMALLINT i = 0; i < paramCount; i++)
  {
    //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfndescprm.htm
    sqlReturnCode = SQLDescribeParam(
        stmth,               //SQLHSTMT StatementHandle
        i + 1,               //SQLSMALLINT ParameterNumber
        &param[i].paramType, //SQLSMALLINT* DataTypePtr -Pointer to buffer to return data type (Output)
        &param[i].paramSize, //SQLINTEGER*  ParameterSizePtr -Pointer to buffer to return size (Output)
        &param[i].decDigits, //SQLSMALLINT* DecimalDigitsPtr -Pointer to buffer to return # dec digits (Output)
        &param[i].nullable); //SQLSMALLINT* NullablePtr -Pointer to buffer to return if nullable (Output)

    if (sqlReturnCode != SQL_SUCCESS)
    {
      DEBUG(this, "SQLDescribeParam(%d)\n", sqlReturnCode)
      error = "SQLDescribeParm FAILED\n" + returnErrMsg(SQL_HANDLE_STMT, stmth);
      return -1;
    }

    if(params->Get(i).IsArray()) // For old BindParams APIs, parameters are 2-D array.
    {
      object = params->Get(i).As<Napi::Array>(); //Get a ? parameter from the array.
      value = object.Get((uint32_t)0);  // have to cast otherwise it complains about ambiguity
      ioValue = object.Get(1);
      bindValue = object.Get(2);

      //validate io
      if (!ioValue.IsNumber())
      {
        error = "IO TYPE OF PARAMETER " + std::to_string(i + 1) + " IS INVALID";
        return -1;
      }

      io = ioValue.ToNumber().Int32Value(); //convert from Napi::Value to an int

      if (io != SQL_PARAM_INPUT && io != SQL_PARAM_OUTPUT && io != SQL_PARAM_INPUT_OUTPUT)
      {
        error = "IO TYPE OF PARAMETER " + std::to_string(i + 1) + " IS INVALID. SHOULD EQUAL PARAM INPUT, OUTPUT, OR INPUT_OUTPUT";
        return -1;
      }

      //validate bindIndicator
      if (!bindValue.IsNumber())
      {
        error = "BIND INDICATOR TYPE OF PARAMETER " + std::to_string(i + 1) + " IS INVALID\n";
        return -1;
      }
      
      param[i].io = io;
      bindIndicator = bindValue.ToNumber().Int32Value(); //convert from Napi::Value to an int

      //validate the value is not undefined
      if (value.IsUndefined())
      { // if value is undefined convert it to null
        error = "VALUE OF PARAMETER " + std::to_string(i + 1) + " IS UNDEFINED\n";
        return -1;
      }

      if (bindIndicator == 0 || bindIndicator == 1)
      { //Parameter is string or clob
        std::string string = value.ToString().Utf8Value();
        size_t str_length = string.length();
        const char *cString = string.c_str();
        param[i].valueType = SQL_C_CHAR;

        if (param[i].io == SQL_PARAM_INPUT || param[i].io == SQL_PARAM_INPUT_OUTPUT)
        {
          param[i].buf = strdup(cString);
          param[i].ind = str_length;
        }
        else if (param[i].io == SQL_PARAM_OUTPUT)
        {
          param[i].buf = (char *)calloc(param[i].paramSize + 1, sizeof(char));
          param[i].ind = param[i].paramSize;
        }
      }
      else if (bindIndicator == 2)
      { //Parameter is Integer
        int64_t *number = (int64_t *)malloc(sizeof(int64_t));
        *number = value.ToNumber().Int32Value();
        param[i].valueType = SQL_C_BIGINT;
        param[i].buf = number;
        param[i].ind = 0;
      }
      else if (bindIndicator == 3 || value.IsNull())
      { //Parameter is NULL
        param[i].valueType = SQL_C_DEFAULT;
        param[i].ind = SQL_NULL_DATA;
        void *dummy = nullptr;
        param[i].buf = &dummy;
      }
      else if (bindIndicator == 4 || value.IsNumber())
      { //Parameter is Decimal
        double *number = (double *)malloc(sizeof(double));
        *number = value.ToNumber();
        param[i].valueType = SQL_C_DOUBLE;
        param[i].buf = number;
        param[i].ind = sizeof(double);
        param[i].decDigits = 7;
        param[i].paramSize = sizeof(double);
      }
      else if (bindIndicator == 5 || value.IsBoolean())
      { //Parameter is Boolean
        bool *boolean = (bool *)malloc(sizeof(bool));
        *boolean = value.ToBoolean();
        param[i].valueType = SQL_C_BIT;
        param[i].buf = boolean;
        param[i].ind = 0;
      }
      else if (bindIndicator == SQL_BINARY || bindIndicator == SQL_BLOB || value.IsBuffer())
      { //Parameter is blob/binary
        //convert into Napi::Buffer
        Napi::Buffer<char> buffer = value.As<Napi::Buffer<char>>();
        int bufferLength = buffer.Length();
        param[i].valueType = SQL_C_BINARY;
        //get a pointer to the buffer
        char *bufferPtr = buffer.Data();
        param[i].buf = bufferPtr;
        param[i].ind = bufferLength;
      }
      else
      { //bindIndicator did not match any cases
        error = "BIND INDICATOR FOR PARAMETER " + std::to_string(i + 1) + "IS INVALID\n";
        return -1;
      }
    }
    else // params->Get(i) is not array. For new BindParameters APIs, parameters are 1-D array.
    {
      value = params->Get(i).As<Napi::Value>();
      param[i].io = SQL_PARAM_INPUT_OUTPUT;

      // Handle null and boolean individually.
      if (value.IsNull())
      { //Parameter is NULL
        param[i].valueType = SQL_C_DEFAULT;
        param[i].ind = SQL_NULL_DATA;
        void *dummy = nullptr;
        param[i].buf = &dummy;
      }
      else if (value.IsBoolean())
      { //Parameter is Boolean
        bool *boolean = (bool *)malloc(sizeof(bool));
        *boolean = value.ToBoolean();
        param[i].valueType = SQL_C_BIT;
        param[i].buf = boolean;
        param[i].ind = 0;
      }
      else switch(param[i].paramType)
      {
      case SQL_SMALLINT:
      case SQL_INTEGER:
      case SQL_BIGINT:
      {
        int64_t *number = (int64_t *)malloc(sizeof(int64_t));
        if(value.IsNumber())  // input param
          *number = value.ToNumber().Int32Value();
        param[i].valueType = SQL_C_BIGINT;
        param[i].buf = number;
        param[i].ind = 0;
      }
      break;
      case SQL_DECIMAL:
      case SQL_NUMERIC:
      case SQL_REAL:
      case SQL_FLOAT:
      case SQL_DOUBLE:
      case SQL_DECFLOAT:
      {
        double *number = (double *)malloc(sizeof(double));
        if(value.IsNumber())  // input param
          *number = value.ToNumber();
        param[i].valueType = SQL_C_DOUBLE;
        param[i].buf = number;
        param[i].ind = sizeof(double);
        param[i].decDigits = 7;
        param[i].paramSize = sizeof(double);
      }
      break;
      case SQL_VARBINARY:
      case SQL_BINARY:
      case SQL_BLOB:
      {
        param[i].valueType = SQL_C_BINARY;
        if(value.IsBuffer())
        {
          Napi::Buffer<char> buffer = value.As<Napi::Buffer<char>>();
          int bufferLength = buffer.Length();
          if(bufferLength > 0)
          {
            //get a pointer to the buffer
            char *bufferPtr = buffer.Data();
            param[i].buf = bufferPtr;
            param[i].ind = bufferLength;
            break;
          }
        }
        if(param[i].paramSize > 0) // bufferLength == 0 or value is ''
        {
          param[i].buf = (char *)malloc(param[i].paramSize);;
          param[i].ind = param[i].paramSize;
        }
      }
      break;
      case SQL_CLOB:
      {
        param[i].valueType = SQL_C_CHAR;
        param[i].buf = (char *)calloc(param[i].paramSize + 1, sizeof(char));
        if(value.IsString())
        {
          std::string string = value.ToString().Utf8Value();
          const char *cString = string.c_str();
          if(strlen(cString) > 0) 
          {
            strcpy((char *)param[i].buf, cString);
            param[i].ind = strlen(cString);
            break;
          }
        }
        // value is '' -> output param
        param[i].ind = param[i].paramSize;
      }
      break;
      default: // SQL_CHAR / SQL_VARCHAR / SQL_WCHAR / SQL_WVARCHAR
      {
        param[i].valueType = SQL_C_CHAR;

        if(value.IsString())
        {
          std::string string = value.ToString().Utf8Value();
          size_t str_length = string.length();
          const char *cString = string.c_str();
          
          if (cString[0] == '\0') // Check for JS empty-string.
           {
            param[i].ind = SQL_NTS;
          } else {
            param[i].ind = str_length;
          }
          param[i].buf = strdup(cString);
        }
        else
        {
          param[i].ind = SQL_NTS;
          param[i].buf = (char *)calloc(param[i].paramSize + 1, sizeof(char));
        }
      }
      break;
      }
    }
    
    //link to doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnbndpm.htm
    sqlReturnCode = SQLBindParameter(
        stmth,              //SQLHSTMT statement handle
        i + 1,              //SQLSMALLINT Parameter Number
        param[i].io,        //SQLSMALLINT InputOutputType
        param[i].valueType, //SQLSMALLINT Value Type -C data type of Parameter
        param[i].paramType, //SQLSMALLINT Parameter Type -SQL data type of the parameter
        param[i].paramSize, //SQLINTEGER Parameter Size -Precision of the param
        param[i].decDigits, //SQLDecimal Digits -Scale of the param
        param[i].buf,       //ParameterValuePtr -Points to buffer that contains actual data for param. OutParams are placed here.
        0,                  //SQLLEN BufferLength (Not Used in CLI)
        &param[i].ind);     //SQLLEN* StrLen_or_IndPtr -length of the parameter marker value stored at ParameterValuePtr.

    DEBUG(this, "SQLBindParameter(%d) TYPE[%2d] SIZE[%3d] DIGI[%d] IO[%d] IND[%3d] INDEX[%i]\n", sqlReturnCode, param[i].paramType, param[i].paramSize, param[i].decDigits, param[i].io, param[i].ind, i)

    if (sqlReturnCode != SQL_SUCCESS)
    {
      error = "SQLBindParmeter FAILED\n" + returnErrMsg(SQL_HANDLE_STMT, stmth);
      return -1;
    }
  }
  return 0;
}

int DbStmt::fetchSp(Napi::Env env, Napi::Array *array)
{
  for (int i = 0, j = 0; i < paramCount; i++)
  {
    db2ParameterDescription *p = &param[i];
    if (p->io != SQL_PARAM_INPUT)
    {
      if (p->valueType == SQL_C_BIGINT) // Integer
        array->Set(j, Napi::Number::New(env, *(int64_t *)p->buf).Int32Value());
      else if (p->valueType == SQL_C_DOUBLE) // Decimal
        array->Set(j, Napi::Number::New(env, *(double *)p->buf));
      else if (p->valueType == SQL_C_BIT) // Boolean
        array->Set(j, Napi::Boolean::New(env, *(bool *)p->buf));
      else
        array->Set(j, Napi::String::New(env, (char *)p->buf));
      j++;
    }
  }
  freeSp();
  return 0;
}

int DbStmt::fetch(Napi::Env env, Napi::Object *row)
{
  for (int col = 0; col < colCount; col++)
  {
    Napi::Value value;
    if (dbColumn[col].rlength == SQL_NULL_DATA)
    { // SQL_NULL_DATA = -1
      value = env.Null();
    }
    else if (dbColumn[col].rlength < 0 && dbColumn[col].rlength != SQL_NTS)
    { // inconsistent data
      value = Napi::String::New(env, "-");
    }
    else
    {
      switch (dbColumn[col].sqlType)
      {
      case SQL_VARBINARY:
      case SQL_BINARY: //Buffers are returned for Binary, Varbinary, Blob types.
      case SQL_BLOB:
        value = Napi::Buffer<char>::New(env, bindingRowInC[col], dbColumn[col].rlength);
        break;
      default:
        value = Napi::String::New(env, bindingRowInC[col]);
        break;
      }
    }
    row->Set(Napi::String::New(env, (char const *)dbColumn[col].name), value);
  }
  return 0;
}
