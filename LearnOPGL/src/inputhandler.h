#pragma once

#include "pch.h"
#include "camera.h"
#include "time.h"

class InputHandler
{
public:
    InputHandler(GLFWwindow* window, Camera& camera) : m_window(window), m_camera(camera) {
		glfwSetWindowUserPointer(window, this); // store pointer to this instance for use in static callbacks

		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
		glfwSetCursorPosCallback(window, mouseCallback);
        glfwSetScrollCallback(window, scrollCallback);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    // Call once per frame inside the render loop
    void processInput() {
        float dt = Time::getInstance().getDeltaTime();

        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_window, true);

        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) m_camera.ProcessKeyboard(FORWARD, dt);
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) m_camera.ProcessKeyboard(BACKWARD, dt);
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) m_camera.ProcessKeyboard(LEFT, dt);
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) m_camera.ProcessKeyboard(RIGHT, dt);
        if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) m_camera.ProcessKeyboard(UP, dt);
        if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) m_camera.ProcessKeyboard(DOWN, dt);

        bool key1IsPressed = glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS;
        if (key1IsPressed && !m_key1WasPressed)
        {
            m_wireframe = !m_wireframe;
            glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);
            std::cout << (m_wireframe ? "Wireframe ON\n" : "Wireframe OFF\n");
        }
        m_key1WasPressed = key1IsPressed;

        bool key2IsPressed = glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS;
        if (key2IsPressed && !m_key2WasPressed)
        {
            bool cursorDisabled = glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
            glfwSetInputMode(m_window, GLFW_CURSOR, cursorDisabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            if (!cursorDisabled) m_firstMouse = true;
            std::cout << (cursorDisabled ? "Cursor Enabled\n" : "Cursor Disabled\n");
        }
        m_key2WasPressed = key2IsPressed;
    }

    // GLFW callback hooks — registered in the constructor
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }
    static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
        auto* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (handler->m_firstMouse)
        {
            handler->m_lastX = xpos;
            handler->m_lastY = ypos;
            handler->m_firstMouse = false;
        }

        handler->m_camera.ProcessMouseMovement(xpos - handler->m_lastX, handler->m_lastY - ypos);
        handler->m_lastX = xpos;
        handler->m_lastY = ypos;
    }
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        auto* handler = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
        handler->m_camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }

private:
    GLFWwindow* m_window;
    Camera& m_camera;

    float m_lastX = 0;
    float m_lastY = 0;
    bool  m_firstMouse = true;
    bool  m_wireframe = false;

    bool m_key1WasPressed = false;
    bool m_key2WasPressed = false;
};