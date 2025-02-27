[n-cryptopro](../README.md) / [Exports](../modules.md) / [CAdES](../modules/CAdES.md) / CAdESSignOptions

# Interface: CAdESSignOptions

[CAdES](../modules/CAdES.md).CAdESSignOptions

## Hierarchy

- [`SignOptions`](SignOptions.md)

  ↳ **`CAdESSignOptions`**

## Table of contents

### Properties

- [hashAlgorithm](CAdES.CAdESSignOptions.md#hashalgorithm)
- [isDetached](CAdES.CAdESSignOptions.md#isdetached)
- [type](CAdES.CAdESSignOptions.md#type)

## Properties

### hashAlgorithm

• **hashAlgorithm**: `string`

OID алгоритма хэширования (напр. CryptoPro.OID_CP_GOST_R3411_12_256)

#### Inherited from

[SignOptions](SignOptions.md).[hashAlgorithm](SignOptions.md#hashalgorithm)

___

### isDetached

• **isDetached**: `boolean`

true, если подпись detached

#### Inherited from

[SignOptions](SignOptions.md).[isDetached](SignOptions.md#isdetached)

___

### type

• `Optional` **type**: `number`

Тип подписи
