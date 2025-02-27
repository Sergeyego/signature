#include "provider.h"

Napi::FunctionReference Provider::constructor;

void Provider::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Provider", {
    InstanceMethod("getName", &Provider::GetName),
    InstanceMethod("getContainerName", &Provider::GetContainerName)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Provider", func);

  exports.Set(Napi::String::New(env, "PROV_GOST_2001_DH"), Napi::Number::New(env, PROV_GOST_2001_DH));
  exports.Set(Napi::String::New(env, "PROV_GOST_2012_256"), Napi::Number::New(env, PROV_GOST_2012_256));
  exports.Set(Napi::String::New(env, "PROV_GOST_2012_512"), Napi::Number::New(env, PROV_GOST_2012_512));
}

Provider::Provider(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Provider>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  hCryptProv_ = *info[0].As<Napi::External<HCRYPTPROV>>().Data();
};

Provider::~Provider() {
  CryptReleaseContext(hCryptProv_, 0);
}

Napi::Value Provider::FromContext(Napi::Env env, HCRYPTPROV hCryptProv) {
  return constructor.New({ Napi::External<HCRYPTPROV>::New(env, &hCryptProv) });
}

Napi::Value Provider::Open(Napi::Env env, Napi::Number nProviderType, Napi::String nsContainerName) {
  HCRYPTPROV hCryptProv;
  if (!CryptAcquireContext(&hCryptProv, nsContainerName.Utf8Value().c_str(), NULL, nProviderType.Int32Value(), CRYPT_SILENT)) {
    HandleError(env, "Error on CryptAcquireContext");
    return env.Undefined();
  }

  return FromContext(env, hCryptProv);
}

Napi::Value Provider::FromCertContext(Napi::Env env, PCCERT_CONTEXT pCertContext) {
  HCRYPTPROV hCryptProv;
  if (!CryptAcquireCertificatePrivateKey(pCertContext, CRYPT_ACQUIRE_SILENT_FLAG, NULL, &hCryptProv, NULL, FALSE)) {
    HandleError(env, "Error on CryptAcquireContext");
    return env.Undefined();
  }

  return FromContext(env, hCryptProv);
}

Napi::Value Provider::GetName(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  DWORD dwNameLength;
  if (!CryptGetProvParam(hCryptProv_, PP_NAME, NULL, &dwNameLength, 0)) {
    HandleError(env, "Error on CryptGetProvParam:1");
    return env.Undefined();
  }

  std::string name(dwNameLength, 0);
  if (!CryptGetProvParam(hCryptProv_, PP_NAME, (BYTE *)name.c_str(), &dwNameLength, 0)) {
    HandleError(env, "Error on CryptGetProvParam:2");
    return env.Undefined();
  }

  return Napi::String::New(env, StringAcpToUtf8(name));
}

Napi::Value Provider::GetContainerName(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  DWORD dwNameLength;
  if (!CryptGetProvParam(hCryptProv_, PP_CONTAINER, NULL, &dwNameLength, 0)) {
    HandleError(env, "Error on CryptGetProvParam:1");
    return env.Undefined();
  }

  std::string name(dwNameLength, 0);
  if (!CryptGetProvParam(hCryptProv_, PP_CONTAINER, (BYTE *)name.c_str(), &dwNameLength, 0)) {
    HandleError(env, "Error on CryptGetProvParam:2");
    return env.Undefined();
  }

  return Napi::String::New(env, StringAcpToUtf8(name));
}
