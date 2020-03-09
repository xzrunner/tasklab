#include "tasklab/Node.h"
#include "tasklab/PinType.h"

#include <blueprint/Pin.h>
#include <blueprint/BackendAdapter.h>

#include <taskgraph/ParamType.h>

namespace tasklab
{

Node::Node(const std::string& title)
    : bp::Node(title)
{
}

void Node::InitPins(const std::string& name)
{
    auto back2front = [](const dag::Node<taskgraph::ParamType>::Port& back) -> bp::PinDesc
    {
        bp::PinDesc front;

        switch (back.var.type)
        {
        case taskgraph::ParamType::Path:
            front.type = PIN_PATH;
            break;
        default:
            assert(0);
        }

        front.name = back.var.full_name;

        return front;
    };

    bp::BackendAdapter<taskgraph::ParamType>
        trans("taskgraph", back2front);
    trans.InitNodePins(*this, name);
}

}