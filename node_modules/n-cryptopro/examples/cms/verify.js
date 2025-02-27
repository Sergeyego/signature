const fs = require('fs')
const CryptoPro = require('../../index.js')
const helpers = require('../helpers')

const signedData = fs.readFileSync('./content.txt.sig')

console.log('Проверяем подпись в файле content.txt.sig:')

const { isValid, certificate, data } = CryptoPro.verifyMessageSignature(signedData)
console.log('Подпись корректна:', isValid)

if (certificate) {
    console.log('Информация о сертификате:')
    helpers.printCertificateInfo(certificate)
} else {
    console.log('Информация о сертификате отсутствует')
}
