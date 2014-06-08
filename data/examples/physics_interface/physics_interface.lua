math.randomseed(os.time())

Object = {}
Object["name"] = "Mass"

function physics_interface()
    
    frequency, factor = get_frequency()
    Frametime = 1.0/frequency * factor
    
    Object["p_x"], Object["p_y"] = get_position(Object["name"])
    Object["v_x"], Object["v_y"] = get_velocity(Object["name"])
    
    output(Object)
    
    local F_x = (math.random()*1000.0-500.0) * Frametime
    local F_y = (math.random()*1000.0-500.0) * Frametime
    
    push(Object, F_x, F_y)
end

function output(_Object)
    
    io.write("Position:  ", _Object["p_x"], ", ", _Object["p_y"], "\n")
    io.write("Velocity:  ", _Object["v_x"], ", ", _Object["v_y"], "\n")
    
end

function push(_Object, _F_x, _F_y)

    apply_force(_Object["name"], _F_x, _F_y, 0.0, 0.0)

end
