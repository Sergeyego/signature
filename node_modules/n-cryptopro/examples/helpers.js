const fs = require('fs')
const CryptoPro = require('../index.js')

module.exports.printProviderInfo = function (prov) {
  console.log('Provider')
  console.log('\tName:', prov.getName())
  console.log('\tContainer name:', prov.getContainerName())
}

module.exports.printCertificateInfo = function (cert) {
  console.log('Certificate')
  console.log('\tSubject name:', cert.getSubjectName())
  console.log('\tIssuer name:', cert.getIssuerName())
  console.log('\tSerial number:', cert.getSerialNumber().toString('hex'))
  console.log('\tSignature algorithm:', cert.getSignatureAlgorithm())
  console.log('\tPublic key algorithm:', cert.getPublicKeyAlgorithm())
  console.log('\tValid period:', cert.getValidPeriod())
  // console.log('\tverifyChain: ' + cert.verifyChain())
}

module.exports.getCertificate = function () {
  return CryptoPro.getCertificates('MY').find(item => item.getSubjectName() === 'Иванов Иван Иванович')
}
