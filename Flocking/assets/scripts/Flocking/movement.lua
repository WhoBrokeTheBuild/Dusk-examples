
local actor = Dusk.GetComponent():GetActor()

function update(data)
    local x, y, z = actor:GetRotation()
    y = y + (50.0 * data.Delta)
    actor:SetRotation(x, y, z)
end

function free(data)
    print("Removing event listeners")
    actor:RemoveEventListener(actor.Events.UPDATE, "update")
    actor:RemoveEventListener(actor.Events.FREE_START, "free")
    print("done")
end

actor:AddEventListener(actor.Events.UPDATE, "update")
actor:AddEventListener(actor.Events.FREE_START, "free")
