#pragma once

#include "core/VulkanContext.h"
#include "core/GPUResourceBase.h"

class IBufferResource
{
public:
    virtual bool IsHostAccessible() const = 0;
    virtual VkBuffer GetVkBuffer() const = 0;
    virtual VkDeviceSize GetBufferSize() const = 0;

    virtual void SetAccessFlags(const VkAccessFlags flags) = 0;
    virtual VkAccessFlags GetAccessFlags() const = 0;

    virtual void* Map() = 0;
    virtual void Unmap() = 0;

    virtual VkDescriptorBufferInfo GetDescriptorInfo() const = 0;
};

template<typename T>
class BufferResource : public GPUResourceBase<T>, public IBufferResource
{
public:
    BufferResource(const BufferResource&) = delete;
    BufferResource& operator=(const BufferResource&) = delete;

    virtual ~BufferResource() { Cleanup(); }
    virtual void Cleanup();

    bool IsHostAccessible() const override { return (m_memProps & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0; }
    VkAccessFlags GetAccessFlags() const override { return m_accessFlags; }
    void SetAccessFlags(const VkAccessFlags flags) override { m_accessFlags = flags; }

    VkBuffer GetVkBuffer() const override { return m_buffer; }
    VkDeviceSize GetBufferSize() const override { return m_size; }

    VkDescriptorBufferInfo GetDescriptorInfo() const override;
protected:
    BufferResource() = default;

    bool CreateBuffer(const VkBufferCreateInfo& createInfo, VkMemoryPropertyFlags memProps);
    VkBuffer m_buffer{};
    VkDeviceMemory m_memory{};
    VkDeviceSize m_size{};
    VkMemoryPropertyFlags m_memProps{};
    VkAccessFlags m_accessFlags = VK_ACCESS_NONE;
};



template<typename T>
void BufferResource<T>::Cleanup()
{
    VulkanContext& context = VulkanContext::Get();
    VkDevice device = context.GetVkDevice();

    if (m_buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(device, m_buffer, nullptr);
        m_buffer = VK_NULL_HANDLE;
    }
    if (m_memory != VK_NULL_HANDLE)
    {
        vkFreeMemory(device, m_memory, nullptr);
        m_memory = VK_NULL_HANDLE;
    }
    m_size = 0;
}

template<typename T>
VkDescriptorBufferInfo BufferResource<T>::GetDescriptorInfo() const
{
    return VkDescriptorBufferInfo
    {
        .buffer = m_buffer,
        .offset = 0,
        .range = m_size
    };
}

template<typename T>
bool BufferResource<T>::CreateBuffer(const VkBufferCreateInfo& createInfo, VkMemoryPropertyFlags memProps)
{
    VulkanContext& context = VulkanContext::Get();
    VkDevice device = context.GetVkDevice();

    auto result = vkCreateBuffer(device, &createInfo, nullptr, &m_buffer);
    if (result != VK_SUCCESS)
    {
        return false;
    }

    // Get required memory size for the buffer
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memRequirements.size,
        .memoryTypeIndex = context.FindMemoryType(memRequirements, memProps),
    };

    result = vkAllocateMemory(device, &allocInfo, nullptr, &m_memory);
    if (result != VK_SUCCESS)
    {
        return false;
    }

    vkBindBufferMemory(device, m_buffer, m_memory, 0);
    m_size = createInfo.size;
    m_memProps = memProps;

    return true;
}

class VertexBuffer : public BufferResource<VertexBuffer>
{
    friend class GPUResourceBase<VertexBuffer>;
private:
    VertexBuffer() = default;
public:
    virtual ~VertexBuffer() = default;

    virtual void* Map() override;
    virtual void Unmap() override;

    bool Initialize(VkDeviceSize size, VkMemoryPropertyFlags memProps);

    //Create, Initializeを1度で処理するための作成関数
    static std::shared_ptr<VertexBuffer> Create(VkDeviceSize size, VkMemoryPropertyFlags memProps)
    {
        auto buffer = GPUResourceBase::Create();
        if (!buffer->Initialize(size, memProps)) { return nullptr; }
        return buffer;
    }
};

class StagingBuffer : public BufferResource<StagingBuffer>
{
    friend class GPUResourceBase<StagingBuffer>;
private:
    StagingBuffer() = default;

public:
    virtual ~StagingBuffer() = default;

    virtual void* Map() override;
    virtual void Unmap() override;

    bool Initialize(VkDeviceSize size);

    // Create, Initializeを1度で処理するための作成関数
    static std::shared_ptr<StagingBuffer> Create(VkDeviceSize size)
    {
        auto buffer = GPUResourceBase::Create();
        if (!buffer->Initialize(size)) { return nullptr; }
        return buffer;
    }
};

class IndexBuffer : public BufferResource<IndexBuffer>
{
    friend class GPUResourceBase<IndexBuffer>;
private:
    IndexBuffer() = default;
public:
    virtual ~IndexBuffer() = default;

    virtual void* Map() override;
    virtual void Unmap() override;

    bool Initialize(VkDeviceSize size, VkMemoryPropertyFlags memProps);

    // Create, Initializeを1度で処理するための作成関数
    static std::shared_ptr<IndexBuffer> Create(VkDeviceSize size, VkMemoryPropertyFlags memProps)
    {
        auto buffer = GPUResourceBase::Create();
        if (!buffer->Initialize(size, memProps)) { return nullptr; }
        return buffer;
    }
};

class UniformBuffer : public BufferResource<UniformBuffer>
{
    friend class GPUResourceBase<UniformBuffer>;
public:
    UniformBuffer() = default;
    virtual ~UniformBuffer() = default;

    virtual void* Map() override;
    virtual void Unmap() override;

    bool Initialize(VkDeviceSize size);

    // Create, Initializeを1度で処理するための作成関数
    static std::shared_ptr<UniformBuffer> Create(VkDeviceSize size)
    {
        auto buffer = GPUResourceBase::Create();
        if (!buffer->Initialize(size)) { return nullptr; }
        return buffer;
    }
};
