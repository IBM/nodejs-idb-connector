# Node.js iDB Connector
The Node.js iDB Connector is a Node.js DB2 driver open source project from IBM. 

# Documentation
https://www.ibm.com/developerworks/community/wikis/home?lang=en#!/wiki/IBM%20i%20Technology%20Updates/page/The%20new%20DB2%20driver%20and%20toolkit%20for%20Node.js%20on%20IBM%20i

# API Reference
https://www.ibm.com/developerworks/community/wikis/home?lang=en#!/wiki/IBM%20i%20Technology%20Updates/page/DB2%20for%20i%20Access%20APIs%20-%20New

# Installation (GCC 4.8+ is required)
	mv src $Node_Install_Path/lib/node_modules/npm/bin/node-gyp-bin
	mv binding.gyp $Node_Install_Path/liblib/node_modules/npm/bin/node-gyp-bin
	cd $Node_Install_Path/lib/node_modules/npm/bin/node-gyp-bin; node-gyp configure --openssl_fips=x; node-gyp build
  Then you will get the $Node_Install_Path/lib/node_modules/npm/bin/node-gyp-bin/build/Release/db2ia.node generated. 

# License
MIT.  View [`LICENSE`](https://bitbucket.org/litmis/nodejs-idb-connector/src/master/LICENSE) file.