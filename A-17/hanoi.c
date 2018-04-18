#include <stdio.h>

void zeichneStange(int zuZeichnen);
void zeichne(int stange1[3], int stange2[3], int stange3[3]);
int lege(int stange1[3], int stange2[3], int stange3[3], int start, int ziel);
int pruefe(int stange1[3], int stange2[3], int stange3[3]);

int main(void)
{
    int stange1[3] = {1, 2, 3};
    int stange2[3] = {0, 0, 0};
    int stange3[3] = {0, 0, 0};
    int start = 0;
    int ziel = 0;
    char c;
    int geloest = 0;
    while(!geloest)
    {
        zeichne(stange1, stange2, stange3);

        printf("Bitte geben sie den Start ein: ");
        scanf("%d", &start);
        while ((c = getchar()) != EOF && c != '\n');
        printf("Bitte geben sie das Ziel ein: ");
        scanf("%d", &ziel);
        while ((c = getchar()) != EOF && c != '\n');
        printf("\n");

        if (!(start > 0 && start < 4 && ziel > 0 && ziel < 4))
        {
            printf("[ERROR]: Start und Ziel muessen zwischen 1 und 3 (inklusiv) liegen.\n\n");
            continue;
        }

        int fehler = lege(stange1, stange2, stange3, start, ziel);
        if (fehler)
        {
            printf("[ERROR]: Unmoeglicher Zug!\n\n");
            continue;
        }
        geloest = pruefe(stange1, stange2, stange3);
    }
    zeichne(stange1, stange2, stange3);
    printf("Sie haben gewonnen!\n");
    return 0;
}

void zeichneStange(int zuZeichnen)
{
    switch(zuZeichnen)
    {
        case 0:
        printf("  |  ");
        break;
        case 1:
        printf("  █  ");
        break;
        case 2:
        printf(" ███ ");
        break;
        case 3:
        printf("█████");
        break;
        default:
        printf("     ");
        break;
    }
}

void zeichne(int stange1[3], int stange2[3], int stange3[3])
{
    printf("   1     2     3\n");
    printf("   |     |     |\n");
    for (int i = 0; i < 3; i++)
    {
        printf(" ");
        zeichneStange(stange1[i]);
        printf(" ");
        zeichneStange(stange2[i]);
        printf(" ");
        zeichneStange(stange3[i]);
        printf("\n");
    }
    printf("\n");
}

int lege(int stange1[3], int stange2[3], int stange3[3], int start, int ziel)
{
    int *startstange = NULL;
    int *zielstange = NULL;
    int *startfeld = NULL;
    int *zielfeld = NULL;
    int zielwert = 0;
    if (start == ziel)
        return 0;
    switch (start)
    {
        case 1:
        startstange = stange1;
        break;
        case 2:
        startstange = stange2;
        break;
        case 3:
        startstange = stange3;
        break;
    }
    switch (ziel)
    {
        case 1:
        zielstange = stange1;
        break;
        case 2:
        zielstange = stange2;
        break;
        case 3:
        zielstange = stange3;
        break;
    }
    for (int i = 0; i < 3; i++)
    {
        if (startstange[i] != 0)
        {
            startfeld = &startstange[i];
            break;
        }
    }
    for (int i = 1; i < 3; i++)
    {
        if (zielstange[i] != 0)
        {
            zielwert = zielstange[i];
            zielfeld = &zielstange[i-1];
            break;
        }
    }
    if (startfeld == NULL)
        return 1;
    if (zielfeld == NULL)
    {
        zielfeld = &zielstange[2];
        zielwert = 4;
    }
    if (zielwert < *startfeld)
    {
        return 2;
    }
    *zielfeld = *startfeld;
    *startfeld = 0;
    return 0;
}

int pruefe(int stange1[3], int stange2[3], int stange3[3])
{
    for (int i = 0; i < 3; i++)
    {
        if (stange1[i] != 0)
            return 0;
        if (stange2[i] != 0)
            return 0;
        if (stange3[i] != i+1)
            return 0;
    }
    return 1;
}