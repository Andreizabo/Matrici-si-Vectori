#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <winbgim.h>
#include <cstdlib>
#include <ctime>
#include "wtypes.h"
#define NRMAX 51
#define TIP double
#define BLACK	0
#define BLUE	1
#define GREEN	2
#define CYAN	3
#define RED	4
#define MAGENTA	5
#define BROWN	6
#define LIGHTGRAY	7
#define DARKGRAY	8
#define LIGHTBLUE	9
#define LIGHTGREEN	10
#define LIGHTCYAN	11
#define LIGHTRED	12
#define LIGHTMAGENTA	13
#define YELLOW	14
#define WHITE	15

using namespace std;

char erori[NRMAX][NRMAX]; //erori

int screenW,screenH; //dimensiuni ecran

int width = 350,height = 800; //marimile ferestrei principale

int boxSize = 80; //marimea unui patrat standard

int spatiustd = 50; //spatiu fata de margine standard

int winds = 0; //id winodw

int nrMatrici = 0,nrVectori = 0,nrButoane = 0, nrMatriciSelectate = 0, nrVectoriSelectati = 0; // nr curent/selectate de matrici/vectori/butoane

int meniucrt = 1; // meniul curent

int y_Offset = 0, x_Offset = 0; // decalajul x,y pentru meniu

int y_OffsetM = 0, x_OffsetM = 0; // decalajul x,y pentru matrici

int y_OffsetV = 0, x_OffsetV = 0; // decalajul x,y pentru vectori

int trebuie_selectate = 0; // cate matrici trebuie selectate

int trebuie_selectateV = 0; // cati vectori trebuie selectati

bool dontrefresh = false; // daca sa nu se dea refresh la meniu

bool editnew = false; // daca sa editezi matricea in loc sa creezi una noua

TIP scalar = 0; // scalar

int inputWinId = 3; // id-ul curent al ferestrei de input

int waitTime = 1000; // wait time pentru animatii

struct coordonateMat {
    int x1,x2,y1,y2;
}CoordMat[NRMAX];

struct coordonateVect {
    int x1,x2,y1,y2;
}CoordVect[NRMAX];

void changemen(int a); // schimba meniul

TIP putereScalar(TIP x, int k); // calculeaza un numar la o putere data

int putereMinusUnu(int k); // calculeaza -1 la o putere

void initErori(); // initializeaza erorile

void square(int x1,int y1,int x2, int y2); // deseneaza un patrat

void citesteInModGraficNR(int nr, int x, int y, int culoareText, int m); // citire dintr-o fereastra grafica de double, rezultatule e pus in variabila globala "scalar", si se schimba la final meniul in m

void citesteInModGraficNRint(int nr, int x, int y, int culoareText, int m); // citire dintr-o fereastra grafica de int, rezultatule e pus in variabila globala "scalar", si se schimba la final meniul in m

void citesteInModGraficNRspecific(int nr, int x, int y, int culoareText, int m, TIP& numar); // citire dintr-o fereastra grafica de double, rezultatule e pus in variabila numar

char* intToString(TIP x) // converteste un numar double in char*
{
    char* buffer = (char*) malloc(256);

    sprintf(buffer, "%.5g", x);

    return buffer;
}

TIP maxim(TIP a, TIP b) // maxim intre 2 numere a si b
{
    return (a > b) ? a : b;
}

struct matrice { // matrice
    TIP elemente[NRMAX][NRMAX];
    int nrLinii,nrColoane;
    int id;

    bool selected = false; // daca a fost selectata pentru o operatie
    bool deleteme = false; // daca trebuie stearsa

    matrice()
    {
        nrLinii = 0;
        nrColoane = 0;
        elemente[0][0] = 0;
    }

    void citireTastatura();

    void citireFisier();

    void genRandom();

    void afisareEcran();

    void afisareFisier();

    void adunareMatrici(matrice a, matrice b);

    void scadereMatrici(matrice a, matrice b);

    void produsMatriceScalar(matrice a, TIP k);

    void impartireMatriceScalar(matrice a, TIP k);

    void inmultireMatrici(matrice a, matrice b);

    void impartireMatrici(matrice a, matrice b);

    void transpusaMatrice(matrice a);

    TIP gammaMatrice(int linie, int coloana);

    TIP determinantMatrice();

    void inversaMatrice(matrice a);

    void adjunctaMatrice(matrice a);

    void ridicaLaPutereMatrice(matrice a, int k);

    bool esteRara();

    int numarElementeZero();

    double densitateMatrice();

    TIP sumaDP();

    TIP sumaSubDP();

    TIP sumaDeasupraDP();

    TIP sumaDS();

    TIP sumaSubDS();

    TIP sumaDeasupraDS();
};

matrice matriciCreate[NRMAX],matriciSelectate[NRMAX],matriciNuSterge[NRMAX]; // matricile curente / matricile selectate acum / matricile care NU vor fi sterse in algoritmul de stergere

struct vect // vector
{
    TIP elemente[NRMAX];
    int nrElemente;
    int id;

    bool selected = false; // daca a fost selectat pentru o operatie
    bool deleteme = false; // daca trebuie sters

    vect()
    {
        nrElemente = 0;
        elemente[0] = 0;
    }

    void citireTastatura();

    void citireFisier();

    void genRandom();

    void afisareEcran();

    void afisareFisier();

    void adunareVectori(vect v, vect w);

    void scadereVectori(vect v, vect w);

    void produsVectorScalar(vect v, TIP k);

    void impartireVectorScalar(vect v, TIP k);

    void sumaElemente();

    void produsElemente();

    void insertElement(TIP x, int k);

    void bubble(char mode);

    void insertie(char mode);

    void interschimbare(char mode);
};

vect vectoriCreati[NRMAX],vectoriSelectati[NRMAX],vectoriNuSterge[NRMAX]; // vectori curenti / vectori selectati acum / vectori care NU vor fi stersi in algoritmul de stergere

void resizeBox();

void afisareMeniu(int x); // afiseaza meniul, adica toate butoanele din meniul curent din fereastra din stanga

void afisareMatrici(); // afiseaza toate matricile care exista acum

void afisareVectori(); // afiseaza toti vectorii care exista acum

void adaugaMatrice(matrice& m); // adauga o matrice in matriciCreate

void adaugaVector(vect& v); // adauga un vector in vectoriCreati

void stergeMatrice(int x); // sterge o matrice din matriciCreate

void stergeVector(int x); // sterge un vector din vectoriCreati

//functii animatie vectori

void afisText_final_suma(TIP s);
void afisText_intermediar_suma(TIP s);
void program_principal_suma(vect v);
void vect_final_suma(vect v);
void vect_intermediar_suma(int m, vect v);

void afisText_final_produs(TIP p);
void afisText_intermediar_produs(TIP p);
void program_principal_produs(vect v);
void vect_final_produs(vect v);
void vect_intermediar_produs(int m, vect v);

void gradatie_Oy_desenare_vector();
void gradatie_Ox_desenare_vector();
void linie_punctata_Ox_desenare_vector(int x2,int y2, int cy);
void linie_punctata_Oy_desenare_vector(int x2,int y2, int cx);
void linie_punctata_Ox_final_desenare_vector(int x2f,int y2f, int cy);
void linie_punctata_Oy_final_desenare_vector(int x2f,int y2f, int cx);
void program_final_desenare_vector();

void mutare_pozitie_dreapta_inserare_element(int poz1, int n, TIP V2[NRMAX]);
void vector_rezultat_inserare_element(int n, TIP V[NRMAX]);
void program_final_inserare_element(vect v, TIP element, int poz);

void afisText_pozitionare_bubble_sort(int i);
void afisText_swap_bubble_sort(int i);
void afisText_vect_ord_bubble_sort();
void vect_col_el_pt_swap_bubble_sort(int p, int n, TIP V[NRMAX]);
void vect_intermediar_swap_bubble_sort(int p, int n, TIP V[NRMAX]);
void vect_ordonat_bubble_sort(int n, TIP V[NRMAX]);
void program_final_bubble_sort_(vect v, bool crescator);

void afisText_VS_insertie(int x1);
void afisText_VNS_insertie(int x1);
void perete_insertie(int x1);
void afisText_vect_init_insertie();
void afisText_vect_ord_insertie();
void vect_col1_insertie(TIP V[NRMAX], int p, int n);
void vect_col2_insertie(TIP V[NRMAX], int p, int n);
void vect_init_insertie(int n, TIP V[NRMAX]);
void vect_final_insertie(int n, TIP V[NRMAX]);
void program_final_sortare_prin_insertie(vect v, bool crescator);


void afisText_pozitionare_interschimbare(int i, int j);
void afisText_swap_interschimbare(int i, int j);
void afisText_vect_ord_interschimbare();
void vect_pozitionare_swap_interschimbare(int n, TIP V[NRMAX], int p, int l);
void vect_intermediar_swap_interschimbare(int n, TIP V[NRMAX], int p, int l);
void vect_ordonat(int n, TIP V[NRMAX]);
void program_final_sortare_prin_interschimbare(vect v, bool crescator);


struct button { // buton
    int x1,x2,y1,y2; // coordonate buton
    char text[NRMAX];
    int id;

    button()
    {
        //oof
    }

    button(int xx1,int yy1,int xx2,int yy2,char t[NRMAX],int k)
    {
        x1 = xx1;
        x2 = xx2;
        y1 = yy1;
        y2 = yy2;
        strcpy(text,t);
        id = k;
    }

    bool didIPress(int a,int b,int men) // functie care verifica daca am apasat pe un anumit buton
    {
        if(getcurrentwindow() != 0) return false;
        if(men == id/10)
        {
            if(a >= x1 && a <= x2 && b - y_Offset >= y1 && b - y_Offset <= y2) return true;
            else return false;
        }
        else return false;
    }
    void onClick() // functie care retine actiunile pe care le face fiecare buton in functie de ID, si in momentul apasarii realizeaza operatia corespunzatoare
    {
        cleardevice();
        y_Offset = 0;
        if(id%10 == 0) //iesi sau inapoi
        {
            changemen(meniucrt/10);
            trebuie_selectate = 0;
            nrMatriciSelectate = 0;
            for(int i = 0; i < nrMatrici; i++)
                matriciCreate[i].selected = false;
            trebuie_selectateV = 0;
            nrVectoriSelectati = 0;
            for(int i = 0; i < nrVectori; i++)
                vectoriSelectati[i].selected = false;
            dontrefresh = false;
        }
        else if(id == 11) //matrici meniu
        {
            changemen(11);
        }
        else if(id == 12) //vectori meniu
        {
            changemen(12);
        }
        else if(id == 111) //mat noua meniu
        {
            changemen(111);
        }
        else if(id == 112) //operatii matrici
        {
            changemen(112);
        }
        else if(id == 114) //matrici rare
        {
            changemen(114);
        }
        else if(id == 115) //sterge mat
        {
            trebuie_selectate = nrMatrici;
            changemen(115);
        }
        else if(id == 121) //vect nou meniu
        {
            changemen(121);
        }
        else if(id == 122) // operatii vect meniu
        {
            changemen(122);
        }
        else if(id == 123) // sterge vect
        {
            trebuie_selectateV = nrVectori;
            changemen(123);
        }
        else if(id == 124) //sort
        {
            changemen(124);
        }
        else if(id == 1111) //citeste mat fisier
        {
            matrice a;
            adaugaMatrice(a);
            matriciCreate[nrMatrici-1].citireFisier();
        }
        else if(id == 1112) //citeste mat ecran
        {
            matrice a;
            adaugaMatrice(a);
            matriciCreate[nrMatrici-1].citireTastatura();
        }
        else if(id == 1113) //random
        {
            matrice a;
            adaugaMatrice(a);
            matriciCreate[nrMatrici-1].genRandom();
        }
        else if(id == 1121) //afiseaza matrice in fisier
        {
            trebuie_selectate = 1;
            changemen(1121);
        }
        else if(id == 1122) //aduna matrici
        {
            trebuie_selectate = nrMatrici;
            changemen(1122);
        }
        else if(id == 1123) //scazi matrici
        {
            trebuie_selectate = nrMatrici;
            changemen(1123);
        }
        else if(id == 1124) //inmultire scalar
        {
            trebuie_selectate = 1;
            changemen(99999);
            initwindow(width,100,"Input",spatiustd,height - 200 + spatiustd,false,false);
            setcurrentwindow(inputWinId);
            citesteInModGraficNR(scalar,25,25,RED,1124);
        }
        else if(id == 1125) //impartire scalar
        {
            trebuie_selectate = 1;
            changemen(99999);
            initwindow(width,100,"Input",spatiustd,height - 200 + spatiustd,false,false);
            setcurrentwindow(inputWinId);
            citesteInModGraficNR(scalar,25,25,RED,1125);
        }
        else if(id == 1126) //inmultire mxm
        {
            trebuie_selectate = nrMatrici;
            changemen(1126);
        }
        else if(id == 1127) //impartire mxm
        {
            trebuie_selectate = nrMatrici;
            changemen(1127);
        }
        else if(id == 1128) //operatii matrici 2
        {
            changemen(113);
        }
        else if(id == 1131) //transpusa
        {
            trebuie_selectate = 1;
            changemen(1131);
        }
        else if(id == 1132) //determinant
        {
            trebuie_selectate = 1;
            changemen(1132);
        }
        else if(id == 1133) //adjuncta
        {
            trebuie_selectate = 1;
            changemen(1133);
        }
        else if(id == 1134) //inversa
        {
            trebuie_selectate = 1;
            changemen(1134);
        }
        else if(id == 1135) //ridicare matrice la putere
        {
            trebuie_selectate = 1;
            changemen(99999);
            initwindow(width,100,"Input",spatiustd,height - 200 + spatiustd,false,false);
            setcurrentwindow(inputWinId);
            citesteInModGraficNRint(scalar,25,25,RED,1135);
        }
        else if(id == 1136) //operatii matrici 1
        {
            changemen(112);
        }
        else if(id == 1137) //operatii matrici 3
        {
            changemen(116);
        }
        else if(id == 1141) // numar 0
        {
            trebuie_selectate = 1;
            changemen(1141);
        }
        else if(id == 1142) // densitate
        {
            trebuie_selectate = 1;
            changemen(1142);
        }
        else if(id == 1143) // este rara?
        {
            trebuie_selectate = 1;
            changemen(1143);
        }
        else if(id == 1151) // sterge matrici
        {
            if(!nrMatriciSelectate)
            {
                char txt[NRMAX];
                strcpy(txt,erori[2]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                for(int i = 0; i < nrMatriciSelectate; i++)
                    matriciCreate[matriciSelectate[i].id].deleteme = true;
                for(int i = 0; i < nrMatrici; i++)
                    if(matriciCreate[i].deleteme) stergeMatrice(matriciCreate[i].id);
                for(int i = 0; i < nrMatrici; i++)
                    if(matriciCreate[i].deleteme) stergeMatrice(matriciCreate[i].id);
                for(int i = 0; i < nrMatrici; i++)
                    if(matriciCreate[i].deleteme) stergeMatrice(matriciCreate[i].id);
                for(int i = 0; i < nrMatrici; i++)
                    if(matriciCreate[i].deleteme) stergeMatrice(matriciCreate[i].id);
                for(int i = 0; i < nrMatrici; i++)
                    if(matriciCreate[i].deleteme) stergeMatrice(matriciCreate[i].id);
                changemen(meniucrt/10);
                trebuie_selectate = 0;
                nrMatriciSelectate = 0;
                for(int i = 0; i < nrMatrici; i++)
                matriciCreate[i].selected = false;
            }
        }
        else if(id == 1161) //suma diag principala
        {
            trebuie_selectate = 1;
            changemen(1161);
        }
        else if(id == 1162) //suma sub diag principala
        {
            trebuie_selectate = 1;
            changemen(1162);
        }
        else if(id == 1163) //suma deasupra diag principala
        {
            trebuie_selectate = 1;
            changemen(1163);
        }
        else if(id == 1164) //suma diag secundara
        {
            trebuie_selectate = 1;
            changemen(1164);
        }
        else if(id == 1165) //suma sub diag secundara
        {
            trebuie_selectate = 1;
            changemen(1165);
        }
        else if(id == 1166) //suma deasupra diag secundara
        {
            trebuie_selectate = 1;
            changemen(1166);
        }
        else if(id == 1167) // operatii mat 2
        {
            changemen(113);
        }
        else if(id == 1211) // citeste vect fisier
        {
            vect v;
            adaugaVector(v);
            vectoriCreati[nrVectori-1].citireFisier();
        }
        else if(id == 1212) // citeste vect ecran
        {
            vect v;
            adaugaVector(v);
            vectoriCreati[nrVectori-1].citireTastatura();
        }
        else if(id == 1213) // vect random
        {
            vect v;
            adaugaVector(v);
            vectoriCreati[nrVectori-1].genRandom();
        }
        else if(id == 1221) // afisare vect
        {
            trebuie_selectateV = 1;
            changemen(1221);
        }
        else if(id == 1222) // adunare vect
        {
            trebuie_selectateV = nrVectori;
            changemen(1222);
        }
        else if(id == 1223) // scadere vect
        {
            trebuie_selectateV = nrVectori;
            changemen(1223);
        }
        else if(id == 1224) // produs cu scalar vect
        {
            trebuie_selectateV = 1;
            changemen(99999);
            initwindow(width,100,"Input",spatiustd,height - 200 + spatiustd,false,false);
            setcurrentwindow(inputWinId);
            citesteInModGraficNR(scalar,25,25,RED,1224);
        }
        else if(id == 1225) // impartire la vect
        {
            trebuie_selectateV = 1;
            changemen(99999);
            initwindow(width,100,"Input",spatiustd,height - 200 + spatiustd,false,false);
            setcurrentwindow(inputWinId);
            citesteInModGraficNR(scalar,25,25,RED,1225);
        }
        else if(id == 1226) // suma el
        {
            trebuie_selectateV = 1;
            changemen(1226);
        }
        else if(id == 1227) // produs el
        {
            trebuie_selectateV = 1;
            changemen(1227);
        }
        else if(id == 1228) // insert el
        {
            trebuie_selectateV = 1;
            changemen(1228);
        }
        else if(id == 1229) // draw vect
        {
            program_final_desenare_vector();
        }
        else if(id == 1241) // bubble c
        {
            trebuie_selectateV = 1;
            changemen(1241);
        }
        else if(id == 1242) // bubble d
        {
            trebuie_selectateV = 1;
            changemen(1242);
        }
        else if(id == 1243) // insertie c
        {
            trebuie_selectateV = 1;
            changemen(1243);
        }
        else if(id == 1244) // insertie d
        {
            trebuie_selectateV = 1;
            changemen(1244);
        }
        else if(id == 1245) // interschimbare c
        {
            trebuie_selectateV = 1;
            changemen(1245);
        }
        else if(id == 1246) // interschimbare d
        {
            trebuie_selectateV = 1;
            changemen(1246);
        }
        else if(id == 1231) // sterge vect
        {
            if(!nrVectoriSelectati)
            {
                char txt[NRMAX];
                strcpy(txt,erori[16]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                for(int i = 0; i < nrVectoriSelectati; i++)
                    vectoriCreati[vectoriSelectati[i].id].deleteme = true;
                for(int i = 0; i < nrVectori; i++)
                    if(vectoriCreati[i].deleteme) stergeVector(vectoriCreati[i].id);
                for(int i = 0; i < nrVectori; i++)
                    if(vectoriCreati[i].deleteme) stergeVector(vectoriCreati[i].id);
                    for(int i = 0; i < nrVectori; i++)
                    if(vectoriCreati[i].deleteme) stergeVector(vectoriCreati[i].id);
                    for(int i = 0; i < nrVectori; i++)
                    if(vectoriCreati[i].deleteme) stergeVector(vectoriCreati[i].id);
                    for(int i = 0; i < nrVectori; i++)
                    if(vectoriCreati[i].deleteme) stergeVector(vectoriCreati[i].id);
                changemen(meniucrt/10);
                trebuie_selectateV = 0;
                nrVectoriSelectati = 0;
                for(int i = 0; i < nrVectori; i++)
                vectoriCreati[i].selected = false;
            }
        }
        else if(id == 11222) //adunare mat
        {
            if(editnew) {
                editnew = false;
                strcpy(text, "<Creeaza Matrice Noua>");
            }
            else {
                editnew = true;
                strcpy(text, "<Modifica Prima Matrice Selectata>");
            }
        }
        else if(id == 11232) //scadere mat
        {
            if(editnew) {
                editnew = false;
                strcpy(text, "<Creeaza Matrice Noua>");
            }
            else {
                editnew = true;
                strcpy(text, "<Modifica Prima Matrice Selectata>");
            }
        }
        else if(id == 11242) //inmultire mat
        {
            if(editnew) {
                editnew = false;
                strcpy(text, "<Creeaza Matrice Noua>");
            }
            else {
                editnew = true;
                strcpy(text, "<Modifica Prima Matrice Selectata>");
            }
        }
        else if(id == 11252) //impartire mat
        {
            if(editnew) {
                editnew = false;
                strcpy(text, "<Creeaza Matrice Noua>");
            }
            else {
                editnew = true;
                strcpy(text, "<Modifica Prima Matrice Selectata>");
            }
        }
        else if(id == 11262) //inmultire mxm
        {
            if(editnew) {
                editnew = false;
                strcpy(text, "<Creeaza Matrice Noua>");
            }
            else {
                editnew = true;
                strcpy(text, "<Modifica Prima Matrice Selectata>");
            }
        }
        else if(id == 11272) //impartire mxm
        {
            if(editnew) {
                editnew = false;
                strcpy(text, "<Creeaza Matrice Noua>");
            }
            else {
                editnew = true;
                strcpy(text, "<Modifica Prima Matrice Selectata>");
            }
        }
        else if(id == 11312) //transpusa m
        {
            if(editnew) {
                editnew = false;
                strcpy(text, "<Creeaza Matrice Noua>");
            }
            else {
                editnew = true;
                strcpy(text, "<Modifica Prima Matrice Selectata>");
            }
        }
        else if(id == 11332) //adjuncta m
        {
            if(editnew) {
                editnew = false;
                strcpy(text, "<Creeaza Matrice Noua>");
            }
            else {
                editnew = true;
                strcpy(text, "<Modifica Prima Matrice Selectata>");
            }
        }
        else if(id == 11342) //inversa m
        {
            if(editnew) {
                editnew = false;
                strcpy(text, "<Creeaza Matrice Noua>");
            }
            else {
                editnew = true;
                strcpy(text, "<Modifica Prima Matrice Selectata>");
            }
        }
        else if(id == 11352) //ridicare matrice la putere
        {
            if(editnew) {
                editnew = false;
                strcpy(text, "<Creeaza Matrice Noua>");
            }
            else {
                editnew = true;
                strcpy(text, "<Modifica Prima Matrice Selectata>");
            }
        }
        else if(id == 12222) // v+v
        {
            if(editnew) {
                editnew = false;
                strcpy(text, "<Creeaza Vector Nou>");
            }
            else {
                editnew = true;
                strcpy(text, "<Modifica Primul vector Selectat>");
            }
        }
        else if(id == 12232) // v-v
        {
            if(editnew) {
                editnew = false;
                strcpy(text, "<Creeaza Vector Nou>");
            }
            else {
                editnew = true;
                strcpy(text, "<Modifica Primul vector Selectat>");
            }
        }
        else if(id == 12242) // v*s
        {
            if(editnew) {
                editnew = false;
                strcpy(text, "<Creeaza Vector Nou>");
            }
            else {
                editnew = true;
                strcpy(text, "<Modifica Primul vector Selectat>");
            }
        }
        else if(id == 12252) // v/s
        {
            if(editnew) {
                editnew = false;
                strcpy(text, "<Creeaza Vector Nou>");
            }
            else {
                editnew = true;
                strcpy(text, "<Modifica Primul vector Selectat>");
            }
        }
        else if(id == 11211) //afisare mat
        {
            if(trebuie_selectate) {
                char txt[NRMAX];
                strcpy(txt,erori[2]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                matriciSelectate[0].afisareFisier();
                changemen(meniucrt/10);
                trebuie_selectate = 0;
                nrMatriciSelectate = 0;
                for(int i = 0; i < nrMatrici; i++)
                matriciCreate[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 11221) //adunare mat
        {
            bool ok = true;
            if(nrMatriciSelectate < 2) {
                char txt[NRMAX];
                strcpy(txt,erori[3]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                for(int i = 0; i < nrMatriciSelectate-1; i++)
                {
                    if(matriciSelectate[i].nrLinii != matriciSelectate[i+1].nrLinii || matriciSelectate[i].nrColoane != matriciSelectate[i+1].nrColoane)
                        ok = false;
                }
                if(!ok)
                {
                    trebuie_selectate = nrMatrici;
                    nrMatriciSelectate = 0;
                    for(int i = 0; i < nrMatrici; i++)
                    matriciCreate[i].selected = false;
                    setcurrentwindow(0);
                    setcolor(RED);
                    char txt[NRMAX];
                    strcpy(txt,erori[0]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                    strcpy(txt,erori[1]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - (int)(boxSize*1.5),txt);
                    setcolor(WHITE);
                    dontrefresh = true;
                }
                else {
                    int ID;
                    if(!editnew)
                    {
                        matrice m;
                        adaugaMatrice(m);
                        matriciCreate[nrMatrici-1].adunareMatrici(matriciSelectate[0],matriciSelectate[1]);
                    }
                    else
                    {
                        for(int i = 0; i < nrMatrici; i++)
                            if(matriciCreate[i].id == matriciSelectate[0].id)
                            {
                                ID = i;
                                break;
                            }
                        matriciCreate[ID].adunareMatrici(matriciSelectate[0],matriciSelectate[1]);
                    }
                    for(int i = 2; i < nrMatriciSelectate; i++)
                    {
                        if(!editnew)
                        {
                            matriciCreate[nrMatrici-1].adunareMatrici(matriciCreate[nrMatrici-1],matriciSelectate[i]);
                        }
                        else
                        {
                            matriciCreate[ID].adunareMatrici(matriciCreate[ID],matriciSelectate[i]);
                        }
                    }
                    if(ok){
                        changemen(meniucrt/10);
                        trebuie_selectate = 0;
                        nrMatriciSelectate = 0;
                        for(int i = 0; i < nrMatrici; i++)
                        matriciCreate[i].selected = false;
                        dontrefresh = false;
                    }
                    else {
                        dontrefresh = false;
                    }
                }
            }
        }
        else if(id == 11231) //scadere mat
        {
            bool ok = true;
            if(nrMatriciSelectate < 2) {
                char txt[NRMAX];
                strcpy(txt,erori[3]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                for(int i = 0; i < nrMatriciSelectate-1; i++)
                {
                    if(matriciSelectate[i].nrLinii != matriciSelectate[i+1].nrLinii || matriciSelectate[i].nrColoane != matriciSelectate[i+1].nrColoane)
                        ok = false;
                }
                if(!ok)
                {
                    trebuie_selectate = nrMatrici;
                    nrMatriciSelectate = 0;
                    for(int i = 0; i < nrMatrici; i++)
                    matriciCreate[i].selected = false;
                    setcurrentwindow(0);
                    setcolor(RED);
                    char txt[NRMAX];
                    strcpy(txt,erori[0]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                    strcpy(txt,erori[1]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - (int)(boxSize*1.5),txt);
                    setcolor(WHITE);
                    dontrefresh = true;
                }
                else {
                    int ID;
                    if(!editnew)
                    {
                        matrice m;
                        adaugaMatrice(m);
                        matriciCreate[nrMatrici-1].scadereMatrici(matriciSelectate[0],matriciSelectate[1]);
                    }
                    else
                    {
                        for(int i = 0; i < nrMatrici; i++)
                            if(matriciCreate[i].id == matriciSelectate[0].id)
                            {
                                ID = i;
                                break;
                            }
                        matriciCreate[ID].scadereMatrici(matriciSelectate[0],matriciSelectate[1]);
                    }
                    for(int i = 2; i < nrMatriciSelectate; i++)
                    {
                            if(!editnew)
                            {
                                matriciCreate[nrMatrici-1].scadereMatrici(matriciCreate[nrMatrici-1],matriciSelectate[i]);
                            }
                            else
                            {
                                matriciCreate[ID].scadereMatrici(matriciCreate[ID],matriciSelectate[i]);
                            }
                    }
                    if(ok){
                        changemen(meniucrt/10);
                        trebuie_selectate = 0;
                        nrMatriciSelectate = 0;
                        for(int i = 0; i < nrMatrici; i++)
                        matriciCreate[i].selected = false;
                        dontrefresh = false;
                    }
                    else {
                        dontrefresh = false;
                    }
                }
            }
        }
        else if(id == 11241) //inmultire scalar
        {
            if(trebuie_selectate)
            {
                char txt[NRMAX];
                strcpy(txt,erori[2]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                int ID;
                if(!editnew)
                {
                    matrice m;
                    adaugaMatrice(m);
                    matriciCreate[nrMatrici-1].produsMatriceScalar(matriciSelectate[0],scalar);
                }
                else
                {
                    for(int i = 0; i < nrMatrici; i++)
                        if(matriciCreate[i].id == matriciSelectate[0].id)
                        {
                            ID = i;
                            break;
                        }
                    matriciCreate[ID].produsMatriceScalar(matriciCreate[ID],scalar);
                }
                changemen(meniucrt/10);
                trebuie_selectate = 0;
                nrMatriciSelectate = 0;
                for(int i = 0; i < nrMatrici; i++)
                matriciCreate[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 11251) //impartire scalar
        {
            if(scalar == 0) changemen(1125);
            if(trebuie_selectate)
            {
                char txt[NRMAX];
                strcpy(txt,erori[2]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                int ID;
                if(!editnew)
                {
                    matrice m;
                    adaugaMatrice(m);
                    matriciCreate[nrMatrici-1].impartireMatriceScalar(matriciSelectate[0],scalar);
                }
                else
                {
                    for(int i = 0; i < nrMatrici; i++)
                        if(matriciCreate[i].id == matriciSelectate[0].id)
                        {
                            ID = i;
                            break;
                        }
                    matriciCreate[ID].impartireMatriceScalar(matriciCreate[ID],scalar);
                }
                changemen(meniucrt/10);
                trebuie_selectate = 0;
                nrMatriciSelectate = 0;
                for(int i = 0; i < nrMatrici; i++)
                matriciCreate[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 11261) //inmultire mxm
        {
            bool ok = true;
            if(nrMatriciSelectate < 2) {
                char txt[NRMAX];
                strcpy(txt,erori[3]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                for(int i = 0; i < nrMatriciSelectate-1; i++)
                {
                    if(matriciSelectate[i].nrColoane != matriciSelectate[i+1].nrLinii)
                        ok = false;
                }
                if(!ok)
                {
                    trebuie_selectate = nrMatrici;
                    nrMatriciSelectate = 0;
                    for(int i = 0; i < nrMatrici; i++)
                    matriciCreate[i].selected = false;
                    setcurrentwindow(0);
                    setcolor(RED);
                    char txt[NRMAX];
                    strcpy(txt,erori[4]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                    strcpy(txt,erori[5]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - (int)(boxSize*1.5),txt);
                    setcolor(WHITE);
                    dontrefresh = true;
                }
                else
                {
                    int ID;
                    if(!editnew)
                    {
                        matrice m;
                        adaugaMatrice(m);
                        matriciCreate[nrMatrici-1].inmultireMatrici(matriciSelectate[0],matriciSelectate[1]);
                    }
                    else
                    {
                        for(int i = 0; i < nrMatrici; i++)
                            if(matriciCreate[i].id == matriciSelectate[0].id)
                            {
                                ID = i;
                                break;
                            }
                        matriciCreate[ID].inmultireMatrici(matriciSelectate[0],matriciSelectate[1]);
                    }
                    for(int i = 2; i < nrMatriciSelectate; i++)
                    {
                        if(!editnew)
                        {
                            matriciCreate[nrMatrici-1].inmultireMatrici(matriciCreate[nrMatrici-1],matriciSelectate[i]);
                        }
                        else
                        {
                            matriciCreate[ID].inmultireMatrici(matriciCreate[ID],matriciSelectate[i]);
                        }
                    }
                    if(ok){
                        changemen(meniucrt/10);
                        trebuie_selectate = 0;
                        nrMatriciSelectate = 0;
                        for(int i = 0; i < nrMatrici; i++)
                        matriciCreate[i].selected = false;
                        dontrefresh = false;
                    }
                    else {
                        dontrefresh = false;
                    }
                }
            }
        }
        else if(id == 11271) //impartire mxm
        {
            bool ok = true;
            if(nrMatriciSelectate < 2) {
                char txt[NRMAX];
                //cout << "HERE" << nrMatriciSelectate << '\n';
                strcpy(txt,erori[3]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                for(int i = 0; i < nrMatriciSelectate-1; i++)
                {
                    if(matriciSelectate[i].nrColoane != matriciSelectate[i].nrLinii || matriciSelectate[i].nrLinii != matriciSelectate[i+1].nrColoane || matriciSelectate[i+1].nrLinii != matriciSelectate[i+1].nrColoane)
                        ok = false;
                }
                if(!ok)
                {
                    trebuie_selectate = nrMatrici;
                    nrMatriciSelectate = 0;
                    for(int i = 0; i < nrMatrici; i++)
                    matriciCreate[i].selected = false;
                    setcurrentwindow(0);
                    setcolor(RED);
                    char txt[NRMAX];
                    strcpy(txt,erori[8]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                    strcpy(txt,erori[9]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - (int)(boxSize*1.5),txt);
                    setcolor(WHITE);
                    dontrefresh = true;
                }
                else
                {
                    int ID;
                    if(!editnew)
                    {
                        matrice m;
                        adaugaMatrice(m);
                        matriciCreate[nrMatrici-1].impartireMatrici(matriciSelectate[0],matriciSelectate[1]);
                    }
                    else
                    {
                        for(int i = 0; i < nrMatrici; i++)
                            if(matriciCreate[i].id == matriciSelectate[0].id)
                            {
                                ID = i;
                                break;
                            }
                        matriciCreate[ID].impartireMatrici(matriciSelectate[0],matriciSelectate[1]);
                    }
                    for(int i = 2; i < nrMatriciSelectate; i++)
                    {
                        if(!editnew)
                        {
                            matriciCreate[nrMatrici-1].impartireMatrici(matriciCreate[nrMatrici-1],matriciSelectate[i]);
                        }
                        else
                        {
                            matriciCreate[ID].impartireMatrici(matriciCreate[ID],matriciSelectate[i]);
                        }
                    }
                    if(ok){
                        changemen(meniucrt/10);
                        trebuie_selectate = 0;
                        nrMatriciSelectate = 0;
                        for(int i = 0; i < nrMatrici; i++)
                        matriciCreate[i].selected = false;
                        dontrefresh = false;
                    }
                    else {
                        dontrefresh = false;
                    }
                }
            }
        }
        else if(id == 11311) //transpusa
        {
            if(nrMatriciSelectate != 1)
            {
                char txt[NRMAX];
                strcpy(txt,erori[2]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                int ID;
                if(!editnew)
                {
                    matrice m;
                    adaugaMatrice(m);
                    matriciCreate[nrMatrici-1].transpusaMatrice(matriciSelectate[0]);
                }
                else
                {
                    for(int i = 0; i < nrMatrici; i++)
                        if(matriciCreate[i].id == matriciSelectate[0].id)
                        {
                            ID = i;
                            break;
                        }
                    matriciCreate[ID].transpusaMatrice(matriciCreate[ID]);
                }
                changemen(meniucrt/10);
                trebuie_selectate = 0;
                nrMatriciSelectate = 0;
                for(int i = 0; i < nrMatrici; i++)
                matriciCreate[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 11321) //determinant
        {
            if(nrMatriciSelectate != 1)
            {
                char txt[NRMAX];
                strcpy(txt,erori[2]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                if(matriciSelectate[0].nrLinii != matriciSelectate[0].nrColoane)
                {
                    setcurrentwindow(0);
                    setcolor(RED);
                    char txt[NRMAX];
                    strcpy(txt,erori[6]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                    strcpy(txt,erori[7]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - (int)(boxSize*1.5),txt);
                    setcolor(WHITE);
                    dontrefresh = true;
                }
                else
                {
                    TIP det = 0;
                    det = matriciSelectate[0].determinantMatrice();
                    initwindow(width,100,"Determinant",spatiustd,height - 200 + spatiustd,false,false);
                    char txt[NRMAX];
                    strcpy(txt,"Determinantul este egal cu ");
                    strcat(txt,intToString(det));
                    strcat(txt, ".");
                    outtextxy(25,25,txt);
                    char tasta;
                    do
                    {
                        tasta = getch();
                    }
                    while(tasta!=13 && tasta!=27);
                    setcurrentwindow(0);
                    closegraph(inputWinId++);
                    changemen(meniucrt/10);
                    trebuie_selectate = 0;
                    nrMatriciSelectate = 0;
                    for(int i = 0; i < nrMatrici; i++)
                    matriciCreate[i].selected = false;
                    dontrefresh = false;
                }
            }
        }
        else if(id == 11331) //adjuncta
        {
            if(nrMatriciSelectate != 1)
            {
                char txt[NRMAX];
                strcpy(txt,erori[2]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                if(matriciSelectate[0].nrColoane != matriciSelectate[0].nrLinii)
                {
                    setcurrentwindow(0);
                    setcolor(RED);
                    char txt[NRMAX];
                    strcpy(txt,erori[10]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                    strcpy(txt,erori[11]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - (int)(boxSize*1.5),txt);
                    setcolor(WHITE);
                    dontrefresh = true;
                }
                else
                {
                    int ID;
                    if(!editnew)
                    {
                        matrice m;
                        adaugaMatrice(m);
                        matriciCreate[nrMatrici-1].adjunctaMatrice(matriciSelectate[0]);
                    }
                    else
                    {
                        for(int i = 0; i < nrMatrici; i++)
                            if(matriciCreate[i].id == matriciSelectate[0].id)
                            {
                                ID = i;
                                break;
                            }
                        matriciCreate[ID].adjunctaMatrice(matriciCreate[ID]);
                    }
                    changemen(meniucrt/10);
                    trebuie_selectate = 0;
                    nrMatriciSelectate = 0;
                    for(int i = 0; i < nrMatrici; i++)
                    matriciCreate[i].selected = false;
                    dontrefresh = false;
                }
            }
        }
        else if(id == 11341) //inversa
        {
            if(nrMatriciSelectate != 1)
            {
                char txt[NRMAX];
                strcpy(txt,erori[2]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                if(matriciSelectate[0].nrColoane != matriciSelectate[0].nrLinii)
                {
                    setcurrentwindow(0);
                    setcolor(RED);
                    char txt[NRMAX];
                    strcpy(txt,erori[12]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                    strcpy(txt,erori[13]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - (int)(boxSize*1.5),txt);
                    setcolor(WHITE);
                    dontrefresh = true;
                }
                else
                {
                    int ID;
                    if(!editnew)
                    {
                        matrice m;
                        adaugaMatrice(m);
                        matriciCreate[nrMatrici-1].inversaMatrice(matriciSelectate[0]);
                    }
                    else
                    {
                        for(int i = 0; i < nrMatrici; i++)
                            if(matriciCreate[i].id == matriciSelectate[0].id)
                            {
                                ID = i;
                                break;
                            }
                        matriciCreate[ID].inversaMatrice(matriciCreate[ID]);
                    }
                    changemen(meniucrt/10);
                    trebuie_selectate = 0;
                    nrMatriciSelectate = 0;
                    for(int i = 0; i < nrMatrici; i++)
                    matriciCreate[i].selected = false;
                    dontrefresh = false;
                }
            }
        }
        else if(id == 11351) //ridicare matrice la putere
        {
            if(trebuie_selectate)
            {
                char txt[NRMAX];
                strcpy(txt,erori[2]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
                return;
            }
            else if(matriciSelectate[0].nrLinii != matriciSelectate[0].nrColoane)
            {
                trebuie_selectate = 1;
                nrMatriciSelectate = 0;
                for(int i = 0; i < nrMatrici; i++)
                matriciCreate[i].selected = false;
                setcurrentwindow(0);
                setcolor(RED);
                char txt[NRMAX];
                strcpy(txt,erori[4]);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                strcpy(txt,erori[5]);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - (int)(boxSize*1.5),txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                int ID;
                if(!editnew)
                {
                    matrice m;
                    adaugaMatrice(m);
                    matriciCreate[nrMatrici-1].ridicaLaPutereMatrice(matriciSelectate[0],scalar);
                }
                else
                {
                    for(int i = 0; i < nrMatrici; i++)
                        if(matriciCreate[i].id == matriciSelectate[0].id)
                        {
                            ID = i;
                            break;
                        }
                    matriciCreate[ID].ridicaLaPutereMatrice(matriciCreate[ID],scalar);
                }
                changemen(meniucrt/10);
                trebuie_selectate = 0;
                nrMatriciSelectate = 0;
                for(int i = 0; i < nrMatrici; i++)
                matriciCreate[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 11411) // numar 0
        {
            TIP nrzero = 0;
            nrzero = matriciSelectate[0].numarElementeZero();
            initwindow(width,100,"Numar 0",spatiustd,height - 200 + spatiustd,false,false);
            char txt[NRMAX];
            strcpy(txt,"Matricea are ");
            strcat(txt,intToString(nrzero));
            strcat(txt," zerouri.");
            outtextxy(25,25,txt);
            char tasta;
            do
            {
                tasta = getch();
            }
            while(tasta!=13 && tasta!=27);
            setcurrentwindow(0);
            closegraph(inputWinId++);
            changemen(meniucrt/10);
            trebuie_selectate = 0;
            nrMatriciSelectate = 0;
            for(int i = 0; i < nrMatrici; i++)
            matriciCreate[i].selected = false;
            dontrefresh = false;
        }
        else if(id == 11421) // densitate
        {
            TIP d = 0;
            d = matriciSelectate[0].densitateMatrice();
            initwindow(width,100,"Densitate",spatiustd,height - 200 + spatiustd,false,false);
            char txt[NRMAX];
            strcpy(txt,"Densitatea matricii este egala cu ");
            strcat(txt,intToString(d));
            strcat(txt,"%.");
            outtextxy(25,25,txt);
            char tasta;
            do
            {
                tasta = getch();
            }
            while(tasta!=13 && tasta!=27);
            setcurrentwindow(0);
            closegraph(inputWinId++);
            changemen(meniucrt/10);
            trebuie_selectate = 0;
            nrMatriciSelectate = 0;
            for(int i = 0; i < nrMatrici; i++)
            matriciCreate[i].selected = false;
            dontrefresh = false;
        }
        else if(id == 11431) // este rara?
        {
            bool b = 0;
            b = matriciSelectate[0].esteRara();
            initwindow(width,100,"Numar 0",spatiustd,height - 200 + spatiustd,false,false);
            char txt[NRMAX];
            if(b) strcpy(txt, "Matricea este rara.");
            else strcpy(txt, "Matricea nu este rara.");
            outtextxy(25,25,txt);
            char tasta;
            do
            {
                tasta = getch();
            }
            while(tasta!=13 && tasta!=27);
            setcurrentwindow(0);
            closegraph(inputWinId++);
            changemen(meniucrt/10);
            trebuie_selectate = 0;
            nrMatriciSelectate = 0;
            for(int i = 0; i < nrMatrici; i++)
            matriciCreate[i].selected = false;
            dontrefresh = false;
        }
        else if(id == 11611) //suma dp
        {
            if(nrMatriciSelectate != 1)
            {
                char txt[NRMAX];
                strcpy(txt,erori[2]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                if(matriciSelectate[0].nrLinii != matriciSelectate[0].nrColoane)
                {
                    setcurrentwindow(0);
                    setcolor(RED);
                    char txt[NRMAX];
                    strcpy(txt,erori[18]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                    setcolor(WHITE);
                    dontrefresh = true;
                }
                else
                {
                    TIP suma = 0;
                    suma = matriciSelectate[0].sumaDP();
                    initwindow(width,100,"Suma",spatiustd,height - 200 + spatiustd,false,false);
                    char txt[NRMAX];
                    strcpy(txt,"Suma elementelor de pe diagonala principala");
                    outtextxy(10,25,txt);
                    strcpy(txt,"este ");
                    strcat(txt,intToString(suma));
                    outtextxy(width/2,50,txt);
                    char tasta;
                    do
                    {
                        tasta = getch();
                    }
                    while(tasta!=13 && tasta!=27);
                    setcurrentwindow(0);
                    closegraph(inputWinId++);
                    changemen(meniucrt/10);
                    trebuie_selectate = 0;
                    nrMatriciSelectate = 0;
                    for(int i = 0; i < nrMatrici; i++)
                    matriciCreate[i].selected = false;
                    dontrefresh = false;
                }
            }
        }
        else if(id == 11621) //suma sub dp
        {
            if(nrMatriciSelectate != 1)
            {
                char txt[NRMAX];
                strcpy(txt,erori[2]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                if(matriciSelectate[0].nrLinii != matriciSelectate[0].nrColoane)
                {
                    setcurrentwindow(0);
                    setcolor(RED);
                    char txt[NRMAX];
                    strcpy(txt,erori[18]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                    setcolor(WHITE);
                    dontrefresh = true;
                }
                else
                {
                    TIP suma = 0;
                    suma = matriciSelectate[0].sumaSubDP();
                    initwindow(width,100,"Suma",spatiustd,height - 200 + spatiustd,false,false);
                    char txt[NRMAX];
                    strcpy(txt,"Suma elementelor de sub diagonala principala");
                    outtextxy(10,25,txt);
                    strcpy(txt,"este ");
                    strcat(txt,intToString(suma));
                    outtextxy(width/2,50,txt);
                    char tasta;
                    do
                    {
                        tasta = getch();
                    }
                    while(tasta!=13 && tasta!=27);
                    setcurrentwindow(0);
                    closegraph(inputWinId++);
                    changemen(meniucrt/10);
                    trebuie_selectate = 0;
                    nrMatriciSelectate = 0;
                    for(int i = 0; i < nrMatrici; i++)
                    matriciCreate[i].selected = false;
                    dontrefresh = false;
                }
            }
        }
        else if(id == 11631) //suma deasupra dp
        {
            if(nrMatriciSelectate != 1)
            {
                char txt[NRMAX];
                strcpy(txt,erori[2]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                if(matriciSelectate[0].nrLinii != matriciSelectate[0].nrColoane)
                {
                    setcurrentwindow(0);
                    setcolor(RED);
                    char txt[NRMAX];
                    strcpy(txt,erori[18]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                    setcolor(WHITE);
                    dontrefresh = true;
                }
                else
                {
                    TIP suma = 0;
                    suma = matriciSelectate[0].sumaDeasupraDP();
                    initwindow(width,100,"Suma",spatiustd,height - 200 + spatiustd,false,false);
                    char txt[NRMAX];
                    strcpy(txt,"Suma elementelor de deasupra diagonalei principale");
                    outtextxy(5,25,txt);
                    strcpy(txt,"este ");
                    strcat(txt,intToString(suma));
                    outtextxy(width/2,50,txt);
                    char tasta;
                    do
                    {
                        tasta = getch();
                    }
                    while(tasta!=13 && tasta!=27);
                    setcurrentwindow(0);
                    closegraph(inputWinId++);
                    changemen(meniucrt/10);
                    trebuie_selectate = 0;
                    nrMatriciSelectate = 0;
                    for(int i = 0; i < nrMatrici; i++)
                    matriciCreate[i].selected = false;
                    dontrefresh = false;
                }
            }
        }
        else if(id == 11641) //suma ds
        {
            if(nrMatriciSelectate != 1)
            {
                char txt[NRMAX];
                strcpy(txt,erori[2]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                if(matriciSelectate[0].nrLinii != matriciSelectate[0].nrColoane)
                {
                    setcurrentwindow(0);
                    setcolor(RED);
                    char txt[NRMAX];
                    strcpy(txt,erori[18]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                    setcolor(WHITE);
                    dontrefresh = true;
                }
                else
                {
                    TIP suma = 0;
                    suma = matriciSelectate[0].sumaDS();
                    initwindow(width,100,"Suma",spatiustd,height - 200 + spatiustd,false,false);
                    char txt[NRMAX];
                    strcpy(txt,"Suma elementelor de pe diagonala secundara");
                    outtextxy(10,25,txt);
                    strcpy(txt,"este ");
                    strcat(txt,intToString(suma));
                    outtextxy(width/2,50,txt);
                    char tasta;
                    do
                    {
                        tasta = getch();
                    }
                    while(tasta!=13 && tasta!=27);
                    setcurrentwindow(0);
                    closegraph(inputWinId++);
                    changemen(meniucrt/10);
                    trebuie_selectate = 0;
                    nrMatriciSelectate = 0;
                    for(int i = 0; i < nrMatrici; i++)
                    matriciCreate[i].selected = false;
                    dontrefresh = false;
                }
            }
        }
        else if(id == 11651) //suma sub ds
        {
            if(nrMatriciSelectate != 1)
            {
                char txt[NRMAX];
                strcpy(txt,erori[2]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                if(matriciSelectate[0].nrLinii != matriciSelectate[0].nrColoane)
                {
                    setcurrentwindow(0);
                    setcolor(RED);
                    char txt[NRMAX];
                    strcpy(txt,erori[18]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                    setcolor(WHITE);
                    dontrefresh = true;
                }
                else
                {
                    TIP suma = 0;
                    suma = matriciSelectate[0].sumaSubDS();
                    initwindow(width,100,"Suma",spatiustd,height - 200 + spatiustd,false,false);
                    char txt[NRMAX];
                    strcpy(txt,"Suma elementelor de sub diagonala secundara");
                    outtextxy(10,25,txt);
                    strcpy(txt,"este ");
                    strcat(txt,intToString(suma));
                    outtextxy(width/2,50,txt);
                    char tasta;
                    do
                    {
                        tasta = getch();
                    }
                    while(tasta!=13 && tasta!=27);
                    setcurrentwindow(0);
                    closegraph(inputWinId++);
                    changemen(meniucrt/10);
                    trebuie_selectate = 0;
                    nrMatriciSelectate = 0;
                    for(int i = 0; i < nrMatrici; i++)
                    matriciCreate[i].selected = false;
                    dontrefresh = false;
                }
            }
        }
        else if(id == 11661) //suma deasupra ds
        {
            if(nrMatriciSelectate != 1)
            {
                char txt[NRMAX];
                strcpy(txt,erori[2]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                if(matriciSelectate[0].nrLinii != matriciSelectate[0].nrColoane)
                {
                    setcurrentwindow(0);
                    setcolor(RED);
                    char txt[NRMAX];
                    strcpy(txt,erori[18]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                    setcolor(WHITE);
                    dontrefresh = true;
                }
                else
                {
                    TIP suma = 0;
                    suma = matriciSelectate[0].sumaDeasupraDS();
                    initwindow(width,100,"Suma",spatiustd,height - 200 + spatiustd,false,false);
                    char txt[NRMAX];
                    strcpy(txt,"Suma elementelor de deasupra diagonalei secundare");
                    outtextxy(5,25,txt);
                    strcpy(txt,"este ");
                    strcat(txt,intToString(suma));
                    outtextxy(width/2,50,txt);
                    char tasta;
                    do
                    {
                        tasta = getch();
                    }
                    while(tasta!=13 && tasta!=27);
                    setcurrentwindow(0);
                    closegraph(inputWinId++);
                    changemen(meniucrt/10);
                    trebuie_selectate = 0;
                    nrMatriciSelectate = 0;
                    for(int i = 0; i < nrMatrici; i++)
                    matriciCreate[i].selected = false;
                    dontrefresh = false;
                }
            }
        }
        else if(id == 12211) // afisare vect
        {
            if(trebuie_selectateV) {
                char txt[NRMAX];
                strcpy(txt,erori[16]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                vectoriSelectati[0].afisareFisier();
                changemen(meniucrt/10);
                trebuie_selectateV = 0;
                nrVectoriSelectati = 0;
                for(int i = 0; i < nrVectori; i++)
                vectoriCreati[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 12221) //adunare vect
        {
            bool ok = true;
            if(nrVectoriSelectati < 2) {
                char txt[NRMAX];
                strcpy(txt,erori[17]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                for(int i = 0; i < nrVectoriSelectati-1; i++)
                {
                    if(vectoriSelectati[i].nrElemente != vectoriSelectati[i+1].nrElemente)
                        ok = false;
                }
                if(!ok)
                {
                    trebuie_selectateV = nrVectori;
                    nrVectoriSelectati = 0;
                    for(int i = 0; i < nrVectori; i++)
                        vectoriCreati[i].selected = false;
                    setcurrentwindow(0);
                    setcolor(RED);
                    char txt[NRMAX];
                    strcpy(txt,erori[14]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                    strcpy(txt,erori[15]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - (int)(boxSize*1.5),txt);
                    setcolor(WHITE);
                    dontrefresh = true;
                }
                else {
                    int ID;
                    if(!editnew)
                    {
                        vect v;
                        adaugaVector(v);
                        vectoriCreati[nrVectori-1].adunareVectori(vectoriSelectati[0],vectoriSelectati[1]);
                    }
                    else
                    {
                        for(int i = 0; i < nrVectori; i++)
                            if(vectoriCreati[i].id == vectoriSelectati[0].id)
                            {
                                ID = i;
                                break;
                            }
                        vectoriCreati[ID].adunareVectori(vectoriSelectati[0],vectoriSelectati[1]);
                    }
                    for(int i = 2; i < nrVectoriSelectati; i++)
                    {
                        if(!editnew)
                        {
                            vectoriCreati[nrVectori-1].adunareVectori(vectoriCreati[nrVectori-1],vectoriSelectati[i]);
                        }
                        else
                        {
                            vectoriCreati[ID].adunareVectori(vectoriCreati[ID],vectoriSelectati[i]);
                        }
                    }
                    if(ok){
                        changemen(meniucrt/10);
                        trebuie_selectateV = 0;
                        nrVectoriSelectati = 0;
                        for(int i = 0; i < nrVectori; i++)
                        vectoriCreati[i].selected = false;
                        dontrefresh = false;
                    }
                    else {
                        dontrefresh = false;
                    }
                }
            }
        }
        else if(id == 12231) //scadere vect
        {
            bool ok = true;
            if(nrVectoriSelectati < 2) {
                char txt[NRMAX];
                strcpy(txt,erori[17]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                for(int i = 0; i < nrVectoriSelectati-1; i++)
                {
                    if(vectoriSelectati[i].nrElemente != vectoriSelectati[i+1].nrElemente)
                        ok = false;
                }
                if(!ok)
                {
                    trebuie_selectateV = nrVectori;
                    nrVectoriSelectati = 0;
                    for(int i = 0; i < nrVectori; i++)
                        vectoriCreati[i].selected = false;
                    setcurrentwindow(0);
                    setcolor(RED);
                    char txt[NRMAX];
                    strcpy(txt,erori[14]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                    strcpy(txt,erori[15]);
                    outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - (int)(boxSize*1.5),txt);
                    setcolor(WHITE);
                    dontrefresh = true;
                }
                else {
                    int ID;
                    if(!editnew)
                    {
                        vect v;
                        adaugaVector(v);
                        vectoriCreati[nrVectori-1].scadereVectori(vectoriSelectati[0],vectoriSelectati[1]);
                    }
                    else
                    {
                        for(int i = 0; i < nrVectori; i++)
                            if(vectoriCreati[i].id == vectoriSelectati[0].id)
                            {
                                ID = i;
                                break;
                            }
                        vectoriCreati[ID].scadereVectori(vectoriSelectati[0],vectoriSelectati[1]);
                    }
                    for(int i = 2; i < nrVectoriSelectati; i++)
                    {
                        if(!editnew)
                        {
                            vectoriCreati[nrVectori-1].scadereVectori(vectoriCreati[nrVectori-1],vectoriSelectati[i]);
                        }
                        else
                        {
                            vectoriCreati[ID].scadereVectori(vectoriCreati[ID],vectoriSelectati[i]);
                        }
                    }
                    if(ok){
                        changemen(meniucrt/10);
                        trebuie_selectateV = 0;
                        nrVectoriSelectati = 0;
                        for(int i = 0; i < nrVectori; i++)
                        vectoriCreati[i].selected = false;
                        dontrefresh = false;
                    }
                    else {
                        dontrefresh = false;
                    }
                }
            }
        }
        else if(id == 12241) //inmultire vect cu scalar
        {
            bool ok = true;
            if(trebuie_selectateV) {
                char txt[NRMAX];
                strcpy(txt,erori[16]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                int ID;
                if(!editnew)
                {
                    vect v;
                    adaugaVector(v);
                    vectoriCreati[nrVectori-1].produsVectorScalar(vectoriSelectati[0],scalar);
                }
                else
                {
                    for(int i = 0; i < nrVectori; i++)
                        if(vectoriCreati[i].id == vectoriSelectati[0].id)
                        {
                            ID = i;
                            break;
                        }
                    vectoriCreati[ID].produsVectorScalar(vectoriSelectati[0],scalar);
                }
                changemen(meniucrt/10);
                trebuie_selectateV = 0;
                nrVectoriSelectati = 0;
                for(int i = 0; i < nrVectori; i++)
                vectoriCreati[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 12251) //impartire vect cu scalar
        {
            bool ok = true;
            if(trebuie_selectateV) {
                char txt[NRMAX];
                strcpy(txt,erori[16]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else
            {
                int ID;
                if(!editnew)
                {
                    vect v;
                    adaugaVector(v);
                    vectoriCreati[nrVectori-1].impartireVectorScalar(vectoriSelectati[0],scalar);
                }
                else
                {
                    for(int i = 0; i < nrVectori; i++)
                        if(vectoriCreati[i].id == vectoriSelectati[0].id)
                        {
                            ID = i;
                            break;
                        }
                    vectoriCreati[ID].impartireVectorScalar(vectoriSelectati[0],scalar);
                }
                changemen(meniucrt/10);
                trebuie_selectateV = 0;
                nrVectoriSelectati = 0;
                for(int i = 0; i < nrVectori; i++)
                vectoriCreati[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 12261) //suma el
        {
            if(trebuie_selectateV) {
                char txt[NRMAX];
                strcpy(txt,erori[16]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                vectoriSelectati[0].sumaElemente();
                changemen(meniucrt/10);
                trebuie_selectateV = 0;
                nrVectoriSelectati = 0;
                for(int i = 0; i < nrVectori; i++)
                vectoriCreati[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 12271) // produs el
        {
            if(trebuie_selectateV) {
                char txt[NRMAX];
                strcpy(txt,erori[16]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                vectoriSelectati[0].produsElemente();
                changemen(meniucrt/10);
                trebuie_selectateV = 0;
                nrVectoriSelectati = 0;
                for(int i = 0; i < nrVectori; i++)
                vectoriCreati[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 12281) // insert el
        {
            if(trebuie_selectateV) {
                char txt[NRMAX];
                strcpy(txt,erori[16]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                TIP k, x;
                int kINT;
                initwindow(width,100,"Element",spatiustd,height - 200 + spatiustd,false,false);
                setcurrentwindow(inputWinId);
                citesteInModGraficNRspecific(scalar,25,25,RED,1228,x);
                initwindow(width,100,"Pozitie",spatiustd,height - 200 + spatiustd,false,false);
                setcurrentwindow(inputWinId);
                citesteInModGraficNRspecific(scalar,25,25,RED,1228,k);
                kINT = (int)(k);
                vectoriSelectati[0].insertElement(x,kINT);
                changemen(meniucrt/10);
                trebuie_selectateV = 0;
                nrVectoriSelectati = 0;
                for(int i = 0; i < nrVectori; i++)
                vectoriCreati[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 12411) //bubble cr
        {
            if(trebuie_selectateV) {
                char txt[NRMAX];
                strcpy(txt,erori[16]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                vectoriSelectati[0].bubble('C');
                changemen(meniucrt/10);
                trebuie_selectateV = 0;
                nrVectoriSelectati = 0;
                for(int i = 0; i < nrVectori; i++)
                vectoriCreati[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 12421) //bubble des
        {
            if(trebuie_selectateV) {
                char txt[NRMAX];
                strcpy(txt,erori[16]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                vectoriSelectati[0].bubble('D');
                changemen(meniucrt/10);
                trebuie_selectateV = 0;
                nrVectoriSelectati = 0;
                for(int i = 0; i < nrVectori; i++)
                vectoriCreati[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 12431) //insertie cr
        {
            if(trebuie_selectateV) {
                char txt[NRMAX];
                strcpy(txt,erori[16]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                vectoriSelectati[0].insertie('C');
                changemen(meniucrt/10);
                trebuie_selectateV = 0;
                nrVectoriSelectati = 0;
                for(int i = 0; i < nrVectori; i++)
                vectoriCreati[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 12441) //insertie des
        {
            if(trebuie_selectateV) {
                char txt[NRMAX];
                strcpy(txt,erori[16]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                vectoriSelectati[0].insertie('D');
                changemen(meniucrt/10);
                trebuie_selectateV = 0;
                nrVectoriSelectati = 0;
                for(int i = 0; i < nrVectori; i++)
                vectoriCreati[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 12451) //interschimbare cr
        {
            if(trebuie_selectateV) {
                char txt[NRMAX];
                strcpy(txt,erori[16]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                vectoriSelectati[0].interschimbare('C');
                changemen(meniucrt/10);
                trebuie_selectateV = 0;
                nrVectoriSelectati = 0;
                for(int i = 0; i < nrVectori; i++)
                vectoriCreati[i].selected = false;
                dontrefresh = false;
            }
        }
        else if(id == 12461) //interschimbare des
        {
            if(trebuie_selectateV) {
                char txt[NRMAX];
                strcpy(txt,erori[16]);
                setcurrentwindow(0);
                setcolor(RED);
                outtextxy(width/2 - (int)(3.14*strlen(txt)),height - 200 - boxSize*2,txt);
                setcolor(WHITE);
                dontrefresh = true;
            }
            else {
                vectoriSelectati[0].interschimbare('D');
                changemen(meniucrt/10);
                trebuie_selectateV = 0;
                nrVectoriSelectati = 0;
                for(int i = 0; i < nrVectori; i++)
                vectoriCreati[i].selected = false;
                dontrefresh = false;
            }
        }
    }

    void show() // afiseaza butonul
    {
        if(id == 11232 || id == 11222 || id == 11242 || id == 11252 || id == 11262 || id == 11272 || id == 11312 || id == 11342 || id == 11332 || id == 11352)
        {
            if(!editnew) {
                strcpy(text, "<Creeaza Matrice Noua>");
            }
            else {
                strcpy(text, "<Modifica Prima Matrice Selectata>");
            }
        }
        if(id == 12222 || id == 12232 || id == 12242 || id == 12252)
        {
            if(!editnew) {
                strcpy(text, "<Creeaza Vector Nou>");
            }
            else {
                strcpy(text, "<Modifica Primul Vector Selectat>");
            }
        }
        square(x1,y1 + y_Offset,x2,y2 + y_Offset);
        outtextxy((x2+x1)/2-(int)(3.14*strlen(text)),(y2+y1)/2 - 8 + y_Offset,text);
    }
};

button butoane[NRMAX*10];

void adaugaButon(button b); // adauga un buton creat local in butoane[]

void adaugaButonARG(int x1, int y1, int x2, int y2, char t[NRMAX], int k); // adauga un buton in butoane[] cu toate argumentele necesare

void adaugaButonARGauto(char t[NRMAX], int k); // adauga un buton in butoane[], si il plaseaza sub ultimul buton creat

void whatPress(int a,int b,int men,int cw); // verifica pe ce am apasat

void initButoane(); // creeaza toate butoanele

void scroll(int x, int y, int xx, int yy, int w); // decaleaza imaginea din fereastra w


int main()
{
    srand(time(NULL));

    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    screenH = desktop.bottom;
    screenW = desktop.right;

    width = (screenW - (spatiustd * 2) ) / 5;
    height = screenH - (spatiustd * 2);

    initErori();
    initButoane();
    initwindow(width, height - 200, "Matrici si vectori", spatiustd, spatiustd); // 0
    initwindow(width * 2, height, "Matrici", width + spatiustd, spatiustd); // 1
    initwindow(width * 2, height, "Vectori", width*3 + spatiustd, spatiustd); // 2
    while(meniucrt)
    {
        resizeBox();
        afisareMeniu(meniucrt);
        afisareMatrici();
        afisareVectori();
        bool pressed = false;
        int crtwindow = -1;
        while(!pressed)
        {
            setcurrentwindow(winds);
            if(ismouseclick(WM_LBUTTONUP))
            {
                pressed = true;
                crtwindow = getcurrentwindow();
            }
            setcurrentwindow(winds+1);
            if(ismouseclick(WM_LBUTTONUP))
            {
                pressed = true;
                crtwindow = getcurrentwindow();
            }
            setcurrentwindow(winds+2);
            if(ismouseclick(WM_LBUTTONUP))
            {
                pressed = true;
                crtwindow = getcurrentwindow();
            }
        }
        setcurrentwindow(crtwindow);
        int a,b,c,d;
        getmouseclick(WM_LBUTTONUP,c,d);
        getmouseclick(WM_LBUTTONDOWN,a,b);
        scroll(a,b,c,d,crtwindow);
        whatPress(c,d,meniucrt,crtwindow);
        //cout << a << ' ' << b << ' ' << c << ' ' << d << ' ' << crtwindow%2 << '\n';
        //closegraph();
        //winds+=2;
        if(!dontrefresh) cleardevice();
    }
    return 0;
}

void initErori()
{
    strcpy(erori[0],"Matricile trebuie sa aiba acelasi\n");
    strcpy(erori[1],"numar de linii si de coloane.\n");

    strcpy(erori[2],"Trebuie sa selectezi o matrice.\n");

    strcpy(erori[3],"Trebuie sa selectezi macar 2 matrici.\n");

    strcpy(erori[4],"Numarul de coloane al primului factor trebuie\n");
    strcpy(erori[5],"sa fie egal cu numarul de linii al urmatorului.\n");

    strcpy(erori[6],"Se poate calcula doar\n");
    strcpy(erori[7],"determinantul unei matrici patratice.\n");

    strcpy(erori[8],"Se pot imparti doar matrici patratice ");
    strcpy(erori[9],"de aceeasi dimensiune.\n");

    strcpy(erori[10],"Se poate calcula doar\n");
    strcpy(erori[11],"adjuncta unei matrici patratice.\n");

    strcpy(erori[12],"Se poate calcula doar\n");
    strcpy(erori[13],"adjuncta unei matrici patratice.\n");

    strcpy(erori[14],"Vectorii trebuie sa aiba\n");
    strcpy(erori[15],"acelasi numar de elemente.\n");

    strcpy(erori[16],"Trebuie sa selectezi un vector.\n");

    strcpy(erori[17],"Trebuie sa selectezi macar 2 vectori.\n");

    strcpy(erori[18],"Doar matricile patratice au diagonale.\n");
}

void initButoane()
{
    //meniu 1, principal
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Meniu Matrici",11);
    adaugaButonARGauto("Meniu Vectori",12);
    adaugaButonARGauto("Iesi din program",10);

    //meniu 1, 1 matrici principal
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Creeaza o matrice",111);
    adaugaButonARGauto("Operatii cu matrici",112);
    adaugaButonARGauto("Matrici rare",114);
    adaugaButonARGauto("Sterge Matrici",115);
    adaugaButonARGauto("Inapoi",110);

    //meniu 1, 2, vectori princpal
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Creeaza un vector",121);
    adaugaButonARGauto("Operatii cu vectori",122);
    adaugaButonARGauto("Sterge Vectori",123);
    adaugaButonARGauto("Sorteaza Vector",124);
    adaugaButonARGauto("Inapoi",120);

    //meniu 1, 1, 1 matrici creat meniu
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Citeste matrice din fisier",1111);
    adaugaButonARGauto("Citeste matrice de la tastatura",1112);
    adaugaButonARGauto("Creeaza matrice generata aleator",1113);
    adaugaButonARGauto("Inapoi",1110);

    //meniu 1, 1, 2 matrici operatii meniu
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Afiseaza o matrice in fisier",1121);
    adaugaButonARGauto("Adunare intre matrici",1122);
    adaugaButonARGauto("Scadere intre matrici",1123);
    adaugaButonARGauto("Inmulteste o matrice cu un scalar",1124);
    adaugaButonARGauto("Imparte o matrice la un scalar",1125);
    adaugaButonARGauto("Inmultire intre matrici",1126);
    adaugaButonARGauto("Impartire intre matrici",1127);
    adaugaButonARGauto("A doua pagina de operatii",1128);
    adaugaButonARGauto("Inapoi",1120);

    //meniu 1, 1, 3 matrici operatii meniu 2
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Transpusa unei matrici",1131);
    adaugaButonARGauto("Determinantul unei matrici",1132);
    adaugaButonARGauto("Adjuncta unei matrici",1133);
    adaugaButonARGauto("Inversa unei matrici",1134);
    adaugaButonARGauto("Ridica o matrice la putere",1135);
    adaugaButonARGauto("Prima pagina de operatii",1136);
    adaugaButonARGauto("A treia pagina de operatii",1137);
    adaugaButonARGauto("Inapoi",1130);

    //meniu 1, 1, 4 matrici rare
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Numar de zerouri",1141);
    adaugaButonARGauto("Densitatea unei matrici",1142);
    adaugaButonARGauto("Este matrice rara?",1143);
    adaugaButonARGauto("Inapoi",1140);

    //meniu 1, 1, 5 sterge mat
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Sterge",1151);
    adaugaButonARGauto("Inapoi",1150);

    //meniu 1, 1, 6 matrici operaii meniu 3
    adaugaButonARG(spatiustd,spatiustd*2,(width-spatiustd),150,"de pe diagonala principala",1161);
    adaugaButonARGauto("de sub diagonala principala",1162);
    adaugaButonARGauto("de deasupra diagonalei principale",1163);
    adaugaButonARGauto("de pe diagonala secundara",1164);
    adaugaButonARGauto("de sub diagonala secundara",1165);
    adaugaButonARGauto("de deasupra diagonalei secundare",1166);
    adaugaButonARGauto("A doua pagina de operatii",1167);
    adaugaButonARGauto("Inapoi",1160);

    //meniu 1, 2, 1 vectori creat meniu
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Citeste vector din fisier",1211);
    adaugaButonARGauto("Citeste vector de la tastatura",1212);
    adaugaButonARGauto("Creeaza vector generat aleator",1213);
     adaugaButonARGauto("Inapoi",1210);

    //meniu 1, 2, 2 vectori operatii meniu
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Afiseaza un vector in fisier",1221);
    adaugaButonARGauto("Adunare intre vectori",1222);
    adaugaButonARGauto("Scadere intre vectori",1223);
    adaugaButonARGauto("Inmulteste un vector cu un scalar",1224);
    adaugaButonARGauto("Imparte un vector la un scalar",1225);
    adaugaButonARGauto("Suma elementelor unui vector",1226);
    adaugaButonARGauto("Produsul elementelor unui vector",1227);
    adaugaButonARGauto("Insereaza un element",1228);
    adaugaButonARGauto("Deseneaza vectorul",1229);
     adaugaButonARGauto("Inapoi",1220);

    //meniu 1, 2, 3 sterge vect
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Sterge",1231);
    adaugaButonARGauto("Inapoi",1230);

    //meniu 1, 2, 4 sort
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Bubble Sort Crescator",1241);
    adaugaButonARGauto("Bubble Sort Descrescator",1242);
    adaugaButonARGauto("Sortare prin insertie Crescator",1243);
    adaugaButonARGauto("Sortare prin insertie Desrescator",1244);
    adaugaButonARGauto("Sortare prin interschimbare Crescator",1245);
    adaugaButonARGauto("Sortare prin interschimbare Descrescator",1246);
    adaugaButonARGauto("Inapoi",1240);

    //meniu 1, 1, 2, 1
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",11211);
    adaugaButonARGauto("Inapoi",11210);

    //meniu 1, 1, 2, 2
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"<Creeaza matrice noua>",11222);
    adaugaButonARGauto("Continua",11221);
    adaugaButonARGauto("Inapoi",11220);

    //meniu 1, 1, 2, 3
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"<Creeaza matrice noua>",11232);
    adaugaButonARGauto("Continua",11231);
    adaugaButonARGauto("Inapoi",11230);

    //meniu 1, 1, 2, 4
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"<Creeaza matrice noua>",11242);
    adaugaButonARGauto("Continua",11241);
    adaugaButonARGauto("Inapoi",11240);

    //meniu 1, 1, 2, 5
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"<Creeaza matrice noua>",11252);
    adaugaButonARGauto("Continua",11251);
    adaugaButonARGauto("Inapoi",11250);

    //meniu 1, 1, 2, 6
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"<Creeaza matrice noua>",11262);
    adaugaButonARGauto("Continua",11261);
    adaugaButonARGauto("Inapoi",11260);

    //meniu 1, 1, 2, 7
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"<Creeaza matrice noua>",11272);
    adaugaButonARGauto("Continua",11271);
    adaugaButonARGauto("Inapoi",11270);

    //meniu 1, 1, 3, 1
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"<Creeaza matrice noua>",11312);
    adaugaButonARGauto("Continua",11311);
    adaugaButonARGauto("Inapoi",11310);

    //meniu 1, 1, 3, 2
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",11321);
    adaugaButonARGauto("Inapoi",11320);

    //meniu 1, 1, 3, 3
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"<Creeaza matrice noua>",11332);
    adaugaButonARGauto("Continua",11331);
    adaugaButonARGauto("Inapoi",11330);

    //meniu 1, 1, 3, 4
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"<Creeaza matrice noua>",11342);
    adaugaButonARGauto("Continua",11341);
    adaugaButonARGauto("Inapoi",11340);

    //meniu 1, 1, 3, 5
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"<Creeaza matrice noua>",11352);
    adaugaButonARGauto("Continua",11351);
    adaugaButonARGauto("Inapoi",11350);

    //meniu 1, 1, 4, 1
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",11411);
    adaugaButonARGauto("Inapoi",11410);

    //meniu 1, 1, 4, 2
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",11421);
    adaugaButonARGauto("Inapoi",11420);

    //meniu 1, 1, 4, 3
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",11431);
    adaugaButonARGauto("Inapoi",11430);

    //meniu 1, 1, 6, 1
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",11611);
    adaugaButonARGauto("Inapoi",11610);

    //meniu 1, 1, 6, 2
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",11621);
    adaugaButonARGauto("Inapoi",11620);

    //meniu 1, 1, 6, 3
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",11631);
    adaugaButonARGauto("Inapoi",11630);

    //meniu 1, 1, 6, 4
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",11641);
    adaugaButonARGauto("Inapoi",11640);

    //meniu 1, 1, 6, 5
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",11651);
    adaugaButonARGauto("Inapoi",11650);

    //meniu 1, 1, 6, 6
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",11661);
    adaugaButonARGauto("Inapoi",11660);

    //meniu 1, 2, 2, 1
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",12211);
    adaugaButonARGauto("Inapoi",12210);

    //meniu 1, 2, 2, 2
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"<Creeaza Vector Nou>",12222);
    adaugaButonARGauto("Continua",12221);
    adaugaButonARGauto("Inapoi",12220);

    //meniu 1, 2, 2, 3
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"<Creeaza Vector Nou>",12232);
    adaugaButonARGauto("Continua",12231);
    adaugaButonARGauto("Inapoi",12230);

    //meniu 1, 2, 2, 4
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"<Creeaza Vector Nou>",12242);
    adaugaButonARGauto("Continua",12241);
    adaugaButonARGauto("Inapoi",12240);

    //meniu 1, 2, 2, 5
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"<Creeaza Vector Nou>",12252);
    adaugaButonARGauto("Continua",12251);
    adaugaButonARGauto("Inapoi",12250);

    //meniu 1, 2, 2, 6
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",12261);
    adaugaButonARGauto("Inapoi",12260);

    //meniu 1, 2, 2, 7
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",12271);
    adaugaButonARGauto("Inapoi",12270);

    //meniu 1, 2, 2, 8
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",12281);
    adaugaButonARGauto("Inapoi",12280);

    //meniu 1, 2, 4, 1
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",12411);
    adaugaButonARGauto("Inapoi",12410);

    //meniu 1, 2, 4, 2
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",12421);
    adaugaButonARGauto("Inapoi",12420);

    //meniu 1, 2, 4, 3
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",12431);
    adaugaButonARGauto("Inapoi",12430);

    //meniu 1, 2, 4, 4
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",12441);
    adaugaButonARGauto("Inapoi",12440);

    //meniu 1, 2, 4, 5
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",12451);
    adaugaButonARGauto("Inapoi",12450);

    //meniu 1, 2, 4, 6
    adaugaButonARG(spatiustd,spatiustd,(width-spatiustd),100,"Continua",12461);
    adaugaButonARGauto("Inapoi",12460);
}

TIP putereScalar(TIP x, int k)
{
    TIP result;
    while(k)
    {
        if(k & 1) result *= x;
        k >>= 1;
        if(!k) break;
        x*=x;
    }
    return result;
}

int putereMinusUnu(int k)
{
    if(k & 1) return -1;
    return 1;
}

void square(int x1,int y1,int x2, int y2)
{
    line(x1,y1,x1,y2);
    line(x1,y1,x2,y1);
    line(x2,y1,x2,y2);
    line(x1,y2,x2,y2);
}

void resizeBox()
{
    int marimeMax = 0;
    for(int i = 0; i < nrMatrici; i++)
        for(int j = 0; j < matriciCreate[i].nrLinii; j++)
            for(int k = 0; k < matriciCreate[i].nrColoane; k++)
                if(strlen(intToString(matriciCreate[i].elemente[j][k])) > marimeMax) marimeMax = strlen(intToString(matriciCreate[i].elemente[j][k]));
    for(int i = 0; i < nrVectori; i++)
        for(int j = 0; j < vectoriCreati[i].nrElemente; j++)
            if(strlen(intToString(vectoriCreati[i].elemente[j])) > marimeMax) marimeMax = strlen(intToString(vectoriCreati[i].elemente[j]));
    boxSize = 50 + 5*marimeMax;
    for(int i = 0; i < nrMatrici; i++)
    {
        CoordMat[matriciCreate[i].id].x1 = boxSize;
        CoordMat[matriciCreate[i].id].y1 = CoordMat[matriciCreate[i].id-1].y2 + boxSize;
        CoordMat[matriciCreate[i].id].x2 = CoordMat[matriciCreate[i].id].x1 + matriciCreate[i].nrColoane * boxSize;
        CoordMat[matriciCreate[i].id].y2 = CoordMat[matriciCreate[i].id].y1 + matriciCreate[i].nrLinii * boxSize;
    }
    for(int i = 0; i < nrVectori; i++)
    {
        CoordVect[vectoriCreati[i].id].x1 = boxSize;
        CoordVect[vectoriCreati[i].id].y1 = CoordVect[vectoriCreati[i].id - 1].y2 + boxSize;
        CoordVect[vectoriCreati[i].id].x2 = CoordVect[vectoriCreati[i].id].x1 + vectoriCreati[i].nrElemente * boxSize;
        CoordVect[vectoriCreati[i].id].y2 = CoordVect[vectoriCreati[i].id].y1 + boxSize;
    }
}

void afisareMeniu(int x)
{
    //initwindow(width,height,"Matrici si vectori",spatiustd,spatiustd);
    setcurrentwindow(0);
    setcolor(GREEN);
    int maxx = 0;
    for(int i = 0; i < nrButoane; i++)
    {
        setcolor(LIGHTGREEN);
        if(butoane[i].id/10 == x)
        {
            //butoane[i].show();
            if(butoane[i].y2 > maxx) maxx = butoane[i].y2;
        }
    }
    square(0,y_Offset,width - 1,maxx + spatiustd + y_Offset);
    square(spatiustd/10,y_Offset + spatiustd/10,width - spatiustd/10 - 1,maxx + spatiustd + y_Offset - spatiustd/10);
    if(x == 116) {
        setcolor(LIGHTGREEN);
        outtextxy(width/2 - 3.14*strlen("Suma elementelor...") + x_Offset,25 + y_Offset,"Suma elementelor...");
    }
    for(int i = 0; i < nrButoane; i++)
    {
        setcolor(LIGHTGREEN);
        if(butoane[i].id/10 == x)
        {
            butoane[i].show();
        }
    }
    setcolor(WHITE);
}

void afisareMatrici()
{
    //initwindow(width,1000,"Matrici",width + spatiustd,spatiustd);
    setcurrentwindow(1);
    cleardevice();
    int xmax=-9999,ymax=-9999;
    for(int i = 0; i < nrMatrici; i++)
    {
        if(CoordMat[i].x2 > xmax) xmax = CoordMat[i].x2;
        if(CoordMat[i].y2 > ymax) ymax = CoordMat[i].y2;
    }
    if(nrMatrici > 0) {
        setcolor(LIGHTBLUE);
        square(x_OffsetM, y_OffsetM, x_OffsetM + xmax + boxSize * 4,y_OffsetM + ymax + spatiustd *2);
        square(x_OffsetM + spatiustd/10,y_OffsetM + spatiustd/10,x_OffsetM + xmax + boxSize * 4 - spatiustd/10,y_OffsetM + ymax + spatiustd *2 - spatiustd/10);
    }
    setcolor(WHITE);
    for(int i = 0; i < nrMatrici; i++)
    {
        matriciCreate[i].afisareEcran();
    }
}

void afisareVectori()
{
    setcurrentwindow(2);
    cleardevice();
    int xmax=-9999,ymax=-9999;
    for(int i = 0; i < nrVectori; i++)
    {
        if(CoordVect[i].x2 > xmax) xmax = CoordVect[i].x2;
        if(CoordVect[i].y2 > ymax) ymax = CoordVect[i].y2;
    }
    if(nrVectori > 0) {
        setcolor(RED);
        square(x_OffsetV, y_OffsetV, x_OffsetV + xmax + boxSize * 4,y_OffsetV + ymax + spatiustd *2);
        square(x_OffsetV + spatiustd/10,y_OffsetV + spatiustd/10,x_OffsetV + xmax + boxSize * 4 - spatiustd/10,y_OffsetV + ymax + spatiustd *2 - spatiustd/10);
    }
    setcolor(WHITE);
    for(int i = 0; i < nrVectori; i++)
    {
        vectoriCreati[i].afisareEcran();
    }
}

void adaugaMatrice(matrice& m)
{
    m.id = nrMatrici;
    if(nrMatrici == 0)
    {
        matriciCreate[nrMatrici++] = m;
        CoordMat[m.id].x1 = boxSize;
        CoordMat[m.id].y1 = boxSize;
        CoordMat[m.id].x2 = boxSize;
        CoordMat[m.id].y2 = boxSize;
    }
    else {
        matriciCreate[nrMatrici++] = m;
        CoordMat[m.id].x1 = boxSize;
        CoordMat[m.id].x2 = boxSize;
        CoordMat[m.id].y1 = CoordMat[m.id-1].y2 + boxSize;
        CoordMat[m.id].y2 = boxSize;
    }
}

void adaugaVector(vect& v)
{
    v.id = nrVectori;
    if(nrVectori == 0)
    {
        vectoriCreati[nrVectori++] = v;
        CoordVect[v.id].x1 = boxSize;
        CoordVect[v.id].y1 = boxSize;
        CoordVect[v.id].x2 = boxSize;
        CoordVect[v.id].y2 = boxSize;
    }
    else {
        vectoriCreati[nrVectori++] = v;
        CoordVect[v.id].x1 = boxSize;
        CoordVect[v.id].x2 = boxSize;
        CoordVect[v.id].y1 = CoordVect[v.id-1].y2 + boxSize;
        CoordVect[v.id].y2 = boxSize;
    }
}

void stergeMatrice(int x)
{
    if(x == nrMatrici - 1) nrMatrici--;
    else
    {
        int j = 0;
        for(int i = 0; i < nrMatrici; i++)
            if(i != x) matriciNuSterge[j++] = matriciCreate[i];
        nrMatrici = 0;
        for(int i = 0; i < j; i++)
            {
                adaugaMatrice(matriciNuSterge[i]);
                CoordMat[nrMatrici - 1].x2 = CoordMat[nrMatrici - 1].x1 + matriciCreate[nrMatrici - 1].nrColoane * boxSize;
                CoordMat[nrMatrici - 1].y2 = CoordMat[nrMatrici - 1].y1 + matriciCreate[nrMatrici - 1].nrLinii * boxSize;
            }
    }
    y_OffsetM = 0;
    x_OffsetM = 0;
}

void stergeVector(int x)
{
    if(x == nrVectori - 1) nrVectori--;
    else
    {
        int j = 0;
        for(int i = 0; i < nrVectori; i++)
            if(i != x) vectoriNuSterge[j++] = vectoriCreati[i];
        nrVectori = 0;
        for(int i = 0; i < j; i++)
            {
                adaugaVector(vectoriNuSterge[i]);
                CoordVect[nrVectori - 1].x2 = CoordVect[nrVectori - 1].x1 + vectoriCreati[nrVectori - 1].nrElemente * boxSize;
                CoordVect[nrVectori - 1].y2 = CoordVect[nrVectori - 1].y1 + boxSize;
            }
    }
    y_OffsetV = 0;
    x_OffsetV = 0;
}

void adaugaButon(button b)
{
    butoane[nrButoane++] = b;
}

void adaugaButonARG(int x1, int y1, int x2, int y2, char t[NRMAX], int k)
{
    button b(x1,y1,x2,y2,t,k);
    adaugaButon(b);
}

void adaugaButonARGauto(char t[NRMAX], int k)
{
    button b(butoane[nrButoane - 1].x1, butoane[nrButoane - 1].y2 + spatiustd, butoane[nrButoane - 1].x2, butoane[nrButoane - 1].y2 + spatiustd*2, t, k);
    adaugaButon(b);
}

void whatPress(int a, int b, int men, int cw)
{
    if(cw == 0) {
        for(int i = 0; i < nrButoane; i++)
        {
            if(butoane[i].didIPress(a,b,men)) butoane[i].onClick();
        }
    }
    else if(cw == 1) {
        for(int i = 0; i < nrMatrici; i++)
        {
            if(CoordMat[i].x1 <= a - x_OffsetM && CoordMat[i].x2 >= a - x_OffsetM && CoordMat[i].y1 <= b - y_OffsetM && CoordMat[i].y2 >= b - y_OffsetM)
                {
                    //cout << "DA" << nrMatriciSelectate;
                    if(matriciCreate[i].selected == false && trebuie_selectate) {
                        matriciCreate[i].selected = true;
                        trebuie_selectate--;
                        matriciSelectate[nrMatriciSelectate++] = matriciCreate[i];
                    }
                    else if(matriciCreate[i].selected == true) {
                        matriciCreate[i].selected = false;
                        trebuie_selectate++;
                        nrMatriciSelectate--;
                    }
                }
        }
    }
    else if(cw == 2) {
        for(int i = 0; i < nrVectori; i++)
        {
            if(CoordVect[i].x1 <= a - x_OffsetV && CoordVect[i].x2 >= a - x_OffsetV && CoordVect[i].y1 <= b - y_OffsetV && CoordVect[i].y2 >= b - y_OffsetV)
            {
                if(vectoriCreati[i].selected == false && trebuie_selectateV) {
                    vectoriCreati[i].selected = true;
                    trebuie_selectateV--;
                    vectoriSelectati[nrVectoriSelectati++] = vectoriCreati[i];
                }
                else if(vectoriCreati[i].selected == true) {
                    vectoriCreati[i].selected = false;
                    trebuie_selectateV++;
                    nrVectoriSelectati--;
                }
            }
        }
    }
}

void changemen(int a)
{
    meniucrt = a;
}

void scroll(int x, int y, int xx, int yy, int w)
{
    if(w == winds+1)
    {
        x_OffsetM -= x-xx;
        y_OffsetM -= y-yy;
    }
    else if(w == winds)
    {
        y_Offset -= y-yy;
    }
    else if(w == winds + 2)
    {
        x_OffsetV -= x-xx;
        y_OffsetV -= y-yy;
    }
}

void citesteInModGraficNR(int nr, int x, int y, int culoareText, int m)
{
    char tasta, sir[2];
    sir[1]='\0';
    char text_[NRMAX],text[NRMAX];
    strcpy(text,"");
    strcpy(text_,"_");
    setcolor(culoareText);
    outtextxy(x,y,text_);
    do
    {
        outtextxy(x,y,text_);
        tasta=getch();
        cleardevice();
        if (tasta==8 && strlen(text)>0)
        {
            strcpy(text_,text);
            strcat(text_,"_");
            cleardevice();
            outtextxy(x,y,text_);
            text[strlen(text)-1]='\0';
            strcpy(text_,text);
            strcat(text_,"_");
            setcolor(culoareText);
            cleardevice();
            outtextxy(x,y,text_);
        }
        if ((tasta>=48 && tasta<=57) || tasta == 45 || tasta == 46)
        {
            strcpy(text_,text);
            strcat(text_,"_");
            cleardevice();
            outtextxy(x,y,text_);
            text_[strlen(text_)-1]='\0';
            sir[0]=tasta;
            setcolor(culoareText);
            strcat(text,sir);
            strcpy(text_,text);
            strcat(text_,"_");
            cleardevice();
            outtextxy(x,y,text_);
        }
    }
    while (tasta!=13 && tasta!=27);
    outtextxy(x,y,text_);
    if (tasta==27)
    {
        strcpy(text,"");
        setcurrentwindow(0);
        changemen(m/10);
        closegraph(inputWinId++);
    }
    else if(tasta == 13)
    {
        scalar = strtod(text,NULL);
        setcurrentwindow(0);
        changemen(m);
        closegraph(inputWinId++);
    }
}

void citesteInModGraficNRint(int nr, int x, int y, int culoareText, int m)
{
    char tasta, sir[2];
    sir[1]='\0';
    char text_[NRMAX],text[NRMAX];
    strcpy(text,"");
    strcpy(text_,"_");
    setcolor(culoareText);
    outtextxy(x,y,text_);
    do
    {
        outtextxy(x,y,text_);
        tasta=getch();
        cleardevice();
        if (tasta==8 && strlen(text)>0)
        {
            strcpy(text_,text);
            strcat(text_,"_");
            cleardevice();
            outtextxy(x,y,text_);
            text[strlen(text)-1]='\0';
            strcpy(text_,text);
            strcat(text_,"_");
            setcolor(culoareText);
            cleardevice();
            outtextxy(x,y,text_);
        }
        if ((tasta>=48 && tasta<=57) || tasta == 45)
        {
            strcpy(text_,text);
            strcat(text_,"_");
            cleardevice();
            outtextxy(x,y,text_);
            text_[strlen(text_)-1]='\0';
            sir[0]=tasta;
            setcolor(culoareText);
            strcat(text,sir);
            strcpy(text_,text);
            strcat(text_,"_");
            cleardevice();
            outtextxy(x,y,text_);
        }
    }
    while (tasta!=13 && tasta!=27);
    outtextxy(x,y,text_);
    if (tasta==27)
    {
        strcpy(text,"");
        setcurrentwindow(0);
        changemen(m/10);
        closegraph(inputWinId++);
    }
    else if(tasta == 13)
    {
        scalar = strtod(text,NULL);
        setcurrentwindow(0);
        changemen(m);
        closegraph(inputWinId++);
    }
}

void citesteInModGraficNRspecific(int nr, int x, int y, int culoareText, int m, TIP& numar)
{
    char tasta, sir[2];
    sir[1]='\0';
    char text_[NRMAX],text[NRMAX];
    strcpy(text,"");
    strcpy(text_,"_");
    setcolor(culoareText);
    outtextxy(x,y,text_);
    do
    {
        outtextxy(x,y,text_);
        tasta=getch();
        cleardevice();
        if (tasta==8 && strlen(text)>0)
        {
            strcpy(text_,text);
            strcat(text_,"_");
            cleardevice();
            outtextxy(x,y,text_);
            text[strlen(text)-1]='\0';
            strcpy(text_,text);
            strcat(text_,"_");
            setcolor(culoareText);
            cleardevice();
            outtextxy(x,y,text_);
        }
        if ((tasta>=48 && tasta<=57) || tasta == 45 || tasta == 46)
        {
            strcpy(text_,text);
            strcat(text_,"_");
            cleardevice();
            outtextxy(x,y,text_);
            text_[strlen(text_)-1]='\0';
            sir[0]=tasta;
            setcolor(culoareText);
            strcat(text,sir);
            strcpy(text_,text);
            strcat(text_,"_");
            cleardevice();
            outtextxy(x,y,text_);
        }
    }
    while (tasta!=13 && tasta!=27);
    outtextxy(x,y,text_);
    if (tasta==27)
    {
        strcpy(text,"");
        setcurrentwindow(0);
        changemen(m/10);
        closegraph(inputWinId++);
    }
    else if(tasta == 13)
    {
        numar = strtod(text,NULL);
        setcurrentwindow(0);
        closegraph(inputWinId++);
    }
}

// functii matrici

void matrice::citireTastatura()
{
    system("cls");
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h,FOREGROUND_GREEN);
    elemente[0][0] = -13.2;
    cout << "Introduceti dimensiunile matricii (numarul de linii si de coloane):\n";
    SetConsoleTextAttribute(h,FOREGROUND_RED);
    cin >> nrLinii >> nrColoane;
    SetConsoleTextAttribute(h,FOREGROUND_GREEN);
    if(elemente[0][0] == -13.2) cout << "Introduceti cele " << nrLinii*nrColoane << " elemente:\n";
    SetConsoleTextAttribute(h,FOREGROUND_RED);
    for(int i = 0; i < nrLinii; i++)
        for(int j = 0; j < nrColoane; j++)
            cin >> elemente[i][j];
    CoordMat[id].x2 = CoordMat[id].x1 + nrColoane * boxSize;
    CoordMat[id].y2 = CoordMat[id].y1 + nrLinii * boxSize;
    system("cls");
}

void matrice::citireFisier()
{
    ifstream fin("matrici.in");
    fin >> nrLinii >> nrColoane;
    for(int i = 0; i < nrLinii; i++)
        for(int j = 0; j < nrColoane; j++)
            fin >> elemente[i][j];
    CoordMat[id].x2 = CoordMat[id].x1 + nrColoane * boxSize;
    CoordMat[id].y2 = CoordMat[id].y1 + nrLinii * boxSize;
    fin.close();
}

void matrice::genRandom()
{
    nrLinii = rand()%6 + 1;
    nrColoane = rand()%6 + 1;
    for(int i = 0; i < nrLinii; i++)
        for(int j = 0; j < nrColoane; j++)
            elemente[i][j] = ((100) - (-100)) * ( (double)rand() / (double)RAND_MAX ) + (-100);
    CoordMat[id].x2 = CoordMat[id].x1 + nrColoane * boxSize;
    CoordMat[id].y2 = CoordMat[id].y1 + nrLinii * boxSize;
}

void matrice::afisareEcran()
{
    setcurrentwindow(1);
    if(selected) setcolor(LIGHTGREEN);
    else setcolor(WHITE);
    for(int i = 0; i < nrLinii; i++)
    {
        for(int j = 0; j < nrColoane; j++)
        {
            square(x_OffsetM + CoordMat[id].x1 + j * boxSize,y_OffsetM +  CoordMat[id].y1 + i * boxSize,x_OffsetM +  CoordMat[id].x1 + (j+1) * boxSize,y_OffsetM +  CoordMat[id].y1 + (i+1) * boxSize);
            outtextxy(x_OffsetM + (int)(CoordMat[id].x1 + (j + 0.5) * boxSize - 3.14 * strlen(intToString(elemente[i][j]))),y_OffsetM + (int)(CoordMat[id].y1 + (i + 0.5) * boxSize - 8),intToString(elemente[i][j]));
        }
    }
    char txt[NRMAX];
    strcpy(txt,"<--- Matricea numarul ");
    strcat(txt,intToString(id + 1));
    outtextxy(x_OffsetM + (nrColoane + 2) * boxSize,y_OffsetM + CoordMat[id].y1 + (CoordMat[id].y2 - CoordMat[id].y1)/2 - 10,txt);
}

void matrice::afisareFisier()
{
    ofstream fout("matrici.out");
    fout << nrLinii << ' ' << nrColoane << '\n';
    for(int i = 0; i < nrLinii; i++)
    {
        for(int j = 0; j < nrColoane; j++)
        {
            fout << elemente[i][j];
            if(j != nrColoane - 1)
                fout << ' ';
        }
        if(i != nrLinii)
            fout << '\n';
    }
    fout.close();
}

void matrice::adunareMatrici(matrice a, matrice b)
{
    if(a.nrColoane != b.nrColoane || a.nrLinii != b.nrLinii) {
        //cout << erori[0];
        return;
    }
    nrColoane = a.nrColoane;
    nrLinii = a.nrLinii;
    for(int i = 0; i < nrLinii; i++)
        for(int j = 0; j < nrColoane; j++)
            elemente[i][j] = a.elemente[i][j] + b.elemente[i][j];
    CoordMat[id].x2 = CoordMat[id].x1 + nrColoane * boxSize;
    CoordMat[id].y2 = CoordMat[id].y1 + nrLinii * boxSize;
}

void matrice::scadereMatrici(matrice a, matrice b)
{
    if(a.nrColoane != b.nrColoane || a.nrLinii != b.nrLinii) {
        //cout << erori[0];
        return;
    }
    nrColoane = a.nrColoane;
    nrLinii = a.nrLinii;
    for(int i = 0; i < nrLinii; i++)
        for(int j = 0; j < nrColoane; j++)
            elemente[i][j] = a.elemente[i][j] - b.elemente[i][j];
    CoordMat[id].x2 = CoordMat[id].x1 + nrColoane * boxSize;
    CoordMat[id].y2 = CoordMat[id].y1 + nrLinii * boxSize;
}

void matrice::produsMatriceScalar(matrice a, TIP k)
{
    nrColoane = a.nrColoane;
    nrLinii = a.nrLinii;
    for(int i = 0; i < a.nrLinii; i++)
        for(int j = 0; j < a.nrColoane; j++)
            elemente[i][j]=a.elemente[i][j] * k;
    CoordMat[id].x2 = CoordMat[id].x1 + nrColoane * boxSize;
    CoordMat[id].y2 = CoordMat[id].y1 + nrLinii * boxSize;
}

void matrice::impartireMatriceScalar(matrice a, TIP k)
{
    nrColoane = a.nrColoane;
    nrLinii = a.nrLinii;
    for(int i = 0; i < a.nrLinii; i++)
        for(int j = 0; j < a.nrColoane; j++)
            {
                elemente[i][j]=a.elemente[i][j] / k;
            }
    CoordMat[id].x2 = CoordMat[id].x1 + nrColoane * boxSize;
    CoordMat[id].y2 = CoordMat[id].y1 + nrLinii * boxSize;
}

void matrice::inmultireMatrici(matrice a, matrice b)
{
    nrColoane = b.nrColoane;
    nrLinii = a.nrLinii;
    for(int i = 0; i < nrLinii; i++)
        for(int j = 0; j < nrColoane; j++)
            elemente[i][j] = 0;
    for(int i = 0; i < a.nrLinii; i++)
        for(int j = 0; j < b.nrColoane; j++)
            for(int k = 0; k < a.nrColoane; k++)
                elemente[i][j] += a.elemente[i][k] * b.elemente[k][j];
    CoordMat[id].x2 = CoordMat[id].x1 + nrColoane * boxSize;
    CoordMat[id].y2 = CoordMat[id].y1 + nrLinii * boxSize;
}

void matrice::impartireMatrici(matrice a, matrice b)
{
    nrLinii = a.nrColoane;
    nrColoane = a.nrLinii;
    matrice aux;
    inversaMatrice(b);
    inmultireMatrici(a,*this);
    CoordMat[id].x2 = CoordMat[id].x1 + nrColoane * boxSize;
    CoordMat[id].y2 = CoordMat[id].y1 + nrLinii * boxSize;
}

void matrice::transpusaMatrice(matrice a)
{
    nrLinii = a.nrColoane;
    nrColoane = a.nrLinii;
    for(int i = 0; i < a.nrLinii; i++)
        for(int j = 0; j < a.nrColoane; j++)
            elemente[j][i] = a.elemente[i][j];
    CoordMat[id].x2 = CoordMat[id].x1 + nrColoane * boxSize;
    CoordMat[id].y2 = CoordMat[id].y1 + nrLinii * boxSize;
}

TIP matrice::gammaMatrice(int linie, int coloana)
{
    matrice b;
    b.nrLinii = nrLinii - 1;
    b.nrColoane = nrColoane - 1;
    int ik = 0, jk = 0;
    for(int i = 0; i < nrLinii; i++)
    {
        if(i == linie) continue;
        jk = 0;
        for(int j = 0; j < nrColoane; j++)
        {
            if(j == coloana) continue;
            b.elemente[ik][jk] = elemente[i][j];
            jk++;
        }
        ik++;
    }
    return b.determinantMatrice();
}

TIP matrice::determinantMatrice()
{
    if(nrColoane == 1) return elemente[0][0];
    else if(nrColoane == 2)
    {
        return (
                elemente[0][0] * elemente[1][1] -
                elemente[0][1] * elemente[1][0]
                );
    }
    else if(nrColoane == 3)
    {
        return (
                elemente[0][0] * elemente[1][1] * elemente[2][2] +
                elemente[1][0] * elemente[2][1] * elemente[0][2] +
                elemente[0][1] * elemente[1][2] * elemente[2][0] -
                elemente[0][2] * elemente[1][1] * elemente[2][0] -
                elemente[0][0] * elemente[1][2] * elemente[2][1] -
                elemente[0][1] * elemente[1][0] * elemente[2][2]
                );
    }
    else
    {
        TIP raspuns;
        for(int i = 0; i < nrColoane; i++)
            raspuns += (putereMinusUnu(i) *elemente[0][i] * gammaMatrice(0, i));
        return raspuns;
    }
}

void matrice::inversaMatrice(matrice a)
{
    nrLinii = a.nrColoane;
    nrColoane = a.nrLinii;
    //matrice b;
    adjunctaMatrice(a);
    TIP c = a.determinantMatrice();
    impartireMatriceScalar(*this,c);
    CoordMat[id].x2 = CoordMat[id].x1 + nrColoane * boxSize;
    CoordMat[id].y2 = CoordMat[id].y1 + nrLinii * boxSize;
}

void matrice::adjunctaMatrice(matrice a)
{
    nrLinii = a.nrColoane;
    nrColoane = a.nrLinii;
    for(int i = 0; i < a.nrLinii; i++)
        for(int j = 0; j < a.nrColoane; j++)
    {
        elemente[i][j] = putereMinusUnu(i+j) * a.gammaMatrice(i, j);
    }
    transpusaMatrice(*this);
    CoordMat[id].x2 = CoordMat[id].x1 + nrColoane * boxSize;
    CoordMat[id].y2 = CoordMat[id].y1 + nrLinii * boxSize;

}

void matrice::ridicaLaPutereMatrice(matrice a, int k)
{
    if(k == 0)
    {
        nrLinii = a.nrLinii;
        nrColoane = a.nrColoane;
        for(int i = 0; i < a.nrLinii; i++)
            for(int j = 0; j < a.nrColoane; j++)
                {
                    elemente[i][j] = 0;
                    if(i == j) elemente[i][j] = 1;
                }
        CoordMat[id].x2 = CoordMat[id].x1 + nrColoane * boxSize;
        CoordMat[id].y2 = CoordMat[id].y1 + nrLinii * boxSize;
    }
    else if(k < 0)
    {
        k*=-1;
        nrLinii = a.nrLinii;
        nrColoane = a.nrColoane;
        for(int i = 0; i < a.nrLinii; i++)
            for(int j = 0; j < a.nrColoane; j++)
                elemente[i][j] = a.elemente[i][j];
        for(int i = 1; i < k; i++)
            inmultireMatrici(*this,a);
        CoordMat[id].x2 = CoordMat[id].x1 + nrColoane * boxSize;
        CoordMat[id].y2 = CoordMat[id].y1 + nrLinii * boxSize;
        inversaMatrice(*this);
    }
    else
    {
        nrLinii = a.nrLinii;
        nrColoane = a.nrColoane;
        for(int i = 0; i < a.nrLinii; i++)
            for(int j = 0; j < a.nrColoane; j++)
                elemente[i][j] = a.elemente[i][j];
        for(int i = 1; i < k; i++)
            inmultireMatrici(*this,a);
        CoordMat[id].x2 = CoordMat[id].x1 + nrColoane * boxSize;
        CoordMat[id].y2 = CoordMat[id].y1 + nrLinii * boxSize;
    }
}

bool matrice::esteRara()
{
    if(densitateMatrice() < 50) return true;
    else return false;
}

int matrice::numarElementeZero()
{
    int n = 0;
    for(int i = 0; i < nrLinii; i++)
        for(int j = 0; j < nrColoane; j++)
            if(elemente[i][j] == 0) n++;
    return n;
}

double matrice::densitateMatrice()
{
    return ((((nrColoane * nrLinii) - numarElementeZero()) * 100) / (nrColoane * nrLinii));
}

TIP matrice::sumaDP()
{
    TIP r = 0;
    for(int i = 0; i < matriciCreate[id].nrLinii; i++)
        r+= matriciCreate[id].elemente[i][i];
    return r;
}

TIP matrice::sumaSubDP()
{
    TIP r = 0;
    for(int i = 0; i < matriciCreate[id].nrLinii; i++)
        for(int j = 0; j < matriciCreate[id].nrColoane; j++)
            if(i > j) r+= matriciCreate[id].elemente[i][j];
    return r;
}

TIP matrice::sumaDeasupraDP()
{
    TIP r = 0;
    for(int i = 0; i < matriciCreate[id].nrLinii; i++)
        for(int j = 0; j < matriciCreate[id].nrColoane; j++)
            if(i < j) r+= matriciCreate[id].elemente[i][j];
    return r;
}

TIP matrice::sumaDS()
{
    TIP r = 0;
    for(int i = 0; i < matriciCreate[id].nrLinii; i++)
        r+= matriciCreate[id].elemente[i][matriciCreate[id].nrLinii - 1 - i];
    return r;
}

TIP matrice::sumaSubDS()
{
    TIP r = 0;
    for(int i = 0; i < matriciCreate[id].nrLinii; i++)
        for(int j = 0; j < matriciCreate[id].nrColoane; j++)
            if(i+j > matriciCreate[id].nrLinii - 1) r+= matriciCreate[id].elemente[i][j];
    return r;
}

TIP matrice::sumaDeasupraDS()
{
    TIP r = 0;
    for(int i = 0; i < matriciCreate[id].nrLinii; i++)
        for(int j = 0; j < matriciCreate[id].nrColoane; j++)
            if(i+j < matriciCreate[id].nrLinii - 1) r+= matriciCreate[id].elemente[i][j];
    return r;
}

//functii vectori

void vect::citireTastatura()
{
    system("cls");
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h,FOREGROUND_BLUE);
    elemente[0] = -13.2;
    cout << "Introduceti dimensiunea vectorului (numarul de elemente):\n";
    SetConsoleTextAttribute(h,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cin >> nrElemente;
    SetConsoleTextAttribute(h,FOREGROUND_BLUE);
    if(elemente[0] == -13.2) cout << "Introduceti cele " << nrElemente << " elemente:\n";
    SetConsoleTextAttribute(h,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    for(int i = 0; i < nrElemente; i++)
        cin >> elemente[i];
    CoordVect[id].x2 = CoordVect[id].x1 + nrElemente * boxSize;
    CoordVect[id].y2 = CoordVect[id].y1 + boxSize;
    system("cls");
}

void vect::citireFisier()
{
    ifstream fin("vectori.in");
    fin >> nrElemente;
    for(int i = 0; i < nrElemente; i++)
        fin >> elemente[i];
    CoordVect[id].x2 = CoordVect[id].x1 + nrElemente * boxSize;
    CoordVect[id].y2 = CoordVect[id].y1 + boxSize;
    fin.close();
}

void vect::genRandom()
{
    nrElemente = rand()%12 + 1;
    for(int i = 0; i < nrElemente; i++)
        elemente[i] = ((100) - (-100)) * ( (double)rand() / (double)RAND_MAX ) + (-100);
    CoordVect[id].x2 = CoordVect[id].x1 + nrElemente * boxSize;
    CoordVect[id].y2 = CoordVect[id].y1 + boxSize;
}

void vect::afisareEcran()
{
    setcurrentwindow(2);
    if(selected) setcolor(YELLOW);
    else setcolor(WHITE);
    for(int i = 0; i < nrElemente; i++)
    {
        square(x_OffsetV + CoordVect[id].x1 + i * boxSize,y_OffsetV +  CoordVect[id].y1 ,x_OffsetV +  CoordVect[id].x1 + (i+1) * boxSize,y_OffsetV +  CoordVect[id].y1 + (1) * boxSize);
        outtextxy(x_OffsetV + (int)(CoordVect[id].x1 + (i + 0.5) * boxSize - 3.14 * strlen(intToString(elemente[i]))),y_OffsetV + (int)(CoordVect[id].y1 + (0.5) * boxSize - 8),intToString(elemente[i]));
    }
    char txt[NRMAX];
    strcpy(txt,"<--- Vectorul numarul ");
    strcat(txt,intToString(id + 1));
    outtextxy(x_OffsetV + (nrElemente + 2) * boxSize,y_OffsetV + CoordVect[id].y1 + (CoordVect[id].y2 - CoordVect[id].y1)/2 - 10,txt);
}

void vect::afisareFisier()
{
    ofstream fout("vectori.out");
    fout << nrElemente << '\n';
    for(int i = 0; i < nrElemente; i++)
        fout << elemente[i] << ' ';
    fout << '\n';
    fout.close();
}

void vect::adunareVectori(vect v, vect w)
{
    nrElemente = v.nrElemente;
    for(int i = 0; i < nrElemente; i++)
        elemente[i] = v.elemente[i] + w.elemente[i];
    CoordVect[id].x2 = CoordVect[id].x1 + nrElemente * boxSize;
    CoordVect[id].y2 = CoordVect[id].y1 + boxSize;
}

void vect::scadereVectori(vect v, vect w)
{
    nrElemente = v.nrElemente;
    for(int i = 0; i < nrElemente; i++)
        elemente[i] = v.elemente[i] - w.elemente[i];
    CoordVect[id].x2 = CoordVect[id].x1 + nrElemente * boxSize;
    CoordVect[id].y2 = CoordVect[id].y1 + boxSize;
}

void vect::produsVectorScalar(vect v, TIP k)
{
    nrElemente = v.nrElemente;
    for(int i = 0; i < nrElemente; i++)
        elemente[i] = v.elemente[i] * k;
    CoordVect[id].x2 = CoordVect[id].x1 + nrElemente * boxSize;
    CoordVect[id].y2 = CoordVect[id].y1 + boxSize;
}

void vect::impartireVectorScalar(vect v, TIP k)
{
    nrElemente = v.nrElemente;
    for(int i = 0; i < nrElemente; i++)
        elemente[i] = v.elemente[i] / k;
    CoordVect[id].x2 = CoordVect[id].x1 + nrElemente * boxSize;
    CoordVect[id].y2 = CoordVect[id].y1 + boxSize;
}

void vect::sumaElemente()
{
    program_principal_suma(vectoriCreati[id]);
    //for(int i = 0; i < nrElemente; i++)
}

void vect::produsElemente()
{
    program_principal_produs(vectoriCreati[id]);
}

void vect::insertElement(TIP x, int k)
{
    program_final_inserare_element(vectoriCreati[id],x,k + 1);
    vectoriCreati[id].nrElemente++;
    for(int i = vectoriCreati[id].nrElemente - 1; i > k; i--)
        vectoriCreati[id].elemente[i] = vectoriCreati[id].elemente[i-1];
    vectoriCreati[id].elemente[k] = x;
    CoordVect[id].x2 += boxSize;
}

void vect::bubble(char mode)
{
    if(mode == 'C')
    {
        program_final_bubble_sort_(vectoriCreati[id],true);
        bool ok = false;
        while(!ok)
        {
            ok=true;
            for(int i=0; i < vectoriCreati[id].nrElemente - 1; i++)
                if(vectoriCreati[id].elemente[i] > vectoriCreati[id].elemente[i+1])
                {
                    swap(vectoriCreati[id].elemente[i],vectoriCreati[id].elemente[i+1]);
                    ok=false;
                }
        }
    }
    else if(mode == 'D')
    {
        program_final_bubble_sort_(vectoriCreati[id],false);
        bool ok = false;
        while(!ok)
        {
            ok=true;
            for(int i=0; i < vectoriCreati[id].nrElemente - 1; i++)
                if(vectoriCreati[id].elemente[i] < vectoriCreati[id].elemente[i+1])
                {
                    swap(vectoriCreati[id].elemente[i],vectoriCreati[id].elemente[i+1]);
                    ok=false;
                }
        }
    }
}

void vect::insertie(char mode)
{
    if(mode == 'C')
    {
        program_final_sortare_prin_insertie(vectoriCreati[id],true);
        for(int i=1; i<vectoriCreati[id].nrElemente; i++)
        {
            if (vectoriCreati[id].elemente[i-1]>vectoriCreati[id].elemente[i])
            {
                TIP aux=vectoriCreati[id].elemente[i];
                int k=i-1;
                while (k>=0 && vectoriCreati[id].elemente[k]>aux)
                {
                    vectoriCreati[id].elemente[k+1]=vectoriCreati[id].elemente[k];
                    k--;
                }
                vectoriCreati[id].elemente[k+1]=aux;
            }
        }
    }
    else if(mode == 'D')
    {
        program_final_sortare_prin_insertie(vectoriCreati[id],false);
        for(int i=1; i<vectoriCreati[id].nrElemente; i++)
        {
            if (vectoriCreati[id].elemente[i-1]<vectoriCreati[id].elemente[i])
            {
                TIP aux=vectoriCreati[id].elemente[i];
                int k=i-1;
                while (k>=0 && vectoriCreati[id].elemente[k]<aux)
                {
                    vectoriCreati[id].elemente[k+1]=vectoriCreati[id].elemente[k];
                    k--;
                }
                vectoriCreati[id].elemente[k+1]=aux;
            }
        }
    }
}

void vect::interschimbare(char mode)
{
    if(mode == 'C')
    {
        program_final_sortare_prin_interschimbare(vectoriCreati[id],true);
        for(int i = 0; i < vectoriCreati[id].nrElemente - 1; i++)
            for(int j = i + 1; j < vectoriCreati[id].nrElemente; j++)
                if(vectoriCreati[id].elemente[i] > vectoriCreati[id].elemente[j])
                    swap(vectoriCreati[id].elemente[i],vectoriCreati[id].elemente[j]);
    }
    else if(mode == 'D')
    {
        program_final_sortare_prin_interschimbare(vectoriCreati[id],false);
        for(int i = 0; i < vectoriCreati[id].nrElemente - 1; i++)
            for(int j = i + 1; j < vectoriCreati[id].nrElemente; j++)
                if(vectoriCreati[id].elemente[i] < vectoriCreati[id].elemente[j])
                    swap(vectoriCreati[id].elemente[i],vectoriCreati[id].elemente[j]);
    }
}

//functii animatii vectori

void afisText_final_suma(TIP s)
{
    bgiout << "SUMA totala a elementelor vectorului este = "<<s;
    outstreamxy(10, 400);
}
void afisText_intermediar_suma(TIP s)
{
    bgiout << "SUMA partiala a elementelor vectorului este = "<<s;
    outstreamxy(10,400);
}
void program_principal_suma(vect v)
{
    TIP s = 0;
    initwindow(1300,600,"Vector",0,0,false,false);
    for(int j=0; j<v.nrElemente; j++)
    {
        s+=v.elemente[j];
        int mid = j;    //lungime intermediara vector

        vect_intermediar_suma(mid,v);
    }
    vect_final_suma(v);
}
void vect_final_suma(vect v)
{
    TIP s=0;
    int q=55;
    int x[NRMAX],y[NRMAX];
    for(int i=0; i < v.nrElemente; i++)
    {
        x[i]=25+q*(i+1);   // coordonatele vectorului pe cele doua axe
        y[i]=200;
        setcolor(LIGHTGREEN);                          //cul.contur Vector
        setfillstyle(SOLID_FILL, GREEN);          //cul.
        fillellipse(x[i],y[i],25,25);       //Deseneaza o elipsa folosind (x[i], y[i])
        //ca punct central si razele de 25 si 25 ca axe orizontala și verticala si o umple cu culoarea
        //curenta de umplere și modelul de umplere curent.
        setcolor(WHITE);                                       //culoare font valoare vector
        outtextxy(x[i]-20,y[i]-5,intToString(v.elemente[i]));
//  afișeaza un sir de caractere in portul de vizualizare la poziția data (x, y), folosind setarile
//actuale de justificare si fontul, directia si dimensiunea curente
        s=s+v.elemente[i];
        afisText_final_suma(s);
    }
    while(!kbhit());//cat timp nu apas nici o tasta afisez reprezentarea grafica
    setcurrentwindow(0);
    closegraph(inputWinId++);      //goleste memoria alocata de sistemul grafic, apoi restabilește ecranul
}
void vect_intermediar_suma(int m, vect v)
{
    int q=105;
    TIP s=0;
    int x[NRMAX],y[NRMAX];
    for(int i=0; i<m; i++)
    {
        x[i]=15+q*(i+1);
        y[i]=200;
        setcolor(LIGHTGREEN);
        setfillstyle(SOLID_FILL, RED);
        fillellipse(x[i],y[i],45,25);

        setcolor(WHITE);
        outtextxy(x[i]-35,y[i]-5,intToString(v.elemente[i]));

        s+=v.elemente[i];
        afisText_intermediar_suma(s);
    }
    Sleep(waitTime);
    //delay(1500);
    cleardevice();
}

void afisText_final_produs(TIP p)
{
    bgiout << "PRODUSUL total al elementelor vectorului este = "<<p;
    outstreamxy(10, 400);
}
void afisText_intermediar_produs(TIP p)
{
    bgiout << "PRODUSUL partial al elementelor vectorului este = "<<p;
    outstreamxy(10,400);
}
void program_principal_produs(vect v)
{
    TIP p = 1;
    initwindow(1300,600,"Vector",0,0,false,false);
    for(int j=0; j<v.nrElemente; j++)
    {
        p*=v.elemente[j];
        int mid = j;    //lungime intermediara vector
        vect_intermediar_produs(mid,v);
    }
    vect_final_produs(v);
}
void vect_final_produs(vect v)
{
    TIP p=1;
    int q=55;
    int x[NRMAX],y[NRMAX];
    for(int i=0; i < v.nrElemente; i++)
    {
        x[i]=25+q*(i+1);   // coordonatele vectorului pe cele doua axe
        y[i]=200;
        setcolor(LIGHTGREEN);                          //cul.contur Vector
        setfillstyle(SOLID_FILL, GREEN);          //cul.
        fillellipse(x[i],y[i],25,25);       //Deseneaza o elipsa folosind (x[i], y[i])
        //ca punct central și razele de 25 și 25 ca axe orizontala și verticala și o umple cu culoarea
        //curenta de umplere și modelul de umplere curent.
        setcolor(WHITE);                                       //culoare font valoare vector
        outtextxy(x[i]-20,y[i]-5,intToString(v.elemente[i]));
//  afișeaza un șir de caractere in portul de vizualizare la poziția data (x, y), folosind setarile
//actuale de justificare și fontul, direcția și dimensiunea curente
        p*=v.elemente[i];
        afisText_final_suma(p);
    }
    while(!kbhit());//cat timp nu apas nici o tasta afisez reprezentarea grafica
    setcurrentwindow(0);
    closegraph(inputWinId++);      //goleste memoria alocata de sistemul grafic, apoi restabilește ecranul
}
void vect_intermediar_produs(int m, vect v)
{
    int q=105;
    TIP p=1;
    int x[NRMAX],y[NRMAX];
    for(int i=0; i<m; i++)
    {
        x[i]=15+q*(i+1);
        y[i]=200;
        setcolor(LIGHTGREEN);
        setfillstyle(SOLID_FILL, RED);
        fillellipse(x[i],y[i],45,25);

        setcolor(WHITE);
        outtextxy(x[i]-35,y[i]-5,intToString(v.elemente[i]));

        p*=v.elemente[i];
        afisText_intermediar_suma(p);
    }
    Sleep(waitTime);
    //delay(1500);
    cleardevice();
}

void gradatie_Oy_desenare_vector()
{
    for(int i=1; i<=27; i++)
    {
        line(647,20*i+20,653,20*i+20);

        int ty=13-(i-1);
        if(ty!=0)
            outtextxy(630,20*i+10,intToString(ty));
    }
}
void gradatie_Ox_desenare_vector()
{
    for(int i=1; i<=61; i++)
    {
        line(i*20+30,297,i*20+30,303);

        int tx=i-31;

        outtextxy(i*20+20,305,intToString(tx));
    }
}
void linie_punctata_Ox_desenare_vector(int x2,int y2, int cy)
{
    setlinestyle(DOTTED_LINE,1,1);
    line(x2,y2,x2,cy);
}
void linie_punctata_Oy_desenare_vector(int x2,int y2, int cx)
{
    setlinestyle(DOTTED_LINE,1,1);
    line(x2,y2,cx,y2);
}
void linie_punctata_Ox_final_desenare_vector(int x2f,int y2f, int cy)
{
    setlinestyle(DOTTED_LINE,1,1);
    line(x2f,y2f,x2f,cy);
}
void linie_punctata_Oy_final_desenare_vector(int x2f,int y2f, int cx)
{
    setlinestyle(DOTTED_LINE,1,1);
    line(x2f,y2f,cx,y2f);
}
void program_final_desenare_vector()
{
    TIP xx = 0,yy = 0,A = 0;
    initwindow(width,100,"x",spatiustd,height - 200 + spatiustd,false,false);
    setcurrentwindow(inputWinId);
    citesteInModGraficNRspecific(scalar,25,25,RED,1229,xx);
    initwindow(width,100,"y",spatiustd,height - 200 + spatiustd,false,false);
    setcurrentwindow(inputWinId);
    citesteInModGraficNRspecific(scalar,25,25,RED,1229,yy);
    initwindow(width,100,"scalar",spatiustd,height - 200 + spatiustd,false,false);
    setcurrentwindow(inputWinId);
    citesteInModGraficNRspecific(scalar,25,25,RED,1229,A);
    int cx=650,cy=300;
    int x2=cx+xx*20;
    int y2=cy-yy*20;
    int x2f=cx+xx*20*A;
    int y2f=cy-yy*20*A;
    initwindow(1300,600,"Grafic",0,0,false,false);
    setbkcolor(15);
    cleardevice();
    setcolor(1);
    line(20, cy, 1280, cy);  //Ox
    line(1280,300,1274,294);
    line(1280,300,1274,306);
    bgiout << "X";
    outstreamxy(1270,280);

    gradatie_Ox_desenare_vector();
    line(cx,10,cx,580);     //Oy
    line(650,10,644,16);
    line(650,10,656,16);
    bgiout << "Y";
    outstreamxy(660,20);


    linie_punctata_Oy_desenare_vector(x2,y2,cx);
    linie_punctata_Oy_final_desenare_vector(x2f,y2f,cx);
    linie_punctata_Ox_desenare_vector(x2,y2,cy);
    linie_punctata_Ox_final_desenare_vector(x2f,y2f,cy);
    gradatie_Oy_desenare_vector();
    setcolor(LIGHTRED );
    setlinestyle(SOLID_LINE,1,3);

    line(cx,cy,x2,y2);        //vect initial
    setcolor(LIGHTBLUE);
    setlinestyle(SOLID_LINE,1,3);
    line(cx-3,cy-3,cx-3+A*xx*20,cy-3-A*yy*20);    //vect inmultit cu scalar a

    getch();
    setcurrentwindow(0);
    closegraph(inputWinId++);
}

void mutare_pozitie_dreapta_inserare_element(int poz1, int n, TIP V2[NRMAX])
{
    //initwindow(1300,600,"Insertie",0,0,false,false);
    int a[NRMAX],b[NRMAX];
    for(int i=1; i<poz1; i++)
    {
        a[i]=100*i;    ///stanga colt sus
        b[i]=100*(i+1);    ///stanga colt jos
        setcolor(LIGHTBLUE);
        setfillstyle(SOLID_FILL,GREEN);
        rectangle(a[i]+10,100,b[i],200);
        outtextxy(100*i+50,150,intToString(V2[i]));
    }

    bar(100*poz1,80,100+100*poz1,90);

    a[poz1]=100*poz1;    ///stanga colt sus
    b[poz1]=100*(poz1+1);    ///stanga colt jos
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL,GREEN);
    rectangle(a[poz1]+10,100,b[poz1],200);

    for(int i=poz1; i<=n; i++)
    {
        a[i]=100*(i+1);    ///stanga colt sus
        b[i]=100*(i+2);    ///stanga colt jos
        setcolor(LIGHTBLUE);
        setfillstyle(SOLID_FILL,GREEN);
        rectangle(a[i]+10,100,b[i],200);
        outtextxy(100*(i+1)+50,150,intToString(V2[i]));
    }
    Sleep(waitTime);
    cleardevice();
}
void vector_rezultat_inserare_element(int n, TIP V[NRMAX])
{
    int a[NRMAX],b[NRMAX];
    for(int i=1; i<=n; i++)
    {
        a[i]=100*i;    ///stanga colt sus
        b[i]=100*(i+1);    ///stanga colt jos
        setcolor(LIGHTBLUE);
        setfillstyle(SOLID_FILL,GREEN);
        rectangle(a[i]+10,100,b[i],200);
        outtextxy(100*i+50,150,intToString(V[i]));
    }
    getch();
    //Sleep(waitTime);
    setcurrentwindow(0);
    closegraph(inputWinId++);
}
void program_final_inserare_element(vect v, TIP element, int poz)
{
    initwindow(1300,600,"Insertie",0,0,false,false);
    int n = v.nrElemente;
    TIP V[NRMAX],V2[NRMAX];
    for(int j=1; j<=n; j++)
    {
        V[j] = v.elemente[j-1];
        V2[j]=V[j];
    }
    poz%=(v.nrElemente);
    if(poz < 0) poz = 1;
    else
    {
        for(int j=n+1; j >= poz; j--)
        {
            mutare_pozitie_dreapta_inserare_element(j, n, V2);
            V[j+1]=V[j];
        }
        n++;
        V[poz]=element;
        vector_rezultat_inserare_element(n,V);
    }
}

void afisText_pozitionare_bubble_sort(int i)
{
    bgiout << "ELEMENTELE   "<<i<<" si "<<i+1<<" ce trebuie INTERSCHIMBATE se coloreaza cu ROSU";
    outstreamxy(30, 400);
}
void afisText_swap_bubble_sort(int i)
{
    bgiout << "SE INTERSCHIMBA   "<<i <<" cu "<<i+1;
    outstreamxy(30, 400);
}
void afisText_vect_ord_bubble_sort()
{
    bgiout << "ACESTA E VECTORUL SORTAT CU BUBBLE SORT ";
    outstreamxy(30, 400);
}
void vect_col_el_pt_swap_bubble_sort(int p, int n, TIP V[NRMAX])
{
    int q=75;
    int x[NRMAX],y[NRMAX];
    for(int j=1; j<=n; j++)
    {
        x[j]=25+q*j;
        y[j]=200;
        setcolor(LIGHTGREEN);
        rectangle(x[j],y[j],x[j]+45,y[j]+35);
        if(j==p||j==p+1)
            setfillstyle(SOLID_FILL,RED);
        else
            setfillstyle(SOLID_FILL,GREEN);
        floodfill(x[j]+1,y[j]+1, LIGHTGREEN);
        setcolor(YELLOW);
        outtextxy(x[j]+5,y[j]+8,intToString(V[j]));
        outtextxy(x[j]+5,y[j]+48,intToString(j));
    }
    afisText_pozitionare_bubble_sort(p);
    Sleep(waitTime);
    cleardevice();
}
void vect_intermediar_swap_bubble_sort(int p, int n, TIP V[NRMAX])
{
    int q=75;
    int x[NRMAX],y[NRMAX];
    for(int j=1; j<=n; j++)
    {
        x[j]=25+q*j;
        y[j]=200;

        setcolor(LIGHTGREEN);
        rectangle(x[j],y[j],x[j]+45,y[j]+35);
        if(j==p)
        {
            setfillstyle(SOLID_FILL,BLUE);
        }
        else if(j==p+1)
            setfillstyle(SOLID_FILL,BLUE);
        else
            setfillstyle(SOLID_FILL,GREEN);
        floodfill(x[j]+1,y[j]+1, LIGHTGREEN);
        setcolor(LIGHTBLUE);
        outtextxy(x[j]+5,y[j]+8,intToString(V[j]));
        outtextxy(x[j]+5,y[j]+48,intToString(j));
    }
    afisText_swap_bubble_sort(p);
    Sleep(waitTime);
    cleardevice();
}
void vect_ordonat_bubble_sort(int n, TIP V[NRMAX])
{
    int q=75;
    int x[NRMAX],y[NRMAX];
    for(int j=1; j<=n; j++)
    {
        x[j]=25+q*j;
        y[j]=200;

        setcolor(LIGHTGREEN);
        rectangle(x[j],y[j],x[j]+45,y[j]+35);
        setfillstyle(SOLID_FILL,GREEN);
        floodfill(x[j]+1,y[j]+1, LIGHTGREEN);
        setcolor(YELLOW);
        outtextxy(x[j]+5,y[j]+8,intToString(V[j]));
        outtextxy(x[j]+5,y[j]+48,intToString(j));
    }
    afisText_vect_ord_bubble_sort();
    while(!kbhit());
    setcurrentwindow(0);
    closegraph(inputWinId++);
}
void program_final_bubble_sort_(vect v, bool crescator)
{
    initwindow(1300,600,"Bubble Sort",0,0,false,false);
    int n = v.nrElemente;
    TIP V[NRMAX],V1[NRMAX];
    for(int i=1; i<=n; i++)
    {
        V[i] = v.elemente[i-1];
        V1[i]=V[i];
    }
    bool ok = false;
    while(!ok)
    {
        ok=true;
        for(int i=1; i<n; i++)
            if((V[i]>V[i+1] && crescator) || (V[i]<V[i+1] && !crescator))
            {
                vect_col_el_pt_swap_bubble_sort(i,n,V);
                swap(V[i],V[i+1]);
                ok=false;
                vect_intermediar_swap_bubble_sort(i,n,V);
            }
    }
    vect_ordonat_bubble_sort(n, V);
}

void afisText_VS_insertie(int x1)
{
    bgiout << "VECTOR SORTAT ";
    outstreamxy(x1-60, 100);
}
void afisText_VNS_insertie(int x1)
{
    bgiout << "VECTOR NESORTAT ";
    outstreamxy(x1+160, 100);
}
void perete_insertie(int x1)
{
    int y1=20;
    setcolor(LIGHTGREEN);
    rectangle(x1+55,y1,x1+65,y1+435);
    setfillstyle(SOLID_FILL,YELLOW);
    floodfill(x1+57,y1+6, LIGHTGREEN);
}
void afisText_vect_init_insertie()
{
    bgiout << "ACESTA E VECTORUL INITIAL  NESORTAT    ";
    outstreamxy(30, 400);
}
void afisText_vect_ord_insertie()
{
    bgiout << "ACESTA E VECTORUL SORTAT CU INSERTIE   ";
    outstreamxy(30, 400);
}
void vect_col1_insertie(TIP V[NRMAX], int p, int n)
{

    int q=75;
    int x1=0;
    int x[NRMAX],y[NRMAX];
    for(int j=1; j<=n; j++)
    {
        x[j]=25+q*j;
        y[j]=200;
        if(j<=p)
        {
            setcolor(LIGHTGREEN);
            rectangle(x[j],y[j],x[j]+45,y[j]+35);
            setfillstyle(SOLID_FILL,RED);
            floodfill(x[j]+1,y[j]+1, LIGHTGREEN);
            setcolor(YELLOW);
            outtextxy(x[j]+5,y[j]+8,intToString(V[j]));
            if(j==p)
                x1=x[j];
        }
        else
        {
            x[j]=25+q*j;
            setcolor(LIGHTGREEN);
            setfillstyle(SOLID_FILL, GREEN);
            fillellipse(x[j]+25,y[j]+10,25,25);

            setcolor(WHITE);
            outtextxy(x[j]+10,y[j]+5,intToString(V[j]));
        }
        outtextxy(x[j]+15,y[j]+58,intToString(j));
    }

    perete_insertie(x1);
    afisText_VS_insertie(x1);
    afisText_VNS_insertie(x1);
    delay(1500);
    //while(!kbhit());
    cleardevice();
}
void vect_col2_insertie(TIP V[NRMAX], int p, int n)
{
    int q=75;
    int x1 = 0;
    int x[NRMAX],y[NRMAX];
    for(int j=1; j<=n; j++)
    {
        x[j]=25+q*j;
        y[j]=200;
        if(j<=p)
        {
            setcolor(LIGHTGREEN);
            rectangle(x[j],y[j],x[j]+45,y[j]+35);
            setfillstyle(SOLID_FILL,RED);
            floodfill(x[j]+1,y[j]+1, LIGHTGREEN);
            setcolor(YELLOW);
            outtextxy(x[j]+5,y[j]+8,intToString(V[j]));
            if(j==p)
                x1=x[j];
        }
        else if(j==p+1)
            {
                setcolor(LIGHTGREEN);
                setfillstyle(SOLID_FILL, RED);
                fillellipse(x1-60,y[j]+150,25,25);
                setcolor(WHITE);
                outtextxy(x1-70,y[j]+154,intToString(V[j]));
                outtextxy(x1-70,y[j]+195,intToString(j));
            }
        else
        {
            x[j]=25+q*j;
            setcolor(LIGHTGREEN);
            setfillstyle(SOLID_FILL, GREEN);
            fillellipse(x[j]+25,y[j]+10,25,25);

            setcolor(WHITE);
            outtextxy(x[j]+10,y[j]+5,intToString(V[j]));
        }
        outtextxy(x[j]+15,y[j]+58,intToString(j));
    }

    perete_insertie(x1);
    afisText_VS_insertie(x1);
    afisText_VNS_insertie(x1);

    delay(waitTime);
    //while(!kbhit());
    cleardevice();
}
void vect_init_insertie(int n, TIP V[NRMAX])
{
    int q=75;
    int x[NRMAX],y[NRMAX];
    for(int j=1; j<=n; j++)
    {
        x[j]=25+q*j;
        y[j]=200;

        setcolor(LIGHTGREEN);
        rectangle(x[j],y[j],x[j]+45,y[j]+35);
        setfillstyle(SOLID_FILL,GREEN);
        floodfill(x[j]+1,y[j]+1, LIGHTGREEN);
        setcolor(YELLOW);
        outtextxy(x[j]+5,y[j]+8,intToString(V[j]));
        outtextxy(x[j]+5,y[j]+48,intToString(j));
    }
    afisText_vect_init_insertie();
    delay(waitTime);
    cleardevice();
}
void vect_final_insertie(int n, TIP V[NRMAX])
{
    int q=75;
    int x[NRMAX],y[NRMAX];
    for(int j=1; j<=n; j++)
    {
        x[j]=25+q*j;
        y[j]=200;

        setcolor(LIGHTGREEN);
        rectangle(x[j],y[j],x[j]+45,y[j]+35);
        setfillstyle(SOLID_FILL,GREEN);
        floodfill(x[j]+1,y[j]+1, LIGHTGREEN);
        setcolor(YELLOW);
        outtextxy(x[j]+5,y[j]+8,intToString(V[j]));
        outtextxy(x[j]+5,y[j]+48,intToString(j));
    }
    afisText_vect_ord_insertie();
    while(!kbhit());
    setcurrentwindow(0);
    closegraph(inputWinId++);
}
void program_final_sortare_prin_insertie(vect v, bool crescator)
{
    initwindow(1300,600,"Insertie",0,0,false,false);
    int n = v.nrElemente;
    TIP V[NRMAX];
    for(int i=1; i<=n; i++)
    {
        V[i] = v.elemente[i-1];
    }
    vect_init_insertie(n,V);
    int p=1;
    for(int i=2; i<=n; i++)  // parcurg vectorul nesortat de la a 2-lea element pana la sfarsit
    {
        vect_col1_insertie(V,p,n);
        vect_col2_insertie(V,p,n);
        p++;
        if ((V[i-1]>V[i] && crescator)||(V[i-1]<V[i] && !crescator))   // primul element din vectorul nesortat se plaseaza pe pozitia corespunzatoare
        {
            TIP aux=V[i];      // valoarea lui v[i] se pierde din vectorul nesortat
            int k=i-1;
            while (k>0 && ((V[k]>aux && crescator) || (V[k]<aux && !crescator))) // mut cu o pozitie spre dreapta toate elementele mai mari decat x=v[i]
            {
                V[k+1]=V[k];
                k--;
            }
            V[k+1]=aux; // insertia primului element pe pozitia corespunzatoare in vectorul sortat
        }

    }
    vect_final_insertie(n, V);
}

void afisText_pozitionare_interschimbare(int i, int j)
{
    bgiout << "ELEMENTELE   "<<i<<" si "<<j<<" ce trebuie INTERSCHIMBATE se coloreaza cu ROSU";
    outstreamxy(30, 400);
}
void afisText_swap_interschimbare(int i, int j)
{
    bgiout << "SE INTERSCHIMBA   "<<i <<" cu "<<j;
    outstreamxy(30, 400);
}
void afisText_vect_ord_interschimbare()
{
    bgiout << "ACESTA E VECTORUL SORTAT CU INTERSCHIMBARE ";
    outstreamxy(30, 400);
}
void vect_pozitionare_swap_interschimbare(int n, TIP V[NRMAX], int p, int l)
{
    //initwindow(1300,600,"Insertie",0,0,false,false);
    int q=75;
    int x[NRMAX],y[NRMAX];
    for(int i1=1; i1<=n; i1++)
    {
        x[i1]=25+q*i1;
        y[i1]=200;
        setcolor(LIGHTGREEN);
        rectangle(x[i1],y[i1],x[i1]+45,y[i1]+35);

        if(i1==p || i1==l)
        {
            setfillstyle(SOLID_FILL,RED);
            outtextxy(x[i1]+5,y[i1]+8,intToString(V[i1]));
            outtextxy(x[i1]+5,y[i1]+48,intToString(i1));
        }
        else
            setfillstyle(SOLID_FILL,GREEN);
        floodfill(x[i1]+1,y[i1]+1, LIGHTGREEN);
        setcolor(YELLOW);
        outtextxy(x[i1]+5,y[i1]+8,intToString(V[i1]));
        outtextxy(x[i1]+5,y[i1]+48,intToString(i1));
    }
    afisText_pozitionare_interschimbare(p,l);
    Sleep(waitTime);
    //setcurrentwindow(0);
    //closegraph(inputWinId++);
    cleardevice();
}
void vect_intermediar_swap_interschimbare(int n, TIP V[NRMAX], int p, int l)
{
    //initwindow(1300,600,"Insertie",0,0,false,false);
    int q=75;
    int x[NRMAX],y[NRMAX];
    for(int i1=1; i1<=n; i1++)
    {
        x[i1]=25+q*i1;
        y[i1]=200;
        setcolor(LIGHTGREEN);
        rectangle(x[i1],y[i1],x[i1]+45,y[i1]+35);
        if(i1==p || i1==l)
        {
            setfillstyle(SOLID_FILL,BLUE);
            outtextxy(x[i1]+5,y[i1]+8,intToString(V[i1]));
            outtextxy(x[i1]+5,y[i1]+48,intToString(i1));
        }
        else
            setfillstyle(SOLID_FILL,GREEN);
        floodfill(x[i1]+1,y[i1]+1, LIGHTGREEN);
        setcolor(LIGHTBLUE);
        outtextxy(x[i1]+5,y[i1]+8,intToString(V[i1]));
        outtextxy(x[i1]+5,y[i1]+48,intToString(i1));
    }

    afisText_swap_interschimbare(p,l);
    Sleep(waitTime);
    //setcurrentwindow(0);
    //closegraph(inputWinId++);
    cleardevice();
}
void vect_ordonat(int n, TIP V[NRMAX])
{
    //initwindow(1300,600,"Insertie",0,0,false,false);
    int q=75;
    int x[NRMAX],y[NRMAX];
    for(int i1=1; i1<=n; i1++)
    {
        x[i1]=25+q*i1;
        y[i1]=200;

        setcolor(LIGHTGREEN);
        rectangle(x[i1],y[i1],x[i1]+45,y[i1]+35);
        setfillstyle(SOLID_FILL,GREEN);
        floodfill(x[i1]+1,y[i1]+1, LIGHTGREEN);
        setcolor(YELLOW);
        outtextxy(x[i1]+5,y[i1]+8,intToString(V[i1]));
        outtextxy(x[i1]+5,y[i1]+48,intToString(i1));
    }
    afisText_vect_ord_interschimbare();
    while(!kbhit());
    setcurrentwindow(0);
    closegraph(inputWinId++);
}
void program_final_sortare_prin_interschimbare(vect v, bool crescator)
{
    initwindow(1300,600,"Insertie",0,0,false,false);
    int n = v.nrElemente;
    TIP V[NRMAX];
    for(int i=1; i<=n; i++)
    {
        V[i] = v.elemente[i-1];
    }
    for(int i=1; i<n; i++)
        for(int j=i+1; j<=n; j++)
            if((V[i]>V[j] && crescator) || (V[i]<V[j] && !crescator))
            {
                vect_pozitionare_swap_interschimbare(n, V, i, j);
                swap(V[i],V[j]);
                vect_intermediar_swap_interschimbare(n, V, i, j);
            }

    vect_ordonat(n,V);
}
