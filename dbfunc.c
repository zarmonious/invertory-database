#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "dbfunc.h"

void initItems(Data_t *items, long len)
{
    for (long i = 0; i < len; i++)
    {
        *items[i].invNr = '\0' ;                 //zugr. wert auf diesem index
    }
}

void printItem(Data_t *item)
{
    printf(
        "Name:                 %s\nMarke:                %s\nInventar-Nr.:         %s\nAnschaffungsjahr:     %ld\n",
        item->name,
        item->brand,
        item->invNr,
        item->year
    );
}

void printDB(Data_t *items, long len)
{
    printf("Datenbankeintraege:\n");

    for (long i = 0; i < len; i++)
    {
        if ( strlen(items[i].invNr) != 0 )
        {
            printf("---------------Eintrag %ld---------------\n", i+1);
            printItem(&items[i]);
        }
    }

}

long readValue(char *name)
{
    long zahl;
    int tempzahl;

    printf("%s: ", name);

    while (1)
    {
        tempzahl = scanf("%ld", &zahl);
        while (getchar() != '\n') {;}

        if (tempzahl == 1)
        {
            break;
        }
        else
        {
            fprintf(stderr, "%s ist ungueltig!Bitte noch mal eingeben: ", name);

        }
    }

    return zahl;
}

void readString(char *name, char *text, long len)       //teilweise Ref04 Bsp
{
    char c, *textend = text + len - 1, *texttemp = text;

    printf("%s: ", name);

    if (len > 0 && text)
    {
        while (1)
        {
            c = getchar();
            if (c == '\n')
            {
                *texttemp = 0;
                break;
            }
            else if (texttemp < textend)
            {
                *texttemp = c;
                texttemp = texttemp + 1;
            }
            else
            {
                printf("Zeichenkette ist zu lang! Bitte nochmal eingeben:\n");
                texttemp = text;
                while (getchar() != '\n') {;}
            }
        }
    }
}

long addItem(Data_t *items, long len)
{
    for (int i = 0; i < len; i++)
    {
        if ( strlen(items[i].invNr) == 0 )                      //items i целая коробка
        {
            readString("Name", items[i].name, TEXT_LEN);
            readString("Marke", items[i].brand, TEXT_LEN);
            readString("Inventar-Nr.", items[i].invNr, TEXT_LEN);
            if ( strlen(items[i].invNr) == 0 )
            {
                fprintf( stderr, "Fehler. Inventar-Nr. ist leer.\n");
                return -1;
            }
            items[i].year = readValue("Anschaffungsjahr");

            return 0;
        }
    }

    fprintf( stderr, "Fehler! Keine freien Eintraege mehr verhanden.\n");
    return -1;
}

long deleteItem(Data_t *items, long len)
{
    long index;

    index = readValue("Welchen Eintrag loeschen");

    if ( index >= 1 && index <= len && strlen(items[index-1].invNr) != 0 )
    {
        items[index-1].invNr[0] = '\0';
        return 0;
    }
    else
    {
        fprintf( stderr, "Fehler! Element ist bereits leer oder der gewaehlte Index ist ungueltig.\n");
        return -1;
    }
}

long saveDB(Data_t *items, long len, char *outputFile)
{
    FILE *datei;

    if ( (datei = fopen( outputFile, "w")) )
    {
        fprintf( datei, "#Name\t#Marke\t#Inventar-Nr.\t#Jahr\n");
        for (int i = 0; i < len; i++)
        {
            if ( strlen(items[i].invNr) != 0 )
            {
                fprintf( datei, "%s\t%s\t%s\t%ld\n",
                         items[i].name,
                         items[i].brand,
                         items[i].invNr,
                         items[i].year
                       );
            }
        }
        fclose(datei);
        return 0;
    }
    else
    {
        fprintf( stderr, "Fehler beim Oeffnen der Datei!\n");
        return -1;
    }
}

long loadDB(Data_t *items, long len, char *inputFile)
{
    FILE *datei;
    int values;
    char dummy[255];

    initItems(items, len);                      // clear alle einträge

    if ( (datei = fopen( inputFile, "r")) )
    {
        fgets(dummy, 255, datei);               // erste zeile lesen und ignorieren

        for ( int i = 0; i < len; i++)
        {
            values = fscanf(
                         datei,
                         "%s\t%s\t%s\t%ld\n",
                         items[i].name,
                         items[i].brand,
                         items[i].invNr,
                         &items[i].year );


            if (values != 4)
            {

                if (feof(datei))                   // EOF -> leave
                {
                    fclose(datei);
                    return 0;
                }
                else
                {
                    fprintf( stderr, "Fehler beim Lesen der Datei!\n");
                    fclose(datei);
                    return -1;
                }
            }
        }
        return 0;
    }
    else
    {
        fprintf( stderr, "Fehler beim Oeffnen der Datei!\n");
        return -1;
    }

}

void caseToLower(char *str, long len)
{
    for(int i = 0; i < len; i++)
    {
        str[i] = tolower(str[i]);
    }
}

long searchFile(char *inputFile, char *key, long ignoreCase)
{
    FILE *datei;
    int values;
    char dummy[255];
    long i = 0, match = 0;
    char jahr[5];
    char keyOutput[TEXT_LEN];

    Data_t fileItemCompare;
    Data_t fileItemOutput;

    strcpy(keyOutput, key);

    if ( (datei = fopen( inputFile, "r")) )
    {
        fgets(dummy, 255, datei);               // erste zeile lesen und ignorieren

        while ( !feof(datei) )
        {
            i++;

            values = fscanf(
                         datei,
                         "%s\t%s\t%s\t%ld\n",
                         fileItemCompare.name,
                         fileItemCompare.brand,
                         fileItemCompare.invNr,
                         &fileItemCompare.year );


            if (values != 4)                       // Fehlerfälle
            {

                if (feof(datei))                   // EOF -> leave
                {
                    fclose(datei);
                    return 0;
                }
                else
                {
                    fprintf( stderr, "Fehler beim Lesen der Datei!\n");
                    fclose(datei);
                    return -1;
                }
            }
            else
            {
                sprintf(jahr, "%ld", fileItemCompare.year);                // converting long to string

                strcpy(fileItemOutput.name, fileItemCompare.name);
                strcpy(fileItemOutput.brand, fileItemCompare.brand);
                strcpy(fileItemOutput.invNr, fileItemCompare.invNr);
                fileItemOutput.year = fileItemCompare.year;

                if ( ignoreCase != 0)                                      // Case ja/nein
                {
                    caseToLower(fileItemCompare.name, TEXT_LEN);
                    caseToLower(fileItemCompare.brand, TEXT_LEN);
                    caseToLower(fileItemCompare.invNr, TEXT_LEN);
                    caseToLower(key, TEXT_LEN);
                }

                if( strstr(fileItemCompare.name, key) || strstr(fileItemCompare.brand, key) || strstr(fileItemCompare.invNr, key) || strstr(jahr, key) )
                {
                    printf(
                        "%s Zeile: %ld, %s %s %s %ld\n",
                        inputFile,
                        i,
                        fileItemOutput.name,
                        fileItemOutput.brand,
                        fileItemOutput.invNr,
                        fileItemOutput.year );

                    match++;
                }
            }
        }

        if ( match == 0)
        {
            fprintf( stderr, "Suche nach \"%s\" war ergebnisslos.\n", keyOutput);
        }

        fclose(datei);
        return 0;
    }
    else
    {
        fprintf( stderr, "Fehler beim Oeffnen der Datei!\n");
        return -1;
    }
}

long sortItems(Data_t *items, long len, long which)
{
    Data_t h;

    if (which == 2)            // 2 für das Anschaffungsjahr; (BubbleSort aus dem Buch)
    {
        for ( long j = len - 1; j > 0; j = j - 1 )
        {
            for ( long i = 0; i < j; i = i + 1 )
            {
                if ( items[i].year > items[i + 1].year )
                {
                    // Tausche
                    h = items[i];
                    items[i] = items[i + 1];
                    items[i + 1] = h;

                }
            }
        }
        return 0;
    }

    else if (which == 1)                       // für die Inventar-Nr.; (Minimumsuche)
    {
        long min, len1;
        len1= len - 1;

        for ( int j = 0; j < len1; j = j + 1 )
        {
            // Suche das Minimum
            min = j;
            for ( int i = j + 1; i < len; i = i + 1 )
            {
                if ( strcmp(items[i].invNr, items[min].invNr) < 0 )
                {
                    min = i;
                }
            }
            // Tausche
            if (min != j)
            {
                h = items[j];
                items[j] = items[min];
                items[min] = h;
            }
        }
        return 0;
    }
    else
    {
        return -1;
    }
}

long addFromFile(Data_t *items, long len, char *inputFile, char *key)
{
    FILE *datei;
    int values;
    char dummy[255];
    long i = 0, match = 0;
    char jahr[5];
    int dublicateFound = 0, freeSpaceFound = 0;

    Data_t fileItemCompare;

    if ( (datei = fopen( inputFile, "r")) )
    {
        fgets(dummy, 255, datei);               // erste zeile lesen und ignorieren

        while ( !feof(datei) )
        {
            i++;

            values = fscanf(
                         datei,
                         "%s\t%s\t%s\t%ld\n",
                         fileItemCompare.name,
                         fileItemCompare.brand,
                         fileItemCompare.invNr,
                         &fileItemCompare.year );


            if (values != 4)                       // Fehlerfälle
            {

                if (feof(datei))                   // EOF -> leave
                {
                    fclose(datei);
                    return 0;
                }
                else
                {
                    fprintf( stderr, "Fehler beim Lesen der Datei!\n");
                    fclose(datei);
                    return -1;
                }
            }
            else
            {
                sprintf(jahr, "%ld", fileItemCompare.year);                // converting long to string

                if( strstr(fileItemCompare.name, key) || strstr(fileItemCompare.brand, key) || strstr(fileItemCompare.invNr, key) || strstr(jahr, key) )
                {
                    match++;

                    printf(
                        "%s Zeile: %ld, %s %s %s %ld\n",
                        inputFile,
                        i,
                        fileItemCompare.name,
                        fileItemCompare.brand,
                        fileItemCompare.invNr,
                        fileItemCompare.year );

                    dublicateFound = 0;
                    freeSpaceFound = 0;

                    for ( int i = 0; i < len; i++)                  // check for dublicate
                    {
                        if ( strcmp(items[i].invNr, fileItemCompare.invNr) == 0 )
                        {
                            dublicateFound =  1;
                            break;
                        }
                    }

                    if ( dublicateFound == 1)
                    {
                        fprintf( stderr, "Gefundener Datensatz bereits vorhanden.\n");
                        continue;
                    }

                    for ( int i = 0; i < len; i++)                  // check for free space
                    {
                        if ( strlen(items[i].invNr) == 0 )
                        {
                            strcpy(items[i].name, fileItemCompare.name);
                            strcpy(items[i].brand, fileItemCompare.brand);
                            strcpy(items[i].invNr, fileItemCompare.invNr);
                            items[i].year = fileItemCompare.year;

                            freeSpaceFound = 1;
                            break;
                        }
                    }

                    if ( freeSpaceFound == 0)
                    {
                         fprintf( stderr, "Konnte keine freie Platz finden, um gefundene Datensatz aus Datei zu laden.\n");
                    }
                    else
                    {
                        printf("Gefundene Datensatz wurde erfolgreich geladen.\n");
                    }
                }
            }
        }

        if ( match == 0)
        {
            fprintf( stderr, "Suche nach \"%s\" war ergebnisslos.\n", key);
        }

        fclose(datei);
        return 0;
    }
    else
    {
        fprintf( stderr, "Fehler beim Oeffnen der Datei!\n");
        return -1;
    }
}

