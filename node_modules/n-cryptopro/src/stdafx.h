#ifndef STDAFX_H
#define STDAFX_H

#include <napi.h>

#define IGNORE_LEGACY_FORMAT_MESSAGE_MSG

#if defined(WIN32) || defined(_WIN32)
  #include <windows.h>
  #include <wincrypt.h>
  #pragma comment(lib, "Advapi32.lib")
  #pragma comment(lib, "crypt32.lib")
#else
  #include <stdlib.h>
  #include <CSP_WinDef.h>
  #include <CSP_WinCrypt.h>
#endif
#include <WinCryptEx.h>
#include <cades.h>
#include <xades.h>

#endif