// Test Case 0816 -- The async HTTP request hanging issue

var http = require('http');
var url = require('url'); 
var fs = require('fs'); 
var db = require('idb-connector');

var sql = 'SELECT STATE FROM QIWS.QCUSTCDT';
var ip = "0.0.0.0"; 
var port = 8086; 

var webserver = http.createServer(function (req, res) { 
  var dbconn = new db.dbconn();
  // dbconn.debug(true);
  dbconn.conn("*LOCAL");	
  var stmt = new db.dbstmt(dbconn);
  res.writeHead(200, {'Content-Type': 'text/plain'}); 
  if(req.url == "/a"){ 
    // fs.readFile('/home/xumeng/test/test', (err, data) => {
      // if (err) throw err;
      // res.end("Async Run: " + data);
    // });
    // console.log("Async");
    stmt.exec(sql, function(rs) {	  // Query the statement 
      // stmt.closeCursor();
      stmt.close();
      dbconn.disconn();
      dbconn.close();
      res.end("Async Run: " + JSON.stringify(rs));
    }); 
  } else if(req.url == "/s") { 
    // res.end("Sync Run: " + fs.readFileSync('/home/xumeng/test/test'));
    // console.log("Sync");
    stmt.execSync(sql, function(rs) {	  // Query the statement 
      // stmt.closeCursor();
      stmt.close();
      dbconn.disconn();
      dbconn.close();
      res.end("Sync Run: " + JSON.stringify(rs));
    }); 
  } else {
    stmt.close();
    dbconn.disconn();
    dbconn.close();
    res.end("Unknown Request");
  }
}); 
webserver.listen(port, ip); 
console.log('Server running at http://' + ip + ':' + port);