function Initialize()
    Engine.Logging.SetLogLevel("Warning")
    Engine.Logging.Log("Info", "Start of main script")

    -- TODO: Move these calls to GameManager init
    Engine.Game.CurrentScene = Engine.Game.AddScene("Main")

    cam = Engine.Game.CurrentScene:AddCamera("MainCamera")

    Engine.Logging.Log("Debug", "Scene Cameras:")
    for k, v in pairs(Engine.Game.CurrentScene.Cameras) do
        Engine.Logging.Log("Debug", k .. ": " .. tostring(v))
    end

    local triangle = Engine.Game.CurrentScene:AddMesh(
        "Triangle",
        {0, 1, 2},
        {
            { 0.1, 0.82},
            { 0.1, -0.82},
            { 0.82, -0.82 }
        }
    )
    triangle = Engine.Game.CurrentScene:AddMesh(
        "Triangle2",
        {0, 1, 2},
        {
            { -0.1, 0.82},
            { -0.82, -0.82},
            { -0.1, -0.82 }
        }
    )
end

function Terminate()
    -- TODO: This can be removed once Engine.Scene.Current is managed by engine
    Engine.Game.CurrentScene:RemoveCamera("MainCamera")
    cam = nil
    Engine.Game.CurrentScene = nil
    Engine.Game.RemoveScene("Main")

    Engine.Logging.Log("Info", "End of main script")
end

loops = 0
time = 0.0

function Loop(dt)
    time = time + dt
    fps = string.format("%.02f", 1000 / dt)
    Engine.Logging.Log("Info", "Looping ("..loops..") at " .. fps .. " FPS")

    -- Engine.Game.CurrentScene:Update()
    -- Engine.Rendering.Target = Engine.Rendering.Screen
    local sin = math.sin(time / 1000)
    local gray = sin * sin
    cam.ClearColor = {gray, gray, gray, 0}

    local r, g, b, a = table.unpack(cam.ClearColor)
    Engine.Logging.Log("Debug", r .. ", " .. g .. ", " .. b .. ", " .. a)

    loops = loops + 1
end

function KeyDown(key)
    Engine.Logging.Log("Warning", "Pressed key: " .. key)
    if key == "escape" then
        Engine.Quit = true
    end
end

function MouseButtonDown(button, x, y)
    Engine.Logging.Log("Warning", "Mouse click(" .. button .."): " .. x .. " " .. y)
end
