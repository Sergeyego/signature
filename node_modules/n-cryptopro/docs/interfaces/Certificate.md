[n-cryptopro](../README.md) / [Exports](../modules.md) / Certificate

# Interface: Certificate

## Table of contents

### Methods

- [getIssuerAttribute](Certificate.md#getissuerattribute)
- [getIssuerName](Certificate.md#getissuername)
- [getProvider](Certificate.md#getprovider)
- [getSerialNumber](Certificate.md#getserialnumber)
- [getSubjectAttribute](Certificate.md#getsubjectattribute)
- [getSubjectName](Certificate.md#getsubjectname)
- [getValidPeriod](Certificate.md#getvalidperiod)
- [verifyChain](Certificate.md#verifychain)

## Methods

### getIssuerAttribute

▸ **getIssuerAttribute**(`type`): `string`

Получение атрибута издателя

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `type` | `string` | OID типа атрибута (напр. CryptoPro.OID_COMMON_NAME) |

#### Returns

`string`

___

### getIssuerName

▸ **getIssuerName**(): `string`

Получение имени издателя

#### Returns

`string`

___

### getProvider

▸ **getProvider**(): [`Provider`](Provider.md)

Получение криптопровайдера

#### Returns

[`Provider`](Provider.md)

___

### getSerialNumber

▸ **getSerialNumber**(): `Buffer`

Получение серийного номера

#### Returns

`Buffer`

___

### getSubjectAttribute

▸ **getSubjectAttribute**(`type`): `string`

Получение атрибута субъекта

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `type` | `string` | OID типа атрибута (напр. CryptoPro.OID_COMMON_NAME) |

#### Returns

`string`

___

### getSubjectName

▸ **getSubjectName**(): `string`

Получение имени субъекта

#### Returns

`string`

___

### getValidPeriod

▸ **getValidPeriod**(): `Object`

Получение срока действия

#### Returns

`Object`

| Name | Type |
| :------ | :------ |
| `from` | `Date` |
| `to` | `Date` |

___

### verifyChain

▸ **verifyChain**(): `boolean`

Проверка цепочки сертификата

#### Returns

`boolean`
