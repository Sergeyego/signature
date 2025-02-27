[n-cryptopro](../README.md) / [Exports](../modules.md) / [XAdES](../modules/XAdES.md) / XAdESVerifySignatureResult

# Interface: XAdESVerifySignatureResult

[XAdES](../modules/XAdES.md).XAdESVerifySignatureResult

## Hierarchy

- [`VerifySignatureResult`](VerifySignatureResult.md)

  ↳ **`XAdESVerifySignatureResult`**

## Table of contents

### Properties

- [certificate](XAdES.XAdESVerifySignatureResult.md#certificate)
- [isValid](XAdES.XAdESVerifySignatureResult.md#isvalid)
- [sigAndRefsTimeStampTime](XAdES.XAdESVerifySignatureResult.md#sigandrefstimestamptime)
- [signatureTimeStampTime](XAdES.XAdESVerifySignatureResult.md#signaturetimestamptime)
- [signingTime](XAdES.XAdESVerifySignatureResult.md#signingtime)
- [type](XAdES.XAdESVerifySignatureResult.md#type)

## Properties

### certificate

• **certificate**: [`Certificate`](Certificate.md)

Сертификат

#### Inherited from

[VerifySignatureResult](VerifySignatureResult.md).[certificate](VerifySignatureResult.md#certificate)

___

### isValid

• **isValid**: `boolean`

Подпись действительна

#### Inherited from

[VerifySignatureResult](VerifySignatureResult.md).[isValid](VerifySignatureResult.md#isvalid)

___

### sigAndRefsTimeStampTime

• **sigAndRefsTimeStampTime**: `Date`

___

### signatureTimeStampTime

• **signatureTimeStampTime**: `Date`

___

### signingTime

• **signingTime**: `Date`

___

### type

• **type**: `number`

Тип подписи (напр. CryptoPro.XAdES.XADES_BES)
