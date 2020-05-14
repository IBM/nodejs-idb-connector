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

static const char* getSQLType(int sqlType)
{
    switch (sqlType)
    {
    case SQL_CHAR:          // SQL_CHAR(SQL_CODE_DATE) = 1
        return "CHAR";
    case SQL_NUMERIC:       // SQL_NUMERIC(SQL_CODE_TIME) = 2
        return "NUMERIC";
    case SQL_DECIMAL:       // SQL_DECIMAL(SQL_CODE_TIMESTAMP) = 3
        return "DECIMAL";
    case SQL_INTEGER:       // SQL_INTEGER = 4
        return "INTEGER";
    case SQL_SMALLINT:      // SQL_SMALLINT = 5
        return "SMALLINT";
    case SQL_FLOAT:         // SQL_FLOAT = 6
        return "FLOAT";
    case SQL_REAL:          // SQL_REAL = 7
        return "REAL";
    case SQL_DOUBLE:        // SQL_DOUBLE = 8
        return "DOUBLE";
    case SQL_DATETIME:      // SQL_DATETIME = 9
        return "DATETIME";
    case SQL_VARCHAR:       // SQL_VARCHAR(SQL_LONGVARCHAR) = 12
        return "VARCHAR";
    case SQL_BLOB:          // SQL_BLOB = 13
        return "BLOB";
    case SQL_CLOB:          // SQL_CLOB = 14
        return "CLOB";
    case SQL_DBCLOB:        // SQL_DBCLOB = 15
        return "DBCLOB";
    case SQL_DATALINK:      // SQL_DATALINK = 16
        return "DATALINK";
    case SQL_WCHAR:         // SQL_WCHAR = 17
        return "WCHAR";
    case SQL_WVARCHAR:      // SQL_WVARCHAR(SQL_WLONGVARCHAR) = 18
        return "WVARCHAR";
    case SQL_BIGINT:        // SQL_BIGINT = 19
        return "BIGINT";
    case SQL_BLOB_LOCATOR:  // SQL_BLOB_LOCATOR = 20
        return "BLOB_LOCATOR";
    case SQL_CLOB_LOCATOR:  // SQL_CLOB_LOCATOR = 21
        return "CLOB_LOCATOR";
    case SQL_DBCLOB_LOCATOR:// SQL_DBCLOB_LOCATOR = 22
        return "DBCLOB_LOCATOR";
    case SQL_UTF8_CHAR:     // SQL_UTF8_CHAR = 23
        return "UTF8_CHAR";
    case SQL_GRAPHIC:       // SQL_GRAPHIC = 95
        return "GRAPHIC";
    case SQL_VARGRAPHIC:    // SQL_VARGRAPHIC(SQL_LONGVARGRAPHIC) = 96
        return "VARGRAPHIC";
    case SQL_BINARY:        // SQL_BINARY = -2
        return "BINARY";
    case SQL_VARBINARY:     // SQL_VARBINARY(SQL_LONGVARBINARY) = -3
        return "VARBINARY";
    case SQL_DATE:          // SQL_DATE(SQL_TYPE_DATE) = 91
        return "DATE";
    case SQL_TIME:          // SQL_TIME(SQL_TYPE_TIME) = 92
        return "TIME";
    case SQL_TIMESTAMP:     // SQL_TIMESTAMP(SQL_TYPE_TIMESTAMP) = 93
        return "TIMESTAMP";
    case SQL_ALL_TYPES:     // SQL_ALL_TYPES = 0
        return "ALL_TYPES";
    case SQL_DECFLOAT:      // SQL_DECFLOAT = -360
        return "DECFLOAT";
    case SQL_XML:           // SQL_XML = -370
        return "XML";
    default:
        return "UNKNOWN";
    }
}