// Test Case 0817 -- The "INSERT" not working issue

var db = require('idb-connector');
var dbconn = new db.dbconn();
dbconn.conn("*LOCAL");
var stmt = new db.dbstmt(dbconn);
for(var i = 0; i < 10000; i++) {
  var sql = "INSERT INTO PMR49664AA.GPPUSER VALUES(" + i + ", 'A', 'A', 12, 'C', 'abc', 'Albert', 'Frank', 'E', 'Mr.', 'Jr', 'Frank', 'Senior', 'N', 'falbert@us.ibm.com', 'description', 'E', '2017-12-31 23:59:59', '2017-12-31 23:59:59', 'A', 123, 'A', 'A', 'A', '2017-12-31 23:59:59', 'A', '2017-12-31 23:59:59')";
  // console.log(sql);
  stmt.execSync(sql);
}
delete stmt;
dbconn.disconn();
delete dbconn;
