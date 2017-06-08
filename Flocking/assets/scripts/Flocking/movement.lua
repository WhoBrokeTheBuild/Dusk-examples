
local actor = Dusk.GetComponent():GetActor()

function update(data)
    local x, y, z = actor:GetRotation()
    y = y + (50.0 * data.Delta)
    actor:SetRotation(x, y, z)
end

function free(data)
    actor:RemoveEventListener(actor.Events.UPDATE, "update")
    actor:RemoveEventListener(actor.Events.FREE, "free")
end

actor:AddEventListener(actor.Events.UPDATE, "update")
actor:AddEventListener(actor.Events.FREE, "free")
