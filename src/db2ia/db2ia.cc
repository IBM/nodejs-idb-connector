/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */

#include "dbconn.h"
#include "dbstmt.h"

using namespace v8;

void CreateConnObject(const ARGUMENTS& args) {
  DbConn::NewInstance(args);
}

void CreateStmtObject(const ARGUMENTS& args) {
  DbStmt::NewInstance(args);
}

void InitAll(Handle<Object> exports) {
  SQLHENV envh;
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
}

NODE_MODULE(db2ia, InitAll)