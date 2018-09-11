const express = require ('express');
const router = express.Router();
const Ninja = require('../models/ninja');

// get a list of sensors from the db
router.get('/test', function(req, res, next){
     Ninja.find({},{"_id":0,"__v":0}).then(function(ninja){
        res.send(ninja); })
});

/*router.get('/', function (req, res) {
    res.render('home', {
        title: 'Smart Agriculture IoT'
    });
});*/

router.get('/temperature', function (req, res) {
    res.render('temp', {
        title: 'Temperature'
    });
});

router.get('/moisture', function (req, res) {
    res.render('moist', {
        title: 'Moisture'
    });
});
router.get('/humidity', function (req, res) {
    res.render('humid', {
        title: 'Humidity'
    });
});



/*router.get('/', function(req, res, next){
     Ninja.find({},function(err,docs){

        if (err) {
                        console.error(err);
                        res.statusCode = 500;
                        res.send({
                            result: 'error',
                            err:    err.code
                        });
                    }
        res.send(JSON.stringify(docs)); })
});*/
/*// add a new sensors to the db
router.post('/ninjas', function(req, res, next){
    Ninja.create(req.body).then(function(ninja){
        res.send(ninja);
    }).catch(next);
});

// update a sensors the db
router.put('/ninjas/:id', function(req, res, next){
    Ninja.findByIdAndUpdate({_id: req.params.id}, req.body).then(function(){
        Ninja.findOne({_id: req.params.id}).then(function(ninja){
            res.send(ninja);
        });
    }).catch(next);
});

// delete a sensor from the db
router.delete('/ninjas/:id', function(req, res, next){
    Ninja.findByIdAndRemove({_id: req.params.id}).then(function(ninja){
        res.send(ninja);
    }).catch(next);
});
*/
module.exports = router;
