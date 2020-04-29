#pragma once

#include "tasklab/ImageRenderer.h"

#include <ee3/WxStageCanvas.h>
#include <blueprint/WxGraphPage.h>

#include <taskgraph/typedef.h>

namespace tasklab
{

class WxEditorPanel;

class WxPreviewCanvas : public ee3::WxStageCanvas
{
public:
    WxPreviewCanvas(const ur::Device& dev, ee0::WxStagePage* stage, ECS_WORLD_PARAM
        const ee0::RenderContext& rc);
    virtual ~WxPreviewCanvas();

    virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

    void SetGraphPage(const bp::WxGraphPage<size_t>* graph_page);

    void InitEditOP();

protected:
    virtual void DrawBackground3D() const override;
    virtual void DrawForeground3D() const override;
    virtual void DrawForeground2D() const override;

private:
    bool OnSelectionInsert(const ee0::VariantSet& variants);
    bool OnSelectionClear(const ee0::VariantSet& variants);

    void DrawSelected(tess::Painter& pt, const sm::mat4& cam_mat,
        const pt0::RenderContext& rc) const;

    void SetupRenderer();

    bp::NodePtr GetSelectedNode() const;
    taskgraph::TaskPtr GetSelectedTask() const;

private:
    enum OperatorID
    {
        OP_DEFAULT,

        OP_MAX_NUM
    };

private:
    const bp::WxGraphPage<size_t>* m_graph_page = nullptr;

    n0::SceneNodePtr m_selected = nullptr;

    std::array<ee0::EditOPPtr, OP_MAX_NUM> m_ops;

    ImageRenderer m_img_rd;

    pt0::CameraPtr m_cam2d = nullptr;
    pt0::CameraPtr m_cam3d = nullptr;

}; // WxPreviewCanvas

}