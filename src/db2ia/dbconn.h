/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */
 
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlcli.h"
#include <as400_protos.h>  // For SQLOverrideCCSID400()

#include <node.h>
#include <node_object_wrap.h>
#include <uv.h>

#define MAX_COLNAME_WIDTH 256
#define SP_PARAM_MAX 128

#define INVALID_PARAM_NUM 8001
#define INVALID_PARAM_TYPE 8002
#define INVALID_PARAM_RANGE 8003

#define ENV_NOT_READY 8011
#define CONN_NOT_READY 8012
#define STMT_NOT_READY 8013
#define RSSET_NOT_READY 8014

#define RETURN(v) args.GetReturnValue().Set(v);
// #define RETURN_UNDEINED args.GetReturnValue().SetUndefined();
#define ARGUMENTS FunctionCallbackInfo<Value>
#define DEBUG(f_, ...) if(obj->isDebug) { printf((f_), ##__VA_ARGS__); }
#define CHECK(a, b, c, d) if((a)) { obj->throwErrMsg( (b), (c), (d) ); return; }

using namespace v8;

class DbConn : public node::ObjectWrap {
  friend class DbStmt;
  public:
    static void Init(SQLHENV envh);
    static void NewInstance(const ARGUMENTS& args);
  private:
    explicit DbConn();
    ~DbConn();
    
    static void New(const ARGUMENTS& args);
    static Persistent<Function> constructor;
    
    static SQLHENV envh;
    
    static void Debug(const ARGUMENTS& args);
    static void SetConnAttr(const ARGUMENTS& args);
    static void GetConnAttr(const ARGUMENTS& args);
    static void Conn(const ARGUMENTS& args);
    static void Disconnect(const ARGUMENTS& args);
    static void IsConnected(const ARGUMENTS& args);
    static void Close(const ARGUMENTS& args);
    static void ValidStmt(const ARGUMENTS& args);
    
    SQLHDBC connh;
    bool connAllocated = false;
    bool connected = false;
    bool isDebug = false;

    void printError(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt)
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
    
    void throwErrMsg(int handleType, Isolate* isolate) 
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
      SQLRETURN rc = -1;
      
      // if(handleType == SQL_HANDLE_STMT && stmtAllocated == true) {
        // rc = SQLGetDiagRec(SQL_HANDLE_STMT, stmth, 1, sqlstate, &sqlcode, msg, SQL_MAX_MESSAGE_LENGTH + 1, &length); 
        // printError(envh, connh, stmth);
      // }
      if(handleType == SQL_HANDLE_DBC && connAllocated == true) {
        rc = SQLGetDiagRec(SQL_HANDLE_DBC, connh, 1, sqlstate, &sqlcode, msg, SQL_MAX_MESSAGE_LENGTH + 1, &length); 
        printError(envh, connh, SQL_NULL_HSTMT);
      }
      else if(handleType == SQL_HANDLE_ENV) {
        rc = SQLGetDiagRec(SQL_HANDLE_ENV, envh, 1, sqlstate, &sqlcode, msg, SQL_MAX_MESSAGE_LENGTH + 1, &length); 
        printError(envh, SQL_NULL_HDBC, SQL_NULL_HSTMT);
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
    
    void throwErrMsg(int code, const char* msg, Isolate* isolate)
    {
      SQLCHAR errMsg[SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10];
      sprintf((char *)errMsg, "SQLSTATE=PAERR SQLCODE=%d %s", code, msg);
      isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, errMsg)));
    }
};