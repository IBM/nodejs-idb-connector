const assert = require('chai').assert;
const expect = require('chai').expect;
const addon = require('bindings')('db2ia');
const db2a = require('../lib/db2a');
const util = require('util');

//if successful returns undefined
describe('close', () => {
  it('frees the statement object. ', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.exec(sql, ()=>{
      let result =  dbStmt.close();
      expect(result).to.be.a('undefined');
    });
  });
});

//if successful returns undefined
describe('closeCursor', () => {
  it('closes any cursor associated with the dbstmt object and discards any pending results. ', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.exec(sql, ()=>{
      let result =  dbStmt.closeCursor();
      expect(result).to.be.a('undefined');
    });
  });
});

//if successful returns undefined
describe('commit', () => {
  it('adds all changes to the database that have been made on the connection since connect time ', () => {
    let sql = 'INSERT INTO QIWS.QCUSTCDT(CUSNUM,LSTNAM,INIT,STREET,CITY,STATE,ZIPCOD,CDTLMT,CHGCOD,BALDUE,CDTDUE) VALUES (?,?,?,?,?,?,?,?,?,?,?) with NONE ',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepare(sql, ()=>{
      dbStmt.bindParam([
        [9997, addon.SQL_PARAM_INPUT, addon.SQL_NUMERIC], //CUSNUM
        ['Johnson', addon.SQL_PARAM_INPUT, addon.SQL_CHAR], //LASTNAME
        ['A J', addon.SQL_PARAM_INPUT, addon.SQL_CHAR], //INITIAL
        ['453 Example', addon.SQL_PARAM_INPUT, addon.SQL_CHAR], //ADDRESS
        ['Fort', addon.SQL_PARAM_INPUT, addon.SQL_CHAR], //CITY
        ['TN', addon.SQL_PARAM_INPUT, addon.SQL_CHAR], //STATE
        [37211, addon.SQL_PARAM_INPUT, addon.SQL_NUMERIC], //ZIP
        [1000, addon.SQL_PARAM_INPUT, addon.SQL_NUMERIC], //CREDIT LIMIT
        [1, addon.SQL_PARAM_INPUT, addon.SQL_NUMERIC], // change
        [150, addon.SQL_PARAM_INPUT, addon.SQL_NUMERIC], //BAL DUE
        [0.00, addon.SQL_PARAM_INPUT, addon.SQL_NUMERIC] //CREDIT DUE
      ], () =>{
        dbStmt.execute(() =>{
          let result =  dbStmt.commit();
          expect(result).to.be.a('undefined');
        });
      });
    });
  });
});

//if successful returns an Int
describe('numFields', () => {
  it('retrieves number of fields contained in result', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepareSync(sql, ()=>{
      dbStmt.executeSync( () => {
        let fields =  dbStmt.numFields();
        console.log(`\nNumber of Fields: ${fields}`);
        expect(fields).to.be.a('number');
      });
    });
  });
});

//if successful returns an Int
describe('numRows', () => {
  it('retrieves number of rows that were effected by a Querry', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepareSync(sql, ()=>{
      dbStmt.executeSync( () =>{
        let rows =  dbStmt.numRows();
        console.log(`Number of Rows: ${rows}`);
        expect(rows).to.be.a('number');
      });
    });
  });
});

//if successful returns an Int
describe('fieldType', () => {
  it('requires an int index parameter. If a valid index is provided, returns the data type of the indicated column', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);
    dbStmt.prepareSync(sql, () =>{
      dbStmt.executeSync(()=>{
        let col1 =  dbStmt.fieldType(0),
          col2 =  dbStmt.fieldType(1);

        console.log(`column 1 fieldType = ${col1}`);
        console.log(`column 2 fieldType = ${col2}`);
        expect(col1).to.be.a('number');
        expect(col2).to.be.a('number');
      });
    });
  });
});

//if successful returns an Int
describe('fieldWidth', () => {
  it('requires an int index parameter. If a valid index is provided, returns the field width of the indicated column', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);
    dbStmt.prepareSync(sql, () =>{
      dbStmt.executeSync(() =>{
        let col1 =  dbStmt.fieldWidth(0),
          col2 =  dbStmt.fieldWidth(1);

        console.log(`column 1 fieldWidth = ${col1}`);
        console.log(`column 2 fieldWidth = ${col2}`);
        expect(col1).to.be.a('number');
        expect(col2).to.be.a('number');
      });
    });
  });
});

//if successful returns an Int but should return boolean based on doc , UPDATE 3-6-18 added logic to return the boolean. (makeBool method in idb-p)
describe('fieldNullable', () => {
  it('requires an int index parameter. If a valid index is provided, returns t/f if the indicated column can be Null', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepare(sql, () =>{
      console.log('In Prepare');
      dbStmt.execute(() =>{
        console.log('In Execute');
        let col1 =  dbStmt.fieldNullable(0),
          col2 =  dbStmt.fieldNullable(1);

        console.log(`column 1 Nullable? = ${col1}`);
        console.log(`column 2 Nullable? = ${col2}`);
        //****Documnetation says it should return a boolean
        expect(col1).to.equal(false);
        expect(col2).to.equal(false);
      });
    });
  });
});

//if successful returns an String
describe('fieldName', () => {
  it('requires an int index parameter. If a valid index is provided,returns name of the indicated column ', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepareSync(sql, ()=>{
      dbStmt.execute(()=>{
        let col1 =  dbStmt.fieldName(0),
          col2 =  dbStmt.fieldName(1);

        console.log(`column 1 Name = ${col1}`);
        console.log(`column 2 Name = ${col2}`);
        expect(col1).to.be.a('string');
        expect(col2).to.be.a('string');
      });
    });
  });
});

//if successful returns an Int
describe('fieldPrecise', () => {
  it('requires an int index parameter. If a valid index is provided, returns the precision of the indicated column', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepare(sql, () => {
      dbStmt.execute(() =>{
        let col1 =  dbStmt.fieldPrecise(0),
          col2 =  dbStmt.fieldPrecise(1);

        console.log('column 1 fieldPrecision = : ' + col1);
        console.log('column 2 fieldPrecision = : ' + col2);
        expect(col1).to.be.a('number');
        expect(col2).to.be.a('number');
      });
    });
  });
});

//if successful returns an Int
describe('fieldScale', () => {
  it('requires an int index parameter. If a valid index is provided, returns the scale of the indicated column', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepareSync(sql, function(){
      dbStmt.executeSync(function(){
        let col1 =  dbStmt.fieldScale(0),
          col2 =  dbStmt.fieldScale(1);

        console.log(`column 1 fieldScale = ${col1}`);
        console.log(`column 2 fieldScale = ${col2}`);
        expect(col1).to.be.a('number');
        expect(col2).to.be.a('number');
      });
    });
  });
});

//if successful returns undefined
describe('setStmtAttr', () => {
  it('sets StmtAttr Attrubte should be INT. Value can String or Int depending on the attribute', () => {
    let attr = db2a.SQL_ATTR_FOR_FETCH_ONLY,
      value = 1,
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn),
      result =  dbStmt.setStmtAttr(attr, value);

    expect(result).to.be.a('undefined');
  });
});

//if successful returns String or Int depending on attribute
describe('getStmtAttr', () => {
  it('if statement attribute exsits should return type String or Int depending on the attribute type', () => {
    let attr = db2a.SQL_ATTR_FOR_FETCH_ONLY,
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn),
      result =  dbStmt.getStmtAttr(attr);

    console.log('Smt Attr: '+result);
    expect(result).to.satisfy(function(result){
      return result === 'string' || typeof result === 'number';
    });
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
// 		let result =  dbStmt.nextResult();
// 		expect(result).to.be.a('object');
// 	});
// })

//if successful returns undefined
describe('rollback', () => {
  it('Rollback all changes to the database that have been made on the connection', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      dbConn = new addon.dbconn();

    dbConn.conn('*LOCAL');
    let dbStmt = new addon.dbstmt(dbConn);

    dbStmt.prepareSync(sql, () =>{
      dbStmt.executeSync(()=>{
        let result =  dbStmt.rollback();
        expect(result).to.be.a('undefined');
      });
    });
  });
});

//how to test this?
// describe('stmtError' , () => {
// 	it('Returns the diagnostic information ', () =>{
// 			let dbStmt =  new addon.Connection().connect().getStatement();
// 			 dbStmt.stmtError(hType, recno);
// 	});
// })
