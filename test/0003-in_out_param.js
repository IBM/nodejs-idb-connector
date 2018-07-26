let db = require('idb-connector')
let conn = new db.dbconn();
let sql = "CALL MOEXU.TEST2(?,?,?,?);";
// conn.debug(true);
conn.conn("*LOCAL");

let stmt = new db.dbstmt(conn);
let stmt2 = new db.dbstmt(conn);
let p1in = 1;
let p2out;
let p3inout = 2;
let p4inout = 10.0;

let params = [
   [p1in, db.SQL_PARAM_INPUT, 2],
   [p2out, db.SQL_PARAM_OUTPUT, 1],
   [p3inout, db.SQL_PARAM_INPUT_OUTPUT, 2],
   [p4inout, db.SQL_PARAM_INPUT_OUTPUT, 2]
];

stmt.prepareSync(sql, err => {
  stmt.bindParamSync(params, err => {
    stmt.executeSync((out) => {
      JSON.stringify(out);
      stmt.reset();
    });
  });
});

stmt2.prepare(sql, err => {
  stmt2.bindParam(params, err => {
    stmt2.execute((out, err) => {
      console.log(JSON.stringify(out));
      stmt2.close();
      conn.disconn();
      conn.close();
    });
  });
});


