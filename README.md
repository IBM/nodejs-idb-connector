# Node.js iDB Connector for IBM i

The Node.js iDB Connector is an IBM i Node.js Db2 driver open source project from IBM

[![NPM](https://nodei.co/npm/idb-connector.png?downloads=true&downloadRank=true)](https://nodei.co/npm/idb-connector/)

[![Node-API v3 Badge](https://github.com/nodejs/abi-stable-node/blob/doc/assets/Node-API%20v3%20Badge.svg)](https://nodejs.org/dist/latest/docs/api/n-api.html#n_api_node_api_version_matrix)

## Installation

```sh
    npm i idb-connector
```

**NOTE:** This package only installs on IBM i systems.

Then you can _require_ in your code, as shown below.

```js
    const db = require('idb-connector');
```

---
**NOTE:** starting with version 12, npm will no longer run install scripts by default. When installing, you will see a message like this:

```sh
npm warn install-scripts   idb-connector@1.3.0 (install: node-gyp-build)
```

We use [node-gyp-build](https://github.com/prebuild/node-gyp-build) to build and ship a prebuilt binary with the package. With the prebuilt binary, the install script doesn't actually do anything and you do not need to approve it for the package to work. You may want to approve it to silence the warning, however.

For more information refer to the [npm blog post](https://github.blog/changelog/2026-06-09-upcoming-breaking-changes-for-npm-v12/).


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
    const {dbconn, dbstmt} = require('idb-connector');

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
        statement.execute((out) => {
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

Note that building isn't necessary for end-users and is more for developers looking to compile the native Node.js extensions (C code).

```sh
    git clone git@github.com:IBM/nodejs-idb-connector.git
    cd nodejs-idb-connector
    npm install --build-from-source
```
### Build Dependencies
Note: sqlcli header files, GCC, and Python are required to compile the code.

```sh
    yum install sqlcli-devel
    yum group install 'Development tools' 
    yum install python2
```

## License

[`MIT`](https://github.com/IBM/nodejs-idb-connector/blob/master/LICENSE)

## **Contributing**

Please read the [contribution guidelines](https://github.com/IBM/nodejs-idb-connector/blob/master/CONTRIBUTING.md).
