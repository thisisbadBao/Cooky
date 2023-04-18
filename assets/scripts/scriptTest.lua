test()

-- Vec2 Test
vec1 = Vec2(6, 3)
vec2 = Vec2(7, 11)
vec3 = vec1 + vec2

-- print("x: " .. vec3.x .. ", y: " .. vec3.y)


-- Asset Test
addFont("charriot-20", "./assets/fonts/charriot.ttf", 30)
addTexture("tank", "./assets/images/tank-tiger-right.png")

-- Entity Test
ent = newEnt()
ent:addSprite("tank", 32, 32, 2, false, Vec2(0, 0))

entity = newEnt()

entity:addSprite("tank", 32, 32, 2, false, Vec2(0, 0))
entity:addTransform(Vec2(200, 200), Vec2(1, 1), 0)
entity:tag("cooky")

local has = entity:hasTransform()
-- print("has: " .. tostring(has))

transform = entity:getTransform()
-- print("transform: " .. tostring(transform.position.x))

text = newEnt()
text:addText(Vec2(300, 60), "COOKY V0.0.1", "charriot-20", Color(224, 224, 224, 150), true)

Def = {
    assets = {
        [0] =
        { type = "texture", id = "chopper-texture", file = "./assets/images/chopper-spritesheet.png" },
        { type = "texture", id = "truck-texture", file = "./assets/images/truck-ford-right.png" },
        { type = "texture", id = "rader-texture", file = "./assets/images/radar.png" },
        { type = "font", id = "pico-font-5", file = "./assets/fonts/pico8.ttf", font_size = 5 },
        { type = "font", id = "pico-font-10", file = "./assets/fonts/pico8.ttf", font_size = 10 },
    },

    entities = {
        [0] =
        {
            tag = "player",
            components = {
                transform = {
                    position = { x = 242, y = 110 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                sprite = {
                    texture_asset_id = "chopper-texture",
                    width = 32,
                    height = 32,
                    z_index = 4,
                    fixed = false,
                    src_rect = { x = 0, y = 0 }
                },
                animation = {
                    num_frames = 2,
                    fps = 9,
                    isLoop = true
                },
                keyboard_controller = {
                    up_vel = { x = 0, y = -180 },
                    right_vel = { x = 180, y = 0 },
                    down_vel = { x = 0, y = 180 },
                    left_vel = { x = -180, y = 0 },
                },
                camera_follow = {
                    follow = true
                }
            }
        },
        {
            group = "back",
            components = {
                transform = {
                    position = { x = 250, y = 450 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0,
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                sprite = {
                    texture_asset_id = "truck-texture",
                    width = 32,
                    height = 32,
                    z_index = 2,
                    fixed = false,
                    src_rect = { x = 0, y = 0 }
                },
            }
        }
    } -- end of entities
}

local i = 1
function update()
    transform.position.x = transform.position.x + 1
    transform.rotation = transform.rotation + 2
    if i % 20 == 0 then
        newText = newEnt()
        newText:addText(Vec2(100, 100 + i), tostring(i//20), "charriot-20", Color(236, 176, 193, 255), true)
        text:getText().color = Color(77 + i, 208 + i, 225 + i, 150 + i)
    end
    if i == 75 then
        print("set sprite off")
        entity:setSprite(false)
    end
    if i == 145 then
        print("set sprite on")
        entity:setSprite(true)
        temp = getEntByTag("cooky")
        print("Get by tag: " .. tostring(temp:getId()))
        print("Has tag: " .. tostring(temp:hasTag("cooky")))
    end
    if i == 185 then
        entity:rmSprite()
    end
    i = i + 1
end

-- print("id: " .. entity:getId())
