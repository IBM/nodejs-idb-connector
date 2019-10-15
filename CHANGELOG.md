# idb-connector changelog
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
