const {expect} = require('chai');
const util = require('util');
const db2a = require('../lib/db2a');

const {
  OUT, IN, CHAR, CLOB, NUMERIC, dbconn, dbstmt,
} = db2a;

describe('Statement Sync Test', () => {
  describe('prepare callback', () => {
    it('Prepares valid SQL and sends it to the DBMS, if fail, error is returned. ', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      const dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      const dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql, (error) => {
        if (error) {
          throw error;
        }
        expect(error).to.be.null;
      });
    });
  });

  describe('prepare no-callback', () => {
    it('Prepares valid SQL and sends it to the DBMS, if fail, error is returned. ', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      const dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      const dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql);
    });
  });

  describe('bindParams callback', () => {
    it('associate parameter markers in an SQL statement to app variables', () => {
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

      const result = dbStmt.execSync('SELECT COUNT(CUSNUM) FROM QIWS.QCUSTCDT');
      let rowsBefore = result[0]['00001'];

      rowsBefore = Number(rowsBefore);
      dbStmt.close();

      dbStmt2.prepareSync(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt2.bindParamSync(params, (error) => {
          if (error) {
            throw error;
          }
          expect(error).to.be.null;

          dbStmt2.executeSync((out, error) => {
            if (error) {
              throw error;
            }
            dbStmt = new dbstmt(dbConn);

            const result2 = dbStmt.execSync('SELECT COUNT(CUSNUM) FROM QIWS.QCUSTCDT');
            let rowsAfter = result2[0]['00001'];

            rowsAfter = Number(rowsAfter);
            dbStmt.close();

            expect(rowsAfter).to.equal(rowsBefore + 1);
          });
        });
      });
    });
  });

  describe('bindParams no-callback', () => {
    it('associate parameter markers in an SQL statement to app variables', () => {
      const sql = 'INSERT INTO QIWS.QCUSTCDT(CUSNUM,LSTNAM,INIT,STREET,CITY,STATE,ZIPCOD,CDTLMT,CHGCOD,BALDUE,CDTDUE) VALUES (?,?,?,?,?,?,?,?,?,?,?) with NONE ';
      const dbConn = new dbconn();
      const dbConn2 = new dbconn();
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
      dbConn.conn('*LOCAL');
      dbConn2.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn);
      const dbStmt2 = new dbstmt(dbConn2);
      // first get count of current rows
      const result = dbStmt.execSync('SELECT COUNT(CUSNUM) FROM QIWS.QCUSTCDT');
      let rowsBefore = result[0]['00001'];

      rowsBefore = Number(rowsBefore); // count retrurns as a String cast it to Number

      dbStmt.close();

      // now perform insert
      dbStmt2.prepareSync(sql);
      dbStmt2.bindParamSync(params);
      dbStmt2.executeSync();

      dbStmt = new dbstmt(dbConn);

      const result2 = dbStmt.execSync('SELECT COUNT(CUSNUM) FROM QIWS.QCUSTCDT');
      let rowsAfter = result2[0]['00001'];

      rowsAfter = Number(rowsAfter);

      expect(rowsAfter).to.equal((rowsBefore + 1));
    });
  });

  describe('exec callback', () => {
    it('performs action of given SQL String', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      const dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      const dbStmt = new dbstmt(dbConn);

      dbStmt.execSync(sql, (result, error) => {
        if (error) {
          throw error;
        }
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(result[0]).to.be.an('object');
      });
    });
  });

  describe('exec no-callback', () => {
    it('performs action of given SQL String', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      const dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      const dbStmt = new dbstmt(dbConn);

      const result = dbStmt.execSync(sql);
      expect(result).to.be.an('array');
      expect(result.length).to.be.greaterThan(0);
      expect(result[0]).to.be.an('object');
    });
  });

  describe('execute callback', () => {
    // it('retrieves output parameters from stored proc using executeSync with a callback', () => {
    //   const sql = 'call QXMLSERV.iPLUG512K(?,?,?,?)';
    //   const dbConn = new dbconn();

    //   dbConn.conn('*LOCAL');
    //   const dbStmt = new dbstmt(dbConn);

    //   const ipc = '*NA';
    //   const ctl = '*here';
    //   const xmlIn = `<xmlservice><sh>system 'wrksbs'</sh></xmlservice>`;
    //   const xmlOut = '';
    //   const params = [
    //     [ipc, IN, CHAR],
    //     [ctl, IN, CHAR],
    //     [xmlIn, IN, CLOB],
    //     [xmlOut, OUT, CLOB],
    //   ];

    //   dbStmt.prepareSync(sql, (error) => {
    //     if (error) {
    //       throw error;
    //     }
    //     dbStmt.bindParamSync(params, (error) => {
    //       if (error) {
    //         throw error;
    //       }
    //       dbStmt.executeSync((result, error) => {
    //         if (error) {
    //           throw error;
    //         }
    //         expect(error).to.be.null;
    //         expect(result).to.be.a('array');
    //         expect(result.length).to.be.eq(1);
    //       });
    //     });
    //   });
    // });


    it('executes prepared statement using executeSync with callback. Returns null because no output params are available', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT WHERE BALDUE > ?';
      const dbConn = new dbconn();
      const params = [
        [10.00, IN, NUMERIC],
      ];

      dbConn.conn('*LOCAL');
      const dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.bindParamSync(params, (error) => {
          if (error) {
            throw error;
          }
          dbStmt.executeSync((out, error) => {
            if (error) {
              throw error;
            }
            expect(error).to.be.null;
            expect(out).to.be.null;
          });
        });
      });
    });
  });

  describe('execute no-callback', () => {
    // it('retrieves output parameters from stored procedure using executeSync without a callback:', () => {
    //   const sql = 'CALL QXMLSERV.iPLUG512K(?,?,?,?)';
    //   const dbConn = new dbconn();

    //   dbConn.conn('*LOCAL');
    //   const dbStmt = new dbstmt(dbConn);

    //   const ipc = '*NA';
    //   const ctl = '*here';
    //   const xmlIn = `<xmlservice><sh>system 'wrksbs'</sh></xmlservice>`;
    //   const xmlOut = '';
    //   const params = [
    //     [ipc, IN, CHAR],
    //     [ctl, IN, CHAR],
    //     [xmlIn, IN, CLOB],
    //     [xmlOut, OUT, CLOB],
    //   ];

    //   dbStmt.prepareSync(sql);
    //   dbStmt.bindParamSync(params);

    //   const out = dbStmt.executeSync();

    //   expect(out).to.be.a('array');
    //   expect(out.length).to.be.eq(1);
    // });


    it('executes prepared statement using executeSync without callback. Returns null because no output params are available', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT WHERE BALDUE > ?';
      const dbConn = new dbconn();
      const params = [
        [10.00, IN, NUMERIC],
      ];

      dbConn.conn('*LOCAL');
      const dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql);
      dbStmt.bindParamSync(params);
      const out = dbStmt.executeSync();
      expect(out).to.be.null;
    });
  });


  describe('fetchAll callback', () => {
    it('retrieves results from execute function:', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      const dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      const dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.executeSync((out, error) => {
          if (error) {
            throw error;
          }
          dbStmt.fetchAllSync((result, error) => {
            if (error) {
              console.log(util.inspect(error));
              throw error;
            }
            expect(error).to.be.null;
            expect(result).to.be.a('array');
            expect(result.length).to.be.greaterThan(0);
          });
        });
      });
    });
  });

  describe('fetchAll no-callback', () => {
    it('retrieves results from execute function:', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      const dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      const dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql);
      dbStmt.executeSync();
      const result = dbStmt.fetchAllSync();
      expect(result).to.be.a('array');
      expect(result.length).to.be.greaterThan(0);
    });
  });

  describe('fetch callback', () => {
    it('retrieves results from execute function:', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      const dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      const dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.executeSync((out, error) => {
          if (error) {
            throw error;
          }
          dbStmt.fetchSync((result, returnCode) => {
            if (returnCode !== 0) { // SQL_SUCCESS
              throw new Error('Rreturn Code was Not SQL SUCESS');
            }
            expect(returnCode).to.equal(0);
            expect(result).to.be.a('object');
          });
        });
      });
    });
  });

  describe('fetch no-callback', () => {
    it('retrieves results from execute function:', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      const dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      const dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql);
      dbStmt.executeSync();
      const result = dbStmt.fetchSync();
      expect(result).to.be.a('object');
    });
  });
});
