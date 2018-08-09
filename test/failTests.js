const assert = require('chai').assert;
const expect = require('chai').expect;
const addon = require('bindings')('db2ia');
const db2a = require('../lib/db2a');
const util = require('util');

//Test Connection Class

// describe('validStmtFail', () => {
//   it('error caused by providing invalid SQL as a param', () => {
//     let sql = 'garbageInput';
//     let dbConn = new addon.DbConn();
//     let result =  dbConn.validStmt();
//     console.log('Valid Stmt output: ' + result);
//     expect(result).to.be.a('string');
//   });
// });

// describe('getConnAttrFail', () => {
//   it('error caused by providing invalid attr as a param', () => {
//     let attr = 50;
//     let dbConn = new addon.DbConn();
//     let result =  dbConn.getConnAttr(attr);
//     console.log('Attrubte: ' + result);
//     expect(result).to.satisfy(function(result){
//       return result === 'string' || typeof result === 'number';
//     });
//   });
// });

// describe('setConnAttrFail', () => {
//   it('error caused by providing invlaid attr and value params', () => {
//     let attr = '';
//     let value = -5;
//     let dbConn = new addon.DbConn();
//     let result =  dbConn.setConnAttr(attr, value);
//     expect(result).to.be.a('undefined');
//   });
// });

// describe('debugFail', () => {
//   it('error caused by using invalid param type instead of a boolean', () => {
//     let choice = 1;
//     let dbConn = new addon.DbConn();
//     let result =  dbConn.debug('choice');
//     expect(result).to.be.a('undefined');
//   });
// });

// need to create a Failure Case for disconn()
// describe('disconnFail' , () => {
// 	it('error caused by calling disconn before Conn was established ', () =>{
// 			let dbConn = new addon.Connection().constructor();
// 			let result =  dbConn.disconn();
// 			expect(result).to.be.a('undefined');
// 	});
// })

// need to create a Failure Case for close()
// describe('closeFail' , () => {
// 	it('error caused by calling close before Conn was established. ', () =>{
// 			//let dbConn = new addon.DbConn();
// 			let result =  dbConn.close();
// 			expect(result).to.be.a('undefined');
// 	});
// })

// need to test the conn method

//Test Statement Class

// describe('prepareFail', () => {
//   it('error caused by preparing invalid SQL as a param', () => {
//     let sql = 'SELECT * ';
//     let dbConn = new addon.DbConn();
//       dbConn.conn("*LOCAL");
//     let dbStmt = new addon.DbStmt(dbConn);
//     let result =  dbStmt.prepare(sql);
//     expect(result).to.be.a('undefined');
//   });
// });

//if successful returns undefined.




// describe('bindParamsFail', () => {
//   it('error caused by not providing correct params within the params[]', () => {
//     let sql = 'INSERT INTO QIWS.QCUSTCDT(CUSNUM,LSTNAM,INIT,STREET,CITY,STATE,ZIPCOD,CDTLMT,CHGCOD,BALDUE,CDTDUE) VALUES (?,?,?,?,?,?,?,?,?,?,?) with NONE ';
//     let dbConn = new addon.DbConn();
//       dbConn.conn("*LOCAL");
//     let dbStmt = new addon.DbStmt(dbConn);
//      dbStmt.prepare(sql);
//     let result =  dbStmt.bindParam([
//       [1, addon.SQL_PARAM_INPUT, addon.SQL_NUMERIC], // change
//       [250, addon.SQL_PARAM_INPUT, addon.SQL_NUMERIC], //BAL DUE
//     ]);
//      dbStmt.execute();
//     expect(result).to.be.a('undefined');
//   });
// });




// describe('closeFail' , () => {
// 	it('error caused by calling close before statement was executed. ', () =>{
// 			let sql = "SELECT * FROM QIWS.QCUSTCDT";
// 			let dbStmt = new addon.Connection().connect().getStatement();
// 			// dbStmt.exec(sql);
// 			let result =  dbStmt.close();
// 			expect(result).to.be.a('undefined');
// 	});
// })



// describe('closeCursorFail' , () => {
// 	it('error caused by calling closeCursor before statement was executed. ', () =>{
// 			let sql = "SELECT * FROM QIWS.QCUSTCDT";
// 			let dbStmt = new addon.Connection().connect().getStatement();
// 			// dbStmt.exec(sql);
// 			let result =  dbStmt.closeCursor();
// 			expect(result).to.be.a('undefined');
// 	});
// })



// need to create a Failure Case for commit()
// describe('commitFail' , () => {
// 	it('error caused by calling commit before statement was executed. ', () =>{
// 			let sql = 'INSERT INTO QIWS.QCUSTCDT(CUSNUM,LSTNAM,INIT,STREET,CITY,STATE,ZIPCOD,CDTLMT,CHGCOD,BALDUE,CDTDUE) VALUES (?,?,?,?,?,?,?,?,?,?,?) with NONE ';
// 			let dbStmt =  new addon.Connection().connect().getStatement();
// 			 dbStmt.prepare(sql);
// 			let result =  dbStmt.bindParam([ [4234,addon.PARM_TYPE_INPUT,2], ['sublime' ,addon.PARM_TYPE_INPUT, 1] ]);
// 			// dbStmt.execute();
// 			let result =  dbStmt.commit();
// 			expect(result).to.be.a('undefined');
// 	});
// })


// describe('execFail', () => {
//   it('error caused by calling exec without params', () => {
//     let sql = 'SELECT * FROM QIWS.QCUSTCDT';
//     let dbConn = new addon.DbConn();
//       dbConn.conn("*LOCAL");
//     let dbStmt = new addon.DbStmt(dbConn);
//     let result =  dbStmt.exec();
//     assert.isNotObject(result, 'object was not returned');
//     console.log('Type of result = ' + typeof result);
//     console.log('Select results: ' + JSON.stringify(result));
//     expect(result).to.be.an('array');
//   });
// });


// describe('executeFail', () => {
//   it('error caused by calling execute before statement was prepared.', () => {
//     let sql = 'SELECT * FROM QIWS.QCUSTCDT';
//     let dbConn = new addon.DbConn();
//       dbConn.conn("*LOCAL");
//     let dbStmt = new addon.DbStmt(dbConn);
//     // dbStmt.prepare(sql);
//     let result =  dbStmt.execute();
//     console.log('Select results: ' + JSON.stringify(result));
//     console.log('Size of the returned array:' + result.length);
//     expect(result).to.be.a('array');
//   });
// });




// describe('fetchAllFail', () => {
//   it('error caused by calling fetchAll before results were available', () => {
//     let sql = 'SELECT * FROM QIWS.QCUSTCDT';
//     let dbConn = new addon.DbConn();
//     let dbStmt = new addon.DbStmt(dbConn);
//      dbStmt.prepare(sql);
//     // dbStmt.execute();
//     let result =  dbStmt.fetchAll();
//     console.log('Select results: ' + JSON.stringify(result));
//     expect(result).to.be.a('array');
//   });
// });



// describe('fetchFail', () => {
//   it('error caused by calling fetch before results were available', () => {
//     let sql = 'SELECT * FROM QIWS.QCUSTCDT';
//     let dbConn = new addon.DbConn();
//     let dbStmt = new addon.DbStmt(dbConn);
//      dbStmt.prepare(sql);
//     // dbStmt.execute();
//     let result =  dbStmt.fetch();
//     console.log('Select results: ' + JSON.stringify(result));
//     expect(result).to.be.a('object');
//   });
// });

// describe('numFieldsFail', () => {
//   it('error caused by calling numFields before results were available.', () => {
//     let sql = 'SELECT * FROM QIWS.QCUSTCDT';
//     let dbConn = new addon.DbConn();
//     let dbStmt = new addon.DbStmt(dbConn);
//      dbStmt.prepare(sql);
//     // dbStmt.execute();
//     let fields =  dbStmt.numFields();
//     console.log('Number of Fields: ' + fields);
//     expect(fields).to.be.a('number');
//   });
// });

// describe('numRowsFail', () => {
//   it('error caused by calling numRows before results were available.', () => {
//     let sql = 'SELECT * FROM QIWS.QCUSTCDT';
//     let dbConn = new addon.DbConn();
//     let dbStmt = new addon.DbStmt(dbConn);
//      dbStmt.prepare(sql);
//     // dbStmt.execute();
//     let rows =  dbStmt.numRows();
//     console.log('Number of Rows: ' + rows);
//     expect(rows).to.be.a('number');
//   });
// });

// describe('fieldTypeFail', () => {
//   it('error caused by not providing an index as a param', () => {
//     let sql = 'SELECT * FROM QIWS.QCUSTCDT';
//     let dbConn = new addon.DbConn();
//     let dbStmt = new addon.DbStmt(dbConn);
//      dbStmt.prepare(sql);
//      dbStmt.execute();
//     let col1 =  dbStmt.fieldType();
//     let col2 =  dbStmt.fieldType();
//     console.log('column 1 fieldType = : ' + col1);
//     console.log('column 2 fieldType = : ' + col2);
//     expect(col1).to.be.a('number');
//     expect(col2).to.be.a('number');
//   });
// });

// describe('fieldWidthFail', () => {
//   it('error caused by not providing an index as a param', () => {
//     let sql = 'SELECT * FROM QIWS.QCUSTCDT';
//     let dbConn = new addon.DbConn();
//     let dbStmt = new addon.DbStmt(dbConn);
//      dbStmt.prepare(sql);
//      dbStmt.execute();
//     let col1 =  dbStmt.fieldWidth();
//     let col2 =  dbStmt.fieldWidth();
//     console.log('column 1 fieldWidth = : ' + col1);
//     console.log('column 2 fieldWidth = : ' + col2);
//     expect(col1).to.be.a('number');
//     expect(col2).to.be.a('number');
//   });
// });

// describe('fieldNullableFail', () => {
//   it('error caused by not providing an index as a param', () => {
//     let sql = 'SELECT * FROM QIWS.QCUSTCDT';
//     let dbConn = new addon.DbConn();
//     let dbStmt = new addon.DbStmt(dbConn);
//      dbStmt.prepare(sql);
//      dbStmt.execute();
//     let col1 =  dbStmt.fieldNullable();
//     console.log(col1);
//   });
// });

// describe('fieldNameFail', () => {
//   it('error caused by providing an invalid index as a param', () => {
//     let sql = 'SELECT * FROM QIWS.QCUSTCDT';
//     let dbConn = new addon.DbConn();
//     let dbStmt = new addon.DbStmt(dbConn);
//      dbStmt.prepare(sql);
//      dbStmt.execute();
//     let col1 =  dbStmt.fieldName('garbageInput');
//     let col2 =  dbStmt.fieldName('fake');
//     console.log('column 1 Name = : ' + col1);
//     console.log('column 2 Name = : ' + col2);
//     expect(col1).to.be.a('string');
//     expect(col2).to.be.a('string');
//   });
// });

// describe('fieldPreciseFail', () => {
//   it('error caused by not providing an index as a param', () => {
//     let sql = 'SELECT * FROM QIWS.QCUSTCDT';
//     let dbConn = new addon.DbConn();
//     let dbStmt = new addon.DbStmt(dbConn);
//      dbStmt.prepare(sql);
//      dbStmt.execute();
//     let col1 =  dbStmt.fieldPrecise();
//     let col2 =  dbStmt.fieldPrecise();
//     console.log('column 1 fieldPrecision = : ' + col1);
//     console.log('column 2 fieldPrecision = : ' + col2);
//     expect(col1).to.be.a('number');
//     expect(col2).to.be.a('number');
//   });
// });

// describe('fieldScaleFail', () => {
//   it('error caused by providing an invalid index as a param', () => {
//     let sql = 'SELECT * FROM QIWS.QCUSTCDT';
//     let dbConn = new addon.DbConn();
//     dbConn.conn("*LOCAL");
//     let dbStmt = new addon.DbStmt(dbConn);
//      dbStmt.prepare(sql);
//      dbStmt.execute();
//     let col1 =  dbStmt.fieldScale('c');
//     let col2 =  dbStmt.fieldScale('a');
//     console.log('column 1 fieldScale = : ' + col1);
//     console.log('column 2 fieldScale = : ' + col2);
//     expect(col1).to.be.a('number');
//     expect(col2).to.be.a('number');
//   });
// });

// describe('setStmtAttrFail', () => {
//   it('error caused by providing invalid attr and value as params', () => {
//     //invalid attr insert
//     let attr = -500;
//     let value = 1;
//     let dbConn = new addon.DbConn();
//       dbConn.conn("*LOCAL");
//     let dbStmt = new addon.DbStmt(dbConn);
//     let result =  dbStmt.setStmtAttr(attr, value);
//     expect(result).to.be.a('undefined');
//   });
// });

// describe('getStmtAttrFail', () => {
//   it('error caused by providing invalid attr as a param.', () => {
//     //insert invalid attr
//     let attr = 2;
//     let dbConn = new addon.DbConn();
//       dbConn.conn("*LOCAL");
//     let dbStmt = new addon.DbStmt(dbConn);
//     let result =  dbStmt.getStmtAttr(attr);
//     expect(result).to.satisfy(function(result){
//       return result === 'string' || typeof result === 'number';
//     });
//   });
// });

// describe('nextResultFail', () => {
//   it('err', () => {
//     let sql = 'SELECT * FROM QIWS.QCUSTCDT';
//     let dbConn = new addon.DbConn();
//       dbConn.conn("*LOCAL");
//     let dbStmt = new addon.DbStmt(dbConn);
//      dbStmt.prepare(sql);
//      dbStmt.execute();
//     let result =  dbStmt.nextResult();
//     expect(result).to.be.a('object');
//   });
// });

// need to create fail case for rollback
// describe('rollbackFail', () => {
// 	it('error caused by ', () => {
// 		let result =  dbStmt.rollback();
// 		let sql = "SELECT * FROM QIWS.QCUSTCDT";
// 		let dbStmt = new addon.Connection().connect().getStatement();
// 		 dbStmt.prepare(sql);
// 		// dbStmt.execute();
// 		expect(result).to.be.a('undefined');
// 	});
// })

// need to create failure case for stmtErr
// describe('stmtError' , () => {
// 	it('error was caused by: ', () =>{
// 			let dbStmt =  new addon.Connection().connect().getStatement();
// 			 dbStmt.stmtError(hType, recno);
// 	});
// })
