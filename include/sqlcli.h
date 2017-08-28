/* begin_generated_IBM_copyright_prolog                              */
/* This is an automatically generated copyright prolog.              */
/* After initializing,  DO NOT MODIFY OR MOVE                        */
/* ----------------------------------------------------------------- */
/*                                                                   */
/* Product(s):                                                       */
/*     5716-SS1                                                      */
/*     5722-SS1                                                      */
/*     5761-SS1                                                      */
/*     5770-SS1                                                      */
/*                                                                   */
/* (C)Copyright IBM Corp.  1995, 2013                                */
/*                                                                   */
/* All rights reserved.                                              */
/* US Government Users Restricted Rights -                           */
/* Use, duplication or disclosure restricted                         */
/* by GSA ADP Schedule Contract with IBM Corp.                       */
/*                                                                   */
/* Licensed Materials-Property of IBM                                */
/*                                                                   */
/*  ---------------------------------------------------------------  */
/*                                                                   */
/* end_generated_IBM_copyright_prolog                                */
 /*** START HEADER FILE SPECIFICATIONS *****************************/
 /*                                                                */
 /* Header File Name: SQLCLI                                       */
 /*                                                                */
 /* Descriptive Name: Structured Query Language (SQL) Call Level   */
 /*                   Interface.                                   */
 /*                                                                */
 /* Description: The SQL Call Level Interface provides access to   */
 /*              most SQL functions, without the need for a        */
 /*              precompiler.                                      */
 /*                                                                */
 /* Header Files Included: SQLCLI                                  */
 /*                                                                */
 /*                                                                */
 /* Function Prototype List:  SQLAllocConnect                      */
 /*                           SQLAllocEnv                          */
 /*                           SQLAllocHandle                       */
 /*                           SQLAllocStmt                         */
 /*                           SQLBindCol                           */
 /*                           SQLBindFileToCol                     */
 /*                           SQLBindFileToParam                   */
 /*                           SQLBindParam                         */
 /*                           SQLBindParameter                     */
 /*                           SQLCancel                            */
 /*                           SQLCloseCursor                       */
 /*                           SQLColAttribute                      */
 /*                           SQLColAttributeW                     */
 /*                           SQLColAttributes                     */
 /*                           SQLColAttributesW                    */
 /*                           SQLColumnPrivileges                  */
 /*                           SQLColumnPrivilegesW                 */
 /*                           SQLColumns                           */
 /*                           SQLColumnsW                          */
 /*                           SQLConnect                           */
 /*                           SQLConnectW                          */
 /*                           SQLCopyDesc                          */
 /*                           SQLDataSources                       */
 /*                           SQLDataSourcesW                      */
 /*                           SQLDescribeCol                       */
 /*                           SQLDescribeColW                      */
 /*                           SQLDescribeParam                     */
 /*                           SQLDisconnect                        */
 /*                           SQLDriverConnect                     */
 /*                           SQLDriverConnectW                    */
 /*                           SQLEndTran                           */
 /*                           SQLError                             */
 /*                           SQLErrorW                            */
 /*                           SQLExecDirect                        */
 /*                           SQLExecDirectW                       */
 /*                           SQLExecute                           */
 /*                           SQLExtendedFetch                     */
 /*                           SQLFetch                             */
 /*                           SQLFetchScroll                       */
 /*                           SQLForeignKeys                       */
 /*                           SQLForeignKeysW                      */
 /*                           SQLFreeConnect                       */
 /*                           SQLFreeEnv                           */
 /*                           SQLFreeHandle                        */
 /*                           SQLFreeStmt                          */
 /*                           SQLGetCol                            */
 /*                           SQLGetConnectOption                  */
 /*                           SQLGetConnectOptionW                 */
 /*                           SQLGetCursorName                     */
 /*                           SQLGetCursorNameW                    */
 /*                           SQLGetConnectAttr                    */
 /*                           SQLGetConnectAttrW                   */
 /*                           SQLGetData                           */
 /*                           SQLGetDescField                      */
 /*                           SQLGetDescFieldW                     */
 /*                           SQLGetDescRec                        */
 /*                           SQLGetDescRecW                       */
 /*                           SQLGetDiagField                      */
 /*                           SQLGetDiagFieldW                     */
 /*                           SQLGetDiagRec                        */
 /*                           SQLGetDiagRecW                       */
 /*                           SQLGetEnvAttr                        */
 /*                           SQLGetFunctions                      */
 /*                           SQLGetInfo                           */
 /*                           SQLGetInfoW                          */
 /*                           SQLGetLength                         */
 /*                           SQLGetPosition                       */
 /*                           SQLGetPositionW                      */
 /*                           SQLGetStmtAttr                       */
 /*                           SQLGetStmtAttrW                      */
 /*                           SQLGetStmtOption                     */
 /*                           SQLGetStmtOptionW                    */
 /*                           SQLGetSubString                      */
 /*                           SQLGetSubStringW                     */
 /*                           SQLGetTypeInfo                       */
 /*                           SQLGetTypeInfoW                      */
 /*                           SQLLanguages                         */
 /*                           SQLMoreResults                       */
 /*                           SQLNativeSql                         */
 /*                           SQLNativeSqlW                        */
 /*                           SQLNextResult                        */
 /*                           SQLNumParams                         */
 /*                           SQLNumResultCols                     */
 /*                           SQLParamData                         */
 /*                           SQLParamOptions                      */
 /*                           SQLPrepare                           */
 /*                           SQLPrepareW                          */
 /*                           SQLPrimaryKeys                       */
 /*                           SQLPrimaryKeysW                      */
 /*                           SQLProcedureColumns                  */
 /*                           SQLProcedureColumnsW                 */
 /*                           SQLProcedures                        */
 /*                           SQLProceduresW                       */
 /*                           SQLPutData                           */
 /*                           SQLReleaseEnv                        */
 /*                           SQLRowCount                          */
 /*                           SQLSetConnectAttr                    */
 /*                           SQLSetConnectAttrW                   */
 /*                           SQLSetConnectOption                  */
 /*                           SQLSetConnectOptionW                 */
 /*                           SQLSetCursorName                     */
 /*                           SQLSetCursorNameW                    */
 /*                           SQLSetDescField                      */
 /*                           SQLSetDescFieldW                     */
 /*                           SQLSetDescRec                        */
 /*                           SQLSetEnvAttr                        */
 /*                           SQLSetParam                          */
 /*                           SQLSetStmtAttr                       */
 /*                           SQLSetStmtAttrW                      */
 /*                           SQLSetStmtOption                     */
 /*                           SQLSetStmtOptionW                    */
 /*                           SQLSpecialColumns                    */
 /*                           SQLSpecialColumnsW                   */
 /*                           SQLStartTran                         */
 /*                           SQLStatistics                        */
 /*                           SQLStatisticsW                       */
 /*                           SQLTablePrivileges                   */
 /*                           SQLTablePrivilegesW                  */
 /*                           SQLTables                            */
 /*                           SQLTablesW                           */
 /*                           SQLTransact                          */
 /*                                                                */
 /* Change Activity:                                               */
 /*                                                                */
 /* CFD List:                                                      */
 /*                                                                */
 /* FLAG REASON       LEVEL DATE   PGMR      CHANGE DESCRIPTION    */
 /* ---- ------------ ----- ------ --------- ----------------------*/
 /* $A0= D91823       3D60  941206 MEGERIAN  New Include           */
 /* $A1= D94881       4D20  960816 MEGERIAN  V4R2M0 enhancements   */
 /* $A2= D95600       4D30  970910 MEGERIAN  V4R3M0 enhancements   */
 /* $A3= P3682850     4D40  981030 MEGERIAN  V4R4M0 enhancements   */
 /* $A4= D97596       4D50  990326 LJAMESON  V4R5M0 enhancements   */
 /* $A5= P9924900     5D10  000512 MEGERIAN  V5R1M0 enhancements   */
 /* $C1= D98562       5D20  010107 MBAILEY   V5R2M0 enhancements   */
 /* $C2= D9856201     5D20  010506 MBAILEY   More enhancements     */
 /* $D1= P9A42663     5D30  031103 AJSLOMA   V5R3M0 enhancements   */
 /* $D2= P9A51843     5Q30  040102 ROCH      Larger Decimal support*/
 /* $D3= P9A61758     5D40  050517 AJSLOMA   V5R4M0 enhancements   */
 /* $D4= P9A72391     5P30  040622 ROCH      Formatting            */
 /* $D5= D99859       5D40  041104 HUEBERT   XA over DRDA          */
 /* $E1= D93586       5D50  060908 ROCH      Wide API support      */
 /* $E2= D93586       5D50  070320 ROCH      V5R5m0 enhancements   */
 /* $F1= D92300       7D10  090108 ROCH      Adding XML data type  */
 /* $F2= D92213       7D10  090202 ROCH      Currently committed   */
 /* $F3= FW481212     7D10  090911 ROCH      TinyInt Support and   */
 /*                                          JOBRUN sort sequence  */
 /* $G1=FW615707    v7r2m0f 140120 kadler    Add missing attrs, new*/
 /*                                          timestamp types       */
 /* End CFD List.                                                  */
 /*                                                                */
 /*  Additional notes about the Change Activity                    */
 /* End Change Activity.                                           */
 /*** END HEADER FILE SPECIFICATIONS *******************************/

#ifndef SQL_H_SQLCLI
  #define SQL_H_SQLCLI                /* Permit duplicate Includes */

  #if (__OS400_TGTVRM__>=510)  /* @B1A*/
   #pragma datamodel(P128)      /* @B1A*/
  #endif                       /* @B1A*/

  #ifdef __ILEC400__
    #pragma checkout(suspend)
    #pragma nomargins nosequence
  #else
    #pragma info(none)
  #endif

  #ifndef __SQL_EXTERN
     #ifdef __ILEC400__
       #define SQL_EXTERN extern
     #else
       #ifdef __cplusplus
         #ifdef __TOS_OS400__
           #define SQL_EXTERN extern "C nowiden"
         #else
           #define SQL_EXTERN extern "C"
         #endif
       #else
       #define SQL_EXTERN extern
       #endif /* __cplusplus */
     #endif /* __ILEC_400__ */
     #define __SQL_EXTERN
  #endif

  #ifdef __ILEC400__
    #pragma argument (SQLAllocConnect       , nowiden)
    #pragma argument (SQLAllocEnv           , nowiden)
    #pragma argument (SQLAllocHandle        , nowiden)
    #pragma argument (SQLAllocStmt          , nowiden)
    #pragma argument (SQLBindCol            , nowiden)
    #pragma argument (SQLBindFileToCol      , nowiden)
    #pragma argument (SQLBindFileToParam    , nowiden)
    #pragma argument (SQLBindParam          , nowiden)
    #pragma argument (SQLBindParameter      , nowiden)
    #pragma argument (SQLCancel             , nowiden)
    #pragma argument (SQLCloseCursor        , nowiden)
    #pragma argument (SQLColAttribute       , nowiden)
    #pragma argument (SQLColAttributeW      , nowiden)
    #pragma argument (SQLColAttributes      , nowiden)
    #pragma argument (SQLColAttributesW     , nowiden)
    #pragma argument (SQLColumnPrivileges   , nowiden)
    #pragma argument (SQLColumnPrivilegesW  , nowiden)
    #pragma argument (SQLColumns            , nowiden)
    #pragma argument (SQLColumnsW           , nowiden)
    #pragma argument (SQLConnect            , nowiden)
    #pragma argument (SQLConnectW           , nowiden)
    #pragma argument (SQLCopyDesc           , nowiden)
    #pragma argument (SQLDataSources        , nowiden)
    #pragma argument (SQLDataSourcesW       , nowiden)
    #pragma argument (SQLDescribeCol        , nowiden)
    #pragma argument (SQLDescribeColW       , nowiden)
    #pragma argument (SQLDescribeParam      , nowiden)
    #pragma argument (SQLDisconnect         , nowiden)
    #pragma argument (SQLDriverConnect      , nowiden)
    #pragma argument (SQLDriverConnectW     , nowiden)
    #pragma argument (SQLEndTran            , nowiden)
    #pragma argument (SQLError              , nowiden)
    #pragma argument (SQLErrorW             , nowiden)
    #pragma argument (SQLExecDirect         , nowiden)
    #pragma argument (SQLExecDirectW        , nowiden)
    #pragma argument (SQLExecute            , nowiden)
    #pragma argument (SQLExecuteW           , nowiden)
    #pragma argument (SQLExtendedFetch      , nowiden)
    #pragma argument (SQLFetch              , nowiden)
    #pragma argument (SQLFetchScroll        , nowiden)
    #pragma argument (SQLForeignKeys        , nowiden)
    #pragma argument (SQLForeignKeysW       , nowiden)
    #pragma argument (SQLFreeConnect        , nowiden)
    #pragma argument (SQLFreeEnv            , nowiden)
    #pragma argument (SQLFreeHandle         , nowiden)
    #pragma argument (SQLFreeStmt           , nowiden)
    #pragma argument (SQLGetCol             , nowiden)
    #pragma argument (SQLGetColW            , nowiden)
    #pragma argument (SQLGetConnectOption   , nowiden)
    #pragma argument (SQLGetConnectOptionW  , nowiden)
    #pragma argument (SQLGetCursorName      , nowiden)
    #pragma argument (SQLGetCursorNameW     , nowiden)
    #pragma argument (SQLGetConnectAttr     , nowiden)
    #pragma argument (SQLGetConnectAttrW    , nowiden)
    #pragma argument (SQLGetData            , nowiden)
    #pragma argument (SQLGetDescField       , nowiden)
    #pragma argument (SQLGetDescFieldW      , nowiden)
    #pragma argument (SQLGetDescRec         , nowiden)
    #pragma argument (SQLGetDescRecW        , nowiden)
    #pragma argument (SQLGetDiagField       , nowiden)
    #pragma argument (SQLGetDiagFieldW      , nowiden)
    #pragma argument (SQLGetDiagRec         , nowiden)
    #pragma argument (SQLGetDiagRecW        , nowiden)
    #pragma argument (SQLGetEnvAttr         , nowiden)
    #pragma argument (SQLGetFunctions       , nowiden)
    #pragma argument (SQLGetInfo            , nowiden)
    #pragma argument (SQLGetInfoW           , nowiden)
    #pragma argument (SQLGetLength          , nowiden)
    #pragma argument (SQLGetPosition        , nowiden)
    #pragma argument (SQLGetPositionW       , nowiden)
    #pragma argument (SQLGetStmtAttr        , nowiden)
    #pragma argument (SQLGetStmtAttrW       , nowiden)
    #pragma argument (SQLGetStmtOption      , nowiden)
    #pragma argument (SQLGetStmtOptionW     , nowiden)
    #pragma argument (SQLGetSubString       , nowiden)
    #pragma argument (SQLGetSubStringW      , nowiden)
    #pragma argument (SQLGetTypeInfo        , nowiden)
    #pragma argument (SQLGetTypeInfoW       , nowiden)
    #pragma argument (SQLLanguages          , nowiden)
    #pragma argument (SQLMoreResults        , nowiden)
    #pragma argument (SQLNativeSql          , nowiden)
    #pragma argument (SQLNativeSqlW         , nowiden)
    #pragma argument (SQLNextResult         , nowiden)
    #pragma argument (SQLNumParams          , nowiden)
    #pragma argument (SQLNumResultCols      , nowiden)
    #pragma argument (SQLParamData          , nowiden)
    #pragma argument (SQLParamOptions       , nowiden)
    #pragma argument (SQLPrepare            , nowiden)
    #pragma argument (SQLPrepareW           , nowiden)
    #pragma argument (SQLPrimaryKeys        , nowiden)
    #pragma argument (SQLPrimaryKeysW       , nowiden)
    #pragma argument (SQLProcedureColumns   , nowiden)
    #pragma argument (SQLProcedureColumnsW  , nowiden)
    #pragma argument (SQLProcedures         , nowiden)
    #pragma argument (SQLProceduresW        , nowiden)
    #pragma argument (SQLPutData            , nowiden)
    #pragma argument (SQLReleaseEnv         , nowiden)
    #pragma argument (SQLRowCount           , nowiden)
    #pragma argument (SQLSetConnectAttr     , nowiden)
    #pragma argument (SQLSetConnectAttrW    , nowiden)
    #pragma argument (SQLSetConnectOption   , nowiden)
    #pragma argument (SQLSetConnectOptionW  , nowiden)
    #pragma argument (SQLSetCursorName      , nowiden)
    #pragma argument (SQLSetCursorNameW     , nowiden)
    #pragma argument (SQLSetDescField       , nowiden)
    #pragma argument (SQLSetDescFieldW      , nowiden)
    #pragma argument (SQLSetDescRec         , nowiden)
    #pragma argument (SQLSetEnvAttr         , nowiden)
    #pragma argument (SQLSetParam           , nowiden)
    #pragma argument (SQLSetStmtAttr        , nowiden)
    #pragma argument (SQLSetStmtAttrW       , nowiden)
    #pragma argument (SQLSetStmtOption      , nowiden)
    #pragma argument (SQLSetStmtOptionW     , nowiden)
    #pragma argument (SQLSpecialColumns     , nowiden)
    #pragma argument (SQLSpecialColumnsW    , nowiden)
    #pragma argument (SQLStartTran          , nowiden)
    #pragma argument (SQLStatistics         , nowiden)
    #pragma argument (SQLStatisticsW        , nowiden)
    #pragma argument (SQLTablePrivileges    , nowiden)
    #pragma argument (SQLTablePrivilegesW   , nowiden)
    #pragma argument (SQLTables             , nowiden)
    #pragma argument (SQLTablesW            , nowiden)
    #pragma argument (SQLTransact           , nowiden)
  #endif

/* generally useful constants */
#define SQL_FALSE                 0
#define SQL_TRUE                  1
#define SQL_NTS                  -3  /* NTS = Null Terminated String    */
#define SQL_SQLSTATE_SIZE         5  /* size of SQLSTATE, not including
                                         null terminating byte           */
#define SQL_MAX_MESSAGE_LENGTH    512
#define SQL_MAX_OPTION_STRING_LENGTH 128

/* RETCODE values             */
/* Note: The return codes will reflect the XA return code specifications,
   when using CLI to execute XA transactions (use of the
   SQLSetConnectAttr - SQL_ATTR_TXN_INFO attribute).
   The XA return codes can be found in the XA.h include file.        @D3A*/
#define SQL_SUCCESS             0
#define SQL_SUCCESS_WITH_INFO   1
#define SQL_NO_DATA_FOUND       100
#define SQL_NEED_DATA           99
#define SQL_NO_DATA             SQL_NO_DATA_FOUND
#define SQL_ERROR              -1
#define SQL_INVALID_HANDLE     -2
#define SQL_STILL_EXECUTING     2

/* SQLFreeStmt option values  */
#define SQL_CLOSE               0
#define SQL_DROP                1
#define SQL_UNBIND              2
#define SQL_RESET_PARAMS        3

/* SQLSetParam defines        */
#define SQL_C_DEFAULT          99

/* SQLEndTran option values  */
#define SQL_COMMIT              0
#define SQL_ROLLBACK            1
#define SQL_COMMIT_HOLD         2
#define SQL_ROLLBACK_HOLD       3
#define SQL_SAVEPOINT_NAME_RELEASE  4
#define SQL_SAVEPOINT_NAME_ROLLBACK 5

/* SQLDriverConnect option values  */
#define SQL_DRIVER_COMPLETE          1
#define SQL_DRIVER_COMPLETE_REQUIRED 1
#define SQL_DRIVER_NOPROMPT          1
#define SQL_DRIVER_PROMPT            0

/*  Valid option codes for GetInfo procedure               */
#define SQL_ACTIVE_CONNECTIONS     0
#define SQL_MAX_DRIVER_CONNECTIONS 0
#define SQL_MAX_CONCURRENT_ACTIVITIES 1
#define SQL_ACTIVE_STATEMENTS         1
#define SQL_PROCEDURES            2
#define SQL_DRIVER_NAME           6                /* @C1A*/
#define SQL_ODBC_API_CONFORMANCE  9                /* @C1A*/
#define SQL_ODBC_SQL_CONFORMANCE  10               /* @C1A*/
#define SQL_DBMS_NAME             17
#define SQL_DBMS_VER              18
#define SQL_DRIVER_VER            18
#define SQL_IDENTIFIER_CASE       28               /* @C1A*/
#define SQL_IDENTIFIER_QUOTE_CHAR 29               /* @C1A*/
#define SQL_MAX_COLUMN_NAME_LEN   30
#define SQL_MAX_CURSOR_NAME_LEN   31
#define SQL_MAX_OWNER_NAME_LEN    32
#define SQL_MAX_SCHEMA_NAME_LEN   33
#define SQL_MAX_TABLE_NAME_LEN    35
#define SQL_MAX_COLUMNS_IN_GROUP_BY 36
#define SQL_MAX_COLUMNS_IN_ORDER_BY 37
#define SQL_MAX_COLUMNS_IN_SELECT   38
#define SQL_MAX_COLUMNS_IN_TABLE    39
#define SQL_MAX_TABLES_IN_SELECT    40
#define SQL_COLUMN_ALIAS            41
#define SQL_DATA_SOURCE_NAME        42
#define SQL_DATASOURCE_NAME         42
#define SQL_MAX_COLUMNS_IN_INDEX    43
#define SQL_PROCEDURE_TERM          44             /* @C1A*/
#define SQL_QUALIFIER_TERM          45             /* @C1A*/
#define SQL_TXN_CAPABLE             46             /* @C1A*/
#define SQL_OWNER_TERM              47             /* @C1A*/
#define SQL_DATA_SOURCE_READ_ONLY   48             /* @C2A*/
#define SQL_DEFAULT_TXN_ISOLATION   49             /* @C2A*/
#define SQL_MULTIPLE_ACTIVE_TXN     55             /* @C2A*/
#define SQL_QUALIFIER_NAME_SEPARATOR 65            /* @C2A*/
#define SQL_CORRELATION_NAME        74             /* @C1A*/
#define SQL_NON_NULLABLE_COLUMNS    75             /* @C1A*/
#define SQL_DRIVER_ODBC_VER         77             /* @C1A*/
#define SQL_GROUP_BY                88             /* @C1A*/
#define SQL_ORDER_BY_COLUMNS_IN_SELECT  90         /* @C1A*/
#define SQL_OWNER_USAGE                 91         /* @C1A*/
#define SQL_QUALIFIER_USAGE             92         /* @C1A*/
#define SQL_QUOTED_IDENTIFIER_CASE      93         /* @C1A*/
#define SQL_MAX_ROW_SIZE               104         /* @C1A*/
#define SQL_QUALIFIER_LOCATION         114         /* @C1A*/
#define SQL_MAX_CATALOG_NAME_LEN       115
#define SQL_MAX_STATEMENT_LEN          116
#define SQL_SEARCH_PATTERN_ESCAPE      117
#define SQL_OUTER_JOINS                118
#define SQL_LIKE_ESCAPE_CLAUSE         119
#define SQL_CATALOG_NAME               120
#define SQL_DESCRIBE_PARAMETER         121
#define SQL_STRING_FUNCTIONS            50
#define SQL_NUMERIC_FUNCTIONS           51
#define SQL_CONVERT_FUNCTIONS           52
#define SQL_TIMEDATE_FUNCTIONS          53
#define SQL_SQL92_PREDICATES           160
#define SQL_SQL92_VALUE_EXPRESSIONS    165
#define SQL_AGGREGATE_FUNCTIONS        169
#define SQL_SQL_CONFORMANCE            170
#define SQL_CONVERT_CHAR               171
#define SQL_CONVERT_NUMERIC            172
#define SQL_CONVERT_DECIMAL            173
#define SQL_CONVERT_INTEGER            174
#define SQL_CONVERT_SMALLINT           175
#define SQL_CONVERT_FLOAT              176
#define SQL_CONVERT_REAL               177
#define SQL_CONVERT_DOUBLE             178
#define SQL_CONVERT_VARCHAR            179
#define SQL_CONVERT_LONGVARCHAR        180
#define SQL_CONVERT_BINARY             181
#define SQL_CONVERT_VARBINARY          182
#define SQL_CONVERT_BIT                183
#define SQL_CONVERT_TINYINT            184
#define SQL_CONVERT_BIGINT             185
#define SQL_CONVERT_DATE               186
#define SQL_CONVERT_TIME               187
#define SQL_CONVERT_TIMESTAMP          188
#define SQL_CONVERT_LONGVARBINARY      189
#define SQL_CONVERT_INTERVAL_YEAR_MONTH 190
#define SQL_CONVERT_INTERVAL_DAY_TIME   191
#define SQL_CONVERT_WCHAR               192
#define SQL_CONVERT_WLONGVARCHAR        193
#define SQL_CONVERT_WVARCHAR            194
#define SQL_CONVERT_BLOB                195
#define SQL_CONVERT_CLOB                196
#define SQL_CONVERT_DBCLOB              197
#define SQL_CURSOR_COMMIT_BEHAVIOR      198
#define SQL_CURSOR_ROLLBACK_BEHAVIOR    199
#define SQL_POSITIONED_STATEMENTS       200
#define SQL_KEYWORDS                    201
#define SQL_CONNECTION_JOB_NAME         202
#define SQL_USER_NAME                   203        /* @D3A*/
#define SQL_DATABASE_NAME               204        /* @D3A*/
#define SQL_CONVERT_DECFLOAT7           205        /* @E2A*/
#define SQL_CONVERT_DECFLOAT16          206        /* @E2A*/
#define SQL_CONVERT_DECFLOAT34          207        /* @E2A*/


/* Unsupported codes for SQLGetInfo  */

#define SQL_LOCK_TYPES                  -1
#define SQL_POS_OPERATIONS              -1

/* Output values for cursor behavior  */

#define SQL_CB_DELETE                    1
#define SQL_CB_CLOSE                     2
#define SQL_CB_PRESERVE                  3


/* Aliased option codes (ODBC 3.0)                     @C1A*/
#define SQL_SCHEMA_TERM      SQL_OWNER_TERM        /* @C1A*/
#define SQL_SCHEMA_USAGE     SQL_OWNER_USAGE       /* @C1A*/
#define SQL_CATALOG_LOCATION SQL_QUALIFIER_LOCATION /*@C1A*/
#define SQL_CATALOG_TERM     SQL_QUALIFIER_TERM    /* @C1A*/
#define SQL_CATALOG_USAGE    SQL_QUALIFIER_USAGE   /* @C1A*/
#define SQL_CATALOG_NAME_SEPARATOR SQL_QUALIFIER_NAME_SEPARATOR
                                                    /* @C2A*/

/*
 * Output values for SQL_ODBC_API_CONFORMANCE
 * info type in SQLGetInfo
 */
#define SQL_OAC_NONE            0                  /* @C1A*/
#define SQL_OAC_LEVEL1          1                  /* @C1A*/
#define SQL_OAC_LEVEL2          2                  /* @C1A*/

/*
 * Output values for SQL_ODBC_SQL_CONFORMANCE
 * info type in SQLGetInfo
 */
#define SQL_OSC_MINIMUM         0                  /* @C1A*/
#define SQL_OSC_CORE            1                  /* @C1A*/
#define SQL_OSC_EXTENDED        2                  /* @C1A*/

/*
 * Output values for SQL_QUALIFIER_USAGE
 * info type in SQLGetInfo
 */
#define SQL_QU_NOT_SUPPORTED           0x00000000  /* @C1A*/
#define SQL_QU_DML_STATEMENTS          0x00000001  /* @C1A*/
#define SQL_QU_PROCEDURE_INVOCATION    0x00000002  /* @C1A*/
#define SQL_QU_TABLE_DEFINITION        0x00000004  /* @C1A*/
#define SQL_QU_INDEX_DEFINITION        0x00000008  /* @C1A*/
#define SQL_QU_PRIVILEGE_DEFINITION    0x00000010  /* @C1A*/

/*
 * Output values for SQL_QUALIFIER_LOCATION
 * info type in SQLGetInfo
 */
#define SQL_QL_START            1                  /* @C1A*/
#define SQL_QL_END              2                  /* @C1A*/

/*
 * Output values for SQL_OWNER_USAGE
 * info type in SQLGetInfo
 */
#define SQL_OU_DML_STATEMENTS          0x00000001  /* @C1A*/
#define SQL_OU_PROCEDURE_INVOCATION    0x00000002  /* @C1A*/
#define SQL_OU_TABLE_DEFINITION        0x00000004  /* @C1A*/
#define SQL_OU_INDEX_DEFINITION        0x00000008  /* @C1A*/
#define SQL_OU_PRIVILEGE_DEFINITION    0x00000010  /* @C1A*/

/*
 * Output values for SQL_TXN_CAPABLE
 * info type in SQLGetInfo
 */
#define SQL_TC_NONE             0                  /* @C1A*/
#define SQL_TC_DML              1                  /* @C1A*/
#define SQL_TC_ALL              2                  /* @C1A*/
#define SQL_TC_DDL_COMMIT       3                  /* @C1A*/
#define SQL_TC_DDL_IGNORE       4                  /* @C1A*/

/*
 * Output values for SQL_DEFAULT_TXN_ISOLATION
 * info type in SQLGetInfo
 */
#define SQL_TXN_READ_UNCOMMITTED_MASK 0x00000001   /* @C2A*/
#define SQL_TXN_READ_COMMITTED_MASK   0x00000002   /* @C2A*/
#define SQL_TXN_REPEATABLE_READ_MASK  0x00000004   /* @C2A*/
#define SQL_TXN_SERIALIZABLE_MASK     0x00000008   /* @C2A*/


/*
 * Output values for SQL_STRING_FUNCTIONS
 * info type in SQLGetInfo
 */
#define SQL_FN_STR_CONCAT              0x00000001
#define SQL_FN_STR_UCASE               0x00000002
#define SQL_FN_STR_LCASE               0x00000004
#define SQL_FN_STR_SUBSTRING           0x00000008
#define SQL_FN_STR_LENGTH              0x00000010
#define SQL_FN_STR_POSITION            0x00000020
#define SQL_FN_STR_LTRIM               0x00000040
#define SQL_FN_STR_RTRIM               0x00000080

/*
 * Output values for SQL_POS_OPERATIONS
 * info type in SQLGetInfo (not currently supported)
 */
#define SQL_POS_POSITION               0x00000001
#define SQL_POS_REFRESH                0x00000002
#define SQL_POS_UPDATE                 0x00000004
#define SQL_POS_DELETE                 0x00000008
#define SQL_POS_ADD                    0x00000010


/*
 * Output values for SQL_NUMERIC_FUNCTIONS
 * info type in SQLGetInfo
 */
#define SQL_FN_NUM_ABS                      0x00000001
#define SQL_FN_NUM_ACOS                     0x00000002
#define SQL_FN_NUM_ASIN                     0x00000004
#define SQL_FN_NUM_ATAN                     0x00000008
#define SQL_FN_NUM_ATAN2                    0x00000010
#define SQL_FN_NUM_CEILING                  0x00000020
#define SQL_FN_NUM_COS                      0x00000040
#define SQL_FN_NUM_COT                      0x00000080
#define SQL_FN_NUM_EXP                      0x00000100
#define SQL_FN_NUM_FLOOR                    0x00000200
#define SQL_FN_NUM_LOG                      0x00000400
#define SQL_FN_NUM_MOD                      0x00000800
#define SQL_FN_NUM_SIGN                     0x00001000
#define SQL_FN_NUM_SIN                      0x00002000
#define SQL_FN_NUM_SQRT                     0x00004000
#define SQL_FN_NUM_TAN                      0x00008000
#define SQL_FN_NUM_PI                       0x00010000
#define SQL_FN_NUM_RAND                     0x00020000
#define SQL_FN_NUM_DEGREES                  0x00040000
#define SQL_FN_NUM_LOG10                    0x00080000
#define SQL_FN_NUM_POWER                    0x00100000
#define SQL_FN_NUM_RADIANS                  0x00200000
#define SQL_FN_NUM_ROUND                    0x00400000
#define SQL_FN_NUM_TRUNCATE                 0x00800000

/* SQL_SQL92_VALUE_EXPRESSIONS bitmasks */
#define SQL_SVE_CASE                    0x00000001
#define SQL_SVE_CAST                    0x00000002
#define SQL_SVE_COALESCE                0x00000004
#define SQL_SVE_NULLIF                  0x00000008

/* SQL_SQL92_PREDICATES bitmasks */
#define SQL_SP_EXISTS                   0x00000001
#define SQL_SP_ISNOTNULL                0x00000002
#define SQL_SP_ISNULL                   0x00000004
#define SQL_SP_MATCH_FULL               0x00000008
#define SQL_SP_MATCH_PARTIAL            0x00000010
#define SQL_SP_MATCH_UNIQUE_FULL        0x00000020
#define SQL_SP_MATCH_UNIQUE_PARTIAL     0x00000040
#define SQL_SP_OVERLAPS                 0x00000080
#define SQL_SP_UNIQUE                   0x00000100
#define SQL_SP_LIKE                     0x00000200
#define SQL_SP_IN                       0x00000400
#define SQL_SP_BETWEEN                  0x00000800
#define SQL_SP_COMPARISON               0x00001000
#define SQL_SP_QUANTIFIED_COMPARISON    0x00002000

/* SQL_AGGREGATE_FUNCTIONS bitmasks */
#define SQL_AF_AVG                      0x00000001
#define SQL_AF_COUNT                    0x00000002
#define SQL_AF_MAX                      0x00000004
#define SQL_AF_MIN                      0x00000008
#define SQL_AF_SUM                      0x00000010
#define SQL_AF_DISTINCT                 0x00000020
#define SQL_AF_ALL                      0x00000040

/* SQL_SQL_CONFORMANCE bitmasks */
#define SQL_SC_SQL92_ENTRY              0x00000001
#define SQL_SC_FIPS127_2_TRANSITIONAL   0x00000002
#define SQL_SC_SQL92_INTERMEDIATE       0x00000004
#define SQL_SC_SQL92_FULL               0x00000008

/* SQL_CONVERT_FUNCTIONS functions */
#define SQL_FN_CVT_CONVERT              0x00000001
#define SQL_FN_CVT_CAST                 0x00000002

/* SQL_POSITIONED_STATEMENTS bitmasks */
#define SQL_PS_POSITIONED_DELETE        0x00000001
#define SQL_PS_POSITIONED_UPDATE        0x00000002
#define SQL_PS_SELECT_FOR_UPDATE        0x00000004

/* SQL supported conversion bitmasks */
#define SQL_CVT_CHAR                        0x00000001
#define SQL_CVT_NUMERIC                     0x00000002
#define SQL_CVT_DECIMAL                     0x00000004
#define SQL_CVT_INTEGER                     0x00000008
#define SQL_CVT_SMALLINT                    0x00000010
#define SQL_CVT_FLOAT                       0x00000020
#define SQL_CVT_REAL                        0x00000040
#define SQL_CVT_DOUBLE                      0x00000080
#define SQL_CVT_VARCHAR                     0x00000100
#define SQL_CVT_LONGVARCHAR                 0x00000200
#define SQL_CVT_BINARY                      0x00000400
#define SQL_CVT_VARBINARY                   0x00000800
#define SQL_CVT_BIT                         0x00001000
#define SQL_CVT_TINYINT                     0x00002000
#define SQL_CVT_BIGINT                      0x00004000
#define SQL_CVT_DATE                        0x00008000
#define SQL_CVT_TIME                        0x00010000
#define SQL_CVT_TIMESTAMP                   0x00020000
#define SQL_CVT_LONGVARBINARY               0x00040000
#define SQL_CVT_INTERVAL_YEAR_MONTH         0x00080000
#define SQL_CVT_INTERVAL_DAY_TIME           0x00100000
#define SQL_CVT_WCHAR                       0x00200000
#define SQL_CVT_WLONGVARCHAR                0x00400000
#define SQL_CVT_WVARCHAR                    0x00800000
#define SQL_CVT_BLOB                        0x01000000
#define SQL_CVT_CLOB                        0x02000000
#define SQL_CVT_DBCLOB                      0x04000000
#define SQL_CVT_DECFLOAT7                   0x08000000 /* @E2A*/
#define SQL_CVT_DECFLOAT16                  0x10000000 /* @E2A*/
#define SQL_CVT_DECFLOAT34                  0x20000000 /* @E2A*/

/* SQL_TIMEDATE_FUNCTIONS bitmasks */
#define SQL_FN_TD_NOW                   0x00000001
#define SQL_FN_TD_CURDATE               0x00000002
#define SQL_FN_TD_DAYOFMONTH            0x00000004
#define SQL_FN_TD_DAYOFWEEK             0x00000008
#define SQL_FN_TD_DAYOFYEAR             0x00000010
#define SQL_FN_TD_MONTH                 0x00000020
#define SQL_FN_TD_QUARTER               0x00000040
#define SQL_FN_TD_WEEK                  0x00000080
#define SQL_FN_TD_YEAR                  0x00000100
#define SQL_FN_TD_CURTIME               0x00000200
#define SQL_FN_TD_HOUR                  0x00000400
#define SQL_FN_TD_MINUTE                0x00000800
#define SQL_FN_TD_SECOND                0x00001000
#define SQL_FN_TD_TIMESTAMPADD          0x00002000
#define SQL_FN_TD_TIMESTAMPDIFF         0x00004000
#define SQL_FN_TD_DAYNAME               0x00008000
#define SQL_FN_TD_MONTHNAME             0x00010000
#define SQL_FN_TD_CURRENT_DATE          0x00020000
#define SQL_FN_TD_CURRENT_TIME          0x00040000
#define SQL_FN_TD_CURRENT_TIMESTAMP     0x00080000
#define SQL_FN_TD_EXTRACT               0x00100000

/*
 * Output values for SQL_CORRELATION_NAME
 * info type in SQLGetInfo
 */
#define SQL_CN_NONE             0                  /* @C1A*/
#define SQL_CN_DIFFERENT        1                  /* @C1A*/
#define SQL_CN_ANY              2                  /* @C1A*/

/*
 * Output values for SQL_IDENTIFIER_CASE
 * info type in SQLGetInfo
 */
#define SQL_IC_UPPER            1                  /* @C1A*/
#define SQL_IC_LOWER            2                  /* @C1A*/
#define SQL_IC_SENSITIVE        3                  /* @C1A*/
#define SQL_IC_MIXED            4                  /* @C1A*/

/*
 * Output values for SQL_NON_NULLABLE_COLUMNS
 * info type in SQLGetInfo
 */
#define SQL_NNC_NULL            0                  /* @C1A*/
#define SQL_NNC_NON_NULL        1                  /* @C1A*/

/*
 * Output values for SQL_GROUP_BY
 * info type in SQLGetInfo
 */
#define SQL_GB_NO_RELATION              0          /* @C1A*/
#define SQL_GB_NOT_SUPPORTED            1          /* @C1A*/
#define SQL_GB_GROUP_BY_EQUALS_SELECT   2          /* @C1A*/
#define SQL_GB_GROUP_BY_CONTAINS_SELECT 3          /* @C1A*/

/* Standard SQL data types */
#define SQL_CHAR                1
#define SQL_NUMERIC             2
#define SQL_DECIMAL             3
#define SQL_INTEGER             4
#define SQL_SMALLINT            5
#define SQL_FLOAT               6
#define SQL_REAL                7
#define SQL_DOUBLE              8
#define SQL_DATETIME            9
#define SQL_VARCHAR            12
#define SQL_BLOB               13
#define SQL_CLOB               14
#define SQL_DBCLOB             15
#define SQL_DATALINK           16
#define SQL_WCHAR              17
#define SQL_WVARCHAR           18
#define SQL_BIGINT             19
#define SQL_BLOB_LOCATOR       20
#define SQL_CLOB_LOCATOR       21
#define SQL_DBCLOB_LOCATOR     22
#define SQL_UTF8_CHAR          23                  /* @D1A*/
#define SQL_WLONGVARCHAR       SQL_WVARCHAR
#define SQL_LONGVARCHAR        SQL_VARCHAR
#define SQL_GRAPHIC            95
#define SQL_VARGRAPHIC         96
#define SQL_LONGVARGRAPHIC     SQL_VARGRAPHIC
#define SQL_BINARY             -2
#define SQL_VARBINARY          -3
#define SQL_LONGVARBINARY      SQL_VARBINARY
#define SQL_DATE               91
#define SQL_TYPE_DATE          91
#define SQL_TIME               92
#define SQL_TYPE_TIME          92
#define SQL_TIMESTAMP          93
#define SQL_TYPE_TIMESTAMP     93
#define SQL_CODE_DATE           1
#define SQL_CODE_TIME           2
#define SQL_CODE_TIMESTAMP      3
#define SQL_ALL_TYPES           0
#define SQL_DECFLOAT         -360                  /* @E2A*/
#define SQL_XML              -370                  /* @F1A*/


/* Handle types  */
#define SQL_UNUSED                             0
#define SQL_HANDLE_ENV                         1
#define SQL_HANDLE_DBC                         2
#define SQL_HANDLE_STMT                        3
#define SQL_HANDLE_DESC                        4
#define SQL_NULL_HANDLE                        0

#define SQL_HANDLE_DBC_UNICODE               100

/*
 * NULL status defines; these are used in SQLColAttributes, SQLDescribeCol,
 * to describe the nullability of a column in a table.
 */
#define SQL_NO_NULLS         0
#define SQL_NULLABLE         1
#define SQL_NULLABLE_UNKNOWN 2

/* Special length values  */
#define SQL_NO_TOTAL         0
#define SQL_NULL_DATA       -1
#define SQL_DATA_AT_EXEC    -2
#define SQL_BIGINT_PREC     19
#define SQL_INTEGER_PREC    10
#define SQL_SMALLINT_PREC    5

/* SQLBindParam and SQLBindParameter Extended Indicator values  @E2A*/
#define SQL_DEFAULT_PARAM   -5
#define SQL_UNASSIGNED      -7

/* SQLColAttributes defines */
#define SQL_ATTR_READONLY            0
#define SQL_ATTR_WRITE               1
#define SQL_ATTR_READWRITE_UNKNOWN   2

/* Valid concurrency values */
#define SQL_CONCUR_LOCK              0
#define SQL_CONCUR_READ_ONLY         1
#define SQL_CONCUR_ROWVER            3
#define SQL_CONCUR_VALUES            4

/*  Valid environment attributes                      */
#define SQL_ATTR_OUTPUT_NTS          10001
#define SQL_ATTR_SYS_NAMING          10002
#define SQL_ATTR_DEFAULT_LIB         10003
#define SQL_ATTR_SERVER_MODE         10004
#define SQL_ATTR_JOB_SORT_SEQUENCE   10005
#define SQL_ATTR_ENVHNDL_COUNTER     10009
#define SQL_ATTR_ESCAPE_CHAR         10010
#define SQL_ATTR_INCLUDE_NULL_IN_LEN 10031
#define SQL_ATTR_UTF8                10032
#define SQL_ATTR_SYSCAP              10033
#define SQL_ATTR_REQUIRE_PROFILE     10034
#define SQL_ATTR_UCS2                10035
#define SQL_ATTR_TRUNCATION_RTNC     10036     /* @D1A*/

/*  Valid environment/connection attributes           */
#define SQL_ATTR_DATE_FMT            10020
#define SQL_ATTR_DATE_SEP            10021
#define SQL_ATTR_TIME_FMT            10022
#define SQL_ATTR_TIME_SEP            10023
#define SQL_ATTR_DECIMAL_SEP         10024
#define SQL_ATTR_TXN_INFO            10025
#define SQL_ATTR_TXN_EXTERNAL        10026
#define SQL_ATTR_2ND_LEVEL_TEXT      10027
#define SQL_ATTR_SAVEPOINT_NAME      10028
#define SQL_ATTR_TRACE               10029
#define SQL_ATTR_MAX_PRECISION       10040
#define SQL_ATTR_MAX_SCALE           10041
#define SQL_ATTR_MIN_DIVIDE_SCALE    10042
#define SQL_ATTR_HEX_LITERALS        10043
#define SQL_ATTR_CORRELATOR          10044    /* @D1A*/
#define SQL_ATTR_QUERY_OPTIMIZE_GOAL 10045    /* @D3A*/
#define SQL_ATTR_CONN_SORT_SEQUENCE  10046    /* @F3A*/
#define SQL_ATTR_PREFETCH            10100    /* @E1A*/
#define SQL_ATTR_CLOSEONEOF          10101    /* @E1A*/
#define SQL_ATTR_ANSI_APP            10102    /* @E1A*/
#define SQL_ATTR_INFO_USERID         10103    /* @E2A*/
#define SQL_ATTR_INFO_WRKSTNNAME     10104    /* @E2A*/
#define SQL_ATTR_INFO_APPLNAME       10105    /* @E2A*/
#define SQL_ATTR_INFO_ACCTSTR        10106    /* @E2A*/
#define SQL_ATTR_INFO_PROGRAMID      10107    /* @E2A*/
#define SQL_ATTR_DECFLOAT_ROUNDING_MODE 10112 /* @E2A*/
#define SQL_ATTR_OLD_MTADTA_BEHAVIOR 10113    /* @E2A*/
#define SQL_ATTR_NULL_REQUIRED       10114    /* @E2A*/
#define SQL_ATTR_FREE_LOCATORS       10115    /* @E2A*/
#define SQL_ATTR_EXTENDED_INDICATORS 10116    /* @E2A*/
#define SQL_ATTR_NULLT_ARRAY_RESULTS 10117    /* @G1A*/
#define SQL_ATTR_NULLT_OUTPUT_PARMS  10118    /* @G1A*/
#define SQL_ATTR_TIMESTAMP_PREC      10119    /* @G1A*/

#define SQL_ATTR_CONCURRENT_ACCESS_RESOLUTION 2595 /*@F2A*/

#define SQL_CONCURRENT_ACCESS_RESOLUTION_UNSET 0   /*@F2A*/
#define SQL_USE_CURRENTLY_COMMITTED            1   /*@F2A*/
#define SQL_WAIT_FOR_OUTCOME                   2   /*@F2A*/
#define SQL_SKIP_LOCKED_DATA                   3   /*@F2A*/

/* Valid transaction info operations                 */
/* Start Options                                     */
#define SQL_TXN_FIND     1         /* TMJOIN         */
#define SQL_TXN_CREATE   2         /* TMNOFLAGS      */
#define SQL_TXN_RESUME   7         /* TMRESUME   @D5A*/
/* End Options                                       */
#define SQL_TXN_CLEAR    3         /* TMSUSPEND      */
#define SQL_TXN_END      4         /* TMSUCCESS      */
                                   /*  w/o HOLD      */
#define SQL_TXN_HOLD     5         /* TMSUCCESS      */
                                   /*  w/HOLD    @D1A*/
#define SQL_TXN_END_FAIL 6         /* TMFAIL     @D5A*/

/*  Valid environment/connection values               */
#define SQL_FMT_ISO                  1
#define SQL_FMT_USA                  2
#define SQL_FMT_EUR                  3
#define SQL_FMT_JIS                  4
#define SQL_FMT_MDY                  5
#define SQL_FMT_DMY                  6
#define SQL_FMT_YMD                  7
#define SQL_FMT_JUL                  8
#define SQL_FMT_HMS                  9
#define SQL_FMT_JOB                  10
#define SQL_SEP_SLASH                1
#define SQL_SEP_DASH                 2
#define SQL_SEP_PERIOD               3
#define SQL_SEP_COMMA                4
#define SQL_SEP_BLANK                5
#define SQL_SEP_COLON                6
#define SQL_SEP_JOB                  7
#define SQL_HEX_IS_CHAR              1
#define SQL_HEX_IS_BINARY            2
#define SQL_FIRST_IO                 1                       /* @D3A*/
#define SQL_ALL_IO                   2                       /* @D3A*/

/*
 *  Options for Rounding Modes. These numeric values can
 *  be set with SQLSetConnectAttr() API for the attribute
 *  SQL_ATTR_DECFLOAT_ROUNDING_MODE. The SQLGetConnectAttr()
 *  API will return these values for the
 *  SQL_ATTR_DECFLOAT_ROUNDING_MODE attribute.                  @E2A*/
#define ROUND_HALF_EVEN              0                        /*  @E2A*/
#define ROUND_HALF_UP                1                        /*  @E2A*/
#define ROUND_DOWN                   2                        /*  @E2A*/
#define ROUND_CEILING                3                        /*  @E2A*/
#define ROUND_FLOOR                  4                        /*  @E2A*/
#define ROUND_HALF_DOWN              5                        /*  @E2A*/
#define ROUND_UP                     6                        /*  @E2A*/

/*  Valid values for type in GetCol                                 */
#define SQL_DEFAULT                 99
#define SQL_ARD_TYPE               -99

/*  Valid values for UPDATE_RULE and DELETE_RULE in SQLForeignKeys  */
#define SQL_CASCADE                  1
#define SQL_RESTRICT                 2
#define SQL_NO_ACTION                3
#define SQL_SET_NULL                 4
#define SQL_SET_DEFAULT              5

/* Valid values for result set column DEFERRABILITY in
   SQLForeignKeys  */
#define SQL_INITIALLY_DEFERRED       5                        /* @E2A*/
#define SQL_INITIALLY_IMMEDIATE      6                        /* @E2A*/
#define SQL_NOT_DEFERRABLE           7                        /* @E2A*/

/* Valid values for result set column PROCEDURE_TYPE in
   SQLProcedures   */
#define SQL_PT_UNKNOWN               0                        /* @E2A*/
#define SQL_PT_PROCEDURE             1                        /* @E2A*/
#define SQL_PT_FUNCTION              2                        /* @E2A*/

/*  Valid values for COLUMN_TYPE in SQLProcedureColumns  */
#define SQL_PARAM_INPUT              1
#define SQL_PARAM_OUTPUT             2
#define SQL_PARAM_INPUT_OUTPUT       3

   /* statement attributes */
#define SQL_ATTR_APP_ROW_DESC      10010
#define SQL_ATTR_APP_PARAM_DESC    10011
#define SQL_ATTR_IMP_ROW_DESC      10012
#define SQL_ATTR_IMP_PARAM_DESC    10013
#define SQL_ATTR_FOR_FETCH_ONLY    10014
#define SQL_ATTR_CONCURRENCY       10014
#define SQL_CONCURRENCY            10014
#define SQL_ATTR_CURSOR_SCROLLABLE 10015
#define SQL_ATTR_ROWSET_SIZE       10016
#define SQL_ROWSET_SIZE            10016
#define SQL_ATTR_ROW_ARRAY_SIZE    10016
#define SQL_ATTR_CURSOR_HOLD       10017
#define SQL_ATTR_FULL_OPEN         10018
#define SQL_ATTR_EXTENDED_COL_INFO 10019
#define SQL_ATTR_BIND_TYPE         10049
#define SQL_BIND_TYPE              10049
#define SQL_ATTR_CURSOR_TYPE       10050
#define SQL_CURSOR_TYPE            10050
#define SQL_ATTR_CURSOR_SENSITIVITY 10051                      /*  @D1A*/
#define SQL_CURSOR_SENSITIVE       10051                       /*  @D1A*/
#define SQL_ATTR_ROW_STATUS_PTR    10052                        /* @D3A*/
#define SQL_ATTR_ROWS_FETCHED_PTR  10053                        /* @D3A*/
#define SQL_ATTR_ROW_BIND_TYPE     10056                        /* @E2A*/
#define SQL_ATTR_PARAM_BIND_TYPE   10057                        /* @E2A*/
#define SQL_ATTR_PARAMSET_SIZE     10058                        /* @E2A*/
#define SQL_ATTR_PARAM_STATUS_PTR  10059                        /* @E2A*/
#define SQL_ATTR_PARAMS_PROCESSED_PTR  10060                    /* @E2A*/
#define SQL_ATTR_NUMBER_RESULTSET_ROWS_PTR 10061                /* @E2A*/

   /* values for setting statement attributes   */
#define SQL_BIND_BY_ROW              0
#define SQL_BIND_BY_COLUMN           1
#define SQL_CURSOR_FORWARD_ONLY      0
#define SQL_CURSOR_STATIC            1
#define SQL_CURSOR_DYNAMIC           2
#define SQL_CURSOR_KEYSET_DRIVEN     3
#define SQL_UNSPECIFIED              0                        /* @D1A*/
#define SQL_INSENSITIVE              1                        /* @D1A*/
#define SQL_SENSITIVE                2                        /* @D1A*/

   /*  Codes used in FetchScroll                                       */
#define SQL_FETCH_NEXT               1
#define SQL_FETCH_FIRST              2
#define SQL_FETCH_LAST               3
#define SQL_FETCH_PRIOR              4
#define SQL_FETCH_ABSOLUTE           5
#define SQL_FETCH_RELATIVE           6

/* SQLColAttributes defines */
#define SQL_DESC_COUNT                   1
#define SQL_DESC_TYPE                    2
#define SQL_DESC_LENGTH                  3
#define SQL_DESC_LENGTH_PTR              4
#define SQL_DESC_PRECISION               5
#define SQL_DESC_SCALE                   6
#define SQL_DESC_DATETIME_INTERVAL_CODE  7
#define SQL_DESC_NULLABLE                8
#define SQL_DESC_INDICATOR_PTR           9
#define SQL_DESC_DATA_PTR                10
#define SQL_DESC_NAME                    11
#define SQL_DESC_UNNAMED                 12
#define SQL_DESC_DISPLAY_SIZE            13
#define SQL_DESC_AUTO_INCREMENT          14
#define SQL_DESC_SEARCHABLE              15
#define SQL_DESC_UPDATABLE               16
#define SQL_DESC_BASE_COLUMN             17
#define SQL_DESC_BASE_TABLE              18
#define SQL_DESC_BASE_SCHEMA             19
#define SQL_DESC_LABEL                   20
#define SQL_DESC_MONEY                   21
#define SQL_DESC_TYPE_NAME               23                     /* @D3A*/
#define SQL_DESC_COLUMN_CCSID            24
#define SQL_DESC_ALLOC_TYPE              99
#define SQL_DESC_ALLOC_AUTO              1
#define SQL_DESC_ALLOC_USER              2

#define SQL_COLUMN_COUNT                   1
#define SQL_COLUMN_TYPE                    2
#define SQL_COLUMN_LENGTH                  3
#define SQL_COLUMN_LENGTH_PTR              4
#define SQL_COLUMN_PRECISION               5
#define SQL_COLUMN_SCALE                   6
#define SQL_COLUMN_DATETIME_INTERVAL_CODE  7
#define SQL_COLUMN_NULLABLE                8
#define SQL_COLUMN_INDICATOR_PTR           9
#define SQL_COLUMN_DATA_PTR                10
#define SQL_COLUMN_NAME                    11
#define SQL_COLUMN_UNNAMED                 12
#define SQL_COLUMN_DISPLAY_SIZE            13
#define SQL_COLUMN_AUTO_INCREMENT          14
#define SQL_COLUMN_SEARCHABLE              15
#define SQL_COLUMN_UPDATABLE               16
#define SQL_COLUMN_BASE_COLUMN             17
#define SQL_COLUMN_BASE_TABLE              18
#define SQL_COLUMN_BASE_SCHEMA             19
#define SQL_COLUMN_LABEL                   20
#define SQL_COLUMN_MONEY                   21
#define SQL_COLUMN_ALLOC_TYPE              99
#define SQL_COLUMN_ALLOC_AUTO              1
#define SQL_COLUMN_ALLOC_USER              2

/*  Valid codes for SpecialColumns procedure              */
#define SQL_SCOPE_CURROW         0
#define SQL_SCOPE_TRANSACTION    1
#define SQL_SCOPE_SESSION        2
#define SQL_PC_UNKNOWN           0
#define SQL_PC_NOT_PSEUDO        1
#define SQL_PC_PSEUDO            2

/*  Valid values for connect attribute                     */
#define SQL_ATTR_AUTO_IPD       10001
#define SQL_ATTR_ACCESS_MODE    10002
#define SQL_ACCESS_MODE         10002
#define SQL_ATTR_AUTOCOMMIT     10003
#define SQL_AUTOCOMMIT          10003
#define SQL_ATTR_DBC_SYS_NAMING 10004
#define SQL_ATTR_DBC_DEFAULT_LIB 10005
#define SQL_ATTR_ADOPT_OWNER_AUTH 10006
#define SQL_ATTR_SYSBAS_CMT     10007
#define SQL_ATTR_SET_SSA        10008               /* @D3A*/
#define SQL_HEX_SORT_SEQUENCE       0               /* @F3A*/
#define SQL_JOB_SORT_SEQUENCE       1               /* @F3A*/
#define SQL_JOBRUN_SORT_SEQUENCE    2               /* @F3A*/
#define SQL_ATTR_COMMIT             0
#define SQL_MODE_READ_ONLY          0
#define SQL_MODE_READ_WRITE         1
#define SQL_MODE_DEFAULT            1
#define SQL_AUTOCOMMIT_OFF          0
#define SQL_AUTOCOMMIT_ON           1
#define SQL_TXN_ISOLATION           0
#define SQL_ATTR_TXN_ISOLATION      0
#define SQL_COMMIT_NONE             1
#define SQL_TXN_NO_COMMIT           1
#define SQL_TXN_NOCOMMIT            1
#define SQL_COMMIT_CHG              2
#define SQL_COMMIT_UR               2
#define SQL_TXN_READ_UNCOMMITTED    2
#define SQL_COMMIT_CS               3
#define SQL_TXN_READ_COMMITTED      3
#define SQL_COMMIT_ALL              4
#define SQL_COMMIT_RS               4
#define SQL_TXN_REPEATABLE_READ     4
#define SQL_COMMIT_RR               5
#define SQL_TXN_SERIALIZABLE        5

/*  Valid index flags                         */
#define SQL_INDEX_UNIQUE            0
#define SQL_INDEX_ALL               1
#define SQL_INDEX_OTHER             3
#define SQL_TABLE_STAT              0
#define SQL_ENSURE                  1
#define SQL_QUICK                   0

/*  Valid trace values                        */
#define SQL_ATTR_TRACE_CLI       1
#define SQL_ATTR_TRACE_DBMON     2
#define SQL_ATTR_TRACE_DEBUG     4
#define SQL_ATTR_TRACE_JOBLOG    8
#define SQL_ATTR_TRACE_STRTRC    16

/*  Valid File Options                        */
#define SQL_FILE_READ               2
#define SQL_FILE_CREATE             8
#define SQL_FILE_OVERWRITE         16
#define SQL_FILE_APPEND            32

/* Valid types for GetDiagField                           */
#define SQL_DIAG_RETURNCODE         1
#define SQL_DIAG_NUMBER             2
#define SQL_DIAG_ROW_COUNT          3
#define SQL_DIAG_SQLSTATE           4
#define SQL_DIAG_NATIVE             5
#define SQL_DIAG_MESSAGE_TEXT       6
#define SQL_DIAG_DYNAMIC_FUNCTION   7
#define SQL_DIAG_CLASS_ORIGIN       8
#define SQL_DIAG_SUBCLASS_ORIGIN    9
#define SQL_DIAG_CONNECTION_NAME    10
#define SQL_DIAG_SERVER_NAME        11
#define SQL_DIAG_MESSAGE_TOKENS     12
#define SQL_DIAG_AUTOGEN_KEY        14

/*
 * SQLColAttributes defines
 * These are also used by SQLGetInfo
 */
#define SQL_UNSEARCHABLE             0
#define SQL_LIKE_ONLY                1
#define SQL_ALL_EXCEPT_LIKE          2
#define SQL_SEARCHABLE               3

/* GetFunctions() values to identify CLI functions   */
#define SQL_API_SQLALLOCCONNECT        1
#define SQL_API_SQLALLOCENV            2
#define SQL_API_SQLALLOCHANDLE      1001
#define SQL_API_SQLALLOCSTMT           3
#define SQL_API_SQLBINDCOL             4
#define SQL_API_SQLBINDFILETOCOL    2002
#define SQL_API_SQLBINDFILETOPARAM  2003
#define SQL_API_SQLBINDPARAM        1002
#define SQL_API_SQLBINDPARAMETER    1023
#define SQL_API_SQLCANCEL              5
#define SQL_API_SQLCLOSECURSOR      1003
#define SQL_API_SQLCOLATTRIBUTE        6
#define SQL_API_SQLCOLATTRIBUTEW    3001
#define SQL_API_SQLCOLATTRIBUTES   11006
#define SQL_API_SQLCOLATTRIBUTESW   3002
#define SQL_API_SQLCOLUMNPRIVILEGES 2010
#define SQL_API_SQLCOLUMNPRIVILEGESW 3003
#define SQL_API_SQLCOLUMNS            40
#define SQL_API_SQLCOLUMNSW         3004
#define SQL_API_SQLCONNECT             7
#define SQL_API_SQLCONNECTW         3005
#define SQL_API_SQLCOPYDESC         1004
#define SQL_API_SQLDATASOURCES        57
#define SQL_API_SQLDATASOURCESW     3006
#define SQL_API_SQLDESCRIBECOL         8
#define SQL_API_SQLDESCRIBECOLW     3007
#define SQL_API_SQLDESCRIBEPARAM      58
#define SQL_API_SQLDISCONNECT          9
#define SQL_API_SQLDRIVERCONNECT      68
#define SQL_API_SQLENDTRAN          1005
#define SQL_API_SQLERROR              10
#define SQL_API_SQLERRORW           3008
#define SQL_API_SQLEXECDIRECT         11
#define SQL_API_SQLEXECDIRECTW      3009
#define SQL_API_SQLEXECUTE            12         /* Add back in. @E1A*/
#define SQL_API_SQLEXTENDEDFETCH    1022
#define SQL_API_SQLFETCH              13
#define SQL_API_SQLFETCHSCROLL      1021
#define SQL_API_SQLFOREIGNKEYS        60
#define SQL_API_SQLFOREIGNKEYSW     3010
#define SQL_API_SQLFREECONNECT        14
#define SQL_API_SQLFREEENV            15
#define SQL_API_SQLFREEHANDLE       1006
#define SQL_API_SQLFREESTMT           16
#define SQL_API_SQLGETCOL             43
#define SQL_API_SQLGETCOLW          3011
#define SQL_API_SQLGETCONNECTATTR   1007
#define SQL_API_SQLGETCONNECTATTRW  3012
#define SQL_API_SQLGETCONNECTOPTION   42
#define SQL_API_SQLGETCONNECTOPTIONW 3013
#define SQL_API_SQLGETCURSORNAME      17
#define SQL_API_SQLGETCURSORNAMEW   3014
#define SQL_API_SQLGETDATA            43
#define SQL_API_SQLGETDESCFIELD     1008
#define SQL_API_SQLGETDESCFIELDW    3015
#define SQL_API_SQLGETDESCREC       1009
#define SQL_API_SQLGETDESCRECW      3016
#define SQL_API_SQLGETDIAGFIELD     1010
#define SQL_API_SQLGETDIAGFIELDW    3017
#define SQL_API_SQLGETDIAGREC       1011
#define SQL_API_SQLGETDIAGRECW      3018
#define SQL_API_SQLGETENVATTR       1012
#define SQL_API_SQLGETFUNCTIONS       44
#define SQL_API_SQLGETINFO            45
#define SQL_API_SQLGETINFOW         3019
#define SQL_API_SQLGETLENGTH        2004
#define SQL_API_SQLGETPOSITION      2005
#define SQL_API_SQLGETPOSITIONW     3020
#define SQL_API_SQLGETSTMTATTR      1014
#define SQL_API_SQLGETSTMTATTRW     3021
#define SQL_API_SQLGETSTMTOPTION      46
#define SQL_API_SQLGETSTMTOPTIONW   3022
#define SQL_API_SQLGETSUBSTRING     2006
#define SQL_API_SQLGETSUBSTRINGW    3023
#define SQL_API_SQLGETTYPEINFO        47
#define SQL_API_SQLGETTYPEINFOW     3024
#define SQL_API_SQLLANGUAGES        2001
#define SQL_API_SQLMORERESULTS        61
#define SQL_API_SQLNATIVESQL          62
#define SQL_API_SQLNATIVESQLW       3025
#define SQL_API_SQLNEXTRESULT       2009
#define SQL_API_SQLNUMPARAMS          63
#define SQL_API_SQLNUMRESULTCOLS      18
#define SQL_API_SQLPARAMDATA          48
#define SQL_API_SQLPARAMOPTIONS     2007
#define SQL_API_SQLPREPARE            19
#define SQL_API_SQLPREPAREW         3026
#define SQL_API_SQLPRIMARYKEYS        65
#define SQL_API_SQLPRIMARYKEYSW     3027
#define SQL_API_SQLPROCEDURECOLUMNS   66
#define SQL_API_SQLPROCEDURECOLUMNSW 3028
#define SQL_API_SQLPROCEDURES         67
#define SQL_API_SQLPROCEDURESW      3029
#define SQL_API_SQLPUTDATA            49
#define SQL_API_SQLRELEASEENV       1015
#define SQL_API_SQLROWCOUNT           20
#define SQL_API_SQLSETCONNECTATTR   1016
#define SQL_API_SQLSETCONNECTATTRW  3030
#define SQL_API_SQLSETCONNECTOPTION   50
#define SQL_API_SQLSETCONNECTOPTIONW 3031
#define SQL_API_SQLSETCURSORNAME      21
#define SQL_API_SQLSETCURSORNAMEW   3032
#define SQL_API_SQLSETDESCFIELD     1017
#define SQL_API_SQLSETDESCFIELDW    3033
#define SQL_API_SQLSETDESCREC       1018
#define SQL_API_SQLSETENVATTR       1019
#define SQL_API_SQLSETPARAM           22
#define SQL_API_SQLSETSTMTATTR      1020
#define SQL_API_SQLSETSTMTATTRW     3034
#define SQL_API_SQLSETSTMTOPTION      51
#define SQL_API_SQLSETSTMTOPTIONW   3035
#define SQL_API_SQLSPECIALCOLUMNS     52
#define SQL_API_SQLSPECIALCOLUMNSW  3036
#define SQL_API_SQLSTARTTRAN        2008
#define SQL_API_SQLSTATISTICS         53
#define SQL_API_SQLSTATISTICSW      3037
#define SQL_API_SQLTABLEPRIVILEGES  2011
#define SQL_API_SQLTABLEPRIVILEGESW 3038
#define SQL_API_SQLTABLES             54
#define SQL_API_SQLTABLESW          3039
#define SQL_API_SQLTRANSACT           23

/* unsupported APIs       */
#define SQL_API_SQLSETPOS             -1

/* NULL handle defines    */
#ifdef __64BIT__
#define SQL_NULL_HENV                0
#define SQL_NULL_HDBC                0
#define SQL_NULL_HSTMT               0
#else
#define SQL_NULL_HENV                0L
#define SQL_NULL_HDBC                0L
#define SQL_NULL_HSTMT               0L
#endif

#ifdef __64BIT__
#if !defined(SDWORD)
typedef int            SDWORD;
#endif
#if !defined(UDWORD)
typedef unsigned int   UDWORD;
#endif
#else
#if !defined(SDWORD)
typedef long int            SDWORD;
#endif
#if !defined(UDWORD)
typedef unsigned long int   UDWORD;
#endif
#endif
#if !defined(UWORD)
typedef unsigned short int  UWORD;
#endif
#if !defined(SWORD)
typedef signed short int    SWORD;
#endif

#include "sql.h"                       /* SQL definitions                 @E1M*/

/* This should be temporary until math.h makes the typedef's below permanent,
   without the need of STDC_WANT_DEC_FP or IBM_DFP declaration. Without this
   fix QCPIMPRT.c fails b/c it includes math.h w/out these declares
   set.                                                                   @E2A*/

#include "math.h"                      /* Decimal floating point types    @E2A*/

typedef  char              SQLCHAR;
typedef  wchar_t           SQLWCHAR;   /* W-API constant.                 @E1A*/
typedef  short    int      SQLSMALLINT;
typedef  UWORD             SQLUSMALLINT;
typedef  UDWORD            SQLUINTEGER;
typedef  double            SQLDOUBLE;
typedef  float             SQLREAL;

typedef  void *            PTR;
typedef  PTR               SQLPOINTER;

#ifdef __64BIT__
typedef  int              SQLINTEGER;
typedef  int              HENV;
typedef  int              HDBC;
typedef  int              HSTMT;
typedef  int              HDESC;
typedef  int              SQLHANDLE;
#else
typedef  long     int      SQLINTEGER;
typedef  long              HENV;
typedef  long              HDBC;
typedef  long              HSTMT;
typedef  long              HDESC;
typedef  long              SQLHANDLE;
#endif

typedef  HENV              SQLHENV;
typedef  HDBC              SQLHDBC;
typedef  HSTMT             SQLHSTMT;
typedef  HDESC             SQLHDESC;

typedef  SQLINTEGER        RETCODE;
typedef  RETCODE           SQLRETURN;

typedef  float             SFLOAT;

typedef SQLPOINTER         SQLHWND;

/*
 * DATE, TIME, and TIMESTAMP structures.  These are for compatibility
 * purposes only.  When actually specifying or retrieving DATE, TIME,
 * and TIMESTAMP values, character strings must be used.
 */
typedef struct DATE_STRUCT
{
    SQLSMALLINT    year;
    SQLUSMALLINT   month;
    SQLUSMALLINT   day;
} DATE_STRUCT;

typedef DATE_STRUCT SQL_DATE_STRUCT;

typedef struct TIME_STRUCT
{
    SQLUSMALLINT   hour;
    SQLUSMALLINT   minute;
    SQLUSMALLINT   second;
} TIME_STRUCT;

typedef TIME_STRUCT SQL_TIME_STRUCT;

typedef struct TIMESTAMP_STRUCT
{
    SQLSMALLINT    year;
    SQLUSMALLINT   month;
    SQLUSMALLINT   day;
    SQLUSMALLINT   hour;
    SQLUSMALLINT   minute;
    SQLUSMALLINT   second;
    SQLUINTEGER    fraction;     /* fraction of a second */
} TIMESTAMP_STRUCT;

typedef struct TIMESTAMP_STRUCT_EXT
{
    SQLSMALLINT    year;
    SQLUSMALLINT   month;
    SQLUSMALLINT   day;
    SQLUSMALLINT   hour;
    SQLUSMALLINT   minute;
    SQLUSMALLINT   second;
    SQLUINTEGER    fraction;     /* hold nanoseconds - 1~9 digit */
    SQLUINTEGER    fraction2;    /* hold picoseconds - 10~12 digit */
} TIMESTAMP_STRUCT_EXT;

typedef TIMESTAMP_STRUCT_EXT SQL_TIMESTAMP_STRUCT_EXT;

/* Additional decimal floating point constants                            @E2A*/
#define SQL_DECIMAL64_COEFFICIENT_LEN 8                                /* @E2A*/
#define SQL_DECIMAL128_COEFFICIENT_LEN 16                              /* @E2A*/

typedef struct tagSQLDECIMAL64
{
    union
    {
        SQLDOUBLE dummy;             /* Dummy member for alignment @E2A*/
        SQLCHAR dec64[SQL_DECIMAL64_COEFFICIENT_LEN];
        #if defined(__STDC_WANT_DEC_FP__) && \
        (__OS400_TGTVRM__ >= 550) && defined(__IBM_DFP__)
        _Decimal64  decfloat64;    /* Native DECFLOAT(16) type   @E2A*/
        #endif
    } udec64;
} SQLDECIMAL64;                             /*                            @E2A*/

typedef struct tagSQLDECIMAL128
{
    union
    {
        SQLDOUBLE dummy;             /* Dummy member for alignment @E2A*/
        SQLCHAR dec128[SQL_DECIMAL128_COEFFICIENT_LEN];
        #if defined(__STDC_WANT_DEC_FP__) && \
        (__OS400_TGTVRM__ >= 550) && defined(__IBM_DFP__)
        _Decimal128  decfloat128;  /* Native DECFLOAT(16) type   @E2A*/
        #endif
    } udec128;
} SQLDECIMAL128;                            /*                            @E2A*/

/* Transaction info structure                        */
typedef struct TXN_STRUCT {
    SQLINTEGER   operation;
    SQLCHAR      tminfo[10];
    SQLCHAR      reserved1[2];
    void         *XID;
    SQLINTEGER   timeoutval;
    SQLINTEGER   locktimeout;
    SQLCHAR      reserved2[8];
} TXN_STRUCT;

SQL_EXTERN SQLRETURN  SQLAllocConnect (SQLHENV              henv,
                                       SQLHDBC             *phdbc);


SQL_EXTERN SQLRETURN  SQLAllocEnv (SQLHENV             *phenv);

SQL_EXTERN SQLRETURN  SQLAllocHandle (SQLSMALLINT           htype,
                                      SQLINTEGER            ihnd,
                                      SQLINTEGER           *ohnd);

SQL_EXTERN SQLRETURN  SQLAllocStmt (SQLHDBC           hdbc,
                                    SQLHSTMT         *phstmt);

SQL_EXTERN SQLRETURN  SQLBindCol (SQLHSTMT          hstmt,
                                  SQLSMALLINT       icol,
                                  SQLSMALLINT       iType,
                                  SQLPOINTER        rgbValue,
                                  SQLINTEGER        cbValueMax,
                                  SQLINTEGER       *pcbValue);

SQL_EXTERN SQLRETURN  SQLBindFileToCol (SQLHSTMT          hstmt,
                                        SQLSMALLINT       icol,
                                        SQLCHAR          *fName,
                                        SQLSMALLINT      *fNameLen,
                                        SQLINTEGER       *fOptions,
                                        SQLSMALLINT       fValueMax,
                                        SQLINTEGER       *sLen,
                                        SQLINTEGER       *pcbValue);

SQL_EXTERN SQLRETURN  SQLBindFileToParam (SQLHSTMT          hstmt,
                                          SQLSMALLINT       ipar,
                                          SQLSMALLINT       iType,
                                          SQLCHAR          *fName,
                                          SQLSMALLINT      *fNameLen,
                                          SQLINTEGER       *fOptions,
                                          SQLSMALLINT       fValueMax,
                                          SQLINTEGER       *pcbValue);

SQL_EXTERN SQLRETURN  SQLBindParam (SQLHSTMT          hstmt,
                                    SQLSMALLINT       iparm,
                                    SQLSMALLINT       iType,
                                    SQLSMALLINT       pType,
                                    SQLINTEGER        pLen,
                                    SQLSMALLINT       pScale,
                                    SQLPOINTER        pData,
                                    SQLINTEGER       *pcbValue);

SQL_EXTERN SQLRETURN  SQLBindParameter (SQLHSTMT          hstmt,
                                        SQLSMALLINT       ipar,
                                        SQLSMALLINT       fParamType,
                                        SQLSMALLINT       fCType,
                                        SQLSMALLINT       fSQLType,
                                        SQLINTEGER        pLen,
                                        SQLSMALLINT       pScale,
                                        SQLPOINTER        pData,
                                        SQLINTEGER        cbValueMax,
                                        SQLINTEGER       *pcbValue);

SQL_EXTERN SQLRETURN  SQLCancel (SQLHSTMT          hstmt);

SQL_EXTERN SQLRETURN  SQLCloseCursor (SQLHSTMT          hstmt);

SQL_EXTERN SQLRETURN  SQLColAttribute (SQLHSTMT          hstmt,
                                       SQLSMALLINT       icol,
                                       SQLSMALLINT       fDescType,
                                       SQLPOINTER        rgbDesc,
                                       SQLSMALLINT       cbDescMax,
                                       SQLSMALLINT       *pcbDesc,
                                       SQLPOINTER        pfDesc);
                                                               /* @E1C*/

SQL_EXTERN SQLRETURN  SQLColAttributeW (SQLHSTMT          hstmt,
                                        SQLSMALLINT       icol,
                                        SQLSMALLINT       fDescType,
                                        SQLPOINTER        rgbDesc,
                                        SQLSMALLINT       cbDescMax,
                                        SQLSMALLINT       *pcbDesc,
                                        SQLPOINTER        pfDesc);
                                                               /* @E1C*/

SQL_EXTERN SQLRETURN  SQLColAttributes (SQLHSTMT          hstmt,
                                        SQLSMALLINT       icol,
                                        SQLSMALLINT       fDescType,
                                        SQLCHAR          *rgbDesc,
                                        SQLINTEGER        cbDescMax,
                                        SQLINTEGER       *pcbDesc,
                                        SQLINTEGER       *pfDesc);

SQL_EXTERN SQLRETURN  SQLColAttributesW (SQLHSTMT          hstmt,
                                         SQLSMALLINT       icol,
                                         SQLSMALLINT       fDescType,
                                         SQLWCHAR          *rgbDesc,
                                         SQLINTEGER        cbDescMax,
                                         SQLINTEGER       *pcbDesc,
                                         SQLINTEGER       *pfDesc);

SQL_EXTERN SQLRETURN   SQLColumnPrivileges (SQLHSTMT       hstmt,
                                            SQLCHAR        *szTableQualifier,
                                            SQLSMALLINT    cbTableQualifier,
                                            SQLCHAR        *szTableOwner,
                                            SQLSMALLINT    cbTableOwner,
                                            SQLCHAR        *szTableName,
                                            SQLSMALLINT    cbTableName,
                                            SQLCHAR        *szColumnName,
                                            SQLSMALLINT    cbColumnName);

SQL_EXTERN SQLRETURN   SQLColumnPrivilegesW (SQLHSTMT       hstmt,
                                             SQLWCHAR        *szTableQualifier,
                                             SQLSMALLINT    cbTableQualifier,
                                             SQLWCHAR        *szTableOwner,
                                             SQLSMALLINT    cbTableOwner,
                                             SQLWCHAR        *szTableName,
                                             SQLSMALLINT    cbTableName,
                                             SQLWCHAR        *szColumnName,
                                             SQLSMALLINT    cbColumnName);

SQL_EXTERN SQLRETURN   SQLColumns (SQLHSTMT       hstmt,
                                   SQLCHAR        *szTableQualifier,
                                   SQLSMALLINT    cbTableQualifier,
                                   SQLCHAR        *szTableOwner,
                                   SQLSMALLINT    cbTableOwner,
                                   SQLCHAR        *szTableName,
                                   SQLSMALLINT    cbTableName,
                                   SQLCHAR        *szColumnName,
                                   SQLSMALLINT    cbColumnName);

SQL_EXTERN SQLRETURN   SQLColumnsW (SQLHSTMT       hstmt,
                                    SQLWCHAR        *szTableQualifier,
                                    SQLSMALLINT    cbTableQualifier,
                                    SQLWCHAR        *szTableOwner,
                                    SQLSMALLINT    cbTableOwner,
                                    SQLWCHAR        *szTableName,
                                    SQLSMALLINT    cbTableName,
                                    SQLWCHAR        *szColumnName,
                                    SQLSMALLINT    cbColumnName);

SQL_EXTERN SQLRETURN  SQLConnect (SQLHDBC           hdbc,
                                  SQLCHAR          *szDSN,
                                  SQLSMALLINT       cbDSN,
                                  SQLCHAR          *szUID,
                                  SQLSMALLINT       cbUID,
                                  SQLCHAR          *szAuthStr,
                                  SQLSMALLINT       cbAuthStr);

SQL_EXTERN SQLRETURN  SQLConnectW (SQLHDBC           hdbc,
                                   SQLWCHAR          *szDSN,
                                   SQLSMALLINT       cbDSN,
                                   SQLWCHAR          *szUID,
                                   SQLSMALLINT       cbUID,
                                   SQLWCHAR          *szAuthStr,
                                   SQLSMALLINT       cbAuthStr);


SQL_EXTERN SQLRETURN  SQLCopyDesc (SQLHDESC   sDesc,
                                   SQLHDESC   tDesc);

SQL_EXTERN SQLRETURN  SQLDataSources (SQLHENV       henv,
                                      SQLSMALLINT   fDirection,
                                      SQLCHAR      *szDSN,
                                      SQLSMALLINT   cbDSNMax,
                                      SQLSMALLINT  *pcbDSN,
                                      SQLCHAR      *szDescription,
                                      SQLSMALLINT   cbDescriptionMax,
                                      SQLSMALLINT  *pcbDescription);

SQL_EXTERN SQLRETURN  SQLDataSourcesW (SQLHENV       henv,
                                       SQLSMALLINT   fDirection,
                                       SQLWCHAR      *szDSN,
                                       SQLSMALLINT   cbDSNMax,
                                       SQLSMALLINT  *pcbDSN,
                                       SQLWCHAR      *szDescription,
                                       SQLSMALLINT   cbDescriptionMax,
                                       SQLSMALLINT  *pcbDescription);

SQL_EXTERN SQLRETURN  SQLDescribeCol (SQLHSTMT          hstmt,
                                      SQLSMALLINT       icol,
                                      SQLCHAR          *szColName,
                                      SQLSMALLINT       cbColNameMax,
                                      SQLSMALLINT      *pcbColName,
                                      SQLSMALLINT      *pfSqlType,
                                      SQLINTEGER       *pcbColDef,
                                      SQLSMALLINT      *pibScale,
                                      SQLSMALLINT      *pfNullable);

SQL_EXTERN SQLRETURN  SQLDescribeColW (SQLHSTMT          hstmt,
                                       SQLSMALLINT       icol,
                                       SQLWCHAR          *szColName,
                                       SQLSMALLINT       cbColNameMax,
                                       SQLSMALLINT      *pcbColName,
                                       SQLSMALLINT      *pfSqlType,
                                       SQLINTEGER       *pcbColDef,
                                       SQLSMALLINT      *pibScale,
                                       SQLSMALLINT      *pfNullable);

SQL_EXTERN SQLRETURN  SQLDescribeParam (SQLHSTMT          hstmt,
                                        SQLSMALLINT       ipar,
                                        SQLSMALLINT      *pfSqlType,
                                        SQLINTEGER       *pcbColDef,
                                        SQLSMALLINT      *pibScale,
                                        SQLSMALLINT      *pfNullable);

SQL_EXTERN SQLRETURN  SQLDisconnect (SQLHDBC              hdbc);

SQL_EXTERN SQLRETURN  SQLDriverConnect (SQLHDBC              hdbc,
                                        SQLPOINTER           hwnd,
                                        SQLCHAR      *szConnStrIn,
                                        SQLSMALLINT   cbConnStrin,
                                        SQLCHAR      *szConnStrOut,
                                        SQLSMALLINT  cbConnStrOutMax,
                                        SQLSMALLINT  *pcbConnStrOut,
                                        SQLSMALLINT fDriverCompletion);

SQL_EXTERN SQLRETURN  SQLDriverConnectW (SQLHDBC              hdbc,
                                         SQLPOINTER           hwnd,
                                         SQLWCHAR      *szConnStrIn,
                                         SQLSMALLINT   cbConnStrin,
                                         SQLWCHAR      *szConnStrOut,
                                         SQLSMALLINT  cbConnStrOutMax,
                                         SQLSMALLINT  *pcbConnStrOut,
                                         SQLSMALLINT fDriverCompletion);

SQL_EXTERN SQLRETURN  SQLEndTran (SQLSMALLINT          htype,
                                 SQLHENV              henv,
                                  SQLSMALLINT          ctype);

SQL_EXTERN SQLRETURN  SQLError (SQLHENV              henv,
                                SQLHDBC              hdbc,
                                SQLHSTMT             hstmt,
                                SQLCHAR          *szSqlState,
                                SQLINTEGER       *pfNativeError,
                                SQLCHAR          *szErrorMsg,
                                SQLSMALLINT       cbErrorMsgMax,
                                SQLSMALLINT      *pcbErrorMsg);

SQL_EXTERN SQLRETURN  SQLErrorW (SQLHENV              henv,
                                 SQLHDBC              hdbc,
                                 SQLHSTMT             hstmt,
                                 SQLWCHAR          *szSqlState,
                                 SQLINTEGER       *pfNativeError,
                                 SQLWCHAR          *szErrorMsg,
                                 SQLSMALLINT       cbErrorMsgMax,
                                 SQLSMALLINT      *pcbErrorMsg);


SQL_EXTERN SQLRETURN  SQLExecDirect (SQLHSTMT             hstmt,
                                     SQLCHAR          *szSqlStr,
                                     SQLINTEGER        cbSqlStr);

SQL_EXTERN SQLRETURN  SQLExecDirectW (SQLHSTMT             hstmt,
                                      SQLWCHAR          *szSqlStr,
                                      SQLINTEGER        cbSqlStr);

SQL_EXTERN SQLRETURN  SQLExecute (SQLHSTMT             hstmt);

SQL_EXTERN SQLRETURN  SQLExtendedFetch (SQLHSTMT             hstmt,
                                        SQLSMALLINT          fOrient,
                                        SQLINTEGER           fOffset,
                                        SQLINTEGER          *pcrow,
                                        SQLSMALLINT         *rgfRowStatus);

SQL_EXTERN SQLRETURN  SQLFetch (SQLHSTMT             hstmt);

SQL_EXTERN SQLRETURN  SQLFetchScroll (SQLHSTMT             hstmt,
                                      SQLSMALLINT          fOrient,
                                      SQLINTEGER           fOffset);

SQL_EXTERN SQLRETURN  SQLForeignKeys (SQLHSTMT          hstmt,
                                      SQLCHAR          *szPkTableQualifier,
                                      SQLSMALLINT       cbPkTableQualifier,
                                      SQLCHAR          *szPkTableOwner,
                                      SQLSMALLINT       cbPkTableOwner,
                                      SQLCHAR          *szPkTableName,
                                      SQLSMALLINT       cbPkTableName,
                                      SQLCHAR          *szFkTableQualifier,
                                      SQLSMALLINT       cbFkTableQualifier,
                                      SQLCHAR          *szFkTableOwner,
                                      SQLSMALLINT       cbFkTableOwner,
                                      SQLCHAR          *szFkTableName,
                                      SQLSMALLINT       cbFkTableName);

SQL_EXTERN SQLRETURN  SQLForeignKeysW (SQLHSTMT          hstmt,
                                       SQLWCHAR          *szPkTableQualifier,
                                       SQLSMALLINT       cbPkTableQualifier,
                                       SQLWCHAR          *szPkTableOwner,
                                       SQLSMALLINT       cbPkTableOwner,
                                       SQLWCHAR          *szPkTableName,
                                       SQLSMALLINT       cbPkTableName,
                                       SQLWCHAR          *szFkTableQualifier,
                                       SQLSMALLINT       cbFkTableQualifier,
                                       SQLWCHAR          *szFkTableOwner,
                                       SQLSMALLINT       cbFkTableOwner,
                                       SQLWCHAR          *szFkTableName,
                                       SQLSMALLINT       cbFkTableName);

SQL_EXTERN SQLRETURN  SQLFreeConnect (SQLHDBC              hdbc);

SQL_EXTERN SQLRETURN  SQLFreeEnv (SQLHENV              henv);

SQL_EXTERN SQLRETURN  SQLFreeStmt (SQLHSTMT             hstmt,
                                   SQLSMALLINT          fOption);

SQL_EXTERN SQLRETURN  SQLFreeHandle (SQLSMALLINT          htype,
                                     SQLINTEGER           hndl);

SQL_EXTERN SQLRETURN  SQLGetCol (SQLHSTMT             hstmt,
                                 SQLSMALLINT          icol,
                                 SQLSMALLINT          itype,
                                 SQLPOINTER           tval,
                                 SQLINTEGER           blen,
                                 SQLINTEGER          *olen);

SQL_EXTERN SQLRETURN  SQLGetColW (SQLHSTMT             hstmt,
                                  SQLSMALLINT          icol,
                                  SQLSMALLINT          itype,
                                  SQLPOINTER           tval,
                                  SQLINTEGER           blen,
                                  SQLINTEGER          *olen);

SQL_EXTERN SQLRETURN  SQLGetConnectAttr (SQLHDBC              hdbc,
                                         SQLINTEGER           attr,
                                         SQLPOINTER           oval,
                                         SQLINTEGER           ilen,
                                         SQLINTEGER          *olen);

SQL_EXTERN SQLRETURN  SQLGetConnectAttrW (SQLHDBC              hdbc,
                                          SQLINTEGER           attr,
                                          SQLPOINTER           oval,
                                          SQLINTEGER           ilen,
                                          SQLINTEGER          *olen);

SQL_EXTERN SQLRETURN  SQLGetConnectOption (SQLHDBC              hdbc,
                                           SQLSMALLINT          iopt,
                                           SQLPOINTER           oval);

SQL_EXTERN SQLRETURN  SQLGetConnectOptionW (SQLHDBC              hdbc,
                                            SQLSMALLINT          iopt,
                                            SQLPOINTER           oval);

SQL_EXTERN SQLRETURN  SQLGetCursorName (SQLHSTMT             hstmt,
                                        SQLCHAR          *szCursor,
                                        SQLSMALLINT       cbCursorMax,
                                        SQLSMALLINT      *pcbCursor);

SQL_EXTERN SQLRETURN  SQLGetCursorNameW (SQLHSTMT             hstmt,
                                         SQLWCHAR          *szCursor,
                                         SQLSMALLINT       cbCursorMax,
                                         SQLSMALLINT      *pcbCursor);

SQL_EXTERN SQLRETURN  SQLGetData (SQLHSTMT          hstmt,
                                  SQLSMALLINT       icol,
                                  SQLSMALLINT       fCType,
                                  SQLPOINTER        rgbValue,
                                  SQLINTEGER        cbValueMax,
                                  SQLINTEGER       *pcbValue);

SQL_EXTERN SQLRETURN SQLGetDescField (SQLHDESC             hdesc,
                                      SQLSMALLINT          rcdNum,
                                      SQLSMALLINT          fieldID,
                                      SQLPOINTER           fValue,
                                      SQLINTEGER           fLength,
                                      SQLINTEGER          *stLength);

SQL_EXTERN SQLRETURN SQLGetDescFieldW (SQLHDESC             hdesc,
                                       SQLSMALLINT          rcdNum,
                                       SQLSMALLINT          fieldID,
                                       SQLPOINTER           fValue,
                                       SQLINTEGER           fLength,
                                       SQLINTEGER          *stLength);

SQL_EXTERN SQLRETURN SQLGetDescRec (SQLHDESC             hdesc,
                                    SQLSMALLINT          rcdNum,
                                    SQLCHAR             *fname,
                                    SQLSMALLINT          bufLen,
                                    SQLSMALLINT         *sLength,
                                    SQLSMALLINT         *sType,
                                    SQLSMALLINT         *sbType,
                                    SQLINTEGER          *fLength,
                                    SQLSMALLINT         *fprec,
                                    SQLSMALLINT         *fscale,
                                    SQLSMALLINT         *fnull);

SQL_EXTERN SQLRETURN SQLGetDescRecW (SQLHDESC             hdesc,
                                     SQLSMALLINT          rcdNum,
                                     SQLWCHAR             *fname,
                                     SQLSMALLINT          bufLen,
                                     SQLSMALLINT         *sLength,
                                     SQLSMALLINT         *sType,
                                     SQLSMALLINT         *sbType,
                                     SQLINTEGER          *fLength,
                                     SQLSMALLINT         *fprec,
                                     SQLSMALLINT         *fscale,
                                     SQLSMALLINT         *fnull);

SQL_EXTERN SQLRETURN SQLGetDiagField (SQLSMALLINT         hType,
                                      SQLINTEGER          hndl,
                                      SQLSMALLINT         rcdNum,
                                      SQLSMALLINT         diagID,
                                      SQLPOINTER          dValue,
                                      SQLSMALLINT         bLength,
                                      SQLSMALLINT        *sLength);

SQL_EXTERN SQLRETURN SQLGetDiagFieldW (SQLSMALLINT         hType,
                                       SQLINTEGER          hndl,
                                       SQLSMALLINT         rcdNum,
                                       SQLSMALLINT         diagID,
                                       SQLPOINTER          dValue,
                                       SQLSMALLINT         bLength,
                                       SQLSMALLINT        *sLength);

SQL_EXTERN SQLRETURN SQLGetDiagRec (SQLSMALLINT         hType,
                                    SQLINTEGER          hndl,
                                    SQLSMALLINT         rcdNum,
                                    SQLCHAR            *SQLstate,
                                    SQLINTEGER         *SQLcode,
                                    SQLCHAR            *msgText,
                                    SQLSMALLINT         bLength,
                                    SQLSMALLINT        *SLength);

SQL_EXTERN SQLRETURN SQLGetDiagRecW (SQLSMALLINT         hType,
                                     SQLINTEGER          hndl,
                                     SQLSMALLINT         rcdNum,
                                     SQLWCHAR            *SQLstate,
                                     SQLINTEGER         *SQLcode,
                                     SQLWCHAR            *msgText,
                                     SQLSMALLINT         bLength,
                                     SQLSMALLINT        *SLength);

SQL_EXTERN SQLRETURN SQLGetEnvAttr  (SQLHENV    hEnv,
                                     SQLINTEGER fAttribute,
                                     SQLPOINTER pParam,
                                     SQLINTEGER cbParamMax,
                                     SQLINTEGER * pcbParam);

SQL_EXTERN SQLRETURN  SQLGetFunctions (SQLHDBC        hdbc,
                                       SQLSMALLINT    fFunction,
                                       SQLSMALLINT    *pfExists);

SQL_EXTERN SQLRETURN  SQLGetInfo (SQLHDBC        hdbc,
                                  SQLSMALLINT    fInfoType,
                                  SQLPOINTER     rgbInfoValue,
                                  SQLSMALLINT    cbInfoValueMax,
                                  SQLSMALLINT    *pcbInfoValue);

SQL_EXTERN SQLRETURN  SQLGetInfoW (SQLHDBC        hdbc,
                                   SQLSMALLINT    fInfoType,
                                   SQLPOINTER     rgbInfoValue,
                                   SQLSMALLINT    cbInfoValueMax,
                                   SQLSMALLINT    *pcbInfoValue);

SQL_EXTERN SQLRETURN  SQLGetLength (SQLHSTMT       hstmt,
                                    SQLSMALLINT    locType,
                                    SQLINTEGER     locator,
                                    SQLINTEGER    *sLength,
                                    SQLINTEGER    *ind);

SQL_EXTERN SQLRETURN  SQLGetPosition (SQLHSTMT       hstmt,
                                      SQLSMALLINT    locType,
                                      SQLINTEGER     srceLocator,
                                      SQLINTEGER     srchLocator,
                                      SQLCHAR       *srchLiteral,
                                      SQLINTEGER     srchLiteralLen,
                                      SQLINTEGER     fPosition,
                                      SQLINTEGER    *located,
                                      SQLINTEGER    *ind);

SQL_EXTERN SQLRETURN  SQLGetPositionW (SQLHSTMT       hstmt,
                                       SQLSMALLINT    locType,
                                       SQLINTEGER     srceLocator,
                                       SQLINTEGER     srchLocator,
                                       SQLWCHAR       *srchLiteral,
                                       SQLINTEGER     srchLiteralLen,
                                       SQLINTEGER     fPosition,
                                       SQLINTEGER    *located,
                                       SQLINTEGER    *ind);

SQL_EXTERN SQLRETURN  SQLGetStmtAttr (SQLHSTMT       hstmt,
                                      SQLINTEGER     fAttr,
                                      SQLPOINTER     pvParam,
                                      SQLINTEGER     bLength,
                                      SQLINTEGER    *SLength);

SQL_EXTERN SQLRETURN  SQLGetStmtAttrW (SQLHSTMT       hstmt,
                                       SQLINTEGER     fAttr,
                                       SQLPOINTER     pvParam,
                                       SQLINTEGER     bLength,
                                       SQLINTEGER    *SLength);

SQL_EXTERN SQLRETURN  SQLGetStmtOption (SQLHSTMT       hstmt,
                                        SQLSMALLINT    fOption,
                                        SQLPOINTER     pvParam);

SQL_EXTERN SQLRETURN  SQLGetStmtOptionW (SQLHSTMT       hstmt,
                                         SQLSMALLINT    fOption,
                                         SQLPOINTER     pvParam);

SQL_EXTERN SQLRETURN  SQLGetSubString (SQLHSTMT       hstmt,
                                       SQLSMALLINT    locType,
                                       SQLINTEGER     srceLocator,
                                       SQLINTEGER     fPosition,
                                       SQLINTEGER     length,
                                       SQLSMALLINT    tType,
                                       SQLPOINTER     rgbValue,
                                       SQLINTEGER     cbValueMax,
                                       SQLINTEGER    *StringLength,
                                       SQLINTEGER    *ind);

SQL_EXTERN SQLRETURN  SQLGetSubStringW (SQLHSTMT       hstmt,
                                        SQLSMALLINT    locType,
                                        SQLINTEGER     srceLocator,
                                        SQLINTEGER     fPosition,
                                        SQLINTEGER     length,
                                        SQLSMALLINT    tType,
                                        SQLPOINTER     rgbValue,
                                        SQLINTEGER     cbValueMax,
                                        SQLINTEGER    *StringLength,
                                        SQLINTEGER    *ind);

SQL_EXTERN SQLRETURN  SQLGetTypeInfo (SQLHSTMT       hstmt,
                                      SQLSMALLINT    fSqlType);

SQL_EXTERN SQLRETURN  SQLGetTypeInfoW (SQLHSTMT       hstmt,
                                       SQLSMALLINT    fSqlType);

SQL_EXTERN SQLRETURN  SQLLanguages (SQLHSTMT          hstmt);

SQL_EXTERN SQLRETURN  SQLMoreResults (SQLHSTMT          hstmt);

SQL_EXTERN SQLRETURN  SQLNativeSql (SQLHDBC           hdbc,
                                    SQLCHAR          *szSqlStrIn,
                                    SQLINTEGER        cbSqlStrIn,
                                    SQLCHAR          *szSqlStr,
                                    SQLINTEGER        cbSqlStrMax,
                                    SQLINTEGER       *pcbSqlStr);

SQL_EXTERN SQLRETURN  SQLNativeSqlW (SQLHDBC           hdbc,
                                     SQLWCHAR          *szSqlStrIn,
                                     SQLINTEGER        cbSqlStrIn,
                                     SQLWCHAR          *szSqlStr,
                                     SQLINTEGER        cbSqlStrMax,
                                     SQLINTEGER       *pcbSqlStr);

SQL_EXTERN SQLRETURN  SQLNextResult (SQLHSTMT          hstmt,
                                     SQLHSTMT              hstmt2);

SQL_EXTERN SQLRETURN  SQLNumParams (SQLHSTMT            hstmt,
                                    SQLSMALLINT      *pcpar);

SQL_EXTERN SQLRETURN  SQLNumResultCols (SQLHSTMT        hstmt,
                                        SQLSMALLINT      *pccol);

SQL_EXTERN SQLRETURN  SQLParamData (SQLHSTMT          hstmt,
                                    SQLPOINTER          *Value);

SQL_EXTERN SQLRETURN  SQLParamOptions (SQLHSTMT         hstmt,
                                       SQLINTEGER          crow,
                                       SQLINTEGER          *pirow);

SQL_EXTERN SQLRETURN  SQLPrepare (SQLHSTMT          hstmt,
                                  SQLCHAR          *szSqlStr,
                                  SQLINTEGER       cbSqlStr);

SQL_EXTERN SQLRETURN  SQLPrepareW (SQLHSTMT          hstmt,
                                   SQLWCHAR          *szSqlStr,
                                   SQLINTEGER        cbSqlStr);

SQL_EXTERN SQLRETURN  SQLPrimaryKeys (SQLHSTMT          hstmt,
                                      SQLCHAR          *szTableQualifier,
                                      SQLSMALLINT       cbTableQualifier,
                                      SQLCHAR          *szTableOwner,
                                      SQLSMALLINT       cbTableOwner,
                                      SQLCHAR          *szTableName,
                                      SQLSMALLINT       cbTableName);

SQL_EXTERN SQLRETURN  SQLPrimaryKeysW (SQLHSTMT          hstmt,
                                       SQLWCHAR          *szTableQualifier,
                                       SQLSMALLINT       cbTableQualifier,
                                       SQLWCHAR          *szTableOwner,
                                       SQLSMALLINT       cbTableOwner,
                                       SQLWCHAR          *szTableName,
                                       SQLSMALLINT       cbTableName);

SQL_EXTERN SQLRETURN  SQLProcedureColumns (SQLHSTMT          hstmt,
                                           SQLCHAR          *szProcQualifier,
                                           SQLSMALLINT       cbProcQualifier,
                                           SQLCHAR          *szProcOwner,
                                           SQLSMALLINT       cbProcOwner,
                                           SQLCHAR          *szProcName,
                                           SQLSMALLINT       cbProcName,
                                           SQLCHAR          *szColumnName,
                                           SQLSMALLINT       cbColumnName);

SQL_EXTERN SQLRETURN  SQLProcedureColumnsW (SQLHSTMT          hstmt,
                                            SQLWCHAR          *szProcQualifier,
                                            SQLSMALLINT       cbProcQualifier,
                                            SQLWCHAR          *szProcOwner,
                                            SQLSMALLINT       cbProcOwner,
                                            SQLWCHAR          *szProcName,
                                            SQLSMALLINT       cbProcName,
                                            SQLWCHAR          *szColumnName,
                                            SQLSMALLINT       cbColumnName);

SQL_EXTERN SQLRETURN  SQLProcedures (SQLHSTMT          hstmt,
                                     SQLCHAR          *szProcQualifier,
                                     SQLSMALLINT       cbProcQualifier,
                                     SQLCHAR          *szProcOwner,
                                     SQLSMALLINT       cbProcOwner,
                                     SQLCHAR          *szProcName,
                                     SQLSMALLINT       cbProcName);

SQL_EXTERN SQLRETURN  SQLProceduresW (SQLHSTMT          hstmt,
                                      SQLWCHAR          *szProcQualifier,
                                      SQLSMALLINT       cbProcQualifier,
                                      SQLWCHAR          *szProcOwner,
                                      SQLSMALLINT       cbProcOwner,
                                      SQLWCHAR          *szProcName,
                                      SQLSMALLINT       cbProcName);

SQL_EXTERN SQLRETURN  SQLPutData (SQLHSTMT             hstmt,
                                  SQLPOINTER           Data,
                                  SQLINTEGER           SLen);

SQL_EXTERN SQLRETURN  SQLReleaseEnv (SQLHENV           henv);

SQL_EXTERN SQLRETURN  SQLRowCount (SQLHSTMT           hstmt,
                                   SQLINTEGER         *pcrow);

SQL_EXTERN SQLRETURN  SQLSetConnectAttr (SQLHDBC        hdbc,
                                         SQLINTEGER     attrib,
                                         SQLPOINTER     vParam,
                                         SQLINTEGER     inlen);

SQL_EXTERN SQLRETURN  SQLSetConnectAttrW (SQLHDBC        hdbc,
                                          SQLINTEGER     attrib,
                                          SQLPOINTER     vParam,
                                          SQLINTEGER     inlen);

SQL_EXTERN SQLRETURN  SQLSetConnectOption (SQLHDBC        hdbc,
                                           SQLSMALLINT    fOption,
                                           SQLPOINTER     vParam);

SQL_EXTERN SQLRETURN  SQLSetConnectOptionW (SQLHDBC        hdbc,
                                            SQLSMALLINT    fOption,
                                            SQLPOINTER     vParam);

SQL_EXTERN SQLRETURN  SQLSetCursorName (SQLHSTMT             hstmt,
                                        SQLCHAR          *szCursor,
                                        SQLSMALLINT       cbCursor);

SQL_EXTERN SQLRETURN  SQLSetCursorNameW (SQLHSTMT             hstmt,
                                         SQLWCHAR          *szCursor,
                                         SQLSMALLINT       cbCursor);

SQL_EXTERN SQLRETURN  SQLSetDescField (SQLHDESC          hdesc,
                                       SQLSMALLINT       rcdNum,
                                       SQLSMALLINT       fID,
                                       SQLPOINTER        Value,
                                       SQLINTEGER        buffLen);

SQL_EXTERN SQLRETURN  SQLSetDescFieldW (SQLHDESC          hdesc,
                                        SQLSMALLINT       rcdNum,
                                        SQLSMALLINT       fID,
                                        SQLPOINTER        Value,
                                        SQLINTEGER        buffLen);

SQL_EXTERN SQLRETURN  SQLSetDescRec (SQLHDESC          hdesc,
                                     SQLSMALLINT       rcdNum,
                                     SQLSMALLINT       Type,
                                     SQLSMALLINT       subType,
                                     SQLINTEGER        fLength,
                                     SQLSMALLINT       fPrec,
                                     SQLSMALLINT       fScale,
                                     SQLPOINTER        Value,
                                     SQLINTEGER       *sLength,
                                     SQLINTEGER       *indic);

SQL_EXTERN SQLRETURN  SQLSetEnvAttr (SQLHENV hEnv,
                                     SQLINTEGER fAttribute,
                                     SQLPOINTER pParam,
                                     SQLINTEGER cbParam);

SQL_EXTERN SQLRETURN  SQLSetParam (SQLHSTMT             hstmt,
                                   SQLSMALLINT       ipar,
                                   SQLSMALLINT       fCType,
                                   SQLSMALLINT       fSqlType,
                                   SQLINTEGER        cbColDef,
                                   SQLSMALLINT       ibScale,
                                   SQLPOINTER        rgbValue,
                                   SQLINTEGER       *pcbValue);

SQL_EXTERN SQLRETURN  SQLSetStmtAttr (SQLHSTMT       hstmt,
                                      SQLINTEGER     fAttr,
                                      SQLPOINTER     pParam,
                                      SQLINTEGER     vParam);

SQL_EXTERN SQLRETURN  SQLSetStmtAttrW (SQLHSTMT       hstmt,
                                       SQLINTEGER     fAttr,
                                       SQLPOINTER     pParam,
                                       SQLINTEGER     vParam);

SQL_EXTERN SQLRETURN  SQLSetStmtOption (SQLHSTMT          hstmt,
                                        SQLSMALLINT    fOption,
                                        SQLPOINTER     vParam);

SQL_EXTERN SQLRETURN  SQLSetStmtOptionW (SQLHSTMT          hstmt,
                                         SQLSMALLINT    fOption,
                                         SQLPOINTER     vParam);

SQL_EXTERN SQLRETURN SQLSpecialColumns (SQLHSTMT       hstmt,
                                        SQLSMALLINT    fColType,
                                        SQLCHAR        *szTableQual,
                                        SQLSMALLINT    cbTableQual,
                                        SQLCHAR        *szTableOwner,
                                        SQLSMALLINT    cbTableOwner,
                                        SQLCHAR        *szTableName,
                                        SQLSMALLINT    cbTableName,
                                        SQLSMALLINT    fScope,
                                        SQLSMALLINT    fNullable);

SQL_EXTERN SQLRETURN SQLSpecialColumnsW (SQLHSTMT       hstmt,
                                         SQLSMALLINT    fColType,
                                         SQLWCHAR        *szTableQual,
                                         SQLSMALLINT    cbTableQual,
                                         SQLWCHAR        *szTableOwner,
                                         SQLSMALLINT    cbTableOwner,
                                         SQLWCHAR        *szTableName,
                                         SQLSMALLINT    cbTableName,
                                         SQLSMALLINT    fScope,
                                         SQLSMALLINT    fNullable);

SQL_EXTERN SQLRETURN  SQLStartTran (SQLSMALLINT          htype,
                                    SQLHENV              henv,
                                    SQLINTEGER           mode,
                                    SQLINTEGER            clevel);

SQL_EXTERN SQLRETURN  SQLStatistics (SQLHSTMT       hstmt,
                                     SQLCHAR        *szTableQualifier,
                                     SQLSMALLINT    cbTableQualifier,
                                     SQLCHAR        *szTableOwner,
                                     SQLSMALLINT    cbTableOwner,
                                     SQLCHAR        *szTableName,
                                     SQLSMALLINT    cbTableName,
                                     SQLSMALLINT    fUnique,
                                     SQLSMALLINT    fres);

SQL_EXTERN SQLRETURN  SQLStatisticsW (SQLHSTMT       hstmt,
                                      SQLWCHAR        *szTableQualifier,
                                      SQLSMALLINT    cbTableQualifier,
                                      SQLWCHAR        *szTableOwner,
                                      SQLSMALLINT    cbTableOwner,
                                      SQLWCHAR        *szTableName,
                                      SQLSMALLINT    cbTableName,
                                      SQLSMALLINT    fUnique,
                                      SQLSMALLINT    fres);

SQL_EXTERN SQLRETURN SQLTablePrivileges (SQLHSTMT       hstmt,
                                         SQLCHAR        *szTableQualifier,
                                         SQLSMALLINT    cbTableQualifier,
                                         SQLCHAR        *szTableOwner,
                                         SQLSMALLINT    cbTableOwner,
                                         SQLCHAR        *szTableName,
                                         SQLSMALLINT    cbTableName);

SQL_EXTERN SQLRETURN SQLTablePrivilegesW (SQLHSTMT       hstmt,
                                          SQLWCHAR        *szTableQualifier,
                                          SQLSMALLINT    cbTableQualifier,
                                          SQLWCHAR        *szTableOwner,
                                          SQLSMALLINT    cbTableOwner,
                                          SQLWCHAR        *szTableName,
                                          SQLSMALLINT    cbTableName);

SQL_EXTERN SQLRETURN SQLTables (SQLHSTMT       hstmt,
                                SQLCHAR        *szTableQualifier,
                                SQLSMALLINT    cbTableQualifier,
                                SQLCHAR        *szTableOwner,
                                SQLSMALLINT    cbTableOwner,
                                SQLCHAR        *szTableName,
                                SQLSMALLINT    cbTableName,
                                SQLCHAR        *szTableType,
                                SQLSMALLINT    cbTableType);

SQL_EXTERN SQLRETURN SQLTablesW (SQLHSTMT       hstmt,
                                 SQLWCHAR        *szTableQualifier,
                                 SQLSMALLINT    cbTableQualifier,
                                 SQLWCHAR        *szTableOwner,
                                 SQLSMALLINT    cbTableOwner,
                                 SQLWCHAR        *szTableName,
                                 SQLSMALLINT    cbTableName,
                                 SQLWCHAR        *szTableType,
                                 SQLSMALLINT    cbTableType);

SQL_EXTERN SQLRETURN  SQLTransact (SQLHENV              henv,
                                   SQLHDBC              hdbc,
                                   SQLSMALLINT       fType);

#define FAR
#define SQL_SQLSTATE_SIZE         5   /* size of SQLSTATE, not including
                                          null terminating byte               */
#define SQL_MAX_DSN_LENGTH       18   /* maximum data source name size       */
#define SQL_MAX_ID_LENGTH        18   /* maximum identifier name size,
                                        e.g. cursor names                     */
#define SQL_MAXLSTR            255     /* Maximum length of an LSTRING        */
#define SQL_LVCHAROH           26      /* Overhead for LONG VARCHAR in        */
                                       /* record                              */
#define SQL_LOBCHAROH          312     /* Overhead for LOB in record          */

/* SQL extended data types (negative means unsupported) */
#define SQL_TINYINT            -6
#define SQL_BIT                -7
#define SQL_UNSIGNED_OFFSET    -22    /*                              @F3A*/
#define SQL_SIGNED_OFFSET      -20    /*                              @F3A*/

/* C data type to SQL data type mapping */
#define SQL_C_CHAR       SQL_CHAR      /* CHAR, VARCHAR, DECIMAL, NUMERIC */
#define SQL_C_LONG       SQL_INTEGER   /* INTEGER                         */
#define SQL_C_SLONG      SQL_INTEGER   /* INTEGER                         */
#define SQL_C_SHORT      SQL_SMALLINT  /* SMALLINT                        */
#define SQL_C_FLOAT      SQL_REAL      /* REAL                            */
#define SQL_C_DOUBLE     SQL_DOUBLE    /* FLOAT, DOUBLE                   */
#define SQL_C_DATE       SQL_DATE      /* DATE                            */
#define SQL_C_TIME       SQL_TIME      /* TIME                            */
#define SQL_C_TIMESTAMP  SQL_TIMESTAMP /* TIMESTAMP                       */
#define SQL_C_BINARY     SQL_BINARY    /* BINARY, VARBINARY               */
#define SQL_C_BIT        SQL_BIT
#define SQL_C_TINYINT    SQL_TINYINT
#define SQL_C_BIGINT     SQL_BIGINT
#define SQL_C_DBCHAR     SQL_DBCLOB
#define SQL_C_WCHAR      SQL_WCHAR     /* UNICODE                         */
#define SQL_C_DATETIME   SQL_DATETIME  /* DATETIME                        */
#define SQL_C_BLOB           SQL_BLOB
#define SQL_C_CLOB           SQL_CLOB
#define SQL_C_DBCLOB         SQL_DBCLOB
#define SQL_C_BLOB_LOCATOR   SQL_BLOB_LOCATOR
#define SQL_C_CLOB_LOCATOR   SQL_CLOB_LOCATOR
#define SQL_C_DBCLOB_LOCATOR SQL_DBCLOB_LOCATOR
#define SQL_ATTR_XML_DECLARATION            2552    /* @F1A*/
#define SQL_ATTR_CURRENT_IMPLICIT_XMLPARSE_OPTION    2553 /* @F1A*/
#define SQL_C_DECIMAL128 -361         /*  128 bit decimal floating point @E2A*/
#define SQL_C_DECIMAL64  SQL_DECFLOAT /*  64  bit decimal floating point @E2A*/
#define SQL_C_DECIMAL32  -362         /*  32  bit decimal floating point @E2A*/
#define SQL_C_UTINYINT (SQL_TINYINT + SQL_UNSIGNED_OFFSET)
                                       /* Unsigned TINYINT type (-28)     @F3A*/
#define SQL_C_STINYINT (SQL_TINYINT + SQL_SIGNED_OFFSET)
                                       /* Signed TINYINT type (-26)       @F3A*/
#define SQL_C_DATE_STRUCT -501
#define SQL_C_TIME_STRUCT -502

#define SQL_C_TIMESTAMP_EXT  -363
#define SQL_C_TYPE_TIMESTAMP_EXT SQL_C_TIMESTAMP_EXT
#define SQL_C_TIMESTAMP_STRUCT SQL_C_TIMESTAMP_EXT

/* miscellaneous constants and unsupported functions */
#define SQL_ADD                        -1
#define SQL_DELETE                     -1
#define SQL_KEYSET_SIZE                -1
#define SQL_LCK_NO_CHANGE              -1
#define SQL_LOCK_NO_CHANGE             -1
#define SQL_LOCK_EXCLUSIVE             -1
#define SQL_LOCK_UNLOCK                -1
#define SQL_METH_D                     -1
#define SQL_POSITION                   -1
#define SQL_QUERY_TIMEOUT              -1
#define SQL_ROW_ADDED                  -1
#define SQL_ROW_NOROW                   1                          /* @D3C*/
#define SQL_ROW_ERROR                  -1
#define SQL_ROW_SUCCESS                 0
#define SQL_ROW_SUCCESS_WITH_INFO      -1
#define SQL_SC_TRY_UNIQUE              -1
#define SQL_SIMULATE_CURSOR            -1
#define SQL_UNKNOWN_TYPE               -1
#define SQL_UPDATE                     -1
#define SQL_UNIC_DATA                  99                          /* @D3A*/

/* Constants used for block array insert support */
#define SQL_PARAM_SUCCESS               0                          /* @E2A*/
#define SQL_PARAM_DIAG_UNAVAILABLE      1                          /* @E2A*/
#define SQL_PARAM_ERROR                 5                          /* @E2A*/
#define SQL_PARAM_SUCCESS_WITH_INFO     6                          /* @E2A*/
#define SQL_PARAM_UNUSED                7                          /* @E2A*/

#define SQL_WARN_VAL_TRUNC                  "01004"

#if (__OS400_TGTVRM__>=510)  /* @B1A*/
#pragma datamodel(pop)       /* @B1A*/
#endif                       /* @B1A*/

#ifndef __ILEC400__
#pragma info(restore)
#endif

#endif /* SQL_H_SQLCLI */
