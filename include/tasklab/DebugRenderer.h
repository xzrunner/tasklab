#pragma once

#include <unirender/Texture.h>
#include <taskgraph/typedef.h>
#include <primitive/Bitmap.h>

#include <vector>

namespace tasklab
{

class DebugRenderer
{
public:
    void Draw(const taskgraph::ParamPtr& param) const;

private:
    void UpdateRenderList() const;

    static ur::TexturePtr CreateTexture(const prim::Bitmap<short>& img);

private:
    mutable taskgraph::ParamPtr m_cached = nullptr;

    mutable std::vector<ur::TexturePtr> m_renderable;

}; // DebugRenderer

}