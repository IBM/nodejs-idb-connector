/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */
#include "dbconn.h"

SQLHENV DbConn::envh;
Napi::FunctionReference DbConn::constructor;

Napi::Object DbConn::Init(Napi::Env env, Napi::Object exports, SQLHENV envh2) {
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

  exports.Set("dbconn" , constructorfunc);
  return exports;

}

DbConn::DbConn(const Napi::CallbackInfo& info) : Napi::ObjectWrap<DbConn>(info) {
  SQLRETURN sqlReturnCode = -1;
  int param = SQL_TRUE;
  if(this->connAllocated == false) {
    sqlReturnCode = SQLAllocConnect( envh, &this->connh );
    if(sqlReturnCode != SQL_SUCCESS) {
      DEBUG(this, "SQLAllocConnect(%d): SQL Connection Allocation Fail", sqlReturnCode);
      return;
    }
    this->connAllocated = true;  // Any Connection Handler processing can not be allowed before this.
  }
  
  sqlReturnCode = SQLSetConnectAttr(this->connh, SQL_ATTR_AUTOCOMMIT, &param, 0);  // Enable auto_commit by default.
}

//Most likely Not needed but was used in version before Napi Compliance.
void DbConn::New(const ARGUMENTS& args) {

}

Napi::Object DbConn::NewInstance(Napi::Value arg){
  Napi::Object dbConn = constructor.New({ arg });
  return dbConn;
}

Napi::Value DbConn::SetConnAttr(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  //validation
  CHECK_WITH_RETURN(length != 2, INVALID_PARAM_NUM, "Expected Two Parameters for SetConnAttr", env, env.Null())
  CHECK_WITH_RETURN(!info[0].IsNumber(), INVALID_PARAM_TYPE, "Expected Parameter 1 to be a Number", env, env.Null())
  CHECK_WITH_RETURN(!info[1].IsNumber() && !info[1].IsString(), INVALID_PARAM_TYPE, "Number or String Expected For Second Parameter of SetConnAttr", env, env.Null())
  CHECK_WITH_RETURN(this->connAllocated == false, CONN_NOT_READY, "Need to call conn() to allocate connection on DbConn first", env, env.Null())

  SQLINTEGER attr = Napi::Number(env, info[0]).Int32Value();
  char* cValue;
  SQLINTEGER sLen = 0;
  SQLRETURN sqlReturnCode = -1;
  //check if the second arg was a Number or a String  
  if(info[1].IsNumber() )
  {
    int param = Napi::Number(env, info[1]).Int32Value();
    sqlReturnCode = SQLSetConnectAttr(this->connh, attr, &param, 0);
    DEBUG(this, "SetConnAttr() attr = %d, value = %d, rc = %d\n", (int)attr, param, (int)sqlReturnCode);
  }
  else if(info[1].IsString())
  {
    std::string arg1 = Napi::String(env , info[1]).Utf8Value();
    std::vector<char> newCString(arg1.begin(), arg1.end());
    newCString.push_back('\0');
    cValue = &newCString[0];
    sLen = strlen(cValue);
    sqlReturnCode = SQLSetConnectAttr(this->connh, attr, cValue, sLen);
    DEBUG(this, "SetConnAttr() attr = %d, value = %s, rc = %d\n", (int)attr, cValue, (int)sqlReturnCode);
  }
  if(sqlReturnCode != SQL_SUCCESS){
    this->throwErrMsg(SQL_HANDLE_DBC, env);
    return env.Null();
  }
  return Napi::Boolean::New(env, 1);
}


Napi::Value DbConn::GetConnAttr(const Napi::CallbackInfo& info) {
  
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
  void* pValue = (char*)&buf;
  SQLRETURN sqlReturnCode = SQLGetConnectAttr(this->connh, attr, pValue, sizeof(buf), &sLen);

  if(!sLen)  //If the returned value is a number.
  {
    pValue = &retVal;
    sqlReturnCode = SQLGetConnectAttr(this->connh, attr, pValue, 0, &sLen);
    DEBUG(this, "GetConnAttr() attr = %d, value = %d, rc = %d\n", (int)attr, *(int*)pValue, (int)sqlReturnCode);
    if(sqlReturnCode == SQL_SUCCESS){
      return Napi::Number::New(env , *(int*)pValue);
    }
  }
  else  //If the returned value is a string.
  {
    DEBUG(this, "GetConnAttr() attr = %d, value = %s, rc = %d\n", (int)attr, (char*)pValue, (int)sqlReturnCode);
    if (sqlReturnCode == SQL_SUCCESS){
       return Napi::String::New(env , buf);
    }
    
  }
  if(sqlReturnCode != SQL_SUCCESS){
    this->throwErrMsg(SQL_HANDLE_DBC, env);
    return env.Null();
  }
}

void DbConn::Conn(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  SQLRETURN sqlReturnCode;
  
  //validation
  if(this->connected == true || this->connAllocated == false){
    return;
  }

  switch(length){
    case 1: //conn(string Database)
      if (!info[0].IsString()){
        Napi::TypeError::New(env, "First Parameter Must be a String").ThrowAsJavaScriptException();
        return;
      }
      break;
    case 2: //conn(string Database, function Callback)
      if (!( info[0].IsString() && info[1].IsFunction() ) ){
        Napi::TypeError::New(env, "First Parameter Must be a String , Second Parameter must be a Function").ThrowAsJavaScriptException();
        return;
      }
      break;
    case 3: //conn(string Database, string User, string Password)
      if (!( info[0].IsString() && info[1].IsString() && info[2].IsString() ) ){
        Napi::TypeError::New(env, "All three Parameters Must be a String").ThrowAsJavaScriptException();
        return;
      }
      break;
    case 4: //conn(string Database, string User, string Password, function Callback)
      if (!( info[0].IsString() && info[1].IsString() && info[2].IsString() && info[3].IsFunction() ) ){
        Napi::TypeError::New(env, "First three Parameters Must be a String , last Parameter must be a Function").ThrowAsJavaScriptException();
        return;
      }
      break;
    default:
      Napi::Error::New(env, "conn() takes either 1,2,3, or 4 parameters").ThrowAsJavaScriptException();
      return;
  }
  std::string arg0 = Napi::String(env, info[0]).Utf8Value();
  // std::vector<char> arg0Vec(arg0.begin(), arg0.end());
  // arg0Vec.push_back('\0');  // may lead to "Authorization failure on distributed database connection attempt"

  SQLCHAR* datasource = &arg0[0u];
  SQLCHAR* loginuser = NULL;
  SQLCHAR* password = NULL;

  if(length >= 3) {

    std::string arg1 = Napi::String(env, info[1]).Utf8Value();
    // std::vector<char> arg1Vec(arg1.begin(), arg1.end());
    // arg1Vec.push_back('\0');

    std::string arg2 = Napi::String(env, info[2]).Utf8Value();
    // std::vector<char> arg2Vec(arg2.begin(), arg2.end());
    // arg2Vec.push_back('\0');

    loginuser = &arg1[0u];
    password = &arg2[0u];
  } 

  sqlReturnCode = SQLConnect(this->connh, datasource, SQL_NTS, loginuser, SQL_NTS, password, SQL_NTS );

  DEBUG(this, "SQLConnect(%d): conn obj [%p] handler [%d]\n", sqlReturnCode, this, this->connh);

  if(sqlReturnCode != SQL_SUCCESS) {
    this->throwErrMsg(SQL_HANDLE_DBC, env);
    SQLFreeConnect(this->connh);
    return;
  }
  this->connected = true;
  //when length is 2 or 4 need to Make Callbacks
  if (length == 2 || length == 4) {
    Napi::Function cb = info[ length -1 ].As<Napi::Function>();
    cb.MakeCallback(env.Null() , {});
  }

}

Napi::Value DbConn::Disconnect(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  SQLRETURN sqlReturnCode;

  if(this->connected) {
    SQLINTEGER auto_commit = 0;
    sqlReturnCode = SQLGetConnectAttr(this->connh, SQL_ATTR_AUTOCOMMIT, &auto_commit, 0, NULL);
    if (auto_commit != SQL_TRUE ) {  // If Auto_Commit is disabled, Rollback all transactions before exit.
      sqlReturnCode = SQLEndTran(SQL_HANDLE_DBC, this->connh, SQL_ROLLBACK);
    }
    DEBUG(this, "SQLDisconnect: conn obj [%p] handler [%d]\n", this, this->connh);
    sqlReturnCode = SQLDisconnect(this->connh);
    CHECK_WITH_RETURN(sqlReturnCode != SQL_SUCCESS, SQL_ERROR, "SQLDisconnect Failed", env,Napi::Boolean::New(env, 0))
    this->connected = false;
  }
  return Napi::Boolean::New(env, 1);
  
}

Napi::Value DbConn::Close(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  SQLRETURN sqlReturnCode = -1;

  if(this->connAllocated) {
    DEBUG(this, "SQLFreeConnect: conn obj [%p] handler [%d]\n", this, this->connh);
    sqlReturnCode = SQLFreeConnect(this->connh);
    DEBUG(this, "SQLFreeConnect[%d]\n", sqlReturnCode);
    CHECK_WITH_RETURN(sqlReturnCode != SQL_SUCCESS, SQL_ERROR,"SQLFreeConnect Failed", env, Napi::Boolean::New(env, 0));
    this->connAllocated = false;
  }
  return Napi::Boolean::New(env, 1);
  
}

Napi::Value DbConn::ValidStmt(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();

  CHECK_WITH_RETURN(length != 1, INVALID_PARAM_NUM,"Expected 1st Parameter to be a String", env, env.Null())
  CHECK_WITH_RETURN(this->connAllocated == false, CONN_NOT_READY,"Expected Connection to be allocated first, you can do so by using conn()", env, env.Null())
  
  std::string argStr = Napi::String(env , info[0]).Utf8Value();
  std::vector<char> newCString(argStr.begin(), argStr.end());
  newCString.push_back('\0');

  SQLCHAR* tmpSqlSt = &newCString[0];
  SQLINTEGER outLen = 0;
  SQLCHAR outSqlSt[2048];
  
  SQLRETURN sqlReturnCode = SQLNativeSql(this->connh, tmpSqlSt, strlen(tmpSqlSt), outSqlSt, sizeof(outSqlSt), &outLen);
  if(sqlReturnCode != SQL_SUCCESS) {
    this->throwErrMsg(SQL_HANDLE_DBC, env);
    return env.Null();
  }
  if(outLen < sizeof(outSqlSt)){
    outSqlSt[outLen] = '\0';
    return Napi::String::New(env , outSqlSt);
  }

  
}

Napi::Value DbConn::Debug(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  //validation
  CHECK_WITH_RETURN(length != 1, INVALID_PARAM_NUM, "debug() Expected One Parameter", env, env.Null())
  CHECK_WITH_RETURN(!info[0].IsBoolean(), INVALID_PARAM_TYPE, "debug() Expected 1st Parameter to be a Boolean ", env, env.Null())

  this->isDebug = Napi::Boolean(env , info[0]).Value();
  return  Napi::Boolean(env , info[0]);
}

Napi::Value DbConn::IsConnected(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  return Napi::Boolean::New(env , this->connected);
  
}