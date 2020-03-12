#pragma once

#include "tasklab/Node.h"

#include <taskgraph/task/WriteImage.h>
#include <taskgraph/task/Group.h>

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