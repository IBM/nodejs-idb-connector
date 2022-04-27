# idb-connector changelog
## 1.2.16
- [build] Fix EBADPLATFORM build error on Node 18 / Python 3.9
- [build] Support Python 3.9 changes

## 1.2.15
- [fix] Check for Javascript empty-string in bindParams
- [feat] Set SQL_ATTR_NON_HEXCCSID by default (#136)

## 1.2.14
- [fix] No error return on SQL_NO_DATA_FOUND (sync) (#146)

## 1.2.13
- [build] support Node.js v16 (#141)

## 1.2.12
- [fix] stop setting error for SQL_NO_DATA_FOUND (#138)
- [build] fix compiler warnings (#132)

## 1.2.11
- [fix] UTF-8 data not properly formed error (#129)

## 1.2.10
- [test] add test case for the input parameter truncation issue (#127)
- [dbstmt] detect data length first to fix the messcode issue (#123)
- [doc] update bindParameters examples (#121)

## 1.2.9
- [dbstmt] add a new API fieldInfo (#115)

## 1.2.8
- [dbstmt] fix: select-insert when binding strings (#114)

## 1.2.7
- [dbstmt] add new simplified API bindParameters (#111)
- [dbstmt] fix: Free LOB locators using CLI functions (#110)

For the details of the new API bindParameters, please refer to the manual and the new example.
The old API bindParams will be deprecated accordingly.

## 1.2.6
- [dbstmt] Refine data length assumptions (#104)
- [dbconn] Have dbconn.close disconnect if connected (#106)
- [ci] Use the enhancement label for the feature template

## 1.2.5
- [dbstmt] Fix incorrect float value returned from procedures (#99)
- [dbstmt] Enable returning SQL_DOUBLE type as a JS Number (#104)
- [dberror] Check SQLState and SQLCode when query success with info
- [test] Refine test cases
- [ci] Add issue template (#100)

## 1.2.4
- [dbstmt] detect inconsistent data (#94)
- [dberror] move all debugging functions to dberror.h
- [test] add more test cases

## 1.2.3
- [dbstmt] fix a memory leak issue (#90)
- [dbstmt] use more meaningful variable names in code

## 1.2.2
- [dbstmt] get accurate CLOB data length (#78)
- [dbstmt] finalize buffer of binary data (#86)

## 1.2.1
- [dbstmt] support returning numeric SQL data (#77)
- [dbstmt] support binding to CLOB for long text (#78)
- [dbstmt] fix the truncated issue for data type REAL (#64)
- [doc] add SET PATH details (#74)
- [doc] stmtError() conflicts with dbconn.debug(true) (#81)

## 1.2.0
- Use N-API versions for build (one binary for all Node.js)
- Patch to allow binding null values (#75)
- Add Node.js v12 support
- Remove Node.js v6 support

## 1.1.10

- Retrieve data when SQLFetch return SQL_SUCCESS_WITH_INFO(#71)
- Add the SQLCLI dependencies for developement build(#69)
- Update example code for eslint(#60)

## 1.1.9

- Guard bindings.gyp and build dummy when not on IBM i(#58)
- Clearly state it is not usable on any platform except IBM i(#57)

## 1.1.8

- Skip build unless installing on IBM i(#54)
- Moved the repository to GitHub

## 1.1.7

- Fixed the connection failure(#52) with user/password provided
- Updated some test cases

## 1.1.6

- Added alias shorthands for use during bindParams()
- Added Add checks for value, io type and bindIndicator during bindParams()
- Return null instead of empty [] when output params are not present, during execute()
- Updated examples, API reference and test cases

## 1.1.5

- Refined docs and trace for scrollable cursor
- Added a new API stmt.getStmtDiag() as alias of stmt.stmtError()
- Added CHANGLOG.md
- Made buffer bigger for some unicode characters

## 1.1.4

- Added test cases for the callback in dbconn.conn()
- Updated examples with ES6 syntax
- Required dep 'node-pre-gyp' 0.11.0 and above

## 1.1.3

- Fixed the NAPI callback error in DbConn::Conn()
- Ported and refined the API manual from 'developerWorks'

## 1.1.2

- Removed 'node-gyp' from dependency list
- Exposed data types symbols for params binding

## 1.1.1

- Supported fetching result set from stored procedures

## 1.1.0

- Swithed to N-API
- Added Blob/Binary support and corresponding docs
- Refined test cases and trace

## 1.0.13

- Supported pre-compiled binary downloading
- Fixed a memory leak issue

## 1.0.12

- Fixed the truncate issue for big numbers

## 1.0.11

- Supported Node.js v10.x

## 1.0.10

- Refined the code for fetching results and binding params
- Fixed a string param binding issue

## 1.0.9

- Fixed a statement executing issue
- Fixed a output param buffer size issue

## 1.0.8

- Added a new API stmt.reset()
- Fixed a param binding issue

## 1.0.7

- Added support for BINARY SQL data
- Fixed the 'in/out' param missing issue

## 1.0.6

- Used accurate column width to save memory usage
- Supported dynamic column width

## 1.0.5

- Provided pre-compiled binary for npm
- Renamed to idb-connector
