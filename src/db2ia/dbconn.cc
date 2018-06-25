/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */
 
#include "dbconn.h"

// using namespace Napi;


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
    InstanceMethod("test", &DbConn::Test)

  });

  constructor = Napi::Persistent(constructorfunc);
  constructor.SuppressDestruct();

  envh = envh2;

  exports.Set("dbconn" , constructorfunc);
  return exports;

}

DbConn::DbConn(const Napi::CallbackInfo& info) : Napi::ObjectWrap<DbConn>(info) {
  SQLRETURN rc = -1;
  int param = SQL_TRUE;
  if(this->connAllocated == false) {
    rc = SQLAllocConnect( envh, &this->connh );
    if(rc != SQL_SUCCESS) {
      printf("SQL Connection Allocation Fail");
      return;
    }
    this->connAllocated = true;  // Any Connection Handler processing can not be allowed before this.
  }
  
  rc=SQLSetConnectAttr(this->connh, SQL_ATTR_AUTOCOMMIT, &param, 0);  // Enable auto_commit by default.
}

//Most likely Not needed but was used in version before Napi Compliance.
void DbConn::New(const ARGUMENTS& args) {

}

Napi::Object DbConn::NewInstance(Napi::Value arg){
  Napi::Object obj = constructor.New({ arg });
  return obj;
}
//Test method Delete Later
Napi::Value DbConn::Test(const Napi::CallbackInfo& info) {
  return Napi::String::New(info.Env() , "Hello From Dbconn");
  
}
void DbConn::SetConnAttr(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  //validation
  if(length != 2){
    Napi::Error::New(env, "Expected Two Parameters for SetConnAttr").ThrowAsJavaScriptException();
    return;
  }  

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number Expected For first Parameter of SetConnAttr").ThrowAsJavaScriptException();
    return;
  }

  if(!info[1].IsNumber() && !info[1].IsString() ){
    Napi::TypeError::New(env, "Number or String Expected For Second Parameter of SetConnAttr").ThrowAsJavaScriptException();
    return;
  }
  if(this->connAllocated == false){
    Napi::Error::New(env, "Need to call conn() to allocate connection on DbConn first").ThrowAsJavaScriptException();
    return;
  }

  SQLINTEGER attr = Napi::Number(env, info[0]).Int32Value();
  char* cValue;
  SQLINTEGER sLen = 0;
  SQLRETURN rc = -1;
  //check if the second arg was a Number or a String  
  if(info[1].IsNumber() )
  {
    int param = Napi::Number(env, info[1]).Int32Value();
    rc = SQLSetConnectAttr(this->connh, attr, &param, 0);
    DEBUG(this, "SetConnAttr() attr = %d, value = %d, rc = %d\n", (int)attr, param, (int)rc);
  }
  else if(info[1].IsString())
  {
    std::string arg1 = Napi::String(env , info[1]).Utf8Value();
    std::vector<char> newCString(arg1.begin(), arg1.end());
    newCString.push_back('\0');
    cValue = &newCString[0];
    sLen = strlen(cValue);
    rc=SQLSetConnectAttr(this->connh, attr, cValue, sLen);
    DEBUG(this, "SetConnAttr() attr = %d, value = %s, rc = %d\n", (int)attr, cValue, (int)rc);
  }
  if(rc != SQL_SUCCESS){
    this->throwErrMsg(SQL_HANDLE_DBC, env);
  }
    
}


Napi::Value DbConn::GetConnAttr(const Napi::CallbackInfo& info) {
  
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  //validation
  if(length != 1){
    Napi::Error::New(env, "Expected One Parameter for getConnAttr").ThrowAsJavaScriptException();
    return env.Null();
  }
  if ( !info[0].IsNumber() ){
    Napi::TypeError::New(env, "Expected Parameter to be a Number").ThrowAsJavaScriptException();
    return env.Null();
  }
  if(this->connAllocated == false){
    Napi::Error::New(env, "Expected Connection to be allocated first, you can do so by using conn()").ThrowAsJavaScriptException();
    return env.Null();
  }

  SQLINTEGER attr = Napi::Number(env, info[0]).Int32Value();

  char buf[1024];
  int retVal = 0;
  SQLINTEGER sLen = 0;
  void* pValue = (char*)&buf;
  SQLRETURN rc = SQLGetConnectAttr(this->connh, attr, pValue, sizeof(buf), &sLen);

  if(!sLen)  //If the returned value is a number.
  {
    pValue = &retVal;
    rc = SQLGetConnectAttr(this->connh, attr, pValue, 0, &sLen);
    DEBUG(this, "GetConnAttr() attr = %d, value = %d, rc = %d\n", (int)attr, *(int*)pValue, (int)rc);
    if(rc == SQL_SUCCESS){
      // RETURN(Number::New(isolate, *(int*)pValue))
      return Napi::Number::New(env , *(int*)pValue);
    }
  }
  else  //If the returned value is a string.
  {
    DEBUG(this, "GetConnAttr() attr = %d, value = %s, rc = %d\n", (int)attr, (char*)pValue, (int)rc);
    if (rc == SQL_SUCCESS){
       //RETURN(String::NewFromUtf8(isolate, buf))
       return Napi::String::New(env , buf);
    }
    
  }
  if(rc != SQL_SUCCESS){
    this->throwErrMsg(SQL_HANDLE_DBC, env);
    return env.Null();
  }
}

void DbConn::Conn(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  SQLRETURN rc;
  
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
  std::string arg0 = Napi::String(env , info[0]).Utf8Value();
  std::vector<char> arg0Vec(arg0.begin(), arg0.end());
  arg0Vec.push_back('\0');

  SQLCHAR* datasource = &arg0Vec[0];
  SQLCHAR* loginuser = NULL;
  SQLCHAR* password = NULL;

  if(length >= 3) {

    std::string arg1 = Napi::String(env , info[1]).Utf8Value();
    std::vector<char> arg1Vec(arg1.begin(), arg1.end());
    arg1Vec.push_back('\0');

    std::string arg2 = Napi::String(env , info[2]).Utf8Value();
    std::vector<char> arg2Vec(arg2.begin(), arg2.end());
    arg2Vec.push_back('\0');

    loginuser = &arg1Vec[0];
    password = &arg2Vec[0];
  } 

  rc = SQLConnect(this->connh, datasource, SQL_NTS, loginuser, SQL_NTS, password, SQL_NTS );

  DEBUG(this, "SQLConnect: conn obj [%p] handler [%d]\n", this, this->connh);

  printf("RC is %d\n" , rc);

  if(rc != SQL_SUCCESS) {
    this->throwErrMsg(SQL_HANDLE_DBC, env);
    printf("RC Fail\n");
    SQLFreeConnect(this->connh);
    return;
  }
  printf("RC Pass\n");
  this->connected = true;
  
  if (length == 2 || length == 4) {
    Napi::Function cb = info[ length -1 ].As<Napi::Function>();
    cb.MakeCallback(env.Global() , 0 , 0);
  }

  
}

void DbConn::Disconnect(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  SQLRETURN rc;

  if(this->connected) {
    SQLINTEGER auto_commit = 0;
    rc = SQLGetConnectAttr(this->connh, SQL_ATTR_AUTOCOMMIT, &auto_commit, 0, NULL);
    if (auto_commit != SQL_TRUE ) {  // If Auto_Commit is disabled, Rollback all transactions before exit.
      rc = SQLEndTran(SQL_HANDLE_DBC, this->connh, SQL_ROLLBACK);
    }
    DEBUG(this, "SQLDisconnect: conn obj [%p] handler [%d]\n", this, this->connh);
    rc = SQLDisconnect(this->connh);
    this->connected = false;
  }
  
}

void DbConn::Close(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  SQLRETURN rc = -1;

  if(this->connAllocated) {
    DEBUG(this, "SQLFreeConnect: conn obj [%p] handler [%d]\n", this, this->connh);
    rc = SQLFreeConnect(this->connh);
    this->connAllocated = false;  // Any Connection Handler processing can not be allowed after this.
  }
  
}

Napi::Value DbConn::ValidStmt(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();

  if(length != 1 ){
    Napi::TypeError::New(env, "Expected 1st Parameter to be a String").ThrowAsJavaScriptException();
    return env.Null();

  }
  if(this->connAllocated == false){
    Napi::Error::New(env, "Expected Connection to be allocated first, you can do so by using conn()").ThrowAsJavaScriptException();
    return env.Null();
  }
  
  std::string argStr = Napi::String(env , info[0]).Utf8Value();
  std::vector<char> newCString(argStr.begin(), argStr.end());
  newCString.push_back('\0');

  SQLCHAR* tmpSqlSt = &newCString[0];
  SQLINTEGER outLen = 0;
  SQLCHAR outSqlSt[2048];
  
  SQLRETURN rc = SQLNativeSql(this->connh, tmpSqlSt, strlen(tmpSqlSt), outSqlSt, sizeof(outSqlSt), &outLen);
  if(rc != SQL_SUCCESS) {
    this->throwErrMsg(SQL_HANDLE_DBC, env);
    return env.Null();
  }
  if(outLen < sizeof(outSqlSt)){
    outSqlSt[outLen] = '\0';
    return Napi::String::New(env , outSqlSt);
  }

  
}

void DbConn::Debug(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  int length = info.Length();
  //validation
  if(length != 1){
    Napi::Error::New(env, "Expected One Parameter for Debug").ThrowAsJavaScriptException();
    return;
  }
  if( !info[0].IsBoolean() ){
    Napi::TypeError::New(env, "Expected 1st Parameter to be a Boolean for Debug").ThrowAsJavaScriptException();
    return;
  }
  
  this->isDebug = Napi::Boolean(env , info[0]).Value();
}

Napi::Value DbConn::IsConnected(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  return Napi::Boolean::New(env , this->connected);
  
}