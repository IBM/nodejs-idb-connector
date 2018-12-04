# idb-connector changelog

## 1.1.5

- Refined docs and trace for scrollable cursor
- Added a new API stmt.getStmtDiag() as alias of stmt.stmtError()
- Added CHANGLOG.md

## 1.1.4

- Added test cases for the callback in dbconn.conn()
- Updated examples with ES6 syntax
- Required dep "node-pre-gyp" 0.11.0 and above

## 1.1.3

- Fixed the NAPI callback error in DbConn::Conn()
- Ported and refined the API manual from "developerWorks"

## 1.1.2

- Removed "node-gyp" from dependency list
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