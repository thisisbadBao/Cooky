test()

-- Vec2 Test
vec1 = Vec2.new(6, 3)
vec2 = Vec2.new(7, 11)
vec3 = vec1 + vec2

-- print("x: " .. vec3.x .. ", y: " .. vec3.y)


-- Asset Test
addFont("charriot-font-20", "./assets/fonts/charriot.ttf", 20)
addTexture("tank", "./assets/images/tank-tiger-right.png")

local i = 1


-- Entity Test
entity = createEntity()

entity:addSprite("tank", 32, 32, 2, false, 0, 0)
entity:addTransform(Vec2.new(200, 200), Vec2.new(1, 1), 0)

local has = entity:hasTransform()
print("has: " .. tostring(has))

transform = entity:getTransform()

print("transform: " .. tostring(transform.position.x))

function update()
    transform.position.x = transform.position.x + 1
    transform.rotation = transform.rotation + 2
end

-- print("id: " .. entity:getId())

