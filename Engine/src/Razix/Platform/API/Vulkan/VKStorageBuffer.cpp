// clang-format off
#include "rzxpch.h"
// clang-format on
#include "VKStorageBuffer.h"

namespace Razix {
    namespace Graphics {

        VKStorageBuffer::VKStorageBuffer(uint32_t size, const std::string& name)
            : VKBuffer(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, size, nullptr, name)
        {
            RAZIX_PROFILE_FUNCTIONC(RZ_PROFILE_COLOR_GRAPHICS);
        }

        void* VKStorageBuffer::GetData()
        {
            VKBuffer::map();
            void* mapped = VKBuffer::getMappedRegion();
            VKBuffer::unMap();
            return mapped;
        }

        void VKStorageBuffer::SetData(uint32_t size, const void* data)
        {
            RAZIX_PROFILE_FUNCTIONC(RZ_PROFILE_COLOR_GRAPHICS);

            VKBuffer::setData(size, data);
        }

        void VKStorageBuffer::Destroy()
        {
            VKBuffer::destroy();
        }

    }    // namespace Graphics
}    // namespace Razix