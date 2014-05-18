var net = require('net');


var objects = {}
objects["earth"]=399;
objects["moon"]=301;

var object = {}
object.ephemeris = {};

if (!Array.prototype.last){
    Array.prototype.last = function(){
        return this[this.length - 1];
    };
};

var Dict = function(){};

Dict.prototype.getCommand = function(value) {
    var command = null;
    for (var key in this) {
        if (this.hasOwnProperty(key) && this[key] == value) {
            command = key;
            break;
        }
    }
    return command;
};

var tlnt = new Dict();
var ascii = new Dict();

// ASCII
ascii.LF = 10; // Line Feed
ascii.CR = 13; // Carriage Return
ascii.ESC = 27; // Escape

// http://www.networksorcery.com/enp/rfc/rfc857.txt
tlnt.ECHO =   1; // Echoing data characters it receives

// http://www.networksorcery.com/enp/rfc/rfc858.txt
tlnt.SUPPRESSGOAHEAD =   3; // Suppressing transmission of the TELNET GO AHEAD (GA) character

// http://www.networksorcery.com/enp/rfc/rfc1091.txt
tlnt.TERMINALTYPE =  24; //Willing to send terminal type information

// http://tools.ietf.org/rfc/rfc1073.txt
tlnt.NAWS =  31; // Negotiate About Window Size

// http://tools.ietf.org/rfc/rfc854.txt
tlnt.SE   = 240; // End of subnegotiation parameters.
tlnt.NOP  = 241; // No operation.
tlnt.DM   = 242; // Data Mark:
                // The data stream portion of a Synch.
                // This should always be accompanied
                // by a TCP Urgent notification.
tlnt.BRK  = 243; // NVT character break.
tlnt.IP   = 244; // Interrupt Process function 
tlnt.AO   = 245; // Abort output function.
tlnt.AYT  = 246; // Are You There function.
tlnt.EC   = 247; // Erase character function.
tlnt.EL   = 248; // Erase Line function.
tlnt.GA   = 249; // Go ahead signal.
tlnt.SB   = 250; // Indicates that what follows is
                // subnegotiation of the indicated
                // option.
tlnt.WILL = 251; // Indicates the desire to begin
                // performing, or confirmation that
                // you are now performing, the
                // indicated option.
tlnt.WONT = 252; // Indicates the refusal to perform,
                // or continue performing, the
                // indicated option.
tlnt.DO   = 253; // Indicates the request that the
                // other party perform, or
                // confirmation that you are expecting
                // the other party to perform, the
                // indicated option.
tlnt.DONT = 254; // Indicates the demand that the
                // other party stop performing,
                // or confirmation that you are no
                // longer expecting the other party
                // to perform, the indicated option.
tlnt.IAC  = 255; // Interpret as command


var Telnet = function(uri, port){
    this.client = new net.Socket();
    this.uri = uri;
    this.port = port;
    
    this.data="";
    
    this.readuntil_callback=null;
    this.readuntil_regexp=null;
    this.readuntil_data="";
    
    
    this.client.on("error", function(err) {
        if (err.errno == "ENOTFOUND") {
            this.console.log("Host " + host + " not found.");
        }
        else {
            this.console.log("Error");
        }
    });

    this.client.on("end", function() {
        console.log("Connection closed");
    });
};

Telnet.prototype.connect = function(callback) {
    var client=this;
    this.client.on("readable", function(){ client._parse()});
    this.client.connect(this.port, this.uri);
};

Telnet.prototype.close = function() {
    this.client.end();
};


Telnet.prototype.readUntil = function(regexp, callback) {
    this.readuntil_callback = callback;
    this.readuntil_regexp = regexp;

    this._readUntil();
}

Telnet.prototype.write = function(data) {
    this.client.write(data, "ASCII");
    console.log("Sending: " + data);
}

Telnet.prototype._readUntil = function() {
    if (this.readuntil_callback) {
        var endpos=null;
        var match = this.readuntil_regexp.exec(this.data);
        if (match) {
            endpos = match.index+match[0].length;
            var tmp_data=this.data.slice(0, endpos);
            this.data=this.data.slice(endpos);

            var tmp_callback = this.readuntil_callback;
            this.readuntil_callback=null;
            this.readuntil_regexp=null;
            
            tmp_callback(null, tmp_data);
        }
    }
}


Telnet.prototype._parse = function() {
    var data;
    
    while (null !== (data=this.client.read(1))) {
        data=data[0];
        
        if (data==tlnt.IAC) {
            var command = tlnt.getCommand(data);
            
            data=this.client.read(1)[0];
            command += " " + tlnt.getCommand(data);
            
            
            if (data == tlnt.WILL || data == tlnt.WONT || data == tlnt.DO || data == tlnt.DONT) {
                data=this.client.read(1)[0];
                command += " " + tlnt.getCommand(data);
                if (data == tlnt.NAWS) {
                    console.log("Negotiate About Window Size")
                    this.client.write(new Buffer([tlnt.IAC, tlnt.WILL, tlnt.NAWS]));
                    this.client.write(new Buffer([tlnt.IAC, tlnt.SB, tlnt.NAWS, 0, 80, 0, 100, tlnt.IAC, tlnt.SE]));
                    
                    
                }
                else
                {
                    console.log("Command unknown: " + command);
                }
            }
            else if (data == tlnt.SB) {
                data=this.client.read(1)[0];
                command += " " + tlnt.getCommand(data);
                var payload = "";
                while (tlnt != (data=this.client.read(1)[0])) {
                    command += " " + tlnt.getCommand(data);
                }
                command += " " + tlnt.getCommand(data);
                
                data=this.client.read(1)[0];
                command += " " + tlnt.getCommand(data);
                
                if (data != tlnt.SE) {
                    console.log("Wrong subnegotiation: " + command);
                }
                else {
                    console.log("Ignoring subnegotiation: " + command);
                }
            }
            else {
                console.log("Command unknown: " + command);
            }
        }
        else if (data<32 && data!=ascii.CR && data!=ascii.LF)
        {
            this.data+="["+data+"]";
        }
        else {
            this.data+=String.fromCharCode(data);
        }
    }
    this._readUntil();
};

module.exports = Telnet;
