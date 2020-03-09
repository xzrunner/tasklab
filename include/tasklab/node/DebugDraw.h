#pragma once

#include "tasklab/Node.h"
#include "tasklab/PinType.h"

#include <blueprint/Pin.h>

namespace tasklab
{
namespace node
{

class DebugDraw : public Node
{
public:
    DebugDraw()
        : Node("DebugDraw")
    {
        m_all_input.push_back(std::make_shared<bp::Pin>(
            true, 0, bp::PIN_ANY_VAR, "in", *this
        ));

        Layout();
    }

private:

    RTTR_ENABLE(Node)

}; // DebugDraw

}
}