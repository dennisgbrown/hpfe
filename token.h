#ifndef __TOKEN_H
#define __TOKEN_H

// A token object couples a name with a value.
class token
{
public:    
  char *name;
  char *value;

  // Constructor just fills in information from the parameters.
  token(char *newName, char *newValue) : name(newName), value(newValue) {}

  // Destructor cleanly deletes the object. (!!! someday maybe)
  ~token()
  {
    //delete name;
    //delete value;
  }
};

// Define a vector of columns and its iterator.
typedef std::vector<token> TokenVector;
typedef TokenVector::iterator TokenVectorIt;

#endif
