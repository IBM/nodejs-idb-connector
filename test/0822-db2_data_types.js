const db = require('idb-connector');
var dbconn = new db.dbconn();
dbconn.conn("*LOCAL");
// dbconn.debug( true );

var sqls = [
  "select * from (values smallint( -32768 )) as x (smallint_val)",
  "select * from (values int( -2147483648 )) as x (int_val)",
  "select * from (values bigint( -9223372036854775808 )) as x (bigint_val)",
  "select * from (values real( -12345.54321 )) as x (real_val)"
];

var stmt = new db.dbstmt(dbconn);
for (i in sqls) {
  console.log( sqls[i] );
  stmt.execSync(sqls[i], (results, error) => {
    if ( error ) console.log(error);
    console.log( JSON.stringify( results ) );
    stmt.closeCursor();
  });
}
stmt.close();
dbconn.disconn();
dbconn.close();