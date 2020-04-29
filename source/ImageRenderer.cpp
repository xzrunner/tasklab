#include "tasklab/ImageRenderer.h"

#include <unirender/RenderState.h>
#include <unirender/Texture.h>
#include <unirender/Device.h>
#include <unirender/TextureDescription.h>
#include <unirender/Factory.h>
#include <painting2/RenderSystem.h>
#include <taskgraph/ParamImpl.h>

namespace
{

const size_t MAX_DRAW_IMAGE_NUM  = 10;
const size_t MAX_DRAW_IMAGE_SIZE = 2048;

}

namespace tasklab
{

void ImageRenderer::Draw(const ur::Device& dev, ur::Context& ctx,
                         const taskgraph::ParamPtr& param) const
{
    if (m_cached != param) {
        m_cached = param;
        UpdateRenderList(dev);
    }

    auto rs = ur::DefaultRenderState2D();
    sm::Matrix2D mt;
    for (auto& r : m_renderable)
    {
        if (!r) {
            continue;
        }

        sm::rect pos;
        pos.xmin = pos.ymin = 0;
        pos.xmax = static_cast<float>(r->GetWidth());
        pos.ymax = static_cast<float>(r->GetHeight());
        pt2::RenderSystem::DrawTexture(dev, ctx, rs, r, pos, mt, false);

        const float dx = static_cast<float>(r->GetWidth() + 10);
        mt.Translate(dx, 0);
    }
}

void ImageRenderer::UpdateRenderList(const ur::Device& dev) const
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
            m_renderable.push_back(CreateTexture(dev, img));
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
                m_renderable.push_back(CreateTexture(dev, img));
            }
        }
    }
        break;
    }
}

ur::TexturePtr ImageRenderer::CreateTexture(const ur::Device& dev,
                                             const prim::Bitmap<short>& img)
{
    ur::TexturePtr tex = nullptr;

    const auto w = img.Width();
    const auto h = img.Height();
    const auto c = img.Channels();

    auto vals = img.GetPixels();

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
        ur::TextureDescription desc;
        desc.target = ur::TextureTarget::Texture2D;
        desc.width  = w;
        desc.height = h;
        desc.format = ur::TextureFormat::RGB;
        tex = dev.CreateTexture(desc, pixels.data());
    }
        break;
    case 3:
    case 4:
    {
        std::vector<char> pixels(w * h * c);
        for (size_t i = 0, n = w * h * c; i < n; ++i) {
            pixels[i] = static_cast<char>(vals[i]);
        }
        ur::TextureDescription desc;
        desc.target = ur::TextureTarget::Texture2D;
        desc.width  = w;
        desc.height = h;
        desc.format = c == 3 ? ur::TextureFormat::RGB : ur::TextureFormat::RGBA8;
        tex = dev.CreateTexture(desc, pixels.data());
    }
        break;
    }

    return tex;
}

}