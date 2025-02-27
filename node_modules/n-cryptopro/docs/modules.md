[n-cryptopro](README.md) / Exports

# n-cryptopro

## Table of contents

### Namespaces

- [CAdES](modules/CAdES.md)
- [XAdES](modules/XAdES.md)

### Interfaces

- [Certificate](interfaces/Certificate.md)
- [Provider](interfaces/Provider.md)
- [SignOptions](interfaces/SignOptions.md)
- [VerifyAttachedSignatureResult](interfaces/VerifyAttachedSignatureResult.md)
- [VerifySignatureResult](interfaces/VerifySignatureResult.md)

### Variables

- [OID\_COMMON\_NAME](modules.md#oid_common_name)
- [OID\_COUNTRY\_NAME](modules.md#oid_country_name)
- [OID\_CP\_GOST\_R3411](modules.md#oid_cp_gost_r3411)
- [OID\_CP\_GOST\_R3411\_12\_256](modules.md#oid_cp_gost_r3411_12_256)
- [OID\_CP\_GOST\_R3411\_12\_512](modules.md#oid_cp_gost_r3411_12_512)
- [OID\_GIVEN\_NAME](modules.md#oid_given_name)
- [OID\_INN](modules.md#oid_inn)
- [OID\_INNLE](modules.md#oid_innle)
- [OID\_LOCALITY\_NAME](modules.md#oid_locality_name)
- [OID\_OGRN](modules.md#oid_ogrn)
- [OID\_OGRNIP](modules.md#oid_ogrnip)
- [OID\_ORGANIZATIONAL\_UNIT\_NAME](modules.md#oid_organizational_unit_name)
- [OID\_ORGANIZATION\_NAME](modules.md#oid_organization_name)
- [OID\_SNILS](modules.md#oid_snils)
- [OID\_STATE\_OR\_PROVINCE\_NAME](modules.md#oid_state_or_province_name)
- [OID\_STREET\_ADDRESS](modules.md#oid_street_address)
- [OID\_SUR\_NAME](modules.md#oid_sur_name)
- [OID\_TITLE](modules.md#oid_title)
- [PROV\_GOST\_2001\_DH](modules.md#prov_gost_2001_dh)
- [PROV\_GOST\_2012\_256](modules.md#prov_gost_2012_256)
- [PROV\_GOST\_2012\_512](modules.md#prov_gost_2012_512)

### Functions

- [decryptMessage](modules.md#decryptmessage)
- [encryptMessage](modules.md#encryptmessage)
- [getCertificateFromBuffer](modules.md#getcertificatefrombuffer)
- [getCertificates](modules.md#getcertificates)
- [hash](modules.md#hash)
- [signHash](modules.md#signhash)
- [signMessage](modules.md#signmessage)
- [verifyDetachedMessageSignature](modules.md#verifydetachedmessagesignature)
- [verifyHashSignature](modules.md#verifyhashsignature)
- [verifyMessageSignature](modules.md#verifymessagesignature)

## Variables

### OID\_COMMON\_NAME

• `Const` **OID\_COMMON\_NAME**: `string`

Общее имя

___

### OID\_COUNTRY\_NAME

• `Const` **OID\_COUNTRY\_NAME**: `string`

Наименование страны

___

### OID\_CP\_GOST\_R3411

• `Const` **OID\_CP\_GOST\_R3411**: `string`

Алгоритм хэширования в соответствии с ГОСТ Р 34.11-94

___

### OID\_CP\_GOST\_R3411\_12\_256

• `Const` **OID\_CP\_GOST\_R3411\_12\_256**: `string`

Алгоритм хэширования в соответствии с ГОСТ Р 34.11-2012, длина выхода 256 бит

___

### OID\_CP\_GOST\_R3411\_12\_512

• `Const` **OID\_CP\_GOST\_R3411\_12\_512**: `string`

Алгоритм хэширования в соответствии с ГОСТ Р 34.11-2012, длина выхода 512 бит

___

### OID\_GIVEN\_NAME

• `Const` **OID\_GIVEN\_NAME**: `string`

Приобретенное имя

___

### OID\_INN

• `Const` **OID\_INN**: `string`

ИНН физического лица

___

### OID\_INNLE

• `Const` **OID\_INNLE**: `string`

ИНН юридического лица

___

### OID\_LOCALITY\_NAME

• `Const` **OID\_LOCALITY\_NAME**: `string`

Наименование населенного пункта

___

### OID\_OGRN

• `Const` **OID\_OGRN**: `string`

ОГРН

___

### OID\_OGRNIP

• `Const` **OID\_OGRNIP**: `string`

ОГРНИП

___

### OID\_ORGANIZATIONAL\_UNIT\_NAME

• `Const` **OID\_ORGANIZATIONAL\_UNIT\_NAME**: `string`

Подразделение организации

___

### OID\_ORGANIZATION\_NAME

• `Const` **OID\_ORGANIZATION\_NAME**: `string`

Наименование организации

___

### OID\_SNILS

• `Const` **OID\_SNILS**: `string`

СНИЛС

___

### OID\_STATE\_OR\_PROVINCE\_NAME

• `Const` **OID\_STATE\_OR\_PROVINCE\_NAME**: `string`

Наименование штата или области

___

### OID\_STREET\_ADDRESS

• `Const` **OID\_STREET\_ADDRESS**: `string`

Наименование улицы, номер дома

___

### OID\_SUR\_NAME

• `Const` **OID\_SUR\_NAME**: `string`

Фамилия

___

### OID\_TITLE

• `Const` **OID\_TITLE**: `string`

Должность

___

### PROV\_GOST\_2001\_DH

• `Const` **PROV\_GOST\_2001\_DH**: `number`

Провайдер типа PROV_GOST_2001_DH создает ключи алгоритма ГОСТ Р 34.10-2001

___

### PROV\_GOST\_2012\_256

• `Const` **PROV\_GOST\_2012\_256**: `number`

Провайдер типа PROV_GOST_2012_256 создает ключи алгоритма ГОСТ Р 34.10-2012 длины 256 бит (длина открытого ключа 512 бит)

___

### PROV\_GOST\_2012\_512

• `Const` **PROV\_GOST\_2012\_512**: `number`

Провайдер типа PROV_GOST_2012_512 создает ключи алгоритма ГОСТ Р 34.10-2012 длины 512 бит (длина открытого ключа 1024 бита)

## Functions

### decryptMessage

▸ **decryptMessage**(`message`): `Buffer`

Расшифровка сообщения

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `message` | `Buffer` | Буфер с сообщением |

#### Returns

`Buffer`

___

### encryptMessage

▸ **encryptMessage**(`certificate`, `message`): `Buffer`

Шифрование сообщения

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `certificate` | [`Certificate`](interfaces/Certificate.md) | Сертификат |
| `message` | `Buffer` | Буфер с зашифрованым сообщением |

#### Returns

`Buffer`

___

### getCertificateFromBuffer

▸ **getCertificateFromBuffer**(`buffer`): [`Certificate`](interfaces/Certificate.md)

Получение сертификата из буфера

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `buffer` | `Buffer` | Буфер с сертификатом в формате X.509 / PKCS #7 |

#### Returns

[`Certificate`](interfaces/Certificate.md)

___

### getCertificates

▸ **getCertificates**(`storeName`): [`Certificate`](interfaces/Certificate.md)[]

Получение списка сертификатов из хранилища сертификатов

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `storeName` | `string` | Название хранилища сертификатов (напр. "My") |

#### Returns

[`Certificate`](interfaces/Certificate.md)[]

___

### hash

▸ **hash**(`algorithm`, `data`): `Buffer`

Вычисление хэша для данных

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `algorithm` | `string` | OID алгоритма (напр. CryptoPro.OID_CP_GOST_R3411_12_256) |
| `data` | `Buffer` | Буфер с данными |

#### Returns

`Buffer`

___

### signHash

▸ **signHash**(`certificate`, `algorithm`, `hash`): `Buffer`

Подпись хэша

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `certificate` | [`Certificate`](interfaces/Certificate.md) | Сертификат |
| `algorithm` | `string` | OID алгоритма (напр. CryptoPro.OID_CP_GOST_R3411_12_256) |
| `hash` | `Buffer` | Буфер с хэшем |

#### Returns

`Buffer`

___

### signMessage

▸ **signMessage**(`certificate`, `message`, `options`): `Buffer`

Подписание сообщения

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `certificate` | [`Certificate`](interfaces/Certificate.md) | Сертификат |
| `message` | `Buffer` | Буфер с сообщением |
| `options` | `object` | Доп. параметры |

#### Returns

`Buffer`

___

### verifyDetachedMessageSignature

▸ **verifyDetachedMessageSignature**(`signature`, `message`): [`VerifySignatureResult`](interfaces/VerifySignatureResult.md)

Проверка подписи сообщения (detached)

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `signature` | `Buffer` | Буфер с подписью |
| `message` | `Buffer` | Буфер с сообщением |

#### Returns

[`VerifySignatureResult`](interfaces/VerifySignatureResult.md)

___

### verifyHashSignature

▸ **verifyHashSignature**(`certificate`, `algorithm`, `hash`, `signature`): `boolean`

Проверка подписи хэша

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `certificate` | [`Certificate`](interfaces/Certificate.md) | Сертификат |
| `algorithm` | `string` | OID алгоритма (напр. CryptoPro.OID_CP_GOST_R3411_12_256) |
| `hash` | `Buffer` | Буфер с хэшем |
| `signature` | `Buffer` | Буфер с подписью |

#### Returns

`boolean`

___

### verifyMessageSignature

▸ **verifyMessageSignature**(`message`): [`VerifyAttachedSignatureResult`](interfaces/VerifyAttachedSignatureResult.md)

Проверка подписи сообщения (attached)

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `message` | `Buffer` | Буфер с подписаным сообщением |

#### Returns

[`VerifyAttachedSignatureResult`](interfaces/VerifyAttachedSignatureResult.md)
