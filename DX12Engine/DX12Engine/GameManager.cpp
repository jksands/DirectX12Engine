#include "GameManager.h"
#include "Application.h"
// #include "Window.h"
#include "pch.h";

GameManager::GameManager(const std::wstring& name, int width, int height, bool vSync)
{
    m_Name = name;
    m_Width = width;
    m_Height = height;
    m_vSync = vSync;
}

GameManager::~GameManager()
{
}

bool GameManager::Initialize()
{
    // Check for DirectX Math library support.
    if (!DirectX::XMVerifyCPUSupport())
    {
        MessageBoxA(NULL, "Failed to verify DirectX Math library support.", "Error", MB_OK | MB_ICONERROR);
        return false;
    }

    m_pWindow = Application::Get().CreateRenderWindow(m_Name, m_Width, m_Height, m_vSync);
    m_pWindow->RegisterCallbacks(shared_from_this());
    m_pWindow->Show();

    return true;
}

bool GameManager::LoadContent()
{
    return false;
}

void GameManager::UnloadContent()
{
}

void GameManager::Destroy()
{
    // Destroy the window instance
    Application::Get().DestroyWindow(m_pWindow);
    m_pWindow.reset();
}

void GameManager::OnUpdate(UpdateEventArgs& e)
{
}

void GameManager::OnRender(RenderEventArgs& e)
{
}

void GameManager::OnKeyPressed(KeyEventArgs& e)
{
}

void GameManager::OnKeyReleased(KeyEventArgs& e)
{
}

void GameManager::OnMouseMoved(MouseMotionEventArgs& e)
{
}

void GameManager::OnMouseButtonPressed(MouseButtonEventArgs& e)
{
}

void GameManager::OnMouseButtonReleased(MouseButtonEventArgs& e)
{
}

void GameManager::OnMouseWheel(MouseWheelEventArgs& e)
{
}

void GameManager::OnResize(ResizeEventArgs& e)
{
}

void GameManager::OnWindowDestroy()
{
}
