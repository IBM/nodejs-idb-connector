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

## Quick Examples

### Example 1: Fetching data using the exec() API

```js
const { dbconn, dbstmt } = require('idb-connector');

const sql = 'SELECT STATE FROM QIWS.QCUSTCDT';
const connection = new dbconn();
connection.conn('*LOCAL');
const statement = new dbstmt(connection);

function cleanup(connection, statement){
    statement.close();
    connection.disconn();
    connection.close();
}

statement.exec(sql, (results, error) => {
    if (error) {
        cleanup(connection, statement);
        throw error;
    }
    console.log(JSON.stringify(results));
    cleanup(connection, statement);
});
```

### Example 2: Fetching data using the fetchAll() API

```js
const { dbconn, dbstmt } = require('idb-connector');

const sql = 'SELECT STATE FROM QIWS.QCUSTCDT';
const connection = new dbconn();
connection.conn('*LOCAL');
const statement = new dbstmt(connection);

function cleanup(connection, statement) {
    statement.close();
    connection.disconn();
    connection.close();
}

statement.prepare(sql, (prepareError) => {
    if (prepareError) {
        cleanup(connection, statement);
        throw prepareError;
    }
    statement.execute((outputParameters, executeError) => {
        if (executeError) {
            cleanup(connection, statement);
            throw executeError;
        }
        statement.fetchAll((results, fetchError) => {
            if (fetchError){
                cleanup(connection, statement);
                throw fetchError;
            }
            console.log(`Result is : ${JSON.stringify(results)}`);
            cleanup(connection, statement);
        });
    });
});
```

### Example 3: Call stored procedures

```js
const { dbconn, dbstmt } = require('idb-connector');

const sql = 'CALL QXMLSERV.iPLUG512K(?,?,?,?)';
const connection = new dbconn();
connection.conn('*LOCAL');
const statement = new dbstmt(connection);

const ipc = '*NA';
const ctl = '*here';
const xmlIn = `<xmlservice><sh>system 'wrksbs'</sh></xmlservice>`;
const xmlOut = '';

function cleanup(connection, statement) {
    statement.close();
    connection.disconn();
    connection.close();
}

statement.prepare(sql, (prepareError) => {
    if (prepareError) {
        cleanup(connection, statement);
        throw prepareError;
    }
    statement.bindParameters([ipc, ctl, xmlIn, xmlOut], (bindError) => {
        if (bindError) {
            cleanup(connection, statement);
            throw bindError;
        }
        statement.execute((outputParameters, executeError) => {
            if (executeError) {
                cleanup(connection, statement);
                throw executeError;
            }
            for (let i = 0; i < outputParameters.length; i += 1) {
                console.log(outputParameters[i]);
            }
            cleanup(connection, statement);
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
## Build Dependencies
Note: sqlcli header files, GCC, and Python are required to compile the code.

```sh
yum install sqlcli-devel
yum group install 'Development tools' 
yum install python2
```

## Release

```sh
git clone git@github.com:IBM/nodejs-idb-connector.git
cd nodejs-idb-connector
npm install
./node_modules/.bin/node-pre-gyp rebuild --production
./node_modules/.bin/node-pre-gyp package
```

Create a new github release and attach the generated tar file in the `build` directory to the github release.

After the release is created the [publish](.github/workflows/publish.yml) action will automatically publish to npm.

## License

[`MIT`](https://github.com/IBM/nodejs-idb-connector/blob/master/LICENSE)

## **Contributing**

Please read the [contribution guidelines](https://github.com/IBM/nodejs-idb-connector/blob/master/CONTRIBUTING.md).
