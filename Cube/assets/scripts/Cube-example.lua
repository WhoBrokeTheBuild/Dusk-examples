
local app = Dusk.GetApp()
local scene = app:GetScene()
local cube = scene:GetActorByName("cube_actor")

function cube_Update(data)
    local x, y, z = cube:GetRotation()
    y = y + (50.0 * data.Delta)
    cube:SetRotation(x, y, z)
end

cube:AddEventListener(cube.Events.UPDATE, "cube_Update")
