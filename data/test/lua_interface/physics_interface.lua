math.randomseed(os.time())

io.write("G read from C++: ", pw.universe.G, "\n")
pw.universe.G = 42
io.write("G modified by Lua and read from C++: ", pw.universe.G, "\n")
io.write("Some more object data: \n")
io.write("Mass:    ", pw.universe.get_mass("Mass"), "\n")
io.write("Inertia: ", pw.universe.get_inertia("Mass"), "\n")
io.write("Paused, press P to resume.\n\n")

pw.system.pause()

function physics_interface()
    
    frequency = pw.system.get_frequency()
    Frametime = 1.0/frequency
    
    output()
    
    local F_x = (math.random()*1000.0-500.0) * Frametime
    local F_y = (math.random()*1000.0-500.0) * Frametime
    
    push("Mass", F_x, F_y)
    
    F_x = (math.random()*100.0-50.0) * Frametime
    F_y = (math.random()*100.0-50.0) * Frametime
    
    push("ReferenceMass", F_x, F_y)
end

function output()
    
    io.write("Simulation time (y) ", pw.universe.get_time_years(), "\n")
    io.write("Simulation time (s) ", pw.universe.get_time(), "\n")
    io.write("Position:           ", pw.universe.get_position("Mass"), "\n")
    io.write("Velocity:           ", pw.universe.get_velocity("Mass"), "\n")
    io.write("Angle:              ", pw.universe.get_angle("Mass"), "\n")
    io.write("Angle Ref:          ", pw.universe.get_angle_ref("Mass", "ReferenceMass"), "\n")
    io.write("Angle Velocity:     ", pw.universe.get_angle_vel("Mass"), "\n")
    io.write("Angle Velocity Ref: ", pw.universe.get_angle_vel_ref("Mass", "ReferenceMass"), "\n")
   
end

function push(_Object, _F_x, _F_y)

    pw.universe.apply_force(_Object, _F_x, _F_y, 0.5, 0.0)

end
