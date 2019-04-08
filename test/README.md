# How to Test

 Mocha & Chai were used to create unit tests. Both are part of dev-deps within the package.json so be sure to run `npm install` first.

 Specific Tests can be run by `npm test path_to_test`.  
 For example when inside the test directory you can run `npm test connectionTest`.  
 All Tests can be run by `npm test`.
 Test connection with user name & password by `DBNAME=yourname DBPWD=password npm test`.