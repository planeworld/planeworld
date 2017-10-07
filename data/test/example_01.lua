-- EXAMPLE_01.LUA
-----------------
-- This is a work in progress example using several different commands. It is
-- mainly used for testing purposes, so there is no meaningful content, yet.


-- KNOWN ISSUES
---------------

-- Setup engine
pw.system.set_frequency_input(100)
pw.system.set_frequency_lua(30)
pw.system.set_frequency_physics(200)
pw.system.set_frequency_visuals(60)
pw.system.set_data_path_visuals("../")
pw.system.create_universe(23479, 10000)
-- pw.physics.set_gravity_vector(0.0, -9.81)
pw.system.win_main_resize(1800, 900)
idCam01 = pw.system.create_camera()
pw.visuals.toggle_grid()

-- Create an object
idObj01 = pw.system.create_obj()
idShp01 = pw.system.create_shp("shp_circle")
idShp02 = pw.system.create_shp("shp_polygon")

pw.system.shp_set_vertices(idShp02,
    {-1.0, -1.0,
      1.0, -1.0,
      2.0,  1.0,
     -2.0,  1.0}
)

pw.system.obj_add_shp(idObj01, idShp01)
pw.system.obj_add_shp(idObj01, idShp02)

idObjEarth=pw.system.create_obj()
pw.system.obj_add_shp(idObjEarth, pw.system.create_shp("shp_planet"))
pw.physics.obj_set_position(idObjEarth, 0.0, -1000.0)

-- Create a text window
idWin01=pw.system.create_window()
idWdg01=pw.system.create_widget("text")
pw.system.widget_set_text(idWdg01, "Welcome to planeworld, press p to start")
pw.system.win_set_widget(idWin01, idWdg01)
pw.system.win_center_keep(idWin01)

-- Create camera view in new window
idWin02=pw.system.create_window()
idWdg02=pw.system.create_widget("camera")
idCam02=pw.system.create_camera()
pw.system.cam_attach_to(idCam02, idObj01)
pw.system.win_set_title(idWin02, "Camera 01")
pw.system.widget_set_cam(idWdg02, idCam02)
pw.system.win_set_widget(idWin02, idWdg02)
pw.system.win_set_position(idWin02, 500, 100)

-- Create a particle emitter
idEm01 = pw.system.create_emitter("particle")
idPa01 = pw.system.create_particles("dot")
pw.system.emitter_set_particles(idEm01, idPa01)
pw.system.emitter_set_distribution(idEm01, "point_source")
pw.system.emitter_set_mode(idEm01, "timed")
pw.system.emitter_set_frequency(idEm01, 100)
pw.system.emitter_set_number(idEm01, 2000)
pw.system.emitter_set_angle(idEm01, 1.57)
pw.system.emitter_set_angle_std(idEm01, 0.2)
pw.system.emitter_set_velocity(idEm01, 10.0)
pw.system.emitter_set_velocity_std(idEm01, 1.0)

-- Create a thruster
idThruster01 = pw.system.create_thruster()
pw.system.thruster_set_object(idThruster01, idObj01)
pw.system.thruster_set_emitter(idThruster01, idEm01)
pw.sim.thruster_activate(idThruster01)

-- Pause simulation
pw.system.pause()

-- Setup callback functions
pw.system.register_lua_callback("e_lua_update", "update")
pw.system.register_lua_callback("e_lua_update", "update_earth")

Force = 1.0
function update()
    pw.sim.thruster_activate(idThruster01, Force);
end

function update_earth()
    pw.physics.obj_apply_force(idObjEarth, 0.0, -Force, 0.0, 0.0)
    Force = Force + 1.0
end
