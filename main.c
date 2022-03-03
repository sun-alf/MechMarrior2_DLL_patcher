/******************************************************************************
*
*
*
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "PatchWpn.h"

#define DEFAULT_MW2_DLL_NAME         ("mercsw.dll")
#define DEFAULT_MW2SHELL_DLL_NAME    ("MW2SHELL.DLL")
#define DEFAULT_MW2_DLL_OFFSET       (0x00098638)
#define DEFAULT_MW2SHELL_DLL_OFFSET  (0x0005CC0C)
#define DEFAULT_WEAPONS_AMOUNT       (66)

#define WEAPONS_RESERVED_SLOTS       (256)


typedef enum
{
	MENU_EXIT = 0,
	MENU_EXPORT_DATA,
	MENU_IMPORT_DATA,
	MENU_PATCH_DATA,
	MENU_INVALID_INPUT,
	MENU_TOTAL_ITEMS
} TMenuItems;

extern void ClearScreen();
extern void WaitAnyKey();

static bool _LoadWeapons(char* dllName, long int offset, tWeapon* weapons, int cnt)
{
	bool result = true;
    FILE* f = fopen(dllName, "rb");
    if (f != NULL)
    {
        fseek(f, offset, SEEK_SET);
        for (int i = 0; i < cnt; i++)
        {
            fread(&weapons[i], sizeof(tWeapon), 1, f);
        }
        fclose(f);
    }
    else
    {
        printf("%s: failed to open file %s\n", __FUNCTION__, dllName);
        result = false;
    }
    return result;
}

static bool _LoadShellWeapons(char* dllName, long int offset, tShellWeapon* weapons, int cnt)
{
	bool result = true;
    FILE* f = fopen(dllName, "rb");
    if (f != NULL)
    {
        fseek(f, offset, SEEK_SET);
        for (int i = 0; i < cnt; i++)
        {
            fread(&weapons[i], sizeof(tShellWeapon), 1, f);
        }
        fclose(f);
    }
    else
    {
        printf("%s: failed to open file %s\n", __FUNCTION__, dllName);
        result = false;
    }
    return result;
}

static bool _SaveWeapons(char* dllName, long int offset, tWeapon* weapons, int cnt)
{
	bool result = true;
    FILE* f = fopen(dllName, "r+cb");
    if (f != NULL)
    {
        fseek(f, offset, SEEK_SET);
        for (int i = 0; i < cnt; i++)
        {
            if (fwrite(&weapons[i], sizeof(tWeapon), 1, f) != 1)
            {
            	printf("%s: failed to patch file %s\n", __FUNCTION__, dllName);
            	break;
            }
        }
        fclose(f);
    }
    else
    {
        printf("%s: failed to open file %s\n", __FUNCTION__, dllName);
        result = false;
    }
    return result;
}

static bool _SaveShellWeapons(char* dllName, long int offset, tShellWeapon* weapons, int cnt)
{
	bool result = true;
    FILE* f = fopen(dllName, "r+cb");
    if (f != NULL)
    {
        fseek(f, offset, SEEK_SET);
        for (int i = 0; i < cnt; i++)
        {
            if (fwrite(&weapons[i], sizeof(tShellWeapon), 1, f) != 1)
            {
            	printf("%s: failed to patch file %s\n", __FUNCTION__, dllName);
            	break;
            }
        }
        fclose(f);
    }
    else
    {
        printf("%s: failed to open file %s\n", __FUNCTION__, dllName);
        result = false;
    }
    return result;
}

static int _MightyPrintf(FILE* f, const char* fmt, ...)
{
    va_list ap;
    if (f != NULL)
    {
    	va_start(ap, fmt);
    	vfprintf(f, fmt, ap);
    	va_end(ap);
    }
    else
    {
    	va_start(ap, fmt);
    	vprintf(fmt, ap);
    	va_end(ap);
    }
}

static void _PrintWeapons(tWeapon* weapons, tShellWeapon* shWeapons, int cnt, FILE* f)
{
    for (int i = 0; i < cnt; i++)
    {
        char nullTermName[SHORT_NAME_LENGTH + 1] = { 0 };
        memcpy(nullTermName, weapons[i].sShortName, SHORT_NAME_LENGTH);
    	_MightyPrintf( f,
    		"=== Item #%d =================\n"
    		"sShortName:               %s\n"
    		"newDword0:                %u (0x%08X)\n"
    		"newDword1:                %u (0x%08X)\n"
    		"newDword2:                %u (0x%08X)\n"
    		"newDword3:                %u (0x%08X)\n"
			"lShotType:                %u (0x%08X)\n"
    		"lHitType:                 %u (0x%08X)\n"
    		"lUnknown1:                %u (0x%08X)\n"
    		"lUnknown2:                %u (0x%08X)\n"
    		"lContinuousFiring:        %u (0x%08X)\n"
    		"lWeaponEnabled:           %u (0x%08X)\n"
    		"lHoming:                  %u (0x%08X)\n"
    		"lShotsPerAttack:          %u (0x%08X)\n"
    		"lAttacksPerTon:           %u (0x%08X)\n"
			"lSoundFX:                 %u (0x%08X)\n"
    		"lShotGravity:             %u (0x%08X)\n"
    		"lShotSpeed:               %u (0x%08X)\n"
    		"lDamageInflictedPerShot:  %u (0x%08X)\n"
    		"lHeatGeneratedPerShot:    %u (0x%08X)\n"
    		"lHeatInflictedPerShot:    %u (0x%08X)\n"
    		"lMinLockRange:            %u (0x%08X)\n"
    		"lRange:                   %u (0x%08X)\n"
    		"lReloadTime:              %u (0x%08X)\n"
    		"lFiringDelay:             %u (0x%08X)\n"
    		"lShotLifespan:            %u (0x%08X)\n"
    		"shell Range:              %u (0x%08X)\n"
    		"shell Mass:               %u (0x%08X)\n"
    		"shell Criticals (size):   %u (0x%08X)\n"
    		"shell AttacksPerTon:      %u (0x%08X)\n\n",
    		i, nullTermName,
    	    weapons[i].newDword0, weapons[i].newDword0,
    		weapons[i].newDword1, weapons[i].newDword1,
    		weapons[i].newDword2, weapons[i].newDword2,
    		weapons[i].newDword3, weapons[i].newDword3,
			weapons[i].lShotType, weapons[i].lShotType,
    		weapons[i].lHitType, weapons[i].lHitType,
    		weapons[i].lUnknown1, weapons[i].lUnknown1,
    		weapons[i].lUnknown2, weapons[i].lUnknown2,
    		weapons[i].lContinuousFiring, weapons[i].lContinuousFiring,
    		weapons[i].lWeaponEnabled, weapons[i].lWeaponEnabled,
    		weapons[i].lHoming, weapons[i].lHoming,
    		weapons[i].lShotsPerAttack, weapons[i].lShotsPerAttack,
    		weapons[i].lAttacksPerTon, weapons[i].lAttacksPerTon,
			weapons[i].lSoundFX, weapons[i].lSoundFX,
    		weapons[i].lShotGravity, weapons[i].lShotGravity,
    		weapons[i].lShotSpeed, weapons[i].lShotSpeed,
    		weapons[i].lDamageInflictedPerShot, weapons[i].lDamageInflictedPerShot,
    		weapons[i].lHeatGeneratedPerShot, weapons[i].lHeatGeneratedPerShot,
    		weapons[i].lHeatInflictedPerShot, weapons[i].lHeatInflictedPerShot,
    		weapons[i].lMinLockRange, weapons[i].lMinLockRange,
    		weapons[i].lRange, weapons[i].lRange,
    		weapons[i].lReloadTime, weapons[i].lReloadTime,
    		weapons[i].lFiringDelay, weapons[i].lFiringDelay,
    		weapons[i].lShotLifespan, weapons[i].lShotLifespan,
    		shWeapons[i].lRange, shWeapons[i].lRange,
    		shWeapons[i].lMass, shWeapons[i].lMass,
    		shWeapons[i].lCritical, shWeapons[i].lCritical,
    		shWeapons[i].lAttacksPerTon, shWeapons[i].lAttacksPerTon);
    }
}

static bool _ImportWeapons(tWeapon* weapons, tShellWeapon* shWeapons, int cnt, FILE* f)
{
	typedef enum
	{
		__PARSER_STATE_NOTHING = 0,
		__PARSER_STATE_WEAPON,
		__PARSER_STATE_WEAPON_NAME,
		__PARSER_STATE_WEAPON_PROPS,
	} __TParserStates;
	
	const char indexMarker[] = "=== Item #";
	const char propertyIndent[] = "sShortName:               ";  // indent before a property value is same for all properties
	const int indexMarkerLength = sizeof(indexMarker) - 1;  // exclude null-terminator!
	const int propertyIndentLength = sizeof(propertyIndent) - 1;  // exclude null-terminator!
	const int propertiesCnt = (sizeof(tWeapon) - SHORT_NAME_LENGTH) / sizeof(unsigned int);  // all props are of the same size
	const int shellPropertiesCnt = 4;  // support only 4 meaningful props in each Shell Weapon entry, just because
	
	bool result = true;
	__TParserStates parserState = __PARSER_STATE_NOTHING;
	int selectedIndex = -1;
	int linesCtr = 0;
	char line[256] = { 0 };

	int propsCtr = 0;
    while (feof(f) == 0 && result == true)
    {
        char* res = fgets(line, 256, f);
        linesCtr++;
        
        int lineLen = strlen(line);
        if (lineLen > 0 && line[lineLen - 1] == '\n')  // clean up EOL char if any
        	line[lineLen - 1] = '\0';
        
        if (strlen(line) > 0)  // skip empty line otherwise
        {
        	if (parserState == __PARSER_STATE_NOTHING)
        	{
        		char part[sizeof(indexMarker)] = { 0 };
        		memcpy(part, line, indexMarkerLength);
        		if (strcmp(part, indexMarker) == 0)  // if the line starts with "=== Item #"
        		{
        			int idx = atoi(&line[indexMarkerLength]);  // then take a number right after the marker
        			if (idx >= 0 && idx < cnt)  // check if that number is a valid index
        			{
        				parserState = __PARSER_STATE_WEAPON;
        				selectedIndex = idx;
        				//printf("%d: selectedIndex = %d\n", linesCtr, idx);  //TEMP:
        			}
        			else
        			{
        				result = false;  // stop parsing and leave
        			}
        		}
        		else
        		{
        			result = false;  // stop parsing and leave
        		}
        	}
        	else if (parserState == __PARSER_STATE_WEAPON)
        	{
        		strcpy(weapons[selectedIndex].sShortName, &line[propertyIndentLength]);
        		weapons[selectedIndex].sShortName[SHORT_NAME_LENGTH - 1] = '\0';
        		parserState = __PARSER_STATE_WEAPON_NAME;
        		propsCtr = 0;
        		//printf("%d: name = %s\n", linesCtr, weapons[selectedIndex].sShortName);  //TEMP:
        	}
        	else if (parserState == __PARSER_STATE_WEAPON_NAME)
        	{
        		unsigned int val = (unsigned int) atol(&line[propertyIndentLength]);
        		unsigned int* props = (unsigned int*) &weapons[selectedIndex];
        		props[propsCtr] = val;
        		propsCtr++;
        		
        		if (propsCtr == propertiesCnt)
        		{
        			parserState = __PARSER_STATE_WEAPON_PROPS;
        			//printf("%d: prop idx = %d, val = %u\n", linesCtr, propsCtr - 1, props[propsCtr-1]);  //TEMP:
        			propsCtr = 0;
        		}
        	}
        	else if (parserState == __PARSER_STATE_WEAPON_PROPS)
        	{
        		unsigned int val = (unsigned int) atol(&line[propertyIndentLength]);
        		unsigned int* props = (unsigned int*) &shWeapons[selectedIndex].lRange;
        		props[propsCtr] = val;
        		propsCtr++;
        		
        		if (propsCtr == shellPropertiesCnt)
        		{
        			parserState = __PARSER_STATE_NOTHING;
        			//printf("%d: prop idx = %d, val = %u\n", linesCtr, propsCtr - 1, props[propsCtr-1]);  //TEMP:
        		}
        	}
        }
    }
    
    if (result == false)
    	printf("%s: input file is broken around line #%d:\n\"%s\"\n", __FUNCTION__, linesCtr, line);
    //WaitAnyKey();  //TEMP:
    return result;
}

static TMenuItems _MainMenu()
{
	printf( "Main menu:\n"
			"%d. Export weapons from DLLs\n"
			"%d. Import weapons from text file\n"
			"%d. Patch DLLs\n"
			"%d. Exit program\n",
			MENU_EXPORT_DATA, MENU_IMPORT_DATA, MENU_PATCH_DATA, MENU_EXIT);
	printf("\nChoose menu item: ");
	char input[256] = { 0 };
	int inputRes = scanf("%s", input);
	int menuItem = atoi(input);
	return (TMenuItems) (inputRes == 1 && menuItem < MENU_TOTAL_ITEMS ? menuItem : MENU_INVALID_INPUT);
}

int main(int argc, char* argv[])
{
	int dllWeaponsCnt = DEFAULT_WEAPONS_AMOUNT;
	char dllName[256] = { 0 };
	char dllShellName[256] = { 0 };
	int dllOffset = DEFAULT_MW2_DLL_OFFSET;
	int dllShellOffset = DEFAULT_MW2SHELL_DLL_OFFSET;

	strcpy(dllName, DEFAULT_MW2_DLL_NAME);
	strcpy(dllShellName, DEFAULT_MW2SHELL_DLL_NAME);

    tWeapon weapons[WEAPONS_RESERVED_SLOTS] = { 0 };
    tShellWeapon shellWeapons[WEAPONS_RESERVED_SLOTS] = { 0 };
    bool readyToPatch = false;
    
    TMenuItems menuItem = MENU_EXIT;
    do
    {
    	ClearScreen();
    	printf( "\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
    			"\t\t~~~ Weapons patcher for game:\n"
    			"\t\t~~~ MechWarrior 2: 31st Century Combat (Titanium edition)\n"
    			"\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n");
    	printf("DLL info:\n  Total amount of weapons = %d\n  DLL name = %s  (start offset = 0x%08X)\n  Shell DLL name = %s  (start offset = 0x%08X)\n\n",
    			dllWeaponsCnt, dllName, dllOffset, dllShellName, dllShellOffset);
    	
    	bool opSucceed = true;
    	menuItem = _MainMenu();
		switch (menuItem)
		{
			// Read weapon data from DLLs and put it into a file
			case MENU_EXPORT_DATA:
			    opSucceed &= _LoadWeapons((char*)dllName, dllOffset, weapons, dllWeaponsCnt);
    			opSucceed &= _LoadShellWeapons((char*)dllShellName, dllShellOffset, shellWeapons, dllWeaponsCnt);
				if (opSucceed)
				{
					char outFileName[256] = { 0 };
					printf("Enter output file name: ");
					scanf("%s", outFileName);
					
					FILE* f = fopen(outFileName, "w");
					if (f != NULL)
					{
						_PrintWeapons(weapons, shellWeapons, dllWeaponsCnt, f);
						fclose(f);
					}
					else
					{
						printf("%s: failed to open file %s\n", __FUNCTION__, outFileName);
						opSucceed = false;
					}
				}
				break;

			// Input textual file may contain not all the weapons, so read everything out from DLLs first,
			// then read contents are provided in the text file and override.
			case MENU_IMPORT_DATA:
			    opSucceed &= _LoadWeapons((char*)dllName, dllOffset, weapons, dllWeaponsCnt);
    			opSucceed &= _LoadShellWeapons((char*)dllShellName, dllShellOffset, shellWeapons, dllWeaponsCnt);
    			if (opSucceed)
    			{
					char inFileName[256] = { 0 };
					printf("Enter input file name: ");
					scanf("%s", inFileName);
					
					FILE* f = fopen(inFileName, "r");
					if (f != NULL)
					{
						opSucceed &= _ImportWeapons(weapons, shellWeapons, dllWeaponsCnt, f);
						fclose(f);
						//{FILE* test = fopen("test.txt", "w"); _PrintWeapons(weapons, shellWeapons, dllWeaponsCnt, test); fclose(test);}	//TEMP:
					}
					else
					{
						printf("%s: failed to open file %s\n", __FUNCTION__, inFileName);
						opSucceed = false;
					}
				}
				
				readyToPatch = opSucceed;
				break;

			// Put all weapon data from memory ('weapons', 'shellWeapons') to DLLs
			case MENU_PATCH_DATA:
				if (readyToPatch)
				{
			    	opSucceed &= _SaveWeapons((char*)dllName, dllOffset, weapons, dllWeaponsCnt);
    				opSucceed &= _SaveShellWeapons((char*)dllShellName, dllShellOffset, shellWeapons, dllWeaponsCnt);
				}
				else
				{
					printf("\nNo. Export (if not yet), edit carefully, and import back. Then use this option.\n");
					opSucceed = false;
				}
				break;
		}
		
		if (opSucceed == false)
		{
			WaitAnyKey();
		}
    } while (menuItem != MENU_EXIT);
    //printf("Hello World\n long = %lu, int = %lu\n", sizeof(long), sizeof(int));
    
    //_LoadWeapons((char*)DEFAULT_MW2_DLL_NAME, DEFAULT_MW2_DLL_OFFSET, weapons);
    //_LoadShellWeapons((char*)DEFAULT_MW2SHELL_DLL_NAME, DEFAULT_MW2SHELL_DLL_OFFSET, shellWeapons);
    //_PrintWeapons(weapons, shellWeapons, false);
/*    FILE* f = fopen("Readme.txt", "r+");
    fseek(f, 0, SEEK_SET);
    
    char buf[256];
    fread(buf, 1, 255, f);
    buf[255] = 0;
    printf("read: %s\n", buf);

    fwrite("Ololo trololo", 1, sizeof("Ololo trololo"), f);
    fclose(f);*/

    return 0;
}
