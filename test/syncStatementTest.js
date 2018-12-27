const expect = require('chai').expect;
const db2a = require('../lib/db2a');
const util = require('util');
const {OUT, IN, CHAR, CLOB, NUMERIC, dbconn, dbstmt} = db2a;

describe('Statement Sync Test', () => {
  describe('prepare callback', () => {
    it('Prepares valid SQL and sends it to the DBMS, if fail, error is returned. ', () => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql, (error) =>{
        if (error){
          throw error;
        }
        expect(error).to.be.null;
      });
    });
  });

  describe('prepare no-callback', () => {
    it('Prepares valid SQL and sends it to the DBMS, if fail, error is returned. ', () => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql);
    });
  });

  describe('bindParams callback', () => {
    it('associate parameter markers in an SQL statement to app variables', () => {
      let sql = 'INSERT INTO QIWS.QCUSTCDT(CUSNUM,LSTNAM,INIT,STREET,CITY,STATE,ZIPCOD,CDTLMT,CHGCOD,BALDUE,CDTDUE) VALUES (?,?,?,?,?,?,?,?,?,?,?) with NONE ',
        dbConn = new dbconn(),
        dbConn2 = new dbconn(),
        dbStmt,
        dbStmt2;

      dbConn.conn('*LOCAL');
      dbConn2.conn('*LOCAL');

      dbStmt = new dbstmt(dbConn),
      dbStmt2 = new dbstmt(dbConn2);

      let params = [
        [9997, IN, NUMERIC], //CUSNUM
        ['Doe', IN, CHAR], //LASTNAME
        ['J D', IN, CHAR], //INITIAL
        ['123 Broadway', IN, CHAR], //ADDRESS
        ['Hope', IN, CHAR], //CITY
        ['WA', IN, CHAR], //STATE
        [98101, IN, NUMERIC], //ZIP
        [2000, IN, NUMERIC], //CREDIT LIMIT
        [1, IN, NUMERIC], // change
        [250, IN, NUMERIC], //BAL DUE
        [0.00, IN, NUMERIC] //CREDIT DUE
      ];

      let result = dbStmt.execSync('SELECT COUNT(CUSNUM) FROM QIWS.QCUSTCDT'),
        rowsBefore = result[0]['00001'];

      rowsBefore = Number(rowsBefore);
      dbStmt.close();

      dbStmt2.prepareSync(sql, (error) => {
        if (error){
          throw error;
        }
        dbStmt2.bindParamSync(params, (error) =>{
          if (error){
            throw error;
          }
          expect(error).to.be.null;

          dbStmt2.executeSync( (out, error) => {
            if (error){
              throw error;
            }
            dbStmt = new dbstmt(dbConn);

            let result2 = dbStmt.execSync('SELECT COUNT(CUSNUM) FROM QIWS.QCUSTCDT'),
              rowsAfter = result2[0]['00001'];

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
      let sql = 'INSERT INTO QIWS.QCUSTCDT(CUSNUM,LSTNAM,INIT,STREET,CITY,STATE,ZIPCOD,CDTLMT,CHGCOD,BALDUE,CDTDUE) VALUES (?,?,?,?,?,?,?,?,?,?,?) with NONE ',
        dbConn = new dbconn(),
        dbConn2 = new dbconn(),
        dbStmt,
        dbStmt2,
        params = [
          [9997,IN, NUMERIC], //CUSNUM
          ['Doe', IN, CHAR], //LASTNAME
          ['J D', IN, CHAR], //INITIAL
          ['123 Broadway', IN, CHAR], //ADDRESS
          ['Hope', IN, CHAR], //CITY
          ['WA', IN, CHAR], //STATE
          [98101, IN, NUMERIC], //ZIP
          [2000, IN, NUMERIC], //CREDIT LIMIT
          [1, IN, NUMERIC], // change
          [250, IN, NUMERIC], //BAL DUE
          [0.00, IN, NUMERIC] //CREDIT DUE
        ];

      dbConn.conn('*LOCAL');
      dbConn2.conn('*LOCAL');

      dbStmt = new dbstmt(dbConn),
      dbStmt2 = new dbstmt(dbConn2);
      // first get count of current rows
      let result = dbStmt.execSync('SELECT COUNT(CUSNUM) FROM QIWS.QCUSTCDT'),
        rowsBefore = result[0]['00001'];

      rowsBefore = Number(rowsBefore); //count retrurns as a String cast it to Number

      dbStmt.close();

      // now perform insert
      dbStmt2.prepareSync(sql);
      dbStmt2.bindParamSync(params);
      dbStmt2.executeSync();

      dbStmt = new dbstmt(dbConn);

      let result2 = dbStmt.execSync('SELECT COUNT(CUSNUM) FROM QIWS.QCUSTCDT'),
        rowsAfter = result2[0]['00001'];

      rowsAfter = Number(rowsAfter);

      expect(rowsAfter).to.equal((rowsBefore + 1));
    });
  });

  describe('exec callback', () => {
    it('performs action of given SQL String', () => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new dbstmt(dbConn);

      dbStmt.execSync(sql, (result, error) => {
        if (error){
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
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new dbstmt(dbConn);

      let result = dbStmt.execSync(sql);
      expect(result).to.be.an('array');
      expect(result.length).to.be.greaterThan(0);
      expect(result[0]).to.be.an('object');
    });
  });

  describe('execute callback', () => {
    it('retrieves output parameters from stored proc using executeSync with a callback', () => {
      let sql = 'call QXMLSERV.iPLUG512K(?,?,?,?)',
        dbConn = new dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new dbstmt(dbConn);

      let ipc = '*NA',
        ctl = '*here',
        xmlIn = '<xmlservice><sh>system "wrksbs"<\/sh><\/xmlservice>',
        xmlOut = '',
        params = [
          [ipc, IN, CHAR],
          [ctl, IN, CHAR],
          [xmlIn, IN, CLOB],
          [xmlOut, OUT, CLOB],
        ];

      dbConn.conn('*LOCAL');
      dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql, (error)=>{
        if (error){
          throw error;
        }
        dbStmt.bindParamSync(params, (error) => {
          if (error){
            throw error;
          }
          dbStmt.executeSync( (result, error) => {
            if (error){
              throw error;
            }
            expect(error).to.be.null;
            expect(result).to.be.a('array');
            expect(result.length).to.be.eq(1);
          });
        });
      });
    });


    it('executes prepared statement using executeSync with callback. Returns null because no output params are available', () => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT WHERE BALDUE > ?',
        dbConn = new dbconn(),
        dbStmt,
        params = [
          [10.00, IN, NUMERIC],
        ];

      dbConn.conn('*LOCAL');
      dbStmt = new dbstmt(dbConn);

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

    it('retrieves output parameters from stored procedure using executeSync without a callback:', () => {
      let sql = 'CALL QXMLSERV.iPLUG512K(?,?,?,?)',
        dbConn = new dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new dbstmt(dbConn);

      let ipc = '*NA',
        ctl = '*here',
        xmlIn = '<xmlservice><sh>system "wrksbs"<\/sh><\/xmlservice>',
        xmlOut = '',
        params = [
          [ipc, IN, CHAR],
          [ctl, IN, CHAR],
          [xmlIn, IN, CLOB],
          [xmlOut, OUT, CLOB],
        ];

      dbStmt.prepareSync(sql);
      dbStmt.bindParamSync(params);

      let out = dbStmt.executeSync();

      expect(out).to.be.a('array');
      expect(out.length).to.be.eq(1);
    });


    it('executes prepared statement using executeSync without callback. Returns null because no output params are available', () => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT WHERE BALDUE > ?',
        dbConn = new dbconn(),
        dbStmt,
        params = [
          [10.00, IN, NUMERIC],
        ];

      dbConn.conn('*LOCAL');
      dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql);
      dbStmt.bindParamSync(params);
      let out = dbStmt.executeSync();
      expect(out).to.be.null;
    });
  });


  describe('fetchAll callback', () => {
    it('retrieves results from execute function:', () => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql, (error) =>{
        if (error){
          throw error;
        }
        dbStmt.executeSync( (out, error) =>{
          if (error){
            throw error;
          }
          dbStmt.fetchAllSync( (result, error) =>{
            if (error){
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
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql);
      dbStmt.executeSync();
      let result = dbStmt.fetchAllSync();
      expect(result).to.be.a('array');
      expect(result.length).to.be.greaterThan(0);
    });
  });

  describe('fetch callback', () => {
    it('retrieves results from execute function:', () => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql, (error)=>{
        if (error){
          throw error;
        }
        dbStmt.executeSync( (out, error)=>{
          if (error){
            throw error;
          }
          dbStmt.fetchSync( (result, returnCode) =>{
            if (returnCode !== 0){ //SQL_SUCCESS
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
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new dbstmt(dbConn);

      dbStmt.prepareSync(sql);
      dbStmt.executeSync();
      let result = dbStmt.fetchSync();
      expect(result).to.be.a('object');
    });
  });

});
