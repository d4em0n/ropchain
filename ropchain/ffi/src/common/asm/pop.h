#pragma once
#include "../regs.h"
#include "../ropchain.h"
#include "../util.h"
namespace Pop {
OptROP find(RegType::Reg op1, const uint64_t dest, const Gadgets &gadgets,
            RegSet &aval);
};
