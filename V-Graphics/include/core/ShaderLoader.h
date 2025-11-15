#pragma once

#include <filesystem>

#include "core/VulkanContext.h"

namespace loader
{
    //SPIR-V‚ð“Ç‚ÝŽæ‚é
    VkShaderModule LoadShaderModule(const std::filesystem::path& shaderSpvPath);
};