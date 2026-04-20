const { expect } = require('chai');
const db2a = require('../lib/db2a');

const {
  OUT, IN, CHAR, CLOB, NUMERIC, INT, dbstmt, dbconn,
} = db2a;

const schema = 'IDBTEST';
const table = 'SCORES';
const selectSchema = `SELECT SCHEMA_NAME FROM QSYS2.sysschemas WHERE SCHEMA_NAME = '${schema}'`;
const createSchema = `CREATE SCHEMA ${schema}`;
const createTable = `CREATE OR REPLACE TABLE ${schema}.${table}(team CHAR(10), score INTEGER)`;
const insertTable = `INSERT INTO ${schema}.${table}(TEAM,SCORE) VALUES (?,?)`
const countTeams = `SELECT COUNT(TEAM) as TEAMCOUNT FROM ${schema}.${table}`;
const dropTable = `DROP TABLE ${schema}.${table}`;

// Test Statement Class Async Methods
describe('Statement Async Test', function () {
  // global connection and statement that will be used for test cases
  // dbConn is initialized in before hook and destoryed in after hook
  // dbStmt is initialized in beforeEach hook and destoryed in afterEach hook
  var dbConn, dbStmt;

  function cleanup(connection, statement) {
    statement.close();
    connection.disconn();
    connection.close();
  }

  before('setup schema for tests', function (done) {
    // setup the test infrastructure
    this.timeout(0); // disbale timeout for hook
    const connection = new dbconn();
    connection.conn('*LOCAL');
    const statement = new dbstmt(connection);
    statement.exec(selectSchema, (schemaResult, schemaError) => {
      if (schemaError){
        cleanup(connection, statement);
        done(schemaError);
        return;
      }
      const rc = statement.closeCursor();
      if (!schemaResult.length) {
        statement.exec(createSchema, (createSchemaResult, createSchemaError) => {
          if (createSchemaError) {
            cleanup(connection, statement);
            done(createSchemaError);
            return;
          }
        });
      }
      statement.exec(createTable, (createTableResult, createTableError) => {
        if (createTableError) {
          cleanup(connection, statement);
          done(createTableError);
          return;
        }
        cleanup(connection, statement);
        // create connection that will be used in all the later test cases
        dbConn = new dbconn();
        dbConn.conn('*LOCAL');
        done();
      });
    });
    });

  after('drop objects after the tests', function (done) {
    // close connection used in thetest cases
    dbConn.disconn();
    dbConn.close();
    // tear down test infrastructure
    this.timeout(0); // disable timeout for hook
    const connection = new dbconn();
    connection.conn('*LOCAL');
    const statement = new dbstmt(connection);
    statement.exec(dropTable, (dropTableResult, dropTableError) => {
      if (dropTableError){
        cleanup(connection, statement);
        done(dropTableError);
        return;
      }
        cleanup(connection, statement);
        done();
    });
  });

  // create a new statement before each test case
  beforeEach( function () {
    dbStmt = new dbstmt(dbConn);
  });

  // close the statement after each test case
  afterEach(function () {
    dbStmt.close();
  });

  describe('async prepare', () => {
    it('Prepares valid SQL and sends it to the DBMS, if fail, error is returned. ', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      dbStmt.prepare(sql, (error) => {
        if (error) {
          done(error);
          return;
        }
        expect(error).to.be.null;
        done();
      });
    });
  });

  describe('async bindParams (2-D array)', () => {
    it('associate parameter markers in an SQL statement to app variables', (done) => {
      const dbConn2 = new dbconn();
      dbConn2.conn('*LOCAL');
      const dbStmt2 = new dbstmt(dbConn2);

      const params = [
        ['TEST', IN, CHAR], // TEAM
        [100, IN, INT], // SCORE
      ];

      dbStmt.exec(countTeams, (result, error) => {
        if (error) {
          done(error);
          return;
        }
        let rowsBefore = result[0]['TEAMCOUNT'];
        rowsBefore = Number(rowsBefore);
        dbStmt.close();

        dbStmt2.prepare(insertTable, (error) => {
          if (error) {
            done(error);
            return;
          }
          dbStmt2.bindParam(params, (error) => {
            if (error) {
              done(error);
              return;
            }
            expect(error).to.be.null;
            dbStmt2.execute((out, error) => {
              if (error) {
                done(error);
                return;
              }
              expect(error).to.be.null;
              dbStmt2.close();
              dbStmt = new dbstmt(dbConn);
              dbStmt.exec(countTeams,
                (result, error) => {
                  if (error) {
                    done(error);
                    return;
                  }
                  let rowsAfter = result[0]['TEAMCOUNT'];
                  rowsAfter = Number(rowsAfter);
                  expect(rowsAfter).to.equal(rowsBefore + 1);
                  done();
                });
            });
          });
        });
      });
    });
  });

  describe('async bindParams (1-D array)', () => {
    it('associate parameter markers in an SQL statement to app variables', (done) => {
      const dbConn2 = new dbconn();
      dbConn2.conn('*LOCAL');
      const dbStmt2 = new dbstmt(dbConn2);

      const params = ['TEST2', 200];

      dbStmt.exec(countTeams, (result, error) => {
        if (error) {
          done(error);
          return;
        }
        let rowsBefore = result[0]['TEAMCOUNT'];
        rowsBefore = Number(rowsBefore);
        dbStmt.close();

        dbStmt2.prepare(insertTable, (error) => {
          if (error) {
            done(error);
            return;
          }
          dbStmt2.bindParam(params, (error) => {
            if (error) {
              done(error);
              return;
            }
            expect(error).to.be.null;
            dbStmt2.execute((out, error) => {
              if (error) {
                done(error);
                return;
              }
              expect(error).to.be.null;
              dbStmt2.close();
              dbConn2.close();
              dbStmt = new dbstmt(dbConn);
              dbStmt.exec(countTeams,
                (result, error) => {
                  if (error) {
                    done(error);
                    return;
                  }
                  let rowsAfter = result[0]['TEAMCOUNT'];
                  rowsAfter = Number(rowsAfter);
                  expect(rowsAfter).to.equal(rowsBefore + 1);
                  done();
                });
            });
          });
        })
      });
    });
    it("should insert spaces into CHAR columns when given a Javascript empty-string.", done => {
      const dbConn2 = new dbconn();
      dbConn2.conn("*LOCAL");
      const dbStmt2 = new dbstmt(dbConn2);
      const params = ["", 0 ];

      dbStmt2.prepare(insertTable, error => {
        if (error) {
          done(error);
          return;
        }
        dbStmt2.bindParameters(params, error => {
          if (error) {
            done(error);
            return;
          }
          expect(error).to.be.null;
          dbStmt2.execute((out, error) => {
            if (error) {
              done(error);
              return;
            }
            expect(error).to.be.null;
            dbStmt2.close();
            dbStmt = new dbstmt(dbConn);
            dbStmt.exec(
              `SELECT q.*, hex(TEAM) AS hexteam FROM ${schema}.${table} q where q.TEAM = '          '`,
              (result, error) => {
                if (error) {
                  done(error);
                  return;
                }
                const rowsSelected = Number(result.length);

                expect(rowsSelected).to.equal(1);
                done();
              } // }4040404040404040
            );
          });
        });
      });
    });
  });

  describe('async bindParameters (1-D array)', () => {
    it('associate parameter markers in an SQL statement to app variables', (done) => {
      const dbConn2 = new dbconn();
      dbConn2.conn('*LOCAL');
      const dbStmt2 = new dbstmt(dbConn2);
      const params = ['TEST3', 300];
      dbStmt.exec(countTeams, (result, error) => {
        if (error) {
          done(error);
          return;
        }
        let rowsBefore = result[0]['TEAMCOUNT'];
        rowsBefore = Number(rowsBefore);
        dbStmt.close();

        dbStmt2.prepare(insertTable, (error) => {
          if (error) {
            done(error);
            return;
          }
          dbStmt2.bindParameters(params, (error) => {
            if (error) {
              done(error);
              return;
            }
            expect(error).to.be.null;
            dbStmt2.execute((out, error) => {
              if (error) {
                done(error);
                return;
              }
              expect(error).to.be.null;
              dbStmt2.close();
              dbStmt = new dbstmt(dbConn);
              dbStmt.exec(countTeams,
                (result, error) => {
                  if (error) {
                    done(error);
                    return;
                  }
                  let rowsAfter = result[0]['TEAMCOUNT'];
                  rowsAfter = Number(rowsAfter);
                  expect(rowsAfter).to.equal(rowsBefore + 1);
                  done();
                });
            });
          });
        });
      });
    });
  });

  describe('async execute', () => {
    it('retrieves output params from stored proc', (done) => {
      const sql = 'call QXMLSERV.iPLUG512K(?,?,?,?)';
      const ipc = '*NA';
      const ctl = '*here';
      const xmlIn = `<xmlservice><sh>system 'wrksbs'</sh></xmlservice>`;
      const xmlOut = '';
      const params = [
        [ipc, IN, CHAR],
        [ctl, IN, CHAR],
        [xmlIn, IN, CLOB],
        [xmlOut, OUT, CLOB],
      ];

      dbStmt.prepare(sql, (error) => {
        if (error) {
          done(error);
          return;
        }
        dbStmt.bindParam(params, (error) => {
          if (error) {
            done(error);
            return;
          }
          dbStmt.execute((out, error) => {
            if (error) {
              done(error);
              return;
            }
            expect(error).to.be.null;
            expect(out).to.be.a('array');
            expect(out.length).to.be.eq(1);
            done();
          });
        });
      });
    });

    it('executes prepared statement returns null because no output params are available', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT WHERE BALDUE > ?';
      const params = [
        [10.00, IN, NUMERIC],
      ];

      dbStmt.prepare(sql, (error) => {
        if (error) {
          done(error);
          return;
        }
        dbStmt.bindParam(params, (error) => {
          if (error) {
            done(error);
            return;
          }
          dbStmt.execute((out, error) => {
            if (error) {
              done(error);
              return;
            }
            expect(error).to.be.null;
            expect(out).to.be.null;
            done();
          });
        });
      });
    });
  });

  describe('async execute (1-D param array)', () => {
    it('retrieves output params from stored proc', (done) => {
      const sql = 'call QXMLSERV.iPLUG512K(?,?,?,?)';
      const ipc = '*NA';
      const ctl = '*here';
      const xmlIn = `<xmlservice><sh>system 'wrksbs'</sh></xmlservice>`;
      const xmlOut = '';
      const params = [ipc, ctl, xmlIn, xmlOut];

      dbStmt.prepare(sql, (error) => {
        if (error) {
          done(error);
          return;
        }
        dbStmt.bindParam(params, (error) => {
          if (error) {
            done(error);
            return;
          }
          dbStmt.execute((out, error) => {
            if (error) {
              done(error);
              return;
            }
            expect(error).to.be.null;
            expect(out).to.be.a('array');
            expect(out.length).to.be.eq(params.length);
            done();
          });
        });
      });
    });

    it('executes prepared statement returns null because no output params are available', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT WHERE BALDUE > ?';
      const params = [ 10.00 ];

      dbStmt.prepare(sql, (error) => {
        if (error) {
          done(error);
          return;
        }
        dbStmt.bindParam(params, (error) => {
          if (error) {
            done(error);
            return;
          }
          dbStmt.execute((out, error) => {
            if (error) {
              done(error);
              return;
            }
            expect(error).to.be.null;
            expect(out).to.be.a('array');
            expect(out.length).to.be.eq(params.length);
            done();
          });
        });
      });
    });
  });

  describe('async execute (bindParameters)', () => {
    it('retrieves output params from stored proc', (done) => {
      const sql = 'call QXMLSERV.iPLUG512K(?,?,?,?)';
      const ipc = '*NA';
      const ctl = '*here';
      const xmlIn = `<xmlservice><sh>system 'wrksbs'</sh></xmlservice>`;
      const xmlOut = '';
      const params = [ipc, ctl, xmlIn, xmlOut];

      dbStmt.prepare(sql, (error) => {
        if (error) {
          done(error);
          return;
        }
        dbStmt.bindParameters(params, (error) => {
          if (error) {
            done(error);
            return;
          }
          dbStmt.execute((out, error) => {
            if (error) {
              done(error);
              return;
            }
            expect(error).to.be.null;
            expect(out).to.be.a('array');
            expect(out.length).to.be.eq(params.length);
            done();
          });
        });
      });
    });

    it('executes prepared statement returns null because no output params are available', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT WHERE BALDUE > ?';
      const params = [ 10.00 ];

      dbStmt.prepare(sql, (error) => {
        if (error) {
          done(error);
          return;
        }
        dbStmt.bindParameters(params, (error) => {
          if (error) {
            done(error);
            return;
          }
          dbStmt.execute((out, error) => {
            if (error) {
              done(error);
              return;
            }
            expect(error).to.be.null;
            expect(out).to.be.a('array');
            expect(out.length).to.be.eq(params.length);
            done();
          });
        });
      });
    });
  });

  describe('async exec', () => {
    it('performs action of given SQL String', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      dbStmt.exec(sql, (result, error) => {
        if (error) {
          done(error);
          return;
        }
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        done();
      });
    });
  });

  describe('async fetchAll', () => {
    it('retrieves all rows from execute function:', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      dbStmt.prepare(sql, (error) => {
        if (error) {
          done(error);
          return;
        }
        dbStmt.execute((out, error) => {
          if (error) {
            done(error);
            return;
          }
          dbStmt.fetchAll((result, error) => {
            if (error) {
              done(error);
              return;
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


  describe('async fetch', () => {
    it('retrieves one row from result set:', (done) => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      dbStmt.prepare(sql, (error) => {
        if (error) {
          done(error);
          return;
        }
        dbStmt.execute((out, error) => {
          if (error) {
            done(error);
            return;
          }
          dbStmt.fetch((row, returnCode) => {
            if (returnCode !== 0) { // SQL_SUCCESS
              done(new Error('Rreturn Code was Not SQL SUCESS'));
              return;
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
