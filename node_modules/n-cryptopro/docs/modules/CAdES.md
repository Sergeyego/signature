[n-cryptopro](../README.md) / [Exports](../modules.md) / CAdES

# Namespace: CAdES

## Table of contents

### Interfaces

- [CAdESSignOptions](../interfaces/CAdES.CAdESSignOptions.md)
- [CAdESVerifyAttachedSignatureResult](../interfaces/CAdES.CAdESVerifyAttachedSignatureResult.md)
- [CAdESVerifySignatureOptions](../interfaces/CAdES.CAdESVerifySignatureOptions.md)
- [CAdESVerifySignatureResult](../interfaces/CAdES.CAdESVerifySignatureResult.md)

### Variables

- [CADES\_BES](CAdES.md#cades_bes)
- [CADES\_T](CAdES.md#cades_t)
- [CADES\_X\_LONG\_TYPE\_1](CAdES.md#cades_x_long_type_1)

### Functions

- [signMessage](CAdES.md#signmessage)
- [verifyDetachedMessageSignature](CAdES.md#verifydetachedmessagesignature)
- [verifyMessageSignature](CAdES.md#verifymessagesignature)

## Variables

### CADES\_BES

• `Const` **CADES\_BES**: `number`

Тип сообщения CAdES BES

___

### CADES\_T

• `Const` **CADES\_T**: `number`

Тип сообщения CAdES-T

___

### CADES\_X\_LONG\_TYPE\_1

• `Const` **CADES\_X\_LONG\_TYPE\_1**: `number`

Тип сообщения CAdES-X Long Type 1

## Functions

### signMessage

▸ **signMessage**(`certificate`, `message`, `options?`): `Buffer`

Подписание сообщения

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `certificate` | [`Certificate`](../interfaces/Certificate.md) | Сертификат |
| `message` | `Buffer` | Буфер с сообщением |
| `options?` | [`CAdESSignOptions`](../interfaces/CAdES.CAdESSignOptions.md) | Доп. параметры |

#### Returns

`Buffer`

___

### verifyDetachedMessageSignature

▸ **verifyDetachedMessageSignature**(`signature`, `message`, `options?`): `any`

Проверка подписи сообщения (detached)

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `signature` | `Buffer` | Буфер с подписью |
| `message` | `Buffer` | Буфер с сообщением |
| `options?` | [`CAdESVerifySignatureOptions`](../interfaces/CAdES.CAdESVerifySignatureOptions.md) | Доп. параметры |

#### Returns

`any`

___

### verifyMessageSignature

▸ **verifyMessageSignature**(`message`, `options?`): `any`

Проверка подписи сообщения (attached)

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `message` | `Buffer` | Буфер с подписаным сообщением |
| `options?` | [`CAdESVerifyAttachedSignatureResult`](../interfaces/CAdES.CAdESVerifyAttachedSignatureResult.md) | Доп. параметры |

#### Returns

`any`
