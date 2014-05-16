var should = require("should");
var xml = require("../lib/xml");

describe('xml', function() {
    describe('object2xml', function() {

        var reference_object = {
            radius : 12345e3,
            mass : 543e21,
        };
    
    
        it('should create xml', function() {
            var data = {
                name : "earth",
                mass : 12,
                radius : 14,
                position : [13, 14 ],
                velocity : [15, 16 ]
            };

            var xml_data = xml.object2xml(data);
            console.log(xml_data);
        });

    });
}) 