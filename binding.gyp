{
  'targets': [
    {
      'cflags': [
        '-std=c++0x',
        '-Wno-unknown-pragmas',
        '-Wno-format',
        '-O3'
      ],
      'ldflags': [
        '-static-libstdc++', 
        '-static-libgcc'
      ],
      'link_settings': {
        'libraries': [
          '-L/usr/lib/libiconv.a',
          '-ldb400'
        ],
      },
      'target_name': 'db2ia',
      'include_dirs': [
        'src/db2ia',
        'include'
      ],
      'sources': [ 
        'src/db2ia/db2ia.cc', 
        'src/db2ia/dbconn.cc',
        'src/db2ia/dbstmt.cc'
      ]
    },
    {
      'cflags': [
        '-std=c++0x',
        '-Wno-unknown-pragmas',
        '-Wno-format',
        '-O3'
      ],
      'ldflags': [
        '-static-libstdc++', 
        '-static-libgcc'
      ],
      'link_settings': {
        'libraries': [
          '-L/usr/lib/libiconv.a',
          '-ldb400'
        ],
      },
      'target_name': 'db2i',
      'include_dirs': [
        'src/db2i',
        'include'
      ],
      'sources': [ 
        'src/db2i/db2i.cc', 
        'src/db2i/dbconn.cc'
      ]
    }
  ]
}