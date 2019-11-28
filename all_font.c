#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>

#define ALL_FONT_16P_SIZE (104340)

int ARG_WINDOW_WIDTH = 1024;
int ARG_WINDOW_HEIGHT = 1024;
int ARG_SCALE = 1;

SDL_Window *window = NULL;
SDL_Surface *screenSurface = NULL;
SDL_Event event;

#define PUT_PIXEL(x, y)                                                       \
    do {                                                                      \
        SDL_Rect rect = {(x)*ARG_SCALE, (y)*ARG_SCALE, ARG_SCALE, ARG_SCALE}; \
        SDL_FillRect(screenSurface, &rect,                                    \
                     SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));    \
    } while (0)

void redraw() {
    uint16_t code = 0;
    uint8_t glyph_buf[32] = {
        0,
    };

    screenSurface = SDL_GetWindowSurface(window);
    SDL_FillRect(screenSurface, NULL,
                 SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

    FILE *fp = fopen("./ALL_FONT.16P", "r");

    int x = 0;
    int y = 0;
    int index = 0;
    while (!feof(fp)) {
        fread(&code, 2, 1, fp);
        fread(glyph_buf, 1, 28, fp);

        printf("index: %d code: %d(0x%04x)\n", index++, code, code);

        for (int i = 0; i < 14; i++) {
            uint8_t left = glyph_buf[2 * i];
            uint8_t right = glyph_buf[2 * i + 1];

            for (int j = 0; j < 8; j++) {
                if (left & (0x80 >> j)) {
                    PUT_PIXEL(x + j, y + i);
                }

                if (right & (0x80 >> j)) {
                    PUT_PIXEL(x + j + 8, y + i);
                }
            }
        }

        x += 16;
        if (x >= ARG_WINDOW_WIDTH) {
            x = 0;
            y += 16;
        }
    }

    fclose(fp);

    SDL_UpdateWindowSurface(window);
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 0;
    }

    window =
        SDL_CreateWindow(argv[0], SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, ARG_WINDOW_WIDTH * ARG_SCALE,
                         ARG_WINDOW_HEIGHT * ARG_SCALE, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(0);
    }

    redraw();

    while (SDL_WaitEvent(&event) >= 0) {
        switch (event.type) {
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        return 0;
                        break;
                }
            } break;

            case SDL_QUIT: {
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            } break;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
