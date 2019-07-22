
#ifndef OUTPUT_EXPANDER_h
#define OUTPUT_EXPANDER_h

#include "SPI_Expander.h"

class OutputExpander : protected SPI_Expander
{
  public:
    OutputExpander( byte enablePin );

    void SetHeater( byte onOff );
    void SetCooler( byte onOff );
    void SetDehumidifier( byte onOff );
    void SetLighting( byte onOff );
    void SetCounter( byte bits );
};

#endif
