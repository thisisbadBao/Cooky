#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

struct SpriteComponent {
    int width;
    int height;

    SpriteComponent(int width = 0, int height = 0) {
        this->width = width;
        this->height = height;
    }
};

#endif
