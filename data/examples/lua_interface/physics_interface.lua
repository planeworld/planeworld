math.randomseed(os.time())

io.write("Mass:    ", get_mass("Mass"), "\n")
io.write("Inertia: ", get_inertia("Mass"), "\n")

function physics_interface()
    
    frequency = get_frequency()
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
    
    io.write("Simulation time (y) ", get_time_years(), "\n")
    io.write("Simulation time (s) ", get_time(), "\n")
    io.write("Position:           ", get_position("Mass"), "\n")
    io.write("Velocity:           ", get_velocity("Mass"), "\n")
    io.write("Angle:              ", get_angle("Mass"), "\n")
    io.write("Angle Ref:          ", get_angle_ref("Mass", "ReferenceMass"), "\n")
    io.write("Angle Velocity:     ", get_angle_vel("Mass"), "\n")
    io.write("Angle Velocity Ref: ", get_angle_vel_ref("Mass", "ReferenceMass"), "\n")
   
end

function push(_Object, _F_x, _F_y)

    apply_force(_Object, _F_x, _F_y, 0.5, 0.0)

end
