#pragma once

namespace tasklab
{

enum PinType
{
#define PIN_INFO(type, color, desc) \
    PIN_##type,
#include "tasklab/pin_cfg.h"
#undef PIN_INFO
};

}