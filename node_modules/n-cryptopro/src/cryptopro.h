#ifndef CRYPTOPRO_H
#define CRYPTOPRO_H

#include "stdafx.h"
#include "helpers.h"
#include "provider.h"
#include "certificate.h"
#include "cades.h"
#include "xades.h"

Napi::Value GetCertificates(const Napi::CallbackInfo& info);
Napi::Value GetCertificateFromBuffer(const Napi::CallbackInfo& info);

Napi::Value Hash(const Napi::CallbackInfo& info);
Napi::Value SignHash(const Napi::CallbackInfo& info);
Napi::Value VerifyHashSignature(const Napi::CallbackInfo& info);

Napi::Value EncryptMessage(const Napi::CallbackInfo& info);
Napi::Value DecryptMessage(const Napi::CallbackInfo& info);

Napi::Value SignMessage(const Napi::CallbackInfo& info);
Napi::Value VerifyMessageSignature(const Napi::CallbackInfo& info);
Napi::Value VerifyDetachedMessageSignature(const Napi::CallbackInfo& info);

#endif
