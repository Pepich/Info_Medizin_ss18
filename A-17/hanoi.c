#include <stdio.h>
#include <stdlib.h>

void zeichneStange(int zuZeichnen);
void zeichne(int stange1[3], int stange2[3], int stange3[3]);
int lege(int stange1[3], int stange2[3], int stange3[3], int start, int ziel);
int pruefe(int stange1[3], int stange2[3]);

// Hauptroutine des Programms (Einstiegspunkt)
int main(void)
{
    // Arrays um Zustand des Spielfeldes (der Stangen) zu speichern.
    // Index 0 = oben, Index 2 = Unten
    // Wert 0 = Leeres Feld, Wert 1 = kleinste Scheibe, ..., Wert 3 = groesste Scheibe
    int stange1[3] = {1, 2, 3};
    int stange2[3] = {0, 0, 0};
    int stange3[3] = {0, 0, 0};
    // Variablen zum Zwischenspeichern der Benutzereingaben
    int start = 0;
    int ziel = 0;
    // Variable zum Leeren des Eingabepuffers
    char c;
    // Rueckgabevariablen der Funktionen lege und pruefe
    int fehler = 0;
    int geloest = 0;

    // Fuehre das Spiel so lange aus, bis es geloest wurde.
    while(!geloest)
    {
        // Zu Beginn eines jeden Schleifendurchlaufes wird das Spielfeld gezeichnet.
        // So hat der Benutzer vor jeder Eingabe erneut das Spielfeld vor Augen
        zeichne(stange1, stange2, stange3);

        // Benutzer nach Start-/Zielstange fragen
        printf("Bitte geben sie den Start ein: ");
        scanf("%d", &start);
        while ((c = getchar()) != EOF && c != '\n');
        printf("Bitte geben sie das Ziel ein: ");
        scanf("%d", &ziel);
        while ((c = getchar()) != EOF && c != '\n');
        printf("\n");

        if (c == EOF)
        {
            // Programmabbruch, da keine Eingaben mehr zur Verfuegung stehen
            printf("Programmabbruch!\n\n");
            exit(0);
        }

        // Eingaben auf gueltigkeit ueberpruefen
        if (!(start > 0 && start < 4 && ziel > 0 && ziel < 4))
        {
            // Fehler ausgeben, und Schleifendurchlauf erneut starten
            printf("[ERROR]: Start und Ziel muessen zwischen 1 und 3 (inklusiv) liegen.\n\n");
            continue;
        }

        // Versuche den Zug auszufuehren
        fehler = lege(stange1, stange2, stange3, start, ziel);
        if (fehler == 1)
        {
            printf("[ERROR]: Unmoeglicher Zug: Die Startstange ist leer!\n\n");
            continue;
        }
        else if (fehler == 2)
        {
            printf("[ERROR]: Unmoeglicher Zug: Die Zielstange hat bereits eine kleinere Scheibe!\n\n");
            continue;
        }
        // Teste, ob das Spiel geloest ist.
        geloest = pruefe(stange1, stange2);
    }
    // Zeichne zum Abschluss noch einmal das Spielfeld, und teile dem Benutzer mit, dass das Spiel geloest wurde.
    zeichne(stange1, stange2, stange3);
    printf("Sie haben gewonnen!\n");
    return 0;
}

/** void zeichneStange(int zuZeichnen)
 * Diese Funktion zeichnet genau einen Abschnitt einer Stange. Der Parameter "zuZeichnen" gibt hierbei an, um welche Art von Abschnitt es sich handelt:
 * 0: leere Stange
 * 1: kleinste Scheibe
 * 2: mittlere Scheibe
 * 3: groesste Scheibe
 * 
 * Sollte ein Wert ausserhalb des Bereiches uebergeben werden, so wird ein komplett leerer Abschnitt gezeichnet (5x Leerzeichen).
 * 
 * Jede Stange hat eine groesse von 5 Zeichen, bestehend aus Leerzeichen, Stange/Scheibe, Leerzeichen. Es wird gleichmaessig mit Leerzeichen aufgefuellt,
 * sodass die Breite IMMER 5 betraegt.
 * */
void zeichneStange(int zuZeichnen)
{
    switch(zuZeichnen)
    {
        // Leere Stange
        case 0:
        printf("  |  ");
        break;

        // Kleinste Scheibe
        case 1:
        printf("  █  ");
        break;

        // Mittlere Scheibe
        case 2:
        printf(" ███ ");
        break;

        // Groesste Scheibe
        case 3:
        printf("█████");
        break;

        // Fehler...?
        default:
        printf("     ");
        break;
    }
}

/** void zeichne(int stange1[3], int stange2[3], int stange3[3])
 * Diese Funktion zeichnet das gegebene Spielfeld.
 * */
void zeichne(int stange1[3], int stange2[3], int stange3[3])
{
    // "Kopf" des Spielfeldes, bestehend aus Index der Stangen (1-3), und einer leeren Reihe.
    printf("   1     2     3\n");
    printf("   |     |     |\n");
    // Zeichnen der einzelnen Stangenabschnitte, i entspricht dem Stangenabschnitt 0-2 (von oben nach unten).
    for (int i = 0; i < 3; i++)
    {
        // Abstandshalter
        printf(" ");
        zeichneStange(stange1[i]);
        // Abstandshalter
        printf(" ");
        zeichneStange(stange2[i]);
        // Abstandshalter
        printf(" ");
        zeichneStange(stange3[i]);
        // Zeilenumbruch, da alle Stangenabschnitte fuer diese Zeile ausgegeben wurden
        printf("\n");
    }
    // Leerzeile (sieht besser aus)
    printf("\n");
}

/** int lege(int stange1[3], int stange2[3], int stange3[3], int start, int ziel)
 * Diese Funktion fuehrt einen Spielzug auf dem gegebenen Spielfeld aus. Die Parameter "start" und "ziel" beschreiben hierbei jeweils den Index der Stange.
 * Diese Funktion gibt 0 zurueck, wenn ein legaler Spielzug ausgefuehrt wurde.
 * Rueckgabewert 1 bedeutet, dass die "Startstange" keinen Spielstein enthaelt (also leer ist).
 * Rueckgabewert 2 bedeutet, dass die Scheibe auf der Zielstange kleiner ist, als die oberste Scheibe der Startstange.
 * */
int lege(int stange1[3], int stange2[3], int stange3[3], int start, int ziel)
{
    // Zeiger auf Start-/Zielstange, macht den spaeteren Programmcode einfacher
    int *startstange = NULL;
    int *zielstange = NULL;
    // Zeiger auf die Felder, die durch den Zug betroffen werden
    int *startfeld = NULL;
    int *zielfeld = NULL;
    // Wert der Scheibe, die sich in der Zielposition befindet. Wenn die Stange leer ist, so ist dieser Wert groesser als die groesste Scheibe (also 4)
    int zielwert = 4;

    // Belege die Zeiger fuer Start-/Zielstange abhaengig von den Parametern start und ziel.
    // Dies wird hier gemacht, damit nicht bei jedem Zugriff erneut die Entscheidung getroffen werden muss, sonder stattdessen einfach die Variable verwendet werden kann.
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

    // Finde die erste Scheibe (von oben) auf der Startstange
    for (int i = 0; i < 3; i++)
    {
        // Wenn der Wert an der Stelle i nicht 0 ist, so befindet sich dort eine Scheibe.
        if (startstange[i] != 0)
        {
            // Position der Scheibe wird in Variable "startfeld" gespeichert
            startfeld = &startstange[i];
            // Abbruch der Schleife
            break;
        }
    }
    
    // Finde die erste Scheibe (von oben) auf der Zielstange
    for (int i = 1; i < 3; i++)
    {
        // Wenn der Wert an der Stelle i nicht 0 ist, so befindet sich dort eine Scheibe.
        if (zielstange[i] != 0)
        {
            // Speichere den Wert der Scheibe ab
            zielwert = zielstange[i];
            // Speichere die Position UEBER der Scheibe als Zielfeld ab.
            // Da die Scheibe AUF die bestehende Scheibe gelegt werden soll, und kein Tausch stattfindet, muss sich das Zielfeld UEBER der Scheibe befinden.
            zielfeld = &zielstange[i-1];
            // Abbruch der Schleife
            break;
        }
    }
    // Wenn die Zielstange leer ist, gib Fehlercode 1 zurueck.
    if (startfeld == NULL)
        return 1;
    // Die Zielstange ist leer, setze das Zielfeld also auf das unterste Element der Zielstange
    if (zielfeld == NULL)
        zielfeld = &zielstange[2];
    // Wenn die Scheibe auf der Zielstange kleiner ist als die Scheibe auf der Startstange, gib Fehlercode 2 zurueck.
    if (zielwert < *startfeld)
        return 2;
    // Wenn Start und Ziel identisch sind, so ist der Zug legal, aber wir muessen nichts tauschen.
    if (start == ziel)
        return 0;
    // Weise dem Zielfeld den Wert des Startfeldes zu
    *zielfeld = *startfeld;
    // Setze Startfeld auf 0
    *startfeld = 0;
    // Zug wurde ausgefuehrt, gib 0 als "kein Fehler" zurueck.
    return 0;
}

/** int pruefe(int stange1[3], int stange2[3])
 * Diese Funktion prueft, ob das Spiel geloest wurde. Eine gueltige Loesung ist, wenn alle Scheiben sich auf Stange 3 befinden.
 * Diese Funktion gibt 0 zurueck, wenn das Spiel nicht geloest wurde.
 * Diese Funktion gibt 1 zurueck, wenn das Spiel geloest wurde.
 * */
int pruefe(int stange1[3], int stange2[3])
{
    // Wenn sich auf der untersten Position von stange1, sowie auf der untersten Position von stange2 keine Scheibe befindet, so muessen alle Scheiben auf stange3 sein.
    return (stange1[2] == 0 && stange2[2] == 0);
}