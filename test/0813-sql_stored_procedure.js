// Test Case 0813 -- Test Basic Query and Stored Procedure
var db = require('idb-connector');
var sql = "call QXMLSERV.iPLUG512K(?,?,?,?)";
var dbconn = new db.dbconn();
dbconn.conn("*LOCAL");
var stmt = new db.dbstmt(dbconn);
var ipc = "*NA";
var ctl = "*here";
var xmlIn ="<xmlservice><sh>system 'wrksbs'</sh></xmlservice>";
var xmlOut = "";
console.log("Test Stored Procedure.");
stmt.prepare(sql, function(){
  stmt.bindParam([
    [ipc, db.SQL_PARAM_INPUT, 1],
    [ctl, db.SQL_PARAM_INPUT, 1],
    [xmlIn, db.SQL_PARAM_INPUT, 0],
    [xmlOut, db.SQL_PARAM_OUTPUT, 0],
  ], function(){
    stmt.execute(function(out) {  //out is an array of the output parameters.
      for(var i=0; i < out.length; i++)
        console.log(out[out.length - 1]);
      console.log("Test Stored Procedure Done.");
      stmt.close();
      dbconn.disconn();
      dbconn.close();
    });
  });
});


