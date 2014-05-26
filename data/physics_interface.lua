globaltest = 0;

function physics_interface()
    x, y = get_position("Rigidbody_1")
    --io.write("Position: ",x, ", ", y, "\n")
    io.write(globaltest,"\n");
    globaltest = globaltest +1;
    apply_force("Rigidbody_1", 1.0e13,0.0,0.0,0.0)
end
