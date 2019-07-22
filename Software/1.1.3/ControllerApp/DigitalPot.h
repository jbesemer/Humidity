
#ifndef DIGITAL_POT_h
#define DIGITAL_POT_h

#include "OutputExpander.h"

class DigitalPot : protected OutputExpander
{
  public:
    DigitalPot( byte enablePin );

    void Set( byte bits );
    void Flush();
};

#endif
