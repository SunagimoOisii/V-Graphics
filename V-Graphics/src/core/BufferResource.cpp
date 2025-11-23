#include "core/BufferResource.h"

bool VertexBuffer::Initialize(VkDeviceSize size, VkMemoryPropertyFlags memProps)
{
    auto& context = VulkanContext::Get();
    VkBufferCreateInfo bufferInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    SetAccessFlags(VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);
    return CreateBuffer(bufferInfo, memProps);
}

void* VertexBuffer::Map()
{
    if (!(m_memProps & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) return nullptr;

    void* mapped = nullptr;
    vkMapMemory(VulkanContext::Get().GetVkDevice(), m_memory, 0, m_size, 0, &mapped);
    return mapped;
}

void VertexBuffer::Unmap()
{
    if (!(m_memProps & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) return;
    vkUnmapMemory(VulkanContext::Get().GetVkDevice(), m_memory);
}

bool StagingBuffer::Initialize(VkDeviceSize size)
{
    VkBufferCreateInfo bufferInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    VkMemoryPropertyFlags memProps = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    SetAccessFlags(VK_ACCESS_HOST_WRITE_BIT);
    return CreateBuffer(bufferInfo, memProps);
}

void* StagingBuffer::Map()
{
    void* mapped = nullptr;
    VkDevice device = VulkanContext::Get().GetVkDevice();
    vkMapMemory(device, m_memory, 0, m_size, 0, &mapped);
    return mapped;
}

void StagingBuffer::Unmap()
{
    VkDevice device = VulkanContext::Get().GetVkDevice();
    vkUnmapMemory(device, m_memory);
}

bool IndexBuffer::Initialize(VkDeviceSize size, VkMemoryPropertyFlags memProps)
{
    VkBufferCreateInfo bufferInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    SetAccessFlags(VK_ACCESS_INDEX_READ_BIT);
    return CreateBuffer(bufferInfo, memProps);
}

void* IndexBuffer::Map()
{
    if (!(m_memProps & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) return nullptr;

    void* mapped = nullptr;
    vkMapMemory(VulkanContext::Get().GetVkDevice(), m_memory, 0, m_size, 0, &mapped);
    return mapped;
}

void IndexBuffer::Unmap()
{
    if (!(m_memProps & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) return;

    vkUnmapMemory(VulkanContext::Get().GetVkDevice(), m_memory);
}

bool UniformBuffer::Initialize(VkDeviceSize size)
{
    VkBufferCreateInfo bufferInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    VkMemoryPropertyFlags memProps = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    SetAccessFlags(VK_ACCESS_SHADER_READ_BIT);
    return CreateBuffer(bufferInfo, memProps);
}

void* UniformBuffer::Map()
{
    void* mapped = nullptr;
    vkMapMemory(VulkanContext::Get().GetVkDevice(), m_memory, 0, m_size, 0, &mapped);
    return mapped;
}

void UniformBuffer::Unmap()
{
    vkUnmapMemory(VulkanContext::Get().GetVkDevice(), m_memory);
}