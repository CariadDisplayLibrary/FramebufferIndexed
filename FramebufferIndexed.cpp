#include <FramebufferIndexed.h>

FramebufferIndexed::FramebufferIndexed(int w, int h, uint8_t *b, color_t *c) {
    _width = w;
    _height = h;
    _buf = b;
    _clut = c;
}

void FramebufferIndexed::initializeDevice() {
    fillScreen(0);
}

void FramebufferIndexed::setPixel(int x, int y, color_t color) {

    if (x < 0 || x >= _width || y < 0 || y >= _height) {
        return;
    }
    
    _buf[x + y * _width] = color & 0xFF;
}

void FramebufferIndexed::fillScreen(color_t color) {
    for (int x = 0; x < _width * _height; x++) {
        _buf[x] = color & 0xFF;
    }
}

color_t FramebufferIndexed::colorAt(int x, int y) {
    return _clut[_buf[x + (y * getWidth())]];
}

void FramebufferIndexed::draw(DisplayCore *dev, int x, int y) {
    if (_filter != NULL) {
        uint32_t p = 0;
        color_t line[getWidth()];
        for (int py = 0; py < getHeight(); py++) {
            for (int px = 0; px < getWidth(); px++) {
                line[px] = _filter->process(_clut[_buf[p]]);
                p++;
            }
            dev->openWindow(x, y + py, getWidth(), 1);
            dev->windowData(line, getWidth());
            dev->closeWindow();
        }
    } else {
        dev->openWindow(x, y, getWidth(), getHeight());
        uint32_t len = getWidth() * (uint32_t)getHeight();
        for (uint32_t p = 0; p < len; p++) {
            dev->windowData(_clut[_buf[p]]);
        }
        dev->closeWindow();
    }
}

void FramebufferIndexed::draw(DisplayCore *dev, int x, int y, color_t t) {
    uint32_t p = 0;
    color_t line[getWidth()];

    for (int py = 0; py < getHeight(); py++) {
        boolean haveTrans = false;
        for (int px = 0; px < getWidth(); px++) {
            color_t col = _clut[_buf[p]];
            if (col == t) {
                haveTrans = true;
                line[px] = col;
            } else {
                if (_filter != NULL) {
                    col = _filter->process(col);
                }
                line[px] = col;
            }
            p++;
        }
        if (!haveTrans) {
            dev->openWindow(x, y + py, getWidth(), 1);
            dev->windowData(line, getWidth());
            dev->closeWindow();
        } else {
            for (int px = 0; px < getWidth(); px++) {
                if (_clut[_buf[py * getWidth() + px]] != t) {
                    dev->setPixel(x + px, y + py, line[px]);
                }
            }
        }
    }
}

void FramebufferIndexed::drawTransformed(DisplayCore *dev, int x, int y, int transform) {
    uint32_t p = 0;
    for (int py = 0; py < getHeight(); py++) {
        for (int px = 0; px < getWidth(); px++) {
            switch (transform) {
                default:
                    dev->setPixel(x + px, y + py, _clut[_buf[p]]);
                    break;
                case MirrorH:
                    dev->setPixel(getWidth() - (x + px) - 1, y + py, _clut[_buf[p]]);
                    break;
                case MirrorV:
                    dev->setPixel(x + px, getHeight() - (y + py) - 1, _clut[_buf[p]]);
                    break;
                case Rotate180:
                    dev->setPixel(getWidth() - (x + px) - 1, getHeight() - (y + py) - 1, _clut[_buf[p]]);
                    break;
            }
            p++;
        }
    }
}

void FramebufferIndexed::drawTransformed(DisplayCore *dev, int x, int y, int transform, color_t t) {
    uint32_t p = 0;
    for (int py = 0; py < getHeight(); py++) {
        for (int px = 0; px < getWidth(); px++) {
            if (_clut[_buf[p]] != t) {
                switch (transform) {
                    default:
                        dev->setPixel(x + px, y + py, _clut[_buf[p]]);
                        break;
                    case MirrorH:
                        dev->setPixel(getWidth() - (x + px) - 1, y + py, _clut[_buf[p]]);
                        break;
                    case MirrorV:
                        dev->setPixel(x + px, getHeight() - (y + py) - 1, _clut[_buf[p]]);
                        break;
                    case Rotate180:
                        dev->setPixel(getWidth() - (x + px) - 1, getHeight() - (y + py) - 1, _clut[_buf[p]]);
                        break;
                }
            }
            p++;
        }
    }
}
