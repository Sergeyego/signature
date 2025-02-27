[n-cryptopro](../README.md) / [Exports](../modules.md) / XAdES

# Namespace: XAdES

## Table of contents

### Interfaces

- [XAdESSignOptions](../interfaces/XAdES.XAdESSignOptions.md)
- [XAdESVerifySignatureOptions](../interfaces/XAdES.XAdESVerifySignatureOptions.md)
- [XAdESVerifySignatureResult](../interfaces/XAdES.XAdESVerifySignatureResult.md)

### Variables

- [XADES\_BES](XAdES.md#xades_bes)
- [XADES\_T](XAdES.md#xades_t)
- [XADES\_XMLDSIG](XAdES.md#xades_xmldsig)
- [XADES\_X\_LONG\_TYPE\_1](XAdES.md#xades_x_long_type_1)
- [XML\_XADES\_SIGNATURE\_TYPE\_ENVELOPED](XAdES.md#xml_xades_signature_type_enveloped)
- [XML\_XADES\_SIGNATURE\_TYPE\_ENVELOPING](XAdES.md#xml_xades_signature_type_enveloping)
- [XML\_XADES\_SIGNATURE\_TYPE\_TEMPLATE](XAdES.md#xml_xades_signature_type_template)

### Functions

- [signMessage](XAdES.md#signmessage)
- [verifyMessageSignature](XAdES.md#verifymessagesignature)

## Variables

### XADES\_BES

• `Const` **XADES\_BES**: `number`

Тип сообщения XAdES BES

___

### XADES\_T

• `Const` **XADES\_T**: `number`

Тип сообщения XAdES-T

___

### XADES\_XMLDSIG

• `Const` **XADES\_XMLDSIG**: `number`

Тип сообщения XMLDSIG

___

### XADES\_X\_LONG\_TYPE\_1

• `Const` **XADES\_X\_LONG\_TYPE\_1**: `number`

Тип сообщения XAdES-X Long Type 1

___

### XML\_XADES\_SIGNATURE\_TYPE\_ENVELOPED

• `Const` **XML\_XADES\_SIGNATURE\_TYPE\_ENVELOPED**: `number`

XML_XADES_SIGNATURE_TYPE_ENVELOPED

___

### XML\_XADES\_SIGNATURE\_TYPE\_ENVELOPING

• `Const` **XML\_XADES\_SIGNATURE\_TYPE\_ENVELOPING**: `number`

XML_XADES_SIGNATURE_TYPE_ENVELOPING

___

### XML\_XADES\_SIGNATURE\_TYPE\_TEMPLATE

• `Const` **XML\_XADES\_SIGNATURE\_TYPE\_TEMPLATE**: `number`

XML_XADES_SIGNATURE_TYPE_TEMPLATE

## Functions

### signMessage

▸ **signMessage**(`certificate`, `message`, `options?`): `Buffer`

Подписание сообщения

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `certificate` | [`Certificate`](../interfaces/Certificate.md) | Сертификат |
| `message` | `Buffer` | Буфер с сообщением |
| `options?` | [`XAdESSignOptions`](../interfaces/XAdES.XAdESSignOptions.md) | Доп. параметры |

#### Returns

`Buffer`

___

### verifyMessageSignature

▸ **verifyMessageSignature**(`message`, `options?`): [`XAdESVerifySignatureResult`](../interfaces/XAdES.XAdESVerifySignatureResult.md)[]

Проверка подписи сообщения

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `message` | `Buffer` | Буфер с подписаным сообщением |
| `options?` | [`XAdESVerifySignatureOptions`](../interfaces/XAdES.XAdESVerifySignatureOptions.md) | Доп. параметры |

#### Returns

[`XAdESVerifySignatureResult`](../interfaces/XAdES.XAdESVerifySignatureResult.md)[]
