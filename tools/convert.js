

module.exports.ephemeris2vector = function(eph) {
    // True anomaly, nu (degrees)
    var TA = eph.TA;

    // Argument of Perifocus, w (degrees)                                
    var W = eph.W;

    // Eccentricity, e                                                   
    var EC = eph.EC;

    // Mean motion, n (degrees/sec)
    var N = eph.N;

    // Semi-major axis, a (km)                                           
    var A = eph.A;

    // Longitude of Ascending Node, OMEGA, (degrees) 
    var OM = eph.OM;


    // -------------------------------

    var data = {};
    data.position=[null, null];
    data.velocity=[null, null];
    
    
    // Radius
    var r = A * (1-EC*EC) / (1 + EC * Math.cos(deg2rad(TA)));

    // X and Y coordinates of object
    data.position[0] = r * Math.cos(deg2rad(TA+W+OM));
    data.position[1] = r * Math.sin(deg2rad(TA+W+OM));

    // Velocities
    var Vr = deg2rad(N) * EC * Math.sin(deg2rad(TA)) * r / Math.sqrt(1-EC*EC);
    var Vnu = deg2rad(N) * A;

    data.velocity[0] = Vr * Math.cos(deg2rad(TA+W+OM)) - Vnu * Math.sin(deg2rad(TA+W+OM));
    data.velocity[1] = Vr * Math.sin(deg2rad(TA+W+OM)) + Vnu * Math.cos(deg2rad(TA+W+OM));

    function deg2rad(degree) {
        return 2*Math.PI/360 * degree;
    }

    return data;
};


