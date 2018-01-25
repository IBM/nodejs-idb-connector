var sql = 'SELECT STATE FROM QIWS.QCUSTCDT';

var dba = require('../lib/db2a');
var dbconns = new dba.dbconn();
// dbconns.debug(true);
dbconns.conn("*LOCAL");
var sqlAs = new dba.dbstmt(dbconns);
var sqlBs = new dba.dbstmt(dbconns);
console.log("Execute A.");
sqlAs.execSync(sql, function(x) {
  console.log("Execute A Done. Row Count: %d", x.length);
  sqlAs.close();
});
console.log("Execute B.");
sqlBs.execSync(sql, function(x) {
  console.log("Execute B Done. Row Count: %d", x.length);
  sqlBs.close();
});
dbconns.disconn();
dbconns.close();

console.log("Test Async in callback.");
var dbconn = new dba.dbconn();
// dbconn.debug(true);
dbconn.conn("*LOCAL");
var sqlA = new dba.dbstmt(dbconn);
var sqlB = new dba.dbstmt(dbconn);
console.log("Execute A.");
sqlA.exec(sql, function(x) {
  console.log("Execute A Done. Row Count: %d", x.length);
  sqlA.close();
  console.log("Execute B.");
  sqlB.exec(sql, function(x) {
    console.log("Execute B Done. Row Count: %d", x.length);
    sqlB.close();
    dbconn.disconn();
    dbconn.close();
  });
});

setTimeout(function(){
  console.log("Test Async concurrently.");
  var dbconn = new dba.dbconn();
  // dbconn.debug(true);
  dbconn.conn("*LOCAL");
  var sqlA = new dba.dbstmt(dbconn);
  var sqlB = new dba.dbstmt(dbconn);
  console.log("Execute A.");
  sqlA.exec(sql, function(x) {
    console.log("Execute A Done. Row Count: %d", x.length);
    sqlA.close();
  });
  console.log("Execute B.");
  sqlB.exec(sql, function(x) {
    console.log("Execute B Done. Row Count: %d", x.length);
    sqlB.close();
  });
  setTimeout(function(){
    dbconn.disconn();
    dbconn.close();
  },1000); //Sleep for 1 sec to wait for both executions done before disconnecting.
},1000); //Sleep for 1 sec to wait for previous test done.