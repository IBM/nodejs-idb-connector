// Test Case 0811 -- V8 failure with the -O3 gcc optimization
var db = require('idb-connector');
var sSql = 'SELECT STATE FROM QIWS.QCUSTCDT';

// Return the SQL status of fetching the row at offset nOffset
function FetchRowSync(sql, nOffset) {
  var rc = sql.fetchSync(db.SQL_FETCH_RELATIVE, nOffset, function(row) {
    console.log("[%d]%s", rc, JSON.stringify(row));
  });
  return rc;
}

var rowCount = 0;
var time, diff;

function FetchRequestSync() {
  var dbconn = new db.dbconn();
  // dbconn.debug(true);
  dbconn.conn("*LOCAL");
  var sql = new db.dbstmt(dbconn);
  console.log('FetchRequestSync (0)');
  sql.setStmtAttr(db.SQL_ATTR_CURSOR_TYPE, db.SQL_CURSOR_DYNAMIC);
  sql.prepareSync(sSql, function(){
    sql.executeSync(function(){
      while (FetchRowSync(sql, 1) !== db.SQL_NO_DATA_FOUND) {
        rowCount++;
      }
      sql.closeCursor();
    });
  });
  
  console.log('FetchRequestSync (fetchSync)');
  
  time = process.hrtime();
  sql.prepareSync(sSql);
  sql.executeSync();
  for(var i = 0; i < rowCount; i++)
    sql.fetchSync(db.SQL_FETCH_RELATIVE, 1, (row) => {
      console.log("%s", JSON.stringify(row));
    });
  diff = process.hrtime(time);
  console.log('benchmark fetchSync took %d ns', diff[0] * 1e9 + diff[1]);
  sql.closeCursor();
  
  console.log('FetchRequestSync (fetchAllSync)');

  time = process.hrtime();
  sql.prepareSync(sSql);
  sql.executeSync();
  sql.fetchAllSync((x) => {
    diff = process.hrtime(time);
    console.log('Row Count: %d, benchmark fetchAllSync took %d ns', x.length, diff[0] * 1e9 + diff[1]);
    console.log("%s", JSON.stringify(x));
    sql.closeCursor();
  });
  
  console.log('FetchRequestSync (execSync)');

  time = process.hrtime();
  sql.execSync(sSql, (x) => {
    diff = process.hrtime(time);
    console.log('Row Count: %d, benchmark execSync took %d ns', x.length, diff[0] * 1e9 + diff[1]);
    console.log("%s", JSON.stringify(x));
  });
  sql.close();
  dbconn.disconn();
  dbconn.close();
      
  console.log('FetchRequestSync done\n');
}

function FetchRequestAsync() {
  var dbconn = new db.dbconn();
  dbconn.conn("*LOCAL");
  
  console.log('FetchRequestAsync (fetch)');

  var sql = new db.dbstmt(dbconn);
  sql.setStmtAttr(db.SQL_ATTR_CURSOR_TYPE, db.SQL_CURSOR_DYNAMIC);
  sql.prepare(sSql, function(){
    sql.execute(function(){
      sql.fetch(db.SQL_FETCH_RELATIVE, 1, function(row, rc){
        if(rc !== db.SQL_NO_DATA_FOUND)
          console.log("%s", JSON.stringify(row));
        sql.closeCursor();
      });
    });
  });
  
  setTimeout(function(){
    console.log('FetchRequestAsync (fetchAll)');
    time = process.hrtime();
    sql.prepare(sSql, function(){
      sql.executeSync(function(){
        sql.fetchAll(function(x){ 
          diff = process.hrtime(time);
          console.log('Row Count: %d, benchmark fetchAll took %d ns', x.length, diff[0] * 1e9 + diff[1]);
          console.log("%s", JSON.stringify(x));
          sql.closeCursor();
        });
      });
    });
  }, 3000);

  setTimeout(function(){
    console.log('FetchRequestAsync (exec)');

    time = process.hrtime();
    sql.exec(sSql, function(x) {
      diff = process.hrtime(time);
      console.log('Row Count: %d, benchmark exec took %d ns', x.length, diff[0] * 1e9 + diff[1]);
      console.log("%s", JSON.stringify(x));
      sql.closeCursor();
      sql.close();
      dbconn.disconn();
      dbconn.close();
      console.log('FetchRequest done');
    });
  }, 5000);
}

FetchRequestSync();
FetchRequestAsync();
