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
    auto back2front = [](const dag::Node<size_t>::Port& back) -> bp::PinDesc
    {
        bp::PinDesc front;

        if (back.var.type == 0) {
            front.type = bp::PIN_ANY_VAR;
        } else if ((back.var.type & taskgraph::PT_FILE) || 
                   (back.var.type & taskgraph::PT_BUFFER)) {
            front.type = PIN_RES;
        } else if ((back.var.type & taskgraph::PT_IMAGE) ||
                   (back.var.type & taskgraph::PT_IMAGE_ARRAY)) {
            front.type = PIN_IMAGE;
        } else {
            assert(0);
        }

        front.name = back.var.full_name;

        return front;
    };

    bp::BackendAdapter<size_t> trans("taskgraph", back2front);
    trans.InitNodePins(*this, name);
}

}