require("Physics")
require("Keyboard")
clog("hello byc")

boxHeight = 8
boxWidth = 6
boxBoder = 0.1
boxPos = Vec2(2,1)

pWidth = 2
pHeight = 0.5
pPos = Vec2(boxWidth/2+boxPos.x,boxPos.y+boxHeight-0.2)
pVelocity = Vec2(6,0)

ballRadius = 0.2
ballPos = pPos + Vec2(1,-2)

leftWall = newEnt()
leftWall:addTransform(Vec2(boxPos.x,boxPos.y+boxHeight/2))
leftWallDef = BodyDef()
leftWallDef.type = BodyType.static
leftWall:addPolygon(leftWallDef, boxBoder, boxHeight, 0.3, 1, 1)

rightWall = newEnt()
rightWall:addTransform(Vec2(boxPos.x+boxWidth,boxPos.y+boxHeight/2))
rightWallDef = BodyDef()
rightWallDef.type = BodyType.static
rightWall:addPolygon(rightWallDef, boxBoder, boxHeight, 0.3, 1, 1)

upWall = newEnt()
upWall:addTransform(Vec2(boxPos.x+boxWidth/2,boxPos.y))
upWallDef = BodyDef()
upWallDef.type = BodyType.static
upWall:addPolygon(upWallDef, boxWidth, boxBoder, 0.3, 1, 1)

-- downWall = newEnt()
-- downWall:addTransform(Vec2(boxPos.x+boxWidth/2,boxPos.y+boxHeight))
-- downWallDef = BodyDef()
-- downWallDef.type = BodyType.static
-- downWall:addPolygon(downWallDef, boxWidth, boxBoder, 0.3, 1, 1)

brickWidth = 1
brickHeight = 0.5
bricks = {}
for i = 1, 4 do
    bricks[i] = newEnt()
    bricks[i]:addTransform(Vec2(boxPos.x + (brickWidth + boxBoder) * i, boxPos.y + 3))
    brickDef = BodyDef()
    brickDef.type = BodyType.static
    bricks[i]:addPolygon(brickDef, brickWidth, brickHeight, 0.3, 1, 1)
    rb:OnCollision(bricks[i], function(tag)
        clog(tostring(bricks[i]:getId()))
        bricks[i]:kill()
    end)
end




player = newEnt()
player:addTransform(pPos);
playerDef = BodyDef()
playerDef.type = BodyType.kinematic
playerVertices = {Vec2(0,0), Vec2(2,0), Vec2(0.5,-pHeight), Vec2(1.5,-pHeight),}
player:addPolygon(playerDef, playerVertices, 4, 0.8, 1, 1)

player:sub("a_up", function ()
    rb:SetLinearVelocity(player, Vec2(0,0))
end)

player:sub("d_up", function ()
    rb:SetLinearVelocity(player, Vec2(0,0))
end)

ball = newEnt()
ball:addTransform(ballPos)
ballDef = BodyDef()
ballDef.type = BodyType.dynamic
ball:addCircle(ballDef, ballRadius, 0.0, 1, 1)
ball:tag("ball")
rb:SetLinearVelocity(ball, Vec2(-10, 10))
rb:SetLinearDamping(ball, 0.0)
rb:SetAngularDamping(ball, 0.0)
rb:SetGravityScale(ball, 0.0)

addFont("charriot-20", "./assets/fonts/charriot.ttf", 30)
tips = newEnt()
tips:addText(Vec2(300, 35), "Press 'a' or 'd'", "charriot-20", Color(0, 150, 136, 255), true)


co = newEnt()
co:addTransform(Vec2(boxPos.x+boxWidth/2,boxPos.y+boxHeight+1))
coDef = BodyDef()
coDef.type = BodyType.static
co:addPolygon(coDef, boxWidth*2, boxBoder, 0.3, 1, 1)
rb:SetSensor(co, true)
rb:OnCollision(co, function(tag)
    clog("tag: " .. tag)
    if tag == "ball" then
        clog("game over")
        ball:kill()
    end
end)


function update()
    if keyboard:IsKeyDown("a") then
        rb:SetLinearVelocity(player, Vec2(0,0)-pVelocity)
    end

    if keyboard:IsKeyDown("d") then
        rb:SetLinearVelocity(player, pVelocity)
    end
    
    -- clog(tostring(ballTrans.position.y))
end
