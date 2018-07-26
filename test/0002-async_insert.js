let db = require('idb-connector')
let dbConn = new db.dbconn();

let limit = parseInt(process.argv[2]);
if(!limit) limit = 1;
console.log("Run %d times:\n", limit);
let sql = "INSERT INTO MOEXU.TEST1 (NAME) VALUES(?)";
dbConn.conn("*LOCAL");

for(let i=0; i<limit; i++) {
  let stmt = new db.dbstmt(dbConn);
  stmt.prepare(sql, err => {
    let id = Math.floor(Math.random() * 1000);
    let name = "XUMENG" + id;
    console.log(name);
    let params = [
       [name, db.SQL_PARAM_INPUT, 1]
    ];
    stmt.bindParam(params, err => {
      stmt.execute(err => {
        stmt.close();
      });
    });
  });
}

