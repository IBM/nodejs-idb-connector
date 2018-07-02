/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */

#include "dbconn.h"
#include "dbstmt.h"
#include <napi.h>
using namespace Napi;

SQLHENV envh;

// void CreateConnObject(const Napi::CallbackInfo& info) {
//   DbConn::NewInstance(info.Env(), info[0]);
// }

// void CreateStmtObject(const ARGUMENTS& args) {
//   DbStmt::NewInstance(args);
// }

// void FreeEnvironment(const ARGUMENTS& args) {
//   SQLFreeEnv(envh);
// }

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  int param = SQL_TRUE;
  char *attr = (char *)"DB2CCSID", *ccsid = NULL;
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
    if(envh != SQL_NULL_HENV){
        SQLFreeEnv(envh);
    }
      //TODO: figure out later
    // return
  }
  rc = SQLSetEnvAttr(envh, SQL_ATTR_SERVER_MODE, &param, 0); // Enable Server Mode by default.
  
  DbConn::Init(env , exports, envh);
  DbStmt::Init(env, exports);
  
  return exports;
}

NODE_API_MODULE(db2ia, InitAll)