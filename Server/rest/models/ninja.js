const mongoose = require('mongoose');
const Schema = mongoose.Schema;

// create ninja Schema & model
const NinjaSchema = new Schema({
    AirTemperature: {
        type: Number
         //required: [true, 'Name field is required']
    },
    AirHumidity: {
        type: Number
    },
    SoilMoisture: {
        type: Number
    },
    SoilTemperature: {
        type: Number
    },
    Date_Time: {
     type: Date, default: Date.now()
     }
    // add in geo location
});

const Ninja = mongoose.model('ninja', NinjaSchema);

module.exports = Ninja;
