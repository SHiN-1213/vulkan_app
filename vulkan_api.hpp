#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3native.h>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <iostream>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <array>
#include <optional>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <fstream>
#include <chrono>

#include "reel_err.hpp"

namespace reel
{
	class VulkanApi
	{
	private:
		static constexpr uint32_t m_WIDTH = 800;
		static constexpr uint32_t m_HEIGHT = 600;

		static constexpr int m_MAX_FRAMES_IN_FLIGHT = 2;


		const std::vector<const char *> m_validation_layers = {
				"VK_LAYER_KHRONOS_validation"
		};
#ifdef NDEBUG
		static constexpr bool m_enable_validation_layers = false;
#else
		static constexpr bool m_enable_validation_layers = true;
#endif
		GLFWwindow *m_window;

		VkInstance m_instance;

		VkDebugUtilsMessengerEXT m_debug_messenger;

		VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;

		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;


			bool isComplete()
			{
				return graphicsFamily.has_value() &&
				       presentFamily.has_value();
			}
		};

		const std::vector<const char *> m_device_extensions = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		VkDevice m_device;

		VkQueue m_graphics_queue;

		VkQueue m_present_queue;

		VkSurfaceKHR m_surface;

		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		VkSwapchainKHR m_swap_chain;
		std::vector<VkImage> m_swap_chain_images;
		VkFormat m_swap_chain_image_format;
		VkExtent2D m_swap_chain_extent;

		std::vector<VkImageView> m_swap_chain_image_views;

		VkRenderPass m_render_pass;

		VkDescriptorSetLayout m_descriptor_set_layout;

		VkPipelineLayout m_pipeline_layout;

		VkPipeline m_graphics_pipeline;

		std::vector<VkFramebuffer> m_swap_chain_frame_buffers;

		VkCommandPool m_command_pool;

		uint32_t m_current_frame = 0;

		std::vector<VkCommandBuffer> m_command_buffers;

		std::vector<VkSemaphore> m_image_available_semaphores;
		std::vector<VkSemaphore> m_render_finished_semaphores;
		std::vector<VkFence> m_in_flight_fences;

		bool m_framebuffer_resized = false;

		struct Vertex
		{
			glm::vec2 pos;
			glm::vec3 color;

			static VkVertexInputBindingDescription getBindingDescription()
			{
				VkVertexInputBindingDescription bindingDescription{};
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(Vertex);
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				return bindingDescription;
			}

			static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
			{
				std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

				attributeDescriptions[0].binding = 0;
				attributeDescriptions[0].location = 0;
				attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
				attributeDescriptions[0].offset = offsetof(Vertex, pos);

				attributeDescriptions[1].binding = 0;
				attributeDescriptions[1].location = 1;
				attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[1].offset = offsetof(Vertex, color);

				return attributeDescriptions;
			}
		};

		const std::vector<Vertex> m_vertices = {
				{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
				{{0.5f,  -0.5f}, {0.0f, 1.0f, 0.0f}},
				{{0.5f,  0.5f},  {0.0f, 0.0f, 1.0f}},
				{{-0.5f, 0.5f},  {1.0f, 1.0f, 1.0f}}
		};

		const std::vector<uint16_t> m_indices = {
				0, 1, 2, 2, 3, 0
		};

		struct UniformBufferObject
		{
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;
		};

		VkBuffer m_vertex_buffer;
		VkDeviceMemory m_vertex_buffer_memory;

		VkBuffer m_index_buffer;
		VkDeviceMemory m_index_buffer_memory;

		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<void *> uniformBuffersMapped;

		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;

	public:
		void run();

	private:
		static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

		void initWindow();

		void initVulkan();

		bool checkValidationLayerSupport();

		std::vector<const char *> getRequiredExtensions();

		void createInstance();

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

		void setupDebugMessenger();

		void createSurface();

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		bool checkDeviceExtensionSupport(VkPhysicalDevice device);

		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		bool isDeviceSuitable(VkPhysicalDevice device);

		void pickPhysicalDevice();

		void createLogicalDevice();

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

		void createSwapChain();

		void cleanupSwapChain();

		void recreateSwapChain();

		void createImageViews();

		void createRenderPass();

		void createDescriptorSetLayout();

		VkShaderModule createShaderModule(const std::vector<char> &code);

		void createGraphicsPipeline();

		void createFramebuffers();

		void createCommandPool();

		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
		                  VkBuffer &buffer,
		                  VkDeviceMemory &bufferMemory);

		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		void createVertexBuffer();

		void createIndexBuffer();

		void createUniformBuffers();

		void createDescriptorPool();

		void createDescriptorSets();

		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		void createCommandBuffers();

		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

		void createSyncObjects();

		void updateUniformBuffer(uint32_t currentImage);

		void drawFrame();

		void mainLoop();

		void cleanup();
	};

}
