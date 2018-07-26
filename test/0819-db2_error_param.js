// Test Case 0819 -- Support passing error messages in parameter

var db = require('idb-connector');
var sql = "call rudip.resset()";

var dbconn = new db.dbconn();
dbconn.debug(true);
dbconn.conn("*LOCAL");
var stmt = new db.dbstmt(dbconn);

console.log("\n[SQL]:" + sql);
stmt.prepare(sql, function(error) {
  if(error) {
    console.log("[prepare error]:" + error);
    stmt.closeCursor();
    delete sql;
    dbconn.disconn();
    delete dbconn;
  }
  else {
    stmt.execute(function(error) {
      if(error) {
        console.log("[execute error]:" + error);
        delete sql;
        dbconn.disconn();
        delete dbconn;
      }
      else {
        stmt.fetchAll(function callback(rs, error){
          if(error)
            console.log("[fetchAll error]:" + error);
          else
            console.log("Result: %s", JSON.stringify(rs));
          delete sql;
          dbconn.disconn();
          delete dbconn;
        });
      }
    });
  }
});
