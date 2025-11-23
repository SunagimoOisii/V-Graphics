#include <Windows.h>
#include <filesystem>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/AssetPath.h"
#include "core/VulkanContext.h"
#include "core/GLFWSurfaceProvider.h"
#include "SimpleCubeApp.h"

int __stdcall wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	//exeのあるディレクトリをカレントディレクトリに設定
	wchar_t exePath[MAX_PATH];
	GetModuleFileNameW(nullptr, exePath, MAX_PATH);
	std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();
	SetCurrentDirectoryW(exeDir.c_str());

	std::filesystem::path assetDir = exeDir / "../../assets";
	SetAssetRootPath(assetDir);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//GLFW初期化
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	//ウィンドウ作成
	GLFWwindow* window = glfwCreateWindow(1280, 720, "DirectX999999999999", nullptr, nullptr);
	GLFWSurfaceProvider surfaceProvider(window);

	//必要な拡張機能を取得し、Vulkan初期化
	auto& vulkanCtx = VulkanContext::Get();
	vulkanCtx.GetWindowSystemExtensions = [=](auto& extensionList)
	{
		uint32_t extCount = 0;
		const char** extensions = glfwGetRequiredInstanceExtensions(&extCount);
		if (extCount > 0)
		{
			extensionList.insert(extensionList.end(), extensions, extensions + extCount);
		}
	};
	vulkanCtx.Initialize("Window", &surfaceProvider);
	vulkanCtx.RecreateSwapchain();

	//アプリケーション初期化
	SimpleCubeApp app{};
	app.OnInitialize();

	//メッセージループ
	while (glfwWindowShouldClose(window) == GLFW_FALSE)
	{
		glfwPollEvents();
		app.OnDrawFrame();
	}

	//終了処理
	app.OnCleanup();
	vulkanCtx.Cleanup();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}