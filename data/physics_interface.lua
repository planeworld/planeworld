function physics_interface()
    x, y = get_position("Earth")
    io.write("Position: ",x, ", ", y, "\n")
    apply_acceleration("Earth", -100000.0,0.0)
end
