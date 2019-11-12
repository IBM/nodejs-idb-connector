const { expect } = require('chai');
const db2a = require('../lib/db2a');

const {
    SQL_ATTR_CURSOR_TYPE,
    SQL_CURSOR_DYNAMIC,
    SQL_FETCH_RELATIVE,
    SQL_FETCH_NEXT,
    SQL_ERROR,
    SQL_NO_DATA_FOUND,
    dbconn,
    dbstmt,
} = db2a;

describe('Misc Test', () => {
    var dbConn, dbStmt;

    before(() => {
        dbConn = new dbconn();
        dbConn.conn('*LOCAL');
    });

    after(() => {
        dbConn.disconn();
        dbConn.close();
    });

    beforeEach(() => {
        dbStmt = new dbstmt(dbConn);
    });

    afterEach(() => {
        dbStmt.close();
    });

    describe('The wide character issue', () => {
        it(`query some wide characters`, (done) => {
            const wchar = "ÇÇÇÇÇÇÇÇ";
            dbStmt.exec("SELECT * FROM (VALUES CHAR('" + wchar + "')) AS X (C)", (rs) => {
                expect(rs[0].C).to.equal(wchar)
                done();
            });
        });
    });

    describe('The fetch with offset issue', () => {
        it(`fetch with offset`, (done) => {
            dbStmt.setStmtAttr(SQL_ATTR_CURSOR_TYPE, SQL_CURSOR_DYNAMIC);
            dbStmt.prepareSync("select schema_name from qsys2.sysschemas limit 10");
            dbStmt.executeSync();

            var rows = [];
            var orientation = SQL_FETCH_RELATIVE;
            do {
                dbStmt.fetchSync(orientation, 9, function (row, error) {
                    if (error != SQL_ERROR && error != SQL_NO_DATA_FOUND)
                        rows.push(row);
                    rc = error;
                });
                orientation = SQL_FETCH_NEXT;
            } while (rc != SQL_ERROR && rc != SQL_NO_DATA_FOUND)
            expect(rows.length).to.equal(1);
            done();
        });
    });
});
