#include "cryptopro.h"

Napi::Value GetCertificates(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsString()) {
    HandleArgumentError(env, "Store name (string) excepted");
    return env.Undefined();
  }
  Napi::String nsStoreName = info[0].ToString();

  HCERTSTORE hStore = CertOpenSystemStore(0, nsStoreName.Utf8Value().c_str());
  if (!hStore) {
    HandleError(env, "Error on CertOpenStore");
    return env.Undefined();
  }

  Napi::Array nCertificates = Napi::Array::New(env);

  uint16_t index = 0;
  PCCERT_CONTEXT pCertContext = NULL;
  while (pCertContext = CertEnumCertificatesInStore(hStore, pCertContext)) {
    nCertificates[index++] = Certificate::FromContext(env, CertDuplicateCertificateContext(pCertContext));
  }

  if (!CertCloseStore(hStore, 0)) {
    HandleError(env, "Error on CertCloseStore");
    return env.Undefined();
  }

  return nCertificates;
}

Napi::Value GetCertificateFromBuffer(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsBuffer()) {
    HandleArgumentError(env, "Certificate (Buffer) excepted");
    return env.Undefined();
  }
  Napi::Buffer<uint8_t> nbCertificate = info[0].As<Napi::Buffer<uint8_t>>();

  return Certificate::FromBuffer(env, nbCertificate);
}

Napi::Value Hash(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsString()) {
    HandleArgumentError(env, "Algorithm OID (string) excepted");
    return env.Undefined();
  }
  Napi::String nsAlgorithm = info[0].ToString();
  DWORD dwAlgId = CertOIDToAlgId(nsAlgorithm.Utf8Value().c_str());

  if (info.Length() < 2 || !info[1].IsBuffer()) {
    HandleArgumentError(env, "Data (Buffer) excepted");
    return env.Undefined();
  }
  Napi::Buffer<uint8_t> nbData = info[1].As<Napi::Buffer<uint8_t>>();

  HCRYPTPROV hCryptProv = 0;
  if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_GOST_2012_256, CRYPT_SILENT)) {
    HandleError(env, "Error on CryptAcquireContext");
    return env.Undefined();
  }

  HCRYPTHASH hHash = 0;
  if (!CryptCreateHash(hCryptProv, dwAlgId, 0, 0, &hHash)) {
    CryptReleaseContext(hCryptProv, 0);
    HandleError(env, "Error on CryptCreateHash");
    return env.Undefined();
  }

  if (!CryptHashData(hHash, nbData.Data(), nbData.Length(), 0)) {
    CryptDestroyHash(hHash);
    CryptReleaseContext(hCryptProv, 0);
    HandleError(env, "Error on CryptHashData");
    return env.Undefined();
  }

  DWORD dwHashLength = 0;
  DWORD dwHashLengthSize = sizeof(dwHashLength);
  if (!CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE *)&dwHashLength, &dwHashLengthSize, 0)) {
    CryptDestroyHash(hHash);
    CryptReleaseContext(hCryptProv, 0);
    HandleError(env, "Error on CryptGetHashParam:HP_HASHSIZE");
    return env.Undefined();
  }

  BYTE *bHashValue = (BYTE *)malloc(dwHashLength);
  if (!CryptGetHashParam(hHash, HP_HASHVAL, bHashValue, &dwHashLength, 0)) {
    CryptDestroyHash(hHash);
    CryptReleaseContext(hCryptProv, 0);
    HandleError(env, "Error on CryptGetHashParam:HP_HASHVAL");
    return env.Undefined();
  }

  Napi::Buffer<uint8_t> nbHash = Napi::Buffer<uint8_t>::New(env, bHashValue, dwHashLength);

  if (!CryptDestroyHash(hHash)) {
    CryptReleaseContext(hCryptProv, 0);
    HandleError(env, "Error on CryptDestroyHash");
    return env.Undefined();
  }

  if (!CryptReleaseContext(hCryptProv, 0)) {
    HandleError(env, "Error on CryptReleaseContext");
    return env.Undefined();
  }

  return nbHash;
}

Napi::Value SignHash(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsObject()) {
    HandleArgumentError(env, "Certificate excepted");
    return env.Undefined();
  }
  Certificate *nCertificate = Napi::ObjectWrap<Certificate>::Unwrap(info[0].As<Napi::Object>());
  PCCERT_CONTEXT pCertContext = nCertificate->Context();

  if (info.Length() < 2 || !info[1].IsString()) {
    HandleArgumentError(env, "Algorithm OID (string) excepted");
    return env.Undefined();
  }
  Napi::String nsAlgorithm = info[1].ToString();
  DWORD dwAlgId = CertOIDToAlgId(nsAlgorithm.Utf8Value().c_str());

  if (info.Length() < 3 || !info[2].IsBuffer()) {
    HandleArgumentError(env, "Hash (Buffer) excepted");
    return env.Undefined();
  }
  Napi::Buffer<uint8_t> nbHash = info[2].As<Napi::Buffer<uint8_t>>();

  HCRYPTPROV hCryptProv = 0;
  DWORD dwKeySpec = 0;
  if (!CryptAcquireCertificatePrivateKey(pCertContext, CRYPT_ACQUIRE_SILENT_FLAG, NULL, &hCryptProv, &dwKeySpec, FALSE)) {
    HandleError(env, "Error on CryptAcquireContext");
    return env.Undefined();
  }

  HCRYPTHASH hHash = 0;
  if (!CryptCreateHash(hCryptProv, dwAlgId, 0, 0, &hHash)) {
    CryptReleaseContext(hCryptProv, 0);
    HandleError(env, "Error on CryptCreateHash");
    return env.Undefined();
  }

  if (!CryptSetHashParam(hHash, HP_HASHVAL, nbHash.Data(), 0)) {
    CryptDestroyHash(hHash);
    CryptReleaseContext(hCryptProv, 0);
    HandleError(env, "Error on CryptSetHashParam");
    return env.Undefined();
  }

  DWORD dwSignatureLength;
  if (!CryptSignHash(hHash, dwKeySpec, NULL, 0, NULL, &dwSignatureLength)) {
    CryptDestroyHash(hHash);
    CryptReleaseContext(hCryptProv, 0);
    HandleError(env, "Error on CryptSignHash:1");
    return env.Undefined();
  }

  Napi::Buffer<uint8_t> nbSignature = Napi::Buffer<uint8_t>::New(env, dwSignatureLength);
  if (!CryptSignHash(hHash, dwKeySpec, NULL, 0, nbSignature.Data(), &dwSignatureLength)) {
    CryptDestroyHash(hHash);
    CryptReleaseContext(hCryptProv, 0);
    HandleError(env, "Error on CryptSignHash:2");
    return env.Undefined();
  }

  if (!CryptDestroyHash(hHash)) {
    CryptReleaseContext(hCryptProv, 0);
    HandleError(env, "Error on CryptDestroyHash");
    return env.Undefined();
  }

  if (!CryptReleaseContext(hCryptProv, 0)) {
    HandleError(env, "Error on CryptReleaseContext");
    return env.Undefined();
  }

  return nbSignature;
}

Napi::Value VerifyHashSignature(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsObject()) {
    HandleArgumentError(env, "Certificate excepted");
    return env.Undefined();
  }
  Certificate *nCertificate = Napi::ObjectWrap<Certificate>::Unwrap(info[0].As<Napi::Object>());
  PCCERT_CONTEXT pCertContext = nCertificate->Context();

  if (info.Length() < 2 || !info[1].IsString()) {
    HandleArgumentError(env, "Algorithm OID (string) excepted");
    return env.Undefined();
  }
  Napi::String nsAlgorithm = info[1].ToString();
  DWORD dwAlgId = CertOIDToAlgId(nsAlgorithm.Utf8Value().c_str());

  if (info.Length() < 3 || !info[2].IsBuffer()) {
    HandleArgumentError(env, "Hash (Buffer) excepted");
    return env.Undefined();
  }
  Napi::Buffer<uint8_t> nbHash = info[2].As<Napi::Buffer<uint8_t>>();

  if (info.Length() < 4 || !info[3].IsBuffer()) {
    HandleArgumentError(env, "Signature (Buffer) excepted");
    return env.Undefined();
  }
  Napi::Buffer<uint8_t> nbSignature = info[3].As<Napi::Buffer<uint8_t>>();

  HCRYPTPROV hCryptProv = 0;
  if (!CryptAcquireCertificatePrivateKey(pCertContext, CRYPT_ACQUIRE_SILENT_FLAG, NULL, &hCryptProv, NULL, FALSE)) {
    HandleError(env, "Error on CryptAcquireContext");
    return env.Undefined();
  }

  HCRYPTHASH hHash = 0;
  if (!CryptCreateHash(hCryptProv, dwAlgId, 0, 0, &hHash)) {
    CryptReleaseContext(hCryptProv, 0);
    HandleError(env, "Error on CryptCreateHash");
    return env.Undefined();
  }

  if (!CryptSetHashParam(hHash, HP_HASHVAL, nbHash.Data(), 0)) {
    CryptDestroyHash(hHash);
    CryptReleaseContext(hCryptProv, 0);
    HandleError(env, "Error on CryptSetHashParam");
    return env.Undefined();
  }

  HCRYPTKEY hPubKey = 0;
  if (!CryptImportPublicKeyInfo(hCryptProv, pCertContext->dwCertEncodingType, &pCertContext->pCertInfo->SubjectPublicKeyInfo, &hPubKey)) {
    CryptDestroyHash(hHash);
    CryptReleaseContext(hCryptProv, 0);
    HandleError(env, "Error on CryptImportPublicKeyInfo");
    return env.Undefined();
  }

  bool bResult = CryptVerifySignature(hHash, nbSignature.Data(), nbSignature.Length(), hPubKey, NULL, 0);

  if (!CryptDestroyHash(hHash)) {
    CryptReleaseContext(hCryptProv, 0);
    HandleError(env, "Error on CryptDestroyHash");
    return env.Undefined();
  }

  if (!CryptReleaseContext(hCryptProv, 0)) {
    HandleError(env, "Error on CryptReleaseContext");
    return env.Undefined();
  }

  return Napi::Boolean::New(env, bResult);
}

Napi::Value EncryptMessage(const Napi::CallbackInfo& info) {
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

  CRYPT_ENCRYPT_MESSAGE_PARA cryptEncryptMessagePara;
  memset(&cryptEncryptMessagePara, 0, sizeof(CRYPT_ENCRYPT_MESSAGE_PARA));
  cryptEncryptMessagePara.cbSize =  sizeof(cryptEncryptMessagePara);
  cryptEncryptMessagePara.dwMsgEncodingType = X509_ASN_ENCODING | PKCS_7_ASN_ENCODING;
  cryptEncryptMessagePara.ContentEncryptionAlgorithm.pszObjId = szOID_CP_GOST_28147;

  DWORD dwEncryptedMessageLength = 0;
  if (!CryptEncryptMessage(&cryptEncryptMessagePara, 1, &pCertContext, nbMessage.Data(), nbMessage.Length(), NULL, &dwEncryptedMessageLength)) {
    HandleError(env, "Error on CryptEncryptMessage:1");
    return env.Undefined();
  }

  Napi::Buffer<uint8_t> nbEncryptedMessage = Napi::Buffer<uint8_t>::New(env, dwEncryptedMessageLength);
  if (!CryptEncryptMessage(&cryptEncryptMessagePara, 1, &pCertContext, nbMessage.Data(), nbMessage.Length(), nbEncryptedMessage.Data(), &dwEncryptedMessageLength)) {
    HandleError(env, "Error on CryptEncryptMessage:2");
    return env.Undefined();
  }

  return nbEncryptedMessage;
}

Napi::Value DecryptMessage(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsBuffer()) {
    HandleArgumentError(env, "Message (Buffer) excepted");
    return env.Undefined();
  }
  Napi::Buffer<uint8_t> nbMessage = info[0].As<Napi::Buffer<uint8_t>>();

  HCERTSTORE hStore = CertOpenSystemStore(0, "MY");

  CRYPT_DECRYPT_MESSAGE_PARA cryptDecryptMessagePara = { sizeof(cryptDecryptMessagePara) };
  cryptDecryptMessagePara.dwMsgAndCertEncodingType = X509_ASN_ENCODING | PKCS_7_ASN_ENCODING;
  cryptDecryptMessagePara.cCertStore = 1;
  cryptDecryptMessagePara.rghCertStore = &hStore;

  PCCERT_CONTEXT pCertContext = NULL;
  DWORD dwDecryptedMessageLength = 0;
  if (!CryptDecryptMessage(&cryptDecryptMessagePara, nbMessage.Data(), nbMessage.Length(), NULL, &dwDecryptedMessageLength, &pCertContext)) {
    HandleError(env, "Error on CryptDecryptMessage:1");
    return env.Undefined();
  }

  Napi::Buffer<uint8_t> nbDecryptedMessage = Napi::Buffer<uint8_t>::New(env, dwDecryptedMessageLength);
  if (!CryptDecryptMessage(&cryptDecryptMessagePara, nbMessage.Data(), nbMessage.Length(), nbDecryptedMessage.Data(), &dwDecryptedMessageLength, &pCertContext)) {
    HandleError(env, "Error on CryptDecryptMessage:2");
    return env.Undefined();
  }

  Napi::Object nResult = Napi::Object::New(env);
  nResult.Set("data", nbDecryptedMessage);

  if (pCertContext) {
    nResult.Set("certificate", Certificate::FromContext(env, pCertContext));
  }

  return nResult;
}

Napi::Value SignMessage(const Napi::CallbackInfo& info) {
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
  char *pHashAlgorithmOid = szOID_CP_GOST_R3411_12_256;
  if (info.Length() >= 3 && info[2].IsObject()) {
    Napi::Object nOptions = info[2].ToObject();
    if (nOptions.Get("isDetached").IsBoolean()) {
      bIsDetached = nOptions.Get("isDetached").ToBoolean();
    }
    if (nOptions.Get("hashAlgorithm").IsString()) {
      pHashAlgorithmOid = (char *)nOptions.Get("hashAlgorithm").ToString().Utf8Value().c_str();
    }
  }

  CRYPT_SIGN_MESSAGE_PARA cryptSignMessagePara = { sizeof(cryptSignMessagePara) };
  cryptSignMessagePara.dwMsgEncodingType = X509_ASN_ENCODING | PKCS_7_ASN_ENCODING;
  cryptSignMessagePara.pSigningCert = pCertContext;
  cryptSignMessagePara.HashAlgorithm.pszObjId = pHashAlgorithmOid;

  cryptSignMessagePara.rgpMsgCert = &pCertContext;
  cryptSignMessagePara.cMsgCert = 1;

  const BYTE *tmpMessageArray[] = { nbMessage.Data() };
  DWORD tmpMessageLengthArray[] = { (DWORD)nbMessage.Length() };

  DWORD dwSignatureLength = 0;
  if (!CryptSignMessage(&cryptSignMessagePara, bIsDetached, 1, tmpMessageArray, tmpMessageLengthArray, NULL, &dwSignatureLength)) {
    HandleError(env, "Error on CryptSignMessage:1");
    return env.Undefined();
  }

  Napi::Buffer<uint8_t> nbSignature = Napi::Buffer<uint8_t>::New(env, dwSignatureLength);
  if (!CryptSignMessage(&cryptSignMessagePara, bIsDetached, 1, tmpMessageArray, tmpMessageLengthArray, nbSignature.Data(), &dwSignatureLength)) {
    HandleError(env, "Error on CryptSignMessage:2");
    return env.Undefined();
  }

  return nbSignature;
}

Napi::Value VerifyMessageSignature(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsBuffer()) {
    HandleArgumentError(env, "Message (Buffer) excepted");
    return env.Undefined();
  }
  Napi::Buffer<uint8_t> nbMessage = info[0].As<Napi::Buffer<uint8_t>>();

  CRYPT_VERIFY_MESSAGE_PARA cryptVerifyMessagePara = { sizeof (CRYPT_VERIFY_MESSAGE_PARA) };
  cryptVerifyMessagePara.dwMsgAndCertEncodingType = X509_ASN_ENCODING | PKCS_7_ASN_ENCODING;

  // Проверка подписи
  DWORD dwDataLength = 0;
  PCCERT_CONTEXT pCertContext = NULL;
  bool bIsValid = CryptVerifyMessageSignature(&cryptVerifyMessagePara, 0, nbMessage.Data(), nbMessage.Length(), NULL, &dwDataLength, &pCertContext);

  Napi::Buffer<uint8_t> nbData = Napi::Buffer<uint8_t>::New(env, dwDataLength);
  CryptVerifyMessageSignature(&cryptVerifyMessagePara, 0, nbMessage.Data(), nbMessage.Length(), nbData.Data(), &dwDataLength, &pCertContext);

  Napi::Object nVerifyResult = Napi::Object::New(env);

  nVerifyResult.Set("isValid", bIsValid);
  nVerifyResult.Set("certificate", Certificate::FromContext(env, pCertContext));

  if (dwDataLength > 0) {
    nVerifyResult.Set("data", nbData);
  }

  return nVerifyResult;
}

Napi::Value VerifyDetachedMessageSignature(const Napi::CallbackInfo& info) {
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

  CRYPT_VERIFY_MESSAGE_PARA cryptVerifyMessagePara = { sizeof(cryptVerifyMessagePara) };
  cryptVerifyMessagePara.dwMsgAndCertEncodingType = X509_ASN_ENCODING | PKCS_7_ASN_ENCODING;

  const BYTE *tmpMessageArray[] = { nbMessage.Data() };
  DWORD tmpMessageLengthArray[] = { (DWORD)nbMessage.Length() };

  // Проверка подписи
  PCCERT_CONTEXT pCertContext = NULL;
  bool bIsValid = CryptVerifyDetachedMessageSignature(&cryptVerifyMessagePara, 0, nbSignature.Data(), nbSignature.Length(), 1, tmpMessageArray, tmpMessageLengthArray, &pCertContext);

  Napi::Object nResult = Napi::Object::New(env);

  nResult.Set("isValid", Napi::Boolean::New(env, bIsValid));

  if (bIsValid) {
    nResult.Set("certificate", Certificate::FromContext(env, pCertContext));
  }

  return nResult;
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  Certificate::Init(env, exports);
  Provider::Init(env, exports);
  CAdES::Init(env, exports);
  XAdES::Init(env, exports);

  exports.Set(Napi::String::New(env, "getCertificates"), Napi::Function::New(env, GetCertificates));
  exports.Set(Napi::String::New(env, "getCertificateFromBuffer"), Napi::Function::New(env, GetCertificateFromBuffer));

  exports.Set(Napi::String::New(env, "hash"), Napi::Function::New(env, Hash));
  exports.Set(Napi::String::New(env, "signHash"), Napi::Function::New(env, SignHash));
  exports.Set(Napi::String::New(env, "verifyHashSignature"), Napi::Function::New(env, VerifyHashSignature));

  exports.Set(Napi::String::New(env, "encryptMessage"), Napi::Function::New(env, EncryptMessage));
  exports.Set(Napi::String::New(env, "decryptMessage"), Napi::Function::New(env, DecryptMessage));

  exports.Set(Napi::String::New(env, "signMessage"), Napi::Function::New(env, SignMessage));
  exports.Set(Napi::String::New(env, "verifyMessageSignature"), Napi::Function::New(env, VerifyMessageSignature));
  exports.Set(Napi::String::New(env, "verifyDetachedMessageSignature"), Napi::Function::New(env, VerifyDetachedMessageSignature));

  exports.Set(Napi::String::New(env, "OID_CP_GOST_R3411"), Napi::String::New(env, szOID_CP_GOST_R3411));
  exports.Set(Napi::String::New(env, "OID_CP_GOST_R3411_12_256"), Napi::String::New(env, szOID_CP_GOST_R3411_12_256));
  exports.Set(Napi::String::New(env, "OID_CP_GOST_R3411_12_512"), Napi::String::New(env, szOID_CP_GOST_R3411_12_512));

  return exports;
}

NODE_API_MODULE(cryptopro, InitAll)
