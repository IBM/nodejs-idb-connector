/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */
 
#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlcli.h"
#include <as400_protos.h>  // For SQLOverrideCCSID400()

#include "napi.h"
// #include <uv.h>

#define MAX_COLNAME_WIDTH 256
#define MAX_COL_WIDTH 32766
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
#define ARGUMENTS Napi::CallbackInfo
#define DEBUG(object, f_, ...) if(object->isDebug) { printf((f_), ##__VA_ARGS__); }
#define CHECK(condition, errorCode, errorMessage, env) if((condition)) { this->throwErrMsg( (errorCode), (errorMessage), (env) ); return; }
#define CHECK_WITH_RETURN(condition, errorCode, errorMessage, env, returnValue) if((condition)) { this->throwErrMsg( (errorCode), (errorMessage), (env) ); return (returnValue); }

// using namespace Napi;

class DbConn : public Napi::ObjectWrap<DbConn> {
  friend class DbStmt;
  public:
    DbConn(const Napi::CallbackInfo& info);
    static Napi::Object Init(Napi::Env env, Napi::Object exports, SQLHENV envh);
    static Napi::Object NewInstance(Napi::Value arg);
  private:
    bool connAllocated = false;
    bool connected = false;
    bool isDebug = false;
    //Most likely not needed in new Napi version
    void New(const ARGUMENTS& args);
    Napi::Value SetConnAttr(const Napi::CallbackInfo& info);
    Napi::Value GetConnAttr(const Napi::CallbackInfo& info);
    void Conn(const Napi::CallbackInfo& info);
    Napi::Value Disconnect(const Napi::CallbackInfo& info);
    Napi::Value Close(const Napi::CallbackInfo& info);
    Napi::Value ValidStmt(const Napi::CallbackInfo& info);
    Napi::Value Debug(const Napi::CallbackInfo& info);
    Napi::Value IsConnected(const Napi::CallbackInfo& info);


    static Napi::FunctionReference constructor;
    static SQLHENV envh;
    SQLHDBC connh;

     void throwErrMsg(int handleType,  Napi::Env env) 
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
    }

    void throwErrMsg(int code, const char* msg, Napi::Env env)
  {
    SQLCHAR errMsg[SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10];
    sprintf((char *)errMsg, "SQLSTATE=PAERR SQLCODE=%d %s", code, msg);
    Napi::Error::New(env, Napi::String::New(env, errMsg)).ThrowAsJavaScriptException();
    return;
  }

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
};