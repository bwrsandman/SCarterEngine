function Initialize()
    -- Engine.Logging.SetLogLevel("Debug")
    Engine.Logging.Log("Info", "Start of main script")

    -- TODO: Move these calls to GameManager init
    Engine.Game.CurrentScene = Engine.Game.AddScene("Main")

    -- TODO: This should work once Scene has AddCamera definition
    -- cam = Engine.Game.CurrentScene:AddCamera("MainCamera")
end

function Terminate()
    -- TODO: This can be removed once Engine.Scene.Current is managed by engine
    -- Engine.Game.CurrentScene:RemoveCamera("MainCamera")
    -- cam = nil
    Engine.Game.CurrentScene = nil
    Engine.Game.RemoveScene("Main")

    Engine.Logging.Log("Info", "End of main script")
end

loops = 0

function Loop(dt)
    fps = string.format("%.02f", 1000 / dt)
    Engine.Logging.Log("Debug", "Looping ("..loops..") at " .. fps .. " FPS")
    loops = loops + 1
end

function Engine.Quit()
    return loops > 10000
end
