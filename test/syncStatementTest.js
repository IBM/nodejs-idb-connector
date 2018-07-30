const assert = require('chai').assert;
const expect = require('chai').expect;
const addon = require('bindings')('db2ia');
const db2a = require('../lib/db2a');
const util = require('util');

describe('prepare sync version callback', () => {
  it('Prepares valid SQL and sends it to the DBMS, if the input SQL Statement cannot be prepared error is returned. ', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn(),
      dbStmt;

    dbConn.conn('*LOCAL');
    dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepareSync(sql, (error) =>{
      if (error){
        console.log(`Error is: ${result}`);
        throw error;
      }
      expect(error).to.be.null;
    });
  });
});

describe('prepare sync version no-callback', () => {
  it('Prepares valid SQL and sends it to the DBMS, if the input SQL Statement cannot be prepared error is returned. ', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn(),
      dbStmt;

    dbConn.conn('*LOCAL');
    dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepareSync(sql);
  });
});

describe('bindParams sync callback version', () => {
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
      dbStmt.executeSync( (out, error)=> {
        if (error){
          throw error;
        }
        dbStmt.fetchAllSync((result, error)=>{
          if (error){
            throw error;
          }
          let rowsBefore = result.length;
          console.log(`NumFields Before is: ${rowsBefore}`);
          dbStmt.close();
          dbStmt2.prepareSync(sql, (error) => {
            if (error){
              throw error;
            }
            console.log(`In Prepare #2`);
            dbStmt2.bindParamSync(params, (error) =>{
              if (error){
                console.log(util.inspect(error));
                throw error;
              }
              expect(error).to.be.null;
              console.log(`In Bind`);
              dbStmt2.executeSync( (out, error)=>{
                if (error){
                  throw error;
                }
                console.log(`In Execute #2`);
                let change = dbStmt2.numRows();
                console.log(`Number Rows Affected is: ${change}`);
                // dbStmt2.commit();
                dbStmt = new addon.dbstmt(dbConn),
                dbStmt.prepareSync('SELECT * FROM QIWS.QCUSTCDT', (error)=> {
                  if (error){
                    throw error;
                  }
                  dbStmt.executeSync( (out, error)=> {
                    if (error){
                      throw error;
                    }
                    dbStmt.fetchAllSync((result, error)=>{
                      if (error){
                        throw error;
                      }
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

describe('bindParams sync no-callback version', () => {
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
    console.log(`NumFields Before is: ${rowsBefore}`);
    dbStmt.close();

    dbStmt2.prepareSync(sql);
    console.log(`In Prepare #2`);
    dbStmt2.bindParamSync(params);
    console.log(`In Bind`);
    dbStmt2.executeSync();
    console.log(`In Execute #2`);
    let change = dbStmt2.numRows();
    console.log(`Number Rows Affected is: ${change}`);
    // dbStmt2.commit();

    dbStmt = new addon.dbstmt(dbConn),
    dbStmt.prepareSync('SELECT * FROM QIWS.QCUSTCDT');
    dbStmt.executeSync();
    let result2 = dbStmt.fetchAllSync();
    let rowsFinal = result2.length;
    console.log(`NumFields Before is: ${rowsBefore}`);
    console.log(`NumFields Final is: ${rowsFinal}`);
    expect(rowsFinal).to.equal(rowsBefore + 1);
  });
});

describe('exec sync version callback', () => {
  it('performs action of given SQL String', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn(),
      dbStmt;

    dbConn.debug(true);
    dbConn.conn('*LOCAL');
    dbStmt = new addon.dbstmt(dbConn);

    dbStmt.execSync(sql, (result, error) => {
      if (error){
        console.log(util.inspect(error));
        throw error;
      }
      expect(error).to.be.null;
      console.log(`Exec Sync results:\n ${JSON.stringify(result)}\n`);
      expect(result).to.be.an('array');
      expect(result.length).to.be.greaterThan(0);
    });
  });
});

describe('exec sync version no-callback', () => {
  it('performs action of given SQL String', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn(),
      dbStmt;

    dbConn.debug(true);
    dbConn.conn('*LOCAL');
    dbStmt = new addon.dbstmt(dbConn);

    let result = dbStmt.execSync(sql);
    console.log(`Exec Sync results:\n ${JSON.stringify(result)}\n`);
    expect(result).to.be.an('array');
    expect(result.length).to.be.greaterThan(0);
  });
});

//if successful returns an array of length 0?. Why,even return it if size === 0?
describe('execute sync version callback', () => {
  before(function() {
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
          console.log(`ExecuteSync results: ${JSON.stringify(result)}`);
          expect(result).to.be.a('array');
          expect(result.length).to.be.greaterThan(0);
        });
      });
    });
  });
});

describe('execute sync version no-callback', () => {
  before(function() {
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

    console.log(`ExecuteSync results: ${JSON.stringify(result)}`);
    expect(result).to.be.a('array');
    expect(result.length).to.be.greaterThan(0);
  });
});


describe('fetchAll sync callback version', () => {
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
          console.log(`FetchAll Sync results:\n ${JSON.stringify(result)}\n`);
          expect(result).to.be.a('array');
          expect(result.length).to.be.greaterThan(0);
        });
      });
    });
  });
});

describe('fetchAll sync no-callback version', () => {
  it('retrieves results from execute function:', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn(),
      dbStmt;

    dbConn.conn('*LOCAL');
    dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepareSync(sql);
    dbStmt.executeSync();
    let result = dbStmt.fetchAllSync();
    console.log(`FetchAll Sync results:\n ${JSON.stringify(result)}\n`);
    expect(result).to.be.a('array');
    expect(result.length).to.be.greaterThan(0);
  });
});

describe('fetch sync callback version', () => {
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
          console.log(`Fetch results:\n ${JSON.stringify(result)}`);
          expect(result).to.be.a('object');
        });
      });
    });
  });
});

describe('fetch sync no-callback version', () => {
  it('retrieves results from execute function:', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn(),
      dbStmt;

    dbConn.conn('*LOCAL');
    dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepareSync(sql);
    dbStmt.executeSync();
    let result = dbStmt.fetchSync();
    console.log(`Fetch results:\n ${JSON.stringify(result)}`);
    expect(result).to.be.a('object');
  });
});

