require("Physics")
require("Keyboard")
require("Sound")

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
bricksCount = 0;
bricks = {}
for i = 1, 1 do
    bricks[i] = {}
    for j = 1, 4 do
        bricks[i][j] = newEnt()
        bricks[i][j]:addTransform(Vec2(boxPos.x - 0.2 + 1.3 * j, boxPos.y + i * 0.6))
        brickDef = BodyDef()
        brickDef.type = BodyType.static
        bricks[i][j]:addPolygon(brickDef, brickWidth, brickHeight, 0.3, 1, 1)
        rb:OnContact(bricks[i][j], function(tag)
            if tag == "ball" then
                bricksCount = bricksCount - 1
                bricks[i][j]:kill()
                sound:Play("sfx_box")
            end

        end)
        bricksCount = bricksCount + 1
    end
end


player = newEnt()
player:addTransform(pPos);
playerDef = BodyDef()
playerDef.type = BodyType.kinematic
playerVertices = {Vec2(0,0), Vec2(2.2,0), Vec2(1,-0.1), Vec2(1.2,-0.1),}
player:addPolygon(playerDef, playerVertices, 4, 0.8, 1, 1)

player:sub("a_up", function ()
    rb:SetLinearVelocity(player, Vec2(0,0))
end)

player:sub("d_up", function ()
    rb:SetLinearVelocity(player, Vec2(0,0))
end)

sound:AddSFX("sfx_ball", "./assets/sounds/select.wav")
sound:AddSFX("sfx_box", "./assets/sounds/switch.wav")
sound:AddStream("bgm", "./assets/sounds/affection.wav")
sound:PlayStream("bgm")

rb:OnContact(player, function (tag)
    if tag == "ball" then
        sound:Play("sfx_ball")
    end
end)

ball = newEnt()
ball:addTransform(ballPos)
ballDef = BodyDef()
ballDef.type = BodyType.dynamic
ball:addCircle(ballDef, ballRadius, 0.0, 1, 1)
ball:tag("ball")
rb:SetLinearVelocity(ball, Vec2(-5, 5))
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
rb:OnContact(co, function(tag)
    if tag == "ball" then
        emit("game_over")
        ball:kill()
    end
end)


player:sub("you_win", function()
    rb:SetLinearVelocity(ball, Vec2(0, 0))
    text_win = newEnt()
    text_win:addText(Vec2(320, 335), "You win!", "charriot-20", Color(255, 160, 0, 255), true)
end)

player:sub("game_over", function()
    text_win = newEnt()
    text_win:addText(Vec2(320, 335), "Game Over", "charriot-20", Color(233, 30, 99, 255), true)
end)

winFlag = false

function update()
    if bricksCount == 0 and winFlag == false then
        emit("you_win")
        winFlag = true
    end

    if keyboard:IsKeyDown("a") then
        rb:SetLinearVelocity(player, Vec2(0,0)-pVelocity)
    end

    if keyboard:IsKeyDown("d") then
        rb:SetLinearVelocity(player, pVelocity)
    end

    -- clog(tostring(ballTrans.position.y))
end
