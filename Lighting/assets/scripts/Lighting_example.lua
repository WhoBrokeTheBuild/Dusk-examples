
local app = Dusk.GetApp()
local scene = app:GetScene()
local actor = scene:GetActorByName("main_actor")

function actor_Update(data)
    local x, y, z = actor:GetRotation()
    y = y + (50.0 * data.Delta)
    actor:SetRotation(x, y, z)
end

actor:AddEventListener(actor.Events.UPDATE, "actor_Update")
