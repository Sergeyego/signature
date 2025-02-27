#ifndef CADES_H
#define CADES_H

#include "stdafx.h"
#include "helpers.h"
#include "certificate.h"

namespace CAdES
{
  void Init(Napi::Env env, Napi::Object exports);

  Napi::Value SignMessage(const Napi::CallbackInfo& info);
  Napi::Value VerifyMessageSignature(const Napi::CallbackInfo& info);
  Napi::Value VerifyDetachedMessageSignature(const Napi::CallbackInfo& info);
}

#endif
