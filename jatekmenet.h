#ifndef JATEKMENET_H_INCLUDED
#define JATEKMENET_H_INCLUDED

#include "adatkezeles.h"
#include "strukturak.h"
#include "kijelzo.h"

int Osszkat_szamol(int id,Prov* gyok);
int Erosites_szamol(Prov* gyok, int id);          //mindig újraszámol
void Kepzes_hozzaad (Osszadat* adat,Prov* gyok);  //inkrementál
int Dobas_vedo(int katonak);
int Dobas_tamado(int katonak);


void Prov_valaszt   (Osszadat* adat,SDL_Event ev,Prov* gyok);
void hova_valaszt   (Osszadat* adat,SDL_Event ev,Prov* gyok);
void Valaszt_master (Osszadat* adat,SDL_Event ev);

void Kepzes_plusz(Osszadat* adat,SDL_Event ev);
void Kepzes_minusz(Osszadat* adat,SDL_Event ev);
void Mozgas_gomb(Osszadat* adat,SDL_Event ev);
void Mennyit_plusz(Osszadat* adat,SDL_Event ev);
void Mennyit_minusz(Osszadat* adat,SDL_Event ev);
void Korvege(Osszadat* adat,SDL_Event ev);
void Mozgas_confirm(Osszadat* adat,SDL_Event ev);

#endif // JATEKMENET_H_INCLUDED
