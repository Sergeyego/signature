#ifndef PROVIDER_H
#define PROVIDER_H

#include "stdafx.h"
#include "helpers.h"

class Provider : public Napi::ObjectWrap<Provider>
{
public:
  static void Init(Napi::Env env, Napi::Object exports);

  static Napi::Value FromContext(Napi::Env env, HCRYPTPROV hCryptProv);
  static Napi::Value Open(Napi::Env env, Napi::Number nProviderType, Napi::String nsContainerName);
  static Napi::Value FromCertContext(Napi::Env env, PCCERT_CONTEXT pCertContext);

  Provider(const Napi::CallbackInfo &info);
  ~Provider();
  HCRYPTHASH CryptProv() const { return hCryptProv_; }

  Napi::Value GetName(const Napi::CallbackInfo &info);
  Napi::Value GetContainerName(const Napi::CallbackInfo &info);
  Napi::Value GetCertificates(const Napi::CallbackInfo& info);

private:
  static Napi::FunctionReference constructor;
  HCRYPTPROV hCryptProv_;
};

#endif
