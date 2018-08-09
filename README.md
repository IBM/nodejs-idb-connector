# Node.js iDB Connector
The Node.js iDB Connector is a Node.js DB2 driver open source project from IBM. 

# Build

	git clone git@bitbucket.org:litmis/nodejs-idb-connector.git
    cd nodejs-idb-connector
    npm install --build-from-source
	
  Note: Gcc and python are required to compile the code.   
  
# Installation  

	npm i idb-connector
	
  Then you can *require('idb-connector')* in your code.   
  
  If you encounter below error messega:  
  
  	Error: Cannot find module 'idb-connector'  
	
  you need to add the idb-connector path:  
  
	export NODE_PATH=/QOpenSys/pkgs/lib/nodejs8/lib/node_modules

# Quick Example

## Example 1: Fectching data using the exec() API
```js
	var db = require('idb-connector');
	var sSql = 'SELECT STATE FROM QIWS.QCUSTCDT';
	var dbconn = new db.dbconn();
	dbconn.conn("*LOCAL");
	var stmt = new db.dbstmt(dbconn);

	stmt.exec(sSql, (x) => {
	  console.log("%s", JSON.stringify(x));
	  stmt.close();
	  dbconn.disconn();
	  dbconn.close();
	});
```

## Example 2: Fectching data using the fetchAll() API
```js
	var db = require('idb-connector');
	var sSql = 'SELECT STATE FROM QIWS.QCUSTCDT';
	var dbconn = new db.dbconn();
	dbconn.conn("*LOCAL");
	var stmt = new db.dbstmt(dbconn);

	stmt.prepare(sSql, () => {
	  stmt.executeSync(() => {
		stmt.fetchAll((x) => { 
		  console.log("%s", JSON.stringify(x));
		  stmt.close();
		});
	  });
	});
```

## Example 3: Call stored procedures
```js
    var db = require('idb-connector');
    var sql = "call QXMLSERV.iPLUG512K(?,?,?,?)";
    var dbconn = new db.dbconn();
    dbconn.conn("*LOCAL");
    var stmt = new db.dbstmt(dbconn);

    var ipc = "*NA";
    var ctl = "*here";
    var xmlIn = "<xmlservice><sh>system 'wrksbs'</sh></xmlservice>";
    var xmlOut;

    stmt.prepare(sql, () => {
      stmt.bindParam([
        [ipc, db.SQL_PARAM_INPUT, 1], // 1 -- null-terminated string
        [ctl, db.SQL_PARAM_INPUT, 1],
        [xmlIn, db.SQL_PARAM_INPUT, 0], // 0 -- CLOB string
        [xmlOut, db.SQL_PARAM_OUTPUT, 0],
      ], function(){
        stmt.execute((out) => { // 'out' is an array of output params
          for(var i = 0; i < out.length; i++)
            console.log(out[i]);
          stmt.close();
          dbconn.disconn();
          dbconn.close();
        });
      });
    });
```

# API Reference
https://www.ibm.com/developerworks/community/wikis/home?lang=en#!/wiki/IBM%20i%20Technology%20Updates/page/DB2%20for%20i%20Access%20APIs%20-%20New

# License
MIT.  View [`LICENSE`](https://bitbucket.org/litmis/nodejs-idb-connector/src/master/LICENSE) file.