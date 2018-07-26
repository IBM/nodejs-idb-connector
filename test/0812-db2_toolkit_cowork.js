// Test Case 0812 -- Test DB2 driver co-works with itoolkit.

var db = require('idb-connector');
var xt = require('itoolkit');

var conn = new xt.iConn("*LOCAL");

DBQuery("SELECT STATE FROM QIWS.QCUSTCDT");
DBQuery("SELECT LSTNAM FROM QIWS.QCUSTCDT");

function DBQuery(Sql, close){
  console.log("Test DB2 Query.");
  var dbconn = new db.dbconn();
  dbconn.conn("*LOCAL");
  XSQuery("XUMENG","passw0rd");
  var stmt = new db.dbstmt(dbconn);
  stmt.exec(Sql, function(x) {
    console.log("DB Query Done.\tRow Count: %d", x.length);
    stmt.close();
    dbconn.disconn();
    dbconn.close();
  });
}

function XSQuery(userName, password){
  var pgm = new xt.iPgm("QSYGETPH",{"lib":"QSYS","error":"on"});
  pgm.addParam(userName.toUpperCase(), "10A");
  pgm.addParam(password.toUpperCase(), "10A");
  pgm.addParam(" ", "12A", {"io":"out", "hex":"on"});
  pgm.addParam([
    [0,"10i0"],
    [0,"10i0"],
    [" ", "7A"],
    [" ", "1A"],
    [" ", "256A"]
  ]);
  pgm.addParam(10, "10i0");
  pgm.addParam(-1, "10i0");
  conn.add(pgm.toXML());
  // conn.setSyncMode(true);
  function cb(str) {
    var results = xt.xmlToJson(str);
    if(results.length == 0)
      console.log("XSToolkit Done.\tsuccess:false");
    else {
      results.forEach(function(result, i){
        if(result.hasOwnProperty('success'))
          console.log("XSToolkit Done.\tsuccess:" + result.success);
        else
          console.log("XSToolkit Done.\tsuccess:false");
      })
    }
  }
  console.log("Test XSToolkit.");
  conn.run(cb);
}
