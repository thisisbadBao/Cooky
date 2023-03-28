Level = {
    assets = {
        [0] =
        { type = "texture", id = "tilemap-texture", file = "./assets/tilemaps/jungle.png" },
        { type = "texture", id = "chopper-texture", file = "./assets/images/chopper-spritesheet.png" },
        { type = "texture", id = "takeoff-texture", file = "./assets/images/takeoff-base.png" },
        { type = "texture", id = "tank-texture", file = "./assets/images/tank-tiger-right.png" },
        { type = "texture", id = "truck-texture", file = "./assets/images/truck-ford-right.png" },
        { type = "texture", id = "rader-texture", file = "./assets/images/radar.png" },
        { type = "font", id = "pico-font-5", file = "./assets/fonts/pico8.ttf", font_size = 5 },
        { type = "font", id = "pico-font-10", file = "./assets/fonts/pico8.ttf", font_size = 10 },
        { type = "font", id = "charriot-font-20", file = "./assets/fonts/charriot.ttf", font_size = 20 },
    },

    tilemap = {
        map_file = "./assets/tilemaps/jungle.map",
        texture_asset_id = "tilemap-texture",
        num_rows = 20,
        num_cols = 25,
        tile_size = 32,
        scale = 2.0
    },

    entities = {
        [0] =
        {
            -- Player
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
                    src_rect_x = 0,
                    src_rect_y = 0
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 15  -- fps
                },
                boxcollider = {
                    offset = { x = 0, y = 5 },
                    width = 32,
                    height = 25
                },
                keyboard_controller = {
                    up_velocity = { x = 0, y = -180 },
                    right_velocity = { x = 180, y = 0 },
                    down_velocity = { x = 0, y = 180 },
                    left_velocity = { x = -180, y = 0 },
                },
                camera_follow = {
                    follow = true
                },
                on_update_script = {
                    [0] =
                        function(entity)
                            local cur_x, cur_y = get_position(entity)
                            -- entity:test()
                            print("Executing player script, x: " .. cur_x .. " y: " .. cur_y .. " id:" .. entity:getId())
                        end
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 800, y = 150 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- deg
                },
                rigidbody = {
                    velocity = { x = 5.0, y = 0.0 },
                },
                sprite = {
                    texture_asset_id = "tank-texture",
                    width = 32,
                    height = 32,
                    z_index = 2
                },
                boxcollider = {
                    offset = { x = 0, y = 10 },
                    width = 32,
                    height = 15
                }
            }
        }
    } -- end of entities
}
