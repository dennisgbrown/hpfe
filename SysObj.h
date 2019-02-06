#ifndef __SYSOBJ_H
#define __SYSOBJ_H

#include <fstream.h>
#include <vector>
#include <afx.h>
#include "token.h"
#include "sharedVars.h"
#include "GameEntry.h"
#include "Emulator.h"


// A System Object contains information about a game system, including its 
// emulators and rom directories.
class SysObj
{
public:
  char *name;                 // display name of system
  char *filename;             // text file of game entries
  charstarVector romsDirs;    // vector of directories to search for roms
  boolVector romsDirHasMultiGameZips; // vector of whether or not corresponding romsDir has multiGame zips
  charstarVector extensions;  // vector of rom file extensions valid for this system
  GameEntryVector games;      // vector of games playable on this system
  EmulatorVector emulators;   // vector of emulators of this system
  Emulator *defaultEmulator;  // default emulator to use if not specified
  FILE *debug;                // debugging output file

public:
  SysObj(char *newName, char *newFilename, FILE *newDebug);
  ~SysObj();
  void addEmulator(Emulator *addMe);
  Emulator *getDefaultEmulator();
  Emulator *findEmulator(char *name);
  void addExtension(char *addMe);
  void addRomsDir(char *addMe, bool multiGameZips);
  void processExtensions();
  void insertGameNameIntoCommandLine(char *fillMe, char *commandLine, char *filename, char *romsDir);

private:
  void readGamesDirs(char *extension);
  void readGamesDir(char *romsDir, bool romsDirHasMultiGameZips, char *extension);
  void processDirLine(char *longName, char *shortName, char *romsDir);
  void processMultiGameZipFile(char *zipFilename, char *romsDir, char *extension);
  void readGamesFile(ifstream gamesFile);
  void processLine(char *inputLine, GameEntry *currGE);
};

// Define a vector of system objects and its iterator.
typedef std::vector<SysObj *> SysObjVector;
typedef SysObjVector::iterator SysObjVectorIt;

#endif
