#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbfunc.h"

int main(int argc, char **argv)
{
    long wahl, whichWahl;
    long caseIns = 0;
    char key[TEXT_LEN];
    Data_t items[DATABASE_SIZE] = {0};

    if (argc >= 2)
    {
        if ( strcmp(argv[1], "-i") == 0 )
        {
            caseIns = 1;

            if (argc > 3)
            {
                for (int i = 3; i < argc; i++)
                {
                    searchFile(argv[i], argv[2], caseIns);
                }
                return 0;
            }
        }
        else if (argc > 2)
        {
            for (int i = 2; i < argc; i++)
            {
                searchFile(argv[i], argv[1], caseIns);
            }
            return 0;
        }
    }

    do
    {
        printf("Menue:\n");
        printf("1 - Show all entries\n");
        printf("2 - Add a new entry\n");
        printf("3 - Delete an entry\n");
        printf("4 - Save in a file\n");
        printf("5 - Load a file\n");
        printf("6 - Search in a file\n");
        printf("7 - Sort entries\n");
        printf("8 - Add entries from file\n");
        printf("0 - Programm beenden\n");
        printf("\n");
        wahl = readValue("Your choice");
        printf("\n");

        switch (wahl)
        {
        case 1:
            printDB(items, DATABASE_SIZE);
            break;

        case 2:
            addItem(items, DATABASE_SIZE);
            break;

        case 3:
            deleteItem(items, DATABASE_SIZE);
            break;

        case 4:
            saveDB(items, DATABASE_SIZE, "outputFile.txt");
            break;

        case 5:
            if ( loadDB(items, DATABASE_SIZE, "outputFile.txt") == 0 )
            {
                printf("Database was loaded successfully from file.\n");
            }
            break;

        case 6:
            readString("Searchword", key, TEXT_LEN);
            searchFile("outputFile.txt", key, caseIns);
            break;

        case 7:
            whichWahl = readValue("Sort by what (1 - Inventory-Nr., 2 - Acquisition-year)");
            sortItems(items, DATABASE_SIZE, whichWahl);
            break;

        case 8:
            readString("Searchword", key, TEXT_LEN);
            addFromFile(items, DATABASE_SIZE, "outputFile.txt", key);
            break;

        case 0:
            return 0;

        default:
            fprintf( stderr, "Error. Invalid Input. Try again.\n");
        }

        printf("\n");
    }
    while (1);

    return 0;
}
