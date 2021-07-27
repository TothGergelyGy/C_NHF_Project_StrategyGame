#ifndef ADATKEZELES_H_INCLUDED
#define ADATKEZELES_H_INCLUDED

#include "strukturak.h"



Jatekos* Jatekos_beolvas();
void Jatekos_free(Jatekos* eleje);
int** Szomszedsag_beolvas();
void Szomszedsag_free(int** tomb);
//fa építõk
Jatekos* birtokoskeres(Jatekos* jatekos,int id);
Prov* Prov_fa_beolvas(Jatekos* jatekos);
Prov* beszur(Prov* gyok,Prov* uj);
Prov* bal_forgat(Prov* gyok);
Prov* jobb_forgat(Prov* gyok);
void Prov_fa_free(Prov* gyok);
int Balancefactor(Prov* gyok);
int height(Prov* gyok);
Prov* Prov_keres(Prov* gyok, int id);

void Jatekos_kieses (Osszadat* adat);
void Adat_inicializal(Osszadat* adat);
void Adat_free(Osszadat* adat);
void Uj_jatek(Osszadat* adat,SDL_Event ev); // SDL_Event ev alakra prtrhez

#endif // ADATKEZELES_H_INCLUDED
