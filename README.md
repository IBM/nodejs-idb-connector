# Node.js iDB Connector for IBM i

The Node.js iDB Connector is an IBM i Node.js Db2 driver open source project from IBM

[![NPM](https://nodei.co/npm/idb-connector.png?downloads=true&downloadRank=true)](https://nodei.co/npm/idb-connector/)

[![Node-API v3 Badge](https://github.com/nodejs/abi-stable-node/blob/doc/assets/Node-API%20v3%20Badge.svg)](https://nodejs.org/dist/latest/docs/api/n-api.html#n_api_node_api_version_matrix)

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


## Release

To create a new release the developer first needs to run `release.sh`
```sh
    ./release.sh
```

This script will create a new release branch, run release-it, push the new branch upstream.

From there the developer needs to:

1) Open a pull request, have it approved by at least 1 reviewer, and merged into the master branch
2) Create a new release and tag from the [GitHub Web UI](https://github.com/IBM/nodejs-idb-connector/releases/new) with the release and tag name matching the version number in package.json.


After the release is created our Jenkins instance will:

1) Build the package
2) Update the GH release with the binary asset
3) Publish the release to NPM

## License

[`MIT`](https://github.com/IBM/nodejs-idb-connector/blob/master/LICENSE)

## **Contributing**

Please read the [contribution guidelines](https://github.com/IBM/nodejs-idb-connector/blob/master/CONTRIBUTING.md).
