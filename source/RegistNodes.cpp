#include "tasklab/RegistNodes.h"
#include "tasklab/node/DebugDraw.h"

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

rttr::registration::class_<tasklab::node::DebugDraw>("tasklab::debug_draw")
    .constructor<>()
;

rttr::registration::class_<tasklab::node::ReadImage>("tasklab::read_img")
    .constructor<>()
    .property("filepath", &tasklab::node::ReadImage::m_filepath)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Filepath")),
        rttr::metadata(js::RTTR::FilePathTag(), true),
        rttr::metadata(ee0::PropOpenFileTag(), ee0::PropOpenFile("*.*"))
    )
;

rttr::registration::class_<tasklab::node::WriteImage>("tasklab::write_img")
    .constructor<>()
    .property("filepath", &tasklab::node::WriteImage::m_filepath)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Filepath")),
        rttr::metadata(js::RTTR::FilePathTag(), true),
        rttr::metadata(ee0::PropOpenFileTag(), ee0::PropOpenFile("*.*"))
    )
    .property("type", &tasklab::node::WriteImage::m_type)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Type"))
    )
;

}

namespace tasklab
{

void nodes_regist_rttr()
{
}

}