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
 
#include <node.h>
#include "dbconn.h"

using namespace v8;

void CreateObject(const ARGUMENTS& args) {
  DbConn::NewInstance(args);
}

void InitAll(Handle<Object> exports) {
  DbConn::Init();
  NODE_SET_METHOD(exports, "db2i", CreateObject);
}

NODE_MODULE(db2i, InitAll)