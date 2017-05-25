
local scene = Dusk.GetScene("default_scene")
local cube = scene:GetActor("cube_actor")

cube:AddEventListener("Update", function()
    local x, y, z = cube:GetRotation()
    y += 1.0
    cube:SetRotation(x, y, z)
end)
