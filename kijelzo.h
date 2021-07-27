#ifndef KIJELZO_H_INCLUDED
#define KIJELZO_H_INCLUDED

#include "adatkezeles.h"
#include "strukturak.h"
#include "kijelzo.h"
#include <SDL2/SDL_ttf.h>


void Prov_kirajzol(Osszadat* adat,Prov* gyok);
void Katona_rajzol_rajzolo(Osszadat* adat,int x, int y, char* szoveg);
void Kijelzo_kiir(Osszadat* adat,char* szoveg, int hanydik);
void Mozgas_jelzo_rajzol(Osszadat* adat);
void Kijelzo_master (Osszadat* adat);
#endif // KIJELZO_H_INCLUDED
