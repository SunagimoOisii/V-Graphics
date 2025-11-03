#pragma once

#include <vulkan/vulkan.h>

struct ImageLayoutTransition
{
    VkImageLayout oldLayout;
    VkImageLayout newLayout;
    VkAccessFlags srcAccessMask;
    VkAccessFlags dstAccessMask;
    VkPipelineStageFlags srcStage;
    VkPipelineStageFlags dstStage;

    //描画でよく使うレイアウトは以下関数で容易に得られるようにする

    //Undefined状態から描画先としてのレイアウトへ
    static ImageLayoutTransition FromUndefinedToColorAttachment();

    //PresentSrc状態から描画先としてのレイアウトへ
    static ImageLayoutTransition FromPresentSrcToColorAttachment();

    //描画先からPresentSrcの状態レイアウトへ
    static ImageLayoutTransition FromColorToPresent();
};