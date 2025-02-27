const fs = require('fs')
const CryptoPro = require('../../index.js')
const helpers = require('../helpers')

const data = fs.readFileSync('./content.txt')
const cert = helpers.getCertificate()

console.log('Подписываем файл content.txt следующим сертификатом:')
helpers.printCertificateInfo(cert)

fs.writeFileSync('./content.txt.sig', CryptoPro.signMessage(cert, data))

console.log('Файл подписан, результат сохранен в файл content.txt.sig')
