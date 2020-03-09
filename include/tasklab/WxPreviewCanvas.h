#pragma once

#include "tasklab/DebugRenderer.h"

#include <ee3/WxStageCanvas.h>
#include <blueprint/WxGraphPage.h>

#include <taskgraph/ParamType.h>
#include <taskgraph/typedef.h>

namespace tasklab
{

class WxEditorPanel;

class WxPreviewCanvas : public ee3::WxStageCanvas
{
public:
    WxPreviewCanvas(ee0::WxStagePage* stage, ECS_WORLD_PARAM
        const ee0::RenderContext& rc);
    virtual ~WxPreviewCanvas();

    virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

    void SetGraphPage(const bp::WxGraphPage<taskgraph::ParamType>* graph_page);

    void InitEditOP();

protected:
    virtual void DrawBackground3D() const override;
    virtual void DrawForeground3D() const override;
    virtual void DrawForeground2D() const override;

private:
    void OnSelectionInsert(const ee0::VariantSet& variants);
    void OnSelectionClear(const ee0::VariantSet& variants);

    void DrawSelected(tess::Painter& pt, const sm::mat4& cam_mat,
        const pt0::RenderContext& rc) const;

    void SetupRenderer();

    bp::NodePtr GetSelectedNode() const;
    taskgraph::TaskPtr GetSelectedOp() const;

private:
    enum OperatorID
    {
        OP_DEFAULT,

        OP_MAX_NUM
    };

private:
    const bp::WxGraphPage<taskgraph::ParamType>* m_graph_page = nullptr;

    n0::SceneNodePtr m_selected = nullptr;

    std::array<ee0::EditOPPtr, OP_MAX_NUM> m_ops;

    DebugRenderer m_debug_rd;

    pt0::CameraPtr m_cam2d = nullptr;
    pt0::CameraPtr m_cam3d = nullptr;

}; // WxPreviewCanvas

}