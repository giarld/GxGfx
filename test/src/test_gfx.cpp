//
// Created by Gxin on 2023/11/15.
//

#include <gxx/gxxapp.h>

#include <gxx/window.h>
#include <gxx/device/charinput.h>
#include <gxx/device/gamepad.h>
#include <gxx/app_entry.h>

#include <gfx/gfx.h>
#include <gfx/gfx_tools.h>

#include <gx/gstring.h>
#include <gx/debug.h>


using namespace gx;
using namespace gxx;
using namespace gfx;

class AWindow : public Window
{
public:
    explicit AWindow(const std::string &title)
            : Window(title)
    {}

protected:
    void init() override
    {
        Window::init();

        mGfxInstance = createInstance({
                                              getApplication()->getName().c_str(),
                                              TargetApiType::Vulkan,
                                              {}});

        Log("device count = %d", mGfxInstance->deviceCount());
        printDeviceInfo(mGfxInstance->deviceInfo(0));

        mGfxContext = createContext(mGfxInstance, {
                0, {}});

        SwapChainInfo swapChainInfo{};
        swapChainInfo.nativeDisplayType = getWinContext()->platformData()->nativeDisplayType;
        swapChainInfo.nativeWindowHandle = getWinContext()->platformData()->nativeWindowHandle;
        swapChainInfo.context = getWinContext()->platformData()->context;
        swapChainInfo.backBuffer = getWinContext()->platformData()->backBuffer;
        swapChainInfo.backBufferDS = getWinContext()->platformData()->backBufferDS;

        Log(">>>>>> Context(%d)", mGfxContext->deviceIndex());
        printDeviceInfo(mGfxContext->deviceInfo());

        mFrame = createFrame(mGfxContext, {
                FrameTargetType::SwapChain,
                &swapChainInfo,
                (uint32_t) width(),
                (uint32_t) height(),
                mVSync});

        mCmdBuffer = createCommandBuffer(mGfxContext, {QueueType::Graphics, mFrame->frameBufferCount()});
    }

    bool update(double delta) override
    {
        RenderPassInfo rpInfo{};
        rpInfo.clear = RenderTargetAttachmentFlag::Color0 | RenderTargetAttachmentFlag::Depth;
        rpInfo.discard = RenderTargetAttachmentFlag::Color0 | RenderTargetAttachmentFlag::Depth;
        mCmdBuffer->begin()
                  ->bindRenderTarget(mFrame)
                  ->setClearColor({1, 0, 0, 1})
                  ->setClearDepthStencil(1.0f, 0)
                  ->beginRenderPass({0, 0, UINT32_MAX, UINT32_MAX}, rpInfo)
                  ->endRenderPass()
                  ->setClearColor({0, 1, 0, 1})
                  ->beginRenderPass({int32_t(width() / 2), 0, UINT32_MAX, UINT32_MAX}, rpInfo)
                  ->endRenderPass()
                  ->setClearColor({0, 0, 1, 1})
                  ->beginRenderPass({0, int32_t(height() / 2), UINT32_MAX, UINT32_MAX}, rpInfo)
                  ->endRenderPass()
                  ->setClearColor({1, 1, 0, 1})
                  ->beginRenderPass({int32_t(width() / 2), int32_t(height() / 2), UINT32_MAX, UINT32_MAX}, rpInfo)
                  ->endRenderPass()
                  ->end();

        if (mFrame->beginFrame()) {
            mFrame->submit(mCmdBuffer);
            mFrame->endFrame();
        }

        setTitle(GString("TestGfx: FPS: {}").arg(int32_t(1000000.0f / float(mFrame->getFrameTime()))).toString());

        return true;
    }

    void onDestroy() override
    {
        mGfxContext->waitIdle();

        destroyFrame(mFrame);
        destroyCommandBuffer(mCmdBuffer);
        destroyContext(mGfxContext);
        destroyInstance(mGfxInstance);

        Window::onDestroy();
    }

    void resetSize(int32_t w, int32_t h) override
    {
        mFrame->reset(w, h, mVSync);
    }

    void keyPressEvent(Key::Enum key, uint8_t modifier) override
    {
        Window::keyPressEvent(key, modifier);

        if (key == Key::KeyV) {
            mVSync = !mVSync;
            mFrame->reset(width(), height(), mVSync);
        }
    }

private:
    Instance mGfxInstance;
    Context mGfxContext;
    Frame mFrame;
    CommandBuffer mCmdBuffer;

    bool mVSync = false;
};

GXX_APP_MAIN(AWindow, "TestGfx", "TestGfx")