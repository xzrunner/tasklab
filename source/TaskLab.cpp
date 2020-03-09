#include "tasklab/TaskLab.h"
#include "tasklab/PinCallback.h"
#include "tasklab/Node.h"
#include "tasklab/RegistNodes.h"

#include <blueprint/NodeBuilder.h>
#include <blueprint/node/Commentary.h>

#include <taskgraph/TaskGraph.h>

namespace bp
{
extern void regist_sm_rttr();
extern void regist_pt0_rttr();
}

namespace tasklab
{

CU_SINGLETON_DEFINITION(TaskLab);

TaskLab::TaskLab()
{
    taskgraph::TaskGraph::Instance();

    RegistRTTR();

	InitNodes();

    InitPinCallback();
}

void TaskLab::RegistRTTR()
{
    bp::regist_sm_rttr();
    bp::regist_pt0_rttr();

    //	prop_types_regist_rttr();
    nodes_regist_rttr();
}

void TaskLab::InitNodes()
{
    const int bp_count = 1;

	auto list = rttr::type::get<Node>().get_derived_classes();
	m_nodes.reserve(bp_count + list.size());

    m_nodes.push_back(std::make_shared<bp::node::Commentary>());

	for (auto& t : list)
	{
		auto obj = t.create();
		assert(obj.is_valid());
		auto node = obj.get_value<bp::NodePtr>();
		assert(node);
		m_nodes.push_back(node);
	}
}

}