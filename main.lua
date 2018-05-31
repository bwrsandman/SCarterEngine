-- Engine.Logging.SetLogLevel("Debug")

Engine.Logging.Log("Info", "Start of main script")

-- TODO: Move these calls to GameManager init
Engine.Scene.Current = Engine.Game.AddScene("Main")

-- TODO: This should work once Scene has AddCamera definition
-- cam = Engine.Game.CurrentScene:AddCamera("MainCamera")

-- TODO: This can be removed once Engine.Scene.Current is managed by engine
-- Engine.Game.CurrentScene:RemoveCamera("MainCamera")
-- cam = nil
Engine.Game.CurrentScene = nil
Engine.Game.RemoveScene("Main")

Engine.Logging.Log("Info", "End of main script")

