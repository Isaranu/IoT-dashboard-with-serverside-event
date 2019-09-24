var express = require('express');
var app = express();
const cors = require('cors');
var port = 3030;

const bodyParser = require('body-parser');
var jsonParser = bodyParser.json();
var urlencodedParser = bodyParser.urlencoded({extended : false});

const EventEmitter = require('eventemitter3');
const emitter = new EventEmitter();

var path = require('path');

app.use(express.static(path.join(__dirname, 'html')));
app.use(cors());

app.get('/', function(req, res){

  res.writeHead(200, {
    "Content-Type": "text/event-stream",
    "Cache-Control": "no-cache",
    Connection: "keep-alive"
  });

  emitter.on('message', function(data){
    res.write(`data: ${JSON.stringify(data)}\n\n`);
  });

});

app.post('/buttonstatus', urlencodedParser, function(req, res){
  console.log(req.body);
  emitter.emit('message', {buttonstatus:req.body.buttonstatus, buttoncnt:req.body.buttoncnt});
  res.jsonp({status:"push event successful"});
});

app.get('/testeventemitter', function(req, res){
  res.sendFile(path.join(__dirname + '/html/testeventemitter.html'));
});

app.listen(port, function(){
  console.log('Server side event emitter started !');
});
