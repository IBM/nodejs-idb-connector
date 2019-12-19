#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlcli.h"
#include "napi.h"

#define DEBUG(object, f_, ...)       \
    if (object->isDebug)             \
    {                                \
        printf((f_), ##__VA_ARGS__); \
    }

#define CHECK(condition, errorCode, errorMessage, env)      \
    if ((condition))                                        \
    {                                                       \
        throwCustomMsg((errorCode), (errorMessage), (env)); \
        return;                                             \
    }

#define CHECK_WITH_RETURN(condition, errorCode, errorMessage, env, returnValue) \
    if ((condition))                                                            \
    {                                                                           \
        throwCustomMsg((errorCode), (errorMessage), (env));                     \
        return (returnValue);                                                   \
    }

typedef struct _sqlError
{
    SQLCHAR sqlState[SQL_SQLSTATE_SIZE + 1];
    SQLINTEGER sqlCode;
    SQLCHAR message[SQL_MAX_MESSAGE_LENGTH + 1];
    int sqlReturnCode;
} sqlError;

static sqlError returnErrObj(int handleType, SQLINTEGER handle)
{
    sqlError errObj;
    SQLSMALLINT length = 0;

    errObj.sqlReturnCode = SQLGetDiagRec(handleType,                 //SQLSMALLINT handleType -Handle Type
                                         handle,                     //SQLINTEGER handle -hadnle for info is wanted
                                         1,                          //SQLSMALLINT recNUM -Indicates which error record to return (if multiple)
                                         errObj.sqlState,            //SQLCHAR* szSQLSTATE -SQLSTATE as a string of 5 characters terminated by a null character. (Output)
                                         &errObj.sqlCode,            //SQLINTEGER* pfNativeError -Error Code (Output)
                                         errObj.message,             //SQLCHAR* szErrorMsg -Pointer to buffer msg text (Output)
                                         SQL_MAX_MESSAGE_LENGTH + 1, //SQLSMALLINT cbErorMsgMax -Max length of the buffer szErrorMsg
                                         &length);                   //SQLSMALLINT* pcbErrorMsg -Pointer total # bytes to return to szErrorMsg (Output)

    if (errObj.sqlReturnCode == SQL_SUCCESS)
        if (errObj.message[length - 1] == '\n')
            errObj.message[length - 1] = '\0';

    return errObj;
}

// In async threads, we cannot access Napi:Env to throw exceptions, but only print the error to the member string `msg`.
static void printErrorToLog(SQLINTEGER handle, char msg[])
{
    sqlError errObj = returnErrObj(SQL_HANDLE_STMT, handle);
    if (errObj.sqlReturnCode == SQL_SUCCESS)
        sprintf((char *)msg, "SQLSTATE=%s SQLCODE=%d %s", errObj.sqlState, (int)errObj.sqlCode, errObj.message);
}

//experimental way to actually return error messages in callbacks
static std::string returnErrMsg(int handleType, SQLINTEGER handle)
{
    SQLCHAR errMsg[SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10];
    std::string error;
    sqlError errObj = returnErrObj(handleType, handle);
    if (errObj.sqlReturnCode == SQL_SUCCESS)
        sprintf((char *)errMsg, "SQLSTATE=%s SQLCODE=%d %s", errObj.sqlState, (int)errObj.sqlCode, errObj.message);

    //  return Napi::String::New(env, errMsg).Utf8Value();
    error = errMsg;
    return error;
}

// In the main thread, we can throw the error message as a Javascript exeception.
static void throwErrMsg(int handleType, SQLINTEGER handle, Napi::Env env)
{
    std::string errorMessage = returnErrMsg(handleType, handle);
    Napi::Error::New(env, Napi::String::New(env, errorMessage)).ThrowAsJavaScriptException();
}

// Throw a customized Javascript exeception.
static void throwCustomMsg(int code, const char *msg, Napi::Env env)
{
    SQLCHAR errMsg[SQL_MAX_MESSAGE_LENGTH + SQL_SQLSTATE_SIZE + 10];
    sprintf((char *)errMsg, "SQLSTATE=PAERR SQLCODE=%d %s", code, msg);
    Napi::Error::New(env, Napi::String::New(env, errMsg)).ThrowAsJavaScriptException();
}

static bool SQLErrorEquals(int handleType, SQLINTEGER handle, const char *sqlState, SQLINTEGER sqlCode)
{
    sqlError errObj = returnErrObj(handleType, handle);
    return errObj.sqlCode == sqlCode && !strncmp(errObj.sqlState, sqlState, SQL_SQLSTATE_SIZE);
}