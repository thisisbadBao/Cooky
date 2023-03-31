test()

-- Vec2 Test
vec1 = Vec2.new(6, 3)
vec2 = Vec2.new(7, 11)
vec3 = vec1 + vec2

-- print("x: " .. vec3.x .. ", y: " .. vec3.y)


-- Asset Test
addFont("charriot-font-20", "./assets/fonts/charriot.ttf", 30)
addTexture("tank", "./assets/images/tank-tiger-right.png")

-- Entity Test
entity = createEntity()

entity:addSprite("tank", 32, 32, 2, false, 0, 0)
entity:addTransform(Vec2.new(200, 200), Vec2.new(1, 1), 0)

local has = entity:hasTransform()
print("has: " .. tostring(has))

transform = entity:getTransform()

print("transform: " .. tostring(transform.position.x))

text = createEntity()
text:addText(Vec2.new(300, 60), "COOKY V0.0.1", "charriot-font-20", Vec3.new(224, 224, 224), true)


local i = 1
function update()
    transform.position.x = transform.position.x + 1
    transform.rotation = transform.rotation + 2
    if i % 10 == 0 then
        text:getText().text = text:getText().text .. "1"
        text:getText().color = Vec3.new(77 + i, 208 + i, 225 + i)
    end
    i = i + 1
end

-- print("id: " .. entity:getId())

