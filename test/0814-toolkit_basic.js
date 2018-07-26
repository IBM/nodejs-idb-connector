// Test Case 0814 -- Test Basic itoolkit functions

var xt = require('itoolkit');

var opt = {
  db   : "*LOCAL",
  user : "XUMENG",
  pwd  : "PASSW0RD",
	host : "9.5.39.183",
	port : 10080,
	path : "/cgi-bin/xmlcgi.pgm"
};

function cbJson(str) {
  var results = xt.xmlToJson(str);
  results.forEach(function(result, i){
    if(result.hasOwnProperty('success'))
      console.log(result.type + " success : " + result.success);
  })
}

var conn = new xt.iConn(opt.db, opt.user, opt.pwd);  // DB2 
conn.debug(true);
console.log("Test DB2 Call");
conn.add(xt.iCmd("RTVJOBA USRLIBL(?) SYSLIBL(?)"));  /* Test iCmd */
conn.add(xt.iSh("system -i wrksyssts"));	/* Test iSh */
var pgm = new xt.iPgm("QWCRSVAL", {"lib":"QSYS"});
var outBuf = [
		[0, "10i0"],
		[0, "10i0"],
		["", "36h"],
		["", "10A"],
		["", "1A"],
		["", "1A"],
		[0, "10i0"],
		[0, "10i0"]
	];
pgm.addParam(outBuf, {"io":"out"});
pgm.addParam(66, "10i0");
pgm.addParam(1, "10i0");
pgm.addParam("QCCSID", "10A");
pgm.addParam(this.errno, {"io":"both", "len" : "rec2"});
conn.add(pgm);

var sql = new xt.iSql();  /* Test iSql Class */
// sql.addQuery("SELECT LSTNAM, STATE FROM QIWS.QCUSTCDT");
sql.prepare("call qsys2.tcpip_info()");
sql.execute();
sql.fetch();
sql.free();
conn.add(sql);
// conn.run(cb);
conn.run(cbJson);

// console.log("Test Restful Call");
// var connRst = new xt.iConn(opt.db, opt.user, opt.pwd, opt);  // REST 
// connRst.add(xt.iCmd("RTVJOBA USRLIBL(?) SYSLIBL(?)"));  /* Test iCmd */
// connRst.add(xt.iSh("system -i wrksyssts"));	/* Test iSh */
// var pgm = new xt.iPgm("QWCRSVAL", {"lib":"QSYS"});
// var outBuf = [
		// [0, "10i0"],
		// [0, "10i0"],
		// ["", "36h"],
		// ["", "10A"],
		// ["", "1A"],
		// ["", "1A"],
		// [0, "10i0"],
		// [0, "10i0"]
	// ];
// pgm.addParam(outBuf, {"io":"out"});
// pgm.addParam(66, "10i0");
// pgm.addParam(1, "10i0");
// pgm.addParam("QCCSID", "10A");
// pgm.addParam(this.errno, {"io":"both", "len" : "rec2"});
// connRst.add(pgm);

// var sql = new xt.iSql();  /* Test iSql Class */
// // sql.addQuery("SELECT LSTNAM, STATE FROM QIWS.QCUSTCDT");
// sql.prepare("call qsys2.tcpip_info()");
// sql.execute();
// sql.fetch();
// sql.free();
// connRst.add(sql);
// // connRst.run(cb);
// connRst.run(cbJson);