const fs = require('fs')
const CryptoPro = require('../../index.js')
const helpers = require('../helpers')

const signedData = fs.readFileSync('./content.txt.sig')

console.log('Проверяем подпись в файле content.txt.sig:')

const verifyResult = CryptoPro.CAdES.verifyMessageSignature(signedData, { type: CryptoPro.CAdES.CADES_BES })
console.log('Подпись корректна:', verifyResult.isValid)
console.log('signingTime:', verifyResult.signingTime)
console.log('signatureTimeStampTime:', verifyResult.signatureTimeStampTime)

if (verifyResult.certificate) {
    console.log('Информация о сертификате:')
    helpers.printCertificateInfo(verifyResult.certificate)
} else {
    console.log('Информация о сертификате отсутствует')
}
