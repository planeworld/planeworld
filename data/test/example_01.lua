-- EXAMPLE_01.LUA
-----------------
-- This is a work in progress example using several different commands. It is
-- mainly used for testing purposes, so there is no meaningful content, yet.


-- KNOWN ISSUES
---------------
-- Objects not created when paused
-- Segfault when no camera/universe present
-- shp_set_vertices should be in domain pw.system

pw.system.set_frequency_input(100)
pw.system.set_frequency_lua(30)
pw.system.set_frequency_physics(200)
pw.system.set_frequency_visuals(60)
pw.system.set_data_path_visuals("../")
pw.system.create_universe(23479, 10000)
pw.physics.set_gravity_vector(0.0, -9.81)
pw.system.win_main_resize(1000, 500)
idCam01 = pw.system.create_camera()

idObj01 = pw.system.create_obj()
idShp01 = pw.system.create_shp("shp_circle")
idShp02 = pw.system.create_shp("shp_polygon")

pw.physics.shp_set_vertices(idShp02,
    {-1.0, -1.0,
      1.0, -1.0,
      2.0,  1.0,
     -2.0,  1.0}
)

pw.system.obj_add_shp(idObj01, idShp01)
pw.system.obj_add_shp(idObj01, idShp02)

pw.visuals.toggle_bboxes()
-- pw.visuals.toggle_com()
pw.visuals.toggle_grid()
pw.visuals.toggle_names()

idWin01=pw.system.create_window()
idWdg01=pw.system.create_widget("text")
pw.system.widget_set_text(idWdg01, "Welcome to planeworld, press p to start")
pw.system.win_set_widget(idWin01, idWdg01)
pw.system.win_center(idWin01)

pw.system.pause()

Force = 1.0
function physics_interface()
    pw.physics.obj_apply_force(idObj01, 0.0, Force, 0.0, 0.0)
    Force = Force+1.0;
end

