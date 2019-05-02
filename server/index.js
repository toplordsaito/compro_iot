var express = require('express')
var bodyParser = require('body-parser')
var request = require('request')
var app = express()
const line = require('@line/bot-sdk');
const config = require('./config.json');
// var mqtt = require('mqtt');
// Your Channel access token (long-lived) 
const CH_ACCESS_TOKEN = 'lC66V/rggnJsjO5RIIvotjM4lOgldeDfLttv5at50dxXspird9TBv+Ojt4mdjG54vIACf/2EIlMxHioJMkQ2q0EdHeFqzbzJLn3qjLpT0zX8IImNKwohwyrxFepmFy5bPrhFEAr3XBVa1CrtW0BXjgdB04t89/1O/w1cDnyilFU=';
var firebase = require("firebase");
const configfire = {
  apiKey: "AIzaSyCFfP5Gmv9_YL3VO_282WXPyHrxGLrg3Vo",
  authDomain: "iot-itcamp-4faf6.firebaseapp.com",
  databaseURL: "https://iot-itcamp-4faf6.firebaseio.com",
  projectId: "iot-itcamp-4faf6",
  storageBucket: "iot-itcamp-4faf6.appspot.com",
  messagingSenderId: "507261967577"
};
firebase.initializeApp(configfire);

var db = firebase.database();
// MQTT Host
// var mqtt_host = 'mqtt://m13.cloudmqtt.com';
// // MQTT Topic
// var mqtt_topic = '/ESP32';
// // MQTT Config
// var options = {
//     port: 14381,
//     host: 'mqtt://m13.cloudmqtt.com',
//     clientId: 'mqttjs_' + Math.random().toString(16).substr(2, 8),
//     username: 'mqttuser',
//     password: 'mqttpass',
//     keepalive: 60,
//     reconnectPeriod: 1000,
//     protocolId: 'MQIsdp',
//     protocolVersion: 3,
//     clean: true,
//     encoding: 'utf8'
// };
const client = new line.Client(config);

app.use(bodyParser.json())
app.set('port', (process.env.PORT || 4000))
app.use(bodyParser.urlencoded({extended: true}))
app.use(bodyParser.json())
app.post('/webhook', (req, res) => {
  var text = req.body.events[0].message.text.toLowerCase()
  var sender = req.body.events[0].source.userId
  var replyToken = req.body.events[0].replyToken
  console.log(text, sender, replyToken)
  console.log(typeof sender, typeof text)
  // console.log(req.body.events[0])
  if (~text.indexOf('status') || ~text.indexOf('รายงาน') || ~text.indexOf('ข้อมูล') || ~text.indexOf('สถานะ')) {
    // Info
    inFo(replyToken, text, 'temp')
  }
  else if (text === '1' ||~text.indexOf('เปิด') || ~text.indexOf('on')) {
    // LED On
    ledOn(replyToken, text)
  }
  else if (text === '0' || ~text.indexOf('ปิด') || ~text.indexOf('off')) {
    // LED Off
    ledOff(replyToken, text)
  }
  else if (~text.indexOf('รักคุณ') || ~text.indexOf('love you') || ~text.indexOf('รักนะ')) {
    // LED Off
    replyText(replyToken, 'ดิฉันก็รักคุณค่ะ');
  }
  else {
    // Other
    sendText(replyToken, text);
  }
  res.sendStatus(200)
})


app.post('/omg', (req, res) => {
  console.log(req.body)
  res.sendStatus(200)
})








function sendText (replyToken, text) {
  db.ref('/users').set(text);
  replyText(replyToken, "กรุณาพิมพ์ : status | on | off | เปิด | ปิด นะคะ");
  
  // let data = {
  //   to: sender,
  //   messages: [
  //     {
  //       type: 'text',
  //       text: 'กรุณาพิมพ์ : info | on | off | เปิด | ปิด เท่านั้น'
  //     }
  //   ]
  // }
  // request({
  //   headers: {
  //     'Content-Type': 'application/json',
  //     'Authorization': 'Bearer '+CH_ACCESS_TOKEN+''
  //   },
  //   url: 'https://api.line.me/v2/bot/message/push',
  //   method: 'POST',
  //   body: data,
  //   json: true
  // }, function (err, res, body) {
  //   if (err) console.log('error')
  //   if (res) console.log('success')
  //   if (body) console.log(body)
  // })
}

function inFo (replyToken, text, get) {
  db.ref('/val').once("value", function(snapshot) {
    var ans = snapshot.val();
    text = 'ความชื้น : ' + ans.humi + '\nอุณหภูมิ : ' + ans.temp+ '\nหลอดไฟ : ' + (ans.LED? 'เปิด':'ปิด');
    console.log(text);
  
    replyText(replyToken, text);
  }, function (errorObject) {
    console.log("The read failed: " + errorObject.code);
  });
  
}
function ledOn (replyToken, text) {
  db.ref('val/LED').set(true);
  replyText(replyToken, 'เปิดไฟให้แล้วค่ะ');
}
function ledOff (replyToken, text) {
  db.ref('val/LED').set(false);
  replyText(replyToken, 'ปิดไฟให้แล้วค่ะ');

}
app.listen(app.get('port'), function () {
  console.log('run at port', app.get('port'))
})



// simple reply function
const replyText = (token, texts) => {
  // texts = Array.isArray(texts) ? texts : [texts];
  // return client.replyMessage(
  //   token,
  //   texts.map((text) => ({ type: 'text', text } ))
  // );
  let data = {
    replyToken: token,
    messages: [
      {
        type: 'text',
        text: texts,
        "quickReply": {
          "items": [
            {
              "type": "action",
              "action": {  
                "type":"message",
                "label":"status",
                "text":"status"
              }
            },
            {
              "type": "action",
              "action": {  
                "type":"message",
                "label":"เปิดไฟ",
                "text":"เปิดไฟ"
              }
            },
            {
              "type": "action",
              "action": {  
                "type":"message",
                "label":"ปิดไฟ",
                "text":"ปิดไฟ"
              }
            },
            
          ]
        }
      },
      
    ]
  }
  request({
    headers: {
      'Content-Type': 'application/json',
      'Authorization': 'Bearer '+CH_ACCESS_TOKEN+''
    },
    url: 'https://api.line.me/v2/bot/message/reply',
    method: 'POST',
    body: data,
    json: true
  }, function (err, res, body) {
    if (err) console.log('error')
    if (res) console.log('success')
    if (body) console.log(body)
  })




};

// callback function to handle a single event

init()

 function init() {
  console.log("start");
  db.ref('/Code').on("value", function(snapshot) {
    var ans = snapshot.val();
    console.log(ans)
    



    //0=t 1=h 2=dis 3=time
    if(ans.code == 0.1){ //ร้อน
      let data = {
        to: '',
        messages: [
          {
            type: 'text',
            text: 'บ้านของคุณร้อนมากเกินไปแล้วนะคะ'
          },
          {
            "type": "sticker",
            "packageId": "11537",
            "stickerId": "52002767"
          }
        ]
      }
      request({
        headers: {
          'Content-Type': 'application/json',
          'Authorization': 'Bearer '+CH_ACCESS_TOKEN+''
        },
        url: 'https://api.line.me/v2/bot/message/broadcast',
        method: 'POST',
        body: data,
        json: true
      }, function (err, res, body) {
        if (err) console.log('error')
        if (res) console.log('success')
        if (body) console.log(body)
      })
    }
    else if(ans.code == 0.2){ //เย็น
      let data = {
        to: '',
        messages: [
          {
            type: 'text',
            text: 'บ้านของคุณอากาศเย็นเกินไปแล้วนะคะ'
          },
          {
            "type": "sticker",
            "packageId": "11537",
            "stickerId": "52002766"
          }
        ]
      }
      request({
        headers: {
          'Content-Type': 'application/json',
          'Authorization': 'Bearer '+CH_ACCESS_TOKEN+''
        },
        url: 'https://api.line.me/v2/bot/message/broadcast',
        method: 'POST',
        body: data,
        json: true
      }, function (err, res, body) {
        if (err) console.log('error')
        if (res) console.log('success')
        if (body) console.log(body)
      })
    }
    else if(ans.code == 0){ //แปรปวน
      let data = {
        to: '',
        messages: [
          {
            type: 'text',
            text: 'ตอนนี้อากาศในบ้านผิดปกติมากค่ะ กรุณาตรวจสอบบ้านของคุณด้วยค่ะ'
          },
          {
            "type": "sticker",
            "packageId": "11538",
            "stickerId": "51626516"
          }
        ]
      }
      request({
        headers: {
          'Content-Type': 'application/json',
          'Authorization': 'Bearer '+CH_ACCESS_TOKEN+''
        },
        url: 'https://api.line.me/v2/bot/message/broadcast',
        method: 'POST',
        body: data,
        json: true
      }, function (err, res, body) {
        if (err) console.log('error')
        if (res) console.log('success')
        if (body) console.log(body)
      })
    }
    else if(ans.code == 3){ //อรุณสวัส
      let data = {
        to: '',
        messages: [
          {
            type: 'text',
            text: `อรุณสวัสดิ์จ้า เช้านี้บ้านของคุณ \nอุณหภูมิ : ${temp}\nความชื้น : ${ans.humi}\nวันที่ : ${ans.time}`
          },
          {
            "type": "sticker",
            "packageId": "11537",
            "stickerId": "52002768"
          }
        ]
      }
      request({
        headers: {
          'Content-Type': 'application/json',
          'Authorization': 'Bearer '+CH_ACCESS_TOKEN+''
        },
        url: 'https://api.line.me/v2/bot/message/broadcast',
        method: 'POST',
        body: data,
        json: true
      }, function (err, res, body) {
        if (err) console.log('error')
        if (res) console.log('success')
        if (body) console.log(body)
      })
    }
    else if(ans.code == 1.1){ //ชื้นไป
      let data = {
        to: '',
        messages: [
          {
            type: 'text',
            text: 'ตอนนี้ความชื้นในบ้านของคุณมีค่ามากเกินไปซึ่งป็นความชื้นที่เหมาะสมต่อการเจริญเติบโตของเชื้อรา เป็นความชื้นที่เหมาะสมต่อการเจริญเติบโตของเชื้อรา ซึ่งเชื้อราจะเป็นอันตรายต่อบุคคลที่ป่วยเป็นโรคหอบหืดซึ่งเชื้อราจะเป็นอันตรายต่อบุคคลที่ป่วยเป็นโรคหอบหืดได้'
          },
          {
            "type": "sticker",
            "packageId": "11537",
            "stickerId": "52002770"
          }
        ]
      }
      request({
        headers: {
          'Content-Type': 'application/json',
          'Authorization': 'Bearer '+CH_ACCESS_TOKEN+''
        },
        url: 'https://api.line.me/v2/bot/message/broadcast',
        method: 'POST',
        body: data,
        json: true
      }, function (err, res, body) {
        if (err) console.log('error')
        if (res) console.log('success')
        if (body) console.log(body)
      })
    }
    else if(ans.code == 1.2){ //ชื้นเหมาะ
      let data = {
        to: '',
        messages: [
          {
            type: 'text',
            text: 'อรุณสวัสดิ์จ้า เช้านี้บ้านของคุณ'
          },
          {
            "type": "sticker",
            "packageId": "11537",
            "stickerId": "52002770"
          }
        ]
      }
      request({
        headers: {
          'Content-Type': 'application/json',
          'Authorization': 'Bearer '+CH_ACCESS_TOKEN+''
        },
        url: 'https://api.line.me/v2/bot/message/broadcast',
        method: 'POST',
        body: data,
        json: true
      }, function (err, res, body) {
        if (err) console.log('error')
        if (res) console.log('success')
        if (body) console.log(body)
      })
    }
    else if(ans.code == 1.3){ //ต่ำไป
      let data = {
        to: '',
        messages: [
          {
            type: 'text',
            text: 'ความชื้นในบ้านของคุณมีค่าต่ำกว่าระดับที่เหมาะสม ควรใช้ครีมบำรุงผิวเพื่อให้ผิวชุ่มชื้นอยู่ตลอดเวลา'
          },
          {
            "type": "sticker",
            "packageId": "11537",
            "stickerId": "52002770"
          }
        ]
      }
      request({
        headers: {
          'Content-Type': 'application/json',
          'Authorization': 'Bearer '+CH_ACCESS_TOKEN+''
        },
        url: 'https://api.line.me/v2/bot/message/broadcast',
        method: 'POST',
        body: data,
        json: true
      }, function (err, res, body) {
        if (err) console.log('error')
        if (res) console.log('success')
        if (body) console.log(body)
      })
    }

    
  
  
  
  }, function (errorObject) {
    console.log("The read failed: " + errorObject.code);
  });
  
  db.ref('/pass').on("value", function(snapshot) {
    var ans = snapshot.val();
    let data = {
      to: '',
      messages: [
        {
          type: 'text',
          text: 'มีบางอย่างผ่านประตูบ้านของคุณ'
        }
      ]
    }


    request({
    headers: {
      'Content-Type': 'application/json',
      'Authorization': 'Bearer '+CH_ACCESS_TOKEN+''
    },
    url: 'https://api.line.me/v2/bot/message/broadcast',
    method: 'POST',
    body: data,
    json: true
  }, function (err, res, body) {
    if (err) console.log('error')
    if (res) console.log('success')
    if (body) console.log(body)
  })


  }, function (errorObject) {
    console.log("The read failed: " + errorObject.code);
  });


}