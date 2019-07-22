
#ifndef DISPLAY_CONTRAST_h
#define DISPLAY_CONTRAST_h

#include "OutputExpander.h"

class DigitalPot : protected OutputExpander
{
  public:
    DigitalPot( byte enablePin );

    void Set( byte bits );
    void Flush();
};

#endif
