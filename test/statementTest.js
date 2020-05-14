const { expect } = require('chai');
const db2a = require('../lib/db2a');

const {
  IN, CHAR, NUMERIC, dbstmt, dbconn,
} = db2a;

// Test Statement Misc
describe('Statement Misc Test', () => {
  var dbConn, dbStmt;

  before(() => {
    dbConn = new dbconn();
    dbConn.conn('*LOCAL');
  });

  after(() => {
    dbConn.disconn();
    dbConn.close();
  });

  beforeEach(() => {
    dbStmt = new dbstmt(dbConn);
  });

  afterEach(() => {
    dbStmt.close();
  });

  describe('setStmtAttr & getStmtAttr', () => {
    it('setStmtAttr(attribute, value) then getStmtAttr(attribute) should equal value', () => {
      const attr = db2a.SQL_ATTR_FOR_FETCH_ONLY;
      let value = db2a.SQL_TRUE;

      let result = dbStmt.setStmtAttr(attr, value);

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
      let sql = 'SELECT * FROM QIWS.QCUSTCDT';
        dbStmt.prepare(sql);
        dbStmt.execute();
      let result = dbStmt.nextResult();
      expect(result).to.be.a('object');
    });
  })
*/

  describe('rollback', () => {
    it('Rollback all changes to the database that have been made on the connection', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.prepare(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.execute((out, error) => {
          if (error) {
            throw error;
          }
          const result = dbStmt.rollback();
          expect(result).to.be.true;
          done();
        });
      });
    });
  });


  describe('commit', () => {
    it('adds all changes to the database that have been made on the connection since connect time ', (done) => {
      const sql = 'INSERT INTO QIWS.QCUSTCDT(CUSNUM,LSTNAM,INIT,STREET,CITY,STATE,ZIPCOD,CDTLMT,CHGCOD,BALDUE,CDTDUE) VALUES (?,?,?,?,?,?,?,?,?,?,?) with NONE ';

      const params = [9997, 'Doe', 'J D', '123 Broadway', 'Hope', 'WA', 98101, 2000, 1, 250, 0.00];

      dbStmt.prepare(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.bindParam(params, (error) => {
          if (error) {
            throw error;
          }
          dbStmt.execute((out, error) => {
            if (error) {
              throw error;
            }
            const result = dbStmt.commit();
            expect(result).to.be.true;
            done();
          });
        });
      });
    });
  });


  describe('numFields', () => {
    it('retrieves number of fields contained in result', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.prepare(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.execute((out, error) => {
          if (error) {
            throw error;
          }
          const fields = dbStmt.numFields();
          expect(fields).to.be.a('number');
          done();
        });
      });
    });
  });


  describe('numRows', () => {
    it('retrieves number of rows that were effected by a Querry', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.prepare(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.execute((out, error) => {
          if (error) {
            throw error;
          }
          const rows = dbStmt.numRows();
          expect(rows).to.be.a('number');
          done();
        });
      });
    });
  });


  describe('fieldType', () => {
    it('requires an int index parameter, returns the data type of the indicated column', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.prepare(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.execute((out, error) => {
          if (error) {
            throw error;
          }
          const col1 = dbStmt.fieldType(0);
          const col2 = dbStmt.fieldType(1);

          expect(col1).to.be.a('number');
          expect(col2).to.be.a('number');
          done();
        });
      });
    });
  });


  describe('fieldWidth', () => {
    it('requires an int index parameter, returns the field width of the indicated column', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.prepare(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.execute((out, error) => {
          if (error) {
            throw error;
          }
          const col1 = dbStmt.fieldWidth(0);
          const col2 = dbStmt.fieldWidth(1);

          expect(col1).to.be.a('number');
          expect(col2).to.be.a('number');
          done();
        });
      });
    });
  });


  describe('fieldNullable', () => {
    it('requires an int index parameter, returns t/f if the indicated column can be Null', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.prepare(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.execute((out, error) => {
          if (error) {
            throw error;
          }
          const col1 = dbStmt.fieldNullable(0);
          const col2 = dbStmt.fieldNullable(1);

          expect(col1).to.equal(false);
          expect(col2).to.equal(false);
          done();
        });
      });
    });
  });


  describe('fieldName', () => {
    it('requires an int index parameter,returns name of the indicated column ', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.prepare(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.execute((out, error) => {
          if (error) {
            throw error;
          }
          const col1 = dbStmt.fieldName(0);
          const col2 = dbStmt.fieldName(1);

          expect(col1).to.be.a('string');
          expect(col2).to.be.a('string');
          done();
        });
      });
    });
  });


  describe('fieldPrecise', () => {
    it('requires an int index parameter, returns the precision of the indicated column', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.prepare(sql, (error) => {
        if (error) {
          throw error();
        }
        dbStmt.execute((out, error) => {
          if (error) {
            throw error;
          }
          const col1 = dbStmt.fieldPrecise(0);
          const col2 = dbStmt.fieldPrecise(1);

          expect(col1).to.be.a('number');
          expect(col2).to.be.a('number');
          done();
        });
      });
    });
  });


  describe('fieldScale', () => {
    it('requires an int index parameter, returns the scale of the indicated column', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.prepare(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.executeSync((out, error) => {
          if (error) {
            throw error;
          }
          const col1 = dbStmt.fieldScale(0);
          const col2 = dbStmt.fieldScale(1);

          expect(col1).to.be.a('number');
          expect(col2).to.be.a('number');
          done();
        });
      });
    });
  });


  describe('fieldInfo', () => {
    it('requires an int index parameter, returns the information of the indicated column', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.prepare(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.execute((out, error) => {
          if (error) {
            throw error;
          }
          const col1 = dbStmt.fieldInfo(0);
          const col2 = dbStmt.fieldInfo(1);
          expect(col1).to.be.a('object');
          expect(col2).to.be.a('object');
          // console.log(JSON.stringify(col1, '', 2));
          // console.log(JSON.stringify(col2, '', 2));
          expect(col1).to.eql({
            "Name": "CUSNUM",
            "Type": 2,
            "TypeName": "NUMERIC",
            "Width": 7,
            "Precise": 6,
            "Scale": 0,
            "Nullable": false
          });
          expect(col2).to.eql({
            "Name": "LSTNAM",
            "Type": 1,
            "TypeName": "CHAR",
            "Width": 7,
            "Precise": 8,
            "Scale": 0,
            "Nullable": false
          });
          done();
        });
      });
    });
  });


  describe('stmtError', () => {
    it('Returns the diagnostic information ', (done) => {
      const sql = 'SELECT * FROM NOT.THERE';
      const expectedError = 'SQLSTATE=42704 SQLCODE=-204';

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
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.exec(sql, () => {
        const result = dbStmt.closeCursor();
        expect(result).to.be.true;
        done();
      });
    });
  });


  describe('reset', () => {
    it('Reset the dbstmt object. ', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.exec(sql, () => {
        const result = dbStmt.reset();
        expect(result).to.be.undefined;
        done();
      });
    });
  });


  describe('close', () => {
    it('frees the statement object. ', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.exec(sql, (result, error) => {
        const isClose = dbStmt.close();
        expect(isClose).to.be.true;
        done();
      });
    });
  });

  describe('asNumber', () => {
    it('should default to false', () => {
      let result = dbStmt.asNumber();
      expect(result).to.be.false;
    });


    it('when false should return numbers as strings', (done) => {
      const sql = `select 
        cast(-32768 as SMALLINT) MIN_SMALLINT,
        cast(+32767 as SMALLINT) MAX_SMALLINT
         from sysibm.sysdummy1`;

      dbStmt.asNumber(false);

      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(result).to.eql([{
          "MIN_SMALLINT": "-32768",
          "MAX_SMALLINT": "32767"
        }]);
        done();
      });
    });

    it('when true should return numbers when safe to do so', (done) => {
      const sql = `select 
        cast(-32768 as SMALLINT) MIN_SMALLINT,
        cast(+32767 as SMALLINT) MAX_SMALLINT,
        cast(-2147483648 as INT) MIN_INT,
        cast(+2147483647 as INT) MAX_INT,
        cast(999999999999999 as DECIMAL(15,0)) as DEC_SAFE_15_0,
        cast(.999999999999999 as DECIMAL(15,15)) as DEC_SAFE_15_15,
        cast(-3.4e38 as REAL) MIN_REAL,
        cast(+3.4e38 as REAL) MAX_REAL,
        cast(-12345.54321 as REAL) LONG_REAL,
        cast(-1.18e-38 as REAL) MAX_NEG_REAL,
        cast(+1.18e-38 as REAL) MIN_POS_REAL,
        cast(-1.79e308 as DOUBLE) MIN_DOUBLE,
        cast(+1.79e308 as DOUBLE) MAX_DOUBLE,
        cast(-2.23e-308 as DOUBLE) MAX_NEG_DOUBLE,
        cast(+2.23e-308 as DOUBLE) MIN_POS_DOUBLE,
        --these values do not fit in a javascript number datatype
        cast(-9223372036854775808 as BIGINT) MIN_BIGINT,
        cast(+9223372036854775807 as BIGINT) MAX_BIGINT,
        cast(9999999999999999 as DECIMAL(16,0)) as DEC_NOT_SAFE_16_0,
        cast(-9.999999999999999e384 as DECFLOAT(16)) MIN_DECFLOAT16,
        cast(+9.999999999999999e384 as DECFLOAT(16)) MAX_DECFLOAT16,
        cast(-1e-383 as DECFLOAT(16)) MAX_NEG_DECFLOAT16,
        cast(+1e-383 as DECFLOAT(16)) MIN_POS_DECFLOAT16,
        cast(-9.999999999999999999999999999999999e6144 as DECFLOAT(34)) MIN_DECFLOAT34,
        cast(+9.999999999999999999999999999999999e6144 as DECFLOAT(34)) MAX_DECFLOAT34,
        cast(-1e-6143 as DECFLOAT(34)) MAX_NEG_DECFLOAT34,
        cast(+1e-6143 as DECFLOAT(34)) MIN_POS_DECFLOAT34,
        cast( -999999999999999999999999999999999999999999999999999999999999999 AS DECIMAL(63)) AS MIN_DEC63,
        cast( +999999999999999999999999999999999999999999999999999999999999999 AS DECIMAL(63)) AS MAX_DEC63,
        cast(-.000000000000000000000000000000000000000000000000000000000000001 AS DECIMAL(63,63)) AS MAX_NEG_DEC63P63,
        cast(+.000000000000000000000000000000000000000000000000000000000000001 AS DECIMAL(63,63)) AS MIN_POS_DEC63P63,
        cast( -999999999999999999999999999999999999999999999999999999999999999 AS NUMERIC(63)) AS MIN_NUM63P0,
        cast( +999999999999999999999999999999999999999999999999999999999999999 AS NUMERIC(63)) AS MAX_NUM63P0,
        cast(-.000000000000000000000000000000000000000000000000000000000000001 AS NUMERIC(63,63)) AS MAX_NEG_NUM63P63,
        cast(+.000000000000000000000000000000000000000000000000000000000000001 AS NUMERIC(63,63)) AS MIN_POS_NUM63P63
         from sysibm.sysdummy1`;

      dbStmt.asNumber(true);

      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(result).to.eql([{
          "MIN_SMALLINT": -32768,
          "MAX_SMALLINT": 32767,
          "MIN_INT": -2147483648,
          "MAX_INT": 2147483647,
          "DEC_SAFE_15_0": 999999999999999,
          "DEC_SAFE_15_15": 0.999999999999999,
          "MIN_REAL": -3.4e38,
          "MAX_REAL": 3.4e38,
          "LONG_REAL": -12345.5,
          "MAX_NEG_REAL": -1.18e-38,
          "MIN_POS_REAL": 1.18e-38,
          "MIN_DOUBLE": -1.79e308,
          "MAX_DOUBLE": 1.79e308,
          "MAX_NEG_DOUBLE": -2.23e-308,
          "MIN_POS_DOUBLE": 2.23e-308,
          "MIN_BIGINT": "-9223372036854775808",
          "MAX_BIGINT": "9223372036854775807",
          "DEC_NOT_SAFE_16_0": "9999999999999999",
          "MIN_DECFLOAT16": "-9.999999999999999E+384",
          "MAX_DECFLOAT16": "9.999999999999999E+384",
          "MAX_NEG_DECFLOAT16": "-1E-383",
          "MIN_POS_DECFLOAT16": "1E-383",
          "MIN_DECFLOAT34": "-9.999999999999999999999999999999999E+6144",
          "MAX_DECFLOAT34": "9.999999999999999999999999999999999E+6144",
          "MAX_NEG_DECFLOAT34": "-1E-6143",
          "MIN_POS_DECFLOAT34": "1E-6143",
          "MIN_DEC63": "-999999999999999999999999999999999999999999999999999999999999999",
          "MAX_DEC63": "999999999999999999999999999999999999999999999999999999999999999",
          "MAX_NEG_DEC63P63": "-.000000000000000000000000000000000000000000000000000000000000001",
          "MIN_POS_DEC63P63": ".000000000000000000000000000000000000000000000000000000000000001",
          "MIN_NUM63P0": "-999999999999999999999999999999999999999999999999999999999999999",
          "MAX_NUM63P0": "999999999999999999999999999999999999999999999999999999999999999",
          "MAX_NEG_NUM63P63": "-.000000000000000000000000000000000000000000000000000000000000001",
          "MIN_POS_NUM63P63": ".000000000000000000000000000000000000000000000000000000000000001"
        }]);
        done();
      });
    });
  });
});
