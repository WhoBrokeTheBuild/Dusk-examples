
local app = Dusk.GetApp()

cube:AddEventListener("Update", function()
    local x, y, z = cube:GetRotation()
    y += 1.0
    cube:SetRotation(x, y, z)
end)
