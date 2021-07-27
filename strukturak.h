#ifndef STRUKTURAK_H_INCLUDED
#define STRUKTURAK_H_INCLUDED


#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Koord {
    int x;
    int y;

} Koord;


typedef struct Jatekos {
    char nev[30+1];
    int id;
    int osszkatona;         //az összes birtoklott provincién lévõ katonák
    int kepezheto;          //az összes fel nem használt újonc
    int erosites;           //a kör elején birtoklott provinciák száma, kör végén hozzáadódik kepezhetohoz, majd 0zodik
    int r,g,b;
    struct Jatekos* kov;

} Jatekos;


typedef struct Prov {
    int id;                 //a fa felépítésének kulcsa, ez alapján rendezve
    char nev[20+1];
    int katona;
    int kepzes;             //az adott körben itt képzett katonák, a kör végén hozzáadódik katona-hoz, majd 0zodik
    Koord kepzeskoord;      //a képzés számát ide írja ki
    Koord katonakoord;      //a jelenlévõ katonákat ide írja ki
    Koord pkoord;
    struct Prov *bal,*jobb;
    Jatekos* birtokos;

} Prov;


typedef struct Osszadat {
    Prov* gyok;
    Jatekos* aktiv;
    SDL_Renderer* renderer;
    TTF_Font* font;
    Koord* kezdo;
    Koord* vegzo;
    int** szomszedsag;
    //fő adatok
    Prov* valasztott;
    Prov* hova;
    int lepesek;
    int mozgas; //bool-ként használva
    int mennyit;
    //számon tartott változók
} Osszadat;

#endif // STRUKTURAK_H_INCLUDED
