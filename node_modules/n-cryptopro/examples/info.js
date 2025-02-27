const helpers = require('./helpers')

const cert = helpers.getCertificate()
helpers.printCertificateInfo(cert)

const provider = cert.getProvider()
helpers.printProviderInfo(provider)
