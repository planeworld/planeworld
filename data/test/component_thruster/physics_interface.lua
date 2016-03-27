math.randomseed(os.time())

K_p = 50000.0
K_v = 100000.0

Object = {}
Object["name"] = "RocketBody"

ObjectRef = {}
ObjectRef["name2"] = "Earth"

pw.system.pause()

function physics_interface()
    
    frequency = pw.system.get_frequency()
    Frametime = 1.0/frequency
    
--     pw.universe.apply_force(Object["name"], "0.0", math.random()*200000000.0 - 100000000.0, "0.0", "0.0")
    
    Object["p_x"], Object["p_y"] = pw.universe.get_position_ref(Object["name"], ObjectRef["name2"])
    Object["v_x"], Object["v_y"] = pw.universe.get_velocity_ref(Object["name"], ObjectRef["name2"])
    
    output(Object)
    
    local F_x = 0.0
    local F_y = 100.0 * Frametime

--     if Object["p_y"] < 1000.0 then
--         pw.sim.activate_thruster("MainThruster",  K_p * (1000.0-Object["p_y"]) + K_v * (0.0 - Object["v_y"]))
--         pw.sim.activate_thruster("BoosterLeft",  K_p * (1000.0-Object["p_y"]) + K_v * (0.0 - Object["v_y"]))
--         pw.sim.activate_thruster("BoosterRight",  K_p * (1000.0-Object["p_y"]) + K_v * (0.0 - Object["v_y"]))
--         pw.sim.activate_thruster("ControllerThrusterLeft", 5000)
--         pw.sim.activate_thruster("ControllerThrusterRight", 5000)
--     end
--     if Object["p_y"] > 200.0 then
--         pw.sim.deactivate_thruster("ControllerThrusterLeft")
--     end
    
    if Object["v_y"] > 200.0 then
        pw.system.set_frequency(0.5)
    end
    
end

function output(_Object)
    
    io.write("Position:  ", _Object["p_x"], ", ", _Object["p_y"], "\n")
    io.write("Velocity:  ", _Object["v_x"], ", ", _Object["v_y"], "\n")
    
end

