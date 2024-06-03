#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#define NAME "Font Example"
#define WIDTH 480
#define HEIGHT 272

int closed = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

TTF_Font *fontSquare = NULL;
SDL_Texture* title = NULL;
SDL_Color fontColor = {0, 0, 0};

int setupSdl() {
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS|SDL_INIT_GAMECONTROLLER);
    SDL_StopTextInput();

    window = SDL_CreateWindow(
        NAME,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initiation window. Error %s", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize renderer. Error: %s", SDL_GetError());
        return 1;
    }

    return 0;
}

void updateTitle(const char * text) {
    SDL_Surface *surface1 = TTF_RenderUTF8_Blended(fontSquare, text, fontColor);
    if (surface1 == NULL) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to load create title! SDL Error: %s\n", SDL_GetError());
        exit(3);
    }
    SDL_DestroyTexture(title);
    title = SDL_CreateTextureFromSurface(renderer, surface1);
    if (title == NULL) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to load texture for image block.bmp! SDL Error: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surface1);
}

void handleEvents(){
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                closed = 1;
                break;
            case SDL_CONTROLLERDEVICEADDED:
                    SDL_GameControllerOpen(event.cdevice.which);
                    break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    closed = 1;
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_START) {
                    closed = 1;
                } else if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
                    if (SDL_HasScreenKeyboardSupport() == SDL_TRUE) {
                        SDL_StartTextInput();
                        updateTitle("Input");
                    } else {
                       updateTitle("Can't");
                    }
                } else if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B) {
                    SDL_StopTextInput();
                    updateTitle("No Input");
                } else if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y) {
                    SDL_StartTextInput();
                    updateTitle("Input");
                }
                break;
            case SDL_TEXTINPUT:
                updateTitle(event.text.text);
                break;
        }
    }
}

int game(int argc, char *argv[]){
    SDL_SetHint("SDL_HINT_ENABLE_SCREEN_KEYBOARD", "1");
    printf("Starting game\n");
    if (setupSdl() != 0) {
        return 1;
    }

    if (TTF_Init() == -1) {
        return 5;
    }
    // open font files
    TTF_Font *font2P = TTF_OpenFont("PressStart2P.ttf", 16);
    if (font2P == NULL) {
        printf("TTF_OpenFont font2P: %s\n", TTF_GetError());
        return 2;
    }
    fontSquare = TTF_OpenFont("square_sans_serif_7.ttf", 64);
    if (fontSquare == NULL) {
        printf("TTF_OpenFont fontSquare: %s\n", TTF_GetError());
        return 2;
    }
    // load title
    updateTitle("Hello!");

    // load subtitle
    SDL_Surface *surface2 = TTF_RenderUTF8_Blended(font2P, "Press start to quit", fontColor);
    if (surface2 == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to load create subtitle! SDL Error: %s\n", SDL_GetError());
        return 3;
    }
    SDL_Texture* subtitle = SDL_CreateTextureFromSurface(renderer, surface2);
    if (title == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to load texture for image block.bmp! SDL Error: %s\n", SDL_GetError());
        return 4;
    }
    SDL_FreeSurface(surface2);

    // Create SDL 
    SDL_Rect titleRect;

    SDL_Rect subtitleRect;
    SDL_QueryTexture(subtitle, NULL, NULL, &subtitleRect.w, &subtitleRect.h);
    subtitleRect.x = WIDTH/2 - subtitleRect.w/2;
    subtitleRect.y = subtitleRect.h/2;

    while (!closed) {
        handleEvents();
        SDL_QueryTexture(title, NULL, NULL, &titleRect.w, &titleRect.h);
        titleRect.x = WIDTH/2 - titleRect.w/2;
        titleRect.y = HEIGHT/2 - titleRect.h/2;
        SDL_RenderCopy(renderer, title, NULL, &titleRect);
        SDL_RenderCopy(renderer, subtitle, NULL, &subtitleRect);
        SDL_RenderPresent(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    return game(argc, argv);
}
