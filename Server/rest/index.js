const express = require('express');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');
var mqtt = require("mqtt");
var client  = mqtt.connect('mqtt://localhost');
const Ninja = require('./models/ninja.js');

// set up express app
const app = express();
 

// connect to mongodb
mongoose.connect('mongodb://localhost/agriserver1');
mongoose.Promise = global.Promise;

// use body-parser middleware
app.use(bodyParser.json());

// initialize routes
app.use('/measure', require('./routes/api'));

// error handling middleware
app.use(function(err, req, res, next){
    console.log(err); // to see properties of message in our console
    res.status(422).send({error: err.message});
});


//used by express first
app.use(express.static('./public'));
app.use(express.static('./src'));

//templating engine
app.set('views', './src/views');
app.set('view engine', 'ejs');





client.subscribe('mypub')
client.on('message', function (topic, message) {
  // message is Buffer
  //console.log(message.toString())
    sensdata = JSON.parse(message)
    Ninja.create(sensdata)
 
//console.log(jsnobj.temperature)
//console.log(jsnobj)

  //client.end()

})


app.listen(process.env.port || 4000, function(){
    console.log('now listening for requests');
});
