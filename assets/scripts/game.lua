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