var dba = require('./db2a');

// The new DB2 driver can reuse one connection with multiple queries.
var conn = new dba.dbconn();
conn.init(function(){
  conn.conn("*LOCAL");
});
    
function testQureySyncI(sql) {
  console.log("\n  testQureySyncI() ----------------------------------");
  stmt = new dba.dbstmt(conn);

  console.log("Executing statement A");
  stmt.execSync(sql, function() {
    console.log("Statement A is done.");
  });
  console.log("Executing statement B");
  stmt.execSync(sql, function() {
    console.log("Statement B is done.");
  });
}

function testQureyAsyncI(sql) {
  console.log("\n  testQureyAsyncI() ----------------------------------");

  stmtA = new dba.dbstmt(conn);
  stmtB = new dba.dbstmt(conn);
  
  console.log("Executing statement A");
  stmtA.exec(sql, function() {
    console.log("Statement A is done.");
  });
  console.log("Executing statement B");
  stmtB.exec(sql, function() {
    console.log("Statement B is done.");
  });
}

function testQureySyncIII(sql) {
	console.log("\n  testQureySyncIII() ----------------------------------");
	try{
    stmt = new dba.dbstmt(conn);
		stmt.prepareSync(sql, function() {
      stmt.executeSync(function() {
        var fieldNum = stmt.numFields();
        console.log("There are %d fields in each row.\n", fieldNum);
        console.log("Name | Length | Type | Precise | Scale | Null");
        for(var i = 0; i < fieldNum; i++)
          console.log("%s | %d | %d | %d | %d | %d", stmt.fieldName(i), stmt.fieldWidth(i), stmt.fieldType(i), stmt.fieldPrecise(i), stmt.fieldScale(i), stmt.fieldNullable(i));
        console.log("There are %d rows affected.", stmt.numRows());
        stmt.fetchAllSync(function(rs) {
          console.log("Result is : %s", JSON.stringify(rs));
        });
      });
    });
	} catch(e) {
		console.log(e);
	}
};

function testQureyAsyncIII(sql, cb) {
	console.log("\n  testQureyAsyncIII() ----------------------------------");
	try{
    stmt = new dba.dbstmt(conn);
		stmt.prepare(sql, function() {
      stmt.execute(function() {
        var fieldNum = stmt.numFields();
        console.log("There are %d fields in each row.\n", fieldNum);
        console.log("Name | Length | Type | Precise | Scale | Null");
        for(var i = 0; i < fieldNum; i++)
          console.log("%s | %d | %d | %d | %d | %d", stmt.fieldName(i), stmt.fieldWidth(i), stmt.fieldType(i), stmt.fieldPrecise(i), stmt.fieldScale(i), stmt.fieldNullable(i));
        console.log("There are %d rows affected.", stmt.numRows());
        stmt.fetchAll(function(rs) {
          console.log("Result is : %s", JSON.stringify(rs));
          cb();
        });
      });
    });
	} catch(e) {
		console.log(e);
	}
};

function testQureySyncV(sql) {
  console.log("\n  testQureySyncV() ----------------------------------");
	var ipc = "*NA";
	var ctl = "*here";
	var xmlIn ="<?xml version='1.0'?><script><comment>your XML script here, cmd, sh, sql, pgm (below)</comment></script>";
	var xmlOut = "";
	try{
    stmt = new dba.dbstmt(conn);
		stmt.prepareSync(sql, function(){
      stmt.bindParamSync([
        [ipc, dba.SQL_PARAM_INPUT, 1],
        [ctl, dba.SQL_PARAM_INPUT, 1],
        [xmlIn, dba.SQL_PARAM_INPUT, 0],
        [xmlOut, dba.SQL_PARAM_OUTPUT, 0],
      ], function(){
        stmt.executeSync(function(out) {  //out is an array of the output parameters.
          for (i=0; i<out.length; i++)
            console.log("Output Param[%d] = %s \n", i, out[i]);
        });
      });
    });
	} catch(e) {
		console.log(e);
	}
};

function testQureyAsyncV(sql, cb) {
  console.log("\n  testQureyAsyncV() ----------------------------------");
	var ipc = "*NA";
	var ctl = "*here";
	var xmlIn ="<?xml version='1.0'?><script><comment>your XML script here, cmd, sh, sql, pgm (below)</comment></script>";
	var xmlOut = "";
	try{
    stmt = new dba.dbstmt(conn);
		stmt.prepare(sql, function(){
      stmt.bindParam([
        [ipc, dba.SQL_PARAM_INPUT, 1],
        [ctl, dba.SQL_PARAM_INPUT, 1],
        [xmlIn, dba.SQL_PARAM_INPUT, 0],
        [xmlOut, dba.SQL_PARAM_OUTPUT, 0],
      ], function(){
        stmt.execute(function(out) {  //out is an array of the output parameters.
          for (i=0; i<out.length; i++)
            console.log("Output Param[%d] = %s \n", i, out[i]);
          cb();
        });
      });
    });
	} catch(e) {
		console.log(e);
	}
};

// testQureySyncI("SELECT STATE FROM QIWS.QCUSTCDT");
// testQureySyncIII("SELECT STATE FROM QIWS.QCUSTCDT");
// testQureySyncV("call QXMLSERV.iPLUG512K(?,?,?,?)");

// testQureyAsyncI("SELECT STATE FROM QIWS.QCUSTCDT");
// testQureyAsyncIII("SELECT STATE FROM QIWS.QCUSTCDT", function(){
  // testQureyAsyncV("call QXMLSERV.iPLUG512K(?,?,?,?)", function(){});
// });

testQureyAsyncIII("SELECT STATE FROM QIWS.QCUSTCDT", function(){});
testQureyAsyncIII("SELECT LSTNAM FROM QIWS.QCUSTCDT", function(){});
