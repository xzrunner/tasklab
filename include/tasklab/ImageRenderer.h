#pragma once

#include <taskgraph/typedef.h>
#include <primitive/Bitmap.h>
#include <unirender2/typedef.h>

#include <vector>

namespace ur2 { class Device; class Context; }

namespace tasklab
{

class ImageRenderer
{
public:
    void Draw(const ur2::Device& dev, ur2::Context& ctx,
        const taskgraph::ParamPtr& param) const;

private:
    void UpdateRenderList(const ur2::Device& dev) const;

    static ur2::TexturePtr CreateTexture(const ur2::Device& dev,
        const prim::Bitmap<short>& img);

private:
    mutable taskgraph::ParamPtr m_cached = nullptr;

    mutable std::vector<ur2::TexturePtr> m_renderable;

}; // ImageRenderer

}