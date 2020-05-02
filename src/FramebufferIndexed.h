#ifndef _FRAMEBUFFERINDEXED_H
#define _FRAMEBUFFERINDEXED_H

#include <Cariad.h>

class FramebufferIndexed : public Image {
    private:
        uint8_t *_buf;
        color_t *_clut;

    public:
        FramebufferIndexed(int w, int h, uint8_t *b, color_t *clut);

        void initializeDevice();

        void setPixel(int x, int y, color_t c);
        color_t colorAt(int x, int y);
        void fillScreen(color_t c);

        void draw(Cariad *dev, int x, int y);
        void draw(Cariad *dev, int x, int y, color_t t);
        void drawTransformed(Cariad *dev, int x, int y, int transform);
        void drawTransformed(Cariad *dev, int x, int y, int transform, color_t t);

        void draw(Cariad &dev, int x, int y) { draw(&dev, x, y); }
        void draw(Cariad &dev, int x, int y, color_t t) { draw(&dev, x, y, t); }
        void drawTransformed(Cariad &dev, int x, int y, int transform) { drawTransformed(&dev, x, y, transform); }
        void drawTransformed(Cariad &dev, int x, int y, int __attribute__((unused)) transform, color_t t) { drawTransformed(&dev, x, y, t); }


};

#endif
