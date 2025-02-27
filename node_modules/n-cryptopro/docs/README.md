n-cryptopro / [Exports](modules.md)

# Модуль для работы с КриптоПро CSP в Node.js

Данный модуль позволяет использовать возможности КриптоПро в Node.js, а именно:

 - Получение сертификатов из файлов и хранилища сертификатов, а так же информации о них
 - Получение крипто-провайдера, связанного с сертификатом, а так же информации о нем
 - Работа с хэшем, а именно:
	 - Вычисление *(WinCrypt: CryptHashData)*
	 - Подпись *(WinCrypt: CryptSignHash)*
	 - Проверка подписи *(WinCrypt: CryptVerifySignature)*
 - Шифрование сообщений *(WinCrypt: CryptEncryptMessage)*
 - Расшифровка сообщений *(WinCrypt: CryptDecryptMessage)*
 - Подпись сообщений:
	 - CMS (attached/detached) *(WinCrypt: CryptSignMessage)*
	 - CAdES (attached/detached) *(КриптоПРО ЭЦП: CadesSignMessage)*
	 - XAdES *(КриптоПРО ЭЦП: XadesSign)*
 - Проверка подписи сообщений:
	 - CMS (attached/detached) *(WinCrypt: CryptVerifyMessageSignature/CryptVerifyDetachedMessageSignature)*
	 - CAdES (attached/detached) *(КриптоПРО ЭЦП: CadesVerifyMessage/CadesVerifyDetachedMessage)*
	 - XAdES *(КриптоПРО ЭЦП: XadesVerify)*

## Подготовка

### Windows:

1. Загрузить и установить [КриптоПро CSP](https://cryptopro.ru/products/csp/downloads)
2. Установить и установить [КриптоПро ЭЦП SDK](https://www.cryptopro.ru/products/cades/downloads)
3. Установить контейнер закрытого ключа и личный сертификат

### Ubuntu:

1. Установить пакеты, необходимые для сборки
`sudo apt install build-essentials`

2. Загрузить и установить [КриптоПро CSP](https://cryptopro.ru/products/csp/downloads)

	2.1. Загрузить дистрибутив

	2.2. Распаковать архив:
	`tar xvfz linux-amd64_deb.tgz && cd linux-amd64_deb`

	2.3. Установить базовый набор пакетов:
	`sudo ./install.sh`

	2.4. Установить пакет разработчика:
	`sudo apt install ./lsb-cprocsp-devel_*_all.deb`

3. Загрузить и установить [КриптоПро ЭЦП SDK](https://www.cryptopro.ru/products/cades/downloads)

	3.1. Загрузить дистрибутив

	3.2. Распаковать архив:
	`tar xvfz cades-linux-amd64.tar.gz && cd cades-linux-amd64`

	3.3. Установить:
	`sudo apt install ./cprocsp-pki-cades-64_*_amd64.deb`

4. Установить контейнер закрытого ключа и личный сертификат

### MacOS:

1. Установить XCode
`xcode-select --install`

2. Загрузить и установить [КриптоПро CSP](https://cryptopro.ru/products/csp/downloads) (дистрибутив включает в себя КриптоПРО ЭЦП SDK)

3. Установить контейнер закрытого ключа и личный сертификат

## Установка пакета

`npm install n-cryptopro`

## Документация

Документация расположена в директории [docs](docs/modules.md).

Примеры использования расположены в директории [examples](examples.md)
