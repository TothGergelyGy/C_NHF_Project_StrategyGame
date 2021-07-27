#include "adatkezeles.h"
#include "strukturak.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


Jatekos* Jatekos_beolvas() {
    Jatekos* eleje;
    eleje=NULL;
    FILE* fp=fopen("jatekos.bin","rb");

    int mennyit;                            //hány játékos lesz
    fread(&mennyit,sizeof(int),1,fp);

    for (int i=0; i<mennyit; i++) {
       Jatekos* uj;
       uj=(Jatekos*)malloc(sizeof(Jatekos));

        fread(uj,sizeof(Jatekos),1,fp);
        uj->kov=NULL;

       if (eleje==NULL)
                eleje=uj;
       else {
                Jatekos* mozgo;
                mozgo=eleje;
                while(mozgo->kov!=NULL)
                    mozgo=mozgo->kov;
                mozgo->kov=uj;
       }
    }


    Jatekos* mozgo;                 //létrehozza a kört:az elejét megjegyzi, elmegy a lista végére, a végét az elejéhez fûzi
    mozgo=eleje;
    while(mozgo->kov!=NULL)
        mozgo=mozgo->kov;
    mozgo->kov=eleje;

    fclose(fp);
    return eleje;
}

void Jatekos_free(Jatekos* eleje) {
    if (eleje==NULL)
        return;


    if (eleje->kov!=eleje) {
        Jatekos *mozgo,*eredeti;
        mozgo=eleje->kov;
        eredeti=eleje;
        while (mozgo!=eredeti) {
            mozgo=eleje->kov;
            free(eleje);
            eleje=mozgo;
        }
    }
    else free(eleje);
}

Jatekos* birtokoskeres(Jatekos* jatekos,int id) {
    Jatekos* mozgo,*eleje;
    mozgo=jatekos;
    eleje=jatekos;
    do {
        if (mozgo->id==id)
            return mozgo;
        mozgo=mozgo->kov;
    } while (mozgo!=eleje);

}

Prov* Prov_fa_beolvas(Jatekos* aktiv) {
    Prov* gyok=NULL;
    int mennyit;
    FILE* fp=fopen("prov.bin","rb");
    fread(&mennyit,sizeof(int),1,fp);

    for (int i=0; i<mennyit; i++) {
        Prov* uj;
        int owner;
        uj=(Prov*)malloc(sizeof(Prov));

        fread(uj,sizeof(Prov),1,fp);
        fread(&owner,sizeof(int),1,fp);

        uj->bal=NULL;
        uj->jobb=NULL;
        uj->birtokos=birtokoskeres(aktiv,owner);
        gyok=beszur(gyok,uj);
    }
    fclose(fp);
    return gyok;

}

Prov* beszur(Prov* gyok,Prov* uj) {
    if (gyok==NULL)
        gyok=uj;


    if (gyok->id < uj->id) {
        gyok->jobb=beszur(gyok->jobb,uj);
        if (Balancefactor(gyok) < -1) {     //a bal és jobb részfák magasságát nézi, bal pozitív, jobb negatív
            if (gyok->jobb->id < uj->id)    // jobb részfa jobb részfája nehéz, 1-jo2-joNehez sorrendben jönnek jobbra, átrendezi 1-joNehez-ba2 sorrendben
                gyok=bal_forgat(gyok);
            else {
                    gyok->jobb=jobb_forgat(gyok->jobb);
                    gyok=bal_forgat(gyok);  // jobb részfa bal részfája nehéz, 1-jo2-baNehez alakot egyenlit ki
            }
        }
    }


    if (uj->id < gyok->id) {
        gyok->bal=beszur(gyok->bal,uj);
        if (Balancefactor(gyok) > 1) {     // a bal részfa nehéz
            if (uj->id < gyok->bal->id)
                gyok=jobb_forgat(gyok);    //bal részfa bal részfája nehéz
            else {
                    gyok->bal=bal_forgat(gyok->bal);
                    gyok=jobb_forgat(gyok); //bal részfa jobb részfája nehéz
            }
        }

    }

    return gyok;

}

Prov* bal_forgat(Prov* gyok) {
    Prov* uj;
    uj=gyok->jobb;
    gyok->jobb=uj->bal;
    uj->bal=gyok;

    return uj;

}

Prov* jobb_forgat(Prov* gyok) {
    Prov* uj;
    uj=gyok->bal;
    gyok->bal=uj->jobb;
    uj->jobb=gyok;

    return uj;

}

void Prov_fa_free(Prov* gyok) {
    if (gyok==NULL)
        return;
    Prov_fa_free(gyok->bal);
    Prov_fa_free(gyok->jobb);
    free(gyok);

}

int Balancefactor(Prov* gyok) {
    return height(gyok->bal)-height(gyok->jobb);   //bal részfa magassága pozitív, jobb részfa magassága negatív

}

int height(Prov* gyok) {
    int balfa,jobbfa;
    if (gyok==NULL)
        return -1;
    balfa=height(gyok->bal);
    jobbfa=height(gyok->jobb);


    if (jobbfa < balfa)
        return balfa+1;         //részfa magassága a saját legmagasabb részfája+1
    return jobbfa+1;
}

Prov* Prov_keres(Prov* gyok, int id) {          //megkeresi a megadott ID-hez tartozó provinciát a fában
   Prov *mozgo = gyok;
   while (mozgo != NULL && mozgo->id != id) {
      if (id < mozgo->id) mozgo = mozgo->bal;
      else mozgo = mozgo->jobb;
   }
   return mozgo;

}

int** Szomszedsag_beolvas() {
    FILE* fp=fopen("szomszedsag.bin","rb");
    int meret;
    fread(&meret,sizeof(int),1,fp);
    int** tomb;

    tomb=(int**) malloc(meret * sizeof(int*));
    for (int i=0; i<meret; i++)
        tomb[i]=(int*)malloc(meret * sizeof(int));


    for (int i=0; i<meret; i++) {
        for (int j=0; j<meret; j++) {
            fread(&tomb[i][j],sizeof(int),1,fp);
        }
    }
    fclose(fp);
    tomb[0][0]=meret;
    return tomb;
}

void Szomszedsag_free(int** tomb) {
    int meret=tomb[0][0];
    for (int i=0; i<meret; i++)
        free(tomb[i]);
    free(tomb);

}



void Jatekos_kieses (Osszadat* adat) {


    Jatekos* mozgo;
    Jatekos* lemarado;
    mozgo=adat->aktiv->kov;
    lemarado=adat->aktiv;

    while (mozgo!=adat->aktiv) {
        if ( (Erosites_szamol(adat->gyok,mozgo->id)) ==0 ) {
            lemarado->kov=mozgo->kov;
            free(mozgo);
            mozgo=lemarado->kov;
        }
        else {
            lemarado=mozgo;
            mozgo=mozgo->kov;
        }
    }


    if (adat->aktiv==adat->aktiv->kov)
        Kijelzo_kiir(adat,"Győztél",8);
}



void Adat_inicializal(Osszadat* adat) {
    adat->aktiv=Jatekos_beolvas();
    adat->gyok=Prov_fa_beolvas(adat->aktiv);
    adat->hova=NULL;
    adat->valasztott=NULL;
    adat->lepesek=3;
    adat->aktiv->erosites=Erosites_szamol(adat->gyok,adat->aktiv->id);
    adat->aktiv->osszkatona=Osszkat_szamol(adat->aktiv->id,adat->gyok);
    adat->mozgas=0;
    adat->mennyit=0;
    adat->szomszedsag=Szomszedsag_beolvas();
}


void Adat_free(Osszadat* adat) {
    Jatekos_free(adat->aktiv);
    Prov_fa_free(adat->gyok);
    adat->hova=NULL;
    adat->valasztott=NULL;
    Szomszedsag_free(adat->szomszedsag);
}

void Uj_jatek(Osszadat* adat,SDL_Event ev) {
    Adat_free(adat);
    Adat_inicializal(adat);
    Prov_kirajzol(adat,adat->gyok);
    Kijelzo_master(adat);
    Mozgas_jelzo_rajzol(adat);

    //győzelmet írja felül
    boxRGBA(adat->renderer,adat->kezdo[8].x,adat->kezdo[8].y,adat->vegzo[8].x,adat->vegzo[8].y,115,0,0,255);

    SDL_RenderPresent(adat->renderer);

}
