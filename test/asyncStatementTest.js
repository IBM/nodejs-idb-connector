const {expect} = require('chai');
const db2a = require('../lib/db2a');

const {
  OUT, IN, CHAR, CLOB, NUMERIC, dbstmt, dbconn,
} = db2a;
// Test Statement Class Async Methods
describe('Statement Async Test', () => {
  describe('async prepare', () => {
    it('Prepares valid SQL and sends it to the DBMS, if fail, error is returned. ', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      const dbConn = new dbconn();
      dbConn.conn('*LOCAL');
      const dbStmt = new dbstmt(dbConn);

      dbStmt.prepare(sql, (error) => {
        if (error) {
          throw error;
        }
        expect(error).to.be.null;
        done();
      });
    });
  });

  describe('async bindParams', () => {
    it('associate parameter markers in an SQL statement to app variables', (done) => {
      const sql = 'INSERT INTO QIWS.QCUSTCDT(CUSNUM,LSTNAM,INIT,STREET,CITY,STATE,ZIPCOD,CDTLMT,CHGCOD,BALDUE,CDTDUE) VALUES (?,?,?,?,?,?,?,?,?,?,?) with NONE ';
      const dbConn = new dbconn();
      const dbConn2 = new dbconn();

      dbConn.conn('*LOCAL');
      dbConn2.conn('*LOCAL');

      let dbStmt = new dbstmt(dbConn);
      const dbStmt2 = new dbstmt(dbConn2);

      const params = [
        [9997, IN, NUMERIC], // CUSNUM
        ['Doe', IN, CHAR], // LASTNAME
        ['J D', IN, CHAR], // INITIAL
        ['123 Broadway', IN, CHAR], // ADDRESS
        ['Hope', IN, CHAR], // CITY
        ['WA', IN, CHAR], // STATE
        [98101, IN, NUMERIC], // ZIP
        [2000, IN, NUMERIC], // CREDIT LIMIT
        [1, IN, NUMERIC], // change
        [250, IN, NUMERIC], // BAL DUE
        [0.00, IN, NUMERIC], // CREDIT DUE
      ];

      dbStmt.exec('SELECT COUNT(CUSNUM) FROM QIWS.QCUSTCDT', (result, error) => {
        if (error) {
          throw error;
        }
        let rowsBefore = result[0]['00001'];
        rowsBefore = Number(rowsBefore);
        dbStmt.close();

        dbStmt2.prepare(sql, (error) => {
          if (error) {
            throw error;
          }
          dbStmt2.bindParam(params, (error) => {
            if (error) {
              throw error;
            }
            expect(error).to.be.null;
            dbStmt2.execute((out, error) => {
              if (error) {
                throw error;
              }
              expect(error).to.be.null;
              dbStmt2.close();
              dbStmt = new dbstmt(dbConn);
              dbStmt.exec('SELECT COUNT(CUSNUM) FROM QIWS.QCUSTCDT',
                (result, error) => {
                  if (error) {
                    throw error;
                  }
                  let rowsAfter = result[0]['00001'];
                  rowsAfter = Number(rowsAfter);
                  dbStmt.close();
                  expect(rowsAfter).to.equal(rowsBefore + 1);
                  done();
                });
            });
          });
        });
      });
    });


    describe('async execute', () => {
      it('retrieves output params from stored proc', (done) => {
        const sql = 'call QXMLSERV.iPLUG512K(?,?,?,?)';
        const dbConn = new dbconn();

        dbConn.conn('*LOCAL');
        const dbStmt = new dbstmt(dbConn);

        const ipc = '*NA';
        const ctl = '*here';
        const xmlIn = `<xmlservice><sh>system 'wrksbs'</sh></xmlservice>`;
        const xmlOut = '';
        const params = [
          [ipc, IN, CHAR],
          [ctl, IN, CHAR],
          [xmlIn, IN, CLOB],
          [xmlOut, OUT, CLOB],
        ];

        dbStmt.prepare(sql, (error) => {
          if (error) {
            throw error;
          }
          dbStmt.bindParam(params, (error) => {
            if (error) {
              throw error;
            }
            dbStmt.execute((out, error) => {
              if (error) {
                throw error;
              }
              expect(error).to.be.null;
              expect(out).to.be.a('array');
              expect(out.length).to.be.eq(1);
              done();
            });
          });
        });
      });

      it('executes prepared statement returns null because no output params are available', (done) => {
        const sql = 'SELECT * FROM QIWS.QCUSTCDT WHERE BALDUE > ?';
        const dbConn = new dbconn();
        const params = [
          [10.00, IN, NUMERIC],
        ];

        dbConn.conn('*LOCAL');
        const dbStmt = new dbstmt(dbConn);

        dbStmt.prepare(sql, (error) => {
          if (error) {
            throw error;
          }
          dbStmt.bindParam(params, (error) => {
            if (error) {
              throw error;
            }
            dbStmt.execute((out, error) => {
              if (error) {
                throw error;
              }
              expect(error).to.be.null;
              expect(out).to.be.null;
              done();
            });
          });
        });
      });
    });

    describe('async exec', () => {
      it('performs action of given SQL String', (done) => {
        const sql = 'SELECT * FROM QIWS.QCUSTCDT';
        const dbConn = new dbconn();

        dbConn.conn('*LOCAL');
        const dbStmt = new dbstmt(dbConn);

        dbStmt.exec(sql, (result, error) => {
          if (error) {
            throw error;
          }
          expect(error).to.be.null;
          expect(result).to.be.an('array');
          expect(result.length).to.be.greaterThan(0);
          done();
        });
      });
    });

    describe('async fetchAll', () => {
      it('retrieves all rows from execute function:', (done) => {
        const sql = 'SELECT * FROM QIWS.QCUSTCDT';
        const dbConn = new dbconn();

        dbConn.conn('*LOCAL');
        const dbStmt = new dbstmt(dbConn);

        dbStmt.prepare(sql, (error) => {
          if (error) {
            throw error;
          }
          dbStmt.execute((out, error) => {
            if (error) {
              throw error;
            }
            dbStmt.fetchAll((result, error) => {
              if (error) {
                throw error;
              }
              expect(error).to.be.null;
              expect(result).to.be.a('array');
              expect(result.length).to.be.greaterThan(0);
              done();
            });
          });
        });
      });
    });


    describe('async fetch', () => {
      it('retrieves one row from result set:', (done) => {
        const sql = 'SELECT * FROM QIWS.QCUSTCDT';
        const dbConn = new dbconn();

        dbConn.conn('*LOCAL');
        const dbStmt = new dbstmt(dbConn);

        dbStmt.prepare(sql, (error) => {
          if (error) {
            throw error;
          }
          dbStmt.execute((out, error) => {
            if (error) {
              throw error;
            }
            dbStmt.fetch((row, returnCode) => {
              if (returnCode !== 0) { // SQL_SUCCESS
                throw new Error('Rreturn Code was Not SQL SUCESS');
              }
              expect(returnCode).to.equal(0);
              expect(row).to.be.a('object');
              done();
            });
          });
        });
      });
    });
  });
});
