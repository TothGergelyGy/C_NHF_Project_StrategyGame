#include "adatkezeles.h"
#include "strukturak.h"
#include "kijelzo.h"
#include "jatekmenet.h"


#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>


int Osszkat_szamol(int id,Prov* gyok) {
 if (gyok==NULL)
    return 0;

 int kat;
 if (gyok->birtokos->id==id)
    kat=gyok->katona;
 else kat=0;

 return kat+Osszkat_szamol(id,gyok->bal)+Osszkat_szamol(id,gyok->jobb);

}

int Erosites_szamol(Prov* gyok, int id) {
    if (gyok==NULL)
        return 0;

    int talalt;
    if (gyok->birtokos->id==id)
        talalt=1;
    else talalt=0;

    return talalt+Erosites_szamol(gyok->bal,id)+Erosites_szamol(gyok->jobb,id);

}

int Dobas_vedo(int katonak) {
        int x,y;
        switch (katonak) {
            case 0:return 0;

            case 1:return rand()%6;

            default:
                x=rand()%6;
                y=rand()%6;
                if (x < y)
                    return y;
                return x;
        }

}

int Dobas_tamado(int katonak) {
        int x,y,z;
        switch (katonak) {
            case 1:
            case 2:
                return rand()%6;

            case 3:
                x=rand()%6;
                y=rand()%6;
                if (x < y)
                    return y;
                return x;

            default:
                x=rand()%6;
                y=rand()%6;
                z=rand()%6;
                if (z<=x && y<=x)
                    return x;
                if (x<=y && z<=y)
                    return y;
                if (x<=z && y<=z)
                    return z;

        }

}

void Kepzes_hozzaad(Osszadat* adat,Prov* gyok) {
    if (gyok==NULL)
        return;

    gyok->katona+=gyok->kepzes;
    gyok->kepzes=0;
    Kepzes_hozzaad(adat,gyok->bal);
    Kepzes_hozzaad(adat,gyok->jobb);

}

void Prov_valaszt(Osszadat* adat,SDL_Event ev,Prov* gyok) {
    if (gyok==NULL)
        return;

    double tav = sqrt(  (gyok->pkoord.x-ev.button.x)*(gyok->pkoord.x-ev.button.x) + (gyok->pkoord.y-ev.button.y)*(gyok->pkoord.y-ev.button.y) );
    if (tav < 30) {
        if (gyok->birtokos==adat->aktiv) {
            adat->valasztott=gyok;
            adat->hova=NULL;
            adat->mennyit=0;
            Kijelzo_master(adat);
            return;
        }
    }

    Prov_valaszt(adat,ev,gyok->bal);
    Prov_valaszt(adat,ev,gyok->jobb);

}

void Valaszt_master (Osszadat* adat,SDL_Event ev) {
    if (adat->mozgas==1 && adat->valasztott!=NULL)
        hova_valaszt(adat,ev,adat->gyok);
    else Prov_valaszt(adat,ev,adat->gyok);

}

void hova_valaszt (Osszadat* adat,SDL_Event ev,Prov* gyok) {
    if (gyok==NULL)
        return;

    double tav=sqrt(  (gyok->pkoord.x-ev.button.x)*(gyok->pkoord.x-ev.button.x) + (gyok->pkoord.y-ev.button.y)*(gyok->pkoord.y-ev.button.y) );
    if (tav < 30) {
        if (adat->szomszedsag[gyok->id][adat->valasztott->id]==1 && gyok!=adat->valasztott) {
            adat->hova=gyok;
            adat->mennyit=0;
            Kijelzo_master(adat);
            return;
        }
    }

    hova_valaszt(adat,ev,gyok->bal);
    hova_valaszt(adat,ev,gyok->jobb);

}

void Kepzes_plusz (Osszadat* adat,SDL_Event ev) {
    if (adat->valasztott==NULL)
        return;
    if (adat->aktiv->kepezheto>0) {
        adat->valasztott->kepzes+=1;
        adat->aktiv->kepezheto-=1;
        Kijelzo_master(adat);
        Prov_kirajzol(adat,adat->gyok);
    }
}

void Kepzes_minusz (Osszadat* adat,SDL_Event ev) {
    if (adat->valasztott==NULL)
        return;
    if (adat->valasztott->kepzes>0) {
        adat->aktiv->kepezheto+=1;
        adat->valasztott->kepzes-=1;
        Kijelzo_master(adat);
        Prov_kirajzol(adat,adat->gyok);
    }

}

void Mozgas_gomb(Osszadat* adat,SDL_Event ev) {

        adat->mozgas=(adat->mozgas-1)*(-1); //nem akartam bool-t használni ehhez az egy dologhoz
        Mozgas_jelzo_rajzol(adat);


}

void Mennyit_plusz(Osszadat* adat,SDL_Event ev) {
    if (adat->hova==NULL)
        return;
    if (adat->mennyit < adat->valasztott->katona) {
        adat->mennyit+=1;
        Kijelzo_master(adat);
    }
}

void Mennyit_minusz(Osszadat* adat,SDL_Event ev) {
    if (adat->hova==NULL)
        return;
    if (adat->mennyit > 0) {
        adat->mennyit-=1;
        Kijelzo_master(adat);
    }
}

void Korvege(Osszadat* adat,SDL_Event ev) {
            adat->aktiv->kepezheto+=adat->aktiv->erosites;
            Kepzes_hozzaad(adat,adat->gyok);
            Jatekos_kieses(adat);
            adat->aktiv=adat->aktiv->kov;

            adat->valasztott=NULL;
            adat->hova=NULL;
            adat->lepesek=3;
            adat->mennyit=0;
            adat->aktiv->erosites=Erosites_szamol(adat->gyok,adat->aktiv->id);
            adat->aktiv->osszkatona=Osszkat_szamol(adat->aktiv->id,adat->gyok);
            Kijelzo_master(adat);
            Prov_kirajzol(adat,adat->gyok);

}


void Mozgas_confirm(Osszadat* adat,SDL_Event ev) {
    if (adat->hova!=NULL && adat->valasztott!=NULL) {

        if (adat->hova->birtokos!=adat->aktiv) {    //harc
            adat->valasztott->katona-=adat->mennyit;                //elmennek a katonak a helyőrségükből
            while (adat->mennyit!=0 && adat->hova->katona!=0) {     //amig valamelyik sereg el nem fogy
                    int vedodobas=Dobas_vedo(adat->hova->katona);
                    int tamadodobas=Dobas_tamado(adat->mennyit);
                    if (tamadodobas > vedodobas)
                        adat->hova->katona-=1;
                    else adat->mennyit-=1;
            }
            if (adat->hova->katona==0) {        // ha a védők vesztettek
                adat->hova->birtokos=adat->aktiv;
                adat->hova->katona=adat->mennyit;
                adat->mennyit=0;
                Prov_kirajzol(adat,adat->gyok);
                Kijelzo_master(adat);
            }
            Korvege(adat,ev);
            Jatekos_kieses(adat);
        }

        else {  //mozgás saját területen belül
            adat->hova->katona+=adat->mennyit;
            adat->valasztott->katona-=adat->mennyit;
            adat->mennyit=0;
            adat->valasztott=NULL;
            adat->hova=NULL;

            adat->lepesek-=1;
            if (adat->lepesek==0)
                Korvege(adat,ev);
            Prov_kirajzol(adat,adat->gyok);
            Kijelzo_master(adat);
        }
    }
}


