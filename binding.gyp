{
  'targets': [
    {
      'target_name': 'db2ia',
      'include_dirs': [
        'src/db2ia',
        '/usr/include'
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
        '-gxcoff',
        '-O0'
      ],
      'conditions': [
        [ 'target_arch=="ppc"', {
          'ldflags': [ '-Wl,-bmaxdata:0x60000000/dsa' ],
        }],
        [ 'target_arch=="ppc64"', {
          'cflags': [ '-maix64' ],
          'ldflags': [ '-maix64' ],
        }],
      ],
      'ldflags': [ 
        '-Wl,-bbigtoc', 
        '-Wl,-brtl', 
        '-Wl,-blibpath:/QOpenSys/pkgs/lib:/QOpenSys/usr/lib:/opt/freeware/lib'
      ],
      'link_settings': {
        'libraries': [
          '-L/QOpenSys/pkgs/lib:/QOpenSys/usr/lib:/opt/freeware/lib',
          '-ldb400'
        ],
      }
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