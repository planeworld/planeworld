var net = require('net');

var HORIZONS_URI = "horizons.jpl.nasa.gov";
var HORIZONS_PORT = 6775;

var startsequence = new Buffer([255, 251, 1, 255, 251, 3, 255, 253, 31, 255, 253, 24]);
var startsequencecounter = 12;
var client = new net.Socket();

var objects = {}
objects["earth"]=399;
objects["moon"]=301;

var object = {}
object.ephemeris = {};


module.exports.get = function(object_name, date, callback){
//    var client = new net.Socket();
    //client.setEncoding("ascii");
    
    client.setTimeout(10000, function() {
        client.destroy();
        callback(new Error("Timeout for host " + HORIZONS_URI));
    });
    
    client.on("data", data_handlers[0]);
    
    client.on("error", function(err) {
        if (err.errno == "ENOTFOUND") {
            callback("Host " + host + " not found.");
        }
        else {
            callback(err);
        }
    });

    client.on("end", function() {
        console.log("Connection closed");
        console.log(object);
        callback(null, object);
    });

    client.connect(HORIZONS_PORT, HORIZONS_URI);
    
};

var data_handlers = [];

data_handlers.push(function(data) {
    // http://tools.ietf.org/rfc/rfc854.txt
    // http://tools.ietf.org/rfc/rfc1073.txt
    console.log("Handshake");
    console.log(data)
    startsequencecounter-=data.length;
    if (startsequencecounter<=0) {
        client.setEncoding("ascii");
        client.on("data", data_handlers[1]);
        client.removeListener("data", data_handlers.shift());
    }
    
    if (startsequencecounter<0) {
        console.log("Remainig data discarded");
    }
 
    client.write(new Buffer([255, 251, 31]));
    client.write(new Buffer([255, 250, 31, 0, 80, 0, 100, 255, 240]));
    
});

data_handlers.push(function(data) {
    console.log("Prompt");
    var prompt_pattern = new RegExp('Horizons> ');
    lines = data.split();
    for (i=0; i<lines.length; i++) {
        if (prompt_pattern.test(lines[i])) {
            console.log("Prompt found");
            client.write("399\n");
            client.on("data", data_handlers[1]);
            client.removeListener("data", data_handlers.shift());
        }
    }
});

data_handlers.push(function(data) {
    console.log(data)
    var prompt_pattern = new RegExp('<cr>: ');
    
    var pattern_radius = new RegExp(' Mean radius, km += ([0-9]*.[0-9]*)');
    var pattern_mass = new RegExp(' Mass, 10\\^([0-9]+) kg = ([0-9]*.[0-9]*)');
    
    var match;
    lines = data.split();
    for (i=0; i<lines.length; i++) {
        if (prompt_pattern.test(lines[i])) {
            console.log("Prompt found");
            client.write("E\n");
            client.on("data", data_handlers[1]);
            client.removeListener("data", data_handlers.shift());
        }
        match = pattern_radius.exec(lines[i]);
        if (match) {
            object.radius=parseFloat(match[1])*1000;
            console.log("Radius = "+ object.radius);
        }
        match = pattern_mass.exec(lines[i]);
        if (match) {
            object.mass=parseFloat(match[2]+"E"+match[1]);
            console.log("Mass = "+ object.mass);
        }
    }
});

data_handlers.push(function(data) {
    var prompt_pattern = new RegExp('] : ');
    lines = data.split();
    for (i=0; i<lines.length; i++) {
        if (prompt_pattern.test(lines[i])) {
            console.log("Prompt found");
            client.write("e\n");
            client.on("data", data_handlers[1]);
            client.removeListener("data", data_handlers.shift());
        }
    }
});

data_handlers.push(function(data) {
    var prompt_pattern = new RegExp('] : ');
    lines = data.split();
    for (i=0; i<lines.length; i++) {
        if (prompt_pattern.test(lines[i])) {
            console.log("Prompt found");
            client.write("10\n");
            client.on("data", data_handlers[1]);
            client.removeListener("data", data_handlers.shift());
        }
    }
});

data_handlers.push(function(data) {
    var prompt_pattern = new RegExp('] : ');
    lines = data.split();
    for (i=0; i<lines.length; i++) {
        if (prompt_pattern.test(lines[i])) {
            console.log("Prompt found");
            client.write("eclip\n");
            client.on("data", data_handlers[1]);
            client.removeListener("data", data_handlers.shift());
        }
    }
});

data_handlers.push(function(data) {
    var prompt_pattern = new RegExp('] : ');
    lines = data.split();
    for (i=0; i<lines.length; i++) {
        if (prompt_pattern.test(lines[i])) {
            console.log("Prompt found");
            client.write("2014-05-13\n");
            client.on("data", data_handlers[1]);
            client.removeListener("data", data_handlers.shift());
        }
    }
});

data_handlers.push(function(data) {
    var prompt_pattern = new RegExp('] : ');
    lines = data.split();
    for (i=0; i<lines.length; i++) {
        if (prompt_pattern.test(lines[i])) {
            console.log("Prompt found");
            client.write("2014-05-14\n");
            client.on("data", data_handlers[1]);
            client.removeListener("data", data_handlers.shift());
        }
    }
});

data_handlers.push(function(data) {
    var prompt_pattern = new RegExp('] : ');
    lines = data.split();
    for (i=0; i<lines.length; i++) {
        if (prompt_pattern.test(lines[i])) {
            console.log("Prompt found");
            client.write("2d\n");
            client.on("data", data_handlers[1]);
            client.removeListener("data", data_handlers.shift());
        }
    }
});


data_handlers.push(function(data) {
    var prompt_pattern = new RegExp('] : ');
    lines = data.split();
    for (i=0; i<lines.length; i++) {
        if (prompt_pattern.test(lines[i])) {
            console.log("Prompt found");
            client.write("n\n");
            client.on("data", data_handlers[1]);
            client.removeListener("data", data_handlers.shift());
        }
    }
});

data_handlers.push(function(data) {
    var prompt_pattern = new RegExp('] : ');
    lines = data.split();
    for (i=0; i<lines.length; i++) {
        if (prompt_pattern.test(lines[i])) {
            console.log("Prompt found");
            client.write("J2000\n");
            client.on("data", data_handlers[1]);
            client.removeListener("data", data_handlers.shift());
        }
    }
});

data_handlers.push(function(data) {
    var prompt_pattern = new RegExp('] : ');
    lines = data.split();
    for (i=0; i<lines.length; i++) {
        if (prompt_pattern.test(lines[i])) {
            console.log("Prompt found");
            client.write("1\n");
            client.on("data", data_handlers[1]);
            client.removeListener("data", data_handlers.shift());
        }
    }
});

data_handlers.push(function(data) {
    var prompt_pattern = new RegExp('] : ');
    lines = data.split();
    for (i=0; i<lines.length; i++) {
        if (prompt_pattern.test(lines[i])) {
            console.log("Prompt found");
            client.write("NO\n");
            client.on("data", data_handlers[1]);
            client.removeListener("data", data_handlers.shift());
        }
    }
});

data_handlers.push(function(data) {
    var prompt_pattern = new RegExp('] : ');
    lines = data.split();
    for (i=0; i<lines.length; i++) {
        if (prompt_pattern.test(lines[i])) {
            console.log("Prompt found");
            client.write("YES\n");
            client.on("data", data_handlers[1]);
            client.removeListener("data", data_handlers.shift());
        }
    }
});

data_handlers.push(function(data) {
    var prompt_pattern = new RegExp('] : ');
    lines = data.split();
    for (i=0; i<lines.length; i++) {
        if (prompt_pattern.test(lines[i])) {
            console.log("Prompt found");
            client.write("ABS\n");
            client.on("data", data_handlers[1]);
            client.removeListener("data", data_handlers.shift());
        }
    }
});

data_handlers.push(function(data) {
    console.log("last data");
    console.log(data);
    var prompt_pattern = new RegExp(' : ');

    var pattern_EC = new RegExp('EC= ([0-9]+.[0-9]+(E.[0-9]+)?) ');
    var pattern_W = new RegExp('W = ([0-9]+.[0-9]+(E.[0-9]+)?) ');
    var pattern_N = new RegExp('N = ([0-9]+.[0-9]+(E.[0-9]+)?) ');
    var pattern_TA = new RegExp('TA= ([0-9]+.[0-9]+(E.[0-9]+)?)');
    var pattern_A = new RegExp('A = ([0-9]+.[0-9]+(E.[0-9]+)?) ');
    var pattern_OM = new RegExp('OM= ([0-9]+.[0-9]+(E.[0-9]+)?) ');


    lines = data.split();
    for (i=0; i<lines.length; i++) {
        if (prompt_pattern.test(lines[i])) {
            console.log("Prompt found");
            client.end()
        }
        match = pattern_EC.exec(lines[i]);
        if (match) {
            object.ephemeris.EC=parseFloat(match[1]);
            console.log("EC = "+ object.ephemeris.EC);
        }

        match = pattern_W.exec(lines[i]);
        if (match) {
            object.ephemeris.W=parseFloat(match[1]);
            console.log("W = "+ object.ephemeris.W);
        }

        match = pattern_N.exec(lines[i]);
        if (match) {
            object.ephemeris.N=parseFloat(match[1]);
            console.log("N = "+ object.ephemeris.N);
        }

        match = pattern_TA.exec(lines[i]);
        if (match) {
            object.ephemeris.TA=parseFloat(match[1]);
            console.log("TA= "+ object.ephemeris.TA);
        }

        match = pattern_A.exec(lines[i]);
        if (match) {
            object.ephemeris.A=parseFloat(match[1]);
            console.log("A= "+ object.ephemeris.A);
        }
        
        match = pattern_OM.exec(lines[i]);
        if (match) {
            object.ephemeris.OM=parseFloat(match[1]);
            console.log("OM= "+ object.ephemeris.OM);
        }
        
    }
});

