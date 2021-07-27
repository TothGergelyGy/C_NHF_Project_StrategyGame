#include "adatkezeles.h"
#include "strukturak.h"
#include "kijelzo.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void Prov_kirajzol(Osszadat* adat,Prov* gyok) {
    if (gyok==NULL)
        return;
    //szines körök
    filledCircleRGBA(adat->renderer,gyok->pkoord.x,gyok->pkoord.y,30,255,255,255,255);
    filledCircleRGBA(adat->renderer,gyok->pkoord.x,gyok->pkoord.y,30,gyok->birtokos->r,gyok->birtokos->g,gyok->birtokos->b,190);

    //helyi katonak
    char szamstring[5];
    sprintf(szamstring,"%d",gyok->katona);
    Katona_rajzol_rajzolo(adat,gyok->katonakoord.x,gyok->katonakoord.y,szamstring);

    //helyi kepzes
    sprintf(szamstring,"+%d",gyok->kepzes);
    Katona_rajzol_rajzolo(adat,gyok->kepzeskoord.x,gyok->kepzeskoord.y,szamstring);


    SDL_RenderPresent(adat->renderer);
    Prov_kirajzol(adat,gyok->bal);
    Prov_kirajzol(adat,gyok->jobb);

}


void Katona_rajzol_rajzolo(Osszadat* adat,int x, int y, char* szoveg) {
    SDL_Surface* felirat;
    SDL_Texture* felirat_t;
    SDL_Color szin = {0,0,0};
    felirat=TTF_RenderUTF8_Blended(adat->font,szoveg,szin);
    felirat_t=SDL_CreateTextureFromSurface(adat->renderer,felirat);

    SDL_Rect hova;
    hova.x=x;
    hova.y=y;
    hova.w=felirat->w;
    hova.h=felirat->h;

    SDL_RenderCopy(adat->renderer,felirat_t,NULL,&hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
    SDL_RenderPresent(adat->renderer);

}


void Kijelzo_kiir(Osszadat* adat,char* szoveg, int hanydik) {
    boxRGBA(adat->renderer,adat->kezdo[hanydik].x,adat->kezdo[hanydik].y,adat->vegzo[hanydik].x,adat->vegzo[hanydik].y,115,0,0,255);

    SDL_Surface* felirat;
    SDL_Texture* felirat_t;
    SDL_Color szin = {adat->aktiv->r,adat->aktiv->g,adat->aktiv->b};

    felirat=TTF_RenderUTF8_Blended(adat->font,szoveg,szin);
    felirat_t=SDL_CreateTextureFromSurface(adat->renderer,felirat);

    SDL_Rect hova;
    hova.x=adat->kezdo[hanydik].x;
    hova.y=adat->kezdo[hanydik].y;
    hova.w=felirat->w;
    hova.h=felirat->h;

    SDL_RenderCopy(adat->renderer,felirat_t,NULL,&hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
    SDL_RenderPresent(adat->renderer);


}


void Kijelzo_master (Osszadat* adat) {
    char szoveg[6][30+1];
    sprintf(szoveg[0],"%s",adat->aktiv->nev);
    sprintf(szoveg[1],"%d",adat->aktiv->osszkatona);
    sprintf(szoveg[2],"%d",adat->aktiv->kepezheto);
    sprintf(szoveg[3],"+%d",adat->aktiv->erosites);
    sprintf(szoveg[4],"%d",adat->lepesek);
    sprintf(szoveg[5],"%d",adat->mennyit);

    for (int i=0; i<6; i++)
        Kijelzo_kiir(adat,szoveg[i],i);

    if (adat->hova==NULL)
        boxRGBA(adat->renderer,adat->kezdo[6].x,adat->kezdo[6].y,adat->vegzo[6].x,adat->vegzo[6].y,115,0,0,255);
    else   Kijelzo_kiir(adat,adat->hova->nev,6);

    if (adat->valasztott==NULL)
        boxRGBA(adat->renderer,adat->kezdo[7].x,adat->kezdo[7].y,adat->vegzo[7].x,adat->vegzo[7].y,115,0,0,255);
    else Kijelzo_kiir(adat,adat->valasztott->nev,7);


}


void Mozgas_jelzo_rajzol(Osszadat* adat) {

    //zöld
        if (adat->mozgas==1) {    //ha be van kapcsolva a zöld világít
            circleRGBA(adat->renderer,113,288,12,0,0,0,255);
            filledCircleRGBA(adat->renderer,113,288,11,34,177,76,255);
        }
        else {
            circleRGBA(adat->renderer,113,288,12,0,0,0,255);
            filledCircleRGBA(adat->renderer,113,288,11,10, 48, 21,255);
        }


    //piros
        if (adat->mozgas==0) {   //ha be van kapcsolva a piros sötét
            circleRGBA(adat->renderer,139,288,12,0,0,0,255);
            filledCircleRGBA(adat->renderer,139,288,11,237, 28, 36,255);
        }
        else {         // ha ki van kapcsolva, a piros világít
            circleRGBA(adat->renderer,139,288,12,0,0,0,255);
            filledCircleRGBA(adat->renderer,139,288,11,138, 0, 21,255);
        }


}

