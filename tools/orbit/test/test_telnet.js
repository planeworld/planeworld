var should = require("should");
var Telnet = require("../lib/telnet");

describe('telnet', function(){
    var config;
    
    beforeEach( function(){
        config = {};
        config.name = "mybackup";
        config.backuppath = "test/mnt/";
        config.minInterval = 0;
        config.rsync = {
            src : ['lib/'],
            recursive : true
        };
    });
    
    it('should return error if parameter is missing', function(done){
        var client = new Telnet("horizons.jpl.nasa.gov", 6775);
        client.connect();
        client.readUntil(new RegExp("Horizons> "), function(err, res) {
            console.log(res);
            client.write("301\n");
            client.readUntil(new RegExp("<cr>: "), function(err, res) {
                console.log(res);
                done();
            });
        });
    });

}) 