const assert = require('chai').assert;
const expect = require('chai').expect;
const addon = require('bindings')('db2ia');
const db2a = require('../lib/db2a');
const util = require('util');

// Test connection Class

//if successful returns String
describe('validStmt', () => {
  it('if the SQL is valid, validStmt , should return type String', () => {
    let sql = 'SELECT * FROM QIWS.QCUSTCDT',
      connection = new addon.dbconn(),
      result =  connection.validStmt(sql);

    console.log(`Valid Stmt output: ${result}`);
    expect(result).to.be.a('string');
  });
});

//if successful returns String or Int depending on attribute
describe('getConnAttr', () => {
  it('if connection attribute exsits should return type String or Int depending on the attribute type', () => {
    let attr = 0,
      connection = new addon.dbconn(),
      result =  connection.getConnAttr(attr);

    console.log(`Attrubte: ${result}`);
    expect(result).to.satisfy(function(result){
      return result === 'string' || typeof result === 'number';
    });
  });
});


//if successful returns undefined
describe('setConnAttr', () => {
  it('sets the ConnAttr. Attrubte should be INT. Value can String or Int depending on the attribute', () => {
    let attr = 0,
      value = 2,
      connection = new addon.dbconn(),
      result =  connection.setConnAttr(attr, value);

    expect(result).to.be.true;
  });
});

//if successful returns undefined
describe('debug', () => {
  it('prints more detailed info if choice = true. Turned off by setting choice = false.', () => {
    let choice = true,
      connection = new addon.dbconn(),
      result =  connection.debug(choice);

    expect(result).to.be.true;
  });
});

//if successful returns undefined
describe('disconn', () => {
  it('disconnects an exsisting connection to the datbase.', () => {
    let connection = new addon.dbconn(),
      result =  connection.disconn();

    expect(result).to.be.true;
  });
});

//if successful returns undefined
describe('close', () => {
  it('frees the connection object. ', () => {
    let connection = new addon.dbconn(),
      result =  connection.close();

    expect(result).to.be.true;
  });
});

