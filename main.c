#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define SZELES 1200
#define MAGAS 650
#include "adatkezeles.h"
#include "strukturak.h"
#include "kijelzo.h"
#include "jatekmenet.h"

/* kulon fuggvenybe, hogy olvashatobb legyen */
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}


int main(int argc, char *argv[]) {

   srand(time(0));


   //ezek a kijelző koordinátái, nem változnak sose
   Koord kezdo[9] = { {10,40}, {170,84}, {170,106}, {170,130}, {189,414}, {117,322}, {9,386}, {10,200}, {8,461} };
   Koord vegzo[9] = {{200,75}, {200,101}, {200,123}, {200,150}, {210,433}, {140,343}, {200,410}, {200,225}, {190,499} };

   //ezek a kijelző gombjainak koordinátái, nem változnak sose
   Koord gombkezdo[8] = { {173,232}, {204,232}, {7,277}, {173,322}, {205,322}, {10,514}, {9,589}, {179,270} };
   Koord gombvegzo[8] = { {194,255}, {225,255}, {93,306}, {194,342}, {225,342}, {124,555}, {126,636}, {217,307} };


   /* ablak létrehozása */
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("Riziko", SZELES, MAGAS, &window, &renderer);
    SDL_Texture* kep=IMG_LoadTexture(renderer,"terkep.jpg");
    SDL_Rect hova={0,0,SZELES,MAGAS};
    SDL_RenderCopy(renderer,kep,NULL,&hova);
    /* térkép betöltése és ablakra másolása*/
    SDL_RenderPresent(renderer);

    TTF_Init();

    void (*fptr[8])(Osszadat* adat,SDL_Event ev) = { Kepzes_plusz, Kepzes_minusz, Mozgas_gomb, Mennyit_plusz, Mennyit_minusz, Korvege, Uj_jatek, Mozgas_confirm   };



    Osszadat adat;
    adat.renderer=renderer;
    adat.kezdo=kezdo;
    adat.vegzo=vegzo;
    adat.font = TTF_OpenFont("LiberationSerif-Regular.ttf", 20);
    if (!adat.font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Event ev;
    Adat_inicializal(&adat);
    Uj_jatek(&adat,ev);
    Mozgas_jelzo_rajzol(&adat);


    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT) {
            if (ev.type==SDL_MOUSEBUTTONDOWN && ev.button.button==SDL_BUTTON_LEFT) {
                for (int i=0; i<8; i++)
                    if (  gombkezdo[i].x<ev.button.x && ev.button.x<gombvegzo[i].x && gombkezdo[i].y<ev.button.y && ev.button.y<gombvegzo[i].y )
                        fptr[i](&adat,ev);
                Valaszt_master(&adat,ev);
                Jatekos_kieses(&adat);
                SDL_RenderPresent(adat.renderer);


            }

    }



    /* ablak bezarasa */
    Adat_free(&adat);
    SDL_DestroyTexture(kep);
    TTF_CloseFont(adat.font);
    SDL_Quit();

    return 0;
}
