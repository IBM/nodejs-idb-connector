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

#include "dbconn.h"
#include "dbstmt.h"

using namespace v8;

SQLHENV envh;

void CreateConnObject(const ARGUMENTS& args) {
  DbConn::NewInstance(args);
}

void CreateStmtObject(const ARGUMENTS& args) {
  DbStmt::NewInstance(args);
}

void FreeEnvironment(const ARGUMENTS& args) {
  SQLFreeEnv(envh);
}

void InitAll(Handle<Object> exports) {
  int param = SQL_TRUE;
  char *attr = "DB2CCSID", *ccsid = NULL;
  ccsid = getenv(attr);
  if (ccsid != NULL)
    SQLOverrideCCSID400(atoi(ccsid));  //CCSID customization.
  else
    SQLOverrideCCSID400(1208); // Run under CCSID 1208(UTF-8) by default.  
  
  SQLRETURN rc = SQLAllocEnv( &envh );
  if(rc != SQL_SUCCESS) {
    /* If SQL_ERROR is returned and phenv is equal to SQL_NULL_HENV, then SQLGetDiagRec() cannot be called */
    /* because there is no handle with which to associate additional diagnostic information. */
    /* If the return code is SQL_ERROR and the pointer to the environment handle is not equal to */
    /* SQL_NULL_HENV, then the handle is a restricted handle. This means the handle can only be used in a call */
    /* to SQLGetDiagRec() to obtain more error information, or to SQLFreeEnv(). */
    if(envh != SQL_NULL_HENV)
      SQLFreeEnv(envh);
    return;
  }
  rc = SQLSetEnvAttr(envh, SQL_ATTR_SERVER_MODE, &param, 0); // Enable Server Mode by default.
  
  DbConn::Init(envh);
  DbStmt::Init();
  NODE_SET_METHOD(exports, "dbconn", CreateConnObject);
  NODE_SET_METHOD(exports, "dbstmt", CreateStmtObject);
  NODE_SET_METHOD(exports, "close", FreeEnvironment);
}

NODE_MODULE(db2ia, InitAll)