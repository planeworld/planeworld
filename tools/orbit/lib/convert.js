

module.exports.ephemeris2vector = function(eph) {
    // True anomaly, nu (rad)
    var TA = deg2rad(eph.TA);

    // Argument of Perifocus, w (rad)                                
    var W = deg2rad(eph.W);

    // Eccentricity, e                                                   
    var EC = eph.EC;

    // Mean motion, n (rad/sec)
    var N = deg2rad(eph.N);

    // Semi-major axis, a (m)                                           
    var A = eph.A*1000;

    // Longitude of Ascending Node, OMEGA, (rad) 
    var OM = deg2rad(eph.OM);


    // -------------------------------

    var data = {};
    data.position=[null, null];
    data.velocity=[null, null];
    
    
    // Radius
    var r = A * (1-EC*EC) / (1 + EC * Math.cos(TA));

    // X and Y coordinates of object
    data.position[0] = r * Math.cos(TA+W+OM);
    data.position[1] = r * Math.sin(TA+W+OM);

    // Velocities
    var Vr = N * EC * Math.sin(TA) * r / Math.sqrt(1-EC*EC);
    var Vnu = N * A;

    data.velocity[0] = Vr * Math.cos(TA+W+OM) - Vnu * Math.sin(TA+W+OM);
    data.velocity[1] = Vr * Math.sin(TA+W+OM) + Vnu * Math.cos(TA+W+OM);

    function deg2rad(degree) {
        return 2*Math.PI/360 * degree;
    }

    return data;
};


