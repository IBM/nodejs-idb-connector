// Test Case 0820 -- The DB2 driver call back running twice issue

var db = require('idb-connector');
let conn = new db.dbconn();
conn.setConnAttr(db.SQL_ATTR_DBC_SYS_NAMING , db.SQL_FALSE);
conn.conn("*LOCAL");
let sSql = 'SELECT STATE FROM QIWS.QCUSTCDT';
let stmt = new db.dbstmt(conn);
let stmt2 = new db.dbstmt(conn);
// Case 1 : Call stored procedure
stmt.prepareSync("call QXMLSERV.iPLUG512K(?,?,?,?)");
let xmlOut;
stmt.bindParamSync([
  ["*NA", db.SQL_PARAM_INPUT, 1],
  ["*here", db.SQL_PARAM_INPUT, 1],
  ["<?xml version='1.0'?><myscript><cmd exec='rexx' error='fast'>RTVJOBA USER(?)</cmd></myscript>", db.SQL_PARAM_INPUT, 0],
  [xmlOut, db.SQL_PARAM_OUTPUT, 0],
]);
stmt.executeSync(out => {    
  console.log('Case 1 out: ' + out)
});
// Case 2 : Run the async calls one by one
stmt.prepareSync(sSql);
stmt.executeSync();
stmt.fetchAllSync(out => {
  delete stmt;
  console.log('Case 2 out: ' + JSON.stringify(out))
});
// Case 3 : Run the async calls in callbacks
stmt2.prepareSync(sSql, () => {
  stmt2.executeSync(() => {
    stmt2.fetchAllSync(function(out){
      delete stmt2;
      conn.disconn();
      delete conn;
      console.log('Case 3 out: ' + JSON.stringify(out))
    });
  });
});

