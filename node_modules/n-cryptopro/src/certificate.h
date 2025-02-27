#ifndef CERTIFICATE_H
#define CERTIFICATE_H

#include "stdafx.h"
#include "helpers.h"
#include "provider.h"

class Certificate : public Napi::ObjectWrap<Certificate>
{
public:
  static void Init(Napi::Env env, Napi::Object exports);

  static Napi::Value FromContext(Napi::Env env, PCCERT_CONTEXT pCertContext);
  static Napi::Value FromBuffer(Napi::Env env, Napi::Buffer<uint8_t> nbBuffer);

  Certificate(const Napi::CallbackInfo &info);
  ~Certificate();
  PCCERT_CONTEXT Context() const { return pContext_; }

  Napi::Value GetSubjectName(const Napi::CallbackInfo &info);
  Napi::Value GetIssuerName(const Napi::CallbackInfo &info);
  Napi::Value GetSubjectAttribute(const Napi::CallbackInfo &info);
  Napi::Value GetIssuerAttribute(const Napi::CallbackInfo &info);
  Napi::Value GetSerialNumber(const Napi::CallbackInfo &info);
  Napi::Value GetSignatureAlgorithm(const Napi::CallbackInfo& info);
  Napi::Value GetPublicKeyAlgorithm(const Napi::CallbackInfo& info);
  Napi::Value GetValidPeriod(const Napi::CallbackInfo &info);
  Napi::Value VerifyChain(const Napi::CallbackInfo &info);
  Napi::Value GetProvider(const Napi::CallbackInfo& info);

private:
  static Napi::FunctionReference constructor;
  PCCERT_CONTEXT pContext_;
};

#endif
