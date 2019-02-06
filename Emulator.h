#ifndef __EMULATOR_H
#define __EMULATOR_H


// An emulator contains information about running it.
class Emulator
{
public:
  char *name;           // display name of emulator
  char *workingDir;     // working directory
  char *commandLine;    // command line (with % variables)
  FILE *debug;          // debugging file

public:
  // Constructor just fills in information from the parameters.
  Emulator(char *newName, char *newWorkingDir, char *newCommandLine, FILE *newDebug) : 
  name(newName), workingDir(newWorkingDir), commandLine(newCommandLine), debug(newDebug) 
  {
    fprintf(debug, "New Emulator: \n");
    fprintf(debug, "        name=%s\n", name);
    fprintf(debug, " working dir=%s\n", workingDir);
    fprintf(debug, "command line=%s\n", commandLine);
    fprintf(debug, "\n");
  }

  // Destructor cleanly deletes the object. (!!! someday maybe)
  ~Emulator()
  {
  }

};

// Define a vector of emulators and its iterator.
typedef std::vector<Emulator *> EmulatorVector;
typedef EmulatorVector::iterator EmulatorVectorIt;

#endif
