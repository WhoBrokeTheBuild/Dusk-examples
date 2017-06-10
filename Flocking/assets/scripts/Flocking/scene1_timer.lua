
local app = Dusk.GetApp()
local scene = app:GetScene()

local timeout = 5

function update(data)
    timeout = timeout - data.ElapsedTime
    if timeout < 0 then
        app:PushScene(app:GetSceneByName("test2_scene"))
    end
end

function free(data)
    print("Removing event listeners")
    scene:RemoveEventListener(scene.Events.UPDATE, "update")
    scene:RemoveEventListener(scene.Events.FREE_START, "free")
    print("done")
end

print("Adding event listeners")
scene:AddEventListener(scene.Events.UPDATE, "update")
scene:AddEventListener(scene.Events.FREE_START, "free")
print("done")
