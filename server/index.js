var http = require("http");
var url = require("url");
var path = require('path');
var fs = require('fs');
var io = require("socket.io");


var mime = require("./mime");

var config = require("./config");


var httpPort = Number(process.argv[2]);

var sendFile = function(filename, resp) {
	var extension = path.extname(filename);
	var contentType = mime[extension];
	resp.writeHead(200, {"Content-Type": contentType,"Access-Control-Allow-Origin" : "*"});
	var file = fs.readFileSync(filename);
	resp.write(file);
	resp.end();
}

var httpServer = http.createServer(function(req, resp) {
	var urlObj = url.parse(req.url, true);
	var pathSplit = urlObj.pathname.split("/");
	
	//default index: 
	if (pathSplit.length==2 && pathSplit[1].length==0) {
		sendFile(config.defaultIndex, resp);
	} else {
		//send static resource
		if (pathSplit[1] == "static") {
			var filename = config.staticDir + pathSplit[2];
			if (fs.existsSync(filename)) {
				sendFile(filename, resp);
			} else {
				resp.writeHead(404, {});
				resp.write("static file not found");
				resp.end();
			}
		}
	}

});

var ioServer = io(httpServer);

var firstClient = undefined;

var count = 1;

var IoClient = function(socket)
{
	var self = this;

	self.previousClient = undefined;
	self.nextClient = undefined;
	self.count = count;
	count += 1;
	//initialize
	socket.on("disconnect", function(){
		if (self.previousClient == undefined) {
			firstClient = self.nextClient;

			if (self.nextClient != undefined) {
				self.nextClient.previousClient = this.previousClient;
			}
		} else {
			self.previousClient.nextClient = self.nextClient;
			if (self.nextClient != undefined) {
				self.nextClient.previousClient = this.previousClient;
			}
		}
	});

	socket.on("test", function(message) {
		console.log("tst message from: " + self.count);
		console.log(message);
	});


	self.insertNew = function(anotherClient) {
		if (self.nextClient == undefined) {
			self.nextClient = anotherClient;
			anotherClient.previousClient = self;
		} else {
			self.nextClient.insertNew(anotherClient);
		}
	}
}

var trivialClient = function()
{
	var currentClient = firstClient;
	while (currentClient != undefined) {
		console.log(currentClient.count);
		currentClient = currentClient.nextClient;
	}
}

ioServer.on("connection", function(socket) {
	var client = new IoClient(socket);
	if (firstClient == undefined) {
		firstClient = client;
	} else {
		firstClient.insertNew(client);
	}

	trivialClient();
});

httpServer.listen(httpPort);