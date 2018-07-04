/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */

#pragma once

#include "dbconn.h"
#include <vector>
#include "node_buffer.h"

#define LOG(a) if((a)) { obj->printErrorToLog(); return; }
#define INITASYNC \
  CallBackData* cbd = reinterpret_cast<CallBackData*>(req->data); \
  DbStmt* obj = cbd->obj; \
  memset(obj->msg, 0, sizeof(obj->msg));

using namespace v8;

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

class DbStmt : public node::ObjectWrap {
  public:
    static void Init();
    static void NewInstance(const ARGUMENTS& args);
    
  private:
    explicit DbStmt(DbConn* conn);
    ~DbStmt();
    
    static Persistent<Function> constructor;
    
    uv_work_t request;

    bool stmtAllocated = false;
    bool resultSetAvailable = false;
    bool colDescAllocated = false;
    bool colDataAllocated = false;
    bool isDebug = false;
    
    SQLHENV envh;
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
    
    db2_param* param = NULL;
    int paramCount = 0;
    
    SQLSMALLINT colCount = 0;
    db2_column* dbColumn;
    SQLCHAR** rowData;
    std::vector<result_item*> result;

    char msg[SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10];

    static void New(const ARGUMENTS& args);

    static void SetStmtAttr(const ARGUMENTS& args);
    static void GetStmtAttr(const ARGUMENTS& args);
    
    static void Exec(const ARGUMENTS& args);
    static void ExecAsync(const ARGUMENTS& args);
    static void ExecAsyncRun(uv_work_t *req);
    static void ExecAsyncAfter(uv_work_t *req, int status);
    
    static void CloseCursor(const ARGUMENTS& args);
    static void Reset(const ARGUMENTS& args);
     
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
    
    static void nop(uv_work_t* req) { }
    
    // Get Column Descriptions [name][type][precise][etc.]
    int getColDesc(Isolate* isolate) {
      if(colDescAllocated == true) 
        freeColDesc();
      
      SQLRETURN rc;
      dbColumn = (db2_column*)calloc(colCount, sizeof(db2_column));
      
      if(isDebug)
        printf("SQLDescribeCol(%d).\n", colCount);
      
      for(int i = 0; i < colCount; i++) {
        dbColumn[i].name = (SQLCHAR*)calloc(MAX_COLNAME_WIDTH, sizeof(SQLCHAR));
        rc = SQLDescribeCol(stmth, i + 1, //Column index starts from 1.
              dbColumn[i].name,  //the buffer to store the Column Name
              MAX_COLNAME_WIDTH, //the size of the store buffer 
              &dbColumn[i].nameLength,  //the accurate length of the Column Name
              &dbColumn[i].sqlType,  //the SQL type of the Column
              &dbColumn[i].colPrecise, &dbColumn[i].colScale, &dbColumn[i].colNull);
        if(isDebug)      
          printf("[%d]sqlType[%d]\tcolScale[%d]\tcolPrecise[%d]\n", i, dbColumn[i].sqlType, dbColumn[i].colScale, dbColumn[i].colPrecise);
        if(rc != SQL_SUCCESS) {
          freeColDesc();
          if(isolate != NULL)
            throwErrMsg(SQL_ERROR, "SQLDescribeCol() failed.", isolate);
          else printErrorToLog();
          return -1;
        }
      }
      colDescAllocated = true;
      return 0;
    }
    
    // Free column descriptions [name][type][precise][etc.]
    void freeColDesc() {
      if(colDescAllocated == true) { 
        for(int i = 0; i < colCount && dbColumn[i].name; i++)
          free(dbColumn[i].name); 
        free(dbColumn); 
        colDescAllocated = false; 
      }
    }
    
    // Bind volumn data to C variables
    int bindColData(Isolate* isolate) {
      if(colDataAllocated == true) 
        freeColData();
      
      if(colDescAllocated != true)
        if(getColDesc(isolate) < 0)
          return -1;
      
      SQLRETURN rc;
      SQLINTEGER rlength = 0;
      SQLINTEGER maxColLen = 0;
      int ctype = SQL_C_CHAR;
      rowData = (SQLCHAR**)calloc(colCount, sizeof(SQLCHAR*)); 
      
      if(isDebug)
        printf("SQLBindCol(%d).\n", colCount);
      
      /* SQL Data Types
      // SQL_CHAR=1
      // SQL_NUMERIC=2    colScale + colScale + 3
      // SQL_DECIMAL=3    colScale + colScale + 3
      // SQL_INTEGER=4    -2147483648 ~ 2147483647  11 chars
      // SQL_SMALLINT=5   -32768 ~ 32767  6 chars
      // SQL_FLOAT=6      colScale + colScale + 3
      // SQL_REAL=7       colScale + colScale + 3
      // SQL_DOUBLE=8     colScale + colScale + 3
      // SQL_DATETIME=9
      // SQL_VARCHAR=12
      // SQL_BLOB=13
      // SQL_CLOB=14
      // SQL_DBCLOB=15
      // SQL_DATALINK=16
      // SQL_WCHAR=17
      // SQL_WVARCHAR=18
      // SQL_BIGINT=19    -9223372036854775808 ~ 9223372036854775807  20 chars
      // SQL_BLOB_LOCATOR=20
      // SQL_CLOB_LOCATOR=21
      // SQL_DBCLOB_LOCATOR=22
      // SQL_UTF8_CHAR=23 
      // SQL_WLONGVARCHAR=SQL_WVARCHAR
      // SQL_LONGVARCHAR=SQL_VARCHAR
      // SQL_GRAPHIC=95
      // SQL_VARGRAPHIC=96
      // SQL_LONGVARGRAPHIC=SQL_VARGRAPHIC
      // SQL_BINARY=-2
      // SQL_VARBINARY=-3
      // SQL_LONGVARBINARY=SQL_VARBINARY
      // SQL_DATE=91
      // SQL_TYPE_DATE=91
      // SQL_TIME=92
      // SQL_TYPE_TIME=92
      // SQL_TIMESTAMP=93
      // SQL_TYPE_TIMESTAMP=93
      // SQL_CODE_DATE=1
      // SQL_CODE_TIME=2
      // SQL_CODE_TIMESTAMP=3
      // SQL_ALL_TYPES=0
      // SQL_DECFLOAT=-360 
      // SQL_XML=-370 
      */
      
      for(int i = 0; i < colCount; i++) {
        switch(dbColumn[i].sqlType) {
          case SQL_SMALLINT :
          {
            maxColLen = 7;
            rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
            rc = SQLBindCol(stmth, i + 1, SQL_C_CHAR, (SQLPOINTER)rowData[i], maxColLen, &dbColumn[i].rlength);
          } break;
          case SQL_INTEGER :
          {
            maxColLen = 12;
            rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
            rc = SQLBindCol(stmth, i + 1, SQL_C_CHAR, (SQLPOINTER)rowData[i], maxColLen, &dbColumn[i].rlength);
          } break;
          case SQL_BIGINT :
          {
            maxColLen = 21;
            rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
            rc = SQLBindCol(stmth, i + 1, SQL_C_CHAR, (SQLPOINTER)rowData[i], maxColLen, &dbColumn[i].rlength);
          } break;
          case SQL_DECIMAL :
          case SQL_NUMERIC :
          case SQL_FLOAT :
          case SQL_REAL :
          case SQL_DOUBLE :
          {
            maxColLen = dbColumn[i].colPrecise + dbColumn[i].colScale + 3;
            rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
            rc = SQLBindCol(stmth, i + 1, SQL_C_CHAR, (SQLPOINTER)rowData[i], maxColLen, &dbColumn[i].rlength);
          } break;
          case SQL_VARBINARY :
          case SQL_BINARY :
          {
            maxColLen = dbColumn[i].colPrecise;
            rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
            rc = SQLBindCol(stmth, i + 1, SQL_C_BINARY, (SQLPOINTER)rowData[i], maxColLen, &dbColumn[i].rlength);
          } break;
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
            // rc = SQLBindCol(stmth, i + 1, SQL_C_BLOB, blob, sizeof(blob_data), &blob->length);
          } break;
          case SQL_WCHAR :
          case SQL_WVARCHAR :
          {
            maxColLen = dbColumn[i].colPrecise << 2;
            rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
            rc = SQLBindCol(stmth, i + 1, SQL_C_CHAR, (SQLPOINTER)rowData[i], maxColLen, &dbColumn[i].rlength);
          } break;
          default :
          {
            maxColLen = dbColumn[i].colPrecise + dbColumn[i].colScale + 3 ;
            rowData[i] = (SQLCHAR*)calloc(maxColLen, sizeof(SQLCHAR));
            rc = SQLBindCol(stmth, i + 1, SQL_C_CHAR, (SQLPOINTER)rowData[i], maxColLen, &dbColumn[i].rlength);
          } break;
        }
        if(rc != SQL_SUCCESS) {
          freeColData();
          if(isolate != NULL)
            throwErrMsg(SQL_ERROR, "SQLBindCol() failed.", isolate);
          else printErrorToLog();
          return -1;
        }
      } 
      colDataAllocated = true;
      return 0;
    }
    
    // Free the C variables binding to column data.
    void freeColData() {
      if(colDataAllocated == true) { 
        for(int i = 0; i < colCount && rowData[i]; i++)
          free(rowData[i]); 
        free(rowData); 
        colDataAllocated = false; 
      }
    }
    
    // Free column descriptions & the C variables binding to column data.
    void freeColDescAndData() {
      freeColDesc();
      freeColData();
    }
    
    // [DB Data] ==> [Handle<Array>]
    int fetchColData(Isolate* isolate, Handle<Array> array) {
      SQLRETURN rc;
      int arrayCount = 0; 
      while((rc = SQLFetch(stmth)) == SQL_SUCCESS) {
        Handle<Object> row = Object::New(isolate);
        for(int i = 0; i < colCount; i++) {
          Local<Value> value;
          if(dbColumn[i].rlength == SQL_NULL_DATA)
            value = Local<Value>::New(isolate, Null(isolate));
          else {
            switch(dbColumn[i].sqlType) {
              case SQL_VARBINARY :
              case SQL_BINARY :
                value = Local<Value>::New(isolate, node::Buffer::New(isolate, rowData[i], dbColumn[i].rlength).ToLocalChecked());
                break;
              case SQL_BLOB : //TODO
                // value = Local<Value>::New(isolate, node::Buffer::New(isolate, rowData[i] + sizeof(int), *(int*)rowData[i]).ToLocalChecked());
                break;
              default : 
                value = Local<Value>::New(isolate, String::NewFromUtf8(isolate, rowData[i]));
                break;
            }
          }
          row->Set(String::NewFromUtf8(isolate, (char const*)dbColumn[i].name), value);
        }
        array->Set(arrayCount++, row);  //Build the JSON data
      }
      if (rc != SQL_NO_DATA_FOUND) {
        freeColDescAndData();
        if(isolate != NULL)
          throwErrMsg(SQL_HANDLE_STMT, isolate);
        else printErrorToLog();
          return -1;
      }
      return 0;
    }
    
    //  [DB Data] ==> [vector result]
    int fetchData(SQLRETURN* rc) {
      while((*rc = SQLFetch(stmth)) == SQL_SUCCESS) {
        result_item* row = (result_item*)calloc(colCount, sizeof(result_item)); 
        for(int i = 0; i < colCount; i++) {
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
    }
    
    // [vector result] ==> [Handle<Array>]
    int fetchColDataAsync(Isolate* isolate, Handle<Array> array) {
      for(int i = 0; i < result.size(); i++) {
        Handle<Object> row = Object::New(isolate);
        for(int j = 0; j < colCount; j++) {
          Local<Value> value;
          if(result[i][j].rlength == SQL_NULL_DATA)
            value = Local<Value>::New(isolate, Null(isolate));
          else {
            switch(dbColumn[j].sqlType) {
              case SQL_VARBINARY :
              case SQL_BINARY :
                value = Local<Value>::New(isolate, node::Buffer::New(isolate, result[i][j].data, result[i][j].rlength).ToLocalChecked());
                break;
              default : 
                value = Local<Value>::New(isolate, String::NewFromUtf8(isolate, result[i][j].data));
                break;
            }
          }
          row->Set(String::NewFromUtf8(isolate, (char const*)dbColumn[j].name), value);
          if(result[i][j].data)
            free(result[i][j].data);
        }
        array->Set(i, row);  //Build the JSON data
        free(result[i]);
      }
      result.clear();
      return 0;
    }

    // [rowData] ==> [Handle<Object>]
    int fetchOneRow(Isolate* isolate, Handle<Object> row) {
      for(int i = 0; i < colCount; i++) {
        Local<Value> value;
        if(dbColumn[i].rlength == SQL_NULL_DATA)
          value = Local<Value>::New(isolate, Null(isolate));
        else {
          switch(dbColumn[i].sqlType) {
            case SQL_VARBINARY :
            case SQL_BINARY :
              value = Local<Value>::New(isolate, node::Buffer::New(isolate, rowData[i], dbColumn[i].rlength).ToLocalChecked());
              break;
            case SQL_BLOB : //TODO
              // value = Local<Value>::New(isolate, node::Buffer::New(isolate, rowData[i] + sizeof(int), *(int*)rowData[i]).ToLocalChecked());
              break;
            default : 
              value = Local<Value>::New(isolate, String::NewFromUtf8(isolate, rowData[i]));
              break;
          }
        }
        row->Set(String::NewFromUtf8(isolate, (char const*)dbColumn[i].name), value);
      }
      return 0;
    }
    
    // Bind parameter markers ? to array param[]
    int bindParams(Isolate* isolate, Handle<Array> params) {
      Handle<Array> object;
      int bindIndicator;
      SQLRETURN rc;
      freeSp();
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
      paramCount = params->Length();
      param = (db2_param*)calloc(paramCount, sizeof(db2_param));

      for(SQLSMALLINT i = 0; i < paramCount; i++) {
        object = Handle<Array>::Cast(params->Get(i));  //Get a  ? parameter from the array.
        param[i].io = object->Get(1)->Int32Value();  //Get the parameter In/Out type.
        bindIndicator = object->Get(2)->Int32Value();  //Get the indicator(str/int).
        
        rc = SQLDescribeParam(stmth, i + 1, 
                            &param[i].paramType, 
                            &param[i].paramSize, 
                            &param[i].decDigits, 
                            &param[i].nullable);
        if(rc != SQL_SUCCESS)
          throwErrMsg(SQL_ERROR, "SQLDescribeParam() failed.", isolate);

        Local<Value> value = object->Get(0);
        
        if(bindIndicator == 0 || bindIndicator == 1) { //Parameter is string 
          String::Utf8Value string(value);
          param[i].valueType = SQL_C_CHAR;
          if(param[i].io == SQL_PARAM_INPUT) {
            param[i].buf = strdup(*string);
            if(bindIndicator == 0) //CLOB
              param[i].ind = strlen(*string);
            else if(bindIndicator == 1) //NTS
              param[i].ind = SQL_NTS;
          }
          else if(param[i].io == SQL_PARAM_OUTPUT) {
            param[i].buf = (char*)calloc(param[i].paramSize + 1, sizeof(char));
            param[i].ind = param[i].paramSize;
          }
          else if(param[i].io == SQL_PARAM_INPUT_OUTPUT) {
            param[i].buf = (char*)calloc(param[i].paramSize + 1, sizeof(char));
            strcpy((char*)param[i].buf, *string);
            if(bindIndicator == 0) //CLOB
              param[i].ind = strlen(*string);
            else if(bindIndicator == 1) //NTS
              param[i].ind = SQL_NTS;
          }
        }
        else if(bindIndicator == 2) { //Parameter is Integer
          int64_t *number = (int64_t*)malloc(sizeof(int64_t));
          *number = value->IntegerValue();
          param[i].valueType = SQL_C_BIGINT;
          param[i].buf = number;
          param[i].ind = 0;
        }
        else if(bindIndicator == 3) { //Parameter is NULL
          param[i].valueType = SQL_C_DEFAULT;
          param[i].ind = SQL_NULL_DATA;
        }
        else if(value->IsNumber() || bindIndicator == 4) { //Parameter is Decimal
          double *number = (double*)malloc(sizeof(double));
          *number = value->NumberValue();
          param[i].valueType = SQL_C_DOUBLE;
          param[i].buf = number;
          param[i].ind = sizeof(double);
          param[i].decDigits = 7;
          param[i].paramSize = sizeof(double);
        }
        else if(value->IsBoolean() || bindIndicator == 5) { //Parameter is Boolean
          bool *boolean = (bool*)malloc(sizeof(bool));
          *boolean = value->BooleanValue();
          param[i].valueType = SQL_C_BIT;
          param[i].buf = boolean;
          param[i].ind = 0;
        }
        
        rc = SQLBindParameter(stmth, i + 1, 
                param[i].io, 
                param[i].valueType, 
                param[i].paramType, 
                param[i].paramSize, 
                param[i].decDigits, 
                param[i].buf, 0, 
                &param[i].ind);
        if(isDebug)
          printf("SQLBindParameter(%d) TYPE[%2d] SIZE[%3d] DIGI[%d] IO[%d] IND[%3d]\n", rc, param[i].paramType, param[i].paramSize, param[i].decDigits, param[i].io, param[i].ind);
      }
    }
    
    // [param] ==> [Handle<Array>]
    int fetchSp(Isolate* isolate, Handle<Array> array) {
      for(int i = 0, j = 0; i < paramCount; i++) {
        db2_param* p = &param[i];
        if(p->io != SQL_PARAM_INPUT) {
          if(p->valueType == SQL_C_BIGINT)  // Integer
            array->Set(j, Integer::New(isolate, *(int64_t*)p->buf));
          else if(p->valueType == SQL_C_DOUBLE)  // Decimal
            array->Set(j, Number::New(isolate, *(double*)p->buf));
          else if(p->valueType == SQL_C_BIT)  // Boolean
            array->Set(j, Boolean::New(isolate, *(bool*)p->buf));
          else
            array->Set(j, String::NewFromUtf8(isolate, (char*)p->buf));
          j++;
        }
      }
      freeSp();
      return 0;
    }
    
    // Free array param[]
    void freeSp() {
      if(param) {
        for(int i = 0; i < paramCount; i++)
          free(param[i].buf);
        free(param);
        param = NULL;
      }
    }

    void printError(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt) {
      if(isDebug == true) { 
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
    
    void printErrorToLog() {
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
    
    void throwErrMsg(int handleType, Isolate* isolate) {
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
    
    void throwErrMsg(int code, const char* msg, Isolate* isolate) {
      SQLCHAR errMsg[SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10];
      sprintf((char *)errMsg, "SQLSTATE=PAERR SQLCODE=%d %s", code, msg);
      isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, errMsg)));
    }
};