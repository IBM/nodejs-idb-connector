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
    IN,
    CHAR,
} = db2a;

describe('Misc Test', () => {
    let dbConn, dbStmt;

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

            let rows = [];
            let orientation = SQL_FETCH_RELATIVE;
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

    describe('The stored procedure with result set issue', () => {
        let user = (process.env.USER).toUpperCase();
        let sql = `CALL ${user}.SPWITHRS`;
        let crtSP = `CREATE OR REPLACE PROCEDURE ${user}.SPWITHRS()     
                        LANGUAGE SQL                                       
                        DYNAMIC RESULT SETS 1
                        BEGIN ATOMIC 
                        DECLARE C1 CURSOR FOR

                        SELECT * FROM QIWS.QCUSTCDT LIMIT 5;

                        OPEN C1 ;
                        
                        SET RESULT SETS WITH RETURN TO CLIENT CURSOR C1 ;
                        
                        END`;

        before((done) => {
            dbStmt = new dbstmt(dbConn);
            dbStmt.exec(crtSP, (result, err) => {
                if (err) throw err;
                dbStmt.close();
                done();
            });
        });

        it(`execSync a stored procedure with result set`, (done) => {
            dbStmt.execSync(sql, (result) => {
                expect(result.length).to.equal(5);
                done();
            });
        });

        it(`exec a stored procedure with result set`, (done) => {
            dbStmt.exec(sql, (result, err) => {
                expect(result.length).to.equal(5);
                done();
            });
        });

        it(`fetchAllSync a stored procedure with result set`, (done) => {
            dbStmt.prepareSync(sql, (err) => {
                dbStmt.executeSync((err) => {
                    dbStmt.fetchAllSync((result, err) => {
                        expect(result.length).to.equal(5);
                        done();
                    });
                });
            });
        });

        it(`fetchAll a stored procedure with result set`, (done) => {
            dbStmt.prepareSync(sql, (err) => {
                dbStmt.executeSync((err) => {
                    dbStmt.fetchAllSync((result, err) => {
                        expect(result.length).to.equal(5);
                        done();
                    });
                });
            });
        });
    });

    describe('The input parameter trunction issue', () => {
        const sql_direct = `SELECT CITY FROM QIWS.QCUSTCDT WHERE CITY = 'Dallas2'`;
        const sql_bind = `SELECT * FROM QIWS.QCUSTCDT WHERE CITY = ?`;
        const params = [ [ 'Dallas2', IN, CHAR  ] ];

        it(`execSync does not truncates the too long input parameter`, (done) => {
            dbStmt.execSync(sql_direct, (result, error) => {
                expect(error).to.be.null;
                expect(result.length).to.equal(0);
                done();
            });
        });

        it(`bindParam truncates the too long input parameter`, (done) => {
            dbStmt.prepareSync(sql_bind, (error) => {
                expect(error).to.be.null;
                dbStmt.bindParamSync(params, (error) => {
                  expect(error).to.be.null;
                  dbStmt.executeSync((out, error) => {
                    expect(error).to.be.null;
                    dbStmt.fetchAllSync((result, returnCode) => {
                          expect(error).to.be.null;
                          expect(result).to.be.a('array');
                          expect(result.length).to.be.greaterThan(0);
                          done();
                      });
                  });
                });
              });
        });
    });
});
