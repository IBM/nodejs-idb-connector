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

#pragma comment (copyright, \
 "5733OPS\
 IBM Open Source Solutions for i5/OS\
 (C) Copyright IBM Corp. 2014, 2016.\
 All rights reserved.\
 US Government Users Restricted Rights -\
 Use, duplication or disclosure restricted\
 by GSA ADP Schedule Contract with IBM Corp.\
 Licensed Materials - Property of IBM ")
 
#include <node.h>
#include "dbconn.h"

using namespace v8;

Persistent<Function> DbConn::constructor;
    
DbConn::DbConn() :
  envAllocated(false),  //Env()/init() make it true.
  connAllocated(false),  //Conn()/conn() make it true.
  stmtAllocated(false),  //Conn()/conn() make it true.
  resultSetAvailable(false), //Execute()/execute() or Exec()/exec() make it true.
  isDebug(false),  //Debug()/debug() make it true.
  colRowAllocated(false),  //Indicates the result set array is allocated
  colCount(0),   //Number of columns in result set.
  spInCount(0),  //Number of output parameters in stored procedures.
  spOutCount(0){  //Number of output parameters in stored procedures.
  }  

DbConn::~DbConn() {}

void DbConn::Init() {
  Isolate* isolate = Isolate::GetCurrent();
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "db2i"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1); 
  // Prototypes
  NODE_SET_PROTOTYPE_METHOD(tpl, "setEnvAttr", SetEnvAttr);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setConnAttr", SetConnAttr);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setStmtAttr", SetStmtAttr);
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "getEnvAttr", GetEnvAttr);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getConnAttr", GetConnAttr);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getStmtAttr", GetStmtAttr);
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "init", Env);
  NODE_SET_PROTOTYPE_METHOD(tpl, "conn", Conn);
  NODE_SET_PROTOTYPE_METHOD(tpl, "close", Close);
  NODE_SET_PROTOTYPE_METHOD(tpl, "exec", Exec);
  NODE_SET_PROTOTYPE_METHOD(tpl, "prepare", Prepare);
  NODE_SET_PROTOTYPE_METHOD(tpl, "bindParam", BindParam);
  NODE_SET_PROTOTYPE_METHOD(tpl, "execute", Execute);
  NODE_SET_PROTOTYPE_METHOD(tpl, "nextResult", NextResult);
  NODE_SET_PROTOTYPE_METHOD(tpl, "closeCursor", CloseCursor);

  NODE_SET_PROTOTYPE_METHOD(tpl, "fetch", Fetch);
  NODE_SET_PROTOTYPE_METHOD(tpl, "fetchAll", FetchAll);
  NODE_SET_PROTOTYPE_METHOD(tpl, "commit", Commit);
  NODE_SET_PROTOTYPE_METHOD(tpl, "rollback", Rollback);

  NODE_SET_PROTOTYPE_METHOD(tpl, "numFields", NumFields);
  NODE_SET_PROTOTYPE_METHOD(tpl, "numRows", NumRows);
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "fieldType", FieldType);
  NODE_SET_PROTOTYPE_METHOD(tpl, "fieldWidth", FieldWidth);
  NODE_SET_PROTOTYPE_METHOD(tpl, "fieldName", FieldName);
  NODE_SET_PROTOTYPE_METHOD(tpl, "fieldPrecise", FieldPrecise);
  NODE_SET_PROTOTYPE_METHOD(tpl, "fieldScale", FieldScale);
  NODE_SET_PROTOTYPE_METHOD(tpl, "fieldNullable", FieldNullable);
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "validStmt", ValidStmt);
  NODE_SET_PROTOTYPE_METHOD(tpl, "stmtError", StmtError);
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "debug", Debug);
  NODE_SET_PROTOTYPE_METHOD(tpl, "runXS", RunXS);
  
  constructor.Reset(isolate, tpl->GetFunction());
}

void DbConn::Debug(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The debug() method accept one boolean parameter(true or false).", isolate )
  CHECK(!args[0]->IsBoolean(), INVALID_PARAM_TYPE, "Debug() The parameter must be a boolean value(true or false).", isolate )
  obj->isDebug = args[0]->Int32Value();
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

void DbConn::SetEnvAttr(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  
  CHECK(args.Length() != 2, INVALID_PARAM_NUM, "The setEnvAttr() method only accept two parameters.", isolate )
  CHECK(!args[0]->IsInt32(),  INVALID_PARAM_TYPE, "SetEnvAttr() Parameter one must be a numeric value.", isolate )
  if (obj->connAllocated == true) {
    // The Environment attribute cannot be set if a connection handler has been allocated.
    // Just ignore it.
    return;
  }
  
  SQLINTEGER attr = args[0]->Int32Value();
  char* cValue;
  SQLINTEGER sLen = 0;
  SQLRETURN rc = -1;
  
  if(args[1]->IsInt32()) {
    int param = args[1]->Int32Value();
    rc=SQLSetEnvAttr(obj->envh, attr, &param, 0);
    DEBUG("SetEnvAttr() attr = %d, value = %d, rc = %d\n", (int)attr, param, (int)rc);
  }
  else if(args[1]->IsString()) {
    v8::String::Utf8Value arg1(args[1]);
    cValue = *arg1;
    sLen = strlen(cValue);
    rc=SQLSetEnvAttr(obj->envh, attr, cValue, sLen);
    DEBUG("SetEnvAttr() attr = %d, value = %s, rc = %d\n", (int)attr, cValue, (int)rc)
  }
}

void DbConn::GetEnvAttr(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The getEnvAttr() method only accept one parameters.", isolate)
  CHECK(!args[0]->IsInt32(), INVALID_PARAM_TYPE, "GetEnvAttr() Parameter one must be a numeric value.", isolate)
  
  SQLINTEGER attr = args[0]->Int32Value();
  char buf[1024];
  int retVal = 0;
  SQLINTEGER sLen = 0;
  void* pValue = (char*)&buf;
  SQLRETURN rc = SQLGetEnvAttr(obj->envh, attr, pValue, sizeof(buf), &sLen);
  
  if(!sLen)  //If the returned value is a number.
  {
    pValue = &retVal;
    rc = SQLGetEnvAttr(obj->envh, attr, pValue, 0, &sLen);
    DEBUG("GetEnvAttr() attr = %d, value = %d, rc = %d\n", (int)attr, *(int*)pValue, (int)rc)
    if (rc == SQL_SUCCESS)
      RETURN(Number::New(isolate, *(int*)pValue))
  }
  else  //If the returned value is a string.
  {
    DEBUG("GetEnvAttr() attr = %d, value = %s, rc = %d\n", (int)attr, (char*)pValue, (int)rc)
    if (rc == SQL_SUCCESS)
      RETURN(String::NewFromUtf8(isolate, buf))
  }
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

void DbConn::SetStmtAttr(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());

  CHECK(args.Length() != 2, INVALID_PARAM_NUM, "The setStmtAttr() method only accept two parameters.", isolate)
  CHECK(!args[0]->IsInt32(), INVALID_PARAM_TYPE, "SetStmtAttr() Parameter one must be a numeric value.", isolate)
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "SetStmtAttr() The SQL Statment handler is not initialized.", isolate)

  SQLINTEGER attr = args[0]->Int32Value();
  char* cValue;
  SQLINTEGER sLen = 0;
  SQLRETURN rc = -1;
  /* TODO: There will be a V8 Exception thrown here if using 'args[1]->IsInt32()'. As there is no string option for stmt attr, work around it for now.*/
  if(args[1]->IsInt32())
  {
    int param = args[1]->Int32Value();
    rc = SQLSetStmtAttr(obj->stmth, attr, &param, 0);
    DEBUG("SetStmtAttr() attr = %d, value = %d, rc = %d\n", (int)attr, param, (int)rc);
  }
  else if(args[1]->IsString())
  {
    v8::String::Utf8Value arg1(args[1]);
    cValue = *arg1;
    sLen = strlen(cValue);
    rc = SQLSetStmtAttr(obj->stmth, attr, cValue, sLen);
    DEBUG("SetStmtAttr() attr = %d, value = %s, rc = %d\n", (int)attr, cValue, (int)rc);
  }
  if(rc != SQL_SUCCESS)
    obj->throwErrMsg(SQL_HANDLE_STMT, isolate);
}

void DbConn::GetStmtAttr(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The getStmtAttr() method only accept one parameters.", isolate )
  CHECK(!args[0]->IsInt32(), INVALID_PARAM_TYPE, "GetStmtAttr() Parameter one must be a numeric value.", isolate )
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "GetStmtAttr() The SQL Statment handler is not initialized.", isolate )
  
  SQLINTEGER attr = args[0]->Int32Value();
  char buf[1024];
  int retVal = 0;
  SQLINTEGER sLen = 0;
  void* pValue = (char*)&buf;
  SQLRETURN rc = SQLGetStmtAttr(obj->stmth, attr, pValue, sizeof(buf), &sLen);
  
  if(!sLen)  //If the returned value is a number.
  {
    pValue = &retVal;
    rc = SQLGetStmtAttr(obj->stmth, attr, pValue, 0, &sLen);
    DEBUG("GetStmtAttr() attr = %d, value = %d, rc = %d\n", (int)attr, *(int*)pValue, (int)rc)
    if(rc == SQL_SUCCESS)
      RETURN(Number::New(isolate, *(int*)pValue))
  }
  else  //If the returned value is a string.
  {
    DEBUG("GetStmtAttr() attr = %d, value = %s, rc = %d\n", (int)attr, (char*)pValue, (int)rc)
    if (rc == SQL_SUCCESS)
      RETURN(String::NewFromUtf8(isolate, buf))
  }
  if(rc != SQL_SUCCESS)
    obj->throwErrMsg(SQL_HANDLE_STMT, isolate);
}

void DbConn::Env(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  int override_ccsid = 1208;
  int cb_idx = -1;
  if (args.Length() == 1) {
    if(args[0]->IsInt32())
      override_ccsid = args[0]->Int32Value();
    else if(args[0]->IsFunction())
      cb_idx = 0;
    else {
      obj->throwErrMsg( INVALID_PARAM_TYPE, "Parameter one must be a callback function.", isolate);
      return;
    }
  }
  if (args.Length() == 2) {
    if(args[0]->IsInt32() && args[1]->IsFunction()) {
      override_ccsid = args[0]->Int32Value();
      cb_idx = 1;
    }
    else {
      obj->throwErrMsg( INVALID_PARAM_TYPE, "Parameter one must be an integer and param two must be a callback function.", isolate);
      return;
    }
  }

  SQLOverrideCCSID400(override_ccsid);
  
  SQLRETURN rc = SQLAllocEnv( &obj->envh );
  if(rc != SQL_SUCCESS) {
    /* If SQL_ERROR is returned and phenv is equal to SQL_NULL_HENV, then SQLGetDiagRec() cannot be called */
    /* because there is no handle with which to associate additional diagnostic information. */
    /* If the return code is SQL_ERROR and the pointer to the environment handle is not equal to */
    /* SQL_NULL_HENV, then the handle is a restricted handle. This means the handle can only be used in a call */
    /* to SQLGetDiagRec() to obtain more error information, or to SQLFreeEnv(). */
    if(obj->envh != SQL_NULL_HENV) {
      SQLFreeEnv(obj->envh);
    }
    return;
  }
  obj->envAllocated = true;  // Any Environment Handler processing can not be allowed before this.
  
  if (cb_idx >= 0) {   // This Callback is for setting Env attributes before connecting.
    Local<Function> cb = Local<Function>::Cast(args[cb_idx]);
    cb->Call(isolate->GetCurrentContext()->Global(), 0, 0);
  }
}

void DbConn::Conn(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  
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
  
  DEBUG("Connect to %s\n", datasource)
  if(args.Length() >= 3) {
    loginuser = *arg1;
    password = *arg2;
  }
  SQLRETURN rc = SQLAllocConnect( obj->envh, &obj->connh );
  if(rc != SQL_SUCCESS) {
    obj->throwErrMsg(SQL_HANDLE_ENV, isolate);
    return;
  }
  obj->connAllocated = true;  // Any Connection Handler processing can not be allowed before this.
  if (args.Length() == 2 || args.Length() == 4) {  // This Callback is for setting Connection attributes before connecting.
    Local<Function> cb = Local<Function>::Cast(args[args.Length() - 1]);
    cb->Call(isolate->GetCurrentContext()->Global(), 0, 0);
  }
  rc = SQLConnect(obj->connh, datasource, SQL_NTS, loginuser, SQL_NTS, password, SQL_NTS );
  DEBUG("Connected to %s with ID %s.\n", datasource, loginuser)
  if(rc != SQL_SUCCESS) {
    obj->throwErrMsg(SQL_HANDLE_DBC, isolate);
    SQLFreeConnect(obj->connh);
    return;
  }
}

void DbConn::Close(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  SQLRETURN rc;
  if(obj->connAllocated) {
    SQLINTEGER auto_commit = 0;
    rc = SQLGetConnectAttr(obj->connh, SQL_ATTR_AUTOCOMMIT, &auto_commit, 0, NULL);
    CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLGetConnectAttr() failed.", isolate)

    if (auto_commit != SQL_TRUE ) {  // If Auto_Commit is disabled, Rollback all transactions before exit.
      rc = SQLEndTran(SQL_HANDLE_DBC, obj->connh, SQL_ROLLBACK);
      CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLEndTran() failed.", isolate)
    }
  }
  if(obj->stmtAllocated) {
    rc = SQLFreeStmt( obj->stmth, SQL_CLOSE );  //Free the statement handle here. No further processing allowed.
    //CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLFreeStmt() failed.", isolate)
    obj->stmtAllocated = false;  // Any SQL Statement Handler processing can not be allowed after this.
    obj->freeColRow();
    obj->resultSetAvailable = false;
  }
  if(obj->connAllocated) {
    rc = SQLDisconnect(obj->connh);
    CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLDisconnect() failed.", isolate)
    rc = SQLFreeConnect(obj->connh);
    CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLFreeConnect() failed.", isolate)
    obj->connAllocated = false;  // Any Connection Handler processing can not be allowed after this.
  }
  if(obj->envAllocated) {
    rc = SQLFreeEnv(obj->envh);
    if (rc != SQL_SUCCESS) {
      return;
    }
    obj->envAllocated = false;  // Any Connection Handler processing can not be allowed after this.
  }
}

void DbConn::Exec(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  SQLINTEGER rowCount;
  int arrayCount = 0;  
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(args.Length() != 1 && args.Length() != 2, INVALID_PARAM_NUM, "The exec() method accept only one or two parameter.", isolate)
  
  SQLRETURN rc;
  if(obj->connAllocated)
    rc = SQLAllocStmt(obj->connh, &obj->stmth);
  else
    return;
  if(rc != SQL_SUCCESS) {
    SQLFreeStmt( obj->stmth, SQL_CLOSE );
    // obj->throwErrMsg(SQL_HANDLE_DBC, isolate);
    return;
  } 
  obj->stmtAllocated = true;  // Any SQL Statement Handler processing can not be allowed before this.

  v8::String::Utf8Value arg0(args[0]);
  SQLCHAR* tmpSqlSt = *arg0;
  DEBUG("SQLExecDirect: %s\n", tmpSqlSt);
  rc = SQLExecDirect(obj->stmth, tmpSqlSt, SQL_NTS);
  if(rc != SQL_SUCCESS) {
    if(rc == SQL_NO_DATA_FOUND)
      args.GetReturnValue().SetUndefined(); //SQL statement is a Searched UPDATE/DELETE and no rows satisfy the search condition
    else {
      obj->throwErrMsg(SQL_HANDLE_STMT, isolate);
      return;
    }
  }
  rc = SQLRowCount(obj->stmth, &rowCount);
  // CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLRowCount() failed.", isolate)
  DEBUG("SQLRowCount: %d\n", rowCount);

  rc = SQLNumResultCols(obj->stmth, &obj->colCount);
  // CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLNumResultCols() failed.", isolate)
  DEBUG("SQLNumResultCols: %d\n", obj->colCount);

  /* determine statement type */
  if (obj->colCount == 0) /* statement is not a select statement */
    return;  /* User can issue numRows() to get the affected rows number. */

  /* It is a select statement */
  obj->resultSetAvailable = true;
  Handle<Array> array = Array::New(isolate);
  obj->allocColRow();
  
  for(int i = 0; i < obj->colCount; i++)
  {
    SQLINTEGER rlength = 0;
    rc = SQLDescribeCol(obj->stmth, i + 1, //Column index starts from 1.
          obj->dbColumn[i].name,  //the buffer to store the Column Name
          MAX_COLNAME_WIDTH, //the size of the store buffer 
          &obj->dbColumn[i].nameLength,  //the accute length of the Column Name
          &obj->dbColumn[i].sqlType,  //the SQL type of the Column
          &obj->dbColumn[i].colPrecise, &obj->dbColumn[i].colScale, &obj->dbColumn[i].colNull);
    if(rc != SQL_SUCCESS) {
      obj->freeColRow();
      obj->throwErrMsg( SQL_ERROR, "SQLDescribeCol() failed.", isolate);
      return;
    }
    rc = SQLBindCol(obj->stmth, i + 1, SQL_CHAR, (SQLPOINTER)obj->rowData[i], MAX_COL_WIDTH, &rlength);
    if(rc != SQL_SUCCESS) {
      obj->freeColRow();
      obj->throwErrMsg( SQL_ERROR, "SQLBindCol() failed.", isolate);
      return;
    }
  }
  while((rc = SQLFetch(obj->stmth)) == SQL_SUCCESS) 
  {
    Handle<Object> row = Object::New(isolate);
    for(int i = 0; i < obj->colCount; i++)
    {
      Local<Value> value = Local<Value>::New(isolate, String::NewFromUtf8(isolate, obj->rowData[i]));
      row->Set(String::NewFromUtf8(isolate, (char const*)obj->dbColumn[i].name), value);
    }
    array->Set(arrayCount++, row);  //Build the JSON data
  }
  if (rc != SQL_NO_DATA_FOUND) {
    obj->freeColRow();
    obj->throwErrMsg(SQL_ERROR, "SQLFetch() failed.", isolate);
  }
  if (args.Length() == 2) {
    Local<Function> cb = Local<Function>::Cast(args[args.Length() - 1]);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { Local<Value>::New(isolate, array) };
    cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
  }
  /* User can issue numRows() to get the affected rows number. */
  obj->freeColRow();
  DEBUG("SQLCloseCursor: stmth %d\n", obj->stmth)
  rc = SQLCloseCursor(obj->stmth);
  CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLCloseCursor() failed.", isolate)
}

void DbConn::CloseCursor(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", isolate)
  
  SQLRETURN rc = SQLCloseCursor(obj->stmth);
  CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLCloseCursor() failed.", isolate)
}

void DbConn::Prepare(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  
  CHECK(args.Length() != 1 && args.Length() != 2, INVALID_PARAM_NUM, "The prepare() method accepts one or two parameters.", isolate)
  
  SQLRETURN rc;
  if(obj->connAllocated)
    rc = SQLAllocStmt(obj->connh, &obj->stmth);
  else
    return;
  if(rc != SQL_SUCCESS) {
    SQLFreeStmt( obj->stmth, SQL_CLOSE );
    // obj->throwErrMsg(SQL_HANDLE_DBC, isolate);
    return;
  } 
  obj->stmtAllocated = true;  // Any SQL Statement Handler processing can not be allowed before this.
  
  v8::String::Utf8Value sql(args[0]);
  DEBUG("SQL: %s\n", *sql)
  rc = SQLPrepare(obj->stmth, *sql, strlen(*sql));
  CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLPrepare() failed.", isolate)
  
  if (args.Length() == 2) {  // Run call back function.
    Local<Function> cb = Local<Function>::Cast(args[1]);
    Local<Value> argv[] = { };
    cb->Call(isolate->GetCurrentContext()->Global(), 0, argv);
  }
}

void DbConn::BindParam(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  SQLRETURN rc;
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "Function prepare() must be called before binding parameters.", isolate)
  CHECK(args.Length() != 1 && args.Length() != 2, INVALID_PARAM_NUM, "The bind() method accept only one or two parameters.", isolate)
  CHECK(!args[0]->IsArray(), INVALID_PARAM_TYPE, "Function bind(): Bad parameters.", isolate)

  Handle<Array> params = Handle<Array>::Cast(args[0]);
  Handle<Array> object;
  SQLSMALLINT inOutType;
  int bindIndicator;
  
  SQLSMALLINT dataType;
  SQLINTEGER paramSize;
  SQLSMALLINT decDigits;
  SQLSMALLINT nullable;
  SQLINTEGER paramLen;
  SQLPOINTER buf;
  
  obj->freeSp();
  for(SQLSMALLINT i = 0; i < params->Length(); i++) {
    object = Handle<Array>::Cast(params->Get(i));  //Get a  ? parameter from the array.
    inOutType = object->Get(1)->Int32Value();  //Get the parameter In/Out type.
    bindIndicator = object->Get(2)->Int32Value();  //Get the parameter indicator(Null Terminated String or Length defined).
    rc = SQLDescribeParam(obj->stmth, i + 1, &dataType, &paramSize, &decDigits, &nullable);
    if(rc != SQL_SUCCESS)
      obj->throwErrMsg(SQL_ERROR, "SQLDescribeParam() failed.", isolate);
    if(bindIndicator == 0 || bindIndicator == 1) { //Parameter is string 
      String::Utf8Value param(object->Get(0));
      if(inOutType == SQL_PARAM_INPUT && obj->spInCount < SP_PARAM_MAX) {  //It is an input parameter.
        buf = obj->spIn[obj->spInCount] = strdup(*param); //Get the parameter string value.
        obj->spInCount++;
        if(bindIndicator == 0) //CLOB
          obj->indicator[i] = paramLen = strlen(*param);
        else  if(bindIndicator == 1) {//NTS
          paramLen = 0;
          obj->indicator[i] = SQL_NTS;
        }
      }
      else if(inOutType == SQL_PARAM_OUTPUT && obj->spOutCount < SP_PARAM_MAX) {  //It is an output parameter.
        buf = obj->spOut[obj->spOutCount] = (char*)calloc(paramSize + 1, sizeof(char));
        obj->spOutCount++;
        obj->indicator[i] = paramLen = paramSize;
      }
      DEBUG("Bind Parameter [%d] = %s \n", i + 1, buf)
      rc = SQLBindParameter(obj->stmth, i + 1, inOutType, SQL_C_CHAR, dataType, paramSize, decDigits, buf, paramLen, &obj->indicator[i]);
    }
    else if(bindIndicator == 2) { //Parameter is integer
      if(inOutType == SQL_PARAM_INPUT && obj->spInNumCount < SP_PARAM_MAX) {  //It is an input parameter.
        obj->spInNum[obj->spInNumCount] = object->Get(0)->Int32Value(); //Get the parameter integer value.
        buf = &obj->spInNum[obj->spInNumCount];
        obj->spInNumCount++;
      }
      else if(inOutType == SQL_PARAM_OUTPUT && obj->spOutNumCount < SP_PARAM_MAX) {  //It is an output parameter.
        obj->spOutNum[obj->spOutNumCount] = object->Get(0)->Int32Value(); //Get the parameter integer value.
        buf = &obj->spOutNum[obj->spOutNumCount];
        obj->spOutNumCount++;
      }
      DEBUG("Bind Parameter [%d] = %d \n", i + 1, *(int*)buf)
      rc = SQLBindParameter(obj->stmth, i + 1, inOutType, SQL_C_LONG, dataType, paramSize, decDigits, buf, 0, NULL);
    }
    else if(bindIndicator == 3) { //Parameter is NULL
      SQLINTEGER nullLen = SQL_NULL_DATA;
      DEBUG("Bind Parameter [%d] = NULL \n", i + 1)
      rc = SQLBindParameter(obj->stmth, i + 1, SQL_PARAM_INPUT, SQL_C_DEFAULT, dataType, paramSize, decDigits, buf, 0, &nullLen);
    }
  }
  if (args.Length() == 2) {  // Run call back function.
    Local<Function> cb = Local<Function>::Cast(args[1]);
    cb->Call(isolate->GetCurrentContext()->Global(), 0, 0);
  }
}

void DbConn::Execute(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  SQLRETURN rc = SQLExecute(obj->stmth);
  DEBUG("SQLExecute() rc=%d.\n", rc);
  if(rc != SQL_SUCCESS) {
    obj->throwErrMsg(SQL_HANDLE_STMT, isolate);
    return;
  }
  
  if (args.Length() == 1 && args[0]->IsFunction() && obj->spOutCount > 0 ) {
    // executeAsync(function(array){...})
    Handle<Array> array = Array::New(isolate);
    Local<Function> cb = Local<Function>::Cast(args[args.Length() - 1]);
    const unsigned argc = 1;
    for(int i = 0; i < obj->spOutCount; i++)
      array->Set(i, String::NewFromUtf8(isolate, obj->spOut[i])); 
    Local<Value> argv[argc] = { Local<Value>::New(isolate, array) };
    cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
  }
  
  SQLINTEGER rowCount;
  rc = SQLRowCount (obj->stmth, &rowCount);
  DEBUG("SQLRowCount() rc=%d.\n", rc);
  // CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLRowCount() failed.", isolate)
  
  rc = SQLNumResultCols(obj->stmth, &obj->colCount);
  DEBUG("SQLNumResultCols() rc=%d.\n", rc);
  // CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLNumResultCols() failed.", isolate)
  /* determine statement type */
  if (obj->colCount == 0) /* statement is not a select statement */
    // args.GetReturnValue().SetUndefined();  /* User can issue the numRows() API to get the affected rows number. */
    return;
  /* It is a select statement */
  obj->resultSetAvailable = true;
  obj->allocColRow();

  for(int i = 0; i < obj->colCount; i++)
  {
    SQLINTEGER rlength = 0;
    rc = SQLDescribeCol(obj->stmth, i + 1, //Column index starts from 1.
          obj->dbColumn[i].name,  //the buffer to store the Column Name
          MAX_COLNAME_WIDTH, //the size of the store buffer 
          &obj->dbColumn[i].nameLength,  //the accute length of the Column Name
          &obj->dbColumn[i].sqlType,  //the SQL type of the Column
          &obj->dbColumn[i].colPrecise, &obj->dbColumn[i].colScale, &obj->dbColumn[i].colNull);
    DEBUG("SQLDescribeCol(%d) rc=%d.\n", i, rc);
    if(rc != SQL_SUCCESS) {
      obj->freeColRow();
      obj->throwErrMsg(SQL_ERROR, "SQLDescribeCol() failed.", isolate);
      return;
    }
    
    rc = SQLBindCol(obj->stmth, i + 1, SQL_CHAR, (SQLPOINTER)obj->rowData[i], MAX_COL_WIDTH, &rlength);
    DEBUG("SQLBindCol(%d) rc=%d.\n", i, rc);
    if(rc != SQL_SUCCESS) {
      obj->freeColRow();
      obj->throwErrMsg(SQL_ERROR, "SQLBindCol() failed.", isolate);
      return;
    }
  }
  
  if (args.Length() == 1 && args[0]->IsFunction() && obj->spOutCount == 0 ) {
    // executeSync(function(){fetch()...})
    Local<Function> cb = Local<Function>::Cast(args[args.Length() - 1]);
    cb->Call(isolate->GetCurrentContext()->Global(), 0, 0);
  }
}

void DbConn::NextResult(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  
  SQLRETURN rc = SQLCloseCursor(obj->stmth);
  CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLCloseCursor() failed.", isolate)
  
  rc = SQLMoreResults(obj->stmth);
  CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLMoreResults() failed.", isolate)
  CHECK(rc == SQL_NO_DATA_FOUND, SQL_ERROR, "No more result set available.", isolate)
  
  rc = SQLNumResultCols(obj->stmth, &obj->colCount);
  CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLNumResultCols() failed.", isolate)

  /* determine statement type */
  if (obj->colCount == 0) /* statement is not a select statement */
    args.GetReturnValue().SetUndefined();  /* User can issue numRows() to the get affected rows number. */

  /* It is a select statement */
  obj->resultSetAvailable = true;
  obj->allocColRow();

  for(int i = 0; i < obj->colCount; i++)
  {
    SQLINTEGER rlength = 0;
    rc = SQLDescribeCol(obj->stmth, i + 1, //Column index starts from 1.
          obj->dbColumn[i].name,  //the buffer to store the Column Name
          MAX_COLNAME_WIDTH, //the size of the store buffer 
          &obj->dbColumn[i].nameLength,  //the accute length of the Column Name
          &obj->dbColumn[i].sqlType,  //the SQL type of the Column
          &obj->dbColumn[i].colPrecise, &obj->dbColumn[i].colScale, &obj->dbColumn[i].colNull);
    if(rc != SQL_SUCCESS) {
      obj->freeColRow();
      obj->throwErrMsg(SQL_ERROR, "SQLDescribeCol() failed.", isolate);
      return;
    }
    
    rc = SQLBindCol(obj->stmth, i + 1, SQL_CHAR, (SQLPOINTER)obj->rowData[i], MAX_COL_WIDTH, &rlength);
    if(rc != SQL_SUCCESS) {
      obj->freeColRow();
      obj->throwErrMsg(SQL_ERROR, "SQLBindCol() failed.", isolate);
      return;
    }
  }
}

void DbConn::Fetch(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  SQLRETURN rc;
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", isolate)
  CHECK(obj->resultSetAvailable == false, RSSET_NOT_READY, "There is no result set to be queried. Please execute a SQL command first.", isolate);
  
  if (args.Length() > 1 && args[0]->IsInt32() && args[1]->IsInt32()) { 
    int orient = args[0]->Int32Value();
    int offset = args[1]->Int32Value();
    int retVal = 0;
    rc = SQLGetStmtAttr(obj->stmth, SQL_ATTR_CURSOR_SCROLLABLE, &retVal, 0, 0);
    if(retVal == SQL_TRUE) {
      rc = SQLFetchScroll(obj->stmth, orient, offset);
      DEBUG("SQLFetchScroll(orient = %d, offset = %d) rc=%d.\n", orient, offset, rc);
    }
    else
      obj->throwErrMsg(SQL_ERROR, "Cursor is not scrollable.", isolate);
  }
  else {
    rc = SQLFetch(obj->stmth);
    DEBUG("Fetch() rc=%d.\n", rc);
  }
  if(rc == SQL_SUCCESS)
  {
    Handle<Object> row = Object::New(isolate);
    for(int i = 0; i < obj->colCount; i++)
    {
      Local<Value> value = Local<Value>::New(isolate, String::NewFromUtf8(isolate, obj->rowData[i]));
      row->Set(String::NewFromUtf8(isolate, (char const*)obj->dbColumn[i].name), value);
    }
    if (args.Length() == 1 ||  args.Length() == 3) {  // Run call back to handle the fetched row.
      Local<Function> cb = Local<Function>::Cast(args[args.Length() - 1]);
      const unsigned argc = 1;
      Local<Value> argv[argc] = { Local<Value>::New(isolate, row) };
      cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
    }
    else
      RETURN(row)
  }
  if(rc == SQL_ERROR)  
    obj->throwErrMsg(SQL_HANDLE_STMT, isolate);
  RETURN(Number::New(isolate, rc))  // SQL_NO_DATA_FOUND indicate the end of the result set. This is not an error. 
}

void DbConn::FetchAll(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  
  DEBUG("FetchAll().\n");

  SQLRETURN rc;
  int arrayCount = 0;  
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", isolate)
  CHECK(obj->resultSetAvailable == false, RSSET_NOT_READY, "There is no result set to be queried. Please execute a SQL command first.", isolate);
  
  Handle<Array> array = Array::New(isolate);
  while((rc = SQLFetch(obj->stmth)) == SQL_SUCCESS) 
  {
    Handle<Object> row = Object::New(isolate);
    for(int i = 0; i < obj->colCount; i++)
    {
      Local<Value> value = Local<Value>::New(isolate, String::NewFromUtf8(isolate, obj->rowData[i]));
      row->Set(String::NewFromUtf8(isolate, (char const*)obj->dbColumn[i].name), value);
    }
    array->Set(arrayCount++, row);  //Build the JSON data
  }
  if (rc != SQL_NO_DATA_FOUND) {
    obj->freeColRow();
    obj->throwErrMsg(SQL_ERROR, "SQLFetch() failed.", isolate);
  }
  if (args.Length() == 1) {
    Local<Function> cb = Local<Function>::Cast(args[0]);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { Local<Value>::New(isolate, array) };
    cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
  }
}

void DbConn::Commit(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", isolate)

  SQLRETURN rc = SQLTransact(obj->envh, obj->connh, SQL_COMMIT);
  CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLTransact(Commit) failed.", isolate)
}

void DbConn::Rollback(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", isolate)
  SQLRETURN rc = SQLTransact(obj->envh, obj->connh, SQL_ROLLBACK);
  CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLTransact(Rollback) failed.", isolate)
}

void DbConn::NumFields(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  SQLSMALLINT colCount;
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", isolate)
  CHECK(obj->resultSetAvailable == false, RSSET_NOT_READY, "There is no result set to be queried. Please execute a SQL command first.", isolate);

  SQLRETURN rc = SQLNumResultCols(obj->stmth, &colCount);
  if(rc != SQL_SUCCESS) {
    obj->throwErrMsg(SQL_ERROR, "SQLNumResultCols() failed.", isolate);
  }
  RETURN(Number::New(isolate, colCount))
}

void DbConn::NumRows(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", isolate)
  SQLINTEGER rowCount;
  if(SQLRowCount (obj->stmth, &rowCount) != SQL_SUCCESS) {
    obj->throwErrMsg(SQL_ERROR, "SQLRowCount() failed.", isolate);
  }
  RETURN(Number::New(isolate, rowCount))
}

void DbConn::FieldType(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The fieldType() method only accept one parameter.", isolate)
  CHECK(obj->resultSetAvailable == false || obj->colRowAllocated == false, RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", isolate)

  SQLSMALLINT i = args[0]->Int32Value();
  CHECK(i >= obj->colCount || i < 0, INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", isolate)
  RETURN(Number::New(isolate, obj->dbColumn[i].sqlType))
}

void DbConn::FieldWidth(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The fieldWidth() method only accept one parameter.", isolate)

  CHECK(obj->resultSetAvailable == false || obj->colRowAllocated == false, RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", isolate)

  SQLSMALLINT i = args[0]->Int32Value();
  CHECK(i >= obj->colCount || i < 0, INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", isolate)
  RETURN(Number::New(isolate, obj->dbColumn[i].nameLength))
}

void DbConn::FieldName(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The fieldName() method only accept one parameter.", isolate)

  CHECK(obj->resultSetAvailable == false || obj->colRowAllocated == false, RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", isolate)

  SQLSMALLINT i = args[0]->Int32Value();
  CHECK(i >= obj->colCount || i < 0, INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", isolate)
  
  RETURN(String::NewFromUtf8(isolate, obj->dbColumn[i].name))
}

void DbConn::FieldPrecise(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The fieldPrecise() method only accept one parameter.", isolate)
  CHECK(obj->resultSetAvailable == false || obj->colRowAllocated == false, RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", isolate)

  SQLSMALLINT i = args[0]->Int32Value();
  CHECK(i >= obj->colCount || i < 0, INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", isolate)

  RETURN(Number::New(isolate, obj->dbColumn[i].colPrecise))
}

void DbConn::FieldScale(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The fieldScale() method only accept one parameter.", isolate)
  CHECK(obj->resultSetAvailable == false || obj->colRowAllocated == false, RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", isolate)

  SQLSMALLINT i = args[0]->Int32Value();
  CHECK(i >= obj->colCount || i < 0, INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", isolate)
  
  RETURN(Number::New(isolate, obj->dbColumn[i].colScale))
}

void DbConn::FieldNullable(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The fieldNullable() method only accept one parameter.", isolate)
  CHECK(obj->resultSetAvailable == false || obj->colRowAllocated == false, RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", isolate)

  SQLSMALLINT i = args[0]->Int32Value();
  CHECK(i >= obj->colCount || i < 0, INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", isolate)
  
  RETURN(Number::New(isolate, obj->dbColumn[i].colNull))
}

void DbConn::StmtError(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", isolate)
  CHECK(args.Length() < 2, INVALID_PARAM_NUM, "The stmtError() method only accept at lease two parameters.", isolate)

  SQLSMALLINT hType = args[0]->Int32Value();
  SQLSMALLINT recno = args[1]->Int32Value();
  SQLINTEGER handle;
  
  switch(hType)
  {
    case SQL_HANDLE_ENV:
      handle = obj->envh;
      break;
    case SQL_HANDLE_DBC:
      handle = obj->connh;
      break;
    case SQL_HANDLE_STMT:
      handle = obj->stmth;
      break;
  }

  SQLCHAR msg[SQL_MAX_MESSAGE_LENGTH + 1];
  SQLCHAR sqlstate[SQL_SQLSTATE_SIZE + 1];
  SQLCHAR errMsg[SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10];
  SQLINTEGER sqlcode = 0;
  SQLSMALLINT length = 0;
  SQLCHAR *p = NULL;

  memset(msg, '\0', SQL_MAX_MESSAGE_LENGTH + 1);
  memset(sqlstate, '\0', SQL_SQLSTATE_SIZE + 1);
  memset(errMsg, '\0', SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10);

  SQLRETURN rc = SQLGetDiagRec(hType, handle, recno, sqlstate, &sqlcode, msg, SQL_MAX_MESSAGE_LENGTH + 1, &length);  
  if (rc == SQL_SUCCESS) {
    if (msg[length-1] == '\n') {
      p = &msg[length-1];
      *p = '\0';
    }
    sprintf((char *)errMsg, "SQLSTATE=%s SQLCODE=%d %s", sqlstate, (int)sqlcode, msg);
  }
  if(args.Length() == 3)
  {
    Local<Function> cb = Local<Function>::Cast(args[args.Length() - 1]);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { Local<Value>::New(isolate, String::NewFromUtf8(isolate, errMsg)) };
    cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
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

void DbConn::RunXS(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbConn* obj = ObjectWrap::Unwrap<DbConn>(args.Holder());
  CHECK(obj->stmtAllocated != true, STMT_NOT_READY, "runXS(): The SQL Statment handler is not initialized.", isolate)
  CHECK(args.Length() != 4 && args.Length() != 5, INVALID_PARAM_NUM, "The runXS() method accept only 4 or 5 parameter.", isolate)
  CHECK(!args[0]->IsString() || !args[1]->IsString() || !args[2]->IsString() || !args[3]->IsString(), INVALID_PARAM_TYPE, "runXS(): Bad parameters.", isolate)

  String::Utf8Value sql(args[0]);
  String::Utf8Value ipc(args[1]);
  String::Utf8Value ctl(args[2]);
  String::Utf8Value xmlIn(args[3]);
  
  DEBUG("SQL: %s\n", *sql)
  SQLRETURN rc = SQLPrepare(obj->stmth, *sql, strlen(*sql));
  CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLPrepare() failed.", isolate)
  
  SQLSMALLINT dataType;
  SQLINTEGER parameterSize;
  SQLSMALLINT decDigits;
  SQLSMALLINT nullable;
  SQLINTEGER indicator = SQL_NTS;
  SQLINTEGER xmlInLen = strlen(*xmlIn);
  
  rc = SQLDescribeParam(obj->stmth, 1, &dataType, &parameterSize, &decDigits, &nullable);
  rc = SQLBindParameter(obj->stmth, 1, SQL_PARAM_INPUT, SQL_C_CHAR, dataType, parameterSize, decDigits, *ipc, 0, &indicator);
  DEBUG("Bind an parameter [%d] %s | %d | %d | %d | %d | %d.\n", 1, *ipc, dataType, parameterSize, decDigits, 0, indicator)
  rc = SQLDescribeParam(obj->stmth, 2, &dataType, &parameterSize, &decDigits, &nullable);
  rc = SQLBindParameter(obj->stmth, 2, SQL_PARAM_INPUT, SQL_C_CHAR, dataType, parameterSize, decDigits, *ctl, 0, &indicator);
  DEBUG("Bind an parameter [%d] %s | %d | %d | %d | %d | %d.\n", 2, *ctl, dataType, parameterSize, decDigits, 0, indicator)
  rc = SQLDescribeParam(obj->stmth, 3, &dataType, &parameterSize, &decDigits, &nullable);
  rc = SQLBindParameter(obj->stmth, 3, SQL_PARAM_INPUT, SQL_C_CHAR, dataType, parameterSize, decDigits, *xmlIn, xmlInLen, &xmlInLen);
  DEBUG("Bind an parameter [%d] %s | %d | %d | %d | %d | %d.\n", 3, *xmlIn, dataType, parameterSize, decDigits, xmlInLen, xmlInLen)
  rc = SQLDescribeParam(obj->stmth, 4, &dataType, &parameterSize, &decDigits, &nullable);
  obj->xmlOut = (char*)calloc(parameterSize + 1, sizeof(char));
  rc = SQLBindParameter(obj->stmth, 4, SQL_PARAM_OUTPUT, SQL_C_CHAR, dataType, parameterSize, decDigits, obj->xmlOut, parameterSize, &parameterSize);
  DEBUG("Bind an parameter [%d] | %d | %d | %d | %d | %d.\n", 4, dataType, parameterSize, decDigits, parameterSize, parameterSize)
  rc = SQLExecute(obj->stmth);
  CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLExecute() failed.", isolate)

  DEBUG("%s\n", obj->xmlOut);
  if (args.Length() == 5 ) {
    Local<Function> cb = Local<Function>::Cast(args[args.Length() - 1]);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { Local<Value>::New(isolate, String::NewFromUtf8(isolate, obj->xmlOut)) };
    cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
  }
  free(obj->xmlOut);
}
