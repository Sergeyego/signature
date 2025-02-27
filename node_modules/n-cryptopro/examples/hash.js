const CryptoPro = require('../index.js')
const helpers = require('./helpers')

const cert = helpers.getCertificate()
const alg = CryptoPro.OID_CP_GOST_R3411_12_256

const data = Buffer.from('Test content')
console.log('Data:', data)

console.log('Algorithm:', alg)

const hash = CryptoPro.hash(alg, data)
console.log('Hash:', hash.toString('hex'))

const hashSignature = CryptoPro.signHash(cert, alg, hash)
console.log('Hash signature:', hashSignature)

const isValidSignature = CryptoPro.verifyHashSignature(cert, alg, hash, hashSignature)
console.log('Signature valid:', isValidSignature)
