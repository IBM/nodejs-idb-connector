/*  The Source code for this program is not published or otherwise  
 *  divested of its trade secrets, irrespective of what has been
 *  deposited with the U.S. Copyright Office.                        
*/

#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlcli.h"
#include <as400_protos.h> // For SQLOverrideCCSID400()
#include "napi.h"

#include "dberror.h"

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

class DbConn : public Napi::ObjectWrap<DbConn>
{
  friend class DbStmt;

public:
  DbConn(const Napi::CallbackInfo &info);
  static Napi::Object Init(Napi::Env env, Napi::Object exports, SQLHENV envh);
  static Napi::Object NewInstance(Napi::Value arg);

private:
  bool connAllocated = false;
  bool connected = false;
  bool isDebug = false;
  Napi::Value SetConnAttr(const Napi::CallbackInfo &info);
  Napi::Value GetConnAttr(const Napi::CallbackInfo &info);
  void Conn(const Napi::CallbackInfo &info);
  Napi::Value Disconnect(const Napi::CallbackInfo &info);
  Napi::Value Close(const Napi::CallbackInfo &info);
  Napi::Value ValidStmt(const Napi::CallbackInfo &info);
  Napi::Value Debug(const Napi::CallbackInfo &info);
  Napi::Value IsConnected(const Napi::CallbackInfo &info);
  Napi::Value GetJobName(const Napi::CallbackInfo &info);

  static Napi::FunctionReference constructor;
  static SQLHENV envh;
  SQLHDBC connh;
};