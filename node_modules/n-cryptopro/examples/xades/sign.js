const fs = require('fs')
const CryptoPro = require('../../index.js')
const helpers = require('../helpers')

const data = fs.readFileSync('./message.xml')
const cert = helpers.getCertificate()

console.log('Подписываем XML-файл message.xml следующим сертификатом:')
helpers.printCertificateInfo(cert)

fs.writeFileSync('./message_signed.xml', CryptoPro.XAdES.signMessage(cert, data))

console.log('Файл подписан, результат сохранен в файл message_signed.xml')
