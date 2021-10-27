
#pragma once

#include "Razix/Core/Core.h"
#include "Razix/Core/OS/KeyCodes.h"

namespace Razix
{    
    /**
     * Input manager for the Engine to interface with various input devices
     * such as Keyboard, Mouse, Joystick and other HID devices
     */
    class RAZIX_API RZInput
    {
    public:
        /* Tells whether a key was pressed or not */
        inline static bool IsKeyPressed(Razix::KeyCode::Key keycode) { return sInstance->IsKeyPressedImpl(int(keycode)); }
        /* Tells whether if a key was released after being pressed */
        inline static bool IsKeyReleased(Razix::KeyCode::Key keycode) { return sInstance->IsKeyReleasedImpl(int(keycode)); }
        /* Tells if a key is being held */
        inline static bool IsKeyHeld(Razix::KeyCode::Key keycode) { return sInstance->IsIsKeyHeldImpl(int(keycode)); }

        /* Tells if a mouse button was being pressed */
        inline static bool IsMouseButtonPressed(Razix::KeyCode::MouseKey button) { return sInstance->IsMouseButtonPressedImpl(int(button)); }

        /* Gets the current position of the mouse */
        inline static std::pair<float, float> GetMousePosition() { return sInstance->GetMousePositionImpl(); }
        /* Gets the X-Axis position of the mouse in screen space */
        inline static float GetMouseX() { return sInstance->GetMouseXImpl(); }
        /* Gets the Y-Axis position of the mouse in screen space */
        inline static float GetMouseY() { return sInstance->GetMouseYImpl(); }

    protected:
        /**
         * OS/API specific Implementation for the key press
         * This should be Implementation per OS
         */
        virtual bool IsKeyPressedImpl(int keycode) = 0;
        /**
         * OS/API specific Implementation for the key release
         * This should be Implementation per OS
         */
        virtual bool IsKeyReleasedImpl(int keycode) = 0;
        /**
         * OS/API specific Implementation for the key hold
         * This should be Implementation per OS
         */
        virtual bool IsIsKeyHeldImpl(int keycode) = 0;

        /**
         * OS/API specific Implementation for the mouse button press
         * This should be Implementation per OS
         */
        virtual bool IsMouseButtonPressedImpl(int button) = 0;

        /**
         * OS/API specific Implementation for the mouse position
         * This should be Implementation per OS
         */
        virtual std::pair<float, float> GetMousePositionImpl() = 0;
        /**
         * OS/API specific Implementation for Mouse X position
         * This should be Implementation per OS
         */
        virtual float GetMouseXImpl() = 0;
        /**
         * OS/API specific Implementation for Mouse Y position
         * This should be Implementation per OS
         */
        virtual float GetMouseYImpl() = 0;

    private:
        /* The Global Input variable for the Engine, from which the Input information is retrieved */
        static RZInput* sInstance;
    };
}