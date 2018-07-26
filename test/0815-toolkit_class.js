// Test Case 0815 -- Test itoolkit classes

var xt = require('itoolkit');

var opt = {
  db   : "*LOCAL",
  user : "XUMENG",
  pwd  : "PASSW0RD",
	host : "9.5.39.183",
	port : 10080,
	path : "/cgi-bin/xmlcgi.pgm"
};

var wk = require('itoolkit/lib/iwork');
var pd = require('itoolkit/lib/iprod');
var usp = require('itoolkit/lib/iuserSpace');
var nt = require('itoolkit/lib/inetwork');
var ob = require('itoolkit/lib/iobj');
var dq = require('itoolkit/lib/idataq');

var conn = new xt.iConn(opt.db, opt.user, opt.pwd);  // DB2 
// var connRst = new xt.iConn(opt.db, opt.user, opt.pwd, opt);  // REST

var work = new wk.iWork(conn);
var prod = new pd.iProd(conn);
var userspace = new usp.iUserSpace(conn);
var net = new nt.iNetwork(conn);
var obj = new ob.iObj(conn);
var dtq = new dq.iDataQueue(conn);

function cb(str) {console.log(str);}

/*********************Sync Test************************/
console.log("QCCSID = " + work.getSysValue("QCCSID"));  
console.log(JSON.stringify(work.getSysStatus()));  
console.log(JSON.stringify(work.getSysStatusExt()));  
console.log(JSON.stringify(work.getJobStatus("603298")));  
console.log(JSON.stringify(work.getJobInfo("ADMIN", "QTMHHTTP", "603298")));  
console.log(JSON.stringify(work.getDataArea("", "*LDA", 100)));  
console.log(JSON.stringify(prod.getPTFInfo("SI51153")));  
console.log(JSON.stringify(prod.getProductInfo("5770DG1")));
console.log(JSON.stringify(prod.getProductInfo("5770SS1", 1)));
console.log(JSON.stringify(prod.getProductInfo("5733OPS")));
console.log(JSON.stringify(prod.getInstalledProducts(),null,4));  
console.log(JSON.stringify(userspace.createUserSpace("TSTUSP", "XUMENG", "", "", "", "")));  
console.log(JSON.stringify(userspace.setUserSpaceData("TSTUSP", "XUMENG", 20, "Hello World![Sync]")));  
console.log(JSON.stringify(userspace.getUserSpaceData("TSTUSP", "XUMENG", 20)));  
console.log(JSON.stringify(userspace.deleteUserSpace("TSTUSP", "XUMENG")));  
console.log(JSON.stringify(net.getTCPIPAttr()));  
console.log(JSON.stringify(net.getNetInterfaceData("localhost")));  
console.log(JSON.stringify(obj.retrUsrAuth("*PUBLIC","*PGM","XMLCGI","QXMLSERV")));  
console.log(JSON.stringify(obj.retrCmdInfo("CRTLIB")));  
console.log(JSON.stringify(obj.retrPgmInfo("XMLCGI","QXMLSERV")));  
console.log(JSON.stringify(obj.retrSrvPgmInfo("QZSRVSSL","QHTTPSVR")));  
console.log(JSON.stringify(obj.retrUserInfo("XUMENG")));  
console.log(JSON.stringify(obj.retrUserAuthToObj("/QOpenSys/QIBM/ProdData/Node/bin/node")));  
console.log(JSON.stringify(obj.addToLibraryList("QHTTPSVR")));
console.log(dtq.sendToDataQueue("TESTQ", "XUMENG", "Hello World![Sync]"));  
console.log(dtq.receiveFromDataQueue("TESTQ", "XUMENG", 20));  
console.log(dtq.clearDataQueue("TESTQ", "XUMENG")); 

/*********************Async Test************************/
work.getSysValue("QCCSID", function(output) {
  console.log("QCCSID = " + output);
  work.getSysStatus(function(output) {
    console.log(JSON.stringify(output));
    work.getSysStatusExt(function(output) {
      console.log(JSON.stringify(output));
    });
  });  
});


work.getJobStatus("306961", function(output) {console.log(JSON.stringify(output));});
work.getJobInfo("ADMIN", "QTMHHTTP", "306961", function(output) {console.log(JSON.stringify(output));});
work.getDataArea("", "*LDA", 100, function(output) {console.log(JSON.stringify(output));});
prod.getPTFInfo("SI54662", function(output) {console.log(JSON.stringify(output));});
prod.getProductInfo("5770DG1", "", function(output) {console.log(JSON.stringify(output));});
prod.getInstalledProducts(function(output) {console.log(JSON.stringify(output));});
userspace.createUserSpace("TSTUSP", "XUMENG", "", "", "", "", function() {
  userspace.setUserSpaceData("TSTUSP", "XUMENG", 20, "Hello World![Async]", function(output) {
    console.log(output);
    userspace.getUserSpaceData("TSTUSP", "XUMENG", 20, function(output) {
      console.log(output);
      userspace.deleteUserSpace("TSTUSP", "XUMENG");  
    });  
  });
});

net.getTCPIPAttr(function(output) {console.log(JSON.stringify(output));});
net.getNetInterfaceData("localhost", function(output) {console.log(JSON.stringify(output));});
obj.retrUsrAuth("*PUBLIC","*PGM","XMLCGI","QXMLSERV", function(output) {console.log(JSON.stringify(output));});
obj.retrCmdInfo("CRTLIB", function(output) {console.log(JSON.stringify(output));});
obj.retrPgmInfo("XMLCGI","QXMLSERV", function(output) {console.log(JSON.stringify(output));});
obj.retrSrvPgmInfo("QZSRVSSL","QHTTPSVR", function(output) {console.log(JSON.stringify(output));});
obj.retrUserInfo("XUMENG", function(output) {console.log(JSON.stringify(output));});
obj.retrUserAuthToObj("/home/xumeng/node-0.10.29-release-ppc.tar", function(output) {console.log(JSON.stringify(output));});
dtq.sendToDataQueue("TESTQ", "XUMENG", "Hello World![Async]", function() {
  dtq.receiveFromDataQueue("TESTQ", "XUMENG", 20, function(output) {
    console.log(output);
    dtq.clearDataQueue("TESTQ", "XUMENG"); 
  });
});  



