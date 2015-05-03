var http = require("http");
var url = require("url");
var path = require('path');
var fs = require('fs');


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

	/*

	var appName = pathSplit[1];

	console.log(pathSplit);
	console.log("request : " + appName);

	if (appName == "static")
	{
		var filename = pathSplit[2];
		sendFile("static/" + filename, resp);
	} else 	if (appName == "getFirArray") {
		sendFile(firFile, resp);
	} else {
		resp.writeHead(404);
		resp.write("Not Found");
		resp.end();
	}
	*/	

});



httpServer.listen(httpPort);