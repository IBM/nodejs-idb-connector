const assert = require('chai').assert;
const expect = require('chai').expect;
const addon = require('bindings')('db2ia');
const db2a = require('../lib/db2a');
const util = require('util');

// Test Statement Misc
describe('Statement Misc Test', () => {
    
  //if successful returns undefined
  describe('setStmtAttr & getStmtAttr', () => {
    it('setStmtAttr(attribute, value) then getStmtAttr(attribute) should equal value', () => {
      let attr = db2a.SQL_ATTR_FOR_FETCH_ONLY,
        value = db2a.SQL_TRUE,
        dbConn = new addon.dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new addon.dbstmt(dbConn),
        result = dbStmt.setStmtAttr(attr, value);

      expect(result).to.be.true;
      
      result = dbStmt.getStmtAttr(attr);
      expect(result).to.be.equal(value);
      
      value = db2a.SQL_FALSE;
      result = dbStmt.setStmtAttr(attr, value);
      expect(result).to.be.true;
      
      result = dbStmt.getStmtAttr(attr);
      expect(result).to.be.equal(value);
    });
  });

  // whats the passing use case for next Result?
  // describe('nextResult', () => {
  // 	it('Determines whether there is more information available on the statement', () => {
  // 		let sql = "SELECT * FROM QIWS.QCUSTCDT";
  //      let dbConn = new addon.dbconn();
  //       dbConn.conn("*LOCAL");
  //      let dbStmt = new addon.dbstmt(dbConn);
  // 		 dbStmt.prepare(sql);
  // 		 dbStmt.execute();
  // 		let result = dbStmt.nextResult();
  // 		expect(result).to.be.a('object');
  // 	});
  // })

  //if successful returns undefined
  describe('rollback', () => {
    it('Rollback all changes to the database that have been made on the connection', (done) => {
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
          let result = dbStmt.rollback();
          expect(result).to.be.true;
          done();
        });
      });
    });
  });

  //if successful returns undefined
  describe('commit', () => {
    it('adds all changes to the database that have been made on the connection since connect time ', (done) => {
      let sql = 'INSERT INTO QIWS.QCUSTCDT(CUSNUM,LSTNAM,INIT,STREET,CITY,STATE,ZIPCOD,CDTLMT,CHGCOD,BALDUE,CDTDUE) VALUES (?,?,?,?,?,?,?,?,?,?,?) with NONE ',
        dbConn = new addon.dbconn();
      // dbConn.debug(true);
      dbConn.conn('*LOCAL');
      let dbStmt = new addon.dbstmt(dbConn);

      let params = [
        [9997, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC], //CUSNUM
        ['Johnson', db2a.SQL_PARAM_INPUT, db2a.SQL_CHAR], //LASTNAME
        ['A J', db2a.SQL_PARAM_INPUT, db2a.SQL_CHAR], //INITIAL
        ['453 Example', db2a.SQL_PARAM_INPUT, db2a.SQL_CHAR], //ADDRESS
        ['Fort', db2a.SQL_PARAM_INPUT, db2a.SQL_CHAR], //CITY
        ['TN', db2a.SQL_PARAM_INPUT, db2a.SQL_CHAR], //STATE
        [37211, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC], //ZIP
        [1000, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC], //CREDIT LIMIT
        [1, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC], // change
        [150, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC], //BAL DUE
        [0.00, db2a.SQL_PARAM_INPUT, db2a.SQL_NUMERIC] //CREDIT DUE
      ];

      dbStmt.prepare(sql, (error)=>{
        if (error){
          throw error;
        }
        dbStmt.bindParam(params, (error) =>{
          if (error){
            console.log(util.inspect(error));
            throw error;
          }
          dbStmt.execute((out, error) =>{
            if (error){
              throw error;
            }
            let result = dbStmt.commit();
            expect(result).to.be.true;
            done();
          });
        });
      });
    });
  });

  //if successful returns an Int
  describe('numFields', () => {
    it('retrieves number of fields contained in result', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.prepare(sql, (error)=>{
        if (error){
          throw error;
        }
        dbStmt.execute( (out, error) => {
          if (error){
            throw error;
          }
          let fields = dbStmt.numFields();
          // console.log(`\nNumber of Fields: ${fields}`);
          expect(fields).to.be.a('number');
          done();
        });
      });
    });
  });

  //if successful returns an Int
  describe('numRows', () => {
    it('retrieves number of rows that were effected by a Querry', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.prepare(sql, (error)=>{
        if (error){
          throw error;
        }
        dbStmt.execute( (out, error) =>{
          if (error){
            throw error;
          }
          let rows = dbStmt.numRows();
          // console.log(`Number of Rows: ${rows}`);
          expect(rows).to.be.a('number');
          done();
        });
      });
    });
  });

  //if successful returns an Int
  describe('fieldType', () => {
    it('requires an int index parameter, returns the data type of the indicated column', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new addon.dbstmt(dbConn);
      dbStmt.prepare(sql, (error) =>{
        if (error){
          throw error;
        }
        dbStmt.execute((out, error)=>{
          if (error){
            throw error;
          }
          let col1 = dbStmt.fieldType(0),
            col2 = dbStmt.fieldType(1);

          // console.log(`column 1 fieldType = ${col1}`);
          // console.log(`column 2 fieldType = ${col2}`);
          expect(col1).to.be.a('number');
          expect(col2).to.be.a('number');
          done();
        });
      });
    });
  });

  //if successful returns an Int
  describe('fieldWidth', () => {
    it('requires an int index parameter, returns the field width of the indicated column', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new addon.dbstmt(dbConn);
      dbStmt.prepare(sql, (error) =>{
        if (error){
          throw error;
        }
        dbStmt.execute((out, error) =>{
          if (error){
            throw error;
          }
          let col1 = dbStmt.fieldWidth(0),
            col2 = dbStmt.fieldWidth(1);

          // console.log(`column 1 fieldWidth = ${col1}`);
          // console.log(`column 2 fieldWidth = ${col2}`);
          expect(col1).to.be.a('number');
          expect(col2).to.be.a('number');
          done();
        });
      });
    });
  });

  //if successful returns an Int but should return boolean based on doc , UPDATE 3-6-18 added logic to return the boolean. (makeBool method in idb-p)
  describe('fieldNullable', () => {
    it('requires an int index parameter, returns t/f if the indicated column can be Null', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.prepare(sql, (error) =>{
        if (error){
          throw error;
        }
        dbStmt.execute((out, error) =>{
          if (error){
            throw error;
          }
          let col1 = dbStmt.fieldNullable(0),
            col2 = dbStmt.fieldNullable(1);

          // console.log(`column 1 Nullable? = ${col1}`);
          // console.log(`column 2 Nullable? = ${col2}`);
          //****Documnetation says it should return a boolean
          expect(col1).to.equal(false);
          expect(col2).to.equal(false);
          done();
        });
      });
    });
  });

  //if successful returns an String
  describe('fieldName', () => {
    it('requires an int index parameter,returns name of the indicated column ', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.prepare(sql, (error)=>{
        if (error){
          throw error;
        }
        dbStmt.execute((out, error)=>{
          if (error){
            throw error;
          }
          let col1 = dbStmt.fieldName(0),
            col2 = dbStmt.fieldName(1);

          // console.log(`column 1 Name = ${col1}`);
          // console.log(`column 2 Name = ${col2}`);
          expect(col1).to.be.a('string');
          expect(col2).to.be.a('string');
          done();
        });
      });
    });
  });

  //if successful returns an Int
  describe('fieldPrecise', () => {
    it('requires an int index parameter, returns the precision of the indicated column', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.prepare(sql, (error) => {
        if (error){
          throw error();
        }
        dbStmt.execute((out, error) =>{
          if (error){
            throw error;
          }
          let col1 = dbStmt.fieldPrecise(0),
            col2 = dbStmt.fieldPrecise(1);

          // console.log('column 1 fieldPrecision = : ' + col1);
          // console.log('column 2 fieldPrecision = : ' + col2);
          expect(col1).to.be.a('number');
          expect(col2).to.be.a('number');
          done();
        });
      });
    });
  });

  //if successful returns an Int
  describe('fieldScale', () => {
    it('requires an int index parameter, returns the scale of the indicated column', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.prepare(sql, (error) => {
        if (error){
          throw error;
        }
        dbStmt.executeSync((out, error) =>{
          if (error){
            throw error;
          }
          let col1 = dbStmt.fieldScale(0),
            col2 = dbStmt.fieldScale(1);

          // console.log(`column 1 fieldScale = ${col1}`);
          // console.log(`column 2 fieldScale = ${col2}`);
          expect(col1).to.be.a('number');
          expect(col2).to.be.a('number');
          done();
        });
      });
    });
  });

  //if successful returns the error message
  describe('stmtError', () => {
    it('Returns the diagnostic information ', (done) =>{
      let sql = 'SELECT * FROM NOT.THERE',
        expectedError = "SQLSTATE=42704 SQLCODE=-204",
        dbConn = new addon.dbconn();
        
      dbConn.conn('*LOCAL');
      let dbStmt = new addon.dbstmt(dbConn);
      dbStmt.exec(sql, (out, error) => {
        dbStmt.stmtError(db2a.SQL_HANDLE_STMT, 1, (rs) => {
          expect(rs).to.include(expectedError);
          done();
        });
      });
    });
  });

  //if successful returns true
  describe('closeCursor', () => {
    it('closes any cursor associated with the dbstmt object and discards any pending results. ', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.exec(sql, ()=>{
        let result = dbStmt.closeCursor();
        expect(result).to.be.true;
        done();
      });
    });
  });

  //if successful returns undefined
  describe('reset', () => {
    it('Reset the dbstmt object. ', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.exec(sql, ()=>{
        let result = dbStmt.reset();
        expect(result).to.be.undefined;
        done();
      });
    });
  });
  
  //if successful returns true
  describe('close', () => {
    it('frees the statement object. ', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new addon.dbconn();

      // dbConn.debug(true);
      dbConn.conn('*LOCAL');
      let dbStmt = new addon.dbstmt(dbConn);

      dbStmt.exec(sql, (result, error)=>{
        let isClose = dbStmt.close();
        console.log(util.inspect(isClose));
        expect(isClose).to.be.true;
        done();
      });
    });
  });

});