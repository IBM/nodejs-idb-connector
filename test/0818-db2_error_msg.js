// Test Case 0818 -- DB2 driver error message test

var db = require('idb-connector');
var badSql = "SELECT * FROM UNKNOWN.NOTTHERE";
var goodSql = "SELECT STREET FROM QIWS.QCUSTCDT LIMIT 1";
var dbconn = new db.dbconn();
// dbconn.debug(true);
dbconn.conn("*LOCAL");
var stmt = new db.dbstmt(dbconn);

function exec(sql){
  console.log("\n[SQL]:" + sql);
  stmt.exec(sql, function(rs, err) {
    if(err) {
      console.log("[error]:" + err);
      stmt.stmtError(db.SQL_HANDLE_STMT, 1, (rs) => {console.log("[stmtError]:" + rs);});
    }
    else
      console.log(rs);
    stmt.closeCursor();
  });
}

function prepare(sql){
  console.log("\n[SQL]:" + sql);
  stmt.prepare(sql, function(error) {
    if(error)
      console.log("[error]:" + error);
    else
      console.log("Prepare No Error.");
    stmt.closeCursor();
  });
}

function execute(sql){
  console.log("\n[SQL]:" + sql);
  if(sql == goodSql) {
    stmt.prepare(sql, function(error) {
      if(error) {
        console.log("[error]:" + error);
        stmt.closeCursor();
      }
      else {
        stmt.execute(function(error) {
          if(error)
            console.log("[error]:" + error);
          else
            console.log("Execute No Error.");
          stmt.closeCursor();
        });
      }
    });
  } else {
    stmt.closeCursor();
    stmt.execute(function(error) {
      console.log("[error]:" + error);
    });
  }
}

function fetch(sql){
  console.log("\n[SQL]:" + sql);
  stmt.prepare(goodSql, function(error) {
    if(error){
      console.log("[error]:" + error);
      stmt.closeCursor();
    }
    else {
      if(sql == goodSql)
        stmt.execute(function(error) {
          if(error){
            console.log("[error]:" + error);
            stmt.closeCursor();
          }
          else {
            stmt.fetch(function callback(row, rc, error){
              if(error)
                console.log("[error]:" + error);
              else
                console.log(row);
              stmt.closeCursor();
            });
          }
        });
      else
        stmt.fetch(function callback(row, rc, error){
          if(error)
            console.log("[error]:" + error);
          else
            console.log(row);
          stmt.closeCursor();
        });  
    }
  });
}

function fetchAll(sql){
  console.log("\n[SQL]:" + sql);
  stmt.prepare(goodSql, function(error) {
    if(error){
      console.log("[error]:" + error);
      stmt.closeCursor();
    }
    else {
      if(sql == goodSql)
        stmt.execute(function(error) {
          if(error){
            console.log("[error]:" + error);
            stmt.closeCursor();
          }
          else {
            stmt.fetchAll(function callback(rs, error){
              if(error)
                console.log("[error]:" + error);
              else
                console.log(rs);
              stmt.closeCursor();
            });
          }
        });
      else
        stmt.fetchAll(function callback(rs, error){
          if(error)
            console.log("[error]:" + error);
          else
            console.log(rs);
          stmt.closeCursor();
        });  
    }
  });
}

// querySync();
var timeout = 0;
exec(badSql);
// setTimeout(function(){exec(goodSql);}, timeout+=1000);
// setTimeout(function(){prepare(badSql);}, timeout+=1000);
// setTimeout(function(){prepare(goodSql);}, timeout+=1000);
// setTimeout(function(){execute(badSql);}, timeout+=1000);
// setTimeout(function(){execute(goodSql);}, timeout+=1000);
// setTimeout(function(){fetch(badSql);}, timeout+=1000);
// setTimeout(function(){fetch(goodSql);}, timeout+=1000);
// setTimeout(function(){fetchAll(badSql);}, timeout+=1000);
// setTimeout(function(){fetchAll(goodSql);}, timeout+=1000);
