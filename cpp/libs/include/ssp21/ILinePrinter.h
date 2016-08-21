
#ifndef SSP21_ILINEPRINTER_H
#define SSP21_ILINEPRINTER_H

namespace ssp21 {

/**
* A simple interface for pretty prininting
* complex messages as a sequence of lines
*/
class ILinePrinter
{

public:

  virtual void print(const char* line);

};

}

#endif
