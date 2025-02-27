#include "helpers.h"

void HandleError (Napi::Env env, char *message) {
  char szMessage[256];
  DWORD dwLastError = GetLastError();
  if (dwLastError) {
    std::string description(256, 0);
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), &description[0], description.size(), NULL);

    snprintf(szMessage, sizeof(szMessage), "%s (0x%X): %s", message, dwLastError, StringAcpToUtf8(description).c_str());
  } else {
    snprintf(szMessage, sizeof(szMessage), "%s: Undefined error", message);
  }

  Napi::Error::New(env, szMessage).ThrowAsJavaScriptException();
}

void HandleArgumentError (Napi::Env env, char *message) {
  Napi::TypeError::New(env, message).ThrowAsJavaScriptException();
}

std::string StringAcpToUtf8(std::string inString)
{
  if(!inString.size()){
    return "";
  }

  int wcLength = MultiByteToWideChar(CP_ACP, 0, inString.c_str(), -1, NULL, 0);
  wchar_t* pWcString = new wchar_t[wcLength + 1];
  MultiByteToWideChar(CP_ACP, 0, inString.c_str(), -1, pWcString, wcLength);

  int outLength = WideCharToMultiByte(CP_UTF8, 0, pWcString, -1, NULL, 0, NULL, NULL);
  char* pOutString = new char[outLength + 1];
  WideCharToMultiByte(CP_UTF8, 0, pWcString, -1, pOutString, outLength, NULL, NULL);

  std::string outString(pOutString);

  delete[] pOutString;
  delete[] pWcString;

  return outString;
}

std::string StringUtf8ToAcp(std::string inString)
{
  if(!inString.size()){
    return "";
  }

  int wcLength = MultiByteToWideChar(CP_UTF8, 0, inString.c_str(), -1, NULL, 0);
  wchar_t* pWcString = new wchar_t[wcLength + 1];
  MultiByteToWideChar(CP_UTF8, 0, inString.c_str(), -1, pWcString, wcLength);

  int outLength = WideCharToMultiByte(CP_ACP, 0, pWcString, -1, NULL, 0, NULL, NULL);
  char* pOutString = new char[outLength + 1];
  WideCharToMultiByte(CP_ACP, 0, pWcString, -1, pOutString, outLength, NULL, NULL);

  std::string outString(pOutString);

  delete[] pOutString;
  delete[] pWcString;

  return outString;
}


double FileTimeToUnixTimeMs(FILETIME& ft) {
  LONGLONG date = static_cast<LONGLONG>(ft.dwHighDateTime) << 32 | ft.dwLowDateTime;

  return (date - 11644473600000 * 10000) / 10000;
}
