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
/* (C)Copyright IBM Corp.  2014, 2014                               */
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
 
#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlcli.h"
#include <as400_protos.h>  // For SQLOverrideCCSID400()

#include <node.h>
#include <node_object_wrap.h>

#define MAX_COLNAME_WIDTH 256
#define MAX_COL_WIDTH 1024
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

const volatile static char copyright[] = "5733OPS\
 IBM Open Source Solutions for i5/OS\
 (C) Copyright IBM Corp. 2014, 2016.\
 All rights reserved.\
 US Government Users Restricted Rights -\
 Use, duplication or disclosure restricted\
 by GSA ADP Schedule Contract with IBM Corp.\
 Licensed Materials - Property of IBM ";

using namespace v8;

typedef struct _db2_column {
  SQLCHAR*    name;
  SQLSMALLINT  nameLength;
  SQLSMALLINT  sqlType;
  SQLINTEGER  colPrecise;
  SQLSMALLINT  colScale;
  SQLSMALLINT  colNull;
} db2_column;

class DbConn : public node::ObjectWrap {
  public:
    static void Init();
    static void NewInstance(const ARGUMENTS& args);
  private:
    explicit DbConn();
    ~DbConn();

    void allocColRow()
    {
      if(colRowAllocated == true) 
        freeColRow(); 
      dbColumn = (db2_column*)calloc(colCount, sizeof(db2_column)); 
      rowData = (SQLCHAR**)calloc(colCount, sizeof(SQLCHAR*)); 
      for(int i = 0; i < colCount; i++) 
      { 
        dbColumn[i].name = (SQLCHAR*)calloc(MAX_COLNAME_WIDTH, sizeof(SQLCHAR)); 
        rowData[i] = (SQLCHAR*)calloc(MAX_COL_WIDTH, sizeof(SQLCHAR)); 
      } 
      colRowAllocated = true; 
    }

    void freeColRow() 
    {
      if(colRowAllocated == true) 
      { 
        for(int i = 0; i < colCount && dbColumn[i].name && rowData[i]; i++)
        { 
          free(dbColumn[i].name); 
          free(rowData[i]); 
        } 
        free(dbColumn); 
        free(rowData); 
        colRowAllocated = false; 
      } 
    }

    void freeSp() 
    {
      if(spInCount > 0) 
      { 
        for(int i = 0; i < spInCount; i++) 
          free(spIn[i]); 
        spInCount = 0;
      } 
      if(spOutCount > 0) 
      { 
        for(int i = 0; i < spOutCount; i++) 
          free(spOut[i]); 
        spOutCount = 0;
      } 
      spInNumCount = 0;
      spOutNumCount = 0;
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
      
      if(handleType == SQL_HANDLE_STMT && stmtAllocated == true) {
        rc = SQLGetDiagRec(SQL_HANDLE_STMT, stmth, 1, sqlstate, &sqlcode, msg, SQL_MAX_MESSAGE_LENGTH + 1, &length); 
        printError(envh, connh, stmth);
      }
      else if(handleType == SQL_HANDLE_DBC && connAllocated == true) {
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
    
    static void New(const ARGUMENTS& args);
    
    static void SetEnvAttr(const ARGUMENTS& args);
    static void SetConnAttr(const ARGUMENTS& args);
    static void SetStmtAttr(const ARGUMENTS& args);
    
    static void GetEnvAttr(const ARGUMENTS& args);
    static void GetConnAttr(const ARGUMENTS& args);
    static void GetStmtAttr(const ARGUMENTS& args);
    
    static void Env(const ARGUMENTS& args);
    static void Conn(const ARGUMENTS& args);
    
    static void Exec(const ARGUMENTS& args);
    static void CloseCursor(const ARGUMENTS& args);
    
    static void Prepare(const ARGUMENTS& args);
    static void BindParam(const ARGUMENTS& args);
    static void Execute(const ARGUMENTS& args);
    static void NextResult(const ARGUMENTS& args);

    static void Fetch(const ARGUMENTS& args);
    static void FetchAll(const ARGUMENTS& args);
    
    static void Commit(const ARGUMENTS& args);
    static void Rollback(const ARGUMENTS& args);

    static void Close(const ARGUMENTS& args);
    
    static void NumFields(const ARGUMENTS& args);
    static void NumRows(const ARGUMENTS& args);
    
    static void FieldType(const ARGUMENTS& args);
    static void FieldWidth(const ARGUMENTS& args);
    static void FieldName(const ARGUMENTS& args);
    static void FieldPrecise(const ARGUMENTS& args);
    static void FieldScale(const ARGUMENTS& args);
    static void FieldNullable(const ARGUMENTS& args);
    
    static void StmtError(const ARGUMENTS& args);
    static void ValidStmt(const ARGUMENTS& args);
    
    static void Debug(const ARGUMENTS& args);
    
    static void RunXS(const ARGUMENTS& args);

    static Persistent<Function> constructor;
    
    bool envAllocated;
    bool connAllocated;
    bool stmtAllocated;
    bool resultSetAvailable;
    bool colRowAllocated;
    
    bool isDebug;
    
    SQLHENV envh;
    SQLHDBC connh;
    SQLHSTMT stmth;
    
    char* xmlOut;
    char* spIn[SP_PARAM_MAX];
    char* spOut[SP_PARAM_MAX];
    int spInNum[SP_PARAM_MAX];
    int spOutNum[SP_PARAM_MAX];
    SQLINTEGER indicator[SP_PARAM_MAX];
    int spInCount;
    int spOutCount;
    int spInNumCount;
    int spOutNumCount;
    
    SQLSMALLINT colCount;
    db2_column* dbColumn;
    SQLCHAR** rowData;
};

#endif
