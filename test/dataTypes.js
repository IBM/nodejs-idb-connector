const { expect } = require('chai');
const util = require('util');
const fs = require('fs');
const db2a = require('../lib/db2a');

const {
  BLOB, BINARY, IN, dbstmt, dbconn,
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
