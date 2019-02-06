#ifndef __COLUMN_H
#define __COLUMN_H

// Column types
#define NAME_COL 0
#define FILENAME_COL 1
#define SYSTEM_COL 2
#define EMULATOR_COL 3
#define COMMENTS_COL 4
#define MAKER_COL 5
#define YEAR_COL 6
#define NEEDTOUNZIP_COL 7
#define ZIPFILE_COL 8
#define FAVORITE_COL 9
#define COMMANDLINE_COL 10
#define WORKINGDIR_COL 11


// A column contains information about what it contains and how to display it.
class Column
{
public:
  int type;          // type of column
  char *display;     // display string
  int position;      // order in list control
  int width;         // width (pixels)
  int align;         // alignment (left, center, right)
  FILE *debug;       // debugging output file

public:
  // Constructor just fills in information from the parameters.
  Column(int newType, char *newDisplay, int newPosition, int newWidth, int newAlign,
         FILE *newDebug) : 
  type(newType), display(newDisplay), position(newPosition), width(newWidth),
  align(newAlign), debug(newDebug) {}

  // Destructor cleanly deletes the object. (!!! someday maybe)
  ~Column()
  {
  }

};

// Define a vector of columns and its iterator.
typedef std::vector<Column *> ColumnVector;
typedef ColumnVector::iterator ColumnVectorIt;

#endif

