const { expect } = require('chai');
const util = require('util');
const fs = require('fs');
const db2a = require('../lib/db2a');

const {
  BLOB, BINARY, IN, OUT, INOUT, INT, CHAR, NUMERIC, BOOLEAN, dbstmt, dbconn,
} = db2a;

describe('Data Type Test', () => {
  let dbConn, dbStmt;

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

  describe('select number types', () => {
    it('smallint', (done) => {
      const sql = 'select * from (values smallint( -32768 )) as x (smallint_val)';
      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0]).to.equal('-32768');
        done();
      });
    });


    it('int', (done) => {
      const sql = 'select * from (values int( -2147483648 )) as x (int_val)';
      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0]).to.equal('-2147483648');
        done();
      });
    });


    it('bigint', (done) => {
      const sql = 'select * from (values bigint( -9223372036854775808 )) as x (bigint_val)';
      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0]).to.equal('-9223372036854775808');
        done();
      });
    });


    // it('real', (done) => {
    //   let sql = 'select * from (values real( -12345.54321 )) as x (real_val)',
    //     dbConn = new dbconn();

    //   dbConn.conn('*LOCAL');

    //   let dbStmt = new dbstmt(dbConn);

    //   dbStmt.exec(sql, (result, error) => {
    //     expect(error).to.be.null;
    //     expect(result).to.be.an('array');
    //     expect(result.length).to.be.greaterThan(0);
    //     expect(Object.values(result[0])[0] ).to.equal("-12345.54321");
    //     done();
    //   });
    // });
  });


  // describe('bind parameters blob/binary/varbinary', () => {
  //   it('create tables for test', (done) => {
  //     const user = (process.env.USER).toUpperCase();
  //     const sql = [
  //       `CREATE SCHEMA IF NOT EXISTS ${user}`,
  //       `CREATE OR REPLACE TABLE ${user}.BLOBTEST(BLOB_COLUMN BLOB(512k))`,
  //       `CREATE OR REPLACE TABLE ${user}.BINARYTEST(BINARY_COLUMN BINARY(5000))`,
  //       `CREATE OR REPLACE TABLE ${user}.VARBINTEST(VARBINARY_COLUMN VARBINARY(5000))`
  //     ];
  //     for (let i = 0; i < sql.length; i++) {
  //       dbStmt.execSync(sql[i], (result, err) => { });
  //       dbStmt.closeCursor();
  //     }
  //     done();
  //   });

  //   it('runs SQLExecute and to bind blob', (done) => {
  //     const user = (process.env.USER).toUpperCase();
  //     // Table which only contains one BLOB(512k) Field
  //     const sql = `INSERT INTO ${user}.BLOBTEST(BLOB_COLUMN) VALUES(?)`;
  //     fs.readFile(`${__dirname}/../README.md`, (error, buffer) => {
  //       if (error) {
  //         throw error;
  //       }
  //       dbStmt.prepare(sql, (error) => {
  //         if (error) {
  //           throw error;
  //         }
  //         dbStmt.bindParam([[buffer, IN, BLOB]], (error) => {
  //           if (error) {
  //             throw error;
  //           }
  //           dbStmt.execute((result, error) => {
  //             if (error) {
  //               console.log(util.inspect(error));
  //               throw error;
  //             }
  //             expect(error).to.be.null;
  //             done();
  //           });
  //         });
  //       });
  //     });
  //   });


  //   it('runs SQLExecute and to bind blob (1-D array)', (done) => {
  //     const user = (process.env.USER).toUpperCase();
  //     // Table which only contains one BLOB(512k) Field
  //     const sql = `INSERT INTO ${user}.BLOBTEST(BLOB_COLUMN) VALUES(?)`;
  //     fs.readFile(`${__dirname}/../README.md`, (error, buffer) => {
  //       if (error) {
  //         throw error;
  //       }
  //       dbStmt.prepare(sql, (error) => {
  //         if (error) {
  //           throw error;
  //         }
  //         dbStmt.bindParam([buffer], (error) => {
  //           if (error) {
  //             throw error;
  //           }
  //           dbStmt.execute((result, error) => {
  //             if (error) {
  //               console.log(util.inspect(error));
  //               throw error;
  //             }
  //             expect(error).to.be.null;
  //             done();
  //           });
  //         });
  //       });
  //     });
  //   });


  //   it('runs SQLExecute and to bind blob (bindParameters)', (done) => {
  //     const user = (process.env.USER).toUpperCase();
  //     // Table which only contains one BLOB(512k) Field
  //     const sql = `INSERT INTO ${user}.BLOBTEST(BLOB_COLUMN) VALUES(?)`;
  //     fs.readFile(`${__dirname}/../README.md`, (error, buffer) => {
  //       if (error) {
  //         throw error;
  //       }
  //       dbStmt.prepare(sql, (error) => {
  //         if (error) {
  //           throw error;
  //         }
  //         dbStmt.bindParameters([buffer], (error) => {
  //           if (error) {
  //             throw error;
  //           }
  //           dbStmt.execute((result, error) => {
  //             if (error) {
  //               console.log(util.inspect(error));
  //               throw error;
  //             }
  //             expect(error).to.be.null;
  //             done();
  //           });
  //         });
  //       });
  //     });
  //   });


  //   it('runs SQLExecute and to bind binary', (done) => {
  //     const user = (process.env.USER).toUpperCase();
  //     // Table which only contains one BLOB(10) Field
  //     const sql = `INSERT INTO ${user}.BINARYTEST(BINARY_COLUMN) VALUES(?)`;
  //     fs.readFile(`${__dirname}/../README.md`, (error, buffer) => {
  //       if (error) {
  //         throw error;
  //       }
  //       dbStmt.prepare(sql, (error) => {
  //         if (error) {
  //           throw error;
  //         }
  //         dbStmt.bindParam([[buffer, IN, BINARY]], (error) => {
  //           if (error) {
  //             throw error;
  //           }
  //           dbStmt.execute((result, error) => {
  //             if (error) {
  //               throw error;
  //             }
  //             expect(error).to.be.null;
  //             done();
  //           });
  //         });
  //       });
  //     });
  //   });


  //   it('runs SQLExecute and to bind binary (1-D array)', (done) => {
  //     const user = (process.env.USER).toUpperCase();
  //     // Table which only contains one BLOB(10) Field
  //     const sql = `INSERT INTO ${user}.BINARYTEST(BINARY_COLUMN) VALUES(?)`;
  //     fs.readFile(`${__dirname}/../README.md`, (error, buffer) => {
  //       if (error) {
  //         throw error;
  //       }
  //       dbStmt.prepare(sql, (error) => {
  //         if (error) {
  //           throw error;
  //         }
  //         dbStmt.bindParam([buffer], (error) => {
  //           if (error) {
  //             throw error;
  //           }
  //           dbStmt.execute((result, error) => {
  //             if (error) {
  //               throw error;
  //             }
  //             expect(error).to.be.null;
  //             done();
  //           });
  //         });
  //       });
  //     });
  //   });


  //   it('runs SQLExecute and to bind binary (bindParameters)', (done) => {
  //     const user = (process.env.USER).toUpperCase();
  //     // Table which only contains one BLOB(10) Field
  //     const sql = `INSERT INTO ${user}.BINARYTEST(BINARY_COLUMN) VALUES(?)`;
  //     fs.readFile(`${__dirname}/../README.md`, (error, buffer) => {
  //       if (error) {
  //         throw error;
  //       }
  //       dbStmt.prepare(sql, (error) => {
  //         if (error) {
  //           throw error;
  //         }
  //         dbStmt.bindParameters([buffer], (error) => {
  //           if (error) {
  //             throw error;
  //           }
  //           dbStmt.execute((result, error) => {
  //             if (error) {
  //               throw error;
  //             }
  //             expect(error).to.be.null;
  //             done();
  //           });
  //         });
  //       });
  //     });
  //   });


  //   it('runs SQLExecute and to bind varbinary', (done) => {
  //     const user = (process.env.USER).toUpperCase();
  //     // Table which only contains one VARBINARY(10) Field
  //     const sql = `INSERT INTO ${user}.VARBINTEST(VARBINARY_COLUMN) VALUES(?)`;
  //     fs.readFile(`${__dirname}/../README.md`, (error, buffer) => {
  //       if (error) {
  //         throw error;
  //       }
  //       dbStmt.prepare(sql, (error) => {
  //         if (error) {
  //           throw error;
  //         }
  //         dbStmt.bindParam([[buffer, IN, BLOB]], (error) => {
  //           if (error) {
  //             throw error;
  //           }
  //           dbStmt.execute((result, error) => {
  //             if (error) {
  //               console.log(util.inspect(error));
  //               throw error;
  //             }
  //             expect(error).to.be.null;
  //             done();
  //           });
  //         });
  //       });
  //     });
  //   });
  // });


  // it('runs SQLExecute and to bind varbinary (1-D array)', (done) => {
  //   const user = (process.env.USER).toUpperCase();
  //   // Table which only contains one VARBINARY(10) Field
  //   const sql = `INSERT INTO ${user}.VARBINTEST(VARBINARY_COLUMN) VALUES(?)`;
  //   fs.readFile(`${__dirname}/../README.md`, (error, buffer) => {
  //     if (error) {
  //       throw error;
  //     }
  //     dbStmt.prepare(sql, (error) => {
  //       if (error) {
  //         throw error;
  //       }
  //       dbStmt.bindParam([buffer], (error) => {
  //         if (error) {
  //           throw error;
  //         }
  //         dbStmt.execute((result, error) => {
  //           if (error) {
  //             console.log(util.inspect(error));
  //             throw error;
  //           }
  //           expect(error).to.be.null;
  //           done();
  //         });
  //       });
  //     });
  //   });
  // });

  // it('runs SQLExecute and to bind varbinary (bindParameters)', (done) => {
  //   const user = (process.env.USER).toUpperCase();
  //   // Table which only contains one VARBINARY(10) Field
  //   const sql = `INSERT INTO ${user}.VARBINTEST(VARBINARY_COLUMN) VALUES(?)`;
  //   fs.readFile(`${__dirname}/../README.md`, (error, buffer) => {
  //     if (error) {
  //       throw error;
  //     }
  //     dbStmt.prepare(sql, (error) => {
  //       if (error) {
  //         throw error;
  //       }
  //       dbStmt.bindParameters([buffer], (error) => {
  //         if (error) {
  //           throw error;
  //         }
  //         dbStmt.execute((result, error) => {
  //           if (error) {
  //             console.log(util.inspect(error));
  //             throw error;
  //           }
  //           expect(error).to.be.null;
  //           done();
  //         });
  //       });
  //     });
  //   });
  // });


  describe('select char types', () => {
    it('char', (done) => {
      const sql = "select * from (values cast('ABCDE' as char(5))) as x (char_val)";
      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0]).to.equal('ABCDE');
        done();
      });
    });

    it('char with padding', (done) => {
      const sql = "select * from (values cast('AB' as char(5))) as x (char_val)";
      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0]).to.equal('AB   ');
        done();
      });
    });

    it('varchar', (done) => {
      const sql = "select * from (values cast('ABCDE' as varchar(10))) as x (varchar_val)";
      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0]).to.equal('ABCDE');
        done();
      });
    });

    it('char with 4-byte UTF-8 characters', (done) => {
      // U+1F600 (😀) is 4 bytes in UTF-8. For CHAR(2), colPrecise=2,
      // bind buffer = 2 * 4 + 1 = 9 bytes. Two 4-byte chars = 8 bytes + null = 9,
      // which exactly fills the buffer, testing the null terminator boundary.
      const emoji = '\u{1F600}\u{1F601}';
      const sql = 'select cast(? as char(8) ccsid 1208) as char_val from sysibm.sysdummy1';
      dbStmt.prepare(sql, (error) => {
        if (error) { throw error; }
        dbStmt.bindParameters([emoji], (error) => {
          if (error) { throw error; }
          dbStmt.execute((out, error) => {
            if (error) { throw error; }
            dbStmt.fetchAll((result, error) => {
              if (error) { throw error; }
              expect(result).to.be.an('array');
              expect(result.length).to.be.greaterThan(0);
              expect(result[0].CHAR_VAL).to.equal(emoji);
              done();
            });
          });
        });
      });
    });
  });

  describe('select boolean type', () => {
    it('boolean true', (done) => {
      const sql = "select * from (values boolean('true')) as x (bool_val)";
      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0]).to.equal(true);
        done();
      });
    });

    it('boolean false', (done) => {
      const sql = "select * from (values boolean('false')) as x (bool_val)";
      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0]).to.equal(false);
        done();
      });
    });

    it('boolean null', (done) => {
      const sql = 'select * from (values cast(null as boolean)) as x (bool_val)';
      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0]).to.be.null;
        done();
      });
    });
  });

  describe('bind boolean type', () => {
    // Round-trips a JS boolean through a real BOOLEAN column: binds true/false/null
    // as a parameter (write path) then reads it back (read path from a driver-
    // described BOOLEAN column). The IBM i CLI rejects SQL_C_BIT for a BOOLEAN
    // parameter, so bindParams binds the value as an integer 1/0 (SQL_C_LONG).
    const user = (process.env.USER).toUpperCase();
    const table = `${user}.BOOLBIND`;

    before(() => {
      const setup = new dbstmt(dbConn);
      try { setup.execSync(`DROP TABLE ${table}`); } catch (e) { /* may not exist */ }
      setup.execSync(`CREATE TABLE ${table} (ID INT, FLAG BOOLEAN)`);
      setup.close();
    });

    after(() => {
      const cleanup = new dbstmt(dbConn);
      try { cleanup.execSync(`DROP TABLE ${table}`); } catch (e) { /* ignore */ }
      cleanup.close();
    });

    function roundTrip(id, value, expected, done) {
      dbStmt.prepare(`INSERT INTO ${table} (ID, FLAG) VALUES (?, ?)`, (error) => {
        expect(error).to.be.null;
        dbStmt.bindParameters([id, value], (error) => {
          expect(error).to.be.null;
          dbStmt.execute((out, error) => {
            expect(error).to.be.null;
            const reader = new dbstmt(dbConn);
            reader.exec(`SELECT FLAG FROM ${table} WHERE ID = ${id}`, (result, error) => {
              expect(error).to.be.null;
              expect(result).to.be.an('array');
              expect(result.length).to.be.greaterThan(0);
              expect(result[0].FLAG).to.equal(expected);
              reader.close();
              done();
            });
          });
        });
      });
    }

    it('binds boolean true', (done) => {
      roundTrip(1, true, true, done);
    });

    it('binds boolean false', (done) => {
      roundTrip(2, false, false, done);
    });

    it('binds boolean null', (done) => {
      roundTrip(3, null, null, done);
    });

    // fetch() has its own column-conversion switch, separate from the one
    // fetchAll()/exec() use, so a BOOLEAN column has to be handled in both.
    it('returns a boolean from single-row fetch', (done) => {
      dbStmt.prepare(`SELECT FLAG FROM ${table} WHERE ID = 1`, (error) => {
        expect(error).to.be.null;
        dbStmt.execute((out, error) => {
          expect(error).to.be.null;
          dbStmt.fetch((result, error) => {
            expect(result).to.be.an('object');
            expect(result.FLAG).to.equal(true);
            done();
          });
        });
      });
    });

    it('returns false from single-row fetch', (done) => {
      dbStmt.prepare(`SELECT FLAG FROM ${table} WHERE ID = 2`, (error) => {
        expect(error).to.be.null;
        dbStmt.execute((out, error) => {
          expect(error).to.be.null;
          dbStmt.fetch((result, error) => {
            expect(result).to.be.an('object');
            expect(result.FLAG).to.equal(false);
            done();
          });
        });
      });
    });

    it('returns null from single-row fetch of a null boolean', (done) => {
      dbStmt.prepare(`SELECT FLAG FROM ${table} WHERE ID = 3`, (error) => {
        expect(error).to.be.null;
        dbStmt.execute((out, error) => {
          expect(error).to.be.null;
          dbStmt.fetch((result, error) => {
            expect(result).to.be.an('object');
            expect(result.FLAG).to.be.null;
            done();
          });
        });
      });
    });

    // Every other boolean read test selects a single row holding a single
    // column. fetchData copies the column per cell and buildJsObject walks
    // rows and columns, so read a mixed-type multi-row result as well.
    it('returns a boolean for every row of a multi-row result', (done) => {
      const sql = `SELECT ID, FLAG FROM ${table} WHERE ID IN (1, 2, 3) ORDER BY ID`;
      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.equal(3);
        expect(result[0].FLAG).to.equal(true);
        expect(result[1].FLAG).to.equal(false);
        expect(result[2].FLAG).to.be.null;
        done();
      });
    });

    // getSQLType() names type 16 BOOLEAN, which reaches JS as fieldInfo().TypeName.
    it('describes a BOOLEAN column as type 16, TypeName BOOLEAN', (done) => {
      dbStmt.prepare(`SELECT FLAG FROM ${table} WHERE ID = 1`, (error) => {
        expect(error).to.be.null;
        dbStmt.execute((out, error) => {
          expect(error).to.be.null;
          const info = dbStmt.fieldInfo(0);
          expect(info.Type).to.equal(16);
          expect(info.TypeName).to.equal('BOOLEAN');
          done();
        });
      });
    });

    // The 2-D bindParams API takes [value, io, indicator] triples and reaches a
    // different branch of bindParams() than the 1-D bindParameters() used above:
    // "bindIndicator == 5" rather than "value.IsBoolean()". Both were changed
    // from SQL_C_BIT to SQL_C_LONG, but only the 1-D branch had coverage.
    function roundTrip2D(id, value, expected, done) {
      dbStmt.prepare(`INSERT INTO ${table} (ID, FLAG) VALUES (?, ?)`, (error) => {
        expect(error).to.be.null;
        dbStmt.bindParam([[id, IN, INT], [value, IN, BOOLEAN]], (error) => {
          expect(error).to.be.null;
          dbStmt.execute((out, error) => {
            expect(error).to.be.null;
            const reader = new dbstmt(dbConn);
            reader.exec(`SELECT FLAG FROM ${table} WHERE ID = ${id}`, (result, error) => {
              expect(error).to.be.null;
              expect(result).to.be.an('array');
              expect(result.length).to.be.greaterThan(0);
              expect(result[0].FLAG).to.equal(expected);
              reader.close();
              done();
            });
          });
        });
      });
    }

    it('binds boolean true through the 2-D bindParams API', (done) => {
      roundTrip2D(4, true, true, done);
    });

    it('binds boolean false through the 2-D bindParams API', (done) => {
      roundTrip2D(5, false, false, done);
    });
  });

  describe('boolean output parameter', () => {
    // Exercises the output side of the boolean parameter binding, which
    // fetchSp() converts back to a JS boolean.
    const user = (process.env.USER).toUpperCase();
    const proc = `${user}.BOOLNOT`;

    before(() => {
      const setup = new dbstmt(dbConn);
      setup.execSync(`CREATE OR REPLACE PROCEDURE ${proc} (INOUT P BOOLEAN)
                      BEGIN
                        SET P = NOT P;
                      END`);
      setup.close();
    });

    after(() => {
      const cleanup = new dbstmt(dbConn);
      try { cleanup.execSync(`DROP PROCEDURE ${proc}`); } catch (e) { /* ignore */ }
      cleanup.close();
    });

    it('returns a boolean output parameter', (done) => {
      dbStmt.prepare(`CALL ${proc}(?)`, (error) => {
        expect(error).to.be.null;
        dbStmt.bindParameters([true], (error) => {
          expect(error).to.be.null;
          dbStmt.execute((out, error) => {
            expect(error).to.be.null;
            expect(out).to.be.an('array');
            expect(out[0]).to.equal(false);
            done();
          });
        });
      });
    });

    it('returns a boolean output parameter in the false to true direction', (done) => {
      dbStmt.prepare(`CALL ${proc}(?)`, (error) => {
        expect(error).to.be.null;
        dbStmt.bindParameters([false], (error) => {
          expect(error).to.be.null;
          dbStmt.execute((out, error) => {
            expect(error).to.be.null;
            expect(out).to.be.an('array');
            expect(out[0]).to.equal(true);
            done();
          });
        });
      });
    });
  });

  describe('boolean output parameter shapes', () => {
    // Two shapes the INOUT round-trip above never reaches: a pure OUT
    // parameter bound through the 2-D API, and a procedure that sets the
    // parameter to NULL.
    const user = (process.env.USER).toUpperCase();
    const proc = `${user}.BOOLSHAPES`;

    before(() => {
      const setup = new dbstmt(dbConn);
      setup.execSync(`CREATE OR REPLACE PROCEDURE ${proc} (OUT P1 BOOLEAN, INOUT P2 BOOLEAN)
                      BEGIN
                        SET P1 = TRUE;
                        SET P2 = NULL;
                      END`);
      setup.close();
    });

    after(() => {
      const cleanup = new dbstmt(dbConn);
      try { cleanup.execSync(`DROP PROCEDURE ${proc}`); } catch (e) { /* ignore */ }
      cleanup.close();
    });

    it('returns a pure OUT boolean parameter', (done) => {
      dbStmt.prepare(`CALL ${proc}(?, ?)`, (error) => {
        expect(error).to.be.null;
        dbStmt.bindParam([[false, OUT, BOOLEAN], [true, INOUT, BOOLEAN]], (error) => {
          expect(error).to.be.null;
          dbStmt.execute((out, error) => {
            expect(error).to.be.null;
            expect(out).to.be.an('array');
            expect(out[0]).to.equal(true);
            done();
          });
        });
      });
    });

    // fetchSp() reads the parameter buffer directly and never consults the
    // null indicator, so a NULL set by the procedure is expected to come back
    // as false rather than null. This test is expected to fail.
    it('returns null for a boolean output parameter set to NULL', (done) => {
      dbStmt.prepare(`CALL ${proc}(?, ?)`, (error) => {
        expect(error).to.be.null;
        dbStmt.bindParam([[false, OUT, BOOLEAN], [true, INOUT, BOOLEAN]], (error) => {
          expect(error).to.be.null;
          dbStmt.execute((out, error) => {
            expect(error).to.be.null;
            expect(out).to.be.an('array');
            expect(out[1]).to.be.null;
            done();
          });
        });
      });
    });
  });

  describe('null output parameters of every bound type', () => {
    // fetchSp() reads each output parameter's buffer directly. Without the
    // indicator check it reports whatever that buffer holds once the procedure
    // has returned NULL, which differs per bound C type. One test per type, so
    // a pre-fix run records exactly what each one came back as.
    const user = (process.env.USER).toUpperCase();
    const proc = `${user}.NULLOUT`;

    before(() => {
      const setup = new dbstmt(dbConn);
      setup.execSync(`CREATE OR REPLACE PROCEDURE ${proc} (
                        INOUT P_CHAR CHAR(10),
                        INOUT P_INT INT,
                        INOUT P_DEC DECIMAL(7,2),
                        INOUT P_BOOL BOOLEAN)
                      BEGIN
                        SET P_CHAR = NULL;
                        SET P_INT = NULL;
                        SET P_DEC = NULL;
                        SET P_BOOL = NULL;
                      END`);
      setup.close();
    });

    after(() => {
      const cleanup = new dbstmt(dbConn);
      try { cleanup.execSync(`DROP PROCEDURE ${proc}`); } catch (e) { /* ignore */ }
      cleanup.close();
    });

    // Bound with the 2-D API so each parameter's C type is chosen explicitly:
    // CHAR -> SQL_C_CHAR, INT -> SQL_C_BIGINT, NUMERIC -> SQL_C_DOUBLE,
    // BOOLEAN -> SQL_C_LONG. Every fetchSp() branch is therefore exercised.
    function callNullOut(assert, done) {
      dbStmt.prepare(`CALL ${proc}(?, ?, ?, ?)`, (error) => {
        expect(error).to.be.null;
        dbStmt.bindParam([
          ['abcdefghij', INOUT, CHAR],
          [42, INOUT, INT],
          [3.14, INOUT, NUMERIC],
          [true, INOUT, BOOLEAN],
        ], (error) => {
          expect(error).to.be.null;
          dbStmt.execute((out, error) => {
            expect(error).to.be.null;
            expect(out).to.be.an('array');
            expect(out.length).to.equal(4);
            assert(out);
            done();
          });
        });
      });
    }

    it('returns null for a NULL CHAR output parameter', (done) => {
      callNullOut((out) => { expect(out[0]).to.be.null; }, done);
    });

    it('returns null for a NULL INTEGER output parameter', (done) => {
      callNullOut((out) => { expect(out[1]).to.be.null; }, done);
    });

    it('returns null for a NULL DECIMAL output parameter', (done) => {
      callNullOut((out) => { expect(out[2]).to.be.null; }, done);
    });

    it('returns null for a NULL BOOLEAN output parameter', (done) => {
      callNullOut((out) => { expect(out[3]).to.be.null; }, done);
    });
  });

  describe('select datalink type', () => {
    // SQL_DATALINK moved from 16 to -400 to make room for the true BOOLEAN type,
    // so these tests guard against a DATALINK column being mis-typed. DB2 for i
    // does not allow casting a character string to DATALINK (SQLCODE -461); a
    // DATALINK value must be built with DLVALUE(). The driver returns it as the
    // string URL via the default string-column path.
    const user = (process.env.USER).toUpperCase();
    const dlTable = `${user}.DLTEST`;

    before(() => {
      const setup = new dbstmt(dbConn);
      try { setup.execSync(`DROP TABLE ${dlTable}`); } catch (e) { /* may not exist */ }
      setup.execSync(`CREATE TABLE ${dlTable} (ID INT, LINK DATALINK(200))`);
      setup.execSync(`INSERT INTO ${dlTable} (ID, LINK) VALUES (1, DLVALUE('http://example.com/file.txt'))`);
      setup.close();
    });

    after(() => {
      const cleanup = new dbstmt(dbConn);
      try { cleanup.execSync(`DROP TABLE ${dlTable}`); } catch (e) { /* ignore */ }
      cleanup.close();
    });

    it('datalink with URL', (done) => {
      const sql = "select dlvalue('http://example.com/file.txt') as datalink_val from sysibm.sysdummy1";
      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        const value = Object.values(result[0])[0];
        expect(value).to.be.a('string');
        // DB2 normalizes the URL scheme and host to uppercase.
        expect(value.toLowerCase()).to.equal('http://example.com/file.txt');
        done();
      });
    });

    // The test above uses dlvalue(), an expression. This reads a real DATALINK
    // column, which is the case the -400 renumbering is actually about.
    //
    // Note: there is deliberately no assertion on fieldInfo().Type here. A real
    // DATALINK column describes as type 12 (VARCHAR) over CLI, as do dlvalue()
    // and cast(null as datalink) -- all three were measured. The -400 code does
    // not appear in result-set metadata at all, so getSQLType()'s SQL_DATALINK
    // case is unreachable from a query and there is nothing to assert against.
    it('reads a real DATALINK column as its URL string', (done) => {
      const sql = `SELECT LINK FROM ${dlTable} WHERE ID = 1`;
      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        const value = Object.values(result[0])[0];
        expect(value).to.be.a('string');
        expect(value.toLowerCase()).to.contain('http://example.com/file.txt');
        done();
      });
    });

    it('datalink null', (done) => {
      const sql = 'select * from (values cast(null as datalink)) as x (datalink_val)';
      dbStmt.exec(sql, (result, error) => {
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0]).to.be.null;
        done();
      });
    });
  });

  describe('exec read blob test', () => {
    it('performs action of given SQL String', (done) => {
      const sql = 'SELECT CAST(\'test\' AS BLOB(10k)) FROM SYSIBM.SYSDUMMY1';
      dbStmt.exec(sql, (result, error) => {
        if (error) {
          console.log(util.inspect(error));
          throw error;
        }
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0]).to.be.instanceOf(Buffer);
        done();
      });
    });
  });


  describe('exec read binary test', () => {
    it('performs action of given SQL String', (done) => {
      const sql = 'SELECT CAST(\'test\' AS BINARY(10)) FROM SYSIBM.SYSDUMMY1';
      dbStmt.exec(sql, (result, error) => {
        if (error) {
          console.log(util.inspect(error));
          throw error;
        }
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0]).to.be.instanceOf(Buffer);
        done();
      });
    });
  });


  describe('exec read varbinary test', () => {
    it('performs action of given SQL String', (done) => {
      const sql = 'SELECT CAST(\'test\' AS VARBINARY(10)) FROM SYSIBM.SYSDUMMY1';
      dbStmt.exec(sql, (result, error) => {
        if (error) {
          console.log(util.inspect(error));
          throw error;
        }
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result.length).to.be.greaterThan(0);
        expect(Object.values(result[0])[0]).to.be.instanceOf(Buffer);
        done();
      });
    });
  });

  describe('inconsitent data', () => {
    it('handle ABC/10 error in exec', (done) => {
      const sql = `SELECT 'ABC'/10 AS DIVERR from sysibm.sysdummy1`;
      dbStmt.exec(sql, (result, error) => {
        if (error) {
          console.log(util.inspect(error));
          throw error;
        }
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result[0].DIVERR).to.equal('-');
        done();
      });
    });

    it('handle ABC/10 error in fetch', (done) => {
      const sql = `SELECT 'ABC'/10 AS DIVERR from sysibm.sysdummy1`;
      dbStmt.prepare(sql, (error) => {
        dbStmt.execute((outParams, error) => {
          dbStmt.fetch((result, error) => {
            expect(error).to.equal(1);
            expect(result).to.be.an('object');
            expect(result.DIVERR).to.equal('-');
            done();
          });
        });
      });
    });

    it('handle ABC/10 error in fetchAll', (done) => {
      const sql = `SELECT 'ABC'/10 AS DIVERR from sysibm.sysdummy1`;
      dbStmt.prepare(sql, (error) => {
        dbStmt.execute((outParams, error) => {
          dbStmt.fetchAll((result, error) => {
            if (error) {
              console.log(util.inspect(error));
              throw error;
            }
            expect(error).to.be.null;
            expect(result).to.be.an('array');
            expect(result[0].DIVERR).to.equal('-');
            done();
          });
        });
      });
    });

    it('handle ABC/10 error in execSync', (done) => {
      const sql = `SELECT 'ABC'/10 AS DIVERR from sysibm.sysdummy1`;
      dbStmt.execSync(sql, (result, error) => {
        if (error) {
          console.log(util.inspect(error));
          throw error;
        }
        expect(error).to.be.null;
        expect(result).to.be.an('array');
        expect(result[0].DIVERR).to.equal('-');
        done();
      });
    });

    it('handle ABC/10 error in fetchSync', (done) => {
      const sql = `SELECT 'ABC'/10 AS DIVERR from sysibm.sysdummy1`;
      const dbConn = new dbconn();
      dbConn.conn('*LOCAL');

      const dbStmt = new dbstmt(dbConn);
      dbStmt.prepareSync(sql, (error) => {
        dbStmt.executeSync((out, error) => {
          dbStmt.fetchSync((result, error) => {
            expect(error).to.equal(1);
            expect(result).to.be.an('object');
            expect(result.DIVERR).to.equal('-');
            done();
          });
        });
      });
    });

    it('handle ABC/10 error in fetchAllSync', (done) => {
      const sql = `SELECT 'ABC'/10 AS DIVERR from sysibm.sysdummy1`;
      dbStmt.prepareSync(sql, (error) => {
        dbStmt.executeSync((outParams, error) => {
          dbStmt.fetchAllSync((result, error) => {
            if (error) {
              console.log(util.inspect(error));
              throw error;
            }
            expect(error).to.be.null;
            expect(result).to.be.an('array');
            expect(result[0].DIVERR).to.equal('-');
            done();
          });
        });
      });
    });
  });
});
