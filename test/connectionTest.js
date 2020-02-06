const {expect} = require('chai');
const db2a = require('../lib/db2a');

const {dbconn} = db2a;
const username = process.env.DBNAME;
const password = process.env.DBPWD;

// Test connection Class
describe('Connection Test', () => {
  // if successful returns undefined
  describe('conn & disconn & isConnected', () => {
    it('disconnects an exsisting connection to the datbase.', () => {
      const connection = new dbconn();
      let result = connection.conn('*LOCAL');
      result = connection.isConnected();
      expect(result).to.be.true;

      result = connection.disconn();
      expect(result).to.be.true;

      result = connection.isConnected();
      expect(result).to.be.false;

      // Test the callback style
      connection.conn('*LOCAL', () => {
        result = connection.isConnected();
        expect(result).to.be.true;

        result = connection.disconn();
        expect(result).to.be.true;

        result = connection.isConnected();
        expect(result).to.be.false;
      });
    });
    if (username && password) {
      it('disconnects an exsisting connection to the datbase with user/pwd.', () => {
        // Test with username/password
        const connection = new dbconn();
        let result = connection.conn('*LOCAL', username, password);
        result = connection.isConnected();
        expect(result).to.be.true;

        result = connection.disconn();
        expect(result).to.be.true;

        result = connection.isConnected();
        expect(result).to.be.false;

        // Test the callback style
        connection.conn('*LOCAL', username, password, () => {
          result = connection.isConnected();
          expect(result).to.be.true;

          result = connection.disconn();
          expect(result).to.be.true;

          result = connection.isConnected();
          expect(result).to.be.false;
        });
      });
    }
  });

  // if successful returns String or Int depending on attribute
  describe('getConnAttr', () => {
    it('getConnAttr(SQL_ATTR_AUTOCOMMIT) should return type Int', () => {
      const attr = db2a.SQL_ATTR_AUTOCOMMIT;
      const connection = new dbconn();
      const result = connection.getConnAttr(attr);
      connection.close();
      expect(result).to.be.a('number');
    });

    it('getConnAttr(SQL_ATTR_DBC_DEFAULT_LIB) should return type String', () => {
      const attr = db2a.SQL_ATTR_DBC_DEFAULT_LIB;
      const connection = new dbconn();
      const result = connection.getConnAttr(attr);
      connection.close();
      expect(result).to.be.a('string');
    });
  });


  // if successful returns undefined
  describe('setConnAttr', () => {
    it('setConnAttr(SQL_ATTR_AUTOCOMMIT, SQL_TRUE) should return true', () => {
      const attr = db2a.SQL_ATTR_AUTOCOMMIT;
      let value = db2a.SQL_TRUE;
      const connection = new dbconn();
      let result = connection.setConnAttr(attr, value);

      expect(result).to.be.true;

      result = connection.getConnAttr(attr);
      expect(result).to.equal(db2a.SQL_TRUE);

      value = db2a.SQL_FALSE;
      result = connection.setConnAttr(attr, value);
      expect(result).to.be.true;

      result = connection.getConnAttr(attr);
      expect(result).to.equal(db2a.SQL_FALSE);

      connection.close();
    });

    it('setConnAttr(SQL_ATTR_INFO_APPLNAME, "NODEJSTEST") should return true', () => {
      const attr = db2a.SQL_ATTR_INFO_APPLNAME;
      const value = 'NODEJSTEST';
      const connection = new dbconn();
      const result = connection.setConnAttr(attr, value);
      connection.close();
      expect(result).to.be.true;
    });
  });

  // if successful returns undefined
  describe('debug', () => {
    it('prints more detailed info if choice = true. Turned off by setting choice = false.', () => {
      let choice = true;
      const connection = new dbconn();
      let result = connection.debug(choice);

      expect(result).to.equal(choice);
      choice = !choice;
      result = connection.debug(choice);
      connection.close();
      expect(result).to.equal(choice);
    });
  });

  // if successful returns String
  describe('validStmt', () => {
    it('if the SQL is valid, validStmt() should return the SQL', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      const connection = new dbconn();
      const result = connection.validStmt(sql);
      connection.close();
      expect(result).to.equal(sql);
    });

    it('if the SQL is invalid, validStmt() should return null', () => {
      try {
        const sql = 'SELECT * FORM QIWS.QCUSTCDT';
        const connection = new dbconn();
        const result = connection.validStmt(sql);
        connection.close();
        expect(result).to.equal(null);
      } catch (e) { }
    });
  });

  // if successful returns undefined
  describe('close', () => {
    it('frees the connection object. ', () => {
      const connection = new dbconn();
      const result = connection.close();

      expect(result).to.be.true;
    });
    it('disconnects if connected and frees the connection object ', () => {
      const connection = new dbconn();
      connection.conn('*LOCAL');
      const result = connection.close();
      expect(result).to.be.true;
    });
  });
});
