#pragma once

#include "tasklab/Node.h"

namespace tasklab
{

void nodes_regist_rttr();

namespace node
{

#define EXE_FILEPATH "tasklab/node_def_gen.h"
#include "tasklab/node_regist_cfg.h"
#undef EXE_FILEPATH

}

}