-- EXAMPLE_01.LUA
-----------------
-- This is a work in progress example using several different commands. It is
-- mainly used for testing purposes, so there is no meaningful content, yet.


-- KNOWN ISSUES
---------------
        
-- Import input script
require('pw_input_default')

-- Setup engine
pw.system.set_frequency_input(100)
pw.system.set_frequency_lua(30)
pw.system.set_frequency_physics(200)
pw.system.set_frequency_visuals(60)

-- Data path has to be set if modified shaders or fonts shall be used 
-- pw.system.set_data_path_visuals("../../")
pw.system.create_universe(23479, 10000)
-- pw.physics.set_gravity_vector(0.0, -9.81)

pw.system.init_visuals()
pw.system.init_physics()
pw.visuals.toggle_grid()
-- pw.system.toggle_fullscreen()

-- Create objects
idObj01 = pw.system.create_obj()

idShp01 = pw.system.create_shp("shp_circle")
idShp02 = pw.system.create_shp("shp_polygon")
pw.physics.obj_set_name(idObj01, "Rocket")
pw.system.shp_set_vertices(idShp02,
    {-2.0, -1.0,
      2.0, -1.0,
      1.0,  1.0,
     -1.0,  1.0}
)

pw.system.obj_add_shp(idObj01, idShp01)
pw.system.obj_add_shp(idObj01, idShp02)
pw.physics.obj_set_position(idObj01, -200.0, 6000010.0)
pw.physics.obj_disable_gravitation(idObj01)

idObjEarth = pw.system.create_obj_planet()
pw.physics.obj_planet_set_radius(idObjEarth, 6000000.0)
pw.physics.obj_planet_set_mass(idObjEarth, 5.972e24)
pw.physics.obj_planet_atmosphere_set_scaleheight(idObjEarth, 1.0e4) -- Barometric formula, every 1km, pressure is reduced by factor ~2.7 (e)
pw.physics.obj_planet_atmosphere_set_pressure_at_ground(idObjEarth, 1.0e5) -- 100 kPa
pw.physics.obj_set_name(idObjEarth, "Earth")
pw.physics.obj_set_inertia(idObjEarth, 8.008e37)
pw.physics.obj_set_position(idObjEarth, 0.0, -6000000.0)
pw.physics.obj_set_velocity(idObjEarth, 2000.0, 0.0)
-- pw.physics.obj_set_angle(idObjEarth, math.rad(20))
-- pw.physics.obj_set_angle_vel(idObjEarth, 7.292115e-5)
pw.physics.obj_refer_state(idObj01, idObjEarth)

-- Setup main camera
idCam01 = pw.system.create_camera()
pw.system.cam_set_position(idCam01, -200.0, 6000100.0)
pw.system.cam_attach_to(idCam01, idObjEarth)

-- Create a text window
idWin01=pw.system.create_window()
idWdg01=pw.system.create_widget("text")
pw.system.widget_set_text(idWdg01, "\n\n\nWelcome to planeworld, press p to start")
pw.system.widget_set_frame_style(idWdg01, "no_background")
pw.system.win_set_widget(idWin01, idWdg01)
pw.system.win_set_title(idWin01, "Start Simulation")
pw.system.win_set_frame_style(idWin01, "borderless")
pw.system.win_center_keep(idWin01)
Win01_Open = true

-- Create camera view in new window
idWin02=pw.system.create_window()
idWdg02=pw.system.create_widget("camera")
idCam02=pw.system.create_camera()
pw.system.cam_attach_to(idCam02, idObj01)
pw.system.cam_set_position(idCam02, 0.0, -5.0)
pw.system.cam_set_resolution_pxm(idCam02, 20)
pw.system.win_set_title(idWin02, "Camera 01")
pw.system.widget_set_cam(idWdg02, idCam02)
pw.system.widget_set_display_alpha(idWdg02, 0.5)
pw.system.win_set_widget(idWin02, idWdg02)
pw.system.win_set_bg_color(idWin02, 1.0, 1.0, 0.7, 0.5)
pw.system.win_set_position(idWin02, 100, 200)
pw.system.win_resize(idWin02, 300, 400)

-- Create a particle emitter
idEm02 = pw.system.create_emitter("particle")
idPa02 = pw.system.create_particles("thrust")
pw.system.particles_set_size_birth(idPa02, 0.5)
pw.system.particles_set_size_death(idPa02, 0.05)
pw.system.particles_set_color_birth(idPa02, 1.0, 1.0, 0.2, 0.3)
pw.system.particles_set_color_death(idPa02, 1.0, 0.0, 0.0, 0.3)
pw.system.particles_set_maximum_age(idPa02, 0.1)
pw.system.emitter_set_particles(idEm02, idPa02)
pw.system.emitter_set_distribution(idEm02, "point_source")
pw.system.emitter_set_mode(idEm02, "timed")
pw.system.emitter_set_frequency(idEm02, 500)
pw.system.emitter_set_number(idEm02, 50)
pw.physics.emitter_set_position(idEm02, 0.0, -1.0)
pw.system.emitter_set_angle(idEm02, math.rad(-90))
pw.system.emitter_set_angle_std(idEm02, math.rad(2))
pw.system.emitter_set_velocity(idEm02, 100.0)
pw.system.emitter_set_velocity_std(idEm02, 2.0)
pw.system.emitter_set_velocity_inheritance(idEm02, 1.0)
idEm01 = pw.system.create_emitter("particle")
idPa01 = pw.system.create_particles("smoke")
pw.system.emitter_set_parent(idEm01, idObjEarth)
pw.system.particles_set_size_birth(idPa01, 1.0)
pw.system.particles_set_size_death(idPa01, 200.0)
pw.system.particles_set_color_birth(idPa01, 1.0, 0.95, 0.95, 0.3)
pw.system.particles_set_color_death(idPa01, 1.0, 1.0, 1.0, 0.0)
pw.system.particles_set_maximum_age(idPa01, 50.0)
pw.system.emitter_set_particles(idEm01, idPa01)
pw.system.emitter_set_distribution(idEm01, "point_source")
pw.system.emitter_set_mode(idEm01, "timed")
pw.system.emitter_set_frequency(idEm01, 100)
pw.system.emitter_set_number(idEm01, 5000)
pw.physics.emitter_set_position(idEm01, 0.0, -3.0)
pw.system.emitter_set_angle(idEm01, math.rad(-90))
pw.system.emitter_set_angle_std(idEm01, math.rad(3))
pw.system.emitter_set_velocity(idEm01, 50.0)
pw.system.emitter_set_velocity_std(idEm01, 5.0)

-- Create a thruster
idThruster01 = pw.system.create_thruster()
pw.system.thruster_set_object(idThruster01, idObj01)
pw.system.thruster_add_emitter(idThruster01, idEm01)
pw.system.thruster_add_emitter(idThruster01, idEm02)
pw.physics.thruster_set_angle(idThruster01, math.rad(90))
pw.physics.thruster_set_thrust_max(idThruster01, 100.0)
pw.sim.thruster_activate(idThruster01, 50.0)

-- Setup callback functions
pw.system.register_lua_callback("e_lua_update", "update")
pw.system.register_lua_callback("toggle_pause", "start_simulation")

function update()
    -- Callback to "e_lua_update" event which is called with 30 Hz
    -- (given by "pw.system.set_frequency_lua(30)", see above)
    -- Regular updates might be implemented here, additional callbacks
    -- to the update are possible
    pw.physics.obj_apply_force(idObj01, 0.1, 0.0, 0.0, 1.0)
    print(pw.physics.obj_get_velocity(idObj01))
end

function start_simulation()
    -- On simulation start close window
    if Win01_Open then
        pw.system.win_close(idWin01)
        Win01_Open = false
    end
end
