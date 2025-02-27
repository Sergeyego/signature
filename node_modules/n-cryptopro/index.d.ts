// Main
interface SignOptions {
    /**
     * true, если подпись detached
     */
    isDetached: boolean;

    /**
     * OID алгоритма хэширования (напр. CryptoPro.OID_CP_GOST_R3411_12_256)
     */
    hashAlgorithm: string;
}

interface VerifySignatureResult {
    /**
     * Подпись действительна
     */
    isValid: boolean;

    /**
     * Сертификат
     */
    certificate: Certificate;
}

// Результат проверки сообщения
interface VerifyAttachedSignatureResult extends VerifySignatureResult {
    data: Buffer;
}

/**
 * Получение списка сертификатов из хранилища сертификатов
 * @param storeName Название хранилища сертификатов (напр. "My")
 */
export function getCertificates(storeName: string): Certificate[];

/**
 * Получение сертификата из буфера
 * @param buffer Буфер с сертификатом в формате X.509 / PKCS #7
 */
export function getCertificateFromBuffer(buffer: Buffer): Certificate;

/**
 * Вычисление хэша для данных
 * @param algorithm OID алгоритма (напр. CryptoPro.OID_CP_GOST_R3411_12_256)
 * @param data Буфер с данными
 */
export function hash(algorithm: string, data: Buffer): Buffer;

/**
 * Подпись хэша
 * @param certificate Сертификат
 * @param algorithm OID алгоритма (напр. CryptoPro.OID_CP_GOST_R3411_12_256)
 * @param hash Буфер с хэшем
 */
export function signHash(certificate: Certificate, algorithm: string, hash: Buffer): Buffer;

/**
 * Проверка подписи хэша
 * @param certificate Сертификат
 * @param algorithm OID алгоритма (напр. CryptoPro.OID_CP_GOST_R3411_12_256)
 * @param hash Буфер с хэшем
 * @param signature Буфер с подписью
 */
export function verifyHashSignature(certificate: Certificate, algorithm: string, hash: Buffer, signature: Buffer): boolean;

/**
 * Шифрование сообщения
 * @param certificate Сертификат
 * @param message Буфер с зашифрованым сообщением
 */
export function encryptMessage(certificate: Certificate, message: Buffer): Buffer;

/**
 * Расшифровка сообщения
 * @param message Буфер с сообщением
 */
export function decryptMessage(message: Buffer): Buffer;

/**
 * Подписание сообщения
 * @param certificate Сертификат
 * @param message Буфер с сообщением
 * @param options Доп. параметры
 */
export function signMessage(certificate: Certificate, message: Buffer, options: object): Buffer;

/**
 * Проверка подписи сообщения (attached)
 * @param message Буфер с подписаным сообщением
 */
export function verifyMessageSignature(message: Buffer): VerifyAttachedSignatureResult;

/**
 * Проверка подписи сообщения (detached)
 * @param signature Буфер с подписью
 * @param message Буфер с сообщением
 */
export function verifyDetachedMessageSignature(signature: Buffer, message: Buffer): VerifySignatureResult;

/**
 * Алгоритм хэширования в соответствии с ГОСТ Р 34.11-94
 */
export const OID_CP_GOST_R3411: string;
/**
 * Алгоритм хэширования в соответствии с ГОСТ Р 34.11-2012, длина выхода 256 бит
 */
export const OID_CP_GOST_R3411_12_256: string;
/**
 * Алгоритм хэширования в соответствии с ГОСТ Р 34.11-2012, длина выхода 512 бит
 */
export const OID_CP_GOST_R3411_12_512: string;

// Provider
declare interface Provider {
    getName(): string;
    getContainerName(): string;
}

/**
 * Провайдер типа PROV_GOST_2001_DH создает ключи алгоритма ГОСТ Р 34.10-2001
 */
export const PROV_GOST_2001_DH: number;
/**
 * Провайдер типа PROV_GOST_2012_256 создает ключи алгоритма ГОСТ Р 34.10-2012 длины 256 бит (длина открытого ключа 512 бит)
 */
export const PROV_GOST_2012_256: number;
/**
 * Провайдер типа PROV_GOST_2012_512 создает ключи алгоритма ГОСТ Р 34.10-2012 длины 512 бит (длина открытого ключа 1024 бита)
 */
export const PROV_GOST_2012_512: number;

// Certificate
declare interface Certificate {
    /**
     * Получение имени субъекта
     */
    getSubjectName(): string;

    /**
     * Получение имени издателя
     */
    getIssuerName(): string;


    /**
     * Получение атрибута субъекта
     * @param type OID типа атрибута (напр. CryptoPro.OID_COMMON_NAME)
     */
     getSubjectAttribute(type: string): string;

    /**
     * Получение атрибута издателя
     * @param type OID типа атрибута (напр. CryptoPro.OID_COMMON_NAME)
     */
     getIssuerAttribute(type: string): string;

    /**
     * Получение серийного номера
     */
    getSerialNumber(): Buffer;

    /**
     * Получение срока действия
     */
    getValidPeriod(): {from: Date, to: Date};

    /**
     * Проверка цепочки сертификата
     */
    verifyChain(): boolean;

    /**
     * Получение криптопровайдера
     */
    getProvider(): Provider;
}

/**
 * Общее имя
 */
export const OID_COMMON_NAME: string;
/**
 * Фамилия
 */
export const OID_SUR_NAME: string;
/**
 * Приобретенное имя
 */
export const OID_GIVEN_NAME: string;
/**
 * Наименование страны
 */
export const OID_COUNTRY_NAME: string;
/**
 * Наименование штата или области
 */
export const OID_STATE_OR_PROVINCE_NAME: string;
/**
 * Наименование населенного пункта
 */
export const OID_LOCALITY_NAME: string;
/**
 * Наименование улицы, номер дома
 */
export const OID_STREET_ADDRESS: string;
/**
 * Наименование организации
 */
export const OID_ORGANIZATION_NAME: string;
/**
 * Подразделение организации
 */
export const OID_ORGANIZATIONAL_UNIT_NAME: string;
/**
 * Должность
 */
export const OID_TITLE: string;
/**
 * ОГРН
 */
export const OID_OGRN: string;
/**
 * СНИЛС
 */
export const OID_SNILS: string;
/**
 * ИНН физического лица
 */
export const OID_INN: string;
/**
 * ИНН юридического лица
 */
export const OID_INNLE: string;
/**
 * ОГРНИП
 */
export const OID_OGRNIP: string;

// CAdES
export namespace CAdES {
    interface CAdESSignOptions extends SignOptions {
        /**
         * Тип подписи
         */
        type?: number;
    }

    interface CAdESVerifySignatureOptions {
        /**
         * Тип подписи
         */
        type: string;
    }

    interface CAdESVerifySignatureResult extends VerifySignatureResult {
        signingTime: Date;
        signatureTimeStampTime: Date;
    }

    interface CAdESVerifyAttachedSignatureResult extends VerifyAttachedSignatureResult {
        signingTime: Date;
        signatureTimeStampTime: Date;
    }

    /**
     * Подписание сообщения
     * @param certificate Сертификат
     * @param message Буфер с сообщением
     * @param options Доп. параметры
     */
    export function signMessage(certificate: Certificate, message: Buffer, options?: CAdESSignOptions): Buffer;
    
    /**
     * Проверка подписи сообщения (attached)
     * @param message Буфер с подписаным сообщением
     * @param options Доп. параметры
     */
    export function verifyMessageSignature(message: Buffer, options?: CAdESVerifyAttachedSignatureResult);
    
    /**
     * Проверка подписи сообщения (detached)
     * @param signature Буфер с подписью
     * @param message Буфер с сообщением
     * @param options Доп. параметры
     */
    export function verifyDetachedMessageSignature(signature: Buffer, message: Buffer, options?: CAdESVerifySignatureOptions);

    /**
     * Тип сообщения CAdES BES
     */
    export const CADES_BES: number;
    /**
     * Тип сообщения CAdES-T
     */
    export const CADES_T: number;
    /**
     * Тип сообщения CAdES-X Long Type 1
     */
    export const CADES_X_LONG_TYPE_1: number;
}

// XAdES
export namespace XAdES {
    interface XAdESSignOptions {
        /**
         * Регулярное выражение для получение узла подписи
         */
        xpath?: string;
        /**
         * Тип подписи
         */
        type?: number;
    }

    interface XAdESVerifySignatureOptions {
        /**
         * Регулярное выражение для получение узла подписи
         */
        xpath?: string;
        /**
         * Тип подписи
         */
        type?: number;
    }

    interface XAdESVerifySignatureResult extends VerifySignatureResult {
        /**
         * Тип подписи (напр. CryptoPro.XAdES.XADES_BES)
         */
        type: number;
        signingTime: Date;
        signatureTimeStampTime: Date;
        sigAndRefsTimeStampTime: Date;
    }

    /**
     * Подписание сообщения
     * @param certificate Сертификат
     * @param message Буфер с сообщением
     * @param options Доп. параметры
     */
    export function signMessage(certificate: Certificate, message: Buffer, options?: XAdESSignOptions): Buffer;
    
    /**
     * Проверка подписи сообщения
     * @param message Буфер с подписаным сообщением
     * @param options Доп. параметры
     */
    export function verifyMessageSignature(message: Buffer, options?: XAdESVerifySignatureOptions): XAdESVerifySignatureResult[];

    /**
     * XML_XADES_SIGNATURE_TYPE_ENVELOPED
     */
    export const XML_XADES_SIGNATURE_TYPE_ENVELOPED: number;
    /**
     * XML_XADES_SIGNATURE_TYPE_ENVELOPING
     */
     export const XML_XADES_SIGNATURE_TYPE_ENVELOPING: number;
    /**
     * XML_XADES_SIGNATURE_TYPE_TEMPLATE
     */
     export const XML_XADES_SIGNATURE_TYPE_TEMPLATE: number;
    /**
     * Тип сообщения XAdES BES
     */
    export const XADES_BES: number;
    /**
     * Тип сообщения XAdES-T
     */
    export const XADES_T: number;
    /**
     * Тип сообщения XAdES-X Long Type 1
     */
    export const XADES_X_LONG_TYPE_1: number;
    /**
     * Тип сообщения XMLDSIG
     */
    export const XADES_XMLDSIG: number;
}
