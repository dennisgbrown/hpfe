#include "stdafx.h"
#include "SysObj.h"

#define ZLIB_DLL
#ifndef _WINDOWS
  #define _WINDOWS
#endif
#include "unzip.h"


///////////////////////////////////
//                               //
// Constructors and Destructors  //
//                               //
///////////////////////////////////

// The constructor assigns values from the parameters then tries to read
// the games file.
SysObj::SysObj(char *newName, char *newFilename, FILE *newDebug) : 
name(newName), filename(newFilename), debug(newDebug) 
{
  fprintf(debug, "New System: \n");
  fprintf(debug, "      name=%s\n", name);
  fprintf(debug, "games file=%s\n", filename);
  fprintf(debug, "\n");

  // Try to read the games file (optional).
  ifstream gamesFile(filename, ios::in);
  if (!gamesFile)
  {
    fprintf(debug, "Couldn't open games file: %s\n", filename);
  }
  else
  {
    readGamesFile(gamesFile);
    gamesFile.close();
  }

  defaultEmulator = NULL;
}


// Destructor cleanly deletes the object. (!!! someday maybe)
SysObj::~SysObj()
{

}


//////////////////////
//                  //
// Helper functions //
//                  //
//////////////////////

// Given a filename with extension (without full path), return
// the base name. This assumes anything before the last separator is 
// part of the base name (ie, hello.there.today returns hello.there).
char *findBaseFilename(char *filename)
{
  if (filename == NULL) return NULL;

  int endOfBaseName = -1;
  int len = strlen(filename);

  // Find out where the base name ends, searching
  // backward from the end of the name for the last separator.
  for (int i = (len - 1); i >= 0; i--)
  {
    if (filename[i] == '.')
    {
      endOfBaseName = i;
      break;
    }
  }

  // If there is no separator, return the name as-is.
  if (endOfBaseName == -1) return strdup(filename);

  // Temporarily truncate filename at the separator.
  filename[endOfBaseName] = '\0';
  // Duplicate truncated name.
  char *returnMe = strdup(filename);
  // Fix full name.
  filename[endOfBaseName] = '.';
  // Return duplicate.
  return returnMe;
}


// Given a filename, return its extension (all characters
// after the last separator).
char *findExtension(char *filename)
{
  if (filename == NULL) return NULL;

  int endOfBaseName = -1;
  int len = strlen(filename);

  // Find location of last separator.
  for (int i = (len - 1); i >= 0; i--)
  {
    if (filename[i] == '.')
    {
      endOfBaseName = i;
      break;
    }
  }

  // If separator isn't in name or is last char, return null.
  if ((endOfBaseName == -1) || (endOfBaseName == (len - 1)))
    return NULL;

  // Duplicate truncated name.
  char *returnMe = strdup(filename + (sizeof(char) * (1 + endOfBaseName)));

  // Return duplicate.
  return returnMe;
}


// Test if this character is a whitespace character.
// (name starts with "so" to not conflict with similar method
//  in HPFEData.cpp)
bool soIsWhitespace(char testMe)
{
  return ((testMe == ' ') || (testMe == '\t') || (testMe == '\n'));
}


// Return the string without leading or trailing whitespace.
// (name starts with "so" to not conflict with similar method
//  in HPFEData.cpp)
char *soTrimWhitespace(char *cleanMe)
{
  int start = -1;
  int end = -1;
  int i;

  if (cleanMe == NULL) return NULL;

  for (i = 0; i < strlen(cleanMe); i++)
  {
    if (!soIsWhitespace(cleanMe[i]))
    {
      start = i;
      break;
    }
  }

  if (start == -1) return NULL;

  for (i = (strlen(cleanMe) - 1); i > start; i--)
  {
    if (!soIsWhitespace(cleanMe[i]))
    {
      end = i;
      break;
    }
  }

  cleanMe[end + 1] = '\0';
  return (cleanMe + (sizeof(char) * start));
}


// My string compare is case-INsensitive, otherwise like strcmp.
// (name starts with "so" to not conflict with similar method
//  in HPFEData.cpp)
int somystrcmp(char *s1, char *s2)
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


//////////////////////////
//                      //
// Public class methods //
//                      //
//////////////////////////

// Add an emulator to the list of emulators.
void SysObj::addEmulator(Emulator *addMe)
{
  emulators.push_back(addMe);

  // If this is the first emulator, make it the default emulator.
  if (emulators.size() == 1)
    defaultEmulator = emulators[0];
}


// Get the default emulator.
Emulator *SysObj::getDefaultEmulator()
{
  return defaultEmulator;
}


// Given a string name of an emulator, return its emulator object.
Emulator *SysObj::findEmulator(char *name)
{
  EmulatorVectorIt it;

  if (name == NULL) return NULL;

  for (it = emulators.begin(); it != emulators.end(); it++)
    if (!strcmp(name, (*it)->name)) return (*it);

  return NULL;
}


// Add a file extension to the vector of extensions.
void SysObj::addExtension(char *addMe)
{
  fprintf(debug, "adding extension %s\n", addMe);
  extensions.push_back(addMe);
}


// Add a roms directory to the vector of roms directories.
void SysObj::addRomsDir(char *addMe, bool multiGameZips)
{
  fprintf(debug, "adding romsDir %s\n", addMe);
  romsDirs.push_back(addMe);
  romsDirHasMultiGameZips.push_back(multiGameZips);
}


// For each extension, search each roms directory looking for files
// with that extension and create game entries.
void SysObj::processExtensions()
{
  charstarVectorIt it; 
  for (it = extensions.begin(); it != extensions.end(); it++)
  {
    readGamesDirs(*it);
  } 

  readGamesDirs("zip");
}


// Given a string space, a command line, a file name, and a roms dir, 
// create an executable command line.
void SysObj::insertGameNameIntoCommandLine(char *fillMe, char *commandLine, char *filename, char *romsDir)
{
  if (commandLine == NULL) return;
  if (filename == NULL) return;

  // Get the base and extension from the filename.
  char *base = findBaseFilename(filename);
  char *ext = findExtension(filename);

  //fprintf(debug, "base: %s, ext: %s\n", base, ext);

  // Copy the input command line into the executable command line,
  // making substitutions as necessary.
  int fillMePos = 0;
  int len = strlen(commandLine);
  for (int i = 0; i < len; i++)
  {
    // If the current char doesn't start a variable, just copy it over.
    if (commandLine[i] != '%')
    {
      fillMe[fillMePos] = commandLine[i];
      fillMePos++;
    }

    // Otherwise, there might be a variable.
    else
    {
      // See if there really is a variable.
      // If so, copy its value into the executable command line.
      if (((i + 2) < len) && (commandLine[i + 2] == '%')) // found a token
      {
        char tokenType = commandLine[i + 1];
        if (tokenType == 'd')
        {
          strcpy(fillMe + (sizeof(char) * fillMePos), romsDir);
          fillMePos += strlen(romsDir);
          i += 2;
        }
        else if (tokenType == 'b')
        {
          strcpy(fillMe + (sizeof(char) * fillMePos), base);
          fillMePos += strlen(base);
          i += 2;
        }
        else if (tokenType == 'e')
        {
          strcpy(fillMe + (sizeof(char) * fillMePos), ext);
          fillMePos += strlen(ext);
          i += 2;
        }

      }
      else
      {
        fillMe[fillMePos] = commandLine[i];
        fillMePos++;
      }
    }
  }

  //delete base;
  //delete ext;

  // Terminate the executable command line string.
  fillMe[fillMePos] = '\0';
}


///////////////////////////
//                       //
// Private class methods //
//                       //
///////////////////////////

// For a given extension, search each roms directory looking for files
// with that extension and create game entries.
void SysObj::readGamesDirs(char *extension)
{
  charstarVectorIt it;
  boolVectorIt it2;
  for (it = romsDirs.begin(), it2 = romsDirHasMultiGameZips.begin(); 
      it != romsDirs.end(); 
      it++, it2++)
  {
    readGamesDir((*it), (*it2), extension);
  }
}


// For a given extension and roms directory, search for files with
// that extension in that directory and create game entries.
void SysObj::readGamesDir(char *romsDir, bool romsDirHasMultiGameZips,
                          char *extension)
{
  fprintf(debug, "readGamesDir %s, hasMultiGameZips %s, extension %s\n", 
          romsDir, (romsDirHasMultiGameZips ? "yes" : "no"), extension);

  // If this is the "zip" extension, and ZIP files in this
  // directory are multi-game zips, quit--no need to process this 
  // extension in this directory.
  if ((!somystrcmp(extension, "zip")) && romsDirHasMultiGameZips) return;

  // Create a filemask for the directory and extension.
  static char filemask[defaultStringLength];
  sprintf(filemask, "%s\\*.%s", romsDir, extension);

  // Use Windows API to find files for that filemask.
  WIN32_FIND_DATA *findFileData = new WIN32_FIND_DATA;
  HANDLE findFileHandle;
  findFileHandle = FindFirstFile(filemask, findFileData);
  BOOL moreFiles = (findFileHandle != INVALID_HANDLE_VALUE);

  // While there are more files that meet the search criteria, process
  // each filename.
  while (moreFiles)
  {
    //fprintf(debug, "found file %s (%s)\n", findFileData->cFileName, findFileData->cAlternateFileName);
    if (findFileData->cAlternateFileName == NULL)
      processDirLine(findFileData->cFileName, findFileData->cFileName, romsDir);
    else
      processDirLine(findFileData->cFileName, findFileData->cAlternateFileName, romsDir);
    moreFiles = FindNextFile(findFileHandle, findFileData);
  }

  FindClose(findFileHandle);

  // If this is the "zip" extension, quit (don't look for
  // file of extension "zip" in zip files--see code below).
  if (!somystrcmp(extension, "zip")) return;

  // Look for files of that extension in ZIP files if the
  // ZIP files in this directory contain more than one game each.
  if (romsDirHasMultiGameZips)
  {
    sprintf(filemask, "%s\\*.zip", romsDir);

    // Use Windows API to find files for that filemask.
    findFileHandle = FindFirstFile(filemask, findFileData);
    moreFiles = (findFileHandle != INVALID_HANDLE_VALUE);

    // While there are more files that meet the search criteria, process
    // each filename.
    while (moreFiles)
    {
      //fprintf(debug, "found zip file %s (%s)\n", findFileData->cFileName, findFileData->cAlternateFileName);
      processMultiGameZipFile(findFileData->cFileName, romsDir, extension);
      moreFiles = FindNextFile(findFileHandle, findFileData);
    }

    FindClose(findFileHandle);
  }
}


// Given information from a roms directory line, create a new game entry
// and store it in the game entries vector.
void SysObj::processDirLine(char *longName, char *shortName, char *romsDir)
{
  GameEntry *thisGE;

  thisGE = new GameEntry(strdup(shortName), name, debug);
  thisGE->name = findBaseFilename(longName);
  thisGE->romDir = strdup(romsDir);

  char* ext = findExtension(longName);
  if (!somystrcmp(ext, "zip"))
  {
    thisGE->filename = strdup("(to be determined)");
    thisGE->needToUnzip = strdup("yes");
    thisGE->zipFile = strdup(longName);
  }

  games.push_back(thisGE);
}


// Search through a ZIP file for files of this extension. Make new game entries 
// for files found.
void SysObj::processMultiGameZipFile(char *zipFilename, char *romsDir, char *extension)
{
  static unz_file_info *fileInfo = new unz_file_info;
  static char filenameInZip[defaultStringLength];
  static char extra[defaultStringLength];
  static char comment[defaultStringLength];
  static char zipPathAndFilename[defaultStringLength];
  sprintf(zipPathAndFilename, "%s\\%s", romsDir, zipFilename);
  unzFile zipfile = unzOpen(zipPathAndFilename);

  if (zipfile == NULL)
  {
    fprintf(debug, "CAN'T OPEN %s\n", zipPathAndFilename);
  }
  else
  {
    //fprintf(debug, "OPENED %s to look for files of extension %s\n", 
    //        zipPathAndFilename, extension);
    
  }

  int zipresult = unzGoToFirstFile(zipfile);
  while (zipresult != UNZ_END_OF_LIST_OF_FILE)
  {
    unzGetCurrentFileInfo(zipfile,
                          fileInfo,
                          filenameInZip, defaultStringLength,
                          extra, defaultStringLength,
                          comment, defaultStringLength);
    //fprintf(debug, "current zip filename is %s; size is %d\n", filenameInZip, fileInfo->uncompressed_size);
    fflush(debug);

    if (strlen(filenameInZip) > 4)
    {
      char *currExtension = findExtension(filenameInZip);
      //fprintf(debug, "current extension is %s\n", currExtension);

      // If the extension matches, add the game.
      if (!somystrcmp(currExtension, extension))
      {
        GameEntry *thisGE;

        thisGE = new GameEntry(strdup(filenameInZip), name, debug);
        thisGE->name = findBaseFilename(filenameInZip);
        thisGE->romDir = strdup(romsDir);
        thisGE->needToUnzip = strdup("yes");
        thisGE->multiGameZip = strdup("yes");
        thisGE->zipFile = strdup(zipFilename);

        games.push_back(thisGE);
      }
    }

    zipresult = unzGoToNextFile(zipfile);
  }

  unzClose(zipfile); 
}


// Read in the file of game systems information.
void SysObj::readGamesFile(ifstream gamesFile)
{
  char inLine[defaultStringLength];
  char *gameFilename = NULL;
  GameEntry *currGE = NULL;

  // While we haven't hit the end of the file...
  while (!gamesFile.eof())
  {
    // Get the next line.
    gamesFile.getline(inLine, defaultStringLength);
    //fprintf(debug, "%s\n", inLine);

    if (strlen(inLine) < 1) continue;  // empty line

    if (inLine[0] == '#') continue; // comment line

    // The [ character signifies the start of a new game entry in the text file.
    if (inLine[0] == '[')
    {
      // If we have an existing game entry, it's done now because we've 
      // hit the start of another. Save the old one in the game vector.
      if (currGE != NULL) games.push_back(currGE);

      if (strlen(inLine) < 2) continue; // garbage line

      // Get the game file name.
      gameFilename = strtok(inLine + sizeof(char), "]");
      if (gameFilename == NULL) continue;

      // Create a new game entry with the game filename, 
      // the system's name, and the debugging file pointer.
      currGE = new GameEntry(strdup(gameFilename), name, debug);
    }

    // It's not the start of a new entry, and not garbage, so
    // process the line in the context of the current game entry.
    else
      processLine(inLine, currGE);
  }

  // Save last game entry...
  if (currGE != NULL) games.push_back(currGE);
}


// Process a line of a game entry text file. 
void SysObj::processLine(char *inputLine, GameEntry *currGE)
{
  if (inputLine == NULL) return;
  if (currGE == NULL) return;

  // Extract token name and value. 
  char *tokenName = strtok(inputLine, "=");
  char *tokenValue = strtok(NULL, "\0");

  if (tokenName == NULL) return;
  if (tokenValue == NULL) return;

  // Trim the surrounding whitespace.
  char *cleanTokenName = soTrimWhitespace(tokenName);
  char *cleanTokenValue = soTrimWhitespace(tokenValue);

  // Set fields of the game entry based on the token's name.
  if (!strcmp(cleanTokenName, "name")) currGE->name = strdup(cleanTokenValue);
  else if (!strcmp(cleanTokenName, "dir")) currGE->romDir = strdup(cleanTokenValue);
  else if (!strcmp(cleanTokenName, "maker")) currGE->maker = strdup(cleanTokenValue);
  else if (!strcmp(cleanTokenName, "year")) currGE->year = strdup(cleanTokenValue);
  else if (!strcmp(cleanTokenName, "comments")) currGE->comments = strdup(cleanTokenValue);
  else if (!strcmp(cleanTokenName, "zipfile")) currGE->zipFile = strdup(cleanTokenValue);
  else if (!strcmp(cleanTokenName, "category")) currGE->category = strdup(cleanTokenValue);
  else if (!strcmp(cleanTokenName, "emulator")) currGE->emulator = strdup(cleanTokenValue);
  else if (!strcmp(cleanTokenName, "commandline")) currGE->commandLine = strdup(cleanTokenValue);
  else if (!strcmp(cleanTokenName, "workingdir")) currGE->workingDir = strdup(cleanTokenValue);
  else if (!strcmp(cleanTokenName, "needtounzip")) currGE->needToUnzip = strdup(cleanTokenValue);
  else if (!strcmp(cleanTokenName, "favorite")) currGE->favorite = strdup(cleanTokenValue);
}

