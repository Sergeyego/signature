#include "xades.h"

void XAdES::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Object object = Napi::Object::New(env);
  object.Set(Napi::String::New(env, "signMessage"), Napi::Function::New(env, XAdES::SignMessage));
  object.Set(Napi::String::New(env, "verifyMessageSignature"), Napi::Function::New(env, XAdES::VerifyMessageSignature));

  object.Set(Napi::String::New(env, "XML_XADES_SIGNATURE_TYPE_ENVELOPED"), Napi::Number::New(env, XML_XADES_SIGNATURE_TYPE_ENVELOPED));
  object.Set(Napi::String::New(env, "XML_XADES_SIGNATURE_TYPE_ENVELOPING"), Napi::Number::New(env, XML_XADES_SIGNATURE_TYPE_ENVELOPING));
  object.Set(Napi::String::New(env, "XML_XADES_SIGNATURE_TYPE_TEMPLATE"), Napi::Number::New(env, XML_XADES_SIGNATURE_TYPE_TEMPLATE));

  object.Set(Napi::String::New(env, "XADES_BES"), Napi::Number::New(env, XADES_BES));
  object.Set(Napi::String::New(env, "XADES_T"), Napi::Number::New(env, XADES_T));
  object.Set(Napi::String::New(env, "XADES_X_LONG_TYPE_1"), Napi::Number::New(env, XADES_X_LONG_TYPE_1));
  object.Set(Napi::String::New(env, "XADES_XMLDSIG"), Napi::Number::New(env, XADES_XMLDSIG));

  exports.Set("XAdES", object);
}

Napi::Value XAdES::SignMessage(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsObject()) {
    HandleArgumentError(env, "Certificate excepted");
    return env.Undefined();
  }
  Certificate *nCertificate = Napi::ObjectWrap<Certificate>::Unwrap(info[0].As<Napi::Object>());
  PCCERT_CONTEXT pCertContext = nCertificate->Context();

  if (info.Length() < 2 || !info[1].IsBuffer()) {
    HandleArgumentError(env, "Message (Buffer) excepted");
    return env.Undefined();
  }
  Napi::Buffer<uint8_t> nbMessage = info[1].As<Napi::Buffer<uint8_t>>();

  char *pcXPath = NULL;
  bool bIsDetached = false;
  DWORD dwSignatureType = 0;
  if (info.Length() >= 3 && info[2].IsObject()) {
    Napi::Object nOptions = info[2].ToObject();
    if (nOptions.Get("xpath").IsString()) {
      pcXPath = (char *)nOptions.Get("xpath").ToString().Utf8Value().c_str();
    }
    if (nOptions.Get("isDetached").IsBoolean()) {
      bIsDetached = nOptions.Get("isDetached").ToBoolean();
    }
    if (nOptions.Get("type").IsNumber()) {
      dwSignatureType = nOptions.Get("type").ToNumber().Uint32Value();
    }
  }

  XADES_SIGN_PARA xadesSignPara = { sizeof(xadesSignPara) };
  xadesSignPara.dwSignatureType = dwSignatureType;
  xadesSignPara.pSignerCert = pCertContext;

  XADES_SIGN_MESSAGE_PARA xadesSignMessagePara = { sizeof(xadesSignMessagePara) };
  xadesSignMessagePara.pXadesSignPara = &xadesSignPara;

  PCRYPT_DATA_BLOB pSignedMessage = NULL;
  if (!XadesSign(&xadesSignMessagePara, pcXPath, bIsDetached, nbMessage.Data(), nbMessage.Length(), &pSignedMessage)) {
    HandleError(env, "Error on XadesSign");
    return env.Undefined();
  }

  Napi::Buffer<uint8_t> nbSignedXml = Napi::Buffer<uint8_t>::Copy(env, pSignedMessage->pbData, pSignedMessage->cbData);

  if (!XadesFreeBlob(pSignedMessage)) {
    HandleError(env, "Error on XadesFreeBlob");
    return env.Undefined();
  }

  return nbSignedXml;
}

Napi::Value XAdES::VerifyMessageSignature(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsBuffer()) {
    HandleArgumentError(env, "Message (Buffer) excepted");
    return env.Undefined();
  }
  Napi::Buffer<uint8_t> nbMessage = info[0].As<Napi::Buffer<uint8_t>>();

  char *pcXPath = NULL;
  DWORD dwSignatureType = 0;
  if (info.Length() >= 2 && info[1].IsObject()) {
    Napi::Object nOptions = info[1].ToObject();
    if (nOptions.Get("xpath").IsString()) {
      pcXPath = (char *)nOptions.Get("xpath").ToString().Utf8Value().c_str();
    }
    if (nOptions.Get("type").IsNumber()) {
      dwSignatureType = nOptions.Get("type").ToNumber().Uint32Value();
    }
  }

  XADES_VERIFICATION_PARA xadesVerificationPara = { sizeof(xadesVerificationPara) };
  xadesVerificationPara.dwSignatureType = dwSignatureType;

  XADES_VERIFY_MESSAGE_PARA xadesVerifyMessagePara = { sizeof(xadesVerifyMessagePara) };
  xadesVerifyMessagePara.pXadesVerifyPara = &xadesVerificationPara;

  PXADES_VERIFICATION_INFO_ARRAY pVerifyInfo = NULL;
  if (!XadesVerify(&xadesVerifyMessagePara, pcXPath, nbMessage.Data(), nbMessage.Length(), &pVerifyInfo)) {
    XadesFreeVerificationInfoArray(pVerifyInfo);
    HandleError(env, "Error on XadesVerify");
    return env.Undefined();
  }

  Napi::Array naVerifyResults = Napi::Array::New(env, pVerifyInfo->cbCount);

  for (uint16_t i = 0; i < pVerifyInfo->cbCount; i++) {
    Napi::Object nVerifyResult = Napi::Object::New(env);

    nVerifyResult.Set(Napi::String::New(env, "isValid"), Napi::Boolean::New(env, pVerifyInfo->pXadesVerificationInfo[i].dwStatus == XADES_VERIFY_SUCCESS));
    nVerifyResult.Set(Napi::String::New(env, "type"), pVerifyInfo->pXadesVerificationInfo[i].dwVerifiedXadesType);

    if (pVerifyInfo->pXadesVerificationInfo[i].pSignerCert) {
      nVerifyResult.Set(Napi::String::New(env, "certificate"), Certificate::FromContext(env, CertDuplicateCertificateContext(pVerifyInfo->pXadesVerificationInfo[i].pSignerCert)));
    }
    if (pVerifyInfo->pXadesVerificationInfo[i].pSigningTime) {
      nVerifyResult.Set(Napi::String::New(env, "signingTime"), Napi::Date::New(env, FileTimeToUnixTimeMs(*pVerifyInfo->pXadesVerificationInfo[i].pSigningTime)));
    }
    if (pVerifyInfo->pXadesVerificationInfo[i].pSignatureTimeStampTime) {
      nVerifyResult.Set(Napi::String::New(env, "signatureTimeStampTime"), Napi::Date::New(env, FileTimeToUnixTimeMs(*pVerifyInfo->pXadesVerificationInfo[i].pSignatureTimeStampTime)));
    }
    if (pVerifyInfo->pXadesVerificationInfo[i].pSigAndRefsTimeStampTime) {
      nVerifyResult.Set(Napi::String::New(env, "sigAndRefsTimeStampTime"), Napi::Date::New(env, FileTimeToUnixTimeMs(*pVerifyInfo->pXadesVerificationInfo[i].pSigAndRefsTimeStampTime)));
    }

    naVerifyResults[i] = nVerifyResult;
  }

  if (!XadesFreeVerificationInfoArray(pVerifyInfo)) {
    HandleError(env, "Error on XadesFreeVerificationInfoArray");
    return env.Undefined();
  }

  return naVerifyResults;
}
