const { expect } = require('chai');
const util = require('util');
const db2a = require('../lib/db2a');

const {
  OUT, IN, CHAR, CLOB, NUMERIC, INT, dbconn, dbstmt,
} = db2a;

const schema = 'IDBTEST';
const table = 'SCORES';
const selectSchema = `SELECT SCHEMA_NAME FROM QSYS2.sysschemas WHERE SCHEMA_NAME = '${schema}'`;
const createSchema = `CREATE SCHEMA ${schema}`;
const createTable = `CREATE OR REPLACE TABLE ${schema}.${table}(team CHAR(10), score INTEGER)`;
const insertTable = `INSERT INTO ${schema}.${table}(TEAM,SCORE) VALUES (?,?)`
const countTeams = `SELECT COUNT(TEAM) as TEAMCOUNT FROM ${schema}.${table}`;
const dropTable = `DROP TABLE ${schema}.${table}`;

describe('Statement Sync Test', () => {
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
            console.error(`failed to create the schema: ${schema}`);
            console.error(createSchemaError);
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
        console.log(`\n\nBefore hook: Created table: ${table}!\n\n`);
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

  beforeEach(() => {
    dbStmt = new dbstmt(dbConn);
  });

  afterEach(() => {
    dbStmt.close();
  });

  describe('prepare callback', () => {
    it('Prepares valid SQL and sends it to the DBMS, if fail, error is returned. ', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      dbStmt.prepareSync(sql, (error) => {
        if (error) {
          throw error;
        }
        expect(error).to.be.null;
      });
    });
  });

  describe('prepare no-callback', () => {
    it('Prepares valid SQL and sends it to the DBMS, if fail, error is returned. ', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      dbStmt.prepareSync(sql);
    });
  });

  describe('bindParams callback', () => {
    it('associate parameter markers in an SQL statement to app variables', () => {
      const params = [
        ['TEST', IN, CHAR], // TEAM
        [100, IN, INT], // SCORE
      ];

      const dbConn2 = new dbconn();
      dbConn2.conn('*LOCAL');
      const dbStmt2 = new dbstmt(dbConn2);

      const result = dbStmt.execSync(countTeams);
      let rowsBefore = result[0]['TEAMCOUNT'];

      rowsBefore = Number(rowsBefore);
      dbStmt.close();

      dbStmt2.prepareSync(insertTable, (error) => {
        if (error) {
          throw error;
        }
        dbStmt2.bindParamSync(params, (error) => {
          if (error) {
            throw error;
          }
          expect(error).to.be.null;

          dbStmt2.executeSync((out, error) => {
            if (error) {
              throw error;
            }
            dbStmt = new dbstmt(dbConn);

            const result2 = dbStmt.execSync(countTeams);
            let rowsAfter = result2[0]['TEAMCOUNT'];

            rowsAfter = Number(rowsAfter);

            expect(rowsAfter).to.equal(rowsBefore + 1);
          });
        });
      });
    });
  });

  describe('bindParams callback (1-D array)', () => {
    it('associate parameter markers in an SQL statement to app variables', () => {
      const params = ['TEST2', 200];
      const dbConn2 = new dbconn();
      dbConn2.conn('*LOCAL');
      const dbStmt2 = new dbstmt(dbConn2);

      const result = dbStmt.execSync(countTeams);
      let rowsBefore = result[0]['TEAMCOUNT'];

      rowsBefore = Number(rowsBefore);
      dbStmt.close();

      dbStmt2.prepareSync(insertTable, (error) => {
        if (error) {
          throw error;
        }
        dbStmt2.bindParamSync(params, (error) => {
          if (error) {
            throw error;
          }
          expect(error).to.be.null;

          dbStmt2.executeSync((out, error) => {
            if (error) {
              throw error;
            }
            dbStmt = new dbstmt(dbConn);

            const result2 = dbStmt.execSync(countTeams);
            let rowsAfter = result2[0]['TEAMCOUNT'];

            rowsAfter = Number(rowsAfter);

            expect(rowsAfter).to.equal(rowsBefore + 1);
          });
        });
      });
    });
  });

  describe('bindParameters callback', () => {
    it('associate parameter markers in an SQL statement to app variables', () => {
      const params = ['TEST3', 300];
      const dbConn2 = new dbconn();
      dbConn2.conn('*LOCAL');
      const dbStmt2 = new dbstmt(dbConn2);

      const result = dbStmt.execSync(countTeams);
      let rowsBefore = result[0]['TEAMCOUNT'];

      rowsBefore = Number(rowsBefore);
      dbStmt.close();

      dbStmt2.prepareSync(insertTable, (error) => {
        if (error) {
          throw error;
        }
        dbStmt2.bindParametersSync(params, (error) => {
          if (error) {
            throw error;
          }
          expect(error).to.be.null;

          dbStmt2.executeSync((out, error) => {
            if (error) {
              throw error;
            }
            dbStmt = new dbstmt(dbConn);

            const result2 = dbStmt.execSync(countTeams);
            let rowsAfter = result2[0]['TEAMCOUNT'];

            rowsAfter = Number(rowsAfter);

            expect(rowsAfter).to.equal(rowsBefore + 1);
          });
        });
      });
    });
  });

  describe('bindParams no-callback', () => {
    it('associate parameter markers in an SQL statement to app variables', () => {
      const params = [
        ['TEST4', IN, CHAR], // TEAM
        [400, IN, INT], // SCORE
      ];


      const dbConn2 = new dbconn();
      dbConn2.conn('*LOCAL');
      const dbStmt2 = new dbstmt(dbConn2);
      // first get count of current rows
      const result = dbStmt.execSync(countTeams);
      let rowsBefore = result[0]['TEAMCOUNT'];

      rowsBefore = Number(rowsBefore); // count retrurns as a String cast it to Number

      dbStmt.close();

      // now perform insert
      dbStmt2.prepareSync(insertTable);
      dbStmt2.bindParamSync(params);
      dbStmt2.executeSync();

      dbStmt = new dbstmt(dbConn);

      const result2 = dbStmt.execSync(countTeams);
      let rowsAfter = result2[0]['TEAMCOUNT'];

      rowsAfter = Number(rowsAfter);

      expect(rowsAfter).to.equal((rowsBefore + 1));
    });
  });

  describe('bindParams no-callback (1-D array)', () => {
    it('associate parameter markers in an SQL statement to app variables', () => {
      const params = ['TEST5', 500];
      

      const dbConn2 = new dbconn();
      dbConn2.conn('*LOCAL');
      const dbStmt2 = new dbstmt(dbConn2);
      // first get count of current rows
      const result = dbStmt.execSync(countTeams);
      let rowsBefore = result[0]['TEAMCOUNT'];

      rowsBefore = Number(rowsBefore); // count retrurns as a String cast it to Number

      dbStmt.close();

      // now perform insert
      dbStmt2.prepareSync(insertTable);
      dbStmt2.bindParamSync(params);
      dbStmt2.executeSync();

      dbStmt = new dbstmt(dbConn);

      const result2 = dbStmt.execSync(countTeams);
      let rowsAfter = result2[0]['TEAMCOUNT'];

      rowsAfter = Number(rowsAfter);

      expect(rowsAfter).to.equal((rowsBefore + 1));
    });
  });

  describe('bindParameters no-callback', () => {
    it('associate parameter markers in an SQL statement to app variables', () => {
      const params = ['TEST6', 600];

      const dbConn2 = new dbconn();
      dbConn2.conn('*LOCAL');
      const dbStmt2 = new dbstmt(dbConn2);
      // first get count of current rows
      const result = dbStmt.execSync(countTeams);
      let rowsBefore = result[0]['TEAMCOUNT'];

      rowsBefore = Number(rowsBefore); // count retrurns as a String cast it to Number

      dbStmt.close();

      // now perform insert
      dbStmt2.prepareSync(insertTable);
      dbStmt2.bindParametersSync(params);
      dbStmt2.executeSync();

      dbStmt = new dbstmt(dbConn);

      const result2 = dbStmt.execSync(countTeams);
      let rowsAfter = result2[0]['TEAMCOUNT'];

      rowsAfter = Number(rowsAfter);

      expect(rowsAfter).to.equal((rowsBefore + 1));
    });
  });

  describe('exec callback', () => {
    it('performs action of given SQL String', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      dbStmt.execSync(sql, (result, error) => {
        if (error) {
          throw error;
        }
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(result[0]).to.be.an('object');
      });
    });
  });

  describe('exec no-callback', () => {
    it('performs action of given SQL String', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';
      const result = dbStmt.execSync(sql);
      expect(result).to.be.an('array');
      expect(result.length).to.be.greaterThan(0);
      expect(result[0]).to.be.an('object');
    });
  });

  describe('execute callback', () => {
    it('retrieves output parameters from stored proc using executeSync with a callback', () => {
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

      dbStmt.prepareSync(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.bindParamSync(params, (error) => {
          if (error) {
            throw error;
          }
          dbStmt.executeSync((result, error) => {
            if (error) {
              throw error;
            }
            expect(error).to.be.null;
            expect(result).to.be.a('array');
            expect(result.length).to.be.eq(1);
          });
        });
      });
    });


    it('executes prepared statement using executeSync with callback. Returns null because no output params are available', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT WHERE BALDUE > ?';
      const params = [
        [10.00, IN, NUMERIC],
      ];
      dbStmt.prepareSync(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.bindParamSync(params, (error) => {
          if (error) {
            throw error;
          }
          dbStmt.executeSync((out, error) => {
            if (error) {
              throw error;
            }
            expect(error).to.be.null;
            expect(out).to.be.null;
          });
        });
      });
    });
  });

  describe('execute callback (1-D array)', () => {
    it('retrieves output parameters from stored proc using executeSync with a callback', () => {
      const sql = 'call QXMLSERV.iPLUG512K(?,?,?,?)';
      const ipc = '*NA';
      const ctl = '*here';
      const xmlIn = `<xmlservice><sh>system 'wrksbs'</sh></xmlservice>`;
      const xmlOut = '';
      const params = [ipc, ctl, xmlIn, xmlOut];

      dbStmt.prepareSync(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.bindParamSync(params, (error) => {
          if (error) {
            throw error;
          }
          dbStmt.executeSync((result, error) => {
            if (error) {
              throw error;
            }
            expect(error).to.be.null;
            expect(result).to.be.a('array');
            expect(result).to.be.a('array');
            expect(result.length).to.be.eq(params.length);
          });
        });
      });
    });


    it('executes prepared statement using executeSync with callback. Returns null because no output params are available', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT WHERE BALDUE > ?';
      const params = [ 10.00 ];
      dbStmt.prepareSync(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.bindParamSync(params, (error) => {
          if (error) {
            throw error;
          }
          dbStmt.executeSync((result, error) => {
            if (error) {
              throw error;
            }
            expect(error).to.be.null;
            expect(result).to.be.a('array');
            expect(result.length).to.be.eq(params.length);
          });
        });
      });
    });
  });

  describe('execute callback (bindParameters)', () => {
    it('retrieves output parameters from stored proc using executeSync with a callback', () => {
      const sql = 'call QXMLSERV.iPLUG512K(?,?,?,?)';
      const ipc = '*NA';
      const ctl = '*here';
      const xmlIn = `<xmlservice><sh>system 'wrksbs'</sh></xmlservice>`;
      const xmlOut = '';
      const params = [ipc, ctl, xmlIn, xmlOut];

      dbStmt.prepareSync(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.bindParametersSync(params, (error) => {
          if (error) {
            throw error;
          }
          dbStmt.executeSync((result, error) => {
            if (error) {
              throw error;
            }
            expect(error).to.be.null;
            expect(result).to.be.a('array');
            expect(result.length).to.be.eq(params.length);
          });
        });
      });
    });


    it('executes prepared statement using executeSync with callback. Returns null because no output params are available', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT WHERE BALDUE > ?';
      const params = [ 10.00 ];
      dbStmt.prepareSync(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.bindParametersSync(params, (error) => {
          if (error) {
            throw error;
          }
          dbStmt.executeSync((result, error) => {
            if (error) {
              throw error;
            }
            expect(error).to.be.null;
            expect(result).to.be.a('array');
            expect(result.length).to.be.eq(params.length);
          });
        });
      });
    });
  });

  describe('execute no-callback', () => {
    it('executes prepared statement using executeSync without callback. Returns null because no output params are available', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT WHERE BALDUE > ?';
      const params = [
        [10.00, IN, NUMERIC],
      ];

      dbStmt.prepareSync(sql);
      dbStmt.bindParamSync(params);
      const out = dbStmt.executeSync();
      expect(out).to.be.null;
    });
  });

  describe('execute no-callback (1-D array)', () => {
    it('executes prepared statement using executeSync without callback. Returns null because no output params are available', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT WHERE BALDUE > ?';
      const params = [ 10.00 ];

      dbStmt.prepareSync(sql);
      dbStmt.bindParamSync(params);
      const result = dbStmt.executeSync();
      expect(result).to.be.a('array');
      expect(result.length).to.be.eq(params.length);
    });
  });

  describe('execute no-callback (bindParameters)', () => {
    it('executes prepared statement using executeSync without callback. Returns null because no output params are available', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT WHERE BALDUE > ?';
      const params = [ 10.00 ];

      dbStmt.prepareSync(sql);
      dbStmt.bindParametersSync(params);
      const result = dbStmt.executeSync();
      expect(result).to.be.a('array');
      expect(result.length).to.be.eq(params.length);
    });
  });

  describe('fetchAll callback', () => {
    it('retrieves results from execute function:', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.prepareSync(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.executeSync((out, error) => {
          if (error) {
            throw error;
          }
          dbStmt.fetchAllSync((result, error) => {
            if (error) {
              console.log(util.inspect(error));
              throw error;
            }
            expect(error).to.be.null;
            expect(result).to.be.a('array');
            expect(result.length).to.be.greaterThan(0);
          });
        });
      });
    });
  });

  describe('fetchAll no-callback', () => {
    it('retrieves results from execute function:', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.prepareSync(sql);
      dbStmt.executeSync();
      const result = dbStmt.fetchAllSync();
      expect(result).to.be.a('array');
      expect(result.length).to.be.greaterThan(0);
    });
  });

  describe('fetch callback', () => {
    it('retrieves results from execute function:', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.prepareSync(sql, (error) => {
        if (error) {
          throw error;
        }
        dbStmt.executeSync((out, error) => {
          if (error) {
            throw error;
          }
          dbStmt.fetchSync((result, returnCode) => {
            if (returnCode !== 0) { // SQL_SUCCESS
              throw new Error('Rreturn Code was Not SQL SUCESS');
            }
            expect(returnCode).to.equal(0);
            expect(result).to.be.a('object');
          });
        });
      });
    });
  });

  describe('fetch no-callback', () => {
    it('retrieves results from execute function:', () => {
      const sql = 'SELECT * FROM QIWS.QCUSTCDT';

      dbStmt.prepareSync(sql);
      dbStmt.executeSync();
      const result = dbStmt.fetchSync();
      expect(result).to.be.a('object');
    });
  });
});
