// Test Case 0821 -- Support passing error messages in parameter
var db = require('idb-connector');
var sql = "CALL QSYS2.QCMDEXC('CHGJOB INQMSGRPY(*SYSRPYL)')";

var dbconn = new db.dbconn();
// dbconn.debug(true);
dbconn.conn("*LOCAL");
var stmt = new db.dbstmt(dbconn);

console.log("\n[SQL]:" + sql);
stmt.exec(sql, (rs, error) => {
  if(error)
    console.log("[error]:" + error);
  console.log("[rs]:" + rs);
  stmt.closeCursor();
  delete stmt;
  dbconn.disconn();
  delete dbconn;
});