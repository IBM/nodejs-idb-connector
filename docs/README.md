# DB2 for i Access APIs - idb-connector

- [DB2 for i Access APIs - idb-connector](#db2-for-i-access-apis---idb-connector)
- [Introduction](#introduction)
  - [Install](#install)
- [Examples](#examples)
  - [Async Exec](#async-exec)
  - [Sync Exec](#sync-exec)
  - [Async Prepare Bind Execute](#async-prepare-bind-execute)
  - [Sync Prepare Bind Execute](#sync-prepare-bind-execute)
  - [Async Fetch](#async-fetch)
  - [Sync Fetch](#sync-fetch)
  - [Async FetchAll](#async-fetchall)
  - [Sync FetchAll](#sync-fetchall)
- [Class: dbconn](#class-dbconn)
  - [Contructor: dbconn()](#contructor-dbconn)
  - [dbconn.setConnAttr(attribute, value)](#dbconnsetconnattrattribute-value)
  - [dbconn.getConnAttr(attribute)](#dbconngetconnattrattribute)
  - [dbconn.conn(database)](#dbconnconndatabase)
  - [dbconn.disconn()](#dbconndisconn)
  - [dbconn.close()](#dbconnclose)
  - [dbconn.debug(flag)](#dbconndebugflag)
  - [dbconn.validStmt(sql)](#dbconnvalidstmtsql)
- [Class dbstmt](#class-dbstmt)
  - [Constructor: dbstmt(connection)](#constructor-dbstmtconnection)
  - [dbstmt.setStmtAttr(attribute, value)](#dbstmtsetstmtattrattribute-value)
  - [dbstmt.getStmtAttr()](#dbstmtgetstmtattr)
  - [dbstmt.exec(sql, callback)](#dbstmtexecsql-callback)
  - [dbstmt.execSync(sql [, callback])](#dbstmtexecsyncsql--callback)
  - [dbstmt.prepare(sql, callback)](#dbstmtpreparesql-callback)
  - [dbstmt.prepareSync(sql [, callback])](#dbstmtpreparesyncsql--callback)
  - [dbstmt.bindParam(params, callback)](#dbstmtbindparamparams-callback)
  - [dbstmt.bindParamSync(params [, callback])](#dbstmtbindparamsyncparams--callback)
  - [dbstmt.execute(callback)](#dbstmtexecutecallback)
  - [dbstmt.executeSync([callback])](#dbstmtexecutesynccallback)
  - [dbstmt.nextResult()](#dbstmtnextresult)
  - [dbstmt.commit()](#dbstmtcommit)
  - [dbstmt.rollback()](#dbstmtrollback)
  - [dbstmt.closeCursor()](#dbstmtclosecursor)
  - [dbstmt.close()](#dbstmtclose)
  - [dbstmt.fetch([orient,] [offset,] callback)](#dbstmtfetchorient-offset-callback)
  - [dbstmt.fetchSync()](#dbstmtfetchsync)
  - [dbstmt.fetchAll(callback)](#dbstmtfetchallcallback)
  - [dbstmt.fetchAllSync([callback])](#dbstmtfetchallsynccallback)
  - [dbstmt.numFields()](#dbstmtnumfields)
  - [dbstmt.numRows()](#dbstmtnumrows)
  - [dbstmt.fieldType(index)](#dbstmtfieldtypeindex)
  - [dbstmt.fieldWidth(index)](#dbstmtfieldwidthindex)
  - [dbstmt.fieldNullable(index)](#dbstmtfieldnullableindex)
  - [dbstmt.fieldName(index)](#dbstmtfieldnameindex)
  - [dbstmt.fieldPrecise(index)](#dbstmtfieldpreciseindex)
  - [dbstmt.fieldScale(index)](#dbstmtfieldscaleindex)
  - [dbstmt.stmtError(callback)](#dbstmtstmterrorcallback)
  - [dbstmt.asNumber(flag)](#dbstmtasnumberflag)

___

## Introduction

- `idb-connector` provides connectivity to DB2 from Node.js.

- `idb-connector` uses DB2 CLI API to access DB2 subsequently this module is only supported for `IBM i` systems.

### Install

 Available on npm

- `npm install idb-connector`

Build from source

- `git clone https://github.com/IBM/nodejs-idb-connector.git`
- `cd nodejs-idb-connector`
- `npm install --build-from-source`

___

## Examples

### Async Exec

```javascript

const {dbconn, dbstmt} = require('idb-connector');

const sql = 'SELECT * FROM QIWS.QCUSTCDT';
const connection = new dbconn(); // Create a connection object.

connection.conn('*LOCAL'); // Connect to a database.

const statement = new dbstmt(dbconn); // Create a statement object of the connection.

statement.exec(sql, (result, error) => {
  if (error) {
    throw error;
  }
  console.log(`Result Set: ${JSON.stringify(result)}`);

  statement.close(); // Clean up the statement object.
  connection.disconn(); // Disconnect from the database.
  connection.close(); // Clean up the connection object.
});

```

___

### Sync Exec

```javascript

const {dbconn, dbstmt} = require('idb-connector');

const sql = 'SELECT STATE FROM QIWS.QCUSTCDT';
const connection = new dbconn(); // Create a connection object.

connection.conn('*LOCAL'); // Connect to the database.

const statement = new dbstmt(connection); // Create a statement object.

const result = statement.execSync(sql);
console.log(`Result Set: ${JSON.stringify(result)}`);

statement.close(); // Clean up the statement object.
connection.disconn(); // Disconnect from the database.
connection.close(); // Clean up the connection object.

```

___

### Async Prepare Bind Execute

Calling a Stored Procedure

```javascript

const {
  dbconn, dbstmt, IN, OUT, CHAR, CLOB,
} = require('idb-connector');

const sql = 'call QXMLSERV.iPLUG512K(?,?,?,?)';
const connection = new dbconn();
const ipc = '*NA';
const ctl = '*here';
const xmlIn = `<xmlservice><sh>system 'wrksbs'</sh></xmlservice>`;
const xmlOut = '';
const params = [[ipc, IN, CHAR],
  [ctl, IN, CHAR],
  [xmlIn, IN, CLOB],
  [xmlOut, OUT, CLOB],
];

connection.conn('*LOCAL');
const statement = new dbstmt(connection);

statement.prepare(sql, (error) => {
  if (error) {
    throw error;
  }
  statement.bindParam(params, (error) => {
    if (error) {
      throw error;
    }
    statement.execute((out, error) => {
      if (error) {
        throw error;
      }
      console.log(`Output Params:${out}\n`);
      statement.close();
      connection.disconn();
      connection.close();
    });
  });
});

```

___

### Sync Prepare Bind Execute

```javascript

const {
  dbconn, dbstmt, IN, OUT, CLOB, CHAR,
} = require('idb-connector');

const sql = 'call QXMLSERV.iPLUG512K(?,?,?,?)';
const connection = new dbconn();
const ipc = '*NA';
const ctl = '*here';
const xmlIn = `<xmlservice><sh>system 'wrksbs'</sh></xmlservice>`;
const xmlOut = '';
const params = [[ipc, IN, CHAR],
  [ctl, IN, CHAR],
  [xmlIn, IN, CLOB],
  [xmlOut, OUT, CLOB],
];

connection.conn('*LOCAL');

const statement = new dbstmt(connection);

statement.prepareSync(sql);
statement.bindParamSync(params);

const out = statement.executeSync();

console.log(`Output Params:${out}\n`);

statement.close();
connection.disconn();
connection.close();

```

___

### Async Fetch

Asynchronously retrieve one row from the result set

```javascript

const {dbconn, dbstmt} = require('idb-connector');

const connection = new dbconn();

connection.conn('*LOCAL');

const statement = new dbstmt(connection);

statement.prepare('SELECT * FROM QIWS.QCUSTCDT', (error) => {
  if (error) {
    throw error;
  }
  statement.execute((error) => {
    if (error) {
      throw error;
    }

    statement.fetch((row, rc) => {
      if (rc instanceof Error) {
        throw rc;
      }
      console.log(`Row: ${JSON.stringify(row)}\n`);

      statement.close();
      connection.disconn();
      connection.close();
    });
  });
});

```

___

### Sync Fetch

Synchronously retrieve one row from the result set

```javascript

const {dbconn, dbstmt} = require('idb-connector');

const connection = new dbconn();

connection.conn('*LOCAL');

const statement = new dbstmt(connection);

statement.prepareSync('SELECT * FROM QIWS.QCUSTCDT');
statement.executeSync();

const row = statement.fetchSync();

console.log(`Row:${JSON.stringify(row)}\n`);

statement.close();
connection.disconn();
connection.close();

```

___

### Async FetchAll

Asynchronously retrieve all rows from the result set

```javascript

const {dbstmt, dbconn} = require('idb-connector');

const connection = new dbconn();

connection.conn('*LOCAL');

const statement = new dbstmt(connection);

statement.prepare('SELECT * FROM QIWS.QCUSTCDT', (error) => {
  if (error) {
    throw error;
  }
  statement.execute((error) => {
    if (error) {
      throw error;
    }
    statement.fetchAll((result, error) => {
      if (error) {
        throw error;
      }
      console.log(`Result Set:${JSON.stringify(result)}\n`);
      statement.close();
      connection.disconn();
      connection.close();
    });
  });
});

```

___

### Sync FetchAll

Synchronously retrieve all rows from the result set

```javascript

const {dbconn, dbstmt} = require('idb-connector');

const connection = new dbconn();

connection.conn('*LOCAL');

const statement = new dbstmt(connection);

statement.prepareSync('SELECT * FROM SCHEMA.MYTABLE');
statement.executeSync();

const result = statement.fetchAllSync();

console.log(`Result Set:${JSON.stringify(result)}`);

statement.close();
connection.disconn();
connection.close();

```

## Class: dbconn

The dbconn class is used to create a connection object.

Once the idb-connector is installed you can gain access with:

`const {dbconn} = require('idb-connector');`

use the new operator to instantiate it

`const conn = new dbconn();`

Once instantiated the methods documented below can be performed.

Make sure to call `disconn` and `close` when finished.

### Contructor: dbconn()

**Description:**

Allocates a new connection handle, ensure `conn` function is called to connect to the target database.

___

### dbconn.setConnAttr(attribute, value)

**Description:**

Sets a connection attribute to a provided valid value.

**Syntax:**

setConnAttr(attribute, value)

**Parameters**

- **attribute:** `number(integer)` is the connection attribute to set.
- **value:** `string | number(integer)` the value to set the specified attribute to.

**Returns:**

`boolean(true)` upon success otherwise an error is thrown.

**DB2 CLI API:** SQLSetConnectAttr

**Valid Scope:** Before connecting to a database.

**Comments:**

Auto Commit attribute is automatically enabled.

Refer to this [table](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_71/cli/rzadpfnsconx.htm%23rzadpfnsconx__tbcono) for more details.

___

### dbconn.getConnAttr(attribute)

**Description:**

Returns the current settings for the specified connection attribute.

**Syntax:**

getConnAttr(attribute)

**Parameters:**

- **attribute:** `number(integer)` the connection attribute to set.

**Returns:**

`string | number(integer)` depending on the attribute.

**DB2 CLI API:** SQLGetConnectAttr

**Valid Scope:** After instantiated the connection object.

**Comments:**

Refer to this [table](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_71/cli/rzadpfngcona.htm) for more details.
___

### dbconn.conn(database)

**Description:**

Establishes a connection to the target database.

**NOTE** 

- `*LOCAL` can be provided as the `database` when connecting to a local database, allowing `user` & `password` to be optionally passed.

- To access SQL functions and stored procedures without fully qualifying them, ensure that the CURRENT PATH is set correctly using the SET PATH SQL statement. See [here](https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_74/db2/rbafzsetpath.htm) for more info.

**Syntax 1:**
conn(database)

**Syntax 2:**
conn(database, callback)

**Syntax 3:**
conn(database, user, password)

**Syntax 4:**
conn(database, user, password, callback)

**Parameters:**

- **database:** `string` is the name or alias name of the database.

- **user:** `string` is the authorization name (user identifier).

- **password:** `string` is the authentication string (password).

- **callback:** `function` is a callback function running after the `conn` is complete

**DB2 CLI API:** SQLConnect

**Valid Scope:** Before calling the dbstmt.exec() or dbstmt.prepare() function.
___

### dbconn.disconn()

**Description:**

Ends the connection associated with the database connection handle.

After calling this function, either call `conn` to connect to another database or `close`.

**Syntax:**

disconn()

**DB2 CLI API:** SQLDisconnect

**Valid Scope:** After calling the conn() function.

**Returns:**

`boolean(true)` upon success otherwise an error is thrown.

___

### dbconn.close()

**Description:**

Frees all DB2 for i resources associated with the connection object.

`disconn()` must be called before calling this function.

**Syntax:**

close()

**DB2 CLI API:** SQLFreeConnect

**Valid Scope:** After calling the disconn() function.

**Returns:**

`boolean(true)` upon success otherwise an error is thrown.

___

### dbconn.debug(flag)

**Description:**

Enables or disables verbose output to the console.

**Syntax:**

debug(flag)

**Parameters:**

- **flag:** `boolean` to turn debug mode on or off. Default value is `false`.

**Returns:**

`boolean` the current state of the debug flag otherwise an error is thrown.

**Valid Scope:** Entire life cycle.

___

### dbconn.validStmt(sql)

**Description:**

Checks if the SQL string is valid and interprets vendor escape clauses.

If the original SQL string that is passed by the application contains vendor escape clause sequences,

DB2 for i CLI returns the transformed SQL string that is seen by the data source (with vendor escape clauses either converted or discarded as appropriate).

**Syntax:**

validStmt(sql)

**Parameters:**

- **sql:** `string` that needs to be checked and escaped.

**Returns:**

`string` the transformed sql string upon success, otherwise an error is thrown.

**DB2 CLI API:** SQLNativeSql

**Valid Scope:** After calling `conn` function

___

## Class dbstmt

Once the idb-connector is installed you can gain access with:

`const {dbstmt} = require('idb-connector');`

A connected `dbconn` object is required to create a new `dbstmt` object from.

```javascript
const {dbconn, dbstmt} = require('idb-connector');

const connection = new dbconn();

connection.conn('*LOCAL');

let statement = new dbstmt(dbconn);

```

Once instantiated the methods documented below can be performed.

Make sure to call `close` when finished with the statement object.

### Constructor: dbstmt(connection)

**Parameters:**

- **dbconn:** `dbconn object` the connection object to create the statement from.

- Ensure `connection` has connected to the database first with `conn` function.

___

### dbstmt.setStmtAttr(attribute, value)

**Description:**

Set an attribute of a specific statement handle.

To set an option for all statement handles associated with a connection handle `setConnAttr` can be used.

**Syntax:**

setStmtAttr(attribute, value)

**Parameters:**

- **attribute:** `number(integer)` is the statement attribute to set.

- **value:** `string | number (integer)` the value to set the specified attribute to.

**DB2 CLI API:** SQLSetStmtAttr

**Valid Scope:** After allocating the statement handler.
___

### dbstmt.getStmtAttr()

**Description:**

Returns the current settings for the specified connection option

**Syntax:**

getStmtAttr(int Attribute)

**Parameters**

- **attribute:** is the connection attribute to set. Refer to this [table](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_71/cli/rzadpfngstma.htm) for more details.

**Returns:**

`string | number (integer)` depending on the attribute.

**DB2 CLI API:** SQLGetStmtAttr

**Valid Scope:** After allocating the statement handler.

___

### dbstmt.exec(sql, callback)

**Description:**

Asynchronously runs the specified SQL statement.

**NOTE** use the `execute` function to call stored procedures

**Syntax:**

exec(sql,callback)

**Parameters**

- **sql:** `string` is the sql statement to execute.

- **callback(resultSet, error):** `function` to process after `exec` is complete.
     - **resultSet:** `array` of `objects` each object represents a row of data.
     - If an error occurred or there is no `resultSet` it is set to `null`.
     - **error:** `Error object` when `execSync` is unsuccessful. Otherwise `error` is set to `null`.

**DB2 CLI API:** SQLExecDirect

**Valid Scope:** After calling the `conn` function.

**Example:** [Here](#async-exec)

___

### dbstmt.execSync(sql [, callback])

**Description:**

The synchronous blocking version of `exec`.

**Syntax 1:**

execSync(sql)

**Parameters**

- **sql:** `string` is the sql statement to execute.

**Returns:**

- **resultSet:** `array` of `objects` each object represents a row of data.
- If an error occurred or there is no `resultSet` , `null` will be returned.

**Syntax 2:**

execSync(sql, callback(resultSet, error))

**Parameters**

- **sql:** `string` is the sql statement to execute.

- **callback(resultSet, error):** `function` to process after `execSync` is complete.
     - **resultSet:** `array` of `objects` each object represents a row of data. If an error occurred or there is no `resultSet` it is set to `null`.
  
     - **error:** `Error object` when `execSync` is unsuccessful. Otherwise `error` is set to `null`.

**Comments:**

- It is recommended to invoke the `numFields`, `numRows`, `fieldName` and other result set related functions in this callback function, because they rely on the temporal result set in memory.
- After running `execSync` the result set will be destroyed and cleaned up.

**DB2 CLI API:** SQLExecDirect

**Valid Scope:** After calling the conn() function.

**Example:** [Here](#sync-exec)

___

### dbstmt.prepare(sql, callback)

**Description:**

Asynchronously associates an SQL statement with the input statement handle and sends the statement to the DBMS to be prepared.

**Syntax:**

prepare(sql, callback)

**Parameters**

- **sql:** is the SQL statement string.

- **callback(error)**: `function` to process after `prepare` is complete.
     - **error**: `Error object` when `prepare` is unsuccessful. Otherwise `error` is set to `null`.

**Comments:**

If the statement handler has been used with a SELECT statement,

`closeCursor` must be called to close the cursor, before calling `prepare` again.

**DB2 CLI API:** SQLPrepare

**Valid Scope:**

- After calling the conn() function.

- Before calling the execute() or bindParam() function.

**Example:** [Here](#async-prepare-bind-execute)
___

### dbstmt.prepareSync(sql [, callback])

**Description:**

Synchronous version of `prepare`.

**Syntax 1:**

prepareSync(sql)

**Parameters:**

- **sql:** `string` the sql statement to prepare.

**Returns:**

`void` no return type, if an error occurred it will be thrown.

**Syntax 2:**

prepareSync(sql, callback)

**Parameters:**

- **sql:** `string` the sql statement to prepare.

- **callback(error)**: `function` to process after `prepareSync` is complete.
     - **error**: `Error object` when `prepareSync` is unsuccessful. Otherwise `error` is set to `null`.

**Comments:**

If the statement handler has been used with a SELECT statement
`closeCursor` must be called first before calling `prepareSync` again.

**DB2 CLI API:** SQLPrepare

**Valid Scope:**

- After calling the `conn` function.

- Before calling the `executeSync` or `bindParamSync` function.

**Example:** [Here](#sync-prepare-bind-execute)

___

### dbstmt.bindParam(params, callback)

**Description:**

Asynchronously associate (bind) parameter markers in an SQL statement to application variables.

Data is transferred from the application to the Database Management System (DBMS) when `execute` function is called.

Data conversion might occur when the data is transferred.

This function must also be used to bind to a parameter of a stored procedure where the parameter can be: input, output, or both

**Syntax:**

bindParam(params, callback)

**Parameters:**

- **params**: `array` representing the binding parameter list.  Each parameter element will also be an Array with 3 values `[value, io, indicator]`.
  
    - `value` is the parameter to bind.
    - `io` specifies whether the parameter is for input, output, or both.
      - `io` can be:
         -  SQL_PARAM_INPUT
         -  SQL_PARAM_OUTPUT
         -  SQL_PARAM_INPUT_OUTPUT
         -  Shorthand equivleants are:
            -  IN
            -  OUT
            -  INOUT
    - `indicator` specifies how to process the parameter. For example to process a string as a CLOB set the indicator to CLOB.
       - `indcator` can be:
          - CHAR
          - INT
          - NUMERIC
          - BINARY
          - BLOB
          - CLOB
          - BOOLEAN
          - NULL

      These values are constants which are attached to object returned when you `const idb = require('idb-connector')`.

      You can access the constants like: `idb.IN`.

- **callback(error):** `function` to process after `bindParam` is complete.
     - **error:** `Error object` when `bindParam` is unsuccessful. Otherwise `error` is set to `null`.

**DB2 CLI API:** SQLBindParameter

**Valid Scope:** In the callback function of the `prepare` function.

**Example:** [Here](#async-prepare-bind-execute)

___

### dbstmt.bindParamSync(params [, callback])

**Description:**

Synchronous version of `bindParam`.

**Syntax 1:**

bindParamSync(params)

**Parmeters:**

- **params**: as described in [bindParam](#dbstmtbindparamparams-callback)

**Returns:**

`void` no return type, if an error occurred it will be thrown.

**Syntax 2:**

bindParamSync(params, callback)

**Parameters**

- **params**: as described in [bindParam](#dbstmtbindparamparams-callback)

- **callback(error)**: `function` to process after `bindParamSync` is complete.
     - **error**: `Error object` when `bindParamSync` is unsuccessful. Otherwise `error` is set to `null`.

**Example:** [Here](#sync-prepare-bind-execute)

**DB2 CLI API:** SQLBindParameter

**Valid Scope:**

- After calling the `prepareSync` function.
- Before calling the `executeSync` function.

___

### dbstmt.execute(callback)

**Description:**

Asynchronously Runs a statement that was successfully prepared using `prepare`.

The statement is processed with the current values of any application variables that were bound to parameters markers by `bindParam`.

**Syntax:**

execute(callback(outputParams, error))

**Parameters**

- **callback(outputParams, error):** `function` to process after `execute` is complete.
     - **outputParams**: an `array` of an output parameters. If an error occurred or no output parameters are available `outputParams` is set to `null`.
  
     - **error**: `Error object` when `execute` is unsuccessful. Otherwise `error` is set to `null`.

**DB2 CLI API:** SQLExecute

**Valid Scope:** In the callback function of the `prepare` or `bindParam` function.

**Example:** [Here](#async-prepare-bind-execute)

___

### dbstmt.executeSync([callback])

**Description:**

The synchronized version of `execute`.

**Syntax 1:**

executeSync()

**Returns:**  

- **outputParams**: an `array` of output parameters. If an error occured it is thrown.
- If no output parameters are available `null` is returned.

**Syntax 2:**

executeSync(callback)

**Parameters:**

- **callback(outputParams, error):** `function` to process after `executeSync` is complete.
     - **outputParams**: `array` of an output parameters. If an error occured or no output parameters are available `outputParams` is set to `null`.
  
     - **error**: `Error object` when `executeSync` is unsuccessful. Otherwise `error` is set to `null`.

**Comments:**

If the statement also returns a result set, user can issue the `fetch` function to retrieve the data row by row.

**DB2 CLI API:** SQLExecute

**Valid Scope:** After calling the prepareSync() function.

**Example:** [Here](#sync-prepare-bind-execute)
___

### dbstmt.nextResult()

**Description:**

Determines whether there is more information available on the statement handle that has been associated with a stored procedure that is returning multiple result sets.

**Syntax:**

nextResult()

**Comments:**

After completely processing the first result set, the application can call `nextResult` to determine if another result set is available.

If the current result set has unfetched rows, `nextResult` discards them by closing the cursor.

**DB2 CLI API:** SQLMoreResults

**Valid Scope:** After calling the execute() function.

___

### dbstmt.commit()

**Description:**

Commit all changes to the database that have been made on the connection since connect time or the previous call to `commit`.

**Syntax:**

commit()

**DB2 CLI API:** SQLTransact

**Valid Scope:** After calling the execute() or exec() function.

**Comments:**

Auto commit is enabled by default.

___

### dbstmt.rollback()

**Description:**

Rollback all changes to the database that have been made on the connection since connect time or the previous call to `commit`.

**Syntax:**

rollback()

**DB2 CLI API:** SQLTransact

**Valid Scope:** After calling the `execute` or `exec` function.

___

### dbstmt.closeCursor()

**Description:**

Calling `closeCursor` closes any cursor associated with the dbstmt object and discards any pending results.

If no open cursor is associated with the dbstmt object, the function has no effect.

If the dbstmt object references a stored procedure that has multiple result sets, `closeCursor` closes only the current result set. Any additional result sets remain open and usable.

If you want to reuse the dbstmt object, please call `closeCursor` before calling `exec` or `prepare` for another SQL statement.

**Syntax:**

closeCursor()

**DB2 CLI API:** SQLCloseCursor

**Valid Scope:** After calling the fetch() or fetchAll() function.

___

### dbstmt.close()

**Description:**

DB2 for i resources associated with the statement object are freed.

The open cursor, if any, is closed and all pending results are discarded.

**Syntax:**

close()

**DB2 CLI API:** SQLFreeStmt

**Valid Scope:** After executing an SQL statement and processing the results.

___

### dbstmt.fetch([orient,] [offset,] callback)

**Description:**

Asynchronously advances the cursor to the next row of the result set, and retrieves any bound columns.

Or positions the cursor based on the requested orientation and then retrieves any bound columns.


**Syntax 1:**

fetch(callback)

**Parameters**

- **callback(row, error):** `function` to process after `fetchS` is complete.
     - **row**: `object` representing a row of data. If an error occured or there is nothing to fetch `row` is set to `null`.
  
     - **error**: `Error object` when `fetch` is unsuccessful. Otherwise `error` is set to the return code from `SQLFETCH`. When error = `SQL_NO_DATA_FOUND` the end of the result set has been reached.

**Syntax 2:**

fetch(orient, offset, callback)

**Parameters**

- **orient:** `number(integer)` sets the fetch orientation. The valid values are below:

     - `SQL_FETCH_ABSOLUTE`: Move to the row specified by the Offset argument.

     - `SQL_FETCH_FIRST`: Move to the first row of the result set.

     - `SQL_FETCH_LAST`: Move to the last row of the result set.

     - `SQL_FETCH_NEXT`: Move to the row following the current cursor position.

     - `SQL_FETCH_PRIOR`: Move to the row preceding the current cursor position.

     - `SQL_FETCH_RELATIVE` If Offset is:

          - Positive, advance the cursor that number of rows.

          - Negative, back up the cursor that number of rows.

          - Zero, do not move the cursor.

- **Offset:** `number(integer)` is the row offset for relative positioning.

**Note:** To use orientation `SQL_FETCH_RELATIVE` with `Offset`, the cursor must be **dynamic**.

`stmt.setStmtAttr(idb.SQL_ATTR_CURSOR_TYPE, idb.SQL_CURSOR_DYNAMIC);`

- **callback(row, rc):** `function` to process after `fetch` is complete.
     - **row**: `object` representing a row of data. If an error occured or there is nothing to fetch `row` is set to `null`.
  
     - **rc**: `Error object` when `fetch` is unsuccessful. Otherwise `rc` is set to the return code from `SQLFETCH`. When error = `SQL_NO_DATA` the end of the result set has been reached.

**DB2 CLI API:** SQLFetch or SQLFetchScroll

**Valid Scope:** When the result set is available.

**Example:** [Here](#async-fetch)

___

### dbstmt.fetchSync()

**Description:**

Synchronous version of `fetch`.

**Syntax 1:**

fetchSync()

**Syntax 2:**

fetchSync(function Callback(Row))

**Syntax 3:**

fetchSync(int Orient, int Offset)

**Syntax 4:**

fetchSync(int Orient, int Offset, function Callback(Row))

**Parameters**

- **orient:** as described in `fetch` above

- **offset:** as described in `fetch` above.

- **callback(row, error):** `function` to process after `fetchSync` is complete.
     - **row**: `object` representing a row of data. If an error occured or there is nothing to fetch `row` is set to `null`.
  
     - **error**: `Error object` when `fetch` is unsuccessful. Otherwise `error` is set to the return code from `SQLFETCH`. When error = `SQL_NO_DATA` the end of the result set has been reached.

**DB2 CLI API:** SQLFetch or SQLFetchScroll

**Valid Scope:** When the result set is available.

**Example:** [Here](#sync-fetch)

### dbstmt.fetchAll(callback)

**Description:**

Asynchronously retrieves all the rows from the result set if available.

**Syntax:**

fetchAll(callback)

**Parameters**

- **callback(resultSet, error):** `function` to process after `fetchAll` is complete.
     - **resultSet**: an `array` of `objects` each object represents a row of data.
     - If an error occured or there is no `resultSet` it is set to `null`
  
     - **error**: `Error object` when `fetchAll` is unsuccessful. Otherwise `error` is set to `null`.

**DB2 CLI API:** SQLFetch

**Valid Scope:** When the result set is available.

**Example:** [Here](#async-fetchAll)
___

### dbstmt.fetchAllSync([callback])

**Description:**

Synchronous version of `fetchAll`.

**Syntax 1:**
fetchAllSync()

**Returns**

- **resultSet**: an `array` of `objects` each object represents a row of data.
- If there is no result set `null` is returned.
- If an error occurs it will be thrown.

**Syntax 2:**

fetchAllSync(callback)

**Parameters**

- **callback(resultSet, error):** `function` to process after `fetchAll` is complete.
     - **resultSet**: an `array` of `objects` each object represents a row of data.
     - If an error occured `resultSet` is set to `null`
     - **error**: `Error object` when `fetchAllSync` is unsuccessful. Otherwise `error` is set to `null`.

**Example:** [Here](#sync-fetchAll)

**DB2 CLI API:** SQLFetch

**Valid Scope:** When the result set is available.

___

### dbstmt.numFields()

**Description:**

Retrieves number of fields contained in the result set if available.

**Syntax:**

numFields()

**Returns:**

`number(integer)` indicating number of fields in the result set.

**DB2 CLI API:** SQLNumResultCols

**Valid Scope:** When the result set is available.
___

### dbstmt.numRows()

**Description:**

Returns the number of rows in a table affected by the last executed sql statement if available.

**Syntax:**

numRows()

**Returns:**

`number(integer)` indicating number of rows affected by the operation.

**DB2 CLI API:** SQLRowCount

**Valid Scope:** When the result set is available.
___

### dbstmt.fieldType(index)

**Description:**

If a valid index is provided, `fieldType` returns the data type of the indicated field.

**Syntax:**

fieldType(index)

**Parameters:**

- **index:** `number(integer)` the column number in a result set, ordered sequentially left to right, starting at 0.

**Returns:**

`number(integer)` indicating the data type of the specified column in the result set.

**DB2 CLI API:** SQLColAttribute

**Valid Scope:** When the result set is available.
___

### dbstmt.fieldWidth(index)
**Description:**

If a valid index is provided, `fieldWidth` returns the field width of the indicated field.

**Syntax:**

fieldWidth(index)

**Parameters**

- **index:** `number(integer)` the column number in a result set, ordered sequentially left to right, starting at 0.

**Returns:**

`number(integer)` indicating the width of the specified column in the result set.

**DB2 CLI API:** SQLColAttribute

**Valid Scope:** When the result set is available.
___

### dbstmt.fieldNullable(index)

**Description:**

If a valid index is provided, fieldNullable returns true | false indicating if field can be set to null.

**Syntax:**

fieldNullable(index)

**Parameters**

- **index:** `number(integer)` the column number in a result set, ordered sequentially left to right, starting at 0.

**Returns:**

`boolean` indicating if the column can be set to NULL.

**DB2 CLI API:** SQLColAttribute

**Valid Scope:** When the result set is available.

___

### dbstmt.fieldName(index)

**Description:**

If a valid index is provided, `fieldName` returns the name of the indicated field.

**Syntax:**

fieldName(index)

**Parameters**

- **index:** `number(integer)` the column number in a result set, ordered sequentially left to right, starting at 0.

**Returns:**

`string` indicating the name of the specified column in the result set.

**DB2 CLI API:** SQLColAttribute

**Valid Scope:** When the result set is available.
___

### dbstmt.fieldPrecise(index)

**Description:**

If a valid index is provided, `fieldPrecise` returns the precision of the indicated field.

**Syntax:**

fieldPrecise(index)

**Parameters**

- **index:** `number(integer)` the column number in a result set, ordered sequentially left to right, starting at 0.

**Returns:**

`number(integer)` indicating the precision of the specified column in the result set.

**DB2 CLI API:** SQLColAttribute

**Valid Scope:** When the result set is available.
___

### dbstmt.fieldScale(index)
**Description:**

If a valid index is provided, `fieldScale` returns the scale of the indicated column.

**Syntax:**

fieldScale(index)

**Parameters:**

- **index:** `number(integer)` the column number in a result set, ordered sequentially left to right, starting at 0.

**Returns:**

`number(integer)` indicating the scale of the specified column in the result set.

**DB2 CLI API:** SQLColAttribute

**Valid Scope:** When the result set is available.
___

### dbstmt.stmtError(callback)

**Description:**

Returns the diagnostic information associated with the most recently called function for a particular statement, connection, or environment handler.

**NOTE** `dbstmt.stmtError` may not work properly when using with `dbconn.debug(true)`

**Syntax:**

stmtError(hType, recno, callback)

**Parameters**

- **hType:** `number(integer)` indicates the handler type of diagnostic information. It can be following values:

    - `SQL_HANDLE_ENV`: Retrieve the environment diagnostic information

    - `SQL_HANDLE_DBC`: Retrieve the connection diagnostic information

    - `SQL_HANDLE_STMT`: Retrieve the statement diagnostic information

- **recno:** `number(integer)` indicates which error should be retrieved. The first error record is number 1.

- **callback(errmsg):** is a callback function to process the retrieved error message.
     - **errmsg:** `string` consists of a standardized SQLSTATE, the error code, and a text message.

**DB2 CLI API:** SQLGetDiagRec

**Valid Scope:** After calling conn() function
___

### dbstmt.asNumber(flag)

**Description:**

Enables or disables automatic numeric conversion.

**Syntax 1:**

asNumber()

**Syntax 2:**

asNumber(flag)

**Parameters:**

- **flag:** `boolean` to turn automatic data conversion mode on or off. Default value is `false`.

    - `true`: The result data is returned as its original type. Due to the data type difference between SQL and Javascript, the data may be truncated.

    - `false`: The result data is returned as strings.
    
**Returns:**

`boolean` the current state of the asNumber flag otherwise an error is thrown.

**Valid Scope:** After allocating the statement handler.

***Diagnostics***

| Error Code | Meaning|
|---|---|
| 0|Success|
|-1|Error|
|1|Success with information|
| 8001|Invalid parameter numbers|
| 8002|Invalid parameter type|
| 8003|Invalid parameter range|
| 8012|Connection handler is not allocated|
| 8013|Statement handler is not allocated|
| 8014|Result set is not ready|
