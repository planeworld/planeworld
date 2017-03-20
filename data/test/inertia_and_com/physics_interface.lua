math.randomseed(os.time())

uid_m1 = pw.system.get_uid("Mass1")
uid_m2 = pw.system.get_uid("Mass2")

io.write("Mass 1:    ", pw.physics.obj_get_mass(uid_m1), "\n")
io.write("Inertia 1: ", pw.physics.obj_get_inertia(uid_m1), "\n")
io.write("Mass 2:    ", pw.physics.obj_get_mass(uid_m2), "\n")
io.write("Inertia 2: ", pw.physics.obj_get_inertia(uid_m2), "\n")
io.write("Paused, press P to resume.\n\n")

pw.system.pause()

function physics_interface()
    pw.physics.obj_apply_force(uid_m1, 0.0, -1.0,  -2.0, 0.0)
    pw.physics.obj_apply_force(uid_m2, 0.0, -1.0,   2.0, 0.0)
    pw.physics.obj_apply_force(uid_m1, 0.0,  1.0, -12.0, 0.0)
    pw.physics.obj_apply_force(uid_m2, 0.0,  1.0,  12.0, 0.0)
end
