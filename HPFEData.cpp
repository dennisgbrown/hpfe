#include "stdafx.h"
#include <stdio.h>
#include "HPFEData.h"
#include "SysObj.h"
#include "GameEntry.h"
#include "sharedVars.h"


///////////////////////////////////
//                               //
// Constructors and Destructors  //
//                               //
///////////////////////////////////

// Constructor just fills in information from the parameters.
HPFEData::HPFEData(FILE *debugFile) : debug(debugFile) 
{

}


// Destructor cleanly deletes the object. (!!! someday maybe)
HPFEData::~HPFEData()
{

}


//////////////////////
//                  //
// Helper functions //
//                  //
//////////////////////

// Test if this character is a whitespace character.
bool isWhitespace(char testMe)
{
  return ((testMe == ' ') || (testMe == '\t') || (testMe == '\n'));
}


// Return the string without leading or trailing whitespace.
char *trimWhitespace(char *cleanMe)
{
  int start = -1;
  int end = -1;
  int i;

  if (cleanMe == NULL) return NULL;

  for (i = 0; i < strlen(cleanMe); i++)
  {
    if (!isWhitespace(cleanMe[i]))
    {
      start = i;
      break;
    }
  }

  if (start == -1) return NULL;

  for (i = (strlen(cleanMe) - 1); i > start; i--)
  {
    if (!isWhitespace(cleanMe[i]))
    {
      end = i;
      break;
    }
  }

  cleanMe[end + 1] = '\0';
  return (cleanMe + (sizeof(char) * start));
}


// My string compare is case-INsensitive, otherwise like strcmp.
int mystrcmp(char *s1, char *s2)
{
  if (s1 == NULL) return 1;
  if (s2 == NULL) return -1;

  int s1len = strlen(s1);
  int s2len = strlen(s2);

  char diff = 'A' - 'a';
  char a;
  char b;

  for (int i = 0; i < s1len; i++)
  {
    if (i >= s2len) return 1;
    if ((s1[i] >= 'a') && (s1[i] <= 'z')) a = s1[i] + diff;
    else a = s1[i];
    if ((s2[i] >= 'a') && (s2[i] <= 'z')) b = s2[i] + diff;
    else b = s2[i];
    if (a > b) return 1;
    if (a < b) return -1;
  }

  return 0;
}


// Given a column and a game entry, get that column's info out of the 
// game entry.
char *getGameEntryString(Column *col, GameEntry *ge)
{
  switch (col->type)
  {
  case NAME_COL: return ge->name;
  case FILENAME_COL: return ge->filename;
  case SYSTEM_COL: return ge->system;
  case EMULATOR_COL: return ge->emulator;
  case COMMENTS_COL: return ge->comments;
  case MAKER_COL: return ge->maker;
  case YEAR_COL: return ge->year;
  case NEEDTOUNZIP_COL: return ge->needToUnzip;
  case ZIPFILE_COL: return ge->zipFile;
  case FAVORITE_COL: return ge->favorite;
  case COMMANDLINE_COL: return ge->commandLine;
  case WORKINGDIR_COL: return ge->workingDir;
  }
  return NULL;
}


//////////////////////////
//                      //
// Public class methods //
//                      //
//////////////////////////

// Read in the file of game systems information.
// Then process the information.
void HPFEData::readConfigFile(ifstream configFile)
{
  char inLine[defaultStringLength];

  while (!configFile.eof())
  {
    configFile.getline(inLine, defaultStringLength);
    if (strlen(inLine) < 1) continue;  // empty line
    if (inLine[0] == '#') continue; // comment line
    addToConfigTokens(inLine);
  } 

  // Process the configuration tokens.
  processConfigTokens();
}


// Read in the file of game systems information into the system tokens.
// Then process the information.
void HPFEData::readSystemsFile(ifstream systemsFile)
{
  char inLine[defaultStringLength];

  char *systemName = NULL;
  int systemNum = 0;
  char systemNumToken[defaultStringLength];

  // Until the end of the file...
  while (!systemsFile.eof())
  {
    systemsFile.getline(inLine, defaultStringLength);
    //fprintf(debug, "%s\n", inLine);

    if (strlen(inLine) < 1) continue;  // empty line

    if (inLine[0] == '#') continue; // comment line

    if (inLine[0] == '[') // new system
    {
      if (strlen(inLine) < 2) continue; // garbage line
      systemName = strtok(inLine + sizeof(char), "]");
      if (systemName == NULL) continue;
      systemNum++;
      //fprintf(debug, "Found system #%d: %s\n", systemNum, systemName);
      sprintf(systemNumToken, "system%d", systemNum);
      systemTokens.push_back(token(strdup(systemNumToken), strdup(systemName)));
    }
    else
      addToSystemTokens(systemNum, inLine);
  }

  processSystemTokens();
}


// Put data into the list control.
void HPFEData::fillListCtrl(CListCtrl* ROMLIST)
{
  // Insert the columns according to the column display order.
  for (int i = 0; i < columns.size(); i++)
  {
    int j = columnDisplayOrder[i];
    ROMLIST->InsertColumn(i, (columns[j])->display, (columns[j])->align, 
                          (columns[j])->width, -1); 
  }

  // Insert game information into each row.
  for (int currRow = 0; currRow < LCRowToSysObj.size(); currRow++)
  {
    GameEntry *ge = getGameEntryFromSortedListCtrl(currRow);
    ROMLIST->InsertItem(currRow, ge->name);
    for (i = 0; i < columns.size(); i++)
    {
      char *displayString = getGameEntryString(columns[columnDisplayOrder[i]], ge);
      if (displayString == NULL) displayString = "";
      ROMLIST->SetItemText(currRow, i, displayString);
    }
  }
}


// Build the data used by the application (mainly list control data).
void HPFEData::buildInitialData()
{
  // For each system...
  for (int so = 0; so < sysObjs.size(); so++)
  {
    // For each game entry for that system...
    for (int ge = 0; ge < ((sysObjs[so])->games).size(); ge++)
    {
      // Push the system object and game entry into the LCRow maps.
      // Assuming everything inserts in order (it should!), the
      // systems and games will be inserted into these maps in the
      // order they would appear in the list control IF IT WERE UNSORTED.
      LCRowToSysObj.push_back(so);
      LCRowToGameEntry.push_back(ge);

      // Fill the game entry with information from the system object.
      fillGameEntry(((sysObjs[so])->games)[ge], sysObjs[so]);
    }
  }

  // Sort the rows of the list control according to the sort order.
  sortLCRows();

  fprintf(debug, "total num games = %d\n", LCRowToSysObj.size());
}


// Given a sorted list control row number, get its game entry.
GameEntry *HPFEData::getGameEntryFromSortedListCtrl(int LCRow)
{
  int SysObjIndex = LCRowToSysObj[sortedLCRowToUnsortedLCRow[LCRow]];
  int GameEntryIndex = LCRowToGameEntry[sortedLCRowToUnsortedLCRow[LCRow]];
  return (sysObjs[SysObjIndex])->games[GameEntryIndex];
}


// Given a sorted list control row number, get its system object.
SysObj *HPFEData::getSysObjFromSortedListCtrl(int LCRow)
{
  int SysObjIndex = LCRowToSysObj[sortedLCRowToUnsortedLCRow[LCRow]];
  return sysObjs[SysObjIndex];
}


// Given an unsorted list control row number, get its game entry.
GameEntry *HPFEData::getGameEntryFromUnsortedListCtrl(int LCRow)
{
  int SysObjIndex = LCRowToSysObj[LCRow];
  int GameEntryIndex = LCRowToGameEntry[LCRow];
  return (sysObjs[SysObjIndex])->games[GameEntryIndex];
}


// Given an unsorted list control row number, get its system object.
SysObj *HPFEData::getSysObjFromUnsortedListCtrl(int LCRow)
{
  int SysObjIndex = LCRowToSysObj[LCRow];
  return sysObjs[SysObjIndex];
}


///////////////////////////
//                       //
// Private class methods //
//                       //
///////////////////////////

// Given a string of form "tokenName = tokenValue," add it to
// the config token vectors.
void HPFEData::addToConfigTokens(char *tokenLine)
{
  if (tokenLine == NULL) return;

  // Extract token name and value. 
  char *tokenName = strtok(tokenLine, "=");
  char *tokenValue = strtok(NULL, "\0");
  //fprintf(debug, "!!%s!! equals !!%s!!\n", tokenName, tokenValue);

  // Trim the surrounding whitespace.
  char *cleanTokenName = trimWhitespace(tokenName);
  char *cleanTokenValue = trimWhitespace(tokenValue);
  //fprintf(debug, "* token name:%s\n  token value:%s\n", cleanTokenName, cleanTokenValue);

  // If both tokens are not null, add then to the token vectors.
  if ((cleanTokenName != NULL) && (cleanTokenValue != NULL))
  {
    //fprintf(debug, "Found token name:%s, token value:%s.\n", finalTokenName, cleanTokenValue);
    configTokens.push_back(token(strdup(cleanTokenName), strdup(cleanTokenValue)));
  }
}


// Given a token name, return its value.
char *HPFEData::findConfigToken(char *tokenName)
{
  // Iterator to loop through the vector.
  TokenVectorIt currToken;

  for (currToken = configTokens.begin(); currToken != configTokens.end(); currToken++)
  {
    if (!strcmp(tokenName, currToken->name))
      return currToken->value;
  }

  return NULL;
}


// Using the configuration tokens, create the columns for the list control
// and read column sorting information.
void HPFEData::processConfigTokens()
{
  bool moreColumns = TRUE;
  int currColumn = 1;
  char currTokenName[defaultStringLength];
  char *currTokenValue = NULL;

  // Look for column entries in the tokens.
  while (moreColumns)
  {
    // Get column type.
    sprintf(currTokenName, "column%d", currColumn);
    currTokenValue = findConfigToken(currTokenName);
    //fprintf(debug, "Looking for token name %s; found value %s.\n", currTokenName, currTokenValue);

    // If type not found, we're done.
    if (currTokenValue == NULL)
      moreColumns = FALSE;
    else
    {
      int columnType = -1;
      if (!strcmp(currTokenValue, "name"))
        columnType = NAME_COL;
      if (!strcmp(currTokenValue, "filename"))
        columnType = FILENAME_COL;
      if (!strcmp(currTokenValue, "system"))
        columnType = SYSTEM_COL;
      if (!strcmp(currTokenValue, "emulator"))
        columnType = EMULATOR_COL;
      if (!strcmp(currTokenValue, "comments"))
        columnType = COMMENTS_COL;
      if (!strcmp(currTokenValue, "maker"))
        columnType = MAKER_COL;
      if (!strcmp(currTokenValue, "year"))
        columnType = YEAR_COL;
      if (!strcmp(currTokenValue, "needtounzip"))
        columnType = NEEDTOUNZIP_COL;
      if (!strcmp(currTokenValue, "zipfile"))
        columnType = ZIPFILE_COL;
      if (!strcmp(currTokenValue, "favorite"))
        columnType = FAVORITE_COL;
      if (!strcmp(currTokenValue, "commandline"))
        columnType = COMMANDLINE_COL;
      if (!strcmp(currTokenValue, "workingdir"))
        columnType = WORKINGDIR_COL ;

      // Get column display name.
      sprintf(currTokenName, "column%d.display", currColumn);
      currTokenValue = findConfigToken(currTokenName);
      //fprintf(debug, "Looking for token name %s; found value %s.\n", currTokenName, currTokenValue);
      char *displayName = strdup((currTokenValue == NULL) ? "" : currTokenValue);

      // Get column position.
      sprintf(currTokenName, "column%d.position", currColumn);
      currTokenValue = findConfigToken(currTokenName);
      //fprintf(debug, "Looking for token name %s; found value %s.\n", currTokenName, currTokenValue);
      int position = atoi(currTokenValue);

      // Get column width.
      sprintf(currTokenName, "column%d.width", currColumn);
      currTokenValue = findConfigToken(currTokenName);
      //fprintf(debug, "Looking for token name %s; found value %s.\n", currTokenName, currTokenValue);
      int width = atoi(currTokenValue);

      // Get column alignment.
      sprintf(currTokenName, "column%d.align", currColumn);
      currTokenValue = findConfigToken(currTokenName);
      //fprintf(debug, "Looking for token name %s; found value %s.\n", currTokenName, currTokenValue);
      int alignment = LVCFMT_LEFT;
      if (!strcmp(currTokenValue, "left")) alignment = LVCFMT_LEFT;
      if (!strcmp(currTokenValue, "center")) alignment = LVCFMT_CENTER;
      if (!strcmp(currTokenValue, "right")) alignment = LVCFMT_RIGHT;

      // Make new system object and add it to the vector.
      Column *newColumn = new Column(columnType, displayName, position, width, alignment, debug);
      columns.push_back(newColumn);
    }
    currColumn++;
  }

  // Create the column display order map; hope that the config file has no errors.
  int numColumns = columns.size();
  columnDisplayOrder = new int[numColumns];
  for (int i = 0; i < numColumns; i++)
  {
    if (((columns[i])->position <= numColumns) &&
        ((columns[i])->position > 0))
      columnDisplayOrder[((columns[i])->position - 1)] = i;
    fprintf(debug, "disp order [%d] = column %d\n", (columns[i])->position - 1, i);
  }

  bool moreSorts = TRUE;
  int currSort = 1;

  // Look for column sort entries.
  while (moreSorts)
  {
    // Get first sort name.
    sprintf(currTokenName, "sort%d", currSort);
    currTokenValue = findConfigToken(currTokenName);
    //fprintf(debug, "Looking for token name %s; found value %s.\n", currTokenName, currTokenValue);

    // If name not found, we're done.
    if (currTokenValue == NULL)
      moreSorts = FALSE;
    else
      sortOrder.push_back(atoi(currTokenValue));
    currSort++;
  }
}


// Given a string of form "tokenName = tokenValue," add it to
// the token vectors.
void HPFEData::addToSystemTokens(int systemNum, char *tokenLine)
{
  if (systemNum == 0) return;
  if (tokenLine == NULL) return;

  char finalTokenName[defaultStringLength];

  // Extract token name and value. 
  char *tokenName = strtok(tokenLine, "=");
  char *tokenValue = strtok(NULL, "\0");
  //fprintf(debug, "!!%s!! equals !!%s!!\n", tokenName, tokenValue);

  // Trim the surrounding whitespace.
  char *cleanTokenName = trimWhitespace(tokenName);
  char *cleanTokenValue = trimWhitespace(tokenValue);
  //fprintf(debug, "* token name:%s\n  token value:%s\n", cleanTokenName, cleanTokenValue);

  // If both tokens are not null, add then to the token vectors.
  if ((cleanTokenName != NULL) && (cleanTokenValue != NULL))
  {
    sprintf(finalTokenName, "system%d.%s", systemNum, cleanTokenName);
    //fprintf(debug, "Found token name:%s, token value:%s.\n", finalTokenName, cleanTokenValue);
    systemTokens.push_back(token(strdup(finalTokenName), strdup(cleanTokenValue)));
  }
}


// Given a token name, find its value in the system tokens.
char *HPFEData::findSystemToken(char *tokenName)
{
  // Iterator to loop through the vector.
  TokenVectorIt currToken;

  for (currToken = systemTokens.begin(); currToken != systemTokens.end(); currToken++)
  {
    if (!strcmp(tokenName, currToken->name))
      return currToken->value;
  }

  return NULL;
}


// Create system objects from the system tokens.
void HPFEData::processSystemTokens()
{
  bool moreSystems = TRUE;
  int currSystem = 1;
  char currTokenName[defaultStringLength];
  char *currTokenValue = NULL;

  // Look for system entries.
  while (moreSystems)
  {
    // Get system name.
    sprintf(currTokenName, "system%d", currSystem);
    currTokenValue = findSystemToken(currTokenName);
    //fprintf(debug, "Looking for token name %s; found value %s.\n", currTokenName, currTokenValue);

    // If name not found, we're done.
    if (currTokenValue == NULL)
      moreSystems = FALSE;
    else
    {
      char *systemName = strdup(currTokenValue);

      // Get system roms list.
      sprintf(currTokenName, "system%d.romslist", currSystem);
      currTokenValue = findSystemToken(currTokenName);
      //fprintf(debug, "Looking for token name %s; found value %s.\n", currTokenName, currTokenValue);
      char *systemFile = strdup((currTokenValue == NULL) ? "" : currTokenValue);

      // Make new system object and add it to the vector.
      SysObj *newSystem = new SysObj(systemName, systemFile, debug);
      sysObjs.push_back(newSystem);

      // Look for roms directories.
      bool moreRomsDirs = true;
      int currRomsDir = 1;
      while (moreRomsDirs)
      {
        sprintf(currTokenName, "system%d.romsdir%d", currSystem, currRomsDir);
        currTokenValue = findSystemToken(currTokenName);
        //fprintf(debug, "Looking for token name %s; found value %s.\n", currTokenName, currTokenValue);
        if (currTokenValue == NULL)
          moreRomsDirs = FALSE;
        else
        {
          char *romsDir = strdup(currTokenValue);
          sprintf(currTokenName, "system%d.romsdir%dhasmultigamezips", currSystem, currRomsDir);
          currTokenValue = findSystemToken(currTokenName);
          bool romsDirHasMultiGameZips = FALSE;
          if (!mystrcmp(currTokenValue, "yes"))
          {
            romsDirHasMultiGameZips = TRUE;
          }
          // Add roms directory to current system object.
          newSystem->addRomsDir(romsDir, romsDirHasMultiGameZips);
        }
        currRomsDir++;
      }

      // Look for file extensions associated with this system.
      bool moreExtensions = true;
      int currExtension = 1;
      while (moreExtensions)
      {
        sprintf(currTokenName, "system%d.extension%d", currSystem, currExtension);
        currTokenValue = findSystemToken(currTokenName);
        //fprintf(debug, "Looking for token name %s; found value %s.\n", currTokenName, currTokenValue);
        if (currTokenValue == NULL)
          moreExtensions = FALSE;
        else
        {
          char *extension = strdup(currTokenValue);
          // Add extension to current system object.
          newSystem->addExtension(extension);
        }
        currExtension++;
      }

      // Look for emulator entries associated with this system.
      bool moreEmulators = true;
      int currEmulator = 1;
      while (moreEmulators)
      {
        sprintf(currTokenName, "system%d.emulator%d", currSystem, currEmulator);
        currTokenValue = findSystemToken(currTokenName);
        //fprintf(debug, "Looking for token name %s; found value %s.\n", currTokenName, currTokenValue);
        if (currTokenValue == NULL)
          moreEmulators = FALSE;
        else
        {
          char *emulatorName = strdup(currTokenValue);

          // Get emulator working dir.
          sprintf(currTokenName, "system%d.emulator%d.workingdir", currSystem, currEmulator);
          currTokenValue = findSystemToken(currTokenName);
          //fprintf(debug, "Looking for token name %s; found value %s.\n", currTokenName, currTokenValue);
          char *emulatorWorkingDir = ((currTokenValue == NULL) ? NULL : strdup(currTokenValue));

          // Get emulator command line.
          sprintf(currTokenName, "system%d.emulator%d.commandline", currSystem, currEmulator);
          currTokenValue = findSystemToken(currTokenName);
          //fprintf(debug, "Looking for token name %s; found value %s.\n", currTokenName, currTokenValue);
          char *emulatorCommandLine = ((currTokenValue == NULL) ? NULL : strdup(currTokenValue));

          // Make new emulator object and give it to the system object.
          Emulator *newEmulator = new Emulator(emulatorName, emulatorWorkingDir, emulatorCommandLine, debug);
          newSystem->addEmulator(newEmulator);
        }
        currEmulator++;
      }

      newSystem->processExtensions();
    }

    currSystem++;
  }
}


// Clean up the mess allocated for the system tokens.
// (!!! doesn't work yet)
void HPFEData::cleanUpTokens()
{
  /*  TokenVectorIt start, end;  
  
    start = systemTokens.begin() ;   // location of first tokens
    end = systemTokens.end() ;       // one past the last token
  
    systemTokens.erase(start, end);*/
}


// Given a game entry and system object, fill related information into
// the game entry.
void HPFEData::fillGameEntry(GameEntry *ge, SysObj *so)
{
  // Attempt to find an emulator entry with the name specified by the game entry.
  Emulator *thisEmulator = so->findEmulator(ge->emulator);

  // If one was not found...
  if (thisEmulator == NULL)
  {
    // See if there is a default emulator entry.
    thisEmulator = so->getDefaultEmulator();

    // If there is a default entry, and no emulator is specified,
    // copy its info into the game entry.
    // Otherwise, do nothing--leave whatever user values are there alone.
    if ((thisEmulator != NULL) && (ge->emulator == NULL))
    {
      ge->emulator = thisEmulator->name;
      ge->commandLine = thisEmulator->commandLine;
      ge->workingDir = thisEmulator->workingDir;
    }
  }
  // If a match was found, copy its info into the non-NULL pieces of the game entry.
  else
  {
    if (ge->commandLine == NULL) ge->commandLine = thisEmulator->commandLine;
    if (ge->workingDir == NULL) ge->workingDir = thisEmulator->workingDir;
  }
}


// Sort the rows of the list control according to the sort order.
// (!!! currently only sorts by game name)
void HPFEData::sortLCRows()
{
  // Pad the first entry, since we start at 1 instead of 0.
  sortedLCRowToUnsortedLCRow.push_back(0);

  // Walk through all game entries row-by-row. For each one,
  // walk through the sorted LC map and see where it fits, then insert it.
  // This is a slow, stupid, insertion sort.
  for (int currRow = 1; currRow < LCRowToSysObj.size(); currRow++)
  {
    GameEntry *insertGE = getGameEntryFromUnsortedListCtrl(currRow);
    IntVectorIt start = sortedLCRowToUnsortedLCRow.begin();
    IntVectorIt end = sortedLCRowToUnsortedLCRow.end();
    IntVectorIt curr;
    for (curr = start; curr != end; curr++)
    {
      GameEntry *thisGE = getGameEntryFromUnsortedListCtrl(*curr);
      if (mystrcmp(insertGE->name, thisGE->name) <= 0) break;
    }
    sortedLCRowToUnsortedLCRow.insert(curr, currRow);
  }
}


