var Telnet = require('./telnet');

var HORIZONS_URI = "horizons.jpl.nasa.gov";
var HORIZONS_PORT = 6775;

var objects = {}
objects["sun"]=10;
objects["mercury"]=199;
objects["venus"]=299;
objects["earth"]=399;
objects["mars"]=499;
objects["jupiter"]=599;
objects["saturn"]=699;
objects["uranus"]=799;
objects["neptune"]=899;

objects["moon"]=301;



module.exports.get = function(object_name, date, callback){
    var object;
    
    var client = new Telnet(HORIZONS_URI, HORIZONS_PORT);
    client.connect();
    client.readUntil(new RegExp("Horizons> "), function(err, res) {
        client.write(objects[object_name] + "\n");
        client.readUntil(new RegExp("<cr>: "), function(err, res) {
            object = parseObjectData(res);
            object.name = object_name;
            if (object_name=="sun") {
                client.close();
                callback(null, object); 
            }
            else {
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
                                                                    object.ephemeris = parseEphemeris(res);
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
            }
        });
    });
};
        

        
function parseObjectData(data) {
    var parser_radius = new Parser();

    parser_radius.add(' Mean radius, km += +([0-9]*.[0-9]*)', function(match) {
       return parseFloat(match[1])*1000; 
    });
    parser_radius.add(' Radius, km += +([0-9]*.[0-9]*)', function(match) {
       return parseFloat(match[1])*1000; 
    });
    parser_radius.add(' Mean radius \\(km\\) += +([0-9]*.[0-9]*)', function(match) {
       return parseFloat(match[1])*1000; 
    });
    parser_radius.add(' Equat. radius \\(1 bar\\) += +([0-9]*.[0-9]*).* km', function(match) {
       return parseFloat(match[1])*1000; 
    });
    parser_radius.add(' Radius \\(photosphere\\) += +([0-9]*.[0-9]*)\\(10\\^([0-9]+)\\) km', function(match) {
       return parseFloat(match[1]+"E"+match[2])*1000; 
    });
    

    var parser_mass = new Parser();

    parser_mass.add(' Mass, 10\\^([0-9]+) kg += +([0-9]*.[0-9]*)', function(match) {
       return parseFloat(match[2]+"E"+match[1]);
    });
    parser_mass.add(' Mass \\(10\\^([0-9]+) kg *\\) +[=~] +([0-9]*.[0-9]*)', function(match) {
       return parseFloat(match[2]+"E"+match[1]);
    });

    var parser_rotation = new Parser();

    parser_rotation.add(' Mean rot. rate, rad s\\^-1 += +([0-9]*.[0-9]*)\\*10\\^([+-]?[0-9]+)', function(match) {
       return parseFloat(match[1]+"E"+match[2]);
    });
    parser_rotation.add(' Rot. rate ?\\(10\\^([+-]?[0-9]+) rad/s\\) += +([0-9]*.[0-9]*)', function(match) {
       return parseFloat(match[2]+"E"+match[1]);
    });
    parser_rotation.add(' Sidereal rot. period += +([+-]?[0-9]*.[0-9]*) (d|hr)', function(match) {
        if (match[2]=="d")
            return 2*Math.PI/(parseFloat(match[1])*24*60*60);
        else if (match[2]=="hr")
            return 2*Math.PI/(parseFloat(match[1])*60*60);
    });
    parser_rotation.add(' Adopted sidereal per += +([+-]?[0-9]*.[0-9]*) (d|hr)', function(match) {
        if (match[2]=="d")
            return 2*Math.PI/(parseFloat(match[1])*24*60*60);
        else if (match[2]=="hr")
            return 2*Math.PI/(parseFloat(match[1])*60*60);
    });
    
    var object = {};
    
    var radius = parser_radius.parse(data);
    if (radius)
        object.radius = radius;
        
    var mass = parser_mass.parse(data);
    if (mass)
        object.mass = mass;
    
    var rotation = parser_rotation.parse(data);
    if (rotation)
        object.rotation = rotation;

    return object;
};

var Parser = function() {
    this.parsers = [];
}

Parser.prototype.add = function(regexp, calc) {
    this.parsers.push({
        pattern: new RegExp(regexp),
        calc: calc
    });
};

Parser.prototype.parse = function(data) {
    var result = null;
    var match;
    
    for (var i=0; i<this.parsers.length; i++) {
        match = this.parsers[i].pattern.exec(data);
        if (match) {
            result = this.parsers[i].calc(match);
            break;
        }
    }
    return result;
}




function parseEphemeris(data) {
    var pattern_EC = new RegExp('EC= ([0-9]+.[0-9]+(E.[0-9]+)?) ');
    var pattern_W = new RegExp('W = ([0-9]+.[0-9]+(E.[0-9]+)?) ');
    var pattern_N = new RegExp('N = ([0-9]+.[0-9]+(E.[0-9]+)?) ');
    var pattern_TA = new RegExp('TA= ([0-9]+.[0-9]+(E.[0-9]+)?)');
    var pattern_A = new RegExp('A = ([0-9]+.[0-9]+(E.[0-9]+)?) ');
    var pattern_OM = new RegExp('OM= ([0-9]+.[0-9]+(E.[0-9]+)?) ');

    var ephemeris = {}
    match = pattern_EC.exec(data);
    if (match) {
        ephemeris.EC=parseFloat(match[1]);
    }

    match = pattern_W.exec(data);
    if (match) {
        ephemeris.W=parseFloat(match[1]);
    }

    match = pattern_N.exec(data);
    if (match) {
        ephemeris.N=parseFloat(match[1]);
    }

    match = pattern_TA.exec(data);
    if (match) {
        ephemeris.TA=parseFloat(match[1]);
    }

    match = pattern_A.exec(data);
    if (match) {
        ephemeris.A=parseFloat(match[1]);
    }
    
    match = pattern_OM.exec(data);
    if (match) {
        ephemeris.OM=parseFloat(match[1]);
    }
    
    return ephemeris;
};

module.exports.objects = objects;
module.exports._parseObjectData = parseObjectData;
module.exports._parseEphemeris = parseEphemeris;
