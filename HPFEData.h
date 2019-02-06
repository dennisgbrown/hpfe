#ifndef __HPFEDATA_H
#define __HPFEDATA_H

#include <fstream.h>
#include <vector>
#include "token.h"
#include "SysObj.h"
#include "Column.h"

// Define an int vector and its iterator
typedef std::vector<int> IntVector;
typedef IntVector::iterator IntVectorIt;


// HPFEData holds info about game systems, display columns, 
// tokens read from the config file, and a map from list control 
// rows to system objects and game entries.
class HPFEData
{
public:
  SysObjVector sysObjs;                  // the system objects
  TokenVector systemTokens;              // tokens associated with all systems (from hpfesys.dat)

  ColumnVector columns;                  // vector of columns for list control
  TokenVector configTokens;              // tokens associated with columns (from hpfeconf.dat)
  int *columnDisplayOrder;               // array that maps column order to column number
  IntVector sortOrder;                   // numbers of columns in the order the data should be sorted

  FILE *debug;                           // file for debugging output

  IntVector LCRowToSysObj;               // maps a list control row to a system object
  IntVector LCRowToGameEntry;            // maps a list control row to a game entry
  IntVector sortedLCRowToUnsortedLCRow;  // the above maps are created unsorted; when the 
                                         // list control is sorted, this vector maps 
                                         // the sorted position to its original unsorted position
                                         // for use in the above maps


public:
  HPFEData(FILE *debugFile);
  ~HPFEData();
  void readConfigFile(ifstream configFile);
  void readSystemsFile(ifstream systemsFile);
  void fillListCtrl(CListCtrl* ROMLIST);
  void buildInitialData();
  GameEntry *getGameEntryFromUnsortedListCtrl(int LCRow);
  SysObj *getSysObjFromUnsortedListCtrl(int LCRow);
  GameEntry *getGameEntryFromSortedListCtrl(int LCRow);
  SysObj *getSysObjFromSortedListCtrl(int LCRow);

private:
  void addToConfigTokens(char *tokenLine);
  char *findConfigToken(char *tokenName);
  void processConfigTokens();
  void addToSystemTokens(int systemNum, char *tokenLine);
  char *findSystemToken(char *tokenName);
  void processSystemTokens();
  void cleanUpTokens();
  void fillGameEntry(GameEntry *ge, SysObj *so);
  void sortLCRows();
};

#endif
