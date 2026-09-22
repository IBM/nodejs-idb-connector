{
  'targets': [
    {
        'target_name': 'db2ia',
        'include_dirs': [
            'src/db2ia',
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
            '-g',
            '-O2',
            '-DNAPI_DISABLE_CPP_EXCEPTIONS',
            '-I/QOpenSys/pkgs/include/cli'
        ],
        'ldflags': [
            '-Wl,-blibpath:/QOpenSys/pkgs/lib:/QOpenSys/usr/lib'
        ],
        'link_settings': {
            'libraries': [
                '-ldb400'
            ],
        }
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ 'db2ia' ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/db2ia.node" ],
          "destination": "<(module_root_dir)/prebuilds/os400-ppc64"
        }
      ]
    }
  ]
}
