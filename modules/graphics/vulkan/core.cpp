#include "core.h"
#include "window.h"
#include "shader.h"
#include "vertex2d.h"

Graphics Graphics::_graphicsInstance{};
bool Graphics::enableValidationLayers = false;
const int Graphics::MAX_FRAMES_IN_FLIGHT = 2;

Graphics::Graphics() {
    _instance = VK_NULL_HANDLE;
    _physicalDevice = VK_NULL_HANDLE;
    _logicalDevice = VK_NULL_HANDLE;
    _queues = {};
    _windows = {};
    validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };
    deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    Debug::AssertIf::isFalse(glfwInit() == GLFW_TRUE, "Failed to initialize GLFW");
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    Graphics::createInstance(enableValidationLayers);
    createPhysicalDevice();
    createLogicalDevice(Graphics::enableValidationLayers);
    createCommandPool();
    createSyncObjects();
}

Graphics::~Graphics() {

    vkDestroyCommandPool(_logicalDevice, _commandPool, nullptr);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(_logicalDevice, imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(_logicalDevice, renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(_logicalDevice, inFlightFences[i], nullptr);
    }

    for(auto& shader : _shaders)
    {
        delete shader;
    }

    for(auto& window : _windows)
    {
        delete window;
    }

    vkDestroyDevice(_logicalDevice, nullptr);
    vkDestroyInstance(_instance, nullptr);
    glfwTerminate();
}

void Graphics::createInstance(bool enableValidationLayers) {

    if(enableValidationLayers)
    {
        Debug::WarnIf::isFalse(hasValidationLayerSupport(), "Validation layers requested, but not available! Continuing without validation layers.");
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Pomegranate Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Pomegranate Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    //Extensions

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    //GLFW Extensions

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> requiredExtensions;

    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
        requiredExtensions.push_back(glfwExtensions[i]);
    }

    //Portability Extensions
    requiredExtensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    //Ensure extensions are supported

    for (const char* extension : requiredExtensions) {
        bool found = false;

        for (const auto& availableExtension : extensions) {
            if (strcmp(availableExtension.extensionName, extension) == 0) {
                found = true;
                break;
            }
        }

        Debug::AssertIf::isFalse(found, std::string("Required extension not supported: ") + extension);
    }

    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    if(enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    Debug::AssertIf::isFalse(vkCreateInstance(&createInfo, nullptr, &_instance) == VK_SUCCESS, "Failed to create Vulkan instance");
}

void Graphics::createPhysicalDevice() {
    if(_physicalDevice != VK_NULL_HANDLE)
        return;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_instance,&deviceCount, nullptr);

    Debug::AssertIf::isZero(deviceCount,"Unable to find GPU(s) with Vulkan support!");
    Debug::Log::pass(std::string("Found ") + std::to_string((int)deviceCount) + " devices with Vulkan support");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

    for(const auto& device : devices)
    {
        if(isDeviceSuitable(device)) {
            _physicalDevice = device;
            break;
        }
    }

    Debug::AssertIf::isNull(_physicalDevice, "Failed to find suitable GPU!");
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(_physicalDevice, &deviceProperties);
    Debug::Log::pass(std::string("Found suitable GPU - ") + deviceProperties.deviceName);
}

void Graphics::createLogicalDevice(bool enableValidationLayers) {
    if(_logicalDevice != VK_NULL_HANDLE)
        return;

    QueueFamilyIndices indices = getQueueFamilies(_physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for(uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }


    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = queueCreateInfos.size();
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

#ifdef __APPLE__
    //Portability Subset extension
    const char* extensions[] = {VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME};

    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = extensions;
#endif


    createInfo.pEnabledFeatures = &deviceFeatures;

    if(enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    createInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    Debug::AssertIf::isFalse(vkCreateDevice(_physicalDevice,&createInfo, nullptr,&_logicalDevice) == VK_SUCCESS, "Failed to create logical device");
    Debug::Log::pass("Created logical device.");

    vkGetDeviceQueue(_logicalDevice, indices.graphicsFamily.value(), 0, &_queues.graphicsQueue);
    vkGetDeviceQueue(_logicalDevice, indices.presentFamily.value(), 0, &_queues.presentQueue);
    Debug::Log::pass("Created queues.");
}

void Graphics::createCommandPool() {
    QueueFamilyIndices queueFamilyIndices = Graphics::getInstance()->getQueueFamilies(_physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    Debug::AssertIf::isFalse(vkCreateCommandPool(_logicalDevice, &poolInfo, nullptr, &_commandPool) == VK_SUCCESS, "Failed to create command pool!");
    Debug::Log::pass("Successfully created command pool!");
}

void Graphics::createRenderPass(Window* window) {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = window->_swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    Debug::AssertIf::isFalse(vkCreateRenderPass(_logicalDevice,&renderPassInfo,nullptr,&window->_renderPass) == VK_SUCCESS, "Failed to create render pass!");
}

Graphics::GraphicsPipelineGroup Graphics::createGraphicsPipeline(ShaderBase* shader, Window* window, RenderInfo renderInfo, VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions) {
    GraphicsPipelineGroup group{};

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
    vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageInfo.module = shader->_vertex;
    vertexShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
    fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = shader->_fragment;
    fragmentShaderStageInfo.pName = "main";

    shaderStages.push_back(vertexShaderStageInfo);
    shaderStages.push_back(fragmentShaderStageInfo);

    std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = (VkPrimitiveTopology)renderInfo.topologyMode;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)window->_swapExtent.width;
    viewport.height = (float)window->_swapExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0,0};
    scissor.extent = window->_swapExtent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = (VkPolygonMode)renderInfo.renderMode;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = (VkCullModeFlagBits)renderInfo.cullMode;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    std::vector<VkDescriptorSetLayout> layouts;
    layouts.reserve(1);
    layouts.push_back(shader->uniform.layout);
    pipelineLayoutInfo.setLayoutCount = layouts.size();
    pipelineLayoutInfo.pSetLayouts = layouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    Debug::AssertIf::isFalse(vkCreatePipelineLayout(_logicalDevice,&pipelineLayoutInfo,nullptr,&group.layout) == VK_SUCCESS, "Failed to create pipeline layout");

    Debug::Log::pass("Created fixed state");

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = shaderStages.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = group.layout;
    pipelineInfo.renderPass = window->_renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    Debug::AssertIf::isFalse(vkCreateGraphicsPipelines(_logicalDevice,VK_NULL_HANDLE,1,&pipelineInfo, nullptr,&group.pipeline) == VK_SUCCESS, "Failed to create graphics pipeline!");
    Debug::Log::pass("Successfully created graphics pipeline!");

    return group;
}

void Graphics::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        Debug::AssertIf::isFalse(vkCreateSemaphore(_logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) == VK_SUCCESS, "Failed to create semaphore!");
        Debug::AssertIf::isFalse(vkCreateSemaphore(_logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) == VK_SUCCESS, "Failed to create semaphore!");
        Debug::AssertIf::isFalse(vkCreateFence(_logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]) == VK_SUCCESS, "Failed to create semaphore!");
    }
}

bool Graphics::hasValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount,availableLayers.data());

    for(const char* layerName : validationLayers) {
        bool layerFound = false;

        for(const auto& layerProperties : availableLayers) {
            if(strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }
        if(!layerFound)
        {
            return false;
        }
    }

    return true;
}

bool Graphics::hasDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr,&extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr,&extensionCount,availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(),deviceExtensions.end());

    for(const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }
    return requiredExtensions.empty();
}

bool Graphics::isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = getQueueFamilies(device);

    bool extensionSupported = hasDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if(extensionSupported) {
        SwapChainSupportDetails swapChainSupportDetails = getSwapChainSupport(device);
        swapChainAdequate = !swapChainSupportDetails.formats.empty() && !swapChainSupportDetails.presentModes.empty();
    }

    return indices.complete() && extensionSupported && swapChainAdequate;
}

QueueFamilyIndices Graphics::getQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties > queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    //Create a temporary surface and window to get the queue family indices
    GLFWwindow* tempWindow = glfwCreateWindow(1,1,"Temp", nullptr, nullptr);
    VkSurfaceKHR tempSurface;
    if(glfwCreateWindowSurface(_instance,tempWindow, nullptr,&tempSurface) == VK_SUCCESS)
    {
        Debug::Log::pass("Created temporary surface");
    }
    else {
        //Get glfw error
        Debug::Log::fail("Failed to create temporary surface");
        const char* error;
        int code = glfwGetError(&error);
        Debug::Log::fail(std::string("Error code: ") + std::to_string(code) + " - " + std::string(error));
        throw std::runtime_error("Failed to create temporary surface");
    }

    int i = 0;
    for(const auto& queueFamily : queueFamilies) {
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, tempSurface, &presentSupport);

        if(presentSupport)
        {
            indices.presentFamily = i;
        }

        if(indices.complete())
            break;

        i++;
    }

    glfwDestroyWindow(tempWindow);
    return indices;
}

SwapChainSupportDetails Graphics::getSwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR* surface) {
    SwapChainSupportDetails details;

    GLFWwindow* tempWindow = nullptr;
    VkSurfaceKHR tempSurface;
    if(surface != nullptr)
    {
        tempSurface = *surface;
    }
    else {
        //Create a temporary surface and window to get the swap chain support details
        tempWindow = glfwCreateWindow(1,1,"Temp", nullptr, nullptr);
        glfwCreateWindowSurface(_instance, tempWindow, nullptr, &tempSurface);
    }

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, tempSurface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device,tempSurface,&formatCount, nullptr);

    if(formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, tempSurface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device,tempSurface,&presentModeCount, nullptr);

    if(presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device,tempSurface,&presentModeCount,details.presentModes.data());
    }
    if(tempWindow != nullptr)
        glfwDestroyWindow(tempWindow);

    return details;
}

VkSurfaceFormatKHR Graphics::getSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> availableFormats) {
    for(const auto& availableFormat : availableFormats)
    {
        if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR Graphics::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
    for(const auto& availablePresentMode : availablePresentModes)
    {
        if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

uint32_t Graphics::getMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

    for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    Debug::Log::fail("Failed to find suitable memory type!");
    return 0;
}

bool QueueFamilyIndices::complete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
}

Graphics* Graphics::getInstance() {
    return &_graphicsInstance;
}
