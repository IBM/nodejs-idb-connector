const assert = require('chai').assert;
const expect = require('chai').expect;
const addon = require('bindings')('db2ia');
const db2a = require('../lib/db2a');
const util = require('util');
const fs = require('fs');

//Test Statement Class Async Methods
describe('Statement Async Test', () => {
  
  describe('prepare', () => {
    it('Prepares valid SQL and sends it to the DBMS, if fail, error is returned. ', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn(),
        dbStmt;

      dbConn.conn('*LOCAL');
      dbStmt = new addon.dbstmt(dbConn);

      dbStmt.prepare(sql, (error) =>{
        if (error){
          console.log(util.inspect(error));
          throw error;
        }
        expect(error).to.be.null;
        done();
      });
    });
  });

  describe('bindParams', () => {
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

  describe('bind parameters blob/binary/varbinary', () => {
    before( () => {
      let user = (process.env.USER).toUpperCase(),
        sql = `CREATE SCHEMA ${user}`,
        sql2 = `CREATE OR REPLACE TABLE ${user}.BLOBTEST(BLOB_COLUMN BLOB(512k))`,
        sql3 = `CREATE OR REPLACE TABLE ${user}.BINARYTEST(BINARY_COLUMN BINARY(3000))`,
        sql4 = `CREATE OR REPLACE TABLE ${user}.VARBINARYTEST(VARBINARY_COLUMN VARBINARY(3000))`,
        dbConn = new addon.dbconn(),
        dbStmt;

      // dbConn.debug(true);
      dbConn.conn('*LOCAL');
      dbStmt = new addon.dbstmt(dbConn);
      
      dbStmt.exec(sql, (result, error) => {
        //if Schema already exsists will error but ignore
        dbStmt.closeCursor();
        dbStmt.exec(sql2, (result, error) => {
          dbStmt.closeCursor();
          dbStmt.exec(sql3, (result, error) => {
            dbStmt.closeCursor();
            dbStmt.exec(sql4, (result, error) => {});
          });
        });
      });
    });

    it('runs SQLExecute and to bind blob', (done) => {

      let user = (process.env.USER).toUpperCase(),
        // Table which only contains one BLOB(512k) Field
        sql = `INSERT INTO ${user}.BLOBTEST(BLOB_COLUMN) VALUES(?)`,
        dbConn = new addon.dbconn(),
        dbStmt;

      fs.readFile(`${__dirname}/../README.md`, (error, buffer) => {
        if (error){
          throw error;
        }
        // console.log(util.inspect(buffer));

        // dbConn.debug(true);
        dbConn.conn('*LOCAL');
        dbStmt = new addon.dbstmt(dbConn);

        dbStmt.prepare(sql, (error)=>{
          if (error){
            throw error;
          }
          dbStmt.bindParam([[buffer, db2a.SQL_PARAM_INPUT, db2a.SQL_BLOB]], (error)=>{
            if (error){
              throw error;
            }
            dbStmt.execute( (result, error) =>{
              if (error){
                console.log(util.inspect(error));
                throw error;
              }
              expect(error).to.be.null;
              done();
            });
          });
        });
      });
    });
    
    it('runs SQLExecute and to bind binary', (done) => {
      
      let user = (process.env.USER).toUpperCase(),
        // Table which only contains one BLOB(10) Field
        sql = `INSERT INTO ${user}.BINARYTEST(BINARY_COLUMN) VALUES(?)`,
        dbConn = new addon.dbconn(),
        dbStmt;

      fs.readFile(`${__dirname}/../README.md`, (error, buffer) => {
        if (error){
          throw error;
        }
        // console.log(util.inspect(buffer));
        // dbConn.debug(true);
        dbConn.conn('*LOCAL');
        dbStmt = new addon.dbstmt(dbConn);

        dbStmt.prepare(sql, (error)=>{
          if (error){
            throw error;
          }
          dbStmt.bindParam([[buffer, db2a.SQL_PARAM_INPUT, db2a.SQL_BINARY]], (error)=>{
            if (error){
              throw error;
            }
            dbStmt.execute( (result, error) =>{
              if (error){
                // console.log(util.inspect(error));
                throw error;
              }
              expect(error).to.be.null;
              done();
            });
          });
        });
      });
    });
    
    it('runs SQLExecute and to bind varbinary', (done) => {
      let user = (process.env.USER).toUpperCase(),
        // Table which only contains one VARBINARY(10) Field
        sql = `INSERT INTO ${user}.VARBINARYTEST(VARBINARY_COLUMN) VALUES(?)`,
        dbConn = new addon.dbconn(),
        dbStmt;

      fs.readFile(`${__dirname}/../README.md`, (error, buffer) => {
        if (error){
          throw error;
        }
        // dbConn.debug(true);
        dbConn.conn('*LOCAL');
        dbStmt = new addon.dbstmt(dbConn);

        dbStmt.prepare(sql, (error)=>{
          if (error){
            throw error;
          }
          dbStmt.bindParam([[buffer, db2a.SQL_PARAM_INPUT, db2a.SQL_BINARY]], (error)=>{
            if (error){
              throw error;
            }
            dbStmt.execute( (result, error) =>{
              if (error){
                console.log(util.inspect(error));
                throw error;
              }
              expect(error).to.be.null;
              done();
            });
          });
        });
      });
    });
    
  });



  describe('execute', () => {
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

      dbStmt.exec(sql, (result, error) => {
        //if Schema already exsists will error but ignore
        dbStmt.closeCursor();
        dbStmt.exec(sql2, (result, error) => {
          if (error){
            console.log(util.inspect(error));
            throw error;
          }
        });
      });
    });

    it('runs SQLExecute and retrieves output params from SP if available', (done) => {
      //SP which returns as an outparam the highest bal from QIWS.QCUSTCDT
      let user = (process.env.USER).toUpperCase(),
        sql = `CALL ${user}.BALMAX(?)`,
        dbConn = new addon.dbconn(),
        dbStmt,
        bal = 0;

      // dbConn.debug(true);
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
          dbStmt.execute( (result, error) =>{
            if (error){
              console.log(util.inspect(error));
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

  describe('execute xmlservice call', () => {
    it('tests xml service SP', (done) => {
      let sql = 'call QXMLSERV.iPLUG512K(?,?,?,?)',
        dbConn = new addon.dbconn(),
        dbStmt,
        bal = 0;

      // dbConn.debug(true);
      dbConn.conn('*LOCAL');
      dbStmt = new addon.dbstmt(dbConn);

      let ipc = '*NA',
        ctl = '*here',
        xmlIn = '<xmlservice><sh>system "wrksbs"<\/sh><\/xmlservice>',
        xmlOut = '';

      // console.log(xmlIn);

      let params = [
        [ipc, db2a.SQL_PARAM_INPUT, 1],
        [ctl, db2a.SQL_PARAM_INPUT, 1],
        [xmlIn, db2a.SQL_PARAM_INPUT, 0],
        [xmlOut, db2a.SQL_PARAM_OUTPUT, 0],
      ];

      dbStmt.prepare(sql, (error)=>{
        if (error){
          throw error;
        }
        dbStmt.bindParam(params, (error)=>{
          if (error){
            throw error;
          }
          dbStmt.execute( (out, error) =>{
            if (error){
              console.log(util.inspect(error));
              throw error;
            }
            // for (let i = 0; i < out.length; i++) {
              // console.log(out[out.length - 1]);
              // console.log('===========================================');
              // console.log('\n\n\n');
            // }
            // console.log(util.inspect(out));
            dbStmt.close();
            dbConn.disconn();
            dbConn.close();
            expect(error).to.be.null;
            // console.log(`ExecuteAsync results:\n ${JSON.stringify(out)}`);
            expect(out).to.be.a('array');
            expect(out.length).to.be.greaterThan(0);
            done();
          });
        });
      });
    });
  });

  describe('exec', () => {
    it('performs action of given SQL String', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn();
      // dbConn.debug(true);
      dbConn.conn('*LOCAL');
      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.exec(sql, (result, error) => {
        if (error){
          console.log(util.inspect(error));
          throw error;
        }
        expect(error).to.be.null;
        // console.log(`Exec Async results:\n ${JSON.stringify(result)}\n`);
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        done();
      });
    });
  });

  describe('exec read blob test', () => {
    it('performs action of given SQL String', (done) => {
      let sql = 'SELECT CAST(\'test\' AS BLOB(10k)) FROM SYSIBM.SYSDUMMY1',
        dbConn = new addon.dbconn();

      // dbConn.debug(true);
      dbConn.conn('*LOCAL');

      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.exec(sql, (result, error) => {
        if (error){
          console.log(util.inspect(error));
          throw error;
        }
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect( Object.values(result[0])[0] ).to.be.instanceOf(Buffer);
        done();
      });
    });
  });

  describe('exec read binary test', () => {
    it('performs action of given SQL String', (done) => {
      let sql = 'SELECT CAST(\'test\' AS BINARY(10)) FROM SYSIBM.SYSDUMMY1',
        dbConn = new addon.dbconn();

      // dbConn.debug(true);
      dbConn.conn('*LOCAL');

      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.exec(sql, (result, error) => {
        if (error){
          console.log(util.inspect(error));
          throw error;
        }
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect( Object.values(result[0])[0] ).to.be.instanceOf(Buffer);
        done();
      });
    });
  });

  describe('exec read varbinary test', () => {
    it('performs action of given SQL String', (done) => {
      let sql = 'SELECT CAST(\'test\' AS VARBINARY(10)) FROM SYSIBM.SYSDUMMY1',
        dbConn = new addon.dbconn();

      // dbConn.debug(true);
      dbConn.conn('*LOCAL');

      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.exec(sql, (result, error) => {
        if (error){
          console.log(util.inspect(error));
          throw error;
        }
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect( Object.values(result[0])[0] ).to.be.instanceOf(Buffer);
        done();
      });
    });
  });

  describe('fetchAll', () => {
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
            // console.log(`FetchAll Async results:\n ${JSON.stringify(result)}\n`);
            expect(result).to.be.a('array');
            expect(result.length).to.be.greaterThan(0);
            done();
          });
        });
      });
    });
  });


  describe('fetch', () => {
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
            if (returnCode !== 0){ //SQL_SUCCESS
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