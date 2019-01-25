{
  'targets': [
    {
      'target_name': 'db2ia',
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
        '-gxcoff',
        '-O0',
        '-DNAPI_DISABLE_CPP_EXCEPTIONS',
        '-I/QOpenSys/pkgs/include'
      ],
      'ldflags': [ 
        '-Wl,-bbigtoc', 
        '-Wl,-brtl', 
        '-Wl,-bmaxdata:0x60000000/dsa',
        '-Wl,-blibpath:/QOpenSys/usr/lib:/opt/freeware/lib',
        '-static-libstdc++', 
        '-static-libgcc'
      ],
      'link_settings': {
        'libraries': [
          '-L/QOpenSys/usr/lib:/opt/freeware/lib',
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