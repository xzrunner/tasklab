#include "tasklab/RegistNodes.h"

#include <ee0/ReflectPropTypes.h>

#include <js/RTTR.h>

RTTR_REGISTRATION
{

// base

rttr::registration::class_<tasklab::Node>("tasklab::node")
.property("name", &tasklab::Node::GetName, &tasklab::Node::SetName)
(
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Name"))
)
;

#define EXE_FILEPATH "tasklab/node_rttr_gen.h"
#define SKIP_FILE_NODE
#include "tasklab/node_regist_cfg.h"
#undef SKIP_FILE_NODE
#undef EXE_FILEPATH

}

namespace tasklab
{

void nodes_regist_rttr()
{
}

}