const assert = require('chai').assert;
const expect = require('chai').expect;
const addon = require('bindings')('db2ia');
const db2a = require('../lib/db2a');

// Test connection Class
describe('Connection Test', () => {
  
  //if successful returns undefined
  describe('conn & disconn & isConnected', () => {
    it('disconnects an exsisting connection to the datbase.', () => {
      let connection = new addon.dbconn(),
        result = connection.conn("*LOCAL");
      result = connection.isConnected();
      expect(result).to.be.true;
      
      result = connection.disconn();
      expect(result).to.be.true;
      
      result = connection.isConnected();
      expect(result).to.be.false;
      
      // Test the callback style
      connection.conn("*LOCAL", () => {
        result = connection.isConnected();
        expect(result).to.be.true;
        
        result = connection.disconn();
        expect(result).to.be.true;
        
        result = connection.isConnected();
        expect(result).to.be.false;
      });
    });
  });

  //if successful returns String or Int depending on attribute
  describe('getConnAttr', () => {
    it('getConnAttr(SQL_ATTR_AUTOCOMMIT) should return type Int', () => {
      let attr = db2a.SQL_ATTR_AUTOCOMMIT,
        connection = new addon.dbconn(),
        result = connection.getConnAttr(attr);

      expect(result).to.be.a('number');
    });
      
    it('getConnAttr(SQL_ATTR_DBC_DEFAULT_LIB) should return type String', () => {
      let attr = db2a.SQL_ATTR_DBC_DEFAULT_LIB,
        connection = new addon.dbconn(),
        result = connection.getConnAttr(attr);

      expect(result).to.be.a('string');
    });  
  });


  //if successful returns undefined
  describe('setConnAttr', () => {
    it('setConnAttr(SQL_ATTR_AUTOCOMMIT, SQL_TRUE) should return true', () => {
      let attr = db2a.SQL_ATTR_AUTOCOMMIT,
        value = db2a.SQL_TRUE,
        connection = new addon.dbconn(),
        result = connection.setConnAttr(attr, value);

      expect(result).to.be.true;
      
      result = connection.getConnAttr(attr);
      expect(result).to.equal(db2a.SQL_TRUE);
      
      value = db2a.SQL_FALSE;
      result = connection.setConnAttr(attr, value);
      expect(result).to.be.true;
      
      result = connection.getConnAttr(attr);
      expect(result).to.equal(db2a.SQL_FALSE);
    });
    
    it('setConnAttr(SQL_ATTR_INFO_APPLNAME, "NODEJSTEST") should return true', () => {
      let attr = db2a.SQL_ATTR_INFO_APPLNAME,
        value = "NODEJSTEST",
        connection = new addon.dbconn(),
        result = connection.setConnAttr(attr, value);

      expect(result).to.be.true;
    });
  });

  //if successful returns undefined
  describe('debug', () => {
    it('prints more detailed info if choice = true. Turned off by setting choice = false.', () => {
      let choice = true,
        connection = new addon.dbconn(),
        result = connection.debug(choice);

      expect(result).to.equal(choice);  
      
      choice = !choice;
      result = connection.debug(choice);
      
      expect(result).to.equal(choice);  
    });
  });
  
  //if successful returns String
  describe('validStmt', () => {
    it('if the SQL is valid, validStmt() should return the SQL', () => {
      let sql = 'SELECT * FROM QIWS.QCUSTCDT',
        connection = new addon.dbconn(),
        result = connection.validStmt(sql);

      expect(result).to.equal(sql);
    });
    
    it('if the SQL is invalid, validStmt() should return null', () => {
      try{
        let sql = 'SELECT * FORM QIWS.QCUSTCDT',
          connection = new addon.dbconn(),
          result = connection.validStmt(sql);
          
        expect(result).to.equal(null);
      } catch(e) { console.error(e); }
    });
  });

  //if successful returns undefined
  describe('close', () => {
    it('frees the connection object. ', () => {
      let connection = new addon.dbconn(),
        result = connection.close();

      expect(result).to.be.true;
    });
  });

});