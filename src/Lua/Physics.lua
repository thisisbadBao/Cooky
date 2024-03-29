rb = {}

function rb:SetLinearVelocity(entity, velocity)
    SetLinearVelocity(entity:getId(), velocity)
end

function rb:ApplyForceToCenter(entity, force)
    ApplyForceToCenter(entity:getId(), force)
end

function rb:SetLinearDamping(entity, linearDamping)
    SetLinearDamping(entity:getId(), linearDamping)
end

function rb:SetAngularVelocity(entity, angularVelocity)
    SetAngularVelocity(entity:getId(), angularVelocity)
end

function rb:SetAngularDamping(entity, angularDamping)
    SetAngularDamping(entity:getId(), angularDamping)
end

function rb:SetGravityScale(entity, scale)
    SetGravityScale(entity:getId(), scale)
end

function rb:SetSensor(entity, sensor)
    SetSensor(entity:getId(), sensor)
end

function rb:OnContact(entity, callback)
    OnContact(entity:getId(), callback)
end

function rb:EndContact(entity, callback)
    EndContact(entity:getId(), callback)
end