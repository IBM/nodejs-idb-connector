/*  The Source code for this program is not published or otherwise  
 *  divested of its trade secrets, irrespective of what has been
 *  deposited with the U.S. Copyright Office.                        
*/

#include "dbconn.h"

SQLHENV DbConn::envh;
Napi::FunctionReference DbConn::constructor;

Napi::Object DbConn::Init(Napi::Env env, Napi::Object exports, SQLHENV envh2)
{
  Napi::HandleScope scope(env);

  Napi::Function constructorfunc = DefineClass(env, "dbconn", {
    InstanceMethod("setConnAttr", &DbConn::SetConnAttr),
    InstanceMethod("getConnAttr", &DbConn::GetConnAttr),
    InstanceMethod("conn", &DbConn::Conn),
    InstanceMethod("disconn", &DbConn::Disconnect),
    InstanceMethod("close", &DbConn::Close),
    InstanceMethod("validStmt", &DbConn::ValidStmt),
    InstanceMethod("debug", &DbConn::Debug),
    InstanceMethod("isConnected", &DbConn::IsConnected),
  });

  constructor = Napi::Persistent(constructorfunc);
  constructor.SuppressDestruct();

  envh = envh2;

  exports.Set("dbconn", constructorfunc);
  return exports;
}

DbConn::DbConn(const Napi::CallbackInfo &info) : Napi::ObjectWrap<DbConn>(info)
{
  SQLRETURN sqlReturnCode = -1;
  int param = SQL_TRUE;
  if (this->connAllocated == false)
  {
    // Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnacon.htm
    sqlReturnCode = SQLAllocConnect(envh,          // SQLHENV henv -Environment Handle
                                    &this->connh); //SQLHDBC* phdbc -Pointer to connection handle
    if (sqlReturnCode != SQL_SUCCESS)
    {
      DEBUG(this, "SQLAllocConnect(%d): SQL Connection Allocation Fail", sqlReturnCode);
      return;
    }
    this->connAllocated = true; // Any Connection Handler processing can not be allowed before this.
  }

  sqlReturnCode = SQLSetConnectAttr(this->connh, SQL_ATTR_AUTOCOMMIT, &param, 0); // Enable auto_commit by default.
}

Napi::Object DbConn::NewInstance(Napi::Value arg)
{
  Napi::Object dbConn = constructor.New({arg});
  return dbConn;
}

/*
 *  DbConn::SetConnAttr
 *    Description: Set an attribute of a specific connection handle.
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
Napi::Value DbConn::SetConnAttr(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  //validation
  CHECK_WITH_RETURN(length != 2, INVALID_PARAM_NUM, "Expected Two Parameters for SetConnAttr", env, env.Null())
  CHECK_WITH_RETURN(!info[0].IsNumber(), INVALID_PARAM_TYPE, "Expected Parameter 1 to be a Number", env, env.Null())
  CHECK_WITH_RETURN(!info[1].IsNumber() && !info[1].IsString(), INVALID_PARAM_TYPE, "Number or String Expected For Second Parameter of SetConnAttr", env, env.Null())
  CHECK_WITH_RETURN(this->connAllocated == false, CONN_NOT_READY, "Need to call conn() to allocate connection on DbConn first", env, env.Null())

  SQLINTEGER attr = Napi::Number(env, info[0]).Int32Value();
  char *cValue;
  SQLINTEGER sLen = 0;
  SQLRETURN sqlReturnCode = -1;
  //check if the second arg was a Number or a String
  if (info[1].IsNumber())
  {
    int param = Napi::Number(env, info[1]).Int32Value();
    // Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnsconx.htm
    sqlReturnCode = SQLSetConnectAttr(this->connh, //SQLHDBC hdbc -Connection Handle
                                      attr,        //SQLINTEGER fAttr -Connection Attr to Set
                                      &param,      //SQLPOINTER vParam -Value for fAttr
                                      0);          //SQLINTEGER sLen -Length of input value (if string)
    DEBUG(this, "SetConnAttr() attr = %d, value = %d, rc = %d\n", (int)attr, param, (int)sqlReturnCode);
  }
  else if (info[1].IsString())
  {
    std::string arg1 = Napi::String(env, info[1]).Utf8Value();
    std::vector<char> newCString(arg1.begin(), arg1.end());
    newCString.push_back('\0');
    cValue = &newCString[0];
    sLen = strlen(cValue);
    sqlReturnCode = SQLSetConnectAttr(this->connh, attr, cValue, sLen);
    DEBUG(this, "SetConnAttr() attr = %d, value = %s, return code = %d\n", (int)attr, cValue, (int)sqlReturnCode);
  }
  if (sqlReturnCode != SQL_SUCCESS)
  {
    throwErrMsg(SQL_HANDLE_DBC, connh, env);
    return env.Null();
  }
  return Napi::Boolean::New(env, 1);
}

/*
 *  DbConn::GetConnAttr
 *    Description: Returns the current settings for the specified connection option
 *    Parameters: 
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes one argument, stored on the info object:
 *          info[0] (Number): Attribute is the connection attribute to set.
 *    Return: The attribute option in the format of a Number or a String.
 * 
 */
Napi::Value DbConn::GetConnAttr(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  //validation
  CHECK_WITH_RETURN(length != 1, INVALID_PARAM_NUM, "Expected One Parameter for getConnAttr", env, env.Null())
  CHECK_WITH_RETURN(!info[0].IsNumber(), INVALID_PARAM_TYPE, "Expected Parameter 1 to be a Number", env, env.Null())
  CHECK_WITH_RETURN(this->connAllocated == false, CONN_NOT_READY, "Need to call conn() to allocate connection on first", env, env.Null())

  SQLINTEGER attr = Napi::Number(env, info[0]).Int32Value();

  char buf[1024];
  int retVal = 0;
  SQLINTEGER sLen = 0;
  void *pValue = (char *)&buf;
  //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfngcona.htm
  SQLRETURN sqlReturnCode = SQLGetConnectAttr(this->connh, //SQLHDBC hdbc -Connection handle
                                              attr,        //SQLINTEGER fAttr -Attribute to retrieve
                                              pValue,      //SQLPOINTER pvParam -Value for fAttr
                                              sizeof(buf), //SQLINTEGER bLen -Max # of bytes store in pvParam
                                              &sLen);      //SQLINTEGER* sLen -Length of the output data.(if string)

  if (!sLen)
  { //If the returned value is a number.
    pValue = &retVal;
    sqlReturnCode = SQLGetConnectAttr(this->connh, attr, pValue, 0, &sLen);
    DEBUG(this, "GetConnAttr() attr = %d, value = %d, return code = %d\n", (int)attr, *(int *)pValue, (int)sqlReturnCode);
    if (sqlReturnCode == SQL_SUCCESS)
    {
      return Napi::Number::New(env, *(int *)pValue);
    }
  }
  else
  { //If the returned value is a string.
    DEBUG(this, "GetConnAttr() attr = %d, value = %s, return code = %d\n", (int)attr, (char *)pValue, (int)sqlReturnCode);
    if (sqlReturnCode == SQL_SUCCESS)
    {
      return Napi::String::New(env, buf);
    }
  }
  if (sqlReturnCode != SQL_SUCCESS)
  {
    throwErrMsg(SQL_HANDLE_DBC, connh, env);
  }
  return env.Null();
}

/*
 *  DbConn::Conn
 *    Description: Establishes a connection to the target database.
 *    Parameters: 
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes either 1-4 arguments, stored on the info object depending on the syntex:
 *         Syntex 1: conn(string Database)
 *         Syntex 2: conn(string Database, function Callback)
 *         Syntex 3: conn(string Database, string User, string Password)
 *         Syntex 4: conn(string Database, string User, string Password, function Callback)
 * 
 */
void DbConn::Conn(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  SQLRETURN sqlReturnCode;

  //validation
  if (this->connected == true || this->connAllocated == false)
  {
    return;
  }

  switch (length)
  {
  case 1: //conn(string Database)
    if (!info[0].IsString())
    {
      Napi::TypeError::New(env, "First Parameter Must be a String").ThrowAsJavaScriptException();
      return;
    }
    break;
  case 2: //conn(string Database, function Callback)
    if (!(info[0].IsString() && info[1].IsFunction()))
    {
      Napi::TypeError::New(env, "First Parameter Must be a String, Second Parameter must be a Function").ThrowAsJavaScriptException();
      return;
    }
    break;
  case 3: //conn(string Database, string User, string Password)
    if (!(info[0].IsString() && info[1].IsString() && info[2].IsString()))
    {
      Napi::TypeError::New(env, "All three Parameters Must be a String").ThrowAsJavaScriptException();
      return;
    }
    break;
  case 4: //conn(string Database, string User, string Password, function Callback)
    if (!(info[0].IsString() && info[1].IsString() && info[2].IsString() && info[3].IsFunction()))
    {
      Napi::TypeError::New(env, "First three Parameters Must be a String, last Parameter must be a Function").ThrowAsJavaScriptException();
      return;
    }
    break;
  default:
    Napi::Error::New(env, "conn() takes either 1,2,3, or 4 parameters").ThrowAsJavaScriptException();
    return;
  }
  SQLCHAR *datasource = strdup(Napi::String(env, info[0]).Utf8Value().c_str());
  SQLCHAR *loginuser = NULL;
  SQLCHAR *password = NULL;
  if (length >= 3)
  {
    loginuser = strdup(Napi::String(env, info[1]).Utf8Value().c_str());
    password = strdup(Napi::String(env, info[2]).Utf8Value().c_str());
  }
  // Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnconn.htm
  sqlReturnCode = SQLConnect(this->connh, //SQLHDBC Connection Handle
                             datasource,  //SQLCHAR* szDSN -Name or alias name of the database
                             SQL_NTS,     //SQLSMALLINT cbDSN -Length of contents of szDSN
                             loginuser,   //SQLCHAR* szUID -Auth Name (UID)
                             SQL_NTS,     //SQLSMALLINT cbUID -Length of contents of szUID
                             password,    //SQLCHAR* szAuthStr -Auth String (password)
                             SQL_NTS);    //SQLSMALLINT cbAuthStr Length of Contents of szAuthStr
  DEBUG(this, "SQLConnect(%d): conn obj [%p] handler [%d]\n", sqlReturnCode, this, this->connh);

  free(datasource);
  if (length >= 3)
  {
    free(loginuser);
    free(password);
  }
  if (sqlReturnCode != SQL_SUCCESS)
  {
    throwErrMsg(SQL_HANDLE_DBC, connh, env);
    SQLFreeConnect(this->connh);
    return;
  }
  this->connected = true;
  //when length is 2 or 4 need to Make Callbacks
  if (length == 2 || length == 4)
  {
    Napi::Function cb = info[length - 1].As<Napi::Function>();
    cb.MakeCallback(env.Global(), {env.Null()});
  }
}

/*
 *  DbConn::Disconnect
 *    Description: Ends the connection associated with the database connection handle.
 *    Parameters: 
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes 0 arguments.
 *    Returns: boolean true indicating success or throws an error.
 * 
 */
Napi::Value DbConn::Disconnect(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  SQLRETURN sqlReturnCode;

  if (this->connected)
  {
    SQLINTEGER auto_commit = 0;
    sqlReturnCode = SQLGetConnectAttr(this->connh, SQL_ATTR_AUTOCOMMIT, &auto_commit, 0, NULL);
    if (auto_commit != SQL_TRUE)
    { // If Auto_Commit is disabled, Rollback all transactions before exit.
      //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnendtr.htm
      sqlReturnCode = SQLEndTran(SQL_HANDLE_DBC, // SQLSMALLINT htype -Type of Handle
                                 this->connh,    //SQLHENV handle -Handle to use
                                 SQL_ROLLBACK);  //SQLSMALLINT fType -Wanted Action for the transaction
    }
    DEBUG(this, "SQLDisconnect: conn obj [%p] handler [%d]\n", this, this->connh);
    //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfndconn.htm
    sqlReturnCode = SQLDisconnect(this->connh); //SQLHDBC hdbc -Connection Handle
    CHECK_WITH_RETURN(sqlReturnCode != SQL_SUCCESS, SQL_ERROR, "SQLDisconnect Failed", env, Napi::Boolean::New(env, 0))
    this->connected = false;
  }
  return Napi::Boolean::New(env, 1);
}

/*
 *  DbConn::Close
 *    Description: Frees the connection object.
 *    Parameters: 
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes 0 arguments.
 *    Returns: boolean true indicating success or throws an error.
 * 
 */
Napi::Value DbConn::Close(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  SQLRETURN sqlReturnCode = -1;

  if (this->connAllocated)
  {
    if (this->connected) {
      this->Disconnect(info);
    }
    DEBUG(this, "SQLFreeConnect: conn obj [%p] handler [%d]\n", this, this->connh);
    //Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnfconn.htm
    sqlReturnCode = SQLFreeConnect(this->connh); //SQLHDBC hdbc -Connection Handle
    DEBUG(this, "SQLFreeConnect[%d]\n", sqlReturnCode);
    CHECK_WITH_RETURN(sqlReturnCode != SQL_SUCCESS, SQL_ERROR, "SQLFreeConnect Failed", env, Napi::Boolean::New(env, 0));
    this->connAllocated = false;
  }
  return Napi::Boolean::New(env, 1);
}

/*
 *  DbConn::ValidSmt
 *    Description: Checks if the SQL string is valid and interprets vendor escape clauses.
 *    Parameters: 
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes 1 argument.
 *        info[0] (String): The SQL String to validate.
 *    Returns: boolean true indicating success or throws an error.
 * 
 */
Napi::Value DbConn::ValidStmt(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();

  CHECK_WITH_RETURN(length != 1, INVALID_PARAM_NUM, "Expected 1st Parameter to be a String", env, env.Null())
  CHECK_WITH_RETURN(this->connAllocated == false, CONN_NOT_READY, "Expected Connection to be allocated first, you can do so by using conn()", env, env.Null())

  std::string argStr = Napi::String(env, info[0]).Utf8Value();
  std::vector<char> newCString(argStr.begin(), argStr.end());
  newCString.push_back('\0');

  SQLCHAR *tmpSqlSt = &newCString[0];
  SQLINTEGER outLen = 0;
  SQLCHAR outSqlSt[2048];
  // Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnnsql.htm
  SQLRETURN sqlReturnCode = SQLNativeSql(this->connh,      //SQL HDBC Connection Handle
                                         tmpSqlSt,         //SQLCHAR* InStatementText -Input SQL String
                                         strlen(tmpSqlSt), //SQLINTEGER TextLength1 -Length of InStatementText
                                         outSqlSt,         //SQLCHAR* OutStatementText -Point to transformed String buffer
                                         sizeof(outSqlSt), //SQLINTEGER BufferLength -Size of buffer -> OutStatementText
                                         &outLen);         //SQLINTGER* TextLength2 -number of bytes available to return in OutStatementText
  if (sqlReturnCode != SQL_SUCCESS)
  {
    throwErrMsg(SQL_HANDLE_DBC, connh, env);
    return env.Null();
  }
  if ((unsigned int)outLen < sizeof(outSqlSt))
  {
    outSqlSt[outLen] = '\0';
    return Napi::String::New(env, outSqlSt);
  }
  return env.Null();
}

/*
 *  DbConn::Debug
 *    Description: Turn on or Off Verbose Output:
 *    Parameters:
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes 1 argument.
 *        info[0] (Boolean): true for ON false for OFF.
 *    Returns: boolean true/false indicating the state of the debug switch.
 * 
 */
Napi::Value DbConn::Debug(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  //validation
  CHECK_WITH_RETURN(length != 1, INVALID_PARAM_NUM, "debug() Expected One Parameter", env, env.Null())
  CHECK_WITH_RETURN(!info[0].IsBoolean(), INVALID_PARAM_TYPE, "debug() Expected 1st Parameter to be a Boolean ", env, env.Null())

  this->isDebug = Napi::Boolean(env, info[0]).Value();
  return Napi::Boolean(env, info[0]);
}

/*
 *  DbConn::IsConnected
 *    Description: Determine if the dbconn has a connection to the db.
 *    Parameters: 
 *      const Napi::CallbackInfo& info:
 *        The information passed by Napi from the JavaScript call, including
 *        arguments from the JavaScript function. In JavaScript, the exported
 *        function takes 0 arguments.
 *    Returns: boolean true/false indicating if connected.
 * 
 */
Napi::Value DbConn::IsConnected(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  return Napi::Boolean::New(env, this->connected);
}