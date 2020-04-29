#include "tasklab/WxPreviewCanvas.h"
#include "tasklab/PreviewPage.h"
#include "tasklab/Node.h"
#include "tasklab/RegistNodes.h"
#include "tasklab/node/DebugDraw.h"

#include <ee0/WxStagePage.h>
#include <ee0/SubjectMgr.h>
#include <ee0/RenderContext.h>
#include <ee2/CamControlOP.h>
#include <blueprint/Node.h>
#include <blueprint/CompNode.h>

#include <unirender/RenderState.h>
#include <painting2/OrthoCamera.h>
#include <painting2/RenderSystem.h>
#include <painting3/MaterialMgr.h>
#include <painting3/Blackboard.h>
#include <painting3/WindowContext.h>
#include <painting3/PerspCam.h>
#include <painting3/Shader.h>
#include <tessellation/Painter.h>
#include <node0/SceneNode.h>
#include <node3/RenderSystem.h>
#include <taskgraph/ParamImpl.h>

namespace
{

const float    NODE_RADIUS = 10;
const uint32_t LIGHT_SELECT_COLOR = 0x88000088;

}

namespace tasklab
{

WxPreviewCanvas::WxPreviewCanvas(const ur::Device& dev, ee0::WxStagePage* stage, ECS_WORLD_PARAM
                                 const ee0::RenderContext& rc)
    : ee3::WxStageCanvas(dev, stage, ECS_WORLD_VAR &rc, nullptr, true)
{
    m_cam3d = m_camera;
    m_cam2d = std::make_shared<pt2::OrthoCamera>();
}

WxPreviewCanvas::~WxPreviewCanvas()
{
    if (m_graph_page)
    {
        auto sub_mgr = m_graph_page->GetSubjectMgr();
        sub_mgr->UnregisterObserver(ee0::MSG_NODE_SELECTION_INSERT, this);
        sub_mgr->UnregisterObserver(ee0::MSG_NODE_SELECTION_CLEAR, this);
    }
}

void WxPreviewCanvas::OnNotify(uint32_t msg, const ee0::VariantSet& variants)
{
    ee3::WxStageCanvas::OnNotify(msg, variants);

    bool dirty = false;
	switch (msg)
	{
	case ee0::MSG_NODE_SELECTION_INSERT:
        dirty = OnSelectionInsert(variants);
		break;
    case ee0::MSG_NODE_SELECTION_CLEAR:
        dirty = OnSelectionClear(variants);
        break;
    case bp::MSG_BP_NODE_PROP_CHANGED:
        dirty = true;
        break;
	}

    if (dirty) {
        SetDirty();
    }
}

void WxPreviewCanvas::SetGraphPage(const bp::WxGraphPage<size_t>* graph_page)
{
    m_graph_page = graph_page;

    auto sub_mgr = graph_page->GetSubjectMgr();
    sub_mgr->RegisterObserver(ee0::MSG_NODE_SELECTION_INSERT, this);
    sub_mgr->RegisterObserver(ee0::MSG_NODE_SELECTION_CLEAR, this);
    sub_mgr->RegisterObserver(bp::MSG_BP_NODE_PROP_CHANGED, this);
}

void WxPreviewCanvas::InitEditOP()
{
    auto cam = m_camera;
    auto& vp = GetViewport();
    auto sub = m_stage->GetSubjectMgr();

    m_ops[OP_DEFAULT] = std::make_shared<ee2::CamControlOP>(m_cam2d, sub);

    m_stage->GetImpl().SetEditOP(m_ops[OP_DEFAULT]);
}

void WxPreviewCanvas::DrawBackground3D() const
{
//    ee3::WxStageCanvas::DrawBackgroundGrids(10.0f, 0.2f);
//    ee3::WxStageCanvas::DrawBackgroundCross();
}

void WxPreviewCanvas::DrawForeground3D() const
{
    pt0::RenderContext rc;
    rc.AddVar(
        pt3::MaterialMgr::PositionUniforms::light_pos.name,
        pt0::RenderVariant(sm::vec3(0, 2, -4))
    );
    if (m_camera->TypeID() == pt0::GetCamTypeID<pt3::PerspCam>())
    {
        auto persp = std::static_pointer_cast<pt3::PerspCam>(m_camera);
        rc.AddVar(
            pt3::MaterialMgr::PositionUniforms::cam_pos.name,
            pt0::RenderVariant(persp->GetPos())
        );
    }

    auto& wc = std::const_pointer_cast<pt3::WindowContext>(GetWidnowContext().wc3);
    assert(wc);
    rc.AddVar(
        pt3::MaterialMgr::PosTransUniforms::view.name,
        pt0::RenderVariant(wc->GetViewMat())
    );
    rc.AddVar(
        pt3::MaterialMgr::PosTransUniforms::projection.name,
        pt0::RenderVariant(wc->GetProjMat())
    );

    tess::Painter pt;

    auto cam_mat = m_camera->GetProjectionMat() * m_camera->GetViewMat();

    DrawSelected(pt, cam_mat, rc);

    ur::RenderState rs;
    pt2::RenderSystem::DrawPainter(m_dev, *GetRenderContext().ur_ctx, rs, pt);
}

void WxPreviewCanvas::DrawForeground2D() const
{
}

bool WxPreviewCanvas::OnSelectionInsert(const ee0::VariantSet& variants)
{
    auto var_obj = variants.GetVariant("obj");
    GD_ASSERT(var_obj.m_type == ee0::VT_PVOID, "no var in vars: obj");
    const ee0::GameObj obj = *static_cast<const ee0::GameObj*>(var_obj.m_val.pv);
    GD_ASSERT(GAME_OBJ_VALID(obj), "err scene obj");

    m_selected = obj;

    SetupRenderer();

    return true;
}

bool WxPreviewCanvas::OnSelectionClear(const ee0::VariantSet& variants)
{
//    m_selected.reset();

    return false;
}

void WxPreviewCanvas::DrawSelected(tess::Painter& pt, const sm::mat4& cam_mat,
                                   const pt0::RenderContext& rc) const
{
    auto node = GetSelectedNode();
    if (!node) {
        return;
    }

    auto type = node->get_type();
    if (type == rttr::type::get<node::DebugDraw>())
    {
        auto& inputs = node->GetAllInput();
        assert(inputs.size() == 1);
        auto& conns = inputs[0]->GetConnecting();
        if (!conns.empty())
        {
            assert(conns.size() == 1);
            auto src_pin = conns[0]->GetFrom();
            auto eval = m_graph_page->GetEval();
            auto back = std::static_pointer_cast<taskgraph::Task>(eval->QueryBackNode(src_pin->GetParent()));
            auto& src_vals = back->GetAllValues();
            const int idx = src_pin->GetPosIdx();
            assert(idx >= 0 && idx < static_cast<int>(src_vals.size()));

            m_img_rd.Draw(m_dev, *GetRenderContext().ur_ctx, src_vals[idx]);

            return;
        }

        return;
    }

    auto task = GetSelectedTask();
    if (task)
    {
        auto& vals = task->GetAllValues();
        for (auto& val : vals)
        {
            auto type = val->Type();
            if (type == taskgraph::PT_IMAGE ||
                type == taskgraph::PT_IMAGE_ARRAY)
            {
                m_img_rd.Draw(m_dev, *GetRenderContext().ur_ctx, val);
                return;
            }
        }
    }
}

void WxPreviewCanvas::SetupRenderer()
{
}

bp::NodePtr WxPreviewCanvas::GetSelectedNode() const
{
    if (!m_selected || !m_selected->HasUniqueComp<bp::CompNode>()) {
        return nullptr;
    }

    auto eval = m_graph_page->GetEval();
    if (!eval) {
        return nullptr;
    }

    auto& cnode = m_selected->GetUniqueComp<bp::CompNode>();
    auto bp_node = cnode.GetNode();
    if (!bp_node) {
        return nullptr;
    }

    return cnode.GetNode();
}

taskgraph::TaskPtr WxPreviewCanvas::GetSelectedTask() const
{
    if (!m_graph_page) {
        return nullptr;
    }

    auto eval = m_graph_page->GetEval();
    if (!eval) {
        return nullptr;
    }

    auto front_node = GetSelectedNode();
    if (front_node) {
        return std::static_pointer_cast<taskgraph::Task>(eval->QueryBackNode(*front_node));
    } else {
        return nullptr;
    }
}

}
