#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include "../VEInclude.h"

#ifndef ENGINE_UI_H
#define ENGINE_UI_H

namespace ui
{

    static ImGui_ImplVulkanH_Window g_MainWindowData;
    static int g_MinImageCount = 2;
    static bool g_SwapChainRebuild = false;

    static void check_vk_result(VkResult err)
    {
        if (err == 0)
            return;
        fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
        if (err < 0)
            abort();
    }

    class EngineUI
    {
    private:
        bool m_is_active = true;
        GLFWwindow *window;

    public:
        EngineUI();
        ~EngineUI();
        void initWindow(int WIDTH, int HEIGHT);
        void render();
    };

    EngineUI::EngineUI()
    {
    }

    EngineUI::~EngineUI()
    {
    }

    void EngineUI::initWindow(int WIDTH, int HEIGHT)
    {
        window = glfwCreateWindow(WIDTH, HEIGHT, "Dear ImGui GLFW+Vulkan example", nullptr, nullptr);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();

        printf("1\n");

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForVulkan(window, true);
        printf("2\n");

        printf("3\n");
        auto desc_pool = getRendererForwardPointer()->getDescriptorPool();
        printf("4\n");

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = getEnginePointer()->getInstance();
        init_info.PhysicalDevice = getRendererForwardPointer()->getPhysicalDevice();
        init_info.Device = getRendererForwardPointer()->getDevice();
        init_info.QueueFamily = 0;
        init_info.Queue = getRendererForwardPointer()->getGraphicsQueue();
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.MinImageCount = 2;
        init_info.ImageCount = 10;
        init_info.DescriptorPool = desc_pool;
        init_info.CheckVkResultFn = check_vk_result;
        printf("5\n");

        VkAttachmentDescription attachment = {};
        attachment.format = getRendererForwardPointer()->getSwapChainImageFormat();
        attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        printf("6\n");
        VkRenderPass renderpass;
        vh::vhRenderCreateRenderPass(getRendererForwardPointer()->getDevice(), getRendererForwardPointer()->getSwapChainImageFormat(), getRendererForwardPointer()->getDepthMap()->m_format, VK_ATTACHMENT_LOAD_OP_LOAD, &renderpass);
        printf("7\n");
        ImGui_ImplVulkan_Init(&init_info, renderpass);
        printf("8\n");
        ImGui_ImplGlfw_InitForVulkan(window, true);
        printf("9\n");

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
    }

    void EngineUI::render()
    {
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            // feed inputs to dear imgui, start new frame
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // render your GUI
            ImGui::Begin("Demo window");
            ImGui::Button("Hello!");
            ImGui::End();

            // Render dear imgui into screen
            ImGui::Render();

            glfwSwapBuffers(window);
        }
    }

} // namespace ui
#endif