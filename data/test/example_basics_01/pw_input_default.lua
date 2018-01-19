-- Setup callback to key press event
pw.system.register_lua_callback("e_key_pressed", "key_pressed")

-- Table of actions by key
-- (Note: These are not scancodes, but codes used by SFML internally
--        Using these keeps the portability that SFML provides)
pw.system.key_action = {
    --   0 = a
    --   1 = b
    --   2 = c
    --   3 = d
    --   4 = e
    --   5 = f
    --   6 = g
    --   7 = h
    --   8 = i
    --   9 = j
    --  10 = k
    --  11 = l
    --  12 = m
    --  13 = n
    --  14 = o
    --  15 = p
    --  16 = q
    --  17 = r
    --  18 = s
    --  19 = t
    --  20 = u
    --  21 = v
    --  22 = w
    --  23 = x
    --  24 = y
    --  25 = z
    --  26 = Num0
    --  27 = Num1
    --  28 = Num2
    --  29 = Num3
    --  30 = Num4
    --  31 = Num5
    --  32 = Num6
    --  33 = Num7
    --  34 = Num8
    --  35 = Num9
    --  36 = Escape
    --  37 = LControl
    --  38 = LShift
    --  39 = LAlt
    --  40 = LSystem
    --  41 = RControl
    --  42 = RShift
    --  43 = RAlt
    --  44 = RSystem
    --  45 = Menu
    --  46 = LBracket
    --  47 = RBracket
    --  48 = SemiColon
    --  49 = Comma
    --  50 = Period
    --  51 = Quote
    --  52 = Slash
    --  53 = BackSlash
    --  54 = Tilde
    --  55 = Equal
    --  56 = Dash
    --  57 = Space
    --  58 = Return
    --  59 = BackSpace
    --  60 = Tab
    --  61 = PageUp
    --  62 = PageDown
    --  63 = End
    --  64 = Home
    --  65 = Insert
    --  66 = Delete
    --  67 = Add
    --  68 = Subtract
    --  69 = Multiply
    --  70 = Divide
    --  71 = Left
    --  72 = Right
    --  73 = Up
    --  74 = Down
    --  75 = Numpad0
    --  76 = Numpad1
    --  77 = Numpad2
    --  78 = Numpad3
    --  79 = Numpad4
    --  80 = Numpad5
    --  81 = Numpad6
    --  82 = Numpad7
    --  83 = Numpad8
    --  84 = Numpad9
    --  85 = F1
    --  86 = F2
    --  87 = F3
    --  88 = F4
    --  89 = F5
    --  90 = F6
    --  91 = F7
    --  92 = F8
    --  93 = F9
    --  94 = F10
    --  95 = F11
    --  96 = F12
    --  97 = F13
    --  98 = F14
    --  99 = F15
    -- 100 = Pause
    
    -- a / Add  Accelerate time 
                    [0] =   function()
                                -- LControl: Allow for time step increment
                                if pw.system.key_is_pressed(37) then
                                    pw.system.accelerate_time(true)
                                else
                                    pw.system.accelerate_time(false)
                                end
                            end,
                    [67] =  function()
                                -- LControl: Allow for time step increment
                                if pw.system.key_is_pressed(37) then
                                    pw.system.accelerate_time(true)
                                else
                                    pw.system.accelerate_time(false)
                                end
                            end,
    -- b        Toggles bounding box visualisation (on/off)
                    [1] = function() pw.visuals.toggle_bboxes() end,
    -- c        Toggles bounding box visualisation (on/off)
                    [2] = function() pw.system.cam_cycle() end,
    -- d
    -- Subtract
    -- Dash     Decelerate time
                    [3]  = function() pw.system.decelerate_time() end,
                    [56] = function() pw.system.decelerate_time() end,
                    [68] = function() pw.system.decelerate_time() end,
    -- f        Toggles bounding box visualisation (on/off)
                    [5] = function() pw.system.toggle_fullscreen() end,
    -- g        Toggles universe grid visualisation (on/off)
                    [6] = function() pw.visuals.toggle_grid() end,
    -- k        Toggles kinematic states visualisation (on/off)
                    [10] = function() pw.visuals.toggle_kin_states() end,
    -- m        Toggles center of mass visualisation (on/off)
                    [12] = function() pw.visuals.toggle_com() end,
    -- n        Toggles entity name visualisation (on/off)
                    [13] = function() pw.visuals.toggle_names() end,
    -- p        Toggles simulation pause (on/off)
                    [15] = function() pw.system.toggle_pause() end,
    -- q        Quit planeworld
                    [16] = function() pw.system.quit() end,
    -- Num0     Toggles timer visualisation (on/off)
                    [26] = function() pw.visuals.toggle_timers() end,
    -- Num1     Toggles timer 1 (start/stop)
                    [27] = function() pw.system.toggle_timer(1) end,
    -- Num2     Toggles timer 2 (start/stop)
                    [28] = function() pw.system.toggle_timer(2) end,
    -- Num3     Toggles timer 3 (start/stop)
                    [29] = function() pw.system.toggle_timer(3) end,
    -- Space    Processes a single simulation step 
                    [57] = function() pw.system.process_one_frame() end,
    -- Return   Reset time acceleration to real time (factor 1)
                    [58] = function() pw.system.reset_time() end,
    -- Pause    Pauses simulation
                    [100] = function() pw.system.pause() end
}

-- Callback for key press event
function key_pressed(keycode)
    if (pw.system.key_action[keycode] ~= nil) then
        pw.system.key_action[keycode]() 
    end
end
