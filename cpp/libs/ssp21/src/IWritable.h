
#ifndef SSP21_IWRITABLE_H
#define SSP21_IWRITABLE_H

#include "crypto/FormatResult.h"
#include "crypto/gen/Function.h"
#include "ssp21/crypto/IMessagePrinter.h"

namespace ssp21 {
/**
    * something that can be written to an output buffer and printed for logging purposes
    */
struct IWritable {

    virtual FormatResult write(wseq32_t& output) const = 0;

    virtual void print(IMessagePrinter& printer) const = 0;

    virtual Function get_function() const = 0;
};

}

#endif
