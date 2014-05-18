

module.exports.object2xml = function(object) {
    var xml = '<!DOCTYPE PlaneworldML>\n';
    xml += '<object type="RigidBody">\n';
    xml += '    <core\n';
    xml += '        name="'+ object.name +'"\n';
    xml += '        mass="'+ object.mass +'"\n';
    xml += '        origin_x="'+ object.position[0] +'"\n';
    xml += '        origin_y="'+ object.position[1] +'"\n';
    xml += '        velocity_x="'+ object.velocity[0] +'"\n';
    xml += '        velocity_y="'+ object.velocity[1] +'"\n';
    xml += '        angle_velocity="'+ object.rotation * 2*Math.PI/360 +'"\n';
    xml += '        dynamics="true"\n';
    xml += '        gravity="true"\n';
    xml += '    />\n';
    xml += '    <shape\n';
    xml += '        type="Circle"\n';
    xml += '        radius="'+ object.radius +'"\n';
    xml += '        center_x="0.0"\n';
    xml += '        center_y="0.0">\n';
    xml += '        <visuals type="Circle"/>\n';
    xml += '    </shape>\n';
    xml += '</object>\n';

    return xml;
};


