const assert = require('chai').assert;
const expect = require('chai').expect;
const addon = require('bindings')('db2ia');
const db2a = require('../lib/db2a');
const util = require('util');

describe('prepare sync version', () => {
  it('Prepares valid SQL and sends it to the DBMS, if the input SQL Statement cannot be prepared error is returned. ', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn(),
      dbStmt;

    dbConn.conn('*LOCAL');
    dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepareSync(sql, (result) =>{
      console.log(`Prepare result is: ${result}`);
      console.log(`TypeOf Prepare result is: ${typeof result}`);
      expect(result).to.be.a('null');
    });
  });
});

describe('bindParams sync version', () => {
  it('associate parameter markers in an SQL statement to app variables', () => {
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

    dbStmt.prepareSync('SELECT * FROM QIWS.QCUSTCDT', ()=> {
      dbStmt.executeSync( ()=> {
        dbStmt.fetchAllSync((result)=>{
          let rowsBefore = result.length;

          console.log(`NumFields Before is: ${rowsBefore}`);
          dbStmt.close();
          dbStmt2.prepareSync(sql, () => {
            console.log(`In Prepare #2`);
            dbStmt2.bindParamSync([
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
              console.log(`In Bind`);
              dbStmt2.executeSync( ()=>{
                console.log(`In Execute #2`);
                let rowsAfter = dbStmt2.numRows();
                console.log(`Number Rows Affected is: ${rowsAfter}`);
                dbStmt2.commit();
                dbStmt = new addon.dbstmt(dbConn),
                dbStmt.prepareSync('SELECT * FROM QIWS.QCUSTCDT', ()=> {
                  dbStmt.executeSync( ()=> {
                    dbStmt.fetchAllSync((result)=>{
                      let rowsFinal = result.length;
                      console.log(`NumFields Before is: ${rowsBefore}`);
                      console.log(`NumFields Final is: ${rowsFinal}`);
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

describe('exec sync version', () => {
  it('performs action of given SQL String', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn(),
      dbStmt;

    dbConn.debug(true);
    dbConn.conn('*LOCAL');
    dbStmt = new addon.dbstmt(dbConn);

    dbStmt.execSync(sql, (err, result) => {
      console.log(`Exec Sync results:\n ${JSON.stringify(result)}\n`);
      expect(result).to.be.an('array');
      expect(result.length).to.be.greaterThan(0);
    });
  });
});

//if successful returns an array of length 0?. Why,even return it if size === 0?
describe('execute sync version', () => {
  it('retrieves results from execute function:', () => {
    let sql = 'CALL AMUSSE.MAXBAL(?)',
      dbConn = new addon.dbconn(),
      dbStmt,
      bal = 0;

    dbConn.conn('*LOCAL');
    dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepareSync(sql, ()=>{
      dbStmt.bindParamSync([[bal, db2a.SQL_PARAM_OUT, db2a.SQL_NUMERIC]], ()=>{
        dbStmt.executeSync( (err, result) =>{
          console.log('Error is ' + JSON.stringify(err));
          console.log('TypeOf error:' + typeof (err));
          console.log('ExecuteSync results: ' + JSON.stringify(result));
          console.log('TypeOf ExecuteSync results:' + typeof (result));
          console.log('Length of results:' + result.length);
          expect(result).to.be.a('array');
          expect(result.length).to.be.greaterThan(0);
        });
      });
    });
  });
});

describe('fetchAll sync version', () => {
  it('retrieves results from execute function:', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn(),
      dbStmt;

    dbConn.conn('*LOCAL');
    dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepareSync(sql, () =>{
      dbStmt.executeSync( () =>{
        dbStmt.fetchAllSync( (result) =>{
          console.log(`FetchAll Sync results:\n ${JSON.stringify(result)}\n`);
          expect(result).to.be.a('array');
          expect(result.length).to.be.greaterThan(0);
        });
      });
    });
  });
});

describe('fetch sync version', () => {
  it('retrieves results from execute function:', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn(),
      dbStmt;

    dbConn.conn('*LOCAL');
    dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepareSync(sql, ()=>{
      dbStmt.executeSync( ()=>{
        dbStmt.fetchSync( (result) =>{
          console.log(`Fetch results:\n ${JSON.stringify(result)}`);
          expect(result).to.be.a('object');
        });
      });
    });
  });
});

