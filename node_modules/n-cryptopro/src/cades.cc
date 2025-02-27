#include "cades.h"

void CAdES::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Object object = Napi::Object::New(env);
  object.Set(Napi::String::New(env, "signMessage"), Napi::Function::New(env, CAdES::SignMessage));
  object.Set(Napi::String::New(env, "verifyMessageSignature"), Napi::Function::New(env, CAdES::VerifyMessageSignature));
  object.Set(Napi::String::New(env, "verifyDetachedMessageSignature"), Napi::Function::New(env, CAdES::VerifyDetachedMessageSignature));

  object.Set(Napi::String::New(env, "CADES_BES"), Napi::Number::New(env, CADES_BES));
  object.Set(Napi::String::New(env, "CADES_T"), Napi::Number::New(env, CADES_T));
  object.Set(Napi::String::New(env, "CADES_X_LONG_TYPE_1"), Napi::Number::New(env, CADES_X_LONG_TYPE_1));

  exports.Set("CAdES", object);
}

Napi::Value CAdES::SignMessage(const Napi::CallbackInfo& info) {
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

  bool bIsDetached = false;
  DWORD dwSignatureType = 0;
  char *pHashAlgorithmOid = szOID_CP_GOST_R3411_12_256;
  if (info.Length() >= 3 && info[2].IsObject()) {
    Napi::Object nOptions = info[2].ToObject();
    if (nOptions.Get("isDetached").IsBoolean()) {
      bIsDetached = nOptions.Get("isDetached").ToBoolean();
    }
    if (nOptions.Get("type").IsNumber()) {
      dwSignatureType = nOptions.Get("type").ToNumber().Uint32Value();
    }
    if (nOptions.Get("hashAlgorithm").IsString()) {
      pHashAlgorithmOid = (char *)nOptions.Get("hashAlgorithm").ToString().Utf8Value().c_str();
    }
  }

  CRYPT_SIGN_MESSAGE_PARA cryptSignMessagePara = { sizeof(cryptSignMessagePara) };
  cryptSignMessagePara.dwMsgEncodingType = X509_ASN_ENCODING | PKCS_7_ASN_ENCODING;
  cryptSignMessagePara.pSigningCert = pCertContext;
  cryptSignMessagePara.HashAlgorithm.pszObjId = pHashAlgorithmOid;

  CADES_SIGN_PARA cadesSignPara = { sizeof(cadesSignPara) };
  cadesSignPara.dwCadesType = dwSignatureType;

  CADES_SIGN_MESSAGE_PARA cadesSignMessagePara = { sizeof(cadesSignMessagePara) };
  cadesSignMessagePara.pSignMessagePara = &cryptSignMessagePara;
  cadesSignMessagePara.pCadesSignPara = &cadesSignPara;

  const BYTE *tmpMessageArray[] = { nbMessage.Data() };
  DWORD tmpMessageLengthArray[] = { (DWORD)nbMessage.Length() };

  PCRYPT_DATA_BLOB pSignedMessage = 0;
  if(!CadesSignMessage(&cadesSignMessagePara, bIsDetached, 1, tmpMessageArray, tmpMessageLengthArray, &pSignedMessage)) {
    HandleError(env, "Error on CadesSignMessage");
    return env.Undefined();
  }

  Napi::Buffer<uint8_t> nbSignature = Napi::Buffer<uint8_t>::Copy(env, pSignedMessage->pbData, pSignedMessage->cbData);

  if (!CadesFreeBlob(pSignedMessage)) {
    HandleError(env, "Error on CadesFreeBlob");
    return env.Undefined();
  }

  return nbSignature;
}

Napi::Value CAdES::VerifyMessageSignature(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsBuffer()) {
    HandleArgumentError(env, "Message (Buffer) excepted");
    return env.Undefined();
  }
  Napi::Buffer<uint8_t> nbMessage = info[0].As<Napi::Buffer<uint8_t>>();

  DWORD dwSignatureType = 0;
  if (info.Length() >= 2 && info[1].IsObject()) {
    Napi::Object nOptions = info[1].ToObject();
    if (nOptions.Get("type").IsNumber()) {
      dwSignatureType = nOptions.Get("type").ToNumber().Uint32Value();
    }
  }

  CRYPT_VERIFY_MESSAGE_PARA cryptVerifyMessagePara = { sizeof (CRYPT_VERIFY_MESSAGE_PARA) };
  cryptVerifyMessagePara.dwMsgAndCertEncodingType = X509_ASN_ENCODING | PKCS_7_ASN_ENCODING;

  CADES_VERIFICATION_PARA cadesVerifyPara = { sizeof(cadesVerifyPara) };
  cadesVerifyPara.dwCadesType = dwSignatureType;

  CADES_VERIFY_MESSAGE_PARA cadesVerifyMessagePara = { sizeof(cadesVerifyMessagePara) };
  cadesVerifyMessagePara.pVerifyMessagePara = &cryptVerifyMessagePara;
  cadesVerifyMessagePara.pCadesVerifyPara = &cadesVerifyPara;

  PCADES_VERIFICATION_INFO pVerifyInfo = NULL;
  PCRYPT_DATA_BLOB pContent = NULL;
  if (!CadesVerifyMessage(&cadesVerifyMessagePara, 0, nbMessage.Data(), nbMessage.Length(), &pContent, &pVerifyInfo)) {
    CadesFreeVerificationInfo(pVerifyInfo);
    HandleError(env, "Error on CadesVerifyMessage");
    return env.Undefined();
  }

  Napi::Object nVerifyResult = Napi::Object::New(env);

  nVerifyResult.Set(Napi::String::New(env, "isValid"), Napi::Boolean::New(env, pVerifyInfo->dwStatus == CADES_VERIFY_SUCCESS));

  if (pVerifyInfo->pSignerCert) {
    nVerifyResult.Set(Napi::String::New(env, "certificate"), Certificate::FromContext(env, CertDuplicateCertificateContext(pVerifyInfo->pSignerCert)));
  }
  if (pVerifyInfo->pSigningTime) {
    nVerifyResult.Set(Napi::String::New(env, "signingTime"), Napi::Date::New(env, FileTimeToUnixTimeMs(*pVerifyInfo->pSigningTime)));
  }
  if (pVerifyInfo->pSignatureTimeStampTime) {
    nVerifyResult.Set(Napi::String::New(env, "signatureTimeStampTime"), Napi::Date::New(env, FileTimeToUnixTimeMs(*pVerifyInfo->pSignatureTimeStampTime)));
  }

  nVerifyResult.Set(Napi::String::New(env, "data"), Napi::Buffer<uint8_t>::Copy(env, pContent->pbData, pContent->cbData));

  if(!CadesFreeVerificationInfo(pVerifyInfo)) {
    CadesFreeBlob(pContent);
    HandleError(env, "Error on CadesFreeVerificationInfo");
    return env.Undefined();
  }

  if(!CadesFreeBlob(pContent)) {
    HandleError(env, "Error on CadesFreeBlob");
    return env.Undefined();
  }

  return nVerifyResult;
}

Napi::Value CAdES::VerifyDetachedMessageSignature(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsBuffer()) {
    HandleArgumentError(env, "Signature (Buffer) excepted");
    return env.Undefined();
  }
  Napi::Buffer<uint8_t> nbSignature = info[0].As<Napi::Buffer<uint8_t>>();

  if (info.Length() < 2 || !info[1].IsBuffer()) {
    HandleArgumentError(env, "Message (Buffer) excepted");
    return env.Undefined();
  }
  Napi::Buffer<uint8_t> nbMessage = info[1].As<Napi::Buffer<uint8_t>>();

  DWORD dwSignatureType = 0;
  if (info.Length() >= 3 && info[2].IsObject()) {
    Napi::Object nOptions = info[2].ToObject();
    if (nOptions.Get("type").IsNumber()) {
      dwSignatureType = nOptions.Get("type").ToNumber().Uint32Value();
    }
  }

  CRYPT_VERIFY_MESSAGE_PARA cryptVerifyMessagePara = { sizeof (CRYPT_VERIFY_MESSAGE_PARA) };
  cryptVerifyMessagePara.dwMsgAndCertEncodingType = X509_ASN_ENCODING | PKCS_7_ASN_ENCODING;

  CADES_VERIFICATION_PARA cadesVerifyPara = { sizeof(cadesVerifyPara) };
  cadesVerifyPara.dwCadesType = dwSignatureType;

  CADES_VERIFY_MESSAGE_PARA cadesVerifyMessagePara = { sizeof(cadesVerifyMessagePara) };
  cadesVerifyMessagePara.pVerifyMessagePara = &cryptVerifyMessagePara;
  cadesVerifyMessagePara.pCadesVerifyPara = &cadesVerifyPara;

  const BYTE *tmpMessageArray[] = { nbMessage.Data() };
  DWORD tmpMessageLengthArray[] = { (DWORD)nbMessage.Length() };

  PCADES_VERIFICATION_INFO pVerifyInfo = NULL;
  if (!CadesVerifyDetachedMessage(&cadesVerifyMessagePara, 0, nbSignature.Data(), nbSignature.Length(), 1, tmpMessageArray, tmpMessageLengthArray, &pVerifyInfo)) {
    CadesFreeVerificationInfo(pVerifyInfo);
    HandleError(env, "Error on CadesVerifyMessage");
    return env.Undefined();
  }

  Napi::Object nVerifyResult = Napi::Object::New(env);

  nVerifyResult.Set(Napi::String::New(env, "isValid"), Napi::Boolean::New(env, pVerifyInfo->dwStatus == CADES_VERIFY_SUCCESS));

  if (pVerifyInfo->pSignerCert) {
    nVerifyResult.Set(Napi::String::New(env, "certificate"), Certificate::FromContext(env, CertDuplicateCertificateContext(pVerifyInfo->pSignerCert)));
  }
  if (pVerifyInfo->pSigningTime) {
    nVerifyResult.Set(Napi::String::New(env, "signingTime"), Napi::Date::New(env, FileTimeToUnixTimeMs(*pVerifyInfo->pSigningTime)));
  }
  if (pVerifyInfo->pSignatureTimeStampTime) {
    nVerifyResult.Set(Napi::String::New(env, "signatureTimeStampTime"), Napi::Date::New(env, FileTimeToUnixTimeMs(*pVerifyInfo->pSignatureTimeStampTime)));
  }

  if(!CadesFreeVerificationInfo(pVerifyInfo)) {
    HandleError(env, "Error on CadesFreeVerificationInfo");
    return env.Undefined();
  }

  return nVerifyResult;
}
