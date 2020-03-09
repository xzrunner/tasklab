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
        case taskgraph::ParamType::Any:
            front.type = bp::PIN_ANY_VAR;
            break;
        case taskgraph::ParamType::File:
            front.type = PIN_FILE;
            break;
        case taskgraph::ParamType::Image:
            front.type = PIN_IMAGE;
            break;
        case taskgraph::ParamType::ImageArray:
            front.type = PIN_IMAGES;
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