# Node.js iDB Connector for IBM i

The Node.js iDB Connector is an IBM i Node.js Db2 driver open source project from IBM

[![NPM](https://nodei.co/npm/idb-connector.png?downloads=true&downloadRank=true)](https://nodei.co/npm/idb-connector/)

## Installation

```sh
    npm i idb-connector
```

**NOTE** This package only installs on IBM i systems.

Then you can _require_ in your code, as shown below.

```js
    const db = require('idb-connector');
```

## Quick Example

### Example 1: Fetching data using the exec() API

```js
    const {dbconn, dbstmt} = require('idb-connector');

    const sSql = 'SELECT STATE FROM QIWS.QCUSTCDT';
    const connection = new dbconn();
    connection.conn('*LOCAL');
    const statement = new dbstmt(connection);

    statement.exec(sSql, (x) => {
      console.log(JSON.stringify(x));
      statement.close();
      connection.disconn();
      connection.close();
    });

```

### Example 2: Fetching data using the fetchAll() API

```js

    const {dbconn, dbstmt} = require('idb-connector');

    const sSql = 'SELECT STATE FROM QIWS.QCUSTCDT';
    const connection = new dbconn();
    connection.conn('*LOCAL');
    const statement = new dbstmt(connection);

    statement.prepare(sSql, () => {
      statement.execute(() => {
        statement.fetchAll((x) => {
          console.log(`Result is : ${JSON.stringify(x)}`);
          statement.close();
        });
      });
    });

```

### Example 3: Call stored procedures

```js
    const {
      dbconn, dbstmt, IN, OUT, CHAR, CLOB,
    } = require('idb-connector');

    const sql = 'CALL QXMLSERV.iPLUG512K(?,?,?,?)';
    const connection = new dbconn();
    connection.conn('*LOCAL');
    const statement = new dbstmt(connection);

    const ipc = '*NA';
    const ctl = '*here';
    const xmlIn = `<xmlservice><sh>system 'wrksbs'</sh></xmlservice>`;
    const xmlOut = '';

    statement.prepare(sql, () => {
      statement.bindParameters([ipc, ctl, xmlIn, xmlOut], () => {
        statement.execute((out) => { // 'out' is an array of output params
          for (let i = 0; i < out.length; i += 1) {
            console.log(out[i]);
          }
          statement.close();
          connection.disconn();
          connection.close();
        });
      });
    });

```

## API Reference

[Db2 for i Access APIs](https://github.com/IBM/nodejs-idb-connector/blob/master/docs/README.md)

## Change Log

View [`CHANGELOG.md`](https://github.com/IBM/nodejs-idb-connector/blob/master/CHANGELOG.md) file.

## Build

Note that building isn't necessary for end users and is more for developers looking to compile the native Node.js extensions (C code).

```sh
    git clone git@github.com:IBM/nodejs-idb-connector.git
    cd nodejs-idb-connector
    npm install --build-from-source
```
## Build Dependencies
Note: sqlcli header files, gcc, and python are required to compile the code.

```sh
    yum install sqlcli-devel
    yum group install 'Development tools' 
    yum install python2
```

## License

[`MIT`](https://github.com/IBM/nodejs-idb-connector/blob/master/LICENSE)

## **Contributing**

Please read the [contribution guidelines](https://github.com/IBM/nodejs-idb-connector/blob/master/CONTRIBUTING.md).