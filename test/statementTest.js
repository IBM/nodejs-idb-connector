const expect = require('chai').expect;
const db2a = require('../lib/db2a');
const {IN, CHAR, NUMERIC, dbstmt, dbconn} = db2a;

// Test Statement Misc
describe('Statement Misc Test', () => {
  describe('setStmtAttr & getStmtAttr', () => {
    it('setStmtAttr(attribute, value) then getStmtAttr(attribute) should equal value', () => {
      let attr = db2a.SQL_ATTR_FOR_FETCH_ONLY,
        value = db2a.SQL_TRUE,
        dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn),
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

  /*
  TODO create pssing unit test for nextResult()

  describe('nextResult', () => {
  	it('Determines whether there is more information available on the statement', () => {
  		let sql = "SELECT * FROM QIWS.QCUSTCDT";
       let dbConn = new dbconn();
        dbConn.conn("*LOCAL");
       let dbStmt = new dbstmt(dbConn);
  		 dbStmt.prepare(sql);
  		 dbStmt.execute();
  		let result = dbStmt.nextResult();
  		expect(result).to.be.a('object');
  	});
  })
*/

  describe('rollback', () => {
    it('Rollback all changes to the database that have been made on the connection', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn);

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


  describe('commit', () => {
    it('adds all changes to the database that have been made on the connection since connect time ', (done) => {
      let sql = 'INSERT INTO QIWS.QCUSTCDT(CUSNUM,LSTNAM,INIT,STREET,CITY,STATE,ZIPCOD,CDTLMT,CHGCOD,BALDUE,CDTDUE) VALUES (?,?,?,?,?,?,?,?,?,?,?) with NONE ',
        dbConn = new dbconn();
      dbConn.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn);

      let params = [
        [9997, IN, NUMERIC], //CUSNUM
        ['Johnson', IN, CHAR], //LASTNAME
        ['A J', IN, CHAR], //INITIAL
        ['453 Example', IN, CHAR], //ADDRESS
        ['Fort', IN, CHAR], //CITY
        ['TN', IN, CHAR], //STATE
        [37211, IN, NUMERIC], //ZIP
        [1000, IN, NUMERIC], //CREDIT LIMIT
        [1, IN, NUMERIC], // change
        [150, IN, NUMERIC], //BAL DUE
        [0.00, IN, NUMERIC] //CREDIT DUE
      ];

      dbStmt.prepare(sql, (error)=>{
        if (error){
          throw error;
        }
        dbStmt.bindParam(params, (error) =>{
          if (error){
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


  describe('numFields', () => {
    it('retrieves number of fields contained in result', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn);

      dbStmt.prepare(sql, (error)=>{
        if (error){
          throw error;
        }
        dbStmt.execute( (out, error) => {
          if (error){
            throw error;
          }
          let fields = dbStmt.numFields();
          expect(fields).to.be.a('number');
          done();
        });
      });
    });
  });


  describe('numRows', () => {
    it('retrieves number of rows that were effected by a Querry', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn);

      dbStmt.prepare(sql, (error)=>{
        if (error){
          throw error;
        }
        dbStmt.execute( (out, error) =>{
          if (error){
            throw error;
          }
          let rows = dbStmt.numRows();
          expect(rows).to.be.a('number');
          done();
        });
      });
    });
  });


  describe('fieldType', () => {
    it('requires an int index parameter, returns the data type of the indicated column', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn);
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

          expect(col1).to.be.a('number');
          expect(col2).to.be.a('number');
          done();
        });
      });
    });
  });


  describe('fieldWidth', () => {
    it('requires an int index parameter, returns the field width of the indicated column', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn);
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

          expect(col1).to.be.a('number');
          expect(col2).to.be.a('number');
          done();
        });
      });
    });
  });


  describe('fieldNullable', () => {
    it('requires an int index parameter, returns t/f if the indicated column can be Null', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn);

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

          expect(col1).to.equal(false);
          expect(col2).to.equal(false);
          done();
        });
      });
    });
  });


  describe('fieldName', () => {
    it('requires an int index parameter,returns name of the indicated column ', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn);

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

          expect(col1).to.be.a('string');
          expect(col2).to.be.a('string');
          done();
        });
      });
    });
  });


  describe('fieldPrecise', () => {
    it('requires an int index parameter, returns the precision of the indicated column', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn);

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

          expect(col1).to.be.a('number');
          expect(col2).to.be.a('number');
          done();
        });
      });
    });
  });


  describe('fieldScale', () => {
    it('requires an int index parameter, returns the scale of the indicated column', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn);

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

          expect(col1).to.be.a('number');
          expect(col2).to.be.a('number');
          done();
        });
      });
    });
  });


  describe('stmtError', () => {
    it('Returns the diagnostic information ', (done) =>{
      let sql = 'SELECT * FROM NOT.THERE',
        expectedError = "SQLSTATE=42704 SQLCODE=-204",
        dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn);
      dbStmt.exec(sql, (out, error) => {
        dbStmt.stmtError(db2a.SQL_HANDLE_STMT, 1, (rs) => {
          expect(rs).to.include(expectedError);
          done();
        });
      });
    });
  });


  describe('closeCursor', () => {
    it('closes any cursor associated with the dbstmt object and discards any pending results. ', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn);

      dbStmt.exec(sql, ()=>{
        let result = dbStmt.closeCursor();
        expect(result).to.be.true;
        done();
      });
    });
  });


  describe('reset', () => {
    it('Reset the dbstmt object. ', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn);

      dbStmt.exec(sql, ()=>{
        let result = dbStmt.reset();
        expect(result).to.be.undefined;
        done();
      });
    });
  });


  describe('close', () => {
    it('frees the statement object. ', (done) => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        dbConn = new dbconn();

      dbConn.conn('*LOCAL');
      let dbStmt = new dbstmt(dbConn);

      dbStmt.exec(sql, (result, error)=>{
        let isClose = dbStmt.close();
        expect(isClose).to.be.true;
        done();
      });
    });
  });
});
