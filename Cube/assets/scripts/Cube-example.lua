
local app = Dusk.GetApp()
local scene = app:GetScene()
local cube = scene:GetActorByName("cube_actor")

function cube_Update()
    local x, y, z = cube:GetRotation()
    y = y + 0.1
    cube:SetRotation(x, y, z)
end

cube:AddEventListener(cube.Events.UPDATE, "cube_Update")
