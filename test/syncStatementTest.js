const assert = require('chai').assert;
const expect = require('chai').expect;
const addon = require('bindings')('db2ia');
const db2a = require('../lib/db2a');
const util = require('util');

describe('Statement Sync Test', () => {
  describe('prepare callback', () => {
    it('Prepares valid SQL and sends it to the DBMS, if fail, error is returned. ', () => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new addon.dbstmt(dbConn);

      dbStmt.prepareSync(sql, (error) =>{
        if (error){
          // console.log(`Error is: ${result}`);
          throw error;
        }
        expect(error).to.be.null;
      });
    });
  });

  describe('prepare no-callback', () => {
    it('Prepares valid SQL and sends it to the DBMS, if fail, error is returned. ', () => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new addon.dbstmt(dbConn);

      dbStmt.prepareSync(sql);
    });
  });

  describe('bindParams callback', () => {
    it('associate parameter markers in an SQL statement to app variables', () => {
      let sql = 'INSERT INTO QIWS.QCUSTCDT(CUSNUM,LSTNAM,INIT,STREET,CITY,STATE,ZIPCOD,CDTLMT,CHGCOD,BALDUE,CDTDUE) VALUES (?,?,?,?,?,?,?,?,?,?,?) with NONE ',
        dbConn = new addon.dbconn(),
        dbConn2 = new addon.dbconn(),
        dbStmt,
        dbStmt2;

      // dbConn.debug(true);
      // dbConn2.debug(true);

      dbConn.conn('*LOCAL');
      dbConn2.conn('*LOCAL');

      dbStmt = new addon.dbstmt(dbConn),
      dbStmt2 = new addon.dbstmt(dbConn2);

      let params = [
        [9997, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC], //CUSNUM
        ['Doe', db2a.SQL_PARAM_INPUT, db2a.SQL_CHAR], //LASTNAME
        ['J D', db2a.SQL_PARAM_INPUT, db2a.SQL_CHAR], //INITIAL
        ['123 Broadway', db2a.SQL_PARAM_INPUT, db2a.SQL_CHAR], //ADDRESS
        ['Hope', db2a.SQL_PARAM_INPUT, db2a.SQL_CHAR], //CITY
        ['WA', db2a.SQL_PARAM_INPUT, db2a.SQL_CHAR], //STATE
        [98101, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC], //ZIP
        [2000, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC], //CREDIT LIMIT
        [1, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC], // change
        [250, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC], //BAL DUE
        [0.00, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC] //CREDIT DUE
      ];

      dbStmt.prepareSync('SELECT * FROM QIWS.QCUSTCDT', (error)=> {
        if (error){
          throw error;
        }
        dbStmt.executeSync( (out, error) => {
          if (error){
            throw error;
          }
          dbStmt.fetchAllSync((result, error) =>{
            if (error){
              throw error;
            }
            let rowsBefore = result.length;
            dbStmt.close();
            dbStmt2.prepareSync(sql, (error) => {
              if (error){
                throw error;
              }
              dbStmt2.bindParamSync(params, (error) =>{
                if (error){
                  console.log(util.inspect(error));
                  throw error;
                }
                expect(error).to.be.null;
                dbStmt2.executeSync( (out, error) => {
                  if (error){
                    throw error;
                  }
                  let change = dbStmt2.numRows();
                  dbStmt = new addon.dbstmt(dbConn),
                  dbStmt.prepareSync('SELECT * FROM QIWS.QCUSTCDT', (error) => {
                    if (error){
                      throw error;
                    }
                    dbStmt.executeSync( (out, error) => {
                      if (error){
                        throw error;
                      }
                      dbStmt.fetchAllSync((result, error) => {
                        if (error){
                          throw error;
                        }
                        let rowsFinal = result.length;
                        expect(rowsFinal).to.equal(rowsBefore + 1);
                      });
                    });
                  });
                });
              });
            });
          });
        });
      });
    });
  });

  describe('bindParams no-callback', () => {
    it('associate parameter markers in an SQL statement to app variables', () => {
      let sql = 'INSERT INTO QIWS.QCUSTCDT(CUSNUM,LSTNAM,INIT,STREET,CITY,STATE,ZIPCOD,CDTLMT,CHGCOD,BALDUE,CDTDUE) VALUES (?,?,?,?,?,?,?,?,?,?,?) with NONE ',
        dbConn = new addon.dbconn(),
        dbConn2 = new addon.dbconn(),
        dbStmt,
        dbStmt2;

      // dbConn.debug(true);
      // dbConn2.debug(true);

      dbConn.conn('*LOCAL');
      dbConn2.conn('*LOCAL');

      dbStmt = new addon.dbstmt(dbConn),
      dbStmt2 = new addon.dbstmt(dbConn2);

      let params = [
        [9997, addon.SQL_PARAM_INPUT, db2a.SQL_NUMERIC], //CUSNUM
        ['Doe', db2a.SQL_PARAM_INPUT, db2a.SQL_CHAR], //LASTNAME
        ['J D', db2a.SQL_PARAM_INPUT, db2a.SQL_CHAR], //INITIAL
        ['123 Broadway', db2a.SQL_PARAM_INPUT, db2a.SQL_CHAR], //ADDRESS
        ['Hope', db2a.SQL_PARAM_INPUT, db2a.SQL_CHAR], //CITY
        ['WA', db2a.SQL_PARAM_INPUT, db2a.SQL_CHAR], //STATE
        [98101, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC], //ZIP
        [2000, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC], //CREDIT LIMIT
        [1, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC], // change
        [250, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC], //BAL DUE
        [0.00, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC] //CREDIT DUE
      ];

      dbStmt.prepareSync('SELECT * FROM QIWS.QCUSTCDT');
      dbStmt.executeSync();
      let result = dbStmt.fetchAllSync();
      let rowsBefore = result.length;
      dbStmt.close();

      dbStmt2.prepareSync(sql);
      dbStmt2.bindParamSync(params);
      dbStmt2.executeSync();
      let change = dbStmt2.numRows();
      
      dbStmt = new addon.dbstmt(dbConn),
      dbStmt.prepareSync('SELECT * FROM QIWS.QCUSTCDT');
      dbStmt.executeSync();
      let result2 = dbStmt.fetchAllSync();
      let rowsFinal = result2.length;
      expect(rowsFinal).to.equal(rowsBefore + 1);
    });
  });

  describe('exec callback', () => {
    it('performs action of given SQL String', () => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn(),
        dbStmt;

      // dbConn.debug(true);
      dbConn.conn('*LOCAL');
      dbStmt = new addon.dbstmt(dbConn);

      dbStmt.execSync(sql, (result, error) => {
        if (error){
          console.log(util.inspect(error));
          throw error;
        }
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
      });
    });
  });

  describe('exec no-callback', () => {
    it('performs action of given SQL String', () => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn(),
        dbStmt;

      // dbConn.debug(true);
      dbConn.conn('*LOCAL');
      dbStmt = new addon.dbstmt(dbConn);

      let result = dbStmt.execSync(sql);
      expect(result).to.be.an('array');
      expect(result.length).to.be.greaterThan(0);
    });
  });

  //if successful returns an array of length 0?. Why,even return it if size === 0?
  describe('execute callback', () => {
    before( () => {
      let user = (process.env.USER).toUpperCase(),
        sql = `CREATE SCHEMA ${user}`,
        sql2 = `CREATE OR REPLACE PROCEDURE ${user}.BALMAX(OUT OUTPUT NUMERIC( 6,2 ))
                    LANGUAGE SQL
                    BEGIN
                      DECLARE MAXBAL NUMERIC ( 6 , 2 );
                      SELECT MAX ( BALDUE ) INTO MAXBAL FROM QIWS . QCUSTCDT;
                      SET OUTPUT = MAXBAL;
                    END`,
        dbConn = new addon.dbconn(),
        dbStmt;

        // dbConn.debug(true);
      dbConn.conn('*LOCAL');
      dbStmt = new addon.dbstmt(dbConn);

      dbStmt.execSync(sql, (result, error) => {
        if (error){
          //if Schema already exsists will error but ignore
          // console.log(util.inspect(error));
        }
        dbStmt.execSync(sql2, (result, error) => {
          if (error){
            console.log(util.inspect(error));
            throw error;
          }
        });
      });
    });

    it('retrieves results from execute function:', () => {
      let user = (process.env.USER).toUpperCase(),
        sql = `CALL ${user}.BALMAX(?)`,
        dbConn = new addon.dbconn(),
        dbStmt,
        bal = 0;

      dbConn.conn('*LOCAL');
      dbStmt = new addon.dbstmt(dbConn);

      dbStmt.prepareSync(sql, (error)=>{
        if (error){
          throw error;
        }
        dbStmt.bindParamSync([[bal, db2a.SQL_PARAM_OUT, db2a.SQL_NUMERIC]], () => {
          if (error){
            throw error;
          }
          dbStmt.executeSync( (result, error) => {
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

  describe('execute no-callback', () => {
    before( () => {
      let user = (process.env.USER).toUpperCase(),
        sql = `CREATE SCHEMA ${user}`,
        sql2 = `CREATE OR REPLACE PROCEDURE ${user}.BALMAX(OUT OUTPUT NUMERIC( 6,2 ))
                    LANGUAGE SQL
                    BEGIN
                      DECLARE MAXBAL NUMERIC ( 6 , 2 );
                      SELECT MAX ( BALDUE ) INTO MAXBAL FROM QIWS . QCUSTCDT;
                      SET OUTPUT = MAXBAL;
                    END`,
        dbConn = new addon.dbconn(),
        dbStmt;

        // dbConn.debug(true);
      dbConn.conn('*LOCAL');
      dbStmt = new addon.dbstmt(dbConn);

      dbStmt.execSync(sql, function(result, error){
        if (error){
          //if Schema already exsists will error but ignore
          // console.log(util.inspect(error));
        }

        dbStmt.execSync(sql2, function(result, error){
          if (error){
            console.log(util.inspect(error));
            throw error;
          }
        });
      });
    });

    it('retrieves results from execute function:', () => {
      let user = (process.env.USER).toUpperCase(),
        sql = `CALL ${user}.BALMAX(?)`,
        dbConn = new addon.dbconn(),
        dbStmt,
        bal = 0;

      dbConn.conn('*LOCAL');
      dbStmt = new addon.dbstmt(dbConn);

      dbStmt.prepareSync(sql);
      dbStmt.bindParamSync([[bal, db2a.SQL_PARAM_OUT, db2a.SQL_NUMERIC]]);
      let result = dbStmt.executeSync();

      expect(result).to.be.a('array');
      expect(result.length).to.be.greaterThan(0);
    });
  });


  describe('fetchAll callback', () => {
    it('retrieves results from execute function:', () => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new addon.dbstmt(dbConn);

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
        dbConn = new addon.dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new addon.dbstmt(dbConn);

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
        dbConn = new addon.dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new addon.dbstmt(dbConn);

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
        dbConn = new addon.dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new addon.dbstmt(dbConn);

      dbStmt.prepareSync(sql);
      dbStmt.executeSync();
      let result = dbStmt.fetchSync();
      expect(result).to.be.a('object');
    });
  });

});