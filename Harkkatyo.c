// Project from 2021
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// käytetty apuna mittaamaan suoritusaikaa/used to help measure performance time https://stackoverflow.com/questions/5248915/execution-time-of-c-program
#include <time.h>

// Linkitetyn listan luonti/Creating a linked list.
typedef struct puusolmu_t
{
    int luku;
    short tila; /* Tasapainon tarkistus/Balance check*/
    struct puusolmu_t *vasen, *oikea;
} puusolmu, *puuosoitin;

// Kutsuttavat metodit/ Methods
void lisaa_solmu(puuosoitin *, int, int *);
void oikea_kierto(puuosoitin *, int *);
void tulosta_puu(puuosoitin, int);
void vasen_kierto(puuosoitin *, int *);
void tulosta(puuosoitin);
int menu();
void etsi(puuosoitin, int);

// Pääohjelma / Main programme
int main()
{
    int etp = 0, i, r = 0, valinta, x, y;
    char tiedosto[30];
    puuosoitin puu = NULL;
    printf("Anna luettavan tiedoston nimi:");
    scanf("%s", &tiedosto);
    clock_t tic = clock();
    // Luetaan tiedosto /Read the file
    FILE *fr = fopen(tiedosto, "r");
    while (fscanf(fr, "%d", &y) != EOF)
    {
        lisaa_solmu(&puu, y, &etp);
        // tulosta_puu(puu,0)/print_tree(puu,0);
    }
    fclose(fr);
    printf("\n");

    tulosta(puu);
    printf("\n");

    tulosta_puu(puu, 0);
    printf("\n");
    clock_t toc = clock();
    printf("kesti: %f sekuntteja\n", (double)(toc - tic) / CLOCKS_PER_SEC);

    // Kysytään käyttäjältä mitä hän haluaa tehdä /Ask the user what they want to do
    do
    {
        valinta = menu();
        switch (valinta)
        {
        case 1:
            tulosta_puu(puu, 0);
            break;
        case 2:
            printf("Mita lukua etsit: ");
            scanf("%d", &x);
            etsi(puu, x);
            break;
        case 3:
            printf("Anna luku: ");
            scanf("%d", &i);
            lisaa_solmu(&puu, i, &etp); // Käytetään valmista metodia / Using a ready-made method
            tulosta_puu(puu, 0);
            break;
        case 0:
            break;
        default:
            printf("Ei ole valinta\n");
            break;
        }
    } while (valinta != 0);
    printf("Kiitos ohjelman kaytosta");
    return 0;
}

// Käyttäjälle tarjottavat vaihtoehdot / Options available to the user
int menu()
{
    int valinta;
    printf("1) Tulosta puu\n");
    printf("2) Etsi Puusta avain\n");
    printf("3) Lisaa yksittainen avain puuhu\n");
    printf("0) Lopeta\n");
    printf("Anna valintasi: ");
    scanf("%d", &valinta);
    return valinta;
}

// Metodi kysytyn avaimen etsimiseen / Method to search for the requested key
// käytetty avuksi / used to help https://www.tutorialspoint.com/data_structures_algorithms/binary_search_tree.htm
void etsi(puuosoitin solmu, int i)
{
    while (1 == 1)
    {
        if (solmu == NULL)
        {
            printf("avainta ei loydy\n");
            break;
        }
        if (solmu->luku == i)
        {
            printf("avain loytyy puusta\n");
            break;
        }
        if (solmu->luku > i)
        {
            solmu = solmu->vasen;
        }
        else
        {
            solmu = solmu->oikea;
        }
    }
}
// Metodi jolla voidaan printata ja tarkistaa puun tasapaino / A method to print and check the balance of a tree
// Kertoo myös millä puolella puuta avain sijaitsee / Also tells you on which side of the tree the key is located
void tulosta(puuosoitin solmu)
{
    if (!solmu)
        return;

    printf("%d(%d) ", solmu->luku, solmu->tila);

    if (solmu->vasen != NULL)
    {
        printf("vas ");
        tulosta(solmu->vasen);
    }

    if (solmu->oikea != NULL)
    {
        printf("oik ");
        tulosta(solmu->oikea);
    }
}

// Metodi solmun lisäykselle puuhun / Method for adding a node to a tree
void lisaa_solmu(puuosoitin *emo, int luku, int *etp)
{
    if (!(*emo))
    {
        *etp = 1;
        if (!(*emo = (puuosoitin)malloc(sizeof(puusolmu))))
        {
            perror("malloc");
            exit(1);
        }
        (*emo)->vasen = (*emo)->oikea = NULL;
        (*emo)->tila = 0;
        (*emo)->luku = luku;
    }
    // Jos lisättävä luku on pienempi kuin se luku mihin verrataan sijoitetaan se vasemalle puolelle puuta / If the number to be added is smaller than the number to be compared, place it to the left of the tree
    // Tarkistetaan myös mikä on vasemman puolen tasapaino tilanne. Jos tasapaino on 1 niin tehdään kierto ja tasapainotetaan puu oikein. / Also check what the balance on the left side is. If the balance is 1, then rotate and balance the tree correctly.
    else if (luku < (*emo)->luku)
    {
        lisaa_solmu(&(*emo)->vasen, luku, etp);
        if (*etp)
        {
            switch ((*emo)->tila)
            {
            case -1:
                (*emo)->tila = 0;
                *etp = 0;
                break;
            case 0:
                (*emo)->tila = 1;
                break;
            case 1:
                vasen_kierto(emo, etp);
            }
        }
    }
    // Jos lisättävä luku on suurempi kuin se luku mihin verrataan sijoitetaan se oikealle puolelle puuta / If the number to be added is greater than the number to be compared, place it to the right of the tree
    // Tarkistetaan myös mikä on oikean puolen tasapaino tilanne. Jos tasapaino on -1 niin tehdään kierto ja tasapainotetaan puu oikein. / Also check what the balance on the right side is. If the balance is -1, make a rotation and balance the tree correctly.
    else if (luku > (*emo)->luku)
    {
        lisaa_solmu(&(*emo)->oikea, luku, etp);
        if (*etp)
        {
            switch ((*emo)->tila)
            {
            case 1:
                (*emo)->tila = 0;
                *etp = 0;
                break;
            case 0:
                (*emo)->tila = -1;
                break;
            case -1:
                oikea_kierto(emo, etp);
            }
        }
    }
    else
    {
        // Jos luku on jo puussa niin ilmoitetaan siitä käyttjälle eikä sitä lisätä uudestaan puuhun
        *etp = 0;
        printf("Luku %d on jo puussa\n", luku);
    }
}

// Metodi puun tulostamiseen vaakatasossa, niin että käyttäjällä on visaallinen kuva puun muodostumisesta / If the number is already in the tree, the user is informed and it is not added to the tree again.
// käytetty apuna / used to help https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
void tulosta_puu(puuosoitin solmu, int vali)
{
    int i, lasku = 10;
    if (!solmu)
        return;

    vali += lasku;

    tulosta_puu(solmu->oikea, vali);
    printf("\n");
    for (i = lasku; i < vali; i++)
        printf(" ");
    printf("%d\n", solmu->luku);

    tulosta_puu(solmu->vasen, vali);
    return;
}

// Metodi vasempaan kiertoon / Method for left rotation
void vasen_kierto(puuosoitin *emo, int *etp)
{
    puuosoitin lapsi, lapsenlapsi;

    lapsi = (*emo)->vasen;
    if (lapsi->tila == 1)
    {
        printf("\nennen: \n");
        tulosta(*emo);
        // Tulostetaan käyttäjälle mihin kohtaan puuta tehdään kierto / Print to the user where to rotate the tree
        printf("\n LL kierto %d\n", lapsi->luku);
        (*emo)->vasen = lapsi->oikea;
        lapsi->oikea = *emo;
        (*emo)->tila = 0;
        (*emo) = lapsi;
    }
    else
    {
        printf("\nennen:\n");
        tulosta(*emo);
        printf("\nLR kierto %d\n", lapsi->luku);
        lapsenlapsi = lapsi->oikea;
        lapsi->oikea = lapsenlapsi->vasen;
        lapsenlapsi->vasen = lapsi;
        (*emo)->vasen = lapsenlapsi->oikea;
        lapsenlapsi->oikea = *emo;
        switch (lapsenlapsi->tila)
        {
        case 1:
            (*emo)->tila = -1;
            lapsi->tila = 0;
            break;
        case 0:
            (*emo)->tila = lapsi->tila = 0;
            break;
        case -1:
            (*emo)->tila = 0;
            lapsi->tila = 1;
            break;
        }
        *emo = lapsenlapsi;
    }
    (*emo)->tila = 0;
    *etp = 0;
}
// Metodi puun oikeanpuoleiseen kiertoon / Method for right-hand tree rotation
void oikea_kierto(puuosoitin *emo, int *etp)
{
    puuosoitin lapsi, lapsenlapsi;

    lapsi = (*emo)->oikea;
    if (lapsi->tila == -1)
    {
        printf("\nennen: \n");
        tulosta(*emo);
        // Tulostetaan käyttäjälle mihin kohtaan puuta tehdään kierto / Print to the user where to rotate the tree
        printf("\nRR kierto  %d\n", lapsi->luku);
        (*emo)->oikea = lapsi->vasen;
        lapsi->vasen = *emo;
        (*emo)->tila = 0;
        (*emo) = lapsi;
    }
    else
    {
        printf("\nennen: \n");
        tulosta(*emo);
        printf("\nRL kierto  %d\n", lapsi->luku);
        lapsenlapsi = lapsi->vasen;
        lapsi->vasen = lapsenlapsi->oikea;
        lapsenlapsi->oikea = lapsi;
        (*emo)->oikea = lapsenlapsi->vasen;
        lapsenlapsi->vasen = *emo;
        switch (lapsenlapsi->tila)
        {
        case -1:
            (*emo)->tila = 1;
            lapsi->tila = 0;
            break;
        case 0:
            (*emo)->tila = lapsi->tila = 0;
            break;
        case 1:
            (*emo)->tila = 0;
            lapsi->tila = -1;
        }
        *emo = lapsenlapsi;
    }
    (*emo)->tila = 0;
    *etp = 0;
}
