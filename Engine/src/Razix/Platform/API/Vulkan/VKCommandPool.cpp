#include "rzxpch.h"
#include "VKCommandPool.h"

#ifdef RAZIX_RENDER_API_VULKAN 

#include "VKDevice.h"
#include "VKUtilities.h"

namespace Razix {
    namespace Graphics {


        VKCommandPool::VKCommandPool(int queueIndex, VkCommandPoolCreateFlags flags)
        {
            VkCommandPoolCreateInfo cmdPoolCI{};
            cmdPoolCI.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            cmdPoolCI.queueFamilyIndex = queueIndex;
            cmdPoolCI.flags = flags;

            VK_CHECK_RESULT(vkCreateCommandPool(VKDevice::Get().getDevice(), &cmdPoolCI, nullptr, &m_Handle));
        }

        void VKCommandPool::reset()
        {
            VK_CHECK_RESULT(vkResetCommandPool(VKDevice::Get().getDevice(), m_Handle, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT));
        }

        void VKCommandPool::destroy()
        {
            vkDestroyCommandPool(VKDevice::Get().getDevice(), m_Handle, nullptr);
        }

    }
}
#endif