const db2i = require("idb-connector");
var dbconn = new db2i.dbconn();
dbconn.debug(true);
dbconn.conn("*LOCAL");
  
// function connect(callback) {
  // var dbconn = new db2i.dbconn();
  // dbconn.debug(true);
  // dbconn.conn("*LOCAL", () => {
    // callback(dbconn);
  // });
// }

function call(cb) {
  var stm = new db2i.dbstmt(dbconn);
  stm.exec("call xumeng.leak()", () => {
    stm.close() //Close statement
    // if (error) throw new Error(error);
    cb();
  });
}

setInterval(() => {
  call(() => { console.log("called"); });
}, 20);
    

// connect((dbconn) => {
  // call(dbconn, () => {
    // setInterval(() => {
      // call(dbconn, () => { console.log("called"); });
    // }, 20);}
  // );
// });
