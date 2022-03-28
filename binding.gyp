{
  'targets': [
    {
      'target_name': 'db2ia',
      "conditions": [
        ['OS in "aix os400"', {
          'variables': {
              'os_name': '<!(uname -s)',
            },
            'conditions': [
              [ '"<(os_name)"=="OS400"', {
                # Here we know we are really on IBM i
                'include_dirs': [
                  'src/db2ia',
                  '/usr/include',
                  "<!@(node -p \"require('node-addon-api').include\")"
                ],
                'sources': [ 
                  'src/db2ia/db2ia.cc', 
                  'src/db2ia/dbconn.cc',
                  'src/db2ia/dbstmt.cc'
                ],
                'cflags': [
                  '-std=c++0x',
                  '-Wno-unknown-pragmas',
                  '-Wno-format',
                  '-Wno-unused-function',
                  '-gxcoff',
                  '-O0',
                  '-DNAPI_DISABLE_CPP_EXCEPTIONS',
                  '-I/QOpenSys/pkgs/include',
                  '-I/QOpenSys/pkgs/include/cli'
                ],
                'ldflags': [ 
                  '-Wl,-bbigtoc', 
                  '-Wl,-brtl', 
                  '-Wl,-blibpath:/QOpenSys/pkgs/lib:/QOpenSys/usr/lib'
                ],
                'link_settings': {
                  'libraries': [
                    '-L/QOpenSys/pkgs/lib:/QOpenSys/usr/lib',
                    '-ldb400'
                  ],
                }
              }],
            ]
        }],
      ],
      # When not on IBM i the following builds the dummy package
      # os: property in package.json should prevent us from building this dummy
      # unless when really on AIX or os is being ignored by npm bug
      # https://github.com/silverwind/default-gateway/issues/10
      # https://npm.community/t/npm-ci-ignores-the-os-field-of-package-json/5607

      'include_dirs': [
        'src/db2ia',
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'sources': [ 
        'src/db2ia/db2ia.cc',
      ],
      'defines': [
          'NAPI_DISABLE_CPP_EXCEPTIONS',
          'NAPI_VERSION=<(napi_build_version)',
        ],
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
          "destination": "<(module_path)"
        }
      ]
    }
  ]
}
