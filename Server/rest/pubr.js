
var mqtt = require("mqtt");
var client  = mqtt.connect('mqtt://test.mosquitto.org');
var variablesPublish = {"AirTemperature": 20, "AirHumidity": 32,"SoilTemperature": "32", "SoilMoisture": 22};
//global.jn=".."
var json = JSON.stringify(variablesPublish);

client.publish("mypub", json, {'qos': 1, 'retain': false},
    function (error, response) {
       // console.log(response);,
    });

/*client.subscribe('mypub')

var c = client.on('message', function (topic, message) {
  // message is Buffer
  console.log(message.toString())
  // jn = message.toString()
  jsnobj = JSON.parse(message)
  //obj=jsnobj
  var no =10;
//console.log(jsnobj.temperature)
console.log(jsnobj)

  //client.end()
//return jsnobj
//return jn;
})
*/
//module.exports = c*?
//exports.c = c;

//console.log(jn)

//module.exports.c=c;
