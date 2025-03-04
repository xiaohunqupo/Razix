#pragma once

#include "Razix/Core/RZCore.h"

#include "Razix/Core/RZRoot.h"

namespace Razix {
    namespace Gfx {

        /**
         * Controls the Camera Input to update it by interacting with the Engine Input system 
         */
        class RAZIX_API CameraInput
        {
        public:
            CameraInput()          = default;
            virtual ~CameraInput() = default;

        private:
        };
    }    // namespace Gfx
}    // namespace Razix
