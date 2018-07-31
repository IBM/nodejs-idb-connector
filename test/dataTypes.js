const assert = require('chai').assert;
const expect = require('chai').expect;
const addon = require('bindings')('db2ia');
const db2a = require('../lib/db2a');
const util = require('util');
const fs = require('fs');

//Test Statement Class Async Methods
describe('Data Type Test', () => {
  describe('select number types', () => {
    it('smallint', (done) => {
      let sql = 'select * from (values smallint( -32768 )) as x (smallint_val)',
        dbConn = new addon.dbconn();

      dbConn.conn('*LOCAL');

      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0] ).to.equal("-32768");
        done();
      });
    });
    
    it('int', (done) => {
      let sql = 'select * from (values int( -2147483648 )) as x (int_val)',
        dbConn = new addon.dbconn();

      dbConn.conn('*LOCAL');

      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0] ).to.equal("-2147483648");
        done();
      });
    });
    
    it('bigint', (done) => {
      let sql = 'select * from (values bigint( -9223372036854775808 )) as x (bigint_val)',
        dbConn = new addon.dbconn();

      dbConn.conn('*LOCAL');

      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0] ).to.equal("-9223372036854775808");
        done();
      });
    });
    
    it('real', (done) => {
      let sql = 'select * from (values real( -12345.54321 )) as x (real_val)',
        dbConn = new addon.dbconn();

      dbConn.conn('*LOCAL');

      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0] ).to.equal("-12345.54321");
        done();
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
});
