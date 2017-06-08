
local actor = Dusk.GetComponent():GetActor()

function update(data)
    local x, y, z = actor:GetRotation()
    y = y + (50.0 * data.Delta)
    actor:SetRotation(x, y, z)
end

actor:AddEventListener(actor.Events.UPDATE, "update")
