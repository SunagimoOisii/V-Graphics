#include "core/BufferResource.h"

class ResourceUploader
{
public:
    ResourceUploader() = default;
    ~ResourceUploader() = default;

    bool Initialize();
    void Cleanup();

    bool UploadBuffer(IBufferResource* target, const void* pData, size_t size, VkAccessFlags nextAccessMask);

    // “o˜^‚³‚ê‚Ä‚¢‚é“]‘—ˆ—‚ğ‚Ü‚Æ‚ß‚ÄÀs‚·‚é
    // “¯ŠúÀs‚ğs‚¢A‘S‚Ä‚Ì“]‘—ˆ—‚ªŠ®—¹Œã‚Éˆ—‚ª–ß‚é
    void SubmitAndWait();
private:
    struct PendingTransfer
    {
        std::shared_ptr<StagingBuffer> stagingBuffer;
        IBufferResource* destinationBuffer;
        VkAccessFlags    dstAccessMask;
    };
    std::vector<PendingTransfer> m_transferEntries;
    VkFence m_transferFence = VK_NULL_HANDLE;
};