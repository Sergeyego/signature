{
  'targets': [
    { 
      'target_name': 'cryptopro',
      'sources': [
        'src/cryptopro.cc',
        'src/certificate.cc',
        'src/provider.cc',
        'src/cades.cc',
        'src/xades.cc',
        'src/helpers.cc'
      ],
      'include_dirs' : [
        '<!@(node -p "require(\'node-addon-api\').include")'
      ],
      'defines': [
        'NAPI_DISABLE_CPP_EXCEPTIONS'
      ],
      'conditions': [
        ['OS=="win"', {
          'include_dirs': [
            '<!(echo %PROGRAMFILES(X86)%)\Crypto Pro\SDK\include'
          ],
          'library_dirs': [
            '<!(echo %PROGRAMFILES(X86)%)\Crypto Pro\SDK\lib\\amd64'
          ],
          'libraries': [
            '-lcades',
            '-lxades'
          ]
        }],
        ['OS=="linux"', {
          'cflags': [
            '-Wno-write-strings',
            '-Wno-missing-field-initializers'
          ],
          'defines': [
            'UNIX',
            'SIZEOF_VOID_P=8'
          ],
          'include_dirs': [
            '/opt/cprocsp/include',
            '/opt/cprocsp/include/cpcsp',
            '/opt/cprocsp/include/pki'
          ],
          'libraries': [
            '-L/opt/cprocsp/lib/amd64',
            '-lcapi10',
            '-lcapi20',
            '-lcades',
            '-lxades',
            '-lrdrsup',
            '-lpthread'
          ]
        }],
        ['OS=="mac"', {
          'cflags': [
            '-Wno-write-strings',
            '-Wno-missing-field-initializers'
          ],
          'defines': [
            'UNIX',
            'SIZEOF_VOID_P=8'
          ],
          'include_dirs': [
            '/opt/cprocsp/include',
            '/opt/cprocsp/include/cpcsp',
            '/Applications/CryptoPro_ECP.app/Contents/Resources/include/pki'
          ],
          'libraries': [
            '-L/opt/cprocsp/lib',
            '-L/Applications/CryptoPro_ECP.app/Contents/MacOS/lib',
            '-lcapi10',
            '-lcapi20',
            '-lcades',
            '-lxades',
            '-lrdrsup',
            '-lpthread',
            '-framework CoreFoundation',
            '-Wl,-rpath,/Applications/CryptoPro_ECP.app/Contents/MacOS/lib/'
          ]
        }]
      ]
    }
  ]
}
