const assert = require('chai').assert;
const expect = require('chai').expect;
const addon = require('bindings')('db2ia');
const db2a = require('../lib/db2a');
const util = require('util');

//Test Statement Class Async Methods

// if successful returns undefined
describe('prepare async version', () => {
  it('Prepares valid SQL and sends it to the DBMS, if the input SQL Statement cannot be prepared error is returned. ', (done) => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn(),
      dbStmt;

    dbConn.conn('*LOCAL');
    dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepare(sql, (error) =>{
      console.log(`Error is: ${error}`);
      if (error){
        console.log(`Type of Error is ${typeof error}`);
        console.log(util.inspect(error));
        throw error;
      }
      expect(error).to.be.null;
      done();
    });
  });
});

describe('bindParams async version', () => {
  it('associate parameter markers in an SQL statement to app variables', (done) => {
    let sql = 'INSERT INTO QIWS.QCUSTCDT(CUSNUM,LSTNAM,INIT,STREET,CITY,STATE,ZIPCOD,CDTLMT,CHGCOD,BALDUE,CDTDUE) VALUES (?,?,?,?,?,?,?,?,?,?,?) with NONE ',
      dbConn = new addon.dbconn(),
      dbConn2 = new addon.dbconn(),
      dbStmt,
      dbStmt2;

    dbConn.debug(true);
    dbConn2.debug(true);

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

    dbStmt.prepare('SELECT * FROM QIWS.QCUSTCDT', ()=> {
      dbStmt.execute( ()=> {
        dbStmt.fetchAll((result)=>{
          let rowsBefore = result.length;

          console.log(`NumFields Before is: ${rowsBefore}`);
          dbStmt.close();

          dbStmt2.prepare(sql, (error) => {
            if (error){
              throw error;
            }
            dbStmt2.bindParam(params, (error) =>{
              if (error){
                console.log(util.inspect(error));
                throw error;
              }
              expect(error).to.be.null;
              dbStmt2.execute( (out, error)=>{
                if (error){
                  throw error;
                }
                let change = dbStmt2.numRows();
                console.log(`Number Rows Affected is: ${change}`);
                dbStmt = new addon.dbstmt(dbConn),
                dbStmt.prepare('SELECT * FROM QIWS.QCUSTCDT', (error)=> {
                  if (error){
                    throw error;
                  }
                  dbStmt.execute( (out, error)=> {
                    if (error){
                      throw error;
                    }
                    dbStmt.fetchAll((result, error)=>{
                      if (error){
                        throw error;
                      }
                      let rowsFinal = result.length;
                      console.log(`NumFields Before is: ${rowsBefore}`);
                      console.log(`NumFields Final is: ${rowsFinal}`);
                      expect(rowsFinal).to.equal(rowsBefore + 1);
                      done();
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

//if output paramters are avilable will return it.
describe('execute async version', () => {
  it('runs SQLExecute and retrieves output params from SP if available', (done) => {
    let sql = 'CALL AMUSSE.MAXBAL(?)',
      dbConn = new addon.dbconn(),
      dbStmt,
      bal = 0;

    dbConn.debug(true);
    dbConn.conn('*LOCAL');
    dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepare(sql, (error)=>{
      if (error){
        throw error;
      }
      dbStmt.bindParam([[bal, db2a.SQL_PARAM_OUT, db2a.SQL_NUMERIC]], (error)=>{
        if (error){
          throw error;
        }
        console.log('completed the bindParam');
        dbStmt.execute( (result, error) =>{
          if (error){
            console.log(util.inspect(error));
            throw error;
          }
          expect(error).to.be.null;
          console.log(`ExecuteAsync results:\n ${JSON.stringify(result)}`);
          expect(result).to.be.a('array');
          expect(result.length).to.be.greaterThan(0);
          done();
        });
      });
    });
  });
});


//if result set is available returns an array of objects
describe('exec async version', () => {
  it('performs action of given SQL String', (done) => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();
    dbConn.debug(true);
    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.exec(sql, (result, error) => {
      console.log(`Error is: ${error}`);
      if (error){
        console.log(util.inspect(error));
        throw error;
      }
      expect(error).to.be.null;
      console.log(`Exec Async results:\n ${JSON.stringify(result)}\n`);
      expect(result).to.be.an('array');
      expect(result.length).to.be.greaterThan(0);
      done();
    });
  });
});

//if successful returns an array of objects
describe('fetchAll async version', () => {
  it('retrieves all rows from execute function:', (done) => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepare(sql, (error) =>{
      if (error){
        throw error;
      }
      dbStmt.execute( (out, error) =>{
        if (error){
          throw error;
        }
        dbStmt.fetchAll( (result, error) =>{
          if (error){
            console.log(util.inspect(error));
            throw error;
          }
          expect(error).to.be.null;
          console.log(`FetchAll Async results:\n ${JSON.stringify(result)}\n`);
          expect(result).to.be.a('array');
          expect(result.length).to.be.greaterThan(0);
          done();
        });
      });
    });
  });
});


//if successful returns an Object of Row
describe('fetch async version', () => {
  it('retrieves one row from execute function:', (done) => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepare(sql, (error)=>{
      if (error){
        throw error;
      }
      dbStmt.execute( (out, error)=>{
        if (error){
          throw error;
        }
        dbStmt.fetch( (row, returnCode) =>{
          console.log(`Return Code is: ${returnCode}`);
          if (returnCode !== 0){ //SQL_SUCCESS
            throw new Error('Rreturn Code was Not SQL SUCESS');
          }
          expect(returnCode).to.equal(0);
          console.log(`Fetch Async results:\n ${JSON.stringify(row)}`);
          expect(row).to.be.a('object');
          done();
        });
      });
    });
  });
});
