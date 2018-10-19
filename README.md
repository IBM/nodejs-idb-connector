# Node.js iDB Connector
The Node.js iDB Connector is a Node.js DB2 driver open source project from IBM. 
  
# Installation  

	npm i idb-connector
	
Then you can _require_ in your code, as shown below.


```js
    const db = require('idb-connector');
```
  

# Quick Example

## Example 1: Fectching data using the exec() API
```js
    const db = require('idb-connector');
    const sSql = 'SELECT STATE FROM QIWS.QCUSTCDT';
    const dbconn = new db.dbconn();
    dbconn.conn("*LOCAL");
    const stmt = new db.dbstmt(dbconn);

    stmt.exec(sSql, (x) => {
      console.log("%s", JSON.stringify(x));
      stmt.close();
      dbconn.disconn();
      dbconn.close();
    });
```

## Example 2: Fectching data using the fetchAll() API
```js
    const db = require('idb-connector');
    const sSql = 'SELECT STATE FROM QIWS.QCUSTCDT';
    const dbconn = new db.dbconn();
    dbconn.conn("*LOCAL");
    const stmt = new db.dbstmt(dbconn);

    stmt.prepare(sSql, () => {
      stmt.execute(() => {
        stmt.fetchAll((x) => { 
          console.log("%s", JSON.stringify(x));
          stmt.close();
        });
      });
    });
```

## Example 3: Call stored procedures
```js
    const db = require('idb-connector');
    const sql = "CALL QXMLSERV.iPLUG512K(?,?,?,?)";
    const dbconn = new db.dbconn();
    dbconn.conn("*LOCAL");
    const stmt = new db.dbstmt(dbconn);

    const ipc = "*NA";
    const ctl = "*here";
    const xmlIn = "<xmlservice><sh>system 'wrksbs'</sh></xmlservice>";
    const xmlOut = "";
    
    stmt.prepare(sql, () => {
      stmt.bindParam([
        [ipc, db.SQL_PARAM_INPUT, db.BIND_STRING],
        [ctl, db.SQL_PARAM_INPUT, db.BIND_STRING],
        [xmlIn, db.SQL_PARAM_INPUT, db.BIND_CLOB],
        [xmlOut, db.SQL_PARAM_OUTPUT, db.BIND_CLOB]
      ], () => {
        stmt.execute((out) => { // 'out' is an array of output params
          for(let i = 0; i < out.length; i++)
            console.log(out[i]);
          stmt.close();
          dbconn.disconn();
          dbconn.close();
        });
      });
    });
```

# API Reference
[DeveloperWorks](https://www.ibm.com/developerworks/community/wikis/home?lang=en#!/wiki/IBM%20i%20Technology%20Updates/page/DB2%20for%20i%20Access%20APIs%20-%20New)

# Build
Note that building isn't necessary for end users and is more for developers looking to compile the native Node.js extensions (C code).

```sh
    git clone git@bitbucket.org:litmis/nodejs-idb-connector.git
    cd nodejs-idb-connector
    npm install --build-from-source
```	
Note: Gcc and python are required to compile the code.   

# License
MIT.  View [`LICENSE`](https://bitbucket.org/litmis/nodejs-idb-connector/src/master/LICENSE) file.