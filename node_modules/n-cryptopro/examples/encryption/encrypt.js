const fs = require('fs')
const CryptoPro = require('../../index.js')
const helpers = require('../helpers')

console.log('Шифруем содержимое файла content.txt с использованием ключа, связанного со следующим сертификатом:')

const cert = helpers.getCertificate()
helpers.printCertificateInfo(cert)

const data = fs.readFileSync('./content.txt')

const encryptedData = CryptoPro.encryptMessage(cert, data)
fs.writeFileSync('./content.txt.enc', encryptedData)

console.log('Данные зашифрованы и сохранены в файл content.txt.enc')
