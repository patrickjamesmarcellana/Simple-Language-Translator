/**
 * @file MarcellanaP_MiderB.c
 * @authors Patrick James T. Marcellana (S22A) and Brett Harley A. Mider (S22A)
 * @brief
 *    This program is a Machine Translator that stores and manages data about
 *    language-translation pairs. It allows a user to utilize a certain number
 *    of features.
 * @version 1.0
 * @date 2022-06-12
 * @copyright Copyright (c) 2022
 *
 */

/**************************************************************************************************
This is to certify that this project is our own work, based on our personal
efforts in studying and applying the concepts learned. We have constructed the
functions and their respective algorithms and corresponding code by ourselves.
The program was run, tested, and debugged by our own efforts. We further certify
that we have not copied in part or whole or otherwise plagiarized the work of
other students and/or persons.

Patrick James T. Marcellana, DLSU ID# 12134457
Brett Harley A. Mider, DLSU ID# 12117601
**************************************************************************************************/

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES  150
#define MAX_COUNT    10
#define MIN_LANG_LEN 1
#define MAX_LANG_LEN 20
#define MIN_TL_LEN   1
#define MAX_TL_LEN   20
#define MIN_FILE_LEN 5
#define MAX_FILE_LEN 30
#define MAX_TEXT_LEN 150

#define STR20LEN  21
#define STR30LEN  31
#define STR35LEN  36
#define STR150LEN 151
#define STR175LEN 176

#define CONFIRM_YES 'y'
#define CONFIRM_NO  'n'

#define ENTRY_OPTION_NEXT 'N'
#define ENTRY_OPTION_PREV 'P'
#define ENTRY_OPTION_EXIT 'X'

// for language and translation strings and the text to be translated
typedef char String20[STR20LEN];
typedef char String30[STR30LEN];
typedef char String35[STR35LEN];
typedef char String150[STR150LEN];
typedef char String175[STR175LEN];

// for each entry in the machine translator
typedef struct entry
{
    String20 lang[MAX_COUNT];
    String20 trans[MAX_COUNT];
    int count;
} Entry;

/**
 * @brief
 *    Removes the first instance of a newline character in a string.
 *    If there is no newline character, it does (effectively) nothing.
 *
 * @details
 *    The function strcspn() was used to remove excess newline from the
 *    user input. This topic was self-studied as it is outside the lessons of
 *    CCPROG2.
 *    Reference: https://stackoverflow.com/questions/2693776/removing-trailing
 *    -newline-character-from-fgets-input
 *
 * @param str   The string which has a newline character that is to be
 *              removed.
 */
void
removeNewline(char *str)
{
    str[strcspn(str, "\n")] = 0;
}

/**
 * @brief Converts a string to title case. This Is Title Case.
 *
 * @note Credit: https://codereview.stackexchange.com/a/239223
 *
 * @param str   The string that is to be converted to title case.
 */
void
titleCase(char *str)
{
    int i = 0;

    while (str[i] != '\0')
    {
        // takes advantage of short-circuit evaluation so if i == 0, then
        // str[i-1] would not be evaluated, so there's no undefined behavior
        if (i == 0 || str[i - 1] == ' ')
        {
            str[i] = toupper(str[i]);
        }
        else
        {
            str[i] = tolower(str[i]);
        }

        i++;
    }
}

/**
 * @brief Converts a string to lowercase. this is lowercase.
 *
 * @param str   The string that is to be converted to lowercase.
 */
void
lowercase(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        str[i] = tolower(str[i]);
        i++;
    }
}

/**
 * @brief
 *    Gets a string from the user with a set minimum and maximum amount
 *    of characters.
 *
 * @details
 *    In obtaining input, fgets() was used instead of scanf() because it
 *    allows input of strings with whitespaces unlike scanf(). This topic was
 *    self-studied because it is outside the lessons of CCPROG2.
 *    Reference: https://www.thecrazyprogrammer.com/2021/05/fgets-vs-scanf.html
 *
 * @param str         Where the string input will be stored.
 * @param minStrLen   The minimum amount of characters of the desired input.
 * @param maxStrLen   The maximum amount of characters of the desired input.
 *
 * @pre   minStrLen is >= 0 and <= maxStrLen.
 * @pre   str's size is >= maxStrLen.
 * @pre   maxStrLen accounts for the null terminator.
 */
void
getStrInput(char *str, 
            int minStrLen, 
            int maxStrLen)
{
    int tempStrSize = maxStrLen + 1; // extra character to detect if user
                                     // entered extra characters
    char *tempStr = (char *)malloc(tempStrSize * sizeof(char));
    int inputIsValid = 0;
    int len = 0;

    while (!inputIsValid)
    {
        printf("Input: ");
        fgets(tempStr, tempStrSize, stdin);
        fflush(stdin); // clears the input stream
        removeNewline(tempStr);

        len = strlen(tempStr);
        if (len >= minStrLen && len <= maxStrLen - 1)
        {
            inputIsValid = 1;
        }
        else
        {
            printf("\nYour input must be ");

            if (minStrLen == maxStrLen - 1)
            {
                printf("%d character%s!\n", minStrLen, 
                                            minStrLen == 1 ? "" : "s");
            }
            else
            {
                printf("between %d and %d characters!\n", minStrLen, 
                                                          maxStrLen - 1);
            }
        }
    }

    strcpy(str, tempStr);
    free(tempStr);
}

/**
 * @brief Gets an int from the user.
 *
 * @note
 *    If the user does not enter a number or enters a number after non-numeric
 *    characters, the assumed value the user entered is 0. If 0 is within the
 *    bounds of min and max, 0 is considered a valid input and is returned by
 *    the function.
 *
 * @param min   Minimum int value to be obtained.
 * @param max   Maximum int value to be obtained.
 *
 * @pre   min is >= INT_MIN.
 * @pre   max is <= INT_MAX.
 *
 * @return Returns the int input from the user.
 */
int
getIntInput(int min, 
            int max)
{
    long value = 0;
    char strInput[11] = "";
    int inputIsValid = 0;

    while (!inputIsValid)
    {
        getStrInput(strInput, 1, 11);
        value = strtol(strInput, NULL, 10);

        if (value >= min && value <= max)
        {
            inputIsValid = 1;
        }
    }

    return (int)value;
}

/**
 * @brief Obtains a character from the user.
 *
 * @return Returns the character input from the user.
 */
char
getCharInput()
{
    char strInput[2] = "";
    getStrInput(strInput, 1, 2);

    return strInput[0];
}

/**
 * @brief Copies a lang-tl pair to another.
 *
 * @param destLang   Destination string for the language.
 * @param destTL     Destination string for the translation.
 * @param srcLang    Source string for the language.
 * @param srcTL      Source string for the translation.
 */
void
copyLangTLPair(char *destLang, 
               char *destTL, 
               char *srcLang, 
               char *srcTL)
{
    strncpy(destLang, srcLang, MAX_LANG_LEN + 1);
    strncpy(destTL, srcTL, MAX_TL_LEN + 1);
}

/**
 * @brief Clears a lang-tl pair (sets both to null strings).
 *
 * @param lang   Language string to be cleared.
 * @param tl     Translation string to be cleared.
 */
void
clearLangTLPair(char *lang, 
                char *tl)
{
    strcpy(lang, "");
    strcpy(tl, "");
}

/**
 * @brief
 *    Clears an entry (sets all lang-tl pairs to null strings and sets count
 *    to 0).
 *
 * @param givenEntry   Entry to be cleared.
 */
void
clearEntry(Entry *givenEntry)
{
    Entry temp = {};
    *givenEntry = temp;
}

/**
 * @brief
 *    This function displays the divider for better readability in the
 *    runtime.
 */
void
displayDivider()
{
    printf("\n");
    for (int i = 0; i < 100; i++)
    {
        printf("-");
    }
    printf("\n\n");
}

/**
 * @brief This function displays the Main Menu to the user.
 *
 */
void
displayMainMenu()
{
    displayDivider();

    char menuText[] = "MAIN MENU\n"
                      "1 - Manage Data\n"
                      "2 - Translate Menu\n"
                      "3 - Exit";

    printf("%s", menuText);
}

/**
 * @brief This function displays the Translate Menu to the user.
 *
 */
void
displayTransMenu()
{
    displayDivider();

    char menuText[] = "TRANSLATE MENU\n"
                      "1 - Translate\n"
                      "2 - Exit";

    printf("%s", menuText);
}

/**
 * @brief
 *    This function asks the user to choose from the given choices of the
 *    Main Menu.
 *
 * @param pMainChoice   The address where the user's choice will be stored.
 */
void
getMainChoice(int *pMainChoice)
{
    printf("\n\nEnter a number from 1 to 3.\n");
    *pMainChoice = getIntInput(1, 3);
}

/**
 * @brief
 *    This function asks the user to choose from the given choices of the
 *    Translate Menu.
 *
 * @param pTransChoice   The address where the user's choice will be stored.
 */
void
getTransChoice(int *pTransChoice)
{
    printf("\n\nEnter a number from 1 to 2.\n");
    *pTransChoice = getIntInput(1, 2);
}

/**
 * @brief This function displays the Manage Data Menu to the user.
 *
 */
void
displayManageMenu()
{
    displayDivider();

    char menuText[] = "MANAGE DATA MENU\n"
                      "1 - Add Entry\t\t\t6 - Search Word\n"
                      "2 - Add Translation\t\t7 - Search Translation\n"
                      "3 - Delete Entry\t\t8 - Export\n"
                      "4 - Delete Translation\t\t9 - Import\n"
                      "5 - Display All Entries\t\t10 - Exit";

    printf("%s", menuText);
}

/**
 * @brief
 *     This function asks the user to choose from the given choices of the
 *     Manage Data Menu.
 *
 * @param pManageChoice   The address where the user's choice will be stored.
 */
void
getManageChoice(int *pManageChoice)
{
    printf("\n\nEnter a number from 1 to 10.\n");
    *pManageChoice = getIntInput(1, 10);
}

/**
 * @brief
 *    This function displays a message to the user, informing them that the
 *    system has already reached the maximum number of entries or pairs in an
 *    entry, depending on the value of nManageChoice.
 *
 * @param nManageChoice   The integer representing the user's choice at the
 *  					  Manage Data Menu.
 *
 * @pre   nManageChoice should only contain either 1 or 2.
 */
void
displayMaxMsg(int nManageChoice)
{
    displayDivider();
    
    if (nManageChoice == 1)
    {
        printf("The system has already reached the maximum number of ");
        printf("entries (%d).", MAX_ENTRIES);
    }
    else
    {
        printf("This entry has already reached the maximum number of ");
        printf("pairs (%d).", MAX_COUNT);
    }

    printf("\nGoing back to the Manage Data Menu now...\n");
}

/**
 * @brief
 *    This function gets the Language and Translation pair input from the
 *    user. Necessary code to handle invalid inputs were also in place.
 *
 * @param tempLangVar    The string where the translation input is temporarily
 *                       stored.
 * @param tempTransVar   The string where the language input is temporarily
 *                       stored.
 */
void
getLangTrans(String20 tempLangVar, 
             String20 tempTransVar)
{
    displayDivider();

    // get language
    printf("Enter language (maximum of 20 characters).\n");
    getStrInput(tempLangVar, MIN_LANG_LEN, MAX_LANG_LEN + 1);
    titleCase(tempLangVar);

    // get translation
    printf("Enter translation (maximum of 20 characters).\n");
    getStrInput(tempTransVar, MIN_TL_LEN, MAX_TL_LEN + 1);
    lowercase(tempTransVar);
}

/**
 * @brief
 *    This function obtains a command of either next, previous, or exit as the
 *    choice of the user when entries are being displayed.
 *
 * @param pDispChoice   The address where the user's command (a char) will be
 *                      stored.
 * @param nIndex        The index of an array.
 * @param nEntryCount   The current number entries in the entry list.
 *
 * @pre   nIndex is >= 0 and <= nEntryCount - 1.
 */
void
getDispChoice(char *pDispChoice, 
              int nIndex, 
              int nEntryCount)
{
    // display menu options
    printf("\n\nYour possible options from here:\n");
    if (nIndex > 0)
    {
        printf("%c - Previous\n", ENTRY_OPTION_PREV);
    }

    if (nEntryCount > 1 && nIndex <= nEntryCount - 2)
    {
        printf("%c - Next\n", ENTRY_OPTION_NEXT);
    }

    printf("%c - Exit\n", ENTRY_OPTION_EXIT);

    // get actual choice from user
    int inputIsValid = 0;

    while (!inputIsValid)
    {
        *pDispChoice = getCharInput();

        // the char obtained is converted to uppercase so it does not
        // need to be checked with two chars every time
        *pDispChoice = toupper(*pDispChoice);

        if ((*pDispChoice == ENTRY_OPTION_PREV && nIndex > 0) ||
            (*pDispChoice == ENTRY_OPTION_NEXT && nEntryCount > 1 &&
             nIndex <= nEntryCount - 2) ||
            (*pDispChoice == ENTRY_OPTION_EXIT))
        {
            inputIsValid = 1;
        }
    }
}

/**
 * @brief This function displays one entry to the user.
 *
 * @param entries         An array of structures containing all the entries and
 *                        language-translation pairs.
 * @param nIndex          The index of an array.
 * @param nManageChoice   The integer representing the user's choice in the
 *                        Manage Data Menu.
 *
 * @pre   nIndex should only be an integer ranging from 0 to array size - 1.
 */
void
displayCurrent(Entry *entries, 
               int nIndex, 
               int nManageChoice)
{
    int i;

    // display header if displaying entry one at a time
    if (nManageChoice != 1 && nManageChoice != 2)
    {
        displayDivider();
        printf("Format (Language: Translation)\n");
    }

    printf("\nEntry No. %d", nIndex + 1);

    // print language-translation pairs
    for (i = 0; i < entries[nIndex].count; i++)
    {
        printf("\n(%d) %s: %s", i + 1, entries[nIndex].lang[i],
                                       entries[nIndex].trans[i]);
    }

    printf("\n");
}

/**
 * @brief This function displays to the user all matched entries found.
 *
 * @param entries         An array of structures containing all the entries
 *                        and language-translation pairs.
 * @param arrMatched      The array where the indices of the matched entries
                          are stored.
 * @param nMatches        The number of matched entries found in the search.
 * @param nManageChoice   The integer representing the user's choice in the
 *                        Manage Data Menu.
 *
 * @pre   nMatches is >= 1 and <= MAX_ENTRIES.
 */
void
displayMEntries(Entry *entries,
                int *arrMatched,
                int nMatches,
                int nManageChoice)
{
    int i, k = 0;
    char cDispChoice;

    displayDivider();

    if (nMatches == 1)
    {
        printf("There is %d matched entry ", nMatches);
    }
    else
    {
        printf("There are %d matched entries ", nMatches);
    }

    printf("that contain%s the ", nMatches == 1 ? "s" : "");

    if (nManageChoice == 1 || nManageChoice == 2 || nManageChoice == 7)
        printf("language-translation pair.\n");
    else
        printf("searched word.\n");

    if (nManageChoice == 1 || nManageChoice == 2)
    {
        displayDivider();
        printf("Format (Language: Translation)\n");
        for (i = 0; i < nMatches; i++)
        {
            displayCurrent(entries, arrMatched[i], nManageChoice);
        }
    }
    else
    {
        while (k >= 0 && k < nMatches)
        {
            displayCurrent(entries, arrMatched[k], nManageChoice);
            getDispChoice(&cDispChoice, k, nMatches);

            switch (cDispChoice)
            {
                case ENTRY_OPTION_NEXT:
                    k++;
                    break;
                case ENTRY_OPTION_PREV:
                    k--;
                    break;
                default:
                    k = nMatches;
                    break; // exit
            }
        }
    }
}

/**
 * @brief Asks confirmation from the user.
 *
 * @return
 *    1   if the user answers 'yes'.
 *    0   if the user answers 'no'.
 */
int
getUserConfirmation()
{
    int confirm = 0;
    int inputIsValid = 0;
    char input = 0;

    printf("[Y/N] \n");
    while (!inputIsValid)
    {
        input = getCharInput();
        input = tolower(input); // to avoid checking with two chars every time
        if (input == CONFIRM_YES || input == CONFIRM_NO)
        {
            inputIsValid = 1;
        }
    }

    switch (input)
    {
        case CONFIRM_YES:
            confirm = 1;
            break;
        case CONFIRM_NO:
            confirm = 0;
            break;
    }

    return confirm;
}

/**
 * @brief This function asks the user if the entry is for a new entry or not.
 *
 * @return
 *    1   if it is for a new entry (the user answered 'yes').
 *    0   if it is not for a new entry (the user answered 'no').
 */
int
isNewEntry()
{
    displayDivider();

    printf("Is this for a new entry? ");
    return getUserConfirmation();
}

/**
 * @brief
 *    This function assigns the new entries from the temporary variables to
 *    the actual array of entries.
 *
 * @param entries        An array of structures containing all the entries and
 *                       language-translation pairs.
 * @param nEntryCount    The current number entries in the entry list.
 * @param tempLangVar    The string where the language input is temporarily
 *                       stored.
 * @param tempTransVar   The string where the translation input is temporarily
 *                       stored.
 *
 * @pre   tempLangVar's length is <= MAX_LANG_LEN.
 * @pre   tempTransVar's length is <= MAX_TL_LEN.
 * @pre   nEntryCount is >= 0 and < MAX_ENTRIES.
 */
void
assignNewEntries(Entry *entries,
                 int nEntryCount,
                 String20 tempLangVar,
                 String20 tempTransVar)
{
    // assign the new entries to the nearest uninitialized entry
    int curEntryCount = entries[nEntryCount].count;
    copyLangTLPair(entries[nEntryCount].lang[curEntryCount],
                   entries[nEntryCount].trans[curEntryCount],
                   tempLangVar, tempTransVar);

    entries[nEntryCount].count++;
}

/**
 * @brief
 *    This function asks the user for more language-translation pairs to add
 *    under the same entry until the maximum number of pairs or if the user
 *    no longer wants to add pairs.
 *
 * @param entries       Array of structures containing all the entries and
 *                      language-translation pairs.
 * @param nEntryCount   The current number entries in the entry list.
 *
 * @pre   nEntryCount is >= 0 and < MAX_ENTRIES.
 */
void
addMoreTrans(Entry *entries, 
             int nEntryCount, 
             int nManageChoice)
{
    String20 tempLangVar, tempTransVar;

    int over = 0;
    while (!over && entries[nEntryCount].count < MAX_COUNT)
    {
        if (entries[nEntryCount].count > 1)
        {
            displayDivider();
            printf("Do you want to add more language-translation pairs ");
            printf("for this entry? ");
            over = !getUserConfirmation();
        }

        if (!over)
        {
            // if the user chooses yes or there are < 2 language-translation
            // pairs in the entry
            getLangTrans(tempLangVar, tempTransVar);
            assignNewEntries(entries, nEntryCount, tempLangVar, tempTransVar);
            displayDivider();
            printf("Format (Language: Translation)\n");
            displayCurrent(entries, nEntryCount, nManageChoice);
        }
        else
        {
            // if the user chooses no
            displayDivider();
            printf("Going back to the Manage Data Menu now...\n");
        }

        // check if the current count of pairs have already reached the maximum
        if (entries[nEntryCount].count == MAX_COUNT)
        {
            displayDivider();
            printf("This entry has already reached the maximum number ");
            printf("of pairs (%d).\n", MAX_COUNT);
            printf("Going back to the Manage Data Menu now...\n");
        }
    }
}

/**
 * @brief
 *    This function handles the display message when the user tries to use a
 *    feature that requires at least one entry, when there is no entry added
 *    yet.
 *
 * @param nManageChoice The user's choice in the Manage Data menu.
 *
 * @pre   nManageChoice is >= 2 and <= 8.
 */
void
displayNoneMsg(int nManageChoice)
{
    displayDivider();

    switch (nManageChoice)
    {
        case 2:
            printf("This entry does not exist. ");
            printf("You must first utilize the Add Entry feature.\n");
            break;
        case 3:
        case 4:
            printf("There are no entries yet. ");
            printf("Hence, you can't delete anything for now.\n");
            break;
        case 5:
            printf("There are no entries to display yet.\n");
            break;
        case 6:
            printf("There are no entries that contain this word.\n");
            break;
        case 7:
            printf("This language-translation pair does not exist.\n");
            break;
        case 8:
            printf("There is no data to export yet.\n");
            break;
    }
}

/**
 * @brief This function asks the user which entry does he or she wants to add
 * the pair into.
 *
 * @param nEntryChoice   The address where the user's choice will be stored.
 * @param arrMatched     The array where the indices of the matched entries
 *                       are stored.
 * @param nMatches       The number of matched entries found in the search.
 *
 * @pre   nMatches is >= 1 and <= MAX_ENTRIES.
 */
void
getEntryChoice(int *nEntryChoice, 
               int *arrMatched, 
               int nMatches)
{
    int nChoice;

    displayDivider();
    printf("Which entry do you want to add a language-translation pair?\n");
    nChoice = getIntInput(1, nMatches);

    // assign the index of the entry to the user's choice variable
    *nEntryChoice = arrMatched[nChoice - 1];
}

/**
 * @brief This function checks if the language input is in the specific entry.
 *
 * @param entries         An array of structures containing all the entries
 *                        and language-translation pairs.
 * @param strLang         The string that contains the tested language.
 * @param nCurrentEntry   The index of the entry being tested.
 *
 * @pre   nCurrentEntry is >= 0 and <= nEntryCount - 1.
 *
 * @return
 *    1   if the language is in the entry.
 *    0   if not.
 */
int
isLangInEntry(Entry *entries, 
              String20 strLang,
              int nCurrentEntry)
{
    int i, nReturn = 0;

    // look for the language within the entry (intra-entry)
    for (i = 0; i < entries[nCurrentEntry].count && nReturn == 0; i++)
    {
        if (!strcmp(entries[nCurrentEntry].lang[i], strLang))
        {
            nReturn = 1;
        }
    }

    return nReturn;
}

/**
 * @brief This function swaps two entries within the array of structures.
 *
 * @param entries   Array of structures containing all the entries and
 *                  language-translation pairs.
 * @param nEntry1   The index of the first entry.
 * @param nEntry2   The index of the second entry.
 *
 * @pre   nEntry1 and nEntry are >= 0 and <= nEntryCount - 1, where
 *        where nEntryCount is the current amount of entries.
 */
void
swapEntry(Entry *entries, 
          int nEntry1, 
          int nEntry2)
{
    Entry temp = {};

    temp = entries[nEntry1];
    entries[nEntry1] = entries[nEntry2];
    entries[nEntry2] = temp;
}

/**
 * @brief This function swaps two language-translation pairs within an entry.
 *
 * @param entries       Array of structures containing all the entries and
 *                      language-translation pairs.
 * @param nEntryIndex   The index of entry where its pairs will be swapped.
 * @param nIndex1       The index of the first language-translation pair.
 * @param nIndex2       The index of the second language-translation pair.
 *
 * @pre   nEntryIndex is >= 0 and <= nEntryCount - 1.
 * @pre   nIndex1 and nIndex2 are >= 0 and <= entries[nEntryIndex].count - 1.
 */
void
swapLangTrans(Entry *entries, 
              int nEntryIndex, 
              int nIndex1, 
              int nIndex2)
{
    String20 tempLangVar, tempTransVar;

    // assign first language-translation pair to temporary string variables
    copyLangTLPair(tempLangVar, tempTransVar,
                   entries[nEntryIndex].lang[nIndex1],
                   entries[nEntryIndex].trans[nIndex1]);

    // assign the second language-translation pair to the first
    // language-translation pair
    copyLangTLPair(entries[nEntryIndex].lang[nIndex1],
                   entries[nEntryIndex].trans[nIndex1],
                   entries[nEntryIndex].lang[nIndex2],
                   entries[nEntryIndex].trans[nIndex2]);

    // assign the temporary variables to the second language-translation pair
    copyLangTLPair(entries[nEntryIndex].lang[nIndex2],
                   entries[nEntryIndex].trans[nIndex2],
                   tempLangVar, tempTransVar);
}

/**
 * @brief
 *    This function arranges the entries by placing entries with the language
 *    string "English" first, and they are arranged based on order of entry.
 *    Meanwhile, entries without the "English" language string are placed at
 *    the end of the entries, unsorted or not.
 *
 * @param entries       Array of structures containing all the entries and
 *                      language-translation pairs.
 * @param nEntryCount   The current number entries in the entry list.
 *
 * @pre   nEntryCount is >= 0 and <= MAX_ENTRIES.
 */
void
arrangeInterEnt(Entry *entries, 
                int nEntryCount)
{
    int i, j;

    // go through each entry and see if it has the "English" language in it
    for (i = 0; i < nEntryCount - 1; i++)
    {
        // if the entry does not have "English" in it, find the nearest
        // entry with "English" and swap them
        if (!isLangInEntry(entries, "English", i))
        {
            for (j = i + 1; j < nEntryCount; j++)
            {
                if (isLangInEntry(entries, "English", j))
                {
                    swapEntry(entries, i, j);
                    j = nEntryCount; // end the inner loop
                }
            }
        }
    }
}

/**
 * @brief
 *    This function arranges the language-translation pairs within an entry
 *    alphabetically based on language.
 *
 * @param entries       An array of structures containing all the entries and
 *                      language-translation pairs.
 * @param nEntryCount   The current number entries in the entry list.
 *
 * @pre   nEntryCount is >= 0 and <= MAX_ENTRIES.
 */
void
arrangeIntraEnt(Entry *entries, 
                int nEntryCount)
{
    int i, j, l, min;

    for (i = 0; i < nEntryCount; i++)
    {
        // set the following initialization in the loop:
        // j = 0; if the intra-entry sorting includes the "source" pair
        // j = 1; if the intra-entry sorting doesn't include the "source" pair
        for (j = 1; j < entries[i].count - 1; j++)
        {
            min = j;

            // look for the "least" entry alphabetically to be
            // replaced with the first unsorted element in the array
            for (l = j + 1; l < entries[i].count; l++)
            {
                if (strcmp(entries[i].lang[l], entries[i].lang[min]) < 0)
                    min = l;
            }

            // don't swap if destination and source pair are the same
            if (min != j)
            {
                swapLangTrans(entries, i, min, j);
            }
        }
    }
}

/**
 * @brief
 *    This function displays all entries to the user by displaying each entry
 *    one at a time.
 *
 * @param entries       Array of structures containing all the entries and
 *                      language-translation pairs.
 * @param nEntryCount   the current number entries in the entry list.
 *
 * @pre   nEntryCount is >= 0 and <= MAX_ENTRIES.
 */
void
displayAllEntries(Entry *entries, 
                  int nEntryCount, 
                  int nManageChoice)
{
    int k = 0;
    char cDispChoice;

    // arrange the entries inter-entry (those with "English" go first)
    arrangeInterEnt(entries, nEntryCount);

    // arrange the entries intra-entry (language-translation pairs)
    arrangeIntraEnt(entries, nEntryCount);

    while (k >= 0 && k < nEntryCount)
    {
        displayCurrent(entries, k, nManageChoice);
        getDispChoice(&cDispChoice, k, nEntryCount);

        // adjust the index of the entry being displayed depending on
        // the user's command
        switch (cDispChoice)
        {
            case ENTRY_OPTION_NEXT:
                k++;
                break;
            case ENTRY_OPTION_PREV:
                k--;
                break;
            default:
                k = nEntryCount;
                break; // exit
        }
    }
}

/**
 * @brief
 *    This function gets the choice of the user on which entry to delete or
 *    delete from.
 *
 * @param nDelChoice    Address where the user's entry choice to delete is
 *                      stored.
 * @param nEntryCount   The current number entries in the entry list.
 *
 * @pre *nDelChoice is >= 1 and <= MAX_ENTRIES.
 *
 * @return
 *    1   if the user entered a valid input.
 *    0   if invalid.
 */
int
getDelChoice(int *nDelChoice, 
             int nEntryCount)
{
    int nReturn;

    displayDivider();
    printf("Enter the entry number.\n");
    *nDelChoice = getIntInput(INT_MIN, INT_MAX);

    // handle invalid input
    if ((*nDelChoice) >= 1 && (*nDelChoice) <= nEntryCount)
        nReturn = 1;
    else
        nReturn = 0;

    return nReturn;
}

/**
 * @brief
 *    This function displays a message to the user that the entered input is
 *    invalid.
 *
 */
void
displayInvDel()
{
    displayDivider();
    printf("Input is invalid. Going back to the Manage Data Menu now...\n");
}

/**
 * @brief This function deletes an entry from the list of entries.
 *
 * @param entries       Array of structures containing all the entries and
 *                      language-translation pairs.
 * @param nEntryCount   The current number entries in the entry list.
 * @param nDelChoice    The index of the entry number that the user wants to
 *                      delete subtracted by 1.
 *
 * @pre   nEntryCount is > 0 and <= MAX_ENTRIES.
 * @pre   nDelChoice is >= 1 and <= MAX_ENTRIES.
 */
void
deleteEntry(Entry *entries, 
            int nEntryCount, 
            int nDelChoice)
{
    int i;

    // immediately start shifting entries forward until the
    // last entry is copied
    for (i = nDelChoice - 1; i <= nEntryCount - 2; i++)
    {
        entries[i] = entries[i + 1];
    }

    clearEntry(&entries[i]); // clear the last entry
}

/**
 * @brief
 *    This function gets the index of the pair from a specific entry that
 *    the user wants to delete.
 *
 * @param entries      Array of structures containing all the entries and
 *                     language-translation pairs.
 * @param nDelIndex    Address where the user's choice of pair within an entry
 *                     to delete will be stored.
 * @param nDelChoice   The index of the entry number that the user wants to
 *                     delete subtracted by 1.
 *
 * @pre   nDelChoice is >= 1 and <= MAX_ENTRIES.
 * @pre   *nDelIndex is >= 1 and <= MAX_COUNT.
 *
 * @return
 *    1   if the user inputs a valid number (1 to the entry's pair count).
 *    0   otherwise.
 */
int
getDelIndex(Entry *entries, 
            int *nDelIndex, 
            int nDelChoice)
{
    int nReturn;

    int over = 0;
    while (!over)
    {
        over = 1; // assume that the input is valid

        printf("\nWhat language-translation pair do you want to delete?\n");
        *nDelIndex = getIntInput(INT_MIN, INT_MAX);

        // if input is invalid, ask user if they wish to continue
        // otherwise, loop will automatically stop
        // set the following conditions in the if condition:
        // *nDelIndex < 1; if the user can delete the "source" pair
        // *nDelIndex < 2; if the user can't delete the "source" pair
        if (*nDelIndex < 2 || *nDelIndex > entries[nDelChoice - 1].count)
        {
            printf("\nInput is invalid. ");

            if (*nDelIndex == 1)
                printf("You can't delete the \"source\" pair. ");

            printf("Do you still want to delete translation? ");
            over = !getUserConfirmation();
        }
    }

    if ((*nDelIndex) >= 1 && (*nDelIndex) <= entries[nDelChoice - 1].count)
        nReturn = 1;
    else
        nReturn = 0;

    return nReturn;
}

/**
 * @brief This function deletes a language-translation pair from an entry.
 *
 * @param entries      Array of structures containing all the entries and
 *                     language-translation pairs.
 * @param nDelChoice   The index of the entry number that the user wants to
 *                     delete, added by 1.
 * @param nDelIndex    The index of the language-translation pair that the
 *                     user wants to delete from an entry, added by 1.
 *
 * @pre   nDelChoice is >= 1 and <= MAX_ENTRIES.
 * @pre   nDelIndex is >= 1 and <= MAX_COUNT.
 */
void
deleteTrans(Entry *entries, 
            int nDelChoice, 
            int nDelIndex)
{
    int i;

    nDelChoice--; // since the entries array is 0-based
    for (i = nDelIndex - 1; i < entries[nDelChoice].count - 1; i++)
    {
        // immediately start shifting pairs forward until the
        // last pair is copied
        copyLangTLPair(entries[nDelChoice].lang[i],
                       entries[nDelChoice].trans[i],
                       entries[nDelChoice].lang[i + 1],
                       entries[nDelChoice].trans[i + 1]);
    }

    // empty the strings of the last initialized pair
    clearLangTLPair(entries[nDelChoice].lang[i], entries[nDelChoice].trans[i]);

    entries[nDelChoice].count--;
}

/**
 * @brief
 *    This function displays the message, asking the user if he or she wants
 *    to delete more language-translation pairs.
 *
 */
void
displayDelAgain()
{
    displayDivider();
    printf("Do you want to delete more language-translation pairs? ");
}

/**
 * @brief
 *    Returns the index of where a language-translation pair is first found in
 *    an entry.
 *
 * @param givenEntry   The entry to be searched in.
 * @param lang         The language string to be searched for.
 * @param tl           The translation string to be searched for.
 * @param start        Index of first string to be searched.
 * @param end          Index of last string to be searched.
 *                     If -1 is passed, until the last string is searched.
 *
 * @pre   start is >= 0.
 * @pre   end is < the amount of language-translation pairs in the entry,
 *        if end is != -1.
 * @pre   start is <= end, if end is != -1.
 *
 * @return
 *    Index of first found lang-tl pair   if the lang-tl pair was found.
 *    -1                                  if the lang-tl pair was not found.
 */
int
isPairInEntry(Entry *givenEntry, 
              char *lang, 
              char *tl, 
              int start, 
              int end)
{
    int i, nReturn = -1;

    end = end == -1 ? givenEntry->count - 1 : end;
    for (i = start; i <= end; i++)
    {
        if (!strcmp(lang, givenEntry->lang[i]) &&
            !strcmp(tl, givenEntry->trans[i]))
        {
            nReturn = i; // return the index of where the pair was found
            i = end + 1; // end the loop
        }
    }

    return nReturn;
}

/**
 * @brief
 *    This function checks if the given language and translation input is
 *    already existing in the list of entries.
 *
 * @param tempLangVar    The string where the language input is temporarily
 *                       stored.
 * @param tempTransVar   The string where the translation input is temporarily
 *                       stored.
 * @param nEntryCount    The current number entries in the entry list.
 * @param entries        An array of structures containing all the entries and
 *                       language-translation pairs.
 * @param arrMatched     The array where the indices of the matched entries
 *                       are stored.
 * @param pMatches       The address where the number of matched entries will
 *                       be stored.
 * @param start          Index of the first pair to be searched in all entries.
 * @param end            Index of the last pair to be searched in all entries.
 *                       If -1 is passed, all pairs in each entry are searched.
 *
 * @pre   nEntryCount is >= 0 and <= MAX_ENTRIES.
 * @pre   start is >= 0.
 * @pre   end is < the amount of language-translation pairs in all entries,
 *        if end is != -1.
 * @pre   start is <= end, if end is != -1.
 *
 * @return
 *    1   if the pair is in at least one entry.
 *    0   if the pair has no matches.
 */
int
findPairInAllEntries(String20 tempLangVar,
                     String20 tempTransVar,
                     int nEntryCount,
                     Entry *entries,
                     int *arrMatched,
                     int *pMatches,
                     int start,
                     int end)
{
    int i, nReturn;
    int pairIsInEntry;

    nReturn = 0;   // assume pair is not found in any entries
    *pMatches = 0; // initial number of matches found

    for (i = 0; i < nEntryCount; i++)
    {
        pairIsInEntry = isPairInEntry(&entries[i], tempLangVar, tempTransVar,
                                      start, end);
        if (pairIsInEntry != -1)
        {
            arrMatched[*pMatches] = i; // store the index
            *pMatches += 1;
            nReturn = 1; // the pair was found in at least one entry
        }
    }

    return nReturn;
}

/**
 * @brief
 *    Returns the index of where a translation string was first found in an
 *    entry.
 *
 * @param givenEntry   The entry to be searched in.
 * @param tl           The translation string to be searched for.
 * @param start        Index of first string to be searched in the entry.
 * @param end          Index of last string to be searched in the entry.
 *                     If -1 is passed, until the last string is searched.
 *
 * @pre   start is >= 0.
 * @pre   end is < the amount of language-translation pairs in the entry,
 *        if end is != -1.
 * @pre   start is <= end, if end is != -1.
 *
 * @return
 *    Index of first found tl string   if the translation was found.
 *    -1                               if the translation was not found.
 */
int
isWordInEntry(Entry *givenEntry, 
              char *tl, 
              int start, 
              int end)
{
    int i, nReturn = -1;

    end = end == -1 ? givenEntry->count - 1 : end;
    for (i = start; i <= end; i++)
    {
        if (!strcmp(tl, givenEntry->trans[i]))
        {
            nReturn = i; // return the index of where the word was found
            i = end + 1; // end the loop
        }
    }

    return nReturn;
}

/**
 * @brief
 *    This function searches for the input word in the list of entries, both
 *    intra-entry and inter-entry.
 *
 * @param entries       Array of structures containing all the entries and
 *                      language-translation pairs.
 * @param strKey        The string containing the word being searched.
 * @param nEntryCount   The current number entries in the entry list.
 * @param arrMatched    The array where the indices of the matched entries are
 * stored.
 * @param pMatches      The address where the number of matched entries will be
 * stored.
 * @param start         Index of the first pair to be searched in all entries.
 * @param end           Index of the last pair to be searched in all entries.
 *                      If -1 is passed, all pairs in each entry are searched.
 *
 * @pre   nEntryCount is >= 0 and <= MAX_ENTRIES.
 * @pre   *pMatches is >= 1 and MAX_ENTRIES.
 * @pre   start is >= 0.
 * @pre   end is < the amount of language-translation pairs in all entries,
 *        if end is != -1.
 * @pre   start is <= end, if end is != -1.
 *
 * @return
 *    1   if the word is in at least one entry.
 *    0   if not.
 */
int
findWordInAllEntries(Entry *entries,
                     String20 strKey,
                     int nEntryCount,
                     int *arrMatched,
                     int *pMatches,
                     int start,
                     int end)
{
    int i;
    int wordIsInEntry;

    int nReturn = 0; // assume translation was not found in any entry
    *pMatches = 0;   // initial number of matches found

    for (i = 0; i < nEntryCount; i++)
    {
        wordIsInEntry = isWordInEntry(&entries[i], strKey, start, end);
        if (wordIsInEntry != -1)
        {
            arrMatched[*pMatches] = i; // store the index
            *pMatches += 1;
            nReturn = 1; // the translation was found in at least one entry
        }
    }

    return nReturn;
}

/**
 * @brief This function gets the word that the user wants to search for.
 *
 * @param strKey   The string containing the word being searched.
 */
void
getKey(String20 strKey)
{
    displayDivider();
    printf("Enter word (maximum of 20 characters).\n");

    getStrInput(strKey, MIN_TL_LEN, MAX_TL_LEN + 1);
    lowercase(strKey);
}

/**
 * @brief This function gets the language input from the user.
 *
 * @param strLang   The string where the language input will be stored.
 * @param nType     The type of language that the user is asked (either the
 *                  language of the source text or the language to be
 *                  translated to).
 *
 * @pre   nType is either 1 or 2.
 */
void
getLang(String20 strLang, 
        int nType)
{
    displayDivider();

    if (nType == 1)
        printf("Enter language of source text ");
    else
        printf("Enter language to be translated to ");

    printf("(maximum of %d characters).\n", MAX_LANG_LEN);

    getStrInput(strLang, MIN_LANG_LEN, MAX_LANG_LEN + 1);
    titleCase(strLang);
}

/**
 * @brief
 *    This function empties the entry count, pair count per entry, and all
 *    string variables initialized when the user chooses to exit from the
 *    Manage Data menu or Translate menu.
 *
 * @param entries       Array of structures containing all the entries and
 *                      language-translation pairs.
 * @param nEntryCount   The current number entries in the entry list.
 *
 * @pre   *nEntryCount is >= 0 and <= MAX_ENTRIES.
 */
void
emptyEntry(Entry *entries, 
           int *nEntryCount)
{
    int i;

    // empty all initialized strings of all initialized entries
    for (i = 0; i < *nEntryCount; i++)
    {
        clearEntry(&entries[i]);
    }

    // reset the entry count
    *nEntryCount = 0;
}

/**
 * @brief
 *    This function checks if the filename input from the user contains the
 *    text file extension (.txt).
 *
 * @param tempFile   The temporary string variable where the filename input
 *                   is stored.
 *
 * @return
 *    1   if the text file extension ".txt" is present.
 *    0   if not.
 */
int
isExtPresent(String35 tempFile)
{
    int nReturn = 0; // assume the extension is not present
    int len;

    // check if ".txt" is present in the file extension
    len = strlen(tempFile);
    if (len >= 5) // min of 1 char for the file name and 4 for the ".txt" part
        if (!strcmp(&tempFile[len - 4], ".txt"))
            nReturn = 1;

    return nReturn;
}

/**
 * @brief
 *    This function checks if the filename input from the user contains any of
 *    the invalid characters in filenames. Invalid characters in filenames
 *    include: \ / : * ? " < > |
 *
 * @param tempFile   The temporary string variable where the filename input is
 *                   stored.
 * @return
 *    1   if there are invalid characters present.
 *    0   if not.
 */
int
isInvCharPres(String35 tempFile)
{
    int i;
    int nReturn = 0;

    // assume the string does not have any invalid characters
    // if there is at least one found, immediately exit the loop
    int len = strlen(tempFile);
    for (i = 0; i < len && nReturn == 0; i++)
    {
        if (tempFile[i] == '\\' || tempFile[i] == '/' || tempFile[i] == ':' ||
            tempFile[i] == '*' || tempFile[i] == '?' || tempFile[i] == '"' ||
            tempFile[i] == '<' || tempFile[i] == '>' || tempFile[i] == '|')
        {
            nReturn = 1;
        }
    }

    return nReturn;
}

/**
 * @brief This function gets filename input from the user.
 *
 * @param filename   The string variable where the filename will be stored.
 */
void
getFileName(String30 filename)
{
    String35 tempFile; // temporary filename string
    int fileExtIsPresent = 0;
    int hasInvalidChars = 0;

    int over = 0;
    while (!over)
    {
        displayDivider();
        printf("Enter filename (maximum of 30 characters including ");
        printf("file extension).\n");

        // guarantees that the string is 5 chars long and <= MAX_FILE_LEN
        getStrInput(tempFile, 5, MAX_FILE_LEN + 1);

        fileExtIsPresent = isExtPresent(tempFile); // needs to be true
        hasInvalidChars = isInvCharPres(tempFile); // needs to be false

        if (!fileExtIsPresent)
        {
            printf("\nInput has no/incorrect file extension. ");
            printf("Please include the file extension for text ");
            printf("files (.txt).\n");
        }
        else if (hasInvalidChars)
        {
            printf("\nInput has invalid characters for filenames. ");
            printf("Filenames should not have these characters:\n");
            printf("\\ / : * ? \" < > | \n");
        }
        else
        {
            over = 1;
        }
    }

    // assign temporary variable to official variable
    strcpy(filename, tempFile);
}

/**
 * @brief
 *    This function removes symbols from the given string. The symbols to be
 *    removed include the exclamation point (!), question mark (?), comma (,),
 *    and period (.).
 *
 * @param strText   The string where the text that the user wants to translate
 *                  is stored.
 */
void
removeSymbols(String150 strText)
{
    int i = 0; // iterates through the string
    int j = 0; // how many non-symbol characters there are

    while (strText[i] != '\0')
    {
        // if the current character is not a symbol, it is retained in
        // the string, while symbols are skipped
        if (strText[i] != ',' && strText[i] != '.' && strText[i] != '!' &&
            strText[i] != '?')
        {
            strText[j] = strText[i];
            j++;
        }

        i++;
    }

    // null-terminate the string
    strText[j] = '\0';
}

/**
 * @brief
 *    This function encompasses the Add Entry Feature of the the
 *    Manage Data Menu.
 *
 * @param entries         Array of structures containing all the entries and
 *                        language-translation pairs.
 * @param nEntryCount     The current number entries in the entry list.
 * @param nManageChoice   The integer of the user's choice in the Manage Data
 * Menu.
 *
 * @pre   *nEntryCount is >= 0 and <= MAX_ENTRIES.
 * @pre   nManageChoice is >= 1 and <= 10 (amount of choices in the
 *        Manage Data Menu).
 */
void
addEntryFeat(Entry *entries, 
             int *nEntryCount, 
             int nManageChoice)
{
    int nMatches;
    int arrMatched[MAX_ENTRIES]; // array to store the indices of
                                 // the matched entries
    int pairIsInEntry;
    int addEntry = 1; // assume that there will be no matches later
    String20 tempLangVar, tempTransVar;

    // just exit immediately if max amount of entries has been reached
    if (*nEntryCount >= MAX_ENTRIES)
    {
        displayMaxMsg(nManageChoice);
        return;
    }

    // obtain "source" language-translation pair from the user
    getLangTrans(tempLangVar, tempTransVar);

    // find pair only in the "source" language-translation pairs of entries
    pairIsInEntry = findPairInAllEntries(tempLangVar, tempTransVar, 
                                         *nEntryCount, entries, arrMatched,
                                         &nMatches, 0, 0);

    if (pairIsInEntry)
    {
        displayMEntries(entries, arrMatched, nMatches, nManageChoice);

        // ask the user if they want to create a new entry
        addEntry = isNewEntry();
    }

    // create an entry if there were no matching entries earlier or
    // if there were, the user chose to create another one
    if (addEntry)
    {
        // add the lang-trans pair earlier as the "source" pair
        assignNewEntries(entries, *nEntryCount, tempLangVar, tempTransVar);
        displayDivider();
        printf("Format (Language: Translation)\n");
        displayCurrent(entries, *nEntryCount, nManageChoice);

        // user must enter at least one "dest/target" language-translation pair
        // after that, the user may or may not enter more language-translation
        // pairs
        addMoreTrans(entries, *nEntryCount, nManageChoice);
        *nEntryCount += 1;
    }
}

/**
 * @brief
 *    This function encompasses the Add Translation Feature of the the
 *    Manage Data Menu.
 *
 * @param entries         Array of structures containing all the entries and
 *                        language-translation pairs.
 * @param nEntryCount     The current number entries in the entry list.
 * @param nManageChoice   The integer of the user's choice in the
 *                        Manage Data Menu.
 *
 * @pre   *nEntryCount is >= 0 and <= MAX_ENTRIES.
 * @pre   nManageChoice is >= 1 and <= 10 (the amount of choices in the
 *        Manage Data Menu).
 */
void
addTransFeat(Entry *entries, 
             int nEntryCount, 
             int nManageChoice)
{
    int nEntryChoice, nMatches;
    int arrMatched[MAX_ENTRIES]; // array to store the indices of the
                                 // matched entries
    String20 tempLangVar, tempTransVar;
    int pairIsInEntry;

    // obtain "source" lang-trans pair to be searched
    getLangTrans(tempLangVar, tempTransVar);

    // find pair only in the "source" language-translation pairs of entries
    pairIsInEntry = findPairInAllEntries(tempLangVar, tempTransVar,
                                         nEntryCount, entries, arrMatched,
                                         &nMatches, 0, 0);

    // exit immediately if there are no matches
    if (!pairIsInEntry)
    {
        displayNoneMsg(nManageChoice);
        return;
    }

    displayMEntries(entries, arrMatched, nMatches, nManageChoice);

    // if there are more than 1 match, ask the user which entry they wish
    // to add lang-trans pairs to; otherwise, immediately start adding
    // pairs to the sole match
    if (nMatches > 1)
    {
        getEntryChoice(&nEntryChoice, arrMatched, nMatches);
    }
    else
    {
        nEntryChoice = arrMatched[0];
    }

    // exit the add translation feature if selected entry already reached
    // the maximum count of pairs
    if (entries[nEntryChoice].count >= MAX_COUNT)
    {
        displayMaxMsg(nManageChoice);
        return;
    }

    getLangTrans(tempLangVar, tempTransVar);
    assignNewEntries(entries, nEntryChoice, tempLangVar, tempTransVar);
    displayDivider();
    printf("Format (Language: Translation)\n");
    displayCurrent(entries, nEntryChoice, nManageChoice);

    // exit the add translation feature if selected entry already reached
    // the maximum count of pairs
    if (entries[nEntryChoice].count >= MAX_COUNT)
    {
        displayMaxMsg(nManageChoice);
        return; // addMoreTrans() will exit early if so, but this is more
                // explicit
    }

    // if the count limit has not been reached, the user may or may not
    // enter more language-translation pairs
    addMoreTrans(entries, nEntryChoice, nManageChoice);
}

/**
 * @brief
 *    This function encompasses the Delete Entry Feature of the the
 *    Manage Data Menu.
 *
 * @param entries         Array of structures containing all the entries and
 *                        language-translation pairs.
 * @param nEntryCount     The current number entries in the entry list.
 * @param nManageChoice   The integer of the user's choice in the Manage Data
 * Menu.
 *
 * @pre   *nEntryCount is >= 0 and <= MAX_ENTRIES.
 * @pre   nManageChoice is >= 1 and <= 10 (the amount of choices in the
 *        Manage Data Menu).
 */
void
deleteEntryFeat(Entry *entries, 
                int *nEntryCount,
                int nManageChoice)
{
    int nDelChoice;
    int inputIsValid;

    // just exit immediately to avoid a huge if block
    if (*nEntryCount == 0)
    {
        displayNoneMsg(nManageChoice);
        return;
    }

    displayAllEntries(entries, *nEntryCount, nManageChoice);

    // get the index of the entry that the user wants to delete
    inputIsValid = getDelChoice(&nDelChoice, *nEntryCount);
    if (inputIsValid)
    {
        deleteEntry(entries, *nEntryCount, nDelChoice);
        *nEntryCount -= 1;
        displayDivider();
        printf("Entry successfully deleted. ");
        printf("Going back to the Manage Data Menu now...\n");
    }
    else
    {
        displayInvDel();
    }
}

/**
 * @brief
 *    This function encompasses the Delete Translation Feature of the the
 *    Manage Data Menu.
 *
 * @param entries         Array of structures containing all the entries and
 *                        language-translation pairs.
 * @param nEntryCount     The current number entries in the entry list.
 * @param nManageChoice   The integer of the user's choice in the
 *                        Manage Data Menu.
 *
 * @pre    *nEntryCount is >= 0 and <= MAX_ENTRIES.
 * @pre    nManageChoice is >= 1 and <= 10 (the amount of choices in the
 *         Manage Data Menu).
 */
void
deleteTransFeat(Entry *entries, 
                int *nEntryCount, 
                int nManageChoice)
{
    int nDelChoice, nDelIndex;
    int inputIsValid;
    int over = 0;

    // exit immediately to avoid huge if block
    if (*nEntryCount == 0)
    {
        displayNoneMsg(nManageChoice);
        return;
    }

    displayAllEntries(entries, *nEntryCount, nManageChoice);

    // exit immediately if user input is invalid to avoid huge if block
    inputIsValid = getDelChoice(&nDelChoice, *nEntryCount);
    if (!inputIsValid)
    {
        displayInvDel();
        return;
    }

    displayCurrent(entries, nDelChoice - 1, nManageChoice);

    // loop until the user chooses to stop deleting or until the entry
    // itself is deleted
    while (!over)
    {
        inputIsValid = getDelIndex(entries, &nDelIndex, nDelChoice);

        // don't force user to enter valid input
        if (!inputIsValid)
            return;

        // if the last "dest/target" language-translation pair is deleted,
        // delete the entry itself
        if (entries[nDelChoice - 1].count == 2)
        {
            deleteEntry(entries, *nEntryCount, nDelChoice);
            *nEntryCount -= 1;

            printf("\n\nThe whole entry has been deleted. ");
            printf("Going back to the Manage Data Menu now...\n");
            over = 1;
        }
        else
        {
            // if the entry was not deleted, delete the pair and
            // ask the user if they wish to delete more pairs
            deleteTrans(entries, nDelChoice, nDelIndex);
            displayCurrent(entries, nDelChoice - 1, nManageChoice);
            displayDelAgain();
            over = !getUserConfirmation();
        }
    }
}

/**
 * @brief
 *    This function encompasses the Display All Entries Feature of the the
 *    Manage Data Menu.
 *
 * @param entries         Array of structures containing all the entries and
 *                        language-translation pairs.
 * @param nEntryCount     The current number entries in the entry list.
 * @param nManageChoice   The integer of the user's choice in the
 *                        Manage Data Menu.
 *
 * @pre   *nEntryCount is >= 0 and <= MAX_ENTRIES.
 * @pre   nManageChoice is >= 1 and <= 10 (the amount of choices in the
 *        Manage Data Menu).
 */
void
displayAllFeat(Entry *entries, 
               int nEntryCount, 
               int nManageChoice)
{
    if (nEntryCount > 0)
        displayAllEntries(entries, nEntryCount, nManageChoice);
    else
        displayNoneMsg(nManageChoice);
}

/**
 * @brief
 *    This function encompasses the Search Word Feature of the the
 *    Manage Data Menu.
 *
 * @param entries         Array of structures containing all the entries and
 *                        language-translation pairs.
 * @param nEntryCount     The current number entries in the entry list.
 * @param nManageChoice   The integer of the user's choice in the
 *                        Manage Data Menu.
 *
 * @pre   *nEntryCount is >= 0 and <= MAX_ENTRIES.
 * @pre   nManageChoice is >= 1 and <= 10 (the amount of choices in the
 *        Manage Data menu).
 */
void
searchWordFeat(Entry *entries, 
               int nEntryCount, 
               int nManageChoice)
{
    int nMatches;
    int arrMatched[MAX_ENTRIES]; // array to store the indices of
                                 // the matched entries
    int wordIsInEntry;
    String20 strKey;

    getKey(strKey);

    // sort entries before searching to avoid mismatch
    arrangeInterEnt(entries, nEntryCount);
    arrangeIntraEnt(entries, nEntryCount);

    // find word in all of the language-translation pairs of entries
    wordIsInEntry = findWordInAllEntries(entries, strKey, nEntryCount,
                                         arrMatched, &nMatches, 0, -1);

    if (wordIsInEntry)
    {
        // if the word was found at least once, sort all entries,
        // then display the matched entries
        displayMEntries(entries, arrMatched, nMatches, nManageChoice);
    }
    else
    {
        // the word is not found in the list of entries
        displayNoneMsg(nManageChoice);
    }
}

/**
 * @brief
 *    This function encompasses the Search Translation Feature of the the
 *    Manage Data Menu.
 *
 * @param entries          Array of structures containing all the entries and
 *                         language-translation pairs.
 * @param nEntryCount      The current number entries in the entry list.
 * @param nManageChoice    The integer of the user's choice in the
 *                         Manage Data Menu.
 *
 * @pre   *nEntryCount is >= 0 and <= MAX_ENTRIES.
 * @pre   nManageChoice is >= 1 and <= 10 (the amount of options in the
 *        Manage Data Menu).
 */
void
searchTransFeat(Entry *entries, 
                int nEntryCount, 
                int nManageChoice)
{
    int nMatches;
    int arrMatched[MAX_ENTRIES]; // array to store the indices of the matched
                                 // entries
    int pairIsInEntry;
    String20 tempLangVar, tempTransVar;

    getLangTrans(tempLangVar, tempTransVar);

    // sort entries before searching to avoid mismatch
    arrangeInterEnt(entries, nEntryCount);
    arrangeIntraEnt(entries, nEntryCount);

    // find pair in all of the language-translation pairs of entries
    pairIsInEntry = findPairInAllEntries(tempLangVar, tempTransVar,
                                         nEntryCount, entries, arrMatched,
                                         &nMatches, 0, -1);

    if (pairIsInEntry)
        displayMEntries(entries, arrMatched, nMatches, nManageChoice);
    else
        displayNoneMsg(nManageChoice);
}

/**
 * @brief
 *    This function encompasses the Export Feature of the the
 *    Manage Data Menu.
 *
 * @param entries          Array of structures containing all the entries and
 *                         language-translation pairs.
 * @param nEntryCount      The current number entries in the entry list.
 * @param nManageChoice    The integer of the user's choice in the
 *                         Manage Data Menu.
 *
 * @pre   *nEntryCount is >= 0 and <= MAX_ENTRIES.
 * @pre   nManageChoice is >= 1 and <= 10 (the amount of options in the
 *        Manage Data Menu).
 */
void
exportFeat(Entry *entries, 
           int nEntryCount, 
           int nManageChoice)
{
    int i, j;
    FILE *fp_export = NULL;
    String30 filename;

    // immediately exit if there are no entries to export
    if (nEntryCount == 0)
    {
        displayNoneMsg(nManageChoice);
        return;
    }

    getFileName(filename);
    fp_export = fopen(filename, "w");

    // exit immediately if file somehow cannot be accessed/written to
    if (fp_export == NULL)
    {
        printf("Something went wrong! Exiting...\n");
        return;
    }

    // write data to file
    for (i = 0; i < nEntryCount; i++)
    {
        for (j = 0; j < entries[i].count; j++)
        {
            fprintf(fp_export, "%s: %s\n", entries[i].lang[j], 
                    entries[i].trans[j]);
        }

        fprintf(fp_export, "\n");
    }

    fclose(fp_export);

    displayDivider();
    printf("File export complete!\n");
}

/**
 * @brief
 *    This function encompasses the Import Feature of the the Manage Data Menu.
 *    The function sscanf () was used in this function to parse strings.
 *    This topic was self-studied as it is outside the topics discussed in
 *    CCPROG2. Lesson Reference: https://www.youtube.com/watch?v=-7cSmcdMryo
 *
 * @param entries       Array of structures containing all the entries and
 *                      language-translation pairs.
 * @param nEntryCount   The current number entries in the entry list.
 *
 * @pre   *nEntryCount is >= 0 and <= MAX_ENTRIES.
 */
void
importFeat(Entry *entries, 
           int *nEntryCount)
{
    FILE *fp_import = NULL;
    String30 filename;
    char lineInput[STR30LEN * 2];
    int i, j;
    int dataParsed;
    int loadEntry;

    // variables for the temporary entry structure
    Entry tempEntry = {};
    String20 tempLangVar, tempTransVar;

    i = 1; // amount of entries scanned (not necessarily imported)

    getFileName(filename);
    displayDivider();

    // exit immediately if file does not exist or cannot be accessed
    fp_import = fopen(filename, "r");
    if (fp_import == NULL)
    {
        printf("File does not exist or cannot be accessed.\n");
        return;
    }
    else
    {
        printf("File successfully loaded.\n");
    }

    // continue reading the file as long as EOF isn't encountered (fgets()
    // returns NULL) and the maximum amount of entries has not been reached
    while (fgets(lineInput, STR30LEN * 2, fp_import) != NULL &&
           *nEntryCount < MAX_ENTRIES)
    {
        dataParsed = sscanf(lineInput, "%[^:\n]: %[^:\n]", tempLangVar, 
                            tempTransVar);

        // if both the language and translation strings have
        // been successfully parsed, store them in the temporary variable
        if (dataParsed == 2 && tempEntry.count < MAX_COUNT)
        {
            titleCase(tempLangVar);
            lowercase(tempTransVar);
            copyLangTLPair(tempEntry.lang[tempEntry.count],
                           tempEntry.trans[tempEntry.count], 
                           tempLangVar, tempTransVar);
            tempEntry.count++;

            // the entry has ended
        }
        else
        {
            // assume the user wants to load the entry
            loadEntry = 1;

            // display the scanned entry
            displayDivider();
            printf("Format (Language: Translation)\n\n");
            printf("Loaded Entry No. %d\n", i);
            for (j = 0; j < tempEntry.count; j++)
            {
                printf("(%d) %s: %s\n", j + 1, tempEntry.lang[j],
                                               tempEntry.trans[j]);
            }

            // ask the user if they wish to import the loaded entry
            printf("\nDo you want to add the loaded ");
            printf("entry to your data? ");
            loadEntry = getUserConfirmation();

            if (loadEntry)
            {
                entries[*nEntryCount] = tempEntry;
                *nEntryCount += 1;
            }

            i++;                    // increment amount of entries scanned
            clearEntry(&tempEntry); // prepare temp entry for next entry
        }
    }

    if (*nEntryCount >= MAX_ENTRIES)
        printf("\nThe maximum amount of entries has already been reached!\n");

    printf("\nFile import complete!\n");

    fclose(fp_import);
}

/**
 * @brief
 *    This function encompasses the Translate Feature of the Translate Menu.
 *    The strtok () function was used in this function to tokenize strings.
 *    This topic was self-studied as it is outside the topics discussed in
 *    CCPROG2. Lesson Reference: https://www.youtube.com/watch?v=-7cSmcdMryo
 *
 * @param entries       Array of structures containing all the entries and
 *                      language-translation pairs.
 * @param nEntryCount   The current number entries in the entry list.
 *
 * @pre   nEntryCount is >= 0 and <= MAX_ENTRIES.
 */
void
translateFeat(Entry *entries, 
              int nEntryCount)
{
    String20 sourceLang, destLang;
    String150 strText;

    char *word;
    int i, j;
    int over = 0;
    int found;
    
    // obtain the source and destination languages of the text
    getLang(sourceLang, 1);
    getLang(destLang, 2);

    while (!over)
    {
        displayDivider();
        printf("Enter Text (maximum of 150 characters).\n");
        getStrInput(strText, 1, MAX_TEXT_LEN + 1);
        removeSymbols(strText);

        printf("\nTranslated Text:\n");

        // tokenize the first word in the text
        word = strtok(strText, " ");

        // until the string has ended, attempt to translate each token
        while (word != NULL)
        {
            found = 0;
            for (i = 0; i < nEntryCount && !found; i++)
            {
                // iterate through each entry and check each "source" pairs
                if (!strcmp(sourceLang, entries[i].lang[0]) &&
                    !strcmp(word, entries[i].trans[0]))
                {
                    // once the "source" pairs have matched, find the matching
                    // destination pair in the same entry
                    for (j = 1; j < entries[i].count && !found; j++)
                    {
                        // if found, print the translation of that word, if not
                        // let the system iterate until the last entry
                        if (!strcmp(destLang, entries[i].lang[j]))
                        {
                            printf("%s", entries[i].trans[j]);
                            found = 1; // stop both loops
                        }
                    }
                }
                
                if (!found && i == nEntryCount - 1)
                {
                    // if the system has iterated through the last entry
                    // (nEntryCount - 1) and the matching source and dest.
                    // pairs have not been found, print the original word
                    printf("%s", word);
                }
            }

            // tokenize the next word in the text
            word = strtok(NULL, " ");

            if (word != NULL)
                printf(" ");
        }

        printf("\n");
        displayDivider();

        printf("Do you want to translate another text from the same ");
        printf("source and target languages? ");
        over = !getUserConfirmation();
    }

    displayDivider();
    printf("Going back to the Translate Menu now...\n");
}

int main()
{
    Entry entries[MAX_ENTRIES] = {};
    int nEntryCount = 0;
    int nMainChoice = 0;
    int nManageChoice, nTransChoice;
    int exitMenu;

    // loop the system until the user exits from the Main Menu
    while (nMainChoice != 3)
    {
        displayMainMenu();
        getMainChoice(&nMainChoice);

        if (nMainChoice == 1)
        {
            // Manage Data Menu
            exitMenu = 0;
            while (!exitMenu)
            {
                displayManageMenu();
                getManageChoice(&nManageChoice);
                switch (nManageChoice)
                {
                    case 1:
                        addEntryFeat(entries, &nEntryCount, nManageChoice);
                        break;
                    case 2:
                        addTransFeat(entries, nEntryCount, nManageChoice);
                        break;
                    case 3:
                        deleteEntryFeat(entries, &nEntryCount, nManageChoice);
                        break;
                    case 4:
                        deleteTransFeat(entries, &nEntryCount, nManageChoice);
                        break;
                    case 5:
                        displayAllFeat(entries, nEntryCount, nManageChoice);
                        break;
                    case 6:
                        searchWordFeat(entries, nEntryCount, nManageChoice);
                        break;
                    case 7:
                        searchTransFeat(entries, nEntryCount, nManageChoice);
                        break;
                    case 8:
                        exportFeat(entries, nEntryCount, nManageChoice);
                        break;
                    case 9:
                        importFeat(entries, &nEntryCount);
                        break;
                    case 10:
                        exitMenu = 1;
                        break;
                }
            }
        }
        else if (nMainChoice == 2)
        {
            // Translate Menu
            exitMenu = 0;
            while (!exitMenu)
            {
                // there must be at least one entry before proceeding
                while (nEntryCount == 0)
                {
                    displayDivider();
                    printf("There must be at least one entry loaded ");
                    printf("to proceed to the Translate Menu.\n");
                    importFeat(entries, &nEntryCount);
                }

                displayTransMenu();
                getTransChoice(&nTransChoice);

                switch (nTransChoice)
                {
                    case 1:
                        translateFeat(entries, nEntryCount);
                        break;
                    case 2:
                        exitMenu = 1;
                        break;
                }
            }
        }

        // clear all entries once the user exits either the
        // Manage Data or the Translate Menu
        emptyEntry(entries, &nEntryCount);
    }

    return 0;
}