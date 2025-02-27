const fs = require('fs')
const CryptoPro = require('../../index.js')
const helpers = require('../helpers')

console.log('Расшифровываем содержимое файла content.txt.enc')

const encryptedData = fs.readFileSync('./content.txt.enc')

const { certificate, data: decryptedData } = CryptoPro.decryptMessage(encryptedData)
fs.writeFileSync('./content_decrypted.txt', decryptedData)

console.log('Данные расшифрованы и сохранены в файл content_decrypted.txt')

if (certificate) {
    console.log('Информация о сертификате:')
    helpers.printCertificateInfo(certificate)
} else {
    console.log('Информация о сертификате отсутствует')
}
