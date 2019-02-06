#ifndef __GAMEENTRY_H
#define __GAMEENTRY_H


// A Game Entry contains enough information to run a game with an emulator.
class GameEntry
{
public:
  char *name;          // name of game for display
  char *filename;      // game's filename
  char *romDir;        // directory file is in
  char *needToUnzip;   // is it in a zip file?
  char *multiGameZip;  // is it in a multi-game zip file?
  char *zipFile;       // name of zip file
  char *category;      // category of game
  char *emulator;      // emulator to run it under
  char *commandLine;   // command line for starting game
  char *workingDir;    // directory to run game in
  char *favorite;      // is it a favorite?
  char *comments;      // comments about game
  char *maker;         // game publisher
  char *year;          // year published
  char *system;        // game system game is for
  FILE *debug;         // debugging output file

public:
  // Constructor just fills in information from the parameters.
  GameEntry(char *newName, char *newFilename, char *newRomDir, char* newNeedToUnzip, 
            char *newMultiGameZip, char *newZipFile, 
            char *newCategory, char *newEmulator, char *newCommandLine, char *newFavorite, 
            char *newComments, char *newMaker, char *newYear, char *newWorkingDir, 
            char *newSystem, FILE *newDebug) : 
  name(newName), filename(newFilename), romDir(newRomDir), needToUnzip(newNeedToUnzip), 
  multiGameZip(newMultiGameZip), zipFile(newZipFile),
  category(newCategory), emulator(newEmulator), commandLine(newCommandLine),
  favorite(newFavorite), comments(newComments), maker(newMaker), year(newYear), 
  workingDir(newWorkingDir), system(newSystem), debug(newDebug)
  {
    /*fprintf(debug, "New Game: \n");
  fprintf(debug, "         name=%s\n", name);
  fprintf(debug, "         file=%s\n", filename);
  fprintf(debug, "      romsDir=%s\n", romsDir);
  fprintf(debug, "  needToUnzip=%s\n", needToUnzip);
  fprintf(debug, " multiGameZip=%s\n", multiGameZip);
  fprintf(debug, "      zipFile=%s\n", zipFile);
  fprintf(debug, "     category=%s\n", category);
  fprintf(debug, "     emulator=%s\n", emulator);
  fprintf(debug, "  commandLine=%s\n", commandLine);
  fprintf(debug, "     favorite=%s\n", favorite);
  fprintf(debug, "     comments=%s\n", comments);
  fprintf(debug, "        maker=%s\n", maker);
  fprintf(debug, "         year=%s\n", year);
  fprintf(debug, "   workingDir=%s\n", workingDir);
  fprintf(debug, "\n");*/
  }

  // Constructor just fills in information from the parameters.
  GameEntry(char *newFilename, char *newSystem, FILE *newDebug) : 
  filename(newFilename), system(newSystem), debug(newDebug)
  {
    name = filename;
    romDir = NULL;
    needToUnzip = strdup("no");
    multiGameZip = strdup("no");
    zipFile = NULL;
    category = NULL;
    emulator = NULL;
    commandLine = NULL;
    favorite = NULL;
    comments = NULL;
    maker = NULL;
    year = NULL;
    workingDir = NULL;
    /*fprintf(debug, "New Game: \n");
  fprintf(debug, "         name=%s\n", name);
  fprintf(debug, "         file=%s\n", filename);
  fprintf(debug, "       romDir=%s\n", romDir);
  fprintf(debug, "  needToUnzip=%s\n", needToUnzip);
  fprintf(debug, "      zipFile=%s\n", zipFile);
  fprintf(debug, "     category=%s\n", category);
  fprintf(debug, "     emulator=%s\n", emulator);
  fprintf(debug, "  commandLine=%s\n", commandLine);
  fprintf(debug, "     favorite=%s\n", favorite);
  fprintf(debug, "     comments=%s\n", comments);
  fprintf(debug, "        maker=%s\n", maker);
  fprintf(debug, "         year=%s\n", year);
  fprintf(debug, "   workingDir=%s\n", workingDir);
  fprintf(debug, "\n");*/
  }

  // Destructor cleanly deletes the object. (!!! someday maybe)
  ~GameEntry()
  {
  }

};

// Define a vector of game entries and its iterator.
typedef std::vector<GameEntry *> GameEntryVector;
typedef GameEntryVector::iterator GameEntryVectorIt;

#endif

