test()

addFont("charriot-font-20", "./assets/fonts/charriot.ttf", 20);
addTexture("tank", "./assets/images/tank-tiger-right.png")

entity = createEntity()

entity:addSprite("tank", 32, 32, 2, false)

print("id: " .. entity:getId())