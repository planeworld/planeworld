math.randomseed(os.time())

K_p = 50000.0
K_v = 100000.0

Object = {}
Object["name"] = "RocketBody"

function physics_interface()
    
    frequency, factor = get_frequency()
    Frametime = 1.0/frequency * factor
    
--     apply_force(Object["name"], "0.0", math.random()*200000000.0 - 100000000.0, "0.0", "0.0")
    
    Object["p_x"], Object["p_y"] = get_position(Object["name"])
    Object["v_x"], Object["v_y"] = get_velocity(Object["name"])
    
    output(Object)
    
    local F_x = 0.0
    local F_y = 100.0 * Frametime

--     if Object["p_y"] < 1000.0 then
        activate_thruster("MainThruster",  K_p * (1000.0-Object["p_y"]) + K_v * (0.0 - Object["v_y"]))
        activate_thruster("BoosterLeft",  K_p * (1000.0-Object["p_y"]) + K_v * (0.0 - Object["v_y"]))
        activate_thruster("BoosterRight",  K_p * (1000.0-Object["p_y"]) + K_v * (0.0 - Object["v_y"]))
        activate_thruster("ControllerThrusterLeft", 5000)
        activate_thruster("ControllerThrusterRight", 5000)
--     end
    if Object["p_y"] > 200.0 then
        deactivate_thruster("ControllerThrusterLeft")
    end
end

function output(_Object)
    
    io.write("Position:  ", _Object["p_x"], ", ", _Object["p_y"], "\n")
    io.write("Velocity:  ", _Object["v_x"], ", ", _Object["v_y"], "\n")
    
end

