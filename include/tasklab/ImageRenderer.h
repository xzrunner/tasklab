#pragma once

#include <taskgraph/typedef.h>
#include <primitive/Bitmap.h>
#include <unirender/typedef.h>

#include <vector>

namespace ur { class Device; class Context; }

namespace tasklab
{

class ImageRenderer
{
public:
    void Draw(const ur::Device& dev, ur::Context& ctx,
        const taskgraph::ParamPtr& param) const;

private:
    void UpdateRenderList(const ur::Device& dev) const;

    static ur::TexturePtr CreateTexture(const ur::Device& dev,
        const prim::Bitmap<short>& img);

private:
    mutable taskgraph::ParamPtr m_cached = nullptr;

    mutable std::vector<ur::TexturePtr> m_renderable;

}; // ImageRenderer

}