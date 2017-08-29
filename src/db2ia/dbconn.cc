/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */
 
#include "dbconn.h"

using namespace v8;

Persistent<Function> DbConn::constructor;
SQLHENV DbConn::envh;

DbConn::DbConn() { 
  SQLRETURN rc = -1;
  int param = SQL_TRUE;
  if(this->connAllocated == false) {
    rc = SQLAllocConnect( envh, &this->connh );
    if(rc != SQL_SUCCESS) {
      return;
    }
    this->connAllocated = true;  // Any Connection Handler processing can not be allowed before this.
  }
  
  rc=SQLSetConnectAttr(this->connh, SQL_ATTR_AUTOCOMMIT, &param, 0);  // Enable auto_commit by default.
  // param = SQL_FALSE;
  // rc=SQLSetConnectAttr(obj->connh, SQL_ATTR_DBC_SYS_NAMING, &param, 0);
}  

DbConn::~DbConn() {}

void DbConn::Init(SQLHENV env) {
  Isolate* isolate = Isolate::GetCurrent();
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "dbconn"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1); 
  // Prototypes
  NODE_SET_PROTOTYPE_METHOD(tpl, "setConnAttr", SetConnAttr);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getConnAttr", GetConnAttr);
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "conn", Conn);
  NODE_SET_PROTOTYPE_METHOD(tpl, "disconn", Disconnect);
  NODE_SET_PROTOTYPE_METHOD(tpl, "close", Close);
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "validStmt", ValidStmt);
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "debug", Debug);
  NODE_SET_PROTOTYPE_METHOD(tpl, "isConnected", IsConnected);
  
  constructor.Reset(isolate, tpl->GetFunction());
  
  envh = env;
}

void DbConn::Debug(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The debug() method accept one boolean parameter(true or false).", isolate )
  CHECK(!args[0]->IsBoolean(), INVALID_PARAM_TYPE, "Debug() The parameter must be a boolean value(true or false).", isolate )
  obj->isDebug = args[0]->Int32Value();
}

void DbConn::IsConnected(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  RETURN(Boolean::New(isolate, obj->isConnected));
}

//new db2i() <==> new DbConn()
void DbConn::New(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  if (args.IsConstructCall()) {
    // Invoked as constructor: `new db2i(...)`
    DbConn* obj = new DbConn();
    obj->Wrap(args.This());
    RETURN(args.This())
  } else {
    // Invoked as plain function `db2i(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    RETURN(cons->NewInstance(argc, argv))
  }
}

void DbConn::NewInstance(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);

  const unsigned argc = 1;
  Handle<Value> argv[argc] = { args[0] };
  Local<Function> cons = Local<Function>::New(isolate, constructor);
  Local<Object> instance = cons->NewInstance(argc, argv);

  RETURN(instance)
}

void DbConn::SetConnAttr(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  
  CHECK(args.Length() != 2, INVALID_PARAM_NUM, "The setConnAttr() method only accept two parameters.", isolate)
  CHECK(!args[0]->IsInt32(), INVALID_PARAM_NUM, "The setConnAttr() method only accept two parameters.", isolate)
  CHECK(obj->connAllocated == false, CONN_NOT_READY, "SetConnAttr() The Connection handler is not initialized.", isolate)
  
  SQLINTEGER attr = args[0]->Int32Value();
  char* cValue;
  SQLINTEGER sLen = 0;
  SQLRETURN rc = -1;
  
  if(args[1]->IsInt32())
  {
    int param = args[1]->Int32Value();
    rc=SQLSetConnectAttr(obj->connh, attr, &param, 0);
    DEBUG("SetConnAttr() attr = %d, value = %d, rc = %d\n", (int)attr, param, (int)rc);
  }
  else if(args[1]->IsString())
  {
    v8::String::Utf8Value arg1(args[1]);
    cValue = *arg1;
    sLen = strlen(cValue);
    rc=SQLSetConnectAttr(obj->connh, attr, cValue, sLen);
    DEBUG("SetConnAttr() attr = %d, value = %s, rc = %d\n", (int)attr, cValue, (int)rc);
  }
  if(rc != SQL_SUCCESS)
    obj->throwErrMsg(SQL_HANDLE_DBC, isolate);
}

void DbConn::GetConnAttr(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The getConnAttr() method only accept one parameters.", isolate)
  CHECK(!args[0]->IsInt32(), INVALID_PARAM_TYPE, "Parameter one must be a numeric value.", isolate)
  CHECK(obj->connAllocated == false, INVALID_PARAM_TYPE, "The Connection handler is not initialized.", isolate)
  
  SQLINTEGER attr = args[0]->Int32Value();
  char buf[1024];
  int retVal = 0;
  SQLINTEGER sLen = 0;
  void* pValue = (char*)&buf;
  SQLRETURN rc = SQLGetConnectAttr(obj->connh, attr, pValue, sizeof(buf), &sLen);
  
  if(!sLen)  //If the returned value is a number.
  {
    pValue = &retVal;
    rc = SQLGetConnectAttr(obj->connh, attr, pValue, 0, &sLen);
    DEBUG("GetConnAttr() attr = %d, value = %d, rc = %d\n", (int)attr, *(int*)pValue, (int)rc);
    if(rc == SQL_SUCCESS)
      RETURN(Number::New(isolate, *(int*)pValue))
  }
  else  //If the returned value is a string.
  {
    DEBUG("GetConnAttr() attr = %d, value = %s, rc = %d\n", (int)attr, (char*)pValue, (int)rc);
    if (rc == SQL_SUCCESS)
      RETURN(String::NewFromUtf8(isolate, buf))
  }
  if(rc != SQL_SUCCESS)
    obj->throwErrMsg(SQL_HANDLE_DBC, isolate);
}

void DbConn::Conn(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  if(obj->connected == true || obj->connAllocated == false)
    return;
  SQLRETURN rc;
  
  CHECK(args.Length() <1 || args.Length() > 4, INVALID_PARAM_NUM, "The conn() method only accept one or three parameters.", isolate)
  CHECK(!args[0]->IsString() || //conn(string Database)
    (args.Length() == 2 && !args[1]->IsFunction()) || //conn(string Database, function Callback)
    (args.Length() == 3 && !args[0]->IsString() && !args[1]->IsString() && !args[2]->IsString()) || //conn(string Database, string User, string Password)
    (args.Length() == 4 && !args[0]->IsString() && !args[1]->IsString() && !args[2]->IsString() && !args[3]->IsFunction()), 
	INVALID_PARAM_TYPE, "Parameter type is not valid.", isolate)

  v8::String::Utf8Value arg0(args[0]);
  v8::String::Utf8Value arg1(args[1]);
  v8::String::Utf8Value arg2(args[2]);
  
  SQLCHAR* datasource = *arg0;
  SQLCHAR* loginuser = NULL;
  SQLCHAR* password = NULL;

  if(args.Length() >= 3) {
    loginuser = *arg1;
    password = *arg2;
  }  
  rc = SQLConnect(obj->connh, datasource, SQL_NTS, loginuser, SQL_NTS, password, SQL_NTS );
  DEBUG("SQLConnect: conn obj [%p] handler [%d]\n", obj, obj->connh);
  if(rc != SQL_SUCCESS) {
    obj->throwErrMsg(SQL_HANDLE_DBC, isolate);
    SQLFreeConnect(obj->connh);
    return;
  }
  obj->connected = true;
  
  if (args.Length() == 2 || args.Length() == 4) {
    Local<Function> cb = Local<Function>::Cast(args[args.Length() - 1]);
    cb->Call(isolate->GetCurrentContext()->Global(), 0, 0);
  }
}

void DbConn::Disconnect(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  SQLRETURN rc;
  if(obj->connected) {
    SQLINTEGER auto_commit = 0;
    rc = SQLGetConnectAttr(obj->connh, SQL_ATTR_AUTOCOMMIT, &auto_commit, 0, NULL);
    // CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLGetConnectAttr() failed.", isolate)
    if (auto_commit != SQL_TRUE ) {  // If Auto_Commit is disabled, Rollback all transactions before exit.
      rc = SQLEndTran(SQL_HANDLE_DBC, obj->connh, SQL_ROLLBACK);
      // CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLEndTran() failed.", isolate)
    }
    DEBUG("SQLDisconnect: conn obj [%p] handler [%d]\n", obj, obj->connh);
    rc = SQLDisconnect(obj->connh);
    // CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLDisconnect() failed.", isolate)
    obj->connected = false;
  }
}

void DbConn::Close(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  SQLRETURN rc = -1;
  if(obj->connAllocated) {
    DEBUG("SQLFreeConnect: conn obj [%p] handler [%d]\n", obj, obj->connh);
    rc = SQLFreeConnect(obj->connh);
    // CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLFreeConnect() failed.", isolate)
    obj->connAllocated = false;  // Any Connection Handler processing can not be allowed after this.
  }
}

void DbConn::ValidStmt(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(obj->connAllocated == false, CONN_NOT_READY, "The Connection handler is not initialized.", isolate)
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The validStmt() method only accept one parameter.", isolate)

  v8::String::Utf8Value arg0(args[0]);
  SQLCHAR* tmpSqlSt = *arg0;
  SQLINTEGER outLen = 0;
  SQLCHAR outSqlSt[2048];
  
  SQLRETURN rc = SQLNativeSql(obj->connh, tmpSqlSt, strlen(tmpSqlSt), outSqlSt, sizeof(outSqlSt), &outLen);
  if(rc != SQL_SUCCESS) {
    obj->throwErrMsg(SQL_HANDLE_DBC, isolate);
    return;
  }
  if(outLen < sizeof(outSqlSt))
    outSqlSt[outLen] = '\0';
  RETURN(String::NewFromUtf8(isolate, outSqlSt))
}
