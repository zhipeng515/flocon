{
  'targets': [
    {
      'target_name': 'flocon',
      'sources': [ 'lib/main.cc', 'lib/flocon.cc' ],
      'include_dirs': [
        "<!(node -e \"require('node-addon-api').include\")",
      ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'MACOSX_DEPLOYMENT_TARGET': '10.7',
            'OTHER_CFLAGS': [
              '-std=c++17',
              '-stdlib=libc++',
			  '-Wno-unused-function'
            ]
          }
        }],
        ['OS=="linux"', {
          'cflags': [ 
			  '-std=c++17',
			  '-Wno-unused-function'
			]
        }]
      ]
    }
  ]
}
