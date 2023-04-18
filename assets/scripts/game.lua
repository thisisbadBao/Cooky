Def = {
    assets = {
        [0] =
        { type = "font", id = "pico", file = "./assets/fonts/pico8.ttf", font_size = 30 }
    },

    entities = {
        [0] =
        {
            tag = "title",
            components = {
                text = {
                    position = { x = 400, y = 200 },
                    content = "Cooky Game",
                    assetId = "pico",
                    color = { r = 255, g = 23, b = 68, a = 255 },
                    isFixed = true
                }
            }
        }
    }
}

box = newEnt()
box:addTransform(Vec2(0,0), Vec2(0,0), 0)
box:addRigidBody(Vec2(0, 0))
boxdef = BodyDef()
boxdef.type = BodyType.dynamic
boxdef.position = b2Vec2(3, 1)
vertices = {Vec2(0, 0), Vec2(1, 0), Vec2(1, 1)}
box:addPolygon(boxdef, vertices, 3, 0.3, 0.5, 1)

platform = newEnt()
platform:addTransform(Vec2(0,0), Vec2(0,0), 0)
platform:addRigidBody(Vec2(0, 0))
platformdef = BodyDef()
platformdef.type = BodyType.static
platformdef.position = b2Vec2(5, 7)
platform:addPolygon(platformdef, 10, 0.6, 0.3, 0.5, 1)
