var pdf = require('@trusted-pdf/pdf');
var pdfSign = require('@trusted-pdf/sign');
var CryptoPro = require('n-cryptopro');
var { createCanvas, Image } = require('canvas');
var express = require('express');
const { Buffer } = require('node:buffer');
var bodyParser = require('body-parser');

var app = express();
const port = 8000;

let insDate = function (dat){
  let dateoptions = {
      year: 'numeric',
      month: 'numeric',
      day: 'numeric',
  };
  return new Intl.DateTimeFormat('ru-RU',dateoptions).format(dat);
}

let getSerialNumber = function(crt){
  var src=crt.getSerialNumber();
  var sn = Buffer.alloc(src.length);
  for (var i = 0, j = src.length - 1; i <= j; ++i, --j) {
    sn[i] = src[j];
    sn[j] = src[i];
  }
  return sn.toString('hex');
}

app.use(bodyParser.raw({ type: 'application/pdf', limit: '10mb'}));

app.get('/', (req, res) => {
  res.status(200).type('text/plain');
  res.send('Welcome to the server');
})

app.get('/certificates', async (req, res) => {
  const cert = CryptoPro.getCertificates('MY');
  var result = [];
  for (let i = 0; i < cert.length; i++) {
    result.push(
      {
        SubjectName: cert[i].getSubjectName(),
        SurName: cert[i].getSubjectAttribute(CryptoPro.OID_SUR_NAME),
        GivenName: cert[i].getSubjectAttribute(CryptoPro.OID_GIVEN_NAME),
        Title: cert[i].getSubjectAttribute(CryptoPro.OID_TITLE),
        SerialNumber: getSerialNumber(cert[i]),
        From: cert[i].getValidPeriod().from,
        To: cert[i].getValidPeriod().to,
      });
  }
  //console.log('certs:', result);
  res.json(result);
})

app.post("/pdf/:sn", async (req, res) => {
  const cert = CryptoPro.getCertificates('MY').find(item => getSerialNumber(item) === String(req.params["sn"]));
  if(cert!=null){
    try {
      //console.log('Subject name:', cert.getSubjectName());
      if (req.body.length==0){
        throw new TypeError("Нет данных для подписи!");
      }

      const canvas = createCanvas(600,252);
      const ctx = canvas.getContext('2d');
      const img = new Image();
      img.onload = () => ctx.drawImage(img, 0, 0,600,252);
      img.onerror = err => { throw err };
      img.src = 'signature.jpg';
      ctx.font = '20px Arial';
      ctx.fillStyle = 'rgb(17, 44, 87)';
      ctx.fillText('Сертификат: '+getSerialNumber(cert), 20, 150);
      ctx.fillText('Владелец: '+cert.getSubjectAttribute(CryptoPro.OID_SUR_NAME)+' '+cert.getSubjectAttribute(CryptoPro.OID_GIVEN_NAME), 20, 180);
      ctx.fillText('Должность: '+cert.getSubjectAttribute(CryptoPro.OID_TITLE), 20, 210);
      ctx.fillText('Действителен: с '+insDate(cert.getValidPeriod().from)+' по '+insDate(cert.getValidPeriod().to), 20, 240);
      ctx.font = '22px Arial';
      ctx.fillText('ДОКУМЕНТ ПОДПИСАН\nЭЛЕКТРОННОЙ ПОДПИСЬЮ', 250, 50);

      const doc = new pdf.Document();
      doc.read(req.body);

      const sign = new pdfSign.Sign(doc);

      const image = doc.createImage(canvas.toBuffer('image/jpeg',{ quality: 0.9 }));
      const uuid = "img-sign";


      sign.addSignatures({
        name: "signature 1",
        page: 1,
        rectangle: {
          x: 370,
          y: 50,
          height: 84,
          width: 200,
        },
      });

      const sig = sign.getSignature("signature 1");

      // Получаем объект отображения сигнатуры.
      const n = sig.dict.get(pdf.Names.AP, pdf.PdfDictionary).get(pdf.Names.N, pdf.PdfStream);

      // Добавляем размеры исходного изображения.
      n.set(pdf.Names.BBox, sig.dict.doc.createArray(
        sig.dict.doc.createNumber(0),
        sig.dict.doc.createNumber(0),
        sig.dict.doc.createNumber(image.width),
        sig.dict.doc.createNumber(image.height),
      ));

      // Добавляем контент отображения изображения.
      n.value = pdf.PdfBuffer.stringToRaw(
        `q\n1 0 0 1 0 0 cm\n${image.width} 0 0 ${image.height} 0 0 cm\n/${uuid} Do\nQ`
      );

      // Добавляем изображение в ресурсы.
      n.set(pdf.Names.Resources, sig.dict.doc.createDictionary({
        XObject: sig.dict.doc.createDictionary({
          [uuid]: image.ref(),
        })
      }));

      sig.sign({
        contentLength: 4096,
        onSign: (content) => {
          const cms = CryptoPro.signMessage(cert, content,{ isDetached: true });
          return new Uint8Array(cms);
        },
      }).then(() => {
        const buf = Buffer.from(doc.save());
        res.type('application/pdf');
        res.send(buf);
      })
      .catch((error) => {
        console.log('ERROR:', error);
        res.status(500).type('text/plain');
        res.send(error.message);
      });
    } catch (error) {
      console.log('ERROR:', error);
      res.status(500).type('text/plain');
      res.send(error.message);
    }
  } else {
    res.status(500).type('text/plain');
    res.send('Сертификат подписи не найден: '+String(req.params["sn"]));
  }
})

app.use((req, res, next) => {
  res.status(404).type('text/plain');
  res.send('Not found');
})

app.listen(port, () => {
  console.log(`Server signature listening on port ${port}`);
})