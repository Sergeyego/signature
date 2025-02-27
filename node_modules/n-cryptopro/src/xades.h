#ifndef XADES_H
#define XADES_H

#include "stdafx.h"
#include "helpers.h"
#include "certificate.h"

namespace XAdES
{
  void Init(Napi::Env env, Napi::Object exports);

  Napi::Value SignMessage(const Napi::CallbackInfo& info);
  Napi::Value VerifyMessageSignature(const Napi::CallbackInfo& info);
}

#endif
