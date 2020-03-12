#include "tasklab/ImageRenderer.h"

#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>
#include <painting2/RenderSystem.h>
#include <taskgraph/ParamImpl.h>

namespace
{

const size_t MAX_DRAW_IMAGE_NUM  = 10;
const size_t MAX_DRAW_IMAGE_SIZE = 2048;

}

namespace tasklab
{

void ImageRenderer::Draw(const taskgraph::ParamPtr& param) const
{
    if (m_cached != param) {
        m_cached = param;
        UpdateRenderList();
    }

    auto& rc = ur::Blackboard::Instance()->GetRenderContext();
    rc.SetZTest(ur::DEPTH_DISABLE);
    rc.SetCullMode(ur::CULL_DISABLE);

    sm::Matrix2D mt;
    for (auto& r : m_renderable)
    {
        if (!r) {
            continue;
        }

        sm::rect pos;
        pos.xmin = pos.ymin = 0;
        pos.xmax = static_cast<float>(r->Width());
        pos.ymax = static_cast<float>(r->Height());
        pt2::RenderSystem::DrawTexture(*r, pos, mt, false);

        const float dx = static_cast<float>(r->Width() + 10);
        mt.Translate(dx, 0);
    }

    rc.SetZTest(ur::DEPTH_LESS_EQUAL);
    rc.SetCullMode(ur::CULL_BACK);
}

void ImageRenderer::UpdateRenderList() const
{
    m_renderable.clear();

    if (!m_cached) {
        return;
    }

    switch (m_cached->Type())
    {
    case taskgraph::PT_IMAGE:
    {
        auto img_p = std::static_pointer_cast<taskgraph::ImageParam>(m_cached);
        auto& img = img_p->GetImage()->bmp;
        if (img.Width() < MAX_DRAW_IMAGE_SIZE && 
            img.Height() < MAX_DRAW_IMAGE_SIZE) {
            m_renderable.push_back(CreateTexture(img));
        }
    }
        break;
    case taskgraph::PT_IMAGE_ARRAY:
    {
        auto& imgs = std::static_pointer_cast<taskgraph::ImageArrayParam>(m_cached)->GetAllImages();
        auto num = std::min(imgs.size(), MAX_DRAW_IMAGE_NUM);
        for (size_t i = 0; i < num; ++i) 
        {
            auto& img = imgs[i]->bmp;
            if (img.Width() < MAX_DRAW_IMAGE_SIZE &&
                img.Height() < MAX_DRAW_IMAGE_SIZE) {
                m_renderable.push_back(CreateTexture(img));
            }
        }
    }
        break;
    }
}

ur::TexturePtr ImageRenderer::CreateTexture(const prim::Bitmap<short>& img)
{
    ur::TexturePtr tex = nullptr;

    const auto w = img.Width();
    const auto h = img.Height();
    const auto c = img.Channels();

    auto vals = img.GetPixels();

    auto& rc = ur::Blackboard::Instance()->GetRenderContext();
    switch (c)
    {
    case 1:
    {
        //tex = std::make_shared<ur::Texture>();
        //tex->Upload(&rc, w, h, ur::TEXTURE_R16, vals.data());

        std::vector<char> pixels(w * h * 3);
        auto smin = std::numeric_limits<short>::min();
        auto smax = std::numeric_limits<short>::max();
        auto dmin = std::numeric_limits<char>::min();
        auto dmax = std::numeric_limits<char>::max();

        int max = 0;
        for (size_t i = 0, n = w * h * c; i < n; ++i)
        {
            short h = std::min(smax, std::max(static_cast<short>(vals[i] * 10), smin));
            char c = static_cast<char>(static_cast<float>(h - smin) / (smax - smin) * (dmax - dmin) + dmin);
            for (size_t j = 0; j < 3; ++j) {
                pixels[i * 3 + j] = c;
            }
        }
        tex = std::make_shared<ur::Texture>();
        tex->Upload(&rc, w, h, ur::TEXTURE_RGB, pixels.data());
    }
        break;
    case 3:
    case 4:
    {
        tex = std::make_shared<ur::Texture>();
        std::vector<char> pixels(w * h * c);
        for (size_t i = 0, n = w * h * c; i < n; ++i) {
            pixels[i] = static_cast<char>(vals[i]);
        }
        auto fmt = c == 3 ? ur::TEXTURE_RGB : ur::TEXTURE_RGBA8;
        tex->Upload(&rc, w, h, fmt, pixels.data());
    }
        break;
    }

    return tex;
}

}