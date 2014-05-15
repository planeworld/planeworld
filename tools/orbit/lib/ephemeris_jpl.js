var Telnet = require('./telnet');

var HORIZONS_URI = "horizons.jpl.nasa.gov";
var HORIZONS_PORT = 6775;

var objects = {}
objects["earth"]=399;
objects["moon"]=301;

var object = {}
object.ephemeris = {};


module.exports.get = function(object_name, date, callback){
    var client = new Telnet(HORIZONS_URI, HORIZONS_PORT);
    client.connect();
    client.readUntil(new RegExp("Horizons> "), function(err, res) {
        client.write(objects[object_name] + "\n");
        client.readUntil(new RegExp("<cr>: "), function(err, res) {
            parseObjectData(res);
            client.write("E\n");
            client.readUntil(new RegExp("] : "), function(err, res) {
                client.write("e\n", "ASCII");
                client.readUntil(new RegExp("] : "), function(err, res) {
                    client.write("10\n", "ASCII");
                    client.readUntil(new RegExp("] : "), function(err, res) {
                        client.write("eclip\n", "ASCII");
                        client.readUntil(new RegExp("] : "), function(err, res) {
                            client.write("2014-05-13\n", "ASCII");
                            client.readUntil(new RegExp("] : "), function(err, res) {
                                client.write("2014-05-14\n", "ASCII");
                                client.readUntil(new RegExp("] : "), function(err, res) {
                                    client.write("2d\n", "ASCII");
                                    client.readUntil(new RegExp("] : "), function(err, res) {
                                        client.write("n\n", "ASCII");
                                        client.readUntil(new RegExp("] : "), function(err, res) {
                                            client.write("J2000\n", "ASCII");
                                            client.readUntil(new RegExp("] : "), function(err, res) {
                                                client.write("1\n", "ASCII");
                                                client.readUntil(new RegExp("] : "), function(err, res) {
                                                    client.write("NO\n", "ASCII");
                                                    client.readUntil(new RegExp("] : "), function(err, res) {
                                                        client.write("YES\n", "ASCII");
                                                        client.readUntil(new RegExp("] : "), function(err, res) {
                                                            client.write("ABS\n", "ASCII");
                                                            client.readUntil(new RegExp("\\? : "), function(err, res) {
                                                                parseEphemeris(res);
                                                                client.close();
                                                                callback(null, object);
                                                            });
                                                        });
                                                    });
                                                });
                                            });
                                        });
                                    });
                                });
                            });
                        });
                    });
                });
            });
        });
    });
};
        

        
function parseObjectData(data) {
    var pattern_radius = new RegExp(' Mean radius, km += ([0-9]*.[0-9]*)');
    var pattern_mass = new RegExp(' Mass, 10\\^([0-9]+) kg = ([0-9]*.[0-9]*)');
    
    var match;
    match = pattern_radius.exec(data);
    if (match) {
        object.radius=parseFloat(match[1])*1000;
        console.log("Radius = "+ object.radius);
    }
    match = pattern_mass.exec(data);
    if (match) {
        object.mass=parseFloat(match[2]+"E"+match[1]);
        console.log("Mass = "+ object.mass);
    }
};

function parseEphemeris(data) {
    var pattern_EC = new RegExp('EC= ([0-9]+.[0-9]+(E.[0-9]+)?) ');
    var pattern_W = new RegExp('W = ([0-9]+.[0-9]+(E.[0-9]+)?) ');
    var pattern_N = new RegExp('N = ([0-9]+.[0-9]+(E.[0-9]+)?) ');
    var pattern_TA = new RegExp('TA= ([0-9]+.[0-9]+(E.[0-9]+)?)');
    var pattern_A = new RegExp('A = ([0-9]+.[0-9]+(E.[0-9]+)?) ');
    var pattern_OM = new RegExp('OM= ([0-9]+.[0-9]+(E.[0-9]+)?) ');


    match = pattern_EC.exec(data);
    if (match) {
        object.ephemeris.EC=parseFloat(match[1]);
        console.log("EC = "+ object.ephemeris.EC);
    }

    match = pattern_W.exec(data);
    if (match) {
        object.ephemeris.W=parseFloat(match[1]);
        console.log("W = "+ object.ephemeris.W);
    }

    match = pattern_N.exec(data);
    if (match) {
        object.ephemeris.N=parseFloat(match[1]);
        console.log("N = "+ object.ephemeris.N);
    }

    match = pattern_TA.exec(data);
    if (match) {
        object.ephemeris.TA=parseFloat(match[1]);
        console.log("TA= "+ object.ephemeris.TA);
    }

    match = pattern_A.exec(data);
    if (match) {
        object.ephemeris.A=parseFloat(match[1]);
        console.log("A= "+ object.ephemeris.A);
    }
    
    match = pattern_OM.exec(data);
    if (match) {
        object.ephemeris.OM=parseFloat(match[1]);
        console.log("OM= "+ object.ephemeris.OM);
    }
        
};

