math.randomseed(os.time())

io.write("Mass 1:    ", pw.physics.get_mass("Mass1"), "\n")
io.write("Inertia 1: ", pw.physics.get_inertia("Mass1"), "\n")
io.write("Mass 2:    ", pw.physics.get_mass("Mass2"), "\n")
io.write("Inertia 2: ", pw.physics.get_inertia("Mass2"), "\n")
io.write("Paused, press P to resume.\n\n")

pw.system.pause()

function physics_interface()
    pw.physics.apply_force("Mass1", 0.0, -1.0,  -2.0, 0.0)
    pw.physics.apply_force("Mass2", 0.0, -1.0,   2.0, 0.0)
    pw.physics.apply_force("Mass1", 0.0,  1.0, -12.0, 0.0)
    pw.physics.apply_force("Mass2", 0.0,  1.0,  12.0, 0.0)
end
