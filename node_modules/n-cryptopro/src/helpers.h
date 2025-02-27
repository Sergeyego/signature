#ifndef HELPERS_H
#define HELPERS_H

#include "stdafx.h"

void HandleError (Napi::Env env, char *message);
void HandleArgumentError (Napi::Env env, char *message);

std::string StringAcpToUtf8(std::string inString);
std::string StringUtf8ToAcp(std::string inString);
double FileTimeToUnixTimeMs(FILETIME& ft);

#endif
