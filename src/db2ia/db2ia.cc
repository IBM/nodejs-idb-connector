// Copyright contributors to the nodejs-idb-connector project
// SPDX-License-Identifier: MIT

#include <napi.h>

#ifdef __PASE__

#include "dbconn.h"
#include "dbstmt.h"
SQLHENV envh;

#ifndef SQL_ATTR_NON_HEXCCSID
#define SQL_ATTR_NON_HEXCCSID 10203
#endif

#endif

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
#ifdef __PASE__
  int param = SQL_TRUE;
  char *attr = (char *)"DB2CCSID", *ccsid = NULL;
  ccsid = getenv(attr);
  if (ccsid != NULL)
    SQLOverrideCCSID400(atoi(ccsid)); //CCSID customization.
  else
    SQLOverrideCCSID400(1208); // Run under CCSID 1208(UTF-8) by default.

  // Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnaenv.htm
  // allocates an environment handle and associated resources.
  // There can be only one active environment at any one time per application.
  SQLRETURN sqlReturnCode = SQLAllocEnv(&envh);

  if (sqlReturnCode != SQL_SUCCESS)
  {
    /* If SQL_ERROR is returned and phenv is equal to SQL_NULL_HENV, then SQLGetDiagRec() cannot be called
     * because there is no handle with which to associate additional diagnostic information.
     * If the return code is SQL_ERROR and the pointer to the environment handle is not equal to
     * SQL_NULL_HENV, then the handle is a restricted handle. This means the handle can only be used in a call
     * to SQLGetDiagRec() to obtain more error information, or to SQLFreeEnv().
     */
    printf("ERROR: SQLALLOCENV(%d)", sqlReturnCode);
    if (envh != SQL_NULL_HENV)
    {
      SQLFreeEnv(envh);
    }
  }
  // Doc https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/cli/rzadpfnsenva.htm
  sqlReturnCode = SQLSetEnvAttr(envh,                 //SQLHENV Environment handle
                                SQL_ATTR_SERVER_MODE, // SQLINTEGER Attribute - Enable Server Mode by default
                                &param,               // SQLPOINTER Value Appropriate value for Attribute
                                0);                   //SQLINTEGER StringLength of Value in bytes if the attribute value is a character string
  
  // SQL_ATTR_NON_HEXCCSID will change the job CCSID to the default job CCSID if
  // the the job CCSID is 65535. The default job CCSID is guaranteed to be a
  // non-65535 CCSID. This would help us avoid the common error:
  // "Character conversion between CCSID 1208 and CCSID 65535 not valid"
  sqlReturnCode = SQLSetEnvAttr(envh, SQL_ATTR_NON_HEXCCSID, &param, 0);

  DbConn::Init(env, exports, envh);
  DbStmt::Init(env, exports);
#endif

  // when not on PASE return a dummy object
  return exports;
}

NODE_API_MODULE(db2ia, InitAll)