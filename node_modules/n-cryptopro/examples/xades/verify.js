const fs = require('fs')
const CryptoPro = require('../../index.js')
const helpers = require('../helpers')

const signedXmlData = fs.readFileSync('./message_signed.xml')

console.log('Проверяем подпись в XML-файле message_signed.xml:')

const verifyResults = CryptoPro.XAdES.verifyMessageSignature(signedXmlData)
for (const result of verifyResults) {
    console.log('Подпись корректна:', result.isValid)
    console.log('signingTime:', result.signingTime)
    console.log('signatureTimeStampTime:', result.signatureTimeStampTime)
    console.log('sigAndRefsTimeStampTime:', result.sigAndRefsTimeStampTime)

    if (result.certificate) {
        console.log('Информация о сертификате:')
        helpers.printCertificateInfo(result.certificate)
    } else {
        console.log('Информация о сертификате отсутствует')
    }
}
