#include "vulkan_api.hpp"

namespace reel
{
	void VulkanApi::run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

	void VulkanApi::initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(m_WIDTH, m_HEIGHT, "Vulkan", nullptr, nullptr);

	}

	void VulkanApi::initVulkan()
	{
		createInstance();
		setupDebugMessenger();
		pickPhysicalDevice();
	}

	bool VulkanApi::checkValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount,
		                                   availableLayers.data());

		for (const char *layerName: m_validation_layers)
		{
			bool layerFound = false;

			for (const auto &layerProperties: availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}
		return true;
	}

	std::vector<const char *> VulkanApi::getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char **glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char *> extensions(glfwExtensions,
		                                     glfwExtensions + glfwExtensionCount);

		if (m_enable_validation_layers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	void VulkanApi::createInstance()
	{
		if (m_enable_validation_layers && !checkValidationLayerSupport())
		{
			throw std::runtime_error("validation layers requested, but not available!");
		}

		VkApplicationInfo app_info;
		app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pApplicationName = "hello vulkan";
		app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.pEngineName = "no engine";
		app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo create_info;
		create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		create_info.pApplicationInfo = &app_info;

		auto extensions = getRequiredExtensions();
		create_info.enabledExtensionCount =
				static_cast<uint32_t>(extensions.size());
		create_info.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (m_enable_validation_layers)
		{
			create_info.enabledLayerCount = static_cast<uint32_t>(m_validation_layers.size());
			create_info.ppEnabledLayerNames = m_validation_layers.data();

			populateDebugMessengerCreateInfo(debugCreateInfo);
			create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
		}
		else
		{
			create_info.enabledLayerCount = 0;

			create_info.pNext = nullptr;
		}

		if (vkCreateInstance(&create_info, nullptr, &m_instance) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create instance!");
		}
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
			void *pUserData)
	{


		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
	                                      const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
	                                      const VkAllocationCallbacks *pAllocator,
	                                      VkDebugUtilsMessengerEXT *pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
				vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
	                                   const VkAllocationCallbacks *pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
		                                                                        "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}

	void VulkanApi::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity =
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType =
				VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	void VulkanApi::setupDebugMessenger()
	{
		if (!m_enable_validation_layers) return;

		VkDebugUtilsMessengerCreateInfoEXT create_info{};
		populateDebugMessengerCreateInfo(create_info);


		if (CreateDebugUtilsMessengerEXT(m_instance, &create_info, nullptr, &m_debug_messenger) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to set up debug messenger!");
		}
	}

	VulkanApi::QueueFamilyIndices VulkanApi::findQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto &queueFamily: queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}

			if (indices.isComplete())
			{
				break;
			}

			i++;
		}

		return indices;
	}

	bool VulkanApi::isDeviceSuitable(VkPhysicalDevice device)
	{
//		VkPhysicalDeviceProperties deviceProperties;
//		VkPhysicalDeviceFeatures deviceFeatures;
//		vkGetPhysicalDeviceProperties(device, &deviceProperties);
//		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		QueueFamilyIndices indices = findQueueFamilies(device);

		return indices.isComplete();

		return true;
	}

	void VulkanApi::pickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
		if (deviceCount == 0)
		{
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

		for (const auto &device: devices)
		{
			if (isDeviceSuitable(device))
			{
				m_physical_device = device;
				break;
			}
		}
		if (m_physical_device == VK_NULL_HANDLE)
		{
			throw std::runtime_error("failed to find a suitable GPU!");
		}

	}


	void VulkanApi::mainLoop()
	{
		while (!glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();
		}

	}

	void VulkanApi::cleanup()
	{
		if (m_enable_validation_layers)
		{
			DestroyDebugUtilsMessengerEXT(m_instance, m_debug_messenger, nullptr);
		}

		vkDestroyInstance(m_instance, nullptr);
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}