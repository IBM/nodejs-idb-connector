/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */
 
#include "dbstmt.h"

using namespace v8;

Persistent<Function> DbStmt::constructor;
SQLHENV DbStmt::envh;

struct CallBackData {
  Persistent<Function> callback;
  int arglength;
  SQLCHAR* sqlSt;
  int arg[10];
  DbStmt* obj;
  Persistent<Array> params;
  int rc;
};

DbStmt::DbStmt(DbConn* conn) {
  SQLRETURN rc;
  if(conn->connected)
    rc = SQLAllocStmt(conn->connh, &stmth);
  else
    return;
  if(rc != SQL_SUCCESS) {
    SQLFreeStmt( stmth, SQL_CLOSE );
    // obj->throwErrMsg(SQL_HANDLE_DBC, isolate);
    return;
  } 
  isDebug = conn->isDebug;
  envh = conn->envh;
  connh = conn->connh;
  con = conn;
  stmtAllocated = true;  // Any SQL Statement Handler processing can not be allowed before this.
}  

DbStmt::~DbStmt() {}

void DbStmt::Init() {
  Isolate* isolate = Isolate::GetCurrent();
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "dbstmt"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1); 

  NODE_SET_PROTOTYPE_METHOD(tpl, "setStmtAttr", SetStmtAttr);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getStmtAttr", GetStmtAttr);

  NODE_SET_PROTOTYPE_METHOD(tpl, "exec", ExecAsync);
  NODE_SET_PROTOTYPE_METHOD(tpl, "execSync", Exec);

  NODE_SET_PROTOTYPE_METHOD(tpl, "prepare", PrepareAsync);  
  NODE_SET_PROTOTYPE_METHOD(tpl, "prepareSync", Prepare);

  NODE_SET_PROTOTYPE_METHOD(tpl, "bindParam", BindParamAsync);
  NODE_SET_PROTOTYPE_METHOD(tpl, "bindParamSync", BindParam);
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "execute", ExecuteAsync);
  NODE_SET_PROTOTYPE_METHOD(tpl, "executeSync", Execute);
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "nextResult", NextResult);
  NODE_SET_PROTOTYPE_METHOD(tpl, "closeCursor", CloseCursor);

  NODE_SET_PROTOTYPE_METHOD(tpl, "fetch", FetchAsync);
  NODE_SET_PROTOTYPE_METHOD(tpl, "fetchSync", Fetch);
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "fetchAll", FetchAllAsync);
  NODE_SET_PROTOTYPE_METHOD(tpl, "fetchAllSync", FetchAll);
    
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
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "stmtError", StmtError);
  
  NODE_SET_PROTOTYPE_METHOD(tpl, "close", Close);
  
  constructor.Reset(isolate, tpl->GetFunction());
}

//new db2i() <==> new DbStmt()
void DbStmt::New(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  if (args.IsConstructCall()) {
    // Invoked as constructor: `new db2i(...)`
    DbConn* conn = node::ObjectWrap::Unwrap<DbConn>(args[0]->ToObject());
    DbStmt* obj = new DbStmt(conn);
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

void DbStmt::NewInstance(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);

  const unsigned argc = 1;
  Handle<Value> argv[argc] = { args[0] };
  Local<Function> cons = Local<Function>::New(isolate, constructor);
  Local<Object> instance = cons->NewInstance(argc, argv);

  RETURN(instance)
}

void DbStmt::SetStmtAttr(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());

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

void DbStmt::GetStmtAttr(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  
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

void DbStmt::Exec(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  
  DEBUG("Exec().\n");
  CHECK(args.Length() != 1 && args.Length() != 2, INVALID_PARAM_NUM, "The execSync() method accept only one or two parameter.", isolate)
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", isolate);

  int arrayCount = 0;  
  String::Utf8Value arg0(args[0]);
  SQLCHAR* tmpSqlSt = *arg0;

  SQLRETURN rc = SQLExecDirect(obj->stmth, tmpSqlSt, SQL_NTS);
  DEBUG("SQLExecDirect(%d): %s\n", rc, tmpSqlSt);  
  if(rc != SQL_SUCCESS) {
    if(rc == SQL_NO_DATA_FOUND)
      args.GetReturnValue().SetUndefined(); //SQL statement is a Searched UPDATE/DELETE and no rows satisfy the search condition
    else {
      obj->throwErrMsg(SQL_HANDLE_STMT, isolate);
      return;
    }
  }

  SQLNumResultCols(obj->stmth, &obj->colCount);

  Handle<Array> array = Array::New(isolate);
  
  /* determine statement type */
  if (obj->colCount > 0) { /* statement is a select statement */
    obj->resultSetAvailable = true;
    if(obj->bindColData(isolate) < 0) return;
    
    while((rc = SQLFetch(obj->stmth)) == SQL_SUCCESS) {
      Handle<Object> row = Object::New(isolate);
      for(int i = 0; i < obj->colCount; i++) {
        Local<Value> value = Local<Value>::New(isolate, String::NewFromUtf8(isolate, obj->rowData[i]));
        row->Set(String::NewFromUtf8(isolate, (char const*)obj->dbColumn[i].name), value);
      }
      array->Set(arrayCount++, row);  //Build the JSON data
    }
    if (rc != SQL_NO_DATA_FOUND) {
      obj->freeCol();
      obj->throwErrMsg(SQL_ERROR, "SQLFetch() failed.", isolate);
    }
  }

  if (args.Length() >= 2) {
    Local<Function> cb = Local<Function>::Cast(args[args.Length() - 1]);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { Local<Value>::New(isolate, array) };
    cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
  }
  obj->freeCol();
  
  // DEBUG("SQLCloseCursor: stmth %d\n", obj->stmth)
  // rc = SQLCloseCursor(obj->stmth);
}

void DbStmt::ExecAsync(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate();
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  
  DEBUG("ExecAsync().\n");
  CHECK(args.Length() != 2, INVALID_PARAM_NUM, "The exec() method accept only two parameters.", isolate)
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", isolate);
  
  String::Utf8Value arg0(args[0]);

  CallBackData* cbd = new CallBackData;
  cbd->obj = obj;
  cbd->arglength = args.Length();
  cbd->sqlSt = strdup(*arg0);
  cbd->callback.Reset(isolate, Handle<Function>::Cast(args[1]));
  cbd->rc = 0;
  
  uv_work_t* request = new uv_work_t;
  request->data = cbd;
  
  uv_queue_work(uv_default_loop(), request, ExecAsyncRun, ExecAsyncAfter);
}

void DbStmt::ExecAsyncRun(uv_work_t *req) {
  INITASYNC
  
  cbd->rc = SQLExecDirect(obj->stmth, cbd->sqlSt, SQL_NTS);
  DEBUG("SQLExecDirect(%d): %s\n", cbd->rc, cbd->sqlSt);  
  LOG(cbd->rc != SQL_SUCCESS && cbd->rc != SQL_NO_DATA_FOUND)
  
  SQLNumResultCols(obj->stmth, &obj->colCount);
  
  if (obj->colCount == 0) /* statement is not a select statement */
    return;
    
  obj->resultSetAvailable = true;
  if(obj->bindColData(NULL) < 0) return;
  
  while((cbd->rc = SQLFetch(obj->stmth)) == SQL_SUCCESS) 
  {
    SQLCHAR** rowData = (SQLCHAR**)calloc(obj->colCount, sizeof(SQLCHAR*)); 
    for(int i = 0; i < obj->colCount; i++)
    {
      int colLen = strlen(obj->rowData[i]) + 1;
      rowData[i] = (SQLCHAR*)calloc(colLen, sizeof(SQLCHAR));
      memcpy(rowData[i], obj->rowData[i], colLen * sizeof(SQLCHAR));
    }
    obj->result.push_back(rowData);
  }
  LOG(cbd->rc != SQL_NO_DATA_FOUND)
}

void DbStmt::ExecAsyncAfter(uv_work_t *req, int status) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  CallBackData* cbd = reinterpret_cast<CallBackData*>(req->data);
  DbStmt* obj = cbd->obj;
  
  Handle<Array> array = Array::New(isolate);
  for(int i = 0; i < obj->result.size(); i++)
  {
    Handle<Object> row = Object::New(isolate);
    for(int j = 0; j < obj->colCount; j++)
    {
      Local<Value> value = Local<Value>::New(isolate, String::NewFromUtf8(isolate, obj->result[i][j]));
      row->Set(String::NewFromUtf8(isolate, (char const*)obj->dbColumn[j].name), value);
      free(obj->result[i][j]);
    }
    array->Set(i, row);  //Build the JSON data
    free(obj->result[i]);
  }
  obj->result.clear();
  
  if (cbd->arglength == 2) {
    Local<Function> cb = Local<Function>::New(isolate, cbd->callback);
    if(strlen(obj->msg) > 0){
      Local<Value> argv[2] = { 
        Local<Value>::New(isolate, array),
        Local<Value>::New(isolate, String::NewFromUtf8(isolate, obj->msg))
      };
      cb->Call(isolate->GetCurrentContext()->Global(), 2, argv);
    } else {
      Local<Value> argv[1] = { Local<Value>::New(isolate, array) };
      cb->Call(isolate->GetCurrentContext()->Global(), 1, argv);
    }
  }
  
  obj->freeCol();
  
  // DEBUG("SQLCloseCursor: stmth %d\n", obj->stmth)
  // SQLCloseCursor(obj->stmth);
  
  delete cbd->sqlSt;
  delete cbd;
  delete req;
}

void DbStmt::CloseCursor(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  
  DEBUG("SQLCloseCursor: stmth %d\n", obj->stmth)
  SQLCloseCursor(obj->stmth);
}

void DbStmt::Close(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  
  obj->freeCol();
  DEBUG("SQLFreeStmt: stmth %d\n", obj->stmth)
  if(obj->stmtAllocated) {
    SQLFreeStmt(obj->stmth, SQL_DROP);  //Free the statement handle here. No further processing allowed.
    //CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLFreeStmt() failed.", isolate)
    obj->stmtAllocated = false;  // Any SQL Statement Handler processing can not be allowed after this.
    obj->resultSetAvailable = false;
  }
}

void DbStmt::Prepare(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  
  DEBUG("Prepare().\n");
  CHECK(args.Length() != 1 && args.Length() != 2, INVALID_PARAM_NUM, "The prepareSync() method accepts one or two parameters.", isolate)
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", isolate)
  
  String::Utf8Value sql(args[0]);
  SQLRETURN rc = SQLPrepare(obj->stmth, *sql, strlen(*sql));
  DEBUG("SQLPrepare(%d): %s\n", rc, *sql)
  CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLPrepare() failed.", isolate)
  
  if (args.Length() == 2) {  // Run call back function.
    Local<Function> cb = Local<Function>::Cast(args[1]);
    Local<Value> argv[] = { };
    cb->Call(isolate->GetCurrentContext()->Global(), 0, argv);
  }
}

void DbStmt::PrepareAsync(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
	DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());

  DEBUG("PrepareAsync().\n");
  CHECK(args.Length() < 2, INVALID_PARAM_NUM, "The prepare() method accept only two parameter.", isolate)
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statment handler is not initialized.", isolate)
  
  String::Utf8Value sql(args[0]);
  
  
  CallBackData* cbd = new CallBackData;
  cbd->obj = obj;
  cbd->arglength = args.Length();
  cbd->sqlSt = strdup(*sql);
  cbd->callback.Reset(isolate, Handle<Function>::Cast(args[args.Length() - 1]));
  
  uv_work_t* request = new uv_work_t;
  request->data = cbd;
  
  uv_queue_work(uv_default_loop(), request, PrepareAsyncRun, PrepareAsyncAfter);
}

void DbStmt::PrepareAsyncRun(uv_work_t *req) {
  INITASYNC
  cbd->rc = SQLPrepare(cbd->obj->stmth, cbd->sqlSt, strlen(cbd->sqlSt));
  DEBUG("SQLPrepare(%d): %s\n", cbd->rc, cbd->sqlSt)
  LOG(cbd->rc != SQL_SUCCESS)
}

void DbStmt::PrepareAsyncAfter(uv_work_t *req, int status) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  CallBackData* cbd = reinterpret_cast<CallBackData*>(req->data);
  DbStmt* obj = cbd->obj;
  
  Local<Function> cb = Local<Function>::New(isolate, cbd->callback);
  
  if(strlen(obj->msg) > 0){
    Local<Value> argv[1] = { 
      Local<Value>::New(isolate, String::NewFromUtf8(isolate, obj->msg))
    };
    cb->Call(isolate->GetCurrentContext()->Global(), 1, argv);
  } else {
    cb->Call(isolate->GetCurrentContext()->Global(), 0, 0);
  }
  
  delete cbd->sqlSt;
  delete cbd;
  delete req;
}

void DbStmt::BindParam(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  
  DEBUG("BindParam().\n");
  SQLRETURN rc;
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "Function bindParamSync() must be called before binding parameters.", isolate)
  CHECK(args.Length() != 1 && args.Length() != 2, INVALID_PARAM_NUM, "The bindParamSync() method accept only one or two parameters.", isolate)
  CHECK(!args[0]->IsArray(), INVALID_PARAM_TYPE, "Function bindParamSync(): Bad parameters.", isolate)

  Handle<Array> params = Handle<Array>::Cast(args[0]);
  Handle<Array> object;
  int bindIndicator;
  
  obj->freeSp();
  
  /*
  SQLBindParameter(SQLHSTMT StatementHandle,
    SQLSMALLINT ParameterNumber,  // i + 1
    SQLSMALLINT InputOutputType,  // In=1;Out=2;InOut=3
    SQLSMALLINT ValueType,  // C data type, SQL_C_DEFAULT: default => ParameterType
    SQLSMALLINT ParameterType,  // SQL data type
    SQLINTEGER ColumnSize,  // Precision of the ?
    SQLSMALLINT DecimalDigits,  // Scale of the ?
    SQLPOINTER ParameterValuePtr, // Input/Output buffer
    SQLINTEGER BufferLength,  // Not used.
    SQLINTEGER *StrLen_or_IndPtr); // For I/IO, *length of ParameterValuePtr. For O, 
  */
  obj->paramCount = params->Length();
  obj->param = (db2_param*)calloc(obj->paramCount, sizeof(db2_param));

  for(SQLSMALLINT i = 0; i < obj->paramCount; i++) {
    object = Handle<Array>::Cast(params->Get(i));  //Get a  ? parameter from the array.
    obj->param[i].io = object->Get(1)->Int32Value();  //Get the parameter In/Out type.
    bindIndicator = object->Get(2)->Int32Value();  //Get the indicator(str/int).
    
    rc = SQLDescribeParam(obj->stmth, i + 1, 
                        &obj->param[i].dataType, 
                        &obj->param[i].paramSize, 
                        &obj->param[i].decDigits, 
                        &obj->param[i].nullable);
    if(rc != SQL_SUCCESS)
      obj->throwErrMsg(SQL_ERROR, "SQLDescribeParam() failed.", isolate);

    int bufSize = obj->param[i].paramSize << 2;
    
    if(bindIndicator == 0 || bindIndicator == 1) { //Parameter is string 
      String::Utf8Value param(object->Get(0));
      obj->param[i].jsType = 1;  // String
      obj->param[i].buf = (char*)calloc(bufSize, sizeof(char));
      
      if(obj->param[i].io == SQL_PARAM_INPUT || obj->param[i].io == SQL_PARAM_INPUT_OUTPUT) {
        memcpy(obj->param[i].buf, *param, strlen(*param) < bufSize ? strlen(*param) : bufSize);
        if(bindIndicator == 0) //CLOB
          obj->param[i].ind = strlen(*param);
        else if(bindIndicator == 1) //NTS
          obj->param[i].ind = SQL_NTS;
      }
      else if(obj->param[i].io == SQL_PARAM_OUTPUT)
        obj->param[i].ind = bufSize;
      
      DEBUG("SQLBindParameter [%d] = %s \n", i + 1, obj->param[i].buf)
      rc = SQLBindParameter(obj->stmth, i + 1, obj->param[i].io, SQL_C_CHAR, obj->param[i].dataType, obj->param[i].paramSize, obj->param[i].decDigits, obj->param[i].buf, 0, &obj->param[i].ind);
    }
    else if(bindIndicator == 2) { //Parameter is integer
      obj->param[i].jsType = 2;  // Number
      obj->param[i].buf = (long*)calloc(1, sizeof(long));
      if(obj->param[i].io == SQL_PARAM_INPUT || obj->param[i].io == SQL_PARAM_INPUT_OUTPUT)
        *(long*)obj->param[i].buf = object->Get(0)->Int32Value();
      DEBUG("SQLBindParameter [%d] = %d \n", i + 1, *(long*)obj->param[i].buf)
      rc = SQLBindParameter(obj->stmth, i + 1, obj->param[i].io, SQL_C_LONG, obj->param[i].dataType, obj->param[i].paramSize, obj->param[i].decDigits, obj->param[i].buf, 0, NULL);
    }
    else if(bindIndicator == 3) { //Parameter is NULL
      SQLINTEGER nullLen = SQL_NULL_DATA;
      DEBUG("SQLBindParameter [%d] = NULL \n", i + 1)
      rc = SQLBindParameter(obj->stmth, i + 1, SQL_PARAM_INPUT, SQL_C_DEFAULT, obj->param[i].dataType, obj->param[i].paramSize, obj->param[i].decDigits, obj->param[i].buf, 0, &nullLen);
    }
  }
  obj->printParam();
  if (args.Length() == 2) {  // Run call back function.
    Local<Function> cb = Local<Function>::Cast(args[1]);
    cb->Call(isolate->GetCurrentContext()->Global(), 0, 0);
  }
}

void DbStmt::BindParamAsync(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  
  DEBUG("BindParamAsync().\n");
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "Function bindParam() must be called before binding parameters.", isolate)
  CHECK(args.Length() != 2, INVALID_PARAM_NUM, "The bindParam() method accept only two parameters.", isolate)
  CHECK(!args[0]->IsArray() && !args[1]->IsFunction(), INVALID_PARAM_TYPE, "Function bindParam(): Bad parameters.", isolate);
  
  CallBackData* cbd = new CallBackData;
  cbd->obj = obj;
  cbd->arglength = args.Length();
  cbd->params.Reset(isolate, Handle<Array>::Cast(args[0]));
  cbd->callback.Reset(isolate, Handle<Function>::Cast(args[1]));

  uv_work_t* request = new uv_work_t;
  request->data = cbd;
  
  uv_queue_work(uv_default_loop(), request, nop, BindParamAsyncAfter);
}

void DbStmt::BindParamAsyncAfter(uv_work_t *req, int status) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  CallBackData* cbd = reinterpret_cast<CallBackData*>(req->data);
  DbStmt* obj = cbd->obj;
  
  Local<Array> params = Local<Array>::New(isolate, cbd->params);
  Handle<Array> object;
  
  int bindIndicator;
  SQLRETURN rc;

  obj->freeSp();
  /*
  SQLBindParameter(SQLHSTMT StatementHandle,
    SQLSMALLINT ParameterNumber,  // i + 1
    SQLSMALLINT InputOutputType,  // In=1;Out=2;InOut=3
    SQLSMALLINT ValueType,  // C data type, SQL_C_DEFAULT: default => ParameterType
    SQLSMALLINT ParameterType,  // SQL data type
    SQLINTEGER ColumnSize,  // Precision of the ?
    SQLSMALLINT DecimalDigits,  // Scale of the ?
    SQLPOINTER ParameterValuePtr, // Input/Output buffer
    SQLINTEGER BufferLength,  // Not used.
    SQLINTEGER *StrLen_or_IndPtr); // For I/IO, *length of ParameterValuePtr. For O, 
  */
  obj->paramCount = params->Length();
  obj->param = (db2_param*)calloc(obj->paramCount, sizeof(db2_param));

  for(SQLSMALLINT i = 0; i < obj->paramCount; i++) {
    object = Handle<Array>::Cast(params->Get(i));  //Get a  ? parameter from the array.
    obj->param[i].io = object->Get(1)->Int32Value();  //Get the parameter In/Out type.
    bindIndicator = object->Get(2)->Int32Value();  //Get the indicator(str/int).
    
    rc = SQLDescribeParam(obj->stmth, i + 1, 
                        &obj->param[i].dataType, 
                        &obj->param[i].paramSize, 
                        &obj->param[i].decDigits, 
                        &obj->param[i].nullable);
    if(rc != SQL_SUCCESS)
      obj->throwErrMsg(SQL_ERROR, "SQLDescribeParam() failed.", isolate);

    int bufSize = obj->param[i].paramSize << 2;
    
    if(bindIndicator == 0 || bindIndicator == 1) { //Parameter is string 
      String::Utf8Value param(object->Get(0));
      obj->param[i].jsType = 1;  // String
      obj->param[i].buf = (char*)calloc(bufSize, sizeof(char));
      
      if(obj->param[i].io == SQL_PARAM_INPUT || obj->param[i].io == SQL_PARAM_INPUT_OUTPUT) {
        memcpy(obj->param[i].buf, *param, strlen(*param) < bufSize ? strlen(*param) : bufSize);
        if(bindIndicator == 0) //CLOB
          obj->param[i].ind = strlen(*param);
        else if(bindIndicator == 1) //NTS
          obj->param[i].ind = SQL_NTS;
      }
      else if(obj->param[i].io == SQL_PARAM_OUTPUT)
        obj->param[i].ind = bufSize;
      
      DEBUG("SQLBindParameter [%d] = %s \n", i + 1, obj->param[i].buf)
      rc = SQLBindParameter(obj->stmth, i + 1, obj->param[i].io, SQL_C_CHAR, obj->param[i].dataType, obj->param[i].paramSize, obj->param[i].decDigits, obj->param[i].buf, 0, &obj->param[i].ind);
    }
    else if(bindIndicator == 2) { //Parameter is integer
      obj->param[i].jsType = 2;  // Number
      obj->param[i].buf = (long*)calloc(1, sizeof(long));
      if(obj->param[i].io == SQL_PARAM_INPUT || obj->param[i].io == SQL_PARAM_INPUT_OUTPUT)
        *(long*)obj->param[i].buf = object->Get(0)->Int32Value();
      DEBUG("SQLBindParameter [%d] = %d \n", i + 1, *(long*)obj->param[i].buf)
      rc = SQLBindParameter(obj->stmth, i + 1, obj->param[i].io, SQL_C_LONG, obj->param[i].dataType, obj->param[i].paramSize, obj->param[i].decDigits, obj->param[i].buf, 0, NULL);
    }
    else if(bindIndicator == 3) { //Parameter is NULL
      SQLINTEGER nullLen = SQL_NULL_DATA;
      DEBUG("SQLBindParameter [%d] = NULL \n", i + 1)
      rc = SQLBindParameter(obj->stmth, i + 1, SQL_PARAM_INPUT, SQL_C_DEFAULT, obj->param[i].dataType, obj->param[i].paramSize, obj->param[i].decDigits, obj->param[i].buf, 0, &nullLen);
    }
  }
  
  obj->printParam();
  
  if (cbd->arglength == 2) {
    const unsigned argc = 0;
    Local<Function> cb = Local<Function>::New(isolate, cbd->callback);
    Local<Value> argv[] = { };
    cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
  }
  
  delete cbd;
  delete req;
}

void DbStmt::Execute(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  
  DEBUG("Execute().\n");

  SQLRETURN rc = SQLExecute(obj->stmth);
  DEBUG("SQLExecute(%d):\n", rc);
  if(rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
    obj->throwErrMsg(SQL_HANDLE_STMT, isolate);
    return;
  }
    
  SQLNumResultCols(obj->stmth, &obj->colCount);
  /* determine statement type */
  if (obj->colCount > 0) {/* statement is a select statement */
    obj->resultSetAvailable = true;
    if(obj->bindColData(isolate) < 0) return;
  }
  /* execute(()=>{fetchAll(...)}) or */ 
  /* execute(out=>{console.log(out)}) or */
  /* execute(out=>{console.log(out); fetchAll(...)}) */
  if (args.Length() == 1 && args[0]->IsFunction()) {
    Handle<Array> array = Array::New(isolate);
    Local<Function> cb = Local<Function>::Cast(args[args.Length() - 1]);
    const unsigned argc = 1;
    for(int i = 0, j = 0; i < obj->paramCount; i++) {
      db2_param* param = &obj->param[i];
      if(param->io != SQL_PARAM_INPUT) {
        if(param->jsType == 1)  // String
          array->Set(j, String::NewFromUtf8(isolate, (char*)param->buf));
        else if(param->jsType == 2)  // Number
          array->Set(j, Integer::New(isolate, *(long*)param->buf));
        j++;
      }
    }
    obj->freeSp();
    Local<Value> argv[argc] = { Local<Value>::New(isolate, array) };
    cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
  }
}

void DbStmt::ExecuteAsync(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  
  DEBUG("ExecuteAsync().\n");
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The execute() method accept only one parameter.", isolate)
  CHECK(!args[0]->IsFunction(), INVALID_PARAM_TYPE, "Function execute(): Bad parameters.", isolate);
  
  CallBackData* cbd = new CallBackData;
  cbd->obj = obj;
  cbd->arglength = args.Length();
  cbd->callback.Reset(isolate, Handle<Function>::Cast(args[0]));

  uv_work_t* request = new uv_work_t;
  request->data = cbd;
  
  uv_queue_work(uv_default_loop(), request, ExecuteAsyncRun, ExecuteAsyncAfter);
}

void DbStmt::ExecuteAsyncRun(uv_work_t *req) {
  INITASYNC
  cbd->rc = SQLExecute(cbd->obj->stmth);
  DEBUG("SQLExecute(%d):\n", cbd->rc);
  LOG(cbd->rc != SQL_SUCCESS && cbd->rc != SQL_SUCCESS_WITH_INFO)

  SQLNumResultCols(obj->stmth, &obj->colCount);
  
  /* determine statement type */
  if(obj->colCount == 0) /* statement is not a select statement */
    return;
    
  obj->resultSetAvailable = true;
  if(obj->bindColData(NULL) < 0) return;
}
  
void DbStmt::ExecuteAsyncAfter(uv_work_t *req, int status) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  CallBackData* cbd = reinterpret_cast<CallBackData*>(req->data);
  DbStmt* obj = cbd->obj;

  if(obj->param && obj->paramCount > 0 ) {  // executeAsync(function(array){...})
    Handle<Array> array = Array::New(isolate);
    Local<Function> cb = Local<Function>::New(isolate, cbd->callback);
    for(int i = 0, j = 0; i < obj->paramCount; i++) {
      db2_param* param = &obj->param[i];
      if(param->io != SQL_PARAM_INPUT) {
        if(param->jsType == 1) // String
          array->Set(j, String::NewFromUtf8(isolate, (char*)param->buf));
        else if(param->jsType == 2)  // Number
          array->Set(j, Integer::New(isolate, *(long*)param->buf));
        j++;
      }
    } 
    obj->freeSp();
    if(strlen(obj->msg) > 0){
      Local<Value> argv[2] = { 
        Local<Value>::New(isolate, array),
        Local<Value>::New(isolate, String::NewFromUtf8(isolate, obj->msg))
      };
      cb->Call(isolate->GetCurrentContext()->Global(), 2, argv);
    } else {
      Local<Value> argv[1] = { Local<Value>::New(isolate, array) };
      cb->Call(isolate->GetCurrentContext()->Global(), 1, argv);
    }
  }
  else {  // executeAsync(function(){fetch()...})
    Local<Function> cb = Local<Function>::New(isolate, cbd->callback);
    if(strlen(obj->msg) > 0){
      Local<Value> argv[1] = { 
        Local<Value>::New(isolate, String::NewFromUtf8(isolate, obj->msg))
      };
      cb->Call(isolate->GetCurrentContext()->Global(), 1, argv);
    } else {
      cb->Call(isolate->GetCurrentContext()->Global(), 0, 0);
    }
  }
  
  delete cbd;
  delete req;
}

void DbStmt::NextResult(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  
  SQLCloseCursor(obj->stmth);
  
  SQLRETURN rc = SQLMoreResults(obj->stmth);
  CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLMoreResults() failed.", isolate)
  CHECK(rc == SQL_NO_DATA_FOUND, SQL_ERROR, "No more result set available.", isolate)
  
  rc = SQLNumResultCols(obj->stmth, &obj->colCount);
  CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLNumResultCols() failed.", isolate)

  /* determine statement type */
  if (obj->colCount == 0) /* statement is not a select statement */
    args.GetReturnValue().SetUndefined();  /* User can issue numRows() to the get affected rows number. */

  /* It is a select statement */
  obj->resultSetAvailable = true;
  if(obj->bindColData(isolate) < 0) return;
}
void DbStmt::Fetch(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", isolate)
  CHECK(obj->resultSetAvailable == false, RSSET_NOT_READY, "There is no result set to be queried. Please execute a SQL command first.", isolate);

  SQLRETURN rc;
  if (args.Length() > 1 && args[0]->IsInt32() && args[1]->IsInt32()) { 
    int orient = args[0]->Int32Value();
    int offset = args[1]->Int32Value();
    int retVal = 0;
    rc = SQLGetStmtAttr(obj->stmth, SQL_ATTR_CURSOR_SCROLLABLE, &retVal, 0, 0);
    if(retVal == SQL_TRUE) {
      rc = SQLFetchScroll(obj->stmth, orient, offset);
      DEBUG("SQLFetchScroll(%d) orient = %d, offset = %d.\n", rc, orient, offset);
    }
    else
      obj->throwErrMsg(SQL_ERROR, "Cursor is not scrollable.", isolate);
  }
  else {
    rc = SQLFetch(obj->stmth);
    DEBUG("SQLFetch(%d).\n", rc);
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

void DbStmt::FetchAsync(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  
  DEBUG("FetchAsync().\n");
  CHECK(args.Length() != 1 && args.Length() != 3 , INVALID_PARAM_NUM, "The fetch() method accept only one or three parameters.", isolate)
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", isolate)
  CHECK(obj->resultSetAvailable == false, RSSET_NOT_READY, "There is no result set to be queried. Please execute a SQL command first.", isolate);

  CallBackData* cbd = new CallBackData;
  cbd->obj = obj;
  cbd->arglength = args.Length();
  if (cbd->arglength > 2 && args[0]->IsInt32() && args[1]->IsInt32()) { 
    cbd->arg[0] = args[0]->Int32Value();
    cbd->arg[1] = args[1]->Int32Value();
  }
  cbd->callback.Reset(isolate, Handle<Function>::Cast(args[args.Length() - 1]));

  uv_work_t* request = new uv_work_t;
  request->data = cbd;
  
  uv_queue_work(uv_default_loop(), request, FetchAsyncRun, FetchAsyncAfter);
}

void DbStmt::FetchAsyncRun(uv_work_t *req) {
  INITASYNC
  if (cbd->arglength > 2) { 
    int orient = cbd->arg[0];
    int offset = cbd->arg[1];
    int retVal = 0;
    cbd->rc = SQLGetStmtAttr(obj->stmth, SQL_ATTR_CURSOR_SCROLLABLE, &retVal, 0, 0);
    if(retVal == SQL_TRUE) {
      cbd->rc = SQLFetchScroll(obj->stmth, orient, offset);
      DEBUG("SQLFetchScroll(%d) orient = %d, offset = %d.\n", cbd->rc, orient, offset);
    }
    else
      DEBUG("Cursor is not scrollable.\n");
  }
  else {
    cbd->rc = SQLFetch(obj->stmth);
    DEBUG("SQLFetch(%d).\n", cbd->rc);
  }
  
  LOG(cbd->rc != SQL_SUCCESS && cbd->rc != SQL_NO_DATA_FOUND)
}

void DbStmt::FetchAsyncAfter(uv_work_t *req, int status) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  CallBackData* cbd = reinterpret_cast<CallBackData*>(req->data);
  DbStmt* obj = cbd->obj;

  Handle<Object> row = Object::New(isolate);
  for(int i = 0; i < obj->colCount; i++) {
    Local<Value> value = Local<Value>::New(isolate, String::NewFromUtf8(isolate, obj->rowData[i]));
    row->Set(String::NewFromUtf8(isolate, (char const*)obj->dbColumn[i].name), value);
  }

  Local<Function> cb = Local<Function>::New(isolate, cbd->callback);
  
  if(strlen(obj->msg) > 0){
    Local<Value> argv[3] = { 
      Local<Value>::New(isolate, row),
      Local<Value>::New(isolate, Number::New(isolate, cbd->rc)),
      Local<Value>::New(isolate, String::NewFromUtf8(isolate, obj->msg))
    };
    cb->Call(isolate->GetCurrentContext()->Global(), 3, argv);
  } else {
    Local<Value> argv[2] = { 
      Local<Value>::New(isolate, row),
      Local<Value>::New(isolate, Number::New(isolate, cbd->rc))
    };
    cb->Call(isolate->GetCurrentContext()->Global(), 2, argv);
  }

  delete cbd;
  delete req;
}

void DbStmt::FetchAll(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  
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
    obj->freeCol();
    obj->throwErrMsg(SQL_ERROR, "SQLFetch() failed.", isolate);
  }
  if (args.Length() == 1) {
    Local<Function> cb = Local<Function>::Cast(args[0]);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { Local<Value>::New(isolate, array) };
    cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
  }
}

void DbStmt::FetchAllAsync(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  
  DEBUG("FetchAllAsync().\n");
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", isolate)
  CHECK(obj->resultSetAvailable == false, RSSET_NOT_READY, "There is no result set to be queried. Please execute a SQL command first.", isolate);
  
  CallBackData* cbd = new CallBackData;
  cbd->obj = obj;
  cbd->arglength = args.Length();
  cbd->callback.Reset(isolate, Handle<Function>::Cast(args[0]));

  uv_work_t* request = new uv_work_t;
  request->data = cbd;
  
  uv_queue_work(uv_default_loop(), request, FetchAllAsyncRun, FetchAllAsyncAfter);
}

void DbStmt::FetchAllAsyncRun(uv_work_t *req) {
  INITASYNC
  while((cbd->rc = SQLFetch(obj->stmth)) == SQL_SUCCESS) 
  {
    SQLCHAR** rowData = (SQLCHAR**)calloc(obj->colCount, sizeof(SQLCHAR*)); 
    for(int i = 0; i < obj->colCount; i++)
    {
      int colLen = strlen(obj->rowData[i]) + 1;
      rowData[i] = (SQLCHAR*)calloc(colLen, sizeof(SQLCHAR));
      memcpy(rowData[i], obj->rowData[i], colLen * sizeof(SQLCHAR));
    }
    obj->result.push_back(rowData);
  }
  LOG(cbd->rc != SQL_NO_DATA_FOUND)
}

void DbStmt::FetchAllAsyncAfter(uv_work_t *req, int status) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  CallBackData* cbd = reinterpret_cast<CallBackData*>(req->data);
  DbStmt* obj = cbd->obj;
  
  Handle<Array> array = Array::New(isolate);
  for(int i = 0; i < obj->result.size(); i++)
  {
    Handle<Object> row = Object::New(isolate);
    for(int j = 0; j < obj->colCount; j++)
    {
      Local<Value> value = Local<Value>::New(isolate, String::NewFromUtf8(isolate, obj->result[i][j]));
      row->Set(String::NewFromUtf8(isolate, (char const*)obj->dbColumn[j].name), value);
      free(obj->result[i][j]);
    }
    array->Set(i, row);  //Build the JSON data
    free(obj->result[i]);
  }
  obj->result.clear();

  if (cbd->arglength == 1) {
    Local<Function> cb = Local<Function>::New(isolate, cbd->callback);
    if(strlen(obj->msg) > 0){
      Local<Value> argv[2] = { 
        Local<Value>::New(isolate, array),
        Local<Value>::New(isolate, String::NewFromUtf8(isolate, obj->msg))
      };
      cb->Call(isolate->GetCurrentContext()->Global(), 2, argv);
    } else {
      Local<Value> argv[1] = { Local<Value>::New(isolate, array) };
      cb->Call(isolate->GetCurrentContext()->Global(), 1, argv);
    }
  }
  obj->freeCol();
  delete cbd;
  delete req;
}

void DbStmt::Commit(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", isolate)

  SQLRETURN rc = SQLTransact(envh, obj->connh, SQL_COMMIT);
  CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLTransact(Commit) failed.", isolate)
}

void DbStmt::Rollback(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", isolate)
  SQLRETURN rc = SQLTransact(envh, obj->connh, SQL_ROLLBACK);
  CHECK(rc != SQL_SUCCESS, SQL_ERROR, "SQLTransact(Rollback) failed.", isolate)
}

void DbStmt::NumFields(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  SQLSMALLINT colCount;
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", isolate)
  CHECK(obj->resultSetAvailable == false, RSSET_NOT_READY, "There is no result set to be queried. Please execute a SQL command first.", isolate);

  SQLRETURN rc = SQLNumResultCols(obj->stmth, &colCount);
  if(rc != SQL_SUCCESS) {
    obj->throwErrMsg(SQL_ERROR, "SQLNumResultCols() failed.", isolate);
  }
  RETURN(Number::New(isolate, colCount))
}

void DbStmt::NumRows(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", isolate)
  SQLINTEGER rowCount;
  if(SQLRowCount (obj->stmth, &rowCount) != SQL_SUCCESS) {
    obj->throwErrMsg(SQL_ERROR, "SQLRowCount() failed.", isolate);
  }
  RETURN(Number::New(isolate, rowCount))
}

void DbStmt::FieldType(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The fieldType() method only accept one parameter.", isolate)
  CHECK(obj->resultSetAvailable == false || obj->colDescAllocated == false, RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", isolate)

  SQLSMALLINT i = args[0]->Int32Value();
  CHECK(i >= obj->colCount || i < 0, INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", isolate)
  RETURN(Number::New(isolate, obj->dbColumn[i].sqlType))
}

void DbStmt::FieldWidth(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The fieldWidth() method only accept one parameter.", isolate)

  CHECK(obj->resultSetAvailable == false || obj->colDescAllocated == false, RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", isolate)

  SQLSMALLINT i = args[0]->Int32Value();
  CHECK(i >= obj->colCount || i < 0, INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", isolate)
  RETURN(Number::New(isolate, obj->dbColumn[i].nameLength))
}

void DbStmt::FieldName(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The fieldName() method only accept one parameter.", isolate)

  CHECK(obj->resultSetAvailable == false || obj->colDescAllocated == false, RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", isolate)

  SQLSMALLINT i = args[0]->Int32Value();
  CHECK(i >= obj->colCount || i < 0, INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", isolate)
  
  RETURN(String::NewFromUtf8(isolate, obj->dbColumn[i].name))
}

void DbStmt::FieldPrecise(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The fieldPrecise() method only accept one parameter.", isolate)
  CHECK(obj->resultSetAvailable == false || obj->colDescAllocated == false, RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", isolate)

  SQLSMALLINT i = args[0]->Int32Value();
  CHECK(i >= obj->colCount || i < 0, INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", isolate)

  RETURN(Number::New(isolate, obj->dbColumn[i].colPrecise))
}

void DbStmt::FieldScale(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The fieldScale() method only accept one parameter.", isolate)
  CHECK(obj->resultSetAvailable == false || obj->colDescAllocated == false, RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", isolate)

  SQLSMALLINT i = args[0]->Int32Value();
  CHECK(i >= obj->colCount || i < 0, INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", isolate)
  
  RETURN(Number::New(isolate, obj->dbColumn[i].colScale))
}

void DbStmt::FieldNullable(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  CHECK(args.Length() != 1, INVALID_PARAM_NUM, "The fieldNullable() method only accept one parameter.", isolate)
  CHECK(obj->resultSetAvailable == false || obj->colDescAllocated == false, RSSET_NOT_READY, "The Result set is unavailable. Try query something first.", isolate)

  SQLSMALLINT i = args[0]->Int32Value();
  CHECK(i >= obj->colCount || i < 0, INVALID_PARAM_RANGE, "The input parameter is beyond the boundary.", isolate)
  
  RETURN(Number::New(isolate, obj->dbColumn[i].colNull))
}

void DbStmt::StmtError(const ARGUMENTS& args) {
  Isolate* isolate = args.GetIsolate(); 
  HandleScope scope(isolate);
  DbStmt* obj = ObjectWrap::Unwrap<DbStmt>(args.Holder());
  CHECK(obj->stmtAllocated == false, STMT_NOT_READY, "The SQL Statement handler is not initialized.", isolate)
  CHECK(args.Length() < 2, INVALID_PARAM_NUM, "The stmtError() method only accept at lease two parameters.", isolate)

  SQLSMALLINT hType = args[0]->Int32Value();
  SQLSMALLINT recno = args[1]->Int32Value();
  SQLINTEGER handle;
  
  switch(hType)
  {
    case SQL_HANDLE_ENV:
      handle = envh;
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
