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

    dbStmt.prepare(sql, (error, result) =>{
      console.log(`Error is: ${error}`);
      if (error){
        console.log(`Type of Error is ${typeof error}`);
        console.log(util.inspect(error));
        throw error;
      }
      console.log(`Prepare result is: ${result}`);
      console.log(`TypeOf Prepare result is: ${typeof result}`);
      expect(result).to.be.a('undefined');
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

    // dbConn.debug(true);
    // dbConn2.debug(true);

    dbConn.conn('*LOCAL');
    dbConn2.conn('*LOCAL');

    dbStmt = new addon.dbstmt(dbConn),
    dbStmt2 = new addon.dbstmt(dbConn2);

    dbStmt.prepare('SELECT * FROM QIWS.QCUSTCDT', ()=> {
      dbStmt.execute( ()=> {
        dbStmt.fetchAll((result)=>{
          let rowsBefore = result.length;

          console.log(`NumFields Before is: ${rowsBefore}`);
          dbStmt.close();

          dbStmt2.prepare(sql, () => {
            dbStmt2.bindParam([
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
            ], () =>{

              dbStmt2.execute( ()=>{
                let rowsAfter = dbStmt2.numRows();
                console.log(`Number Rows Affected is: ${rowsAfter}`);
                dbStmt2.commit();
                dbStmt = new addon.dbstmt(dbConn),
                dbStmt.prepare('SELECT * FROM QIWS.QCUSTCDT', ()=> {
                  dbStmt.execute( ()=> {
                    dbStmt.fetchAll((result)=>{
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

//if successful returns an array of length 0?. Why,even return it if size === 0?
describe('execute async version', () => {
  it('retrieves results from execute function:', (done) => {
    let sql = 'CALL AMUSSE.MAXBAL(?)',
      dbConn = new addon.dbconn(),
      dbStmt,
      bal = 0;

    dbConn.debug(true);
    dbConn.conn('*LOCAL');
    dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepare(sql, ()=>{
      dbStmt.bindParam([[bal, db2a.SQL_PARAM_OUT, db2a.SQL_NUMERIC]], ()=>{
        console.log('completed the bindParam');
        dbStmt.execute( (result, err) =>{
          console.log(`Error is JSON.stringify(err)`);
          console.log('TypeOf error:' + typeof (err));
          console.log(`ExecuteAsync results:\n ${JSON.stringify(result)}`);
          console.log(`TypeOf ExecuteAsync results: ${typeof (result)}`);
          console.log(`Length of results: ${result.length}`);
          expect(result).to.be.a('array');
          expect(result.length).to.be.greaterThan(0);
          done();
        });
      });
    });
  });
});


//if successful returns an array. of Type of objects
describe.only('exec async version', () => {
  it('performs action of given SQL String', (done) => {
    let sql = 'DELETE FROM AMUSSE.TABLE1 WHERE COLUMN1 = 2018',
      dbConn = new addon.dbconn();
    dbConn.debug(true);
    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.exec(sql, (result, error) => {
      console.log(`Error is: ${error}`);
      if (error){
        console.log(`Type of Error is ${typeof error}`);
        console.log(util.inspect(error));
        throw error;
      }
      console.log(`Exec Async results:\n ${JSON.stringify(result)}\n`);
      expect(result).to.be.an('array');
      expect(result.length).to.be.greaterThan(0);
      done();
    });
  });
});

//if successful returns an array. of Type of objects
describe('fetchAll async version', () => {
  it('retrieves all rows from execute function:', (done) => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepare(sql, () =>{
      dbStmt.execute( () =>{
        dbStmt.fetchAll( (result) =>{
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
//kind of weird because FetchAll returns an Array(of objects? )
describe('fetch async version', () => {
  it('retrieves one row from execute function:', (done) => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepare(sql, ()=>{
      dbStmt.execute( ()=>{
        dbStmt.fetch( (result, err) =>{
          console.log(`Fetch Async results:\n ${JSON.stringify(result)}`);
          expect(result).to.be.a('object');
          done();
        });
      });
    });
  });
});
