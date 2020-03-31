
/* Copyright (c) 2015-2016 Valve Corporation
 * Copyright (c) 2015-2016 LunarG, Inc.
 * Copyright (c) 2015-2016 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Lenny Komow <lenny@lunarg.com>
 * Author: Shannon McPherson <shannon@lunarg.com>
 * Author: Charles Giessen <charles@lunarg.com>
 */

/*
 * This file is generated from the Khronos Vulkan XML API Registry.
 */

#include "api_dump_text.h"
#include "api_dump_html.h"
#include "api_dump_json.h"

//============================= Dump Functions ==============================//

inline void dump_head_vkCreatePipelineCache(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreatePipelineCache(dump_inst, device, pCreateInfo, pAllocator, pPipelineCache);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreatePipelineCache(dump_inst, device, pCreateInfo, pAllocator, pPipelineCache);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreatePipelineCache(dump_inst, device, pCreateInfo, pAllocator, pPipelineCache);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdBindTransformFeedbackBuffersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdBindTransformFeedbackBuffersEXT(dump_inst, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdBindTransformFeedbackBuffersEXT(dump_inst, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdBindTransformFeedbackBuffersEXT(dump_inst, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateEvent(ApiDumpInstance& dump_inst, VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateEvent(dump_inst, device, pCreateInfo, pAllocator, pEvent);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateEvent(dump_inst, device, pCreateInfo, pAllocator, pEvent);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateEvent(dump_inst, device, pCreateInfo, pAllocator, pEvent);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetSwapchainImagesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetSwapchainImagesKHR(dump_inst, device, swapchain, pSwapchainImageCount, pSwapchainImages);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetSwapchainImagesKHR(dump_inst, device, swapchain, pSwapchainImageCount, pSwapchainImages);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetSwapchainImagesKHR(dump_inst, device, swapchain, pSwapchainImageCount, pSwapchainImages);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdBeginTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdBeginTransformFeedbackEXT(dump_inst, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdBeginTransformFeedbackEXT(dump_inst, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdBeginTransformFeedbackEXT(dump_inst, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetEventStatus(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetEventStatus(dump_inst, device, event);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetEventStatus(dump_inst, device, event);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetEventStatus(dump_inst, device, event);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateImage(ApiDumpInstance& dump_inst, VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateImage(dump_inst, device, pCreateInfo, pAllocator, pImage);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateImage(dump_inst, device, pCreateInfo, pAllocator, pImage);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateImage(dump_inst, device, pCreateInfo, pAllocator, pImage);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPipelineCacheData(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPipelineCacheData(dump_inst, device, pipelineCache, pDataSize, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPipelineCacheData(dump_inst, device, pipelineCache, pDataSize, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPipelineCacheData(dump_inst, device, pipelineCache, pDataSize, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdEndTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdEndTransformFeedbackEXT(dump_inst, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdEndTransformFeedbackEXT(dump_inst, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdEndTransformFeedbackEXT(dump_inst, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkSetEvent(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkSetEvent(dump_inst, device, event);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkSetEvent(dump_inst, device, event);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkSetEvent(dump_inst, device, event);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyBufferView(ApiDumpInstance& dump_inst, VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyBufferView(dump_inst, device, bufferView, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyBufferView(dump_inst, device, bufferView, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyBufferView(dump_inst, device, bufferView, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyEvent(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyEvent(dump_inst, device, event, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyEvent(dump_inst, device, event, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyEvent(dump_inst, device, event, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyPipelineCache(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyPipelineCache(dump_inst, device, pipelineCache, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyPipelineCache(dump_inst, device, pipelineCache, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyPipelineCache(dump_inst, device, pipelineCache, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdBeginQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdBeginQueryIndexedEXT(dump_inst, commandBuffer, queryPool, query, flags, index);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdBeginQueryIndexedEXT(dump_inst, commandBuffer, queryPool, query, flags, index);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdBeginQueryIndexedEXT(dump_inst, commandBuffer, queryPool, query, flags, index);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetImageSubresourceLayout(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetImageSubresourceLayout(dump_inst, device, image, pSubresource, pLayout);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetImageSubresourceLayout(dump_inst, device, image, pSubresource, pLayout);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetImageSubresourceLayout(dump_inst, device, image, pSubresource, pLayout);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkMergePipelineCaches(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkMergePipelineCaches(dump_inst, device, dstCache, srcCacheCount, pSrcCaches);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkMergePipelineCaches(dump_inst, device, dstCache, srcCacheCount, pSrcCaches);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkMergePipelineCaches(dump_inst, device, dstCache, srcCacheCount, pSrcCaches);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkResetEvent(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkResetEvent(dump_inst, device, event);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkResetEvent(dump_inst, device, event);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkResetEvent(dump_inst, device, event);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateGraphicsPipelines(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateGraphicsPipelines(dump_inst, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateGraphicsPipelines(dump_inst, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateGraphicsPipelines(dump_inst, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetMemoryHostPointerPropertiesEXT(ApiDumpInstance& dump_inst, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetMemoryHostPointerPropertiesEXT(dump_inst, device, handleType, pHostPointer, pMemoryHostPointerProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetMemoryHostPointerPropertiesEXT(dump_inst, device, handleType, pHostPointer, pMemoryHostPointerProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetMemoryHostPointerPropertiesEXT(dump_inst, device, handleType, pHostPointer, pMemoryHostPointerProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdEndQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdEndQueryIndexedEXT(dump_inst, commandBuffer, queryPool, query, index);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdEndQueryIndexedEXT(dump_inst, commandBuffer, queryPool, query, index);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdEndQueryIndexedEXT(dump_inst, commandBuffer, queryPool, query, index);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDrawIndirectByteCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDrawIndirectByteCountEXT(dump_inst, commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDrawIndirectByteCountEXT(dump_inst, commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDrawIndirectByteCountEXT(dump_inst, commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateQueryPool(ApiDumpInstance& dump_inst, VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateQueryPool(dump_inst, device, pCreateInfo, pAllocator, pQueryPool);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateQueryPool(dump_inst, device, pCreateInfo, pAllocator, pQueryPool);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateQueryPool(dump_inst, device, pCreateInfo, pAllocator, pQueryPool);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdWriteBufferMarkerAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdWriteBufferMarkerAMD(dump_inst, commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdWriteBufferMarkerAMD(dump_inst, commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdWriteBufferMarkerAMD(dump_inst, commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroySwapchainKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroySwapchainKHR(dump_inst, device, swapchain, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroySwapchainKHR(dump_inst, device, swapchain, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroySwapchainKHR(dump_inst, device, swapchain, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkAcquireNextImageKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkAcquireNextImageKHR(dump_inst, device, swapchain, timeout, semaphore, fence, pImageIndex);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkAcquireNextImageKHR(dump_inst, device, swapchain, timeout, semaphore, fence, pImageIndex);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkAcquireNextImageKHR(dump_inst, device, swapchain, timeout, semaphore, fence, pImageIndex);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkQueuePresentKHR(ApiDumpInstance& dump_inst, VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkQueuePresentKHR(dump_inst, queue, pPresentInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkQueuePresentKHR(dump_inst, queue, pPresentInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkQueuePresentKHR(dump_inst, queue, pPresentInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateComputePipelines(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateComputePipelines(dump_inst, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateComputePipelines(dump_inst, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateComputePipelines(dump_inst, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetDisplayPlaneSupportedDisplaysKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetDisplayPlaneSupportedDisplaysKHR(dump_inst, physicalDevice, planeIndex, pDisplayCount, pDisplays);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetDisplayPlaneSupportedDisplaysKHR(dump_inst, physicalDevice, planeIndex, pDisplayCount, pDisplays);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetDisplayPlaneSupportedDisplaysKHR(dump_inst, physicalDevice, planeIndex, pDisplayCount, pDisplays);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyImage(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyImage(dump_inst, device, image, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyImage(dump_inst, device, image, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyImage(dump_inst, device, image, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetCalibratedTimestampsEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetCalibratedTimestampsEXT(dump_inst, device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetCalibratedTimestampsEXT(dump_inst, device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetCalibratedTimestampsEXT(dump_inst, device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateImageView(ApiDumpInstance& dump_inst, VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateImageView(dump_inst, device, pCreateInfo, pAllocator, pView);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateImageView(dump_inst, device, pCreateInfo, pAllocator, pView);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateImageView(dump_inst, device, pCreateInfo, pAllocator, pView);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceDisplayPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceDisplayPropertiesKHR(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceDisplayPropertiesKHR(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceDisplayPropertiesKHR(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyQueryPool(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyQueryPool(dump_inst, device, queryPool, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyQueryPool(dump_inst, device, queryPool, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyQueryPool(dump_inst, device, queryPool, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(dump_inst, physicalDevice, pTimeDomainCount, pTimeDomains);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(dump_inst, physicalDevice, pTimeDomainCount, pTimeDomains);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(dump_inst, physicalDevice, pTimeDomainCount, pTimeDomains);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdBlitImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdBlitImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdBlitImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdBlitImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetDisplayModePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetDisplayModePropertiesKHR(dump_inst, physicalDevice, display, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetDisplayModePropertiesKHR(dump_inst, physicalDevice, display, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetDisplayModePropertiesKHR(dump_inst, physicalDevice, display, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceProperties(dump_inst, physicalDevice, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceProperties(dump_inst, physicalDevice, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceProperties(dump_inst, physicalDevice, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateDisplayModeKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateDisplayModeKHR(dump_inst, physicalDevice, display, pCreateInfo, pAllocator, pMode);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateDisplayModeKHR(dump_inst, physicalDevice, display, pCreateInfo, pAllocator, pMode);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateDisplayModeKHR(dump_inst, physicalDevice, display, pCreateInfo, pAllocator, pMode);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreatePipelineLayout(ApiDumpInstance& dump_inst, VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreatePipelineLayout(dump_inst, device, pCreateInfo, pAllocator, pPipelineLayout);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreatePipelineLayout(dump_inst, device, pCreateInfo, pAllocator, pPipelineLayout);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreatePipelineLayout(dump_inst, device, pCreateInfo, pAllocator, pPipelineLayout);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdBuildAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdBuildAccelerationStructureNV(dump_inst, commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdBuildAccelerationStructureNV(dump_inst, commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdBuildAccelerationStructureNV(dump_inst, commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetDisplayPlaneCapabilitiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetDisplayPlaneCapabilitiesKHR(dump_inst, physicalDevice, mode, planeIndex, pCapabilities);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetDisplayPlaneCapabilitiesKHR(dump_inst, physicalDevice, mode, planeIndex, pCapabilities);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetDisplayPlaneCapabilitiesKHR(dump_inst, physicalDevice, mode, planeIndex, pCapabilities);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetAccelerationStructureMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetAccelerationStructureMemoryRequirementsNV(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetAccelerationStructureMemoryRequirementsNV(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetAccelerationStructureMemoryRequirementsNV(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyAccelerationStructureNV(dump_inst, device, accelerationStructure, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyAccelerationStructureNV(dump_inst, device, accelerationStructure, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyAccelerationStructureNV(dump_inst, device, accelerationStructure, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(dump_inst, physicalDevice, pCombinationCount, pCombinations);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(dump_inst, physicalDevice, pCombinationCount, pCombinations);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(dump_inst, physicalDevice, pCombinationCount, pCombinations);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkBindAccelerationStructureMemoryNV(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkBindAccelerationStructureMemoryNV(dump_inst, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkBindAccelerationStructureMemoryNV(dump_inst, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkBindAccelerationStructureMemoryNV(dump_inst, device, bindInfoCount, pBindInfos);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateDisplayPlaneSurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateDisplayPlaneSurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateDisplayPlaneSurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateDisplayPlaneSurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkResetCommandBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkResetCommandBuffer(dump_inst, commandBuffer, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkResetCommandBuffer(dump_inst, commandBuffer, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkResetCommandBuffer(dump_inst, commandBuffer, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdCopyBufferToImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdCopyBufferToImage(dump_inst, commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdCopyBufferToImage(dump_inst, commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdCopyBufferToImage(dump_inst, commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetViewport(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetViewport(dump_inst, commandBuffer, firstViewport, viewportCount, pViewports);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetViewport(dump_inst, commandBuffer, firstViewport, viewportCount, pViewports);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetViewport(dump_inst, commandBuffer, firstViewport, viewportCount, pViewports);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyPipeline(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyPipeline(dump_inst, device, pipeline, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyPipeline(dump_inst, device, pipeline, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyPipeline(dump_inst, device, pipeline, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdCopyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeNV mode)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdCopyAccelerationStructureNV(dump_inst, commandBuffer, dst, src, mode);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdCopyAccelerationStructureNV(dump_inst, commandBuffer, dst, src, mode);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdCopyAccelerationStructureNV(dump_inst, commandBuffer, dst, src, mode);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdBindPipeline(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdBindPipeline(dump_inst, commandBuffer, pipelineBindPoint, pipeline);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdBindPipeline(dump_inst, commandBuffer, pipelineBindPoint, pipeline);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdBindPipeline(dump_inst, commandBuffer, pipelineBindPoint, pipeline);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetSampleLocationsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetSampleLocationsEXT(dump_inst, commandBuffer, pSampleLocationsInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetSampleLocationsEXT(dump_inst, commandBuffer, pSampleLocationsInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetSampleLocationsEXT(dump_inst, commandBuffer, pSampleLocationsInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetImageViewHandleNVX(ApiDumpInstance& dump_inst, VkDevice device, const VkImageViewHandleInfoNVX* pInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetImageViewHandleNVX(dump_inst, device, pInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetImageViewHandleNVX(dump_inst, device, pInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetImageViewHandleNVX(dump_inst, device, pInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetImageSparseMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetImageSparseMemoryRequirements2KHR(dump_inst, device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetImageSparseMemoryRequirements2KHR(dump_inst, device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetImageSparseMemoryRequirements2KHR(dump_inst, device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetLineWidth(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float lineWidth)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetLineWidth(dump_inst, commandBuffer, lineWidth);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetLineWidth(dump_inst, commandBuffer, lineWidth);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetLineWidth(dump_inst, commandBuffer, lineWidth);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkInitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkInitializePerformanceApiINTEL(dump_inst, device, pInitializeInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkInitializePerformanceApiINTEL(dump_inst, device, pInitializeInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkInitializePerformanceApiINTEL(dump_inst, device, pInitializeInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateSampler(ApiDumpInstance& dump_inst, VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateSampler(dump_inst, device, pCreateInfo, pAllocator, pSampler);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateSampler(dump_inst, device, pCreateInfo, pAllocator, pSampler);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateSampler(dump_inst, device, pCreateInfo, pAllocator, pSampler);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetScissor(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetScissor(dump_inst, commandBuffer, firstScissor, scissorCount, pScissors);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetScissor(dump_inst, commandBuffer, firstScissor, scissorCount, pScissors);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetScissor(dump_inst, commandBuffer, firstScissor, scissorCount, pScissors);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkUninitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkDevice device)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkUninitializePerformanceApiINTEL(dump_inst, device);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkUninitializePerformanceApiINTEL(dump_inst, device);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkUninitializePerformanceApiINTEL(dump_inst, device);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetDepthBias(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetDepthBias(dump_inst, commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetDepthBias(dump_inst, commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetDepthBias(dump_inst, commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceMultisamplePropertiesEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceMultisamplePropertiesEXT(dump_inst, physicalDevice, samples, pMultisampleProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceMultisamplePropertiesEXT(dump_inst, physicalDevice, samples, pMultisampleProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceMultisamplePropertiesEXT(dump_inst, physicalDevice, samples, pMultisampleProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdTraceRaysNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdTraceRaysNV(dump_inst, commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdTraceRaysNV(dump_inst, commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdTraceRaysNV(dump_inst, commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetPerformanceMarkerINTEL(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetPerformanceMarkerINTEL(dump_inst, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetPerformanceMarkerINTEL(dump_inst, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetPerformanceMarkerINTEL(dump_inst, commandBuffer, pMarkerInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDrawIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDrawIndirectCountAMD(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDrawIndirectCountAMD(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDrawIndirectCountAMD(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDrawIndexedIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDrawIndexedIndirectCountAMD(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDrawIndexedIndirectCountAMD(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDrawIndexedIndirectCountAMD(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetImageMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetImageMemoryRequirements2KHR(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetImageMemoryRequirements2KHR(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetImageMemoryRequirements2KHR(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetPerformanceStreamMarkerINTEL(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetPerformanceStreamMarkerINTEL(dump_inst, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetPerformanceStreamMarkerINTEL(dump_inst, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetPerformanceStreamMarkerINTEL(dump_inst, commandBuffer, pMarkerInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateRayTracingPipelinesNV(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateRayTracingPipelinesNV(dump_inst, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateRayTracingPipelinesNV(dump_inst, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateRayTracingPipelinesNV(dump_inst, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetBufferMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetBufferMemoryRequirements2KHR(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetBufferMemoryRequirements2KHR(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetBufferMemoryRequirements2KHR(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetPerformanceOverrideINTEL(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetPerformanceOverrideINTEL(dump_inst, commandBuffer, pOverrideInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetPerformanceOverrideINTEL(dump_inst, commandBuffer, pOverrideInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetPerformanceOverrideINTEL(dump_inst, commandBuffer, pOverrideInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdCopyImageToBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdCopyImageToBuffer(dump_inst, commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdCopyImageToBuffer(dump_inst, commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdCopyImageToBuffer(dump_inst, commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetBlendConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const float blendConstants[4])
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetBlendConstants(dump_inst, commandBuffer, blendConstants);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetBlendConstants(dump_inst, commandBuffer, blendConstants);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetBlendConstants(dump_inst, commandBuffer, blendConstants);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkReleasePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkDevice device, VkPerformanceConfigurationINTEL configuration)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkReleasePerformanceConfigurationINTEL(dump_inst, device, configuration);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkReleasePerformanceConfigurationINTEL(dump_inst, device, configuration);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkReleasePerformanceConfigurationINTEL(dump_inst, device, configuration);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetStencilWriteMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetStencilWriteMask(dump_inst, commandBuffer, faceMask, writeMask);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetStencilWriteMask(dump_inst, commandBuffer, faceMask, writeMask);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetStencilWriteMask(dump_inst, commandBuffer, faceMask, writeMask);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_head_vkGetPhysicalDeviceSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceSurfacePresentModes2EXT(dump_inst, physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceSurfacePresentModes2EXT(dump_inst, physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceSurfacePresentModes2EXT(dump_inst, physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_head_vkAcquirePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkAcquirePerformanceConfigurationINTEL(dump_inst, device, pAcquireInfo, pConfiguration);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkAcquirePerformanceConfigurationINTEL(dump_inst, device, pAcquireInfo, pConfiguration);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkAcquirePerformanceConfigurationINTEL(dump_inst, device, pAcquireInfo, pConfiguration);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyPipelineLayout(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyPipelineLayout(dump_inst, device, pipelineLayout, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyPipelineLayout(dump_inst, device, pipelineLayout, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyPipelineLayout(dump_inst, device, pipelineLayout, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetBufferDeviceAddressEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferDeviceAddressInfoEXT* pInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetBufferDeviceAddressEXT(dump_inst, device, pInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetBufferDeviceAddressEXT(dump_inst, device, pInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetBufferDeviceAddressEXT(dump_inst, device, pInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateSharedSwapchainsKHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateSharedSwapchainsKHR(dump_inst, device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateSharedSwapchainsKHR(dump_inst, device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateSharedSwapchainsKHR(dump_inst, device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkQueueSetPerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkQueue queue, VkPerformanceConfigurationINTEL configuration)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkQueueSetPerformanceConfigurationINTEL(dump_inst, queue, configuration);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkQueueSetPerformanceConfigurationINTEL(dump_inst, queue, configuration);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkQueueSetPerformanceConfigurationINTEL(dump_inst, queue, configuration);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_head_vkReleaseFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkReleaseFullScreenExclusiveModeEXT(dump_inst, device, swapchain);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkReleaseFullScreenExclusiveModeEXT(dump_inst, device, swapchain);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkReleaseFullScreenExclusiveModeEXT(dump_inst, device, swapchain);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_head_vkAcquireFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkAcquireFullScreenExclusiveModeEXT(dump_inst, device, swapchain);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkAcquireFullScreenExclusiveModeEXT(dump_inst, device, swapchain);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkAcquireFullScreenExclusiveModeEXT(dump_inst, device, swapchain);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_head_vkCmdSetDepthBounds(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetDepthBounds(dump_inst, commandBuffer, minDepthBounds, maxDepthBounds);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetDepthBounds(dump_inst, commandBuffer, minDepthBounds, maxDepthBounds);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetDepthBounds(dump_inst, commandBuffer, minDepthBounds, maxDepthBounds);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetRayTracingShaderGroupHandlesNV(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetRayTracingShaderGroupHandlesNV(dump_inst, device, pipeline, firstGroup, groupCount, dataSize, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetRayTracingShaderGroupHandlesNV(dump_inst, device, pipeline, firstGroup, groupCount, dataSize, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetRayTracingShaderGroupHandlesNV(dump_inst, device, pipeline, firstGroup, groupCount, dataSize, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPerformanceParameterINTEL(ApiDumpInstance& dump_inst, VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPerformanceParameterINTEL(dump_inst, device, parameter, pValue);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPerformanceParameterINTEL(dump_inst, device, parameter, pValue);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPerformanceParameterINTEL(dump_inst, device, parameter, pValue);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyRenderPass(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyRenderPass(dump_inst, device, renderPass, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyRenderPass(dump_inst, device, renderPass, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyRenderPass(dump_inst, device, renderPass, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdUpdateBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdUpdateBuffer(dump_inst, commandBuffer, dstBuffer, dstOffset, dataSize, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdUpdateBuffer(dump_inst, commandBuffer, dstBuffer, dstOffset, dataSize, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdUpdateBuffer(dump_inst, commandBuffer, dstBuffer, dstOffset, dataSize, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetStencilCompareMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetStencilCompareMask(dump_inst, commandBuffer, faceMask, compareMask);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetStencilCompareMask(dump_inst, commandBuffer, faceMask, compareMask);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetStencilCompareMask(dump_inst, commandBuffer, faceMask, compareMask);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetAccelerationStructureHandleNV(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetAccelerationStructureHandleNV(dump_inst, device, accelerationStructure, dataSize, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetAccelerationStructureHandleNV(dump_inst, device, accelerationStructure, dataSize, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetAccelerationStructureHandleNV(dump_inst, device, accelerationStructure, dataSize, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_head_vkGetDeviceGroupSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetDeviceGroupSurfacePresentModes2EXT(dump_inst, device, pSurfaceInfo, pModes);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetDeviceGroupSurfacePresentModes2EXT(dump_inst, device, pSurfaceInfo, pModes);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetDeviceGroupSurfacePresentModes2EXT(dump_inst, device, pSurfaceInfo, pModes);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_head_vkCmdFillBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdFillBuffer(dump_inst, commandBuffer, dstBuffer, dstOffset, size, data);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdFillBuffer(dump_inst, commandBuffer, dstBuffer, dstOffset, size, data);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdFillBuffer(dump_inst, commandBuffer, dstBuffer, dstOffset, size, data);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(dump_inst, physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(dump_inst, physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(dump_inst, physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceMemoryProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceMemoryProperties(dump_inst, physicalDevice, pMemoryProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceMemoryProperties(dump_inst, physicalDevice, pMemoryProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceMemoryProperties(dump_inst, physicalDevice, pMemoryProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
inline void dump_head_vkCreateWaylandSurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateWaylandSurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateWaylandSurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateWaylandSurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WAYLAND_KHR
inline void dump_head_vkGetPhysicalDeviceQueueFamilyProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceQueueFamilyProperties(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceQueueFamilyProperties(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceQueueFamilyProperties(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_head_vkGetMemoryWin32HandleNV(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetMemoryWin32HandleNV(dump_inst, device, memory, handleType, pHandle);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetMemoryWin32HandleNV(dump_inst, device, memory, handleType, pHandle);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetMemoryWin32HandleNV(dump_inst, device, memory, handleType, pHandle);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_head_vkCmdProcessCommandsNVX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCmdProcessCommandsInfoNVX* pProcessCommandsInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdProcessCommandsNVX(dump_inst, commandBuffer, pProcessCommandsInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdProcessCommandsNVX(dump_inst, commandBuffer, pProcessCommandsInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdProcessCommandsNVX(dump_inst, commandBuffer, pProcessCommandsInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
inline void dump_head_vkGetPhysicalDeviceWaylandPresentationSupportKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceWaylandPresentationSupportKHR(dump_inst, physicalDevice, queueFamilyIndex, display);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceWaylandPresentationSupportKHR(dump_inst, physicalDevice, queueFamilyIndex, display);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceWaylandPresentationSupportKHR(dump_inst, physicalDevice, queueFamilyIndex, display);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WAYLAND_KHR
inline void dump_head_vkTrimCommandPoolKHR(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkTrimCommandPoolKHR(dump_inst, device, commandPool, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkTrimCommandPoolKHR(dump_inst, device, commandPool, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkTrimCommandPoolKHR(dump_inst, device, commandPool, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDebugMarkerSetObjectTagEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDebugMarkerSetObjectTagEXT(dump_inst, device, pTagInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDebugMarkerSetObjectTagEXT(dump_inst, device, pTagInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDebugMarkerSetObjectTagEXT(dump_inst, device, pTagInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkEnumeratePhysicalDeviceGroupsKHR(ApiDumpInstance& dump_inst, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkEnumeratePhysicalDeviceGroupsKHR(dump_inst, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkEnumeratePhysicalDeviceGroupsKHR(dump_inst, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkEnumeratePhysicalDeviceGroupsKHR(dump_inst, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdReserveSpaceForCommandsNVX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCmdReserveSpaceForCommandsInfoNVX* pReserveSpaceInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdReserveSpaceForCommandsNVX(dump_inst, commandBuffer, pReserveSpaceInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdReserveSpaceForCommandsNVX(dump_inst, commandBuffer, pReserveSpaceInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdReserveSpaceForCommandsNVX(dump_inst, commandBuffer, pReserveSpaceInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceExternalBufferPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceExternalBufferPropertiesKHR(dump_inst, physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceExternalBufferPropertiesKHR(dump_inst, physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceExternalBufferPropertiesKHR(dump_inst, physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateIndirectCommandsLayoutNVX(ApiDumpInstance& dump_inst, VkDevice device, const VkIndirectCommandsLayoutCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNVX* pIndirectCommandsLayout)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateIndirectCommandsLayoutNVX(dump_inst, device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateIndirectCommandsLayoutNVX(dump_inst, device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateIndirectCommandsLayoutNVX(dump_inst, device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateObjectTableNVX(ApiDumpInstance& dump_inst, VkDevice device, const VkObjectTableCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkObjectTableNVX* pObjectTable)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateObjectTableNVX(dump_inst, device, pCreateInfo, pAllocator, pObjectTable);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateObjectTableNVX(dump_inst, device, pCreateInfo, pAllocator, pObjectTable);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateObjectTableNVX(dump_inst, device, pCreateInfo, pAllocator, pObjectTable);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkRegisterObjectsNVX(ApiDumpInstance& dump_inst, VkDevice device, VkObjectTableNVX objectTable, uint32_t objectCount, const VkObjectTableEntryNVX* const*    ppObjectTableEntries, const uint32_t* pObjectIndices)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkRegisterObjectsNVX(dump_inst, device, objectTable, objectCount, ppObjectTableEntries, pObjectIndices);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkRegisterObjectsNVX(dump_inst, device, objectTable, objectCount, ppObjectTableEntries, pObjectIndices);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkRegisterObjectsNVX(dump_inst, device, objectTable, objectCount, ppObjectTableEntries, pObjectIndices);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyIndirectCommandsLayoutNVX(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectCommandsLayoutNVX indirectCommandsLayout, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyIndirectCommandsLayoutNVX(dump_inst, device, indirectCommandsLayout, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyIndirectCommandsLayoutNVX(dump_inst, device, indirectCommandsLayout, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyIndirectCommandsLayoutNVX(dump_inst, device, indirectCommandsLayout, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_XCB_KHR)
inline void dump_head_vkCreateXcbSurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateXcbSurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateXcbSurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateXcbSurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_XCB_KHR
inline void dump_head_vkUnregisterObjectsNVX(ApiDumpInstance& dump_inst, VkDevice device, VkObjectTableNVX objectTable, uint32_t objectCount, const VkObjectEntryTypeNVX* pObjectEntryTypes, const uint32_t* pObjectIndices)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkUnregisterObjectsNVX(dump_inst, device, objectTable, objectCount, pObjectEntryTypes, pObjectIndices);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkUnregisterObjectsNVX(dump_inst, device, objectTable, objectCount, pObjectEntryTypes, pObjectIndices);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkUnregisterObjectsNVX(dump_inst, device, objectTable, objectCount, pObjectEntryTypes, pObjectIndices);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyObjectTableNVX(ApiDumpInstance& dump_inst, VkDevice device, VkObjectTableNVX objectTable, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyObjectTableNVX(dump_inst, device, objectTable, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyObjectTableNVX(dump_inst, device, objectTable, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyObjectTableNVX(dump_inst, device, objectTable, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDebugMarkerBeginEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDebugMarkerBeginEXT(dump_inst, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDebugMarkerBeginEXT(dump_inst, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDebugMarkerBeginEXT(dump_inst, commandBuffer, pMarkerInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateAccelerationStructureNV(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateAccelerationStructureNV(dump_inst, device, pCreateInfo, pAllocator, pAccelerationStructure);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateAccelerationStructureNV(dump_inst, device, pCreateInfo, pAllocator, pAccelerationStructure);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateAccelerationStructureNV(dump_inst, device, pCreateInfo, pAllocator, pAccelerationStructure);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPipelineExecutablePropertiesKHR(ApiDumpInstance& dump_inst, VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPipelineExecutablePropertiesKHR(dump_inst, device, pPipelineInfo, pExecutableCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPipelineExecutablePropertiesKHR(dump_inst, device, pPipelineInfo, pExecutableCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPipelineExecutablePropertiesKHR(dump_inst, device, pPipelineInfo, pExecutableCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDebugMarkerInsertEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDebugMarkerInsertEXT(dump_inst, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDebugMarkerInsertEXT(dump_inst, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDebugMarkerInsertEXT(dump_inst, commandBuffer, pMarkerInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDebugMarkerEndEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDebugMarkerEndEXT(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDebugMarkerEndEXT(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDebugMarkerEndEXT(dump_inst, commandBuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkBindBufferMemory2(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkBindBufferMemory2(dump_inst, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkBindBufferMemory2(dump_inst, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkBindBufferMemory2(dump_inst, device, bindInfoCount, pBindInfos);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdWriteAccelerationStructuresPropertiesNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdWriteAccelerationStructuresPropertiesNV(dump_inst, commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdWriteAccelerationStructuresPropertiesNV(dump_inst, commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdWriteAccelerationStructuresPropertiesNV(dump_inst, commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkQueueInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkQueueInsertDebugUtilsLabelEXT(dump_inst, queue, pLabelInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkQueueInsertDebugUtilsLabelEXT(dump_inst, queue, pLabelInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkQueueInsertDebugUtilsLabelEXT(dump_inst, queue, pLabelInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdBeginDebugUtilsLabelEXT(dump_inst, commandBuffer, pLabelInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdBeginDebugUtilsLabelEXT(dump_inst, commandBuffer, pLabelInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdBeginDebugUtilsLabelEXT(dump_inst, commandBuffer, pLabelInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateHeadlessSurfaceEXT(ApiDumpInstance& dump_inst, VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateHeadlessSurfaceEXT(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateHeadlessSurfaceEXT(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateHeadlessSurfaceEXT(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetCoarseSampleOrderNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetCoarseSampleOrderNV(dump_inst, commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetCoarseSampleOrderNV(dump_inst, commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetCoarseSampleOrderNV(dump_inst, commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetDisplayModeProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetDisplayModeProperties2KHR(dump_inst, physicalDevice, display, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetDisplayModeProperties2KHR(dump_inst, physicalDevice, display, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetDisplayModeProperties2KHR(dump_inst, physicalDevice, display, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdEndDebugUtilsLabelEXT(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdEndDebugUtilsLabelEXT(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdEndDebugUtilsLabelEXT(dump_inst, commandBuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDeviceGeneratedCommandsFeaturesNVX* pFeatures, VkDeviceGeneratedCommandsLimitsNVX* pLimits)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(dump_inst, physicalDevice, pFeatures, pLimits);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(dump_inst, physicalDevice, pFeatures, pLimits);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(dump_inst, physicalDevice, pFeatures, pLimits);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdInsertDebugUtilsLabelEXT(dump_inst, commandBuffer, pLabelInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdInsertDebugUtilsLabelEXT(dump_inst, commandBuffer, pLabelInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdInsertDebugUtilsLabelEXT(dump_inst, commandBuffer, pLabelInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetShaderInfoAMD(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetShaderInfoAMD(dump_inst, device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetShaderInfoAMD(dump_inst, device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetShaderInfoAMD(dump_inst, device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateDebugUtilsMessengerEXT(dump_inst, instance, pCreateInfo, pAllocator, pMessenger);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateDebugUtilsMessengerEXT(dump_inst, instance, pCreateInfo, pAllocator, pMessenger);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateDebugUtilsMessengerEXT(dump_inst, instance, pCreateInfo, pAllocator, pMessenger);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkSubmitDebugUtilsMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkSubmitDebugUtilsMessageEXT(dump_inst, instance, messageSeverity, messageTypes, pCallbackData);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkSubmitDebugUtilsMessageEXT(dump_inst, instance, messageSeverity, messageTypes, pCallbackData);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkSubmitDebugUtilsMessageEXT(dump_inst, instance, messageSeverity, messageTypes, pCallbackData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetViewportWScalingNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetViewportWScalingNV(dump_inst, commandBuffer, firstViewport, viewportCount, pViewportWScalings);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetViewportWScalingNV(dump_inst, commandBuffer, firstViewport, viewportCount, pViewportWScalings);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetViewportWScalingNV(dump_inst, commandBuffer, firstViewport, viewportCount, pViewportWScalings);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCompileDeferredNV(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, uint32_t shader)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCompileDeferredNV(dump_inst, device, pipeline, shader);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCompileDeferredNV(dump_inst, device, pipeline, shader);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCompileDeferredNV(dump_inst, device, pipeline, shader);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkSetLocalDimmingAMD(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkSetLocalDimmingAMD(dump_inst, device, swapChain, localDimmingEnable);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkSetLocalDimmingAMD(dump_inst, device, swapChain, localDimmingEnable);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkSetLocalDimmingAMD(dump_inst, device, swapChain, localDimmingEnable);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkReleaseDisplayEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkReleaseDisplayEXT(dump_inst, physicalDevice, display);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkReleaseDisplayEXT(dump_inst, physicalDevice, display);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkReleaseDisplayEXT(dump_inst, physicalDevice, display);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceDisplayProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceDisplayProperties2KHR(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceDisplayProperties2KHR(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceDisplayProperties2KHR(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyDebugUtilsMessengerEXT(dump_inst, instance, messenger, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyDebugUtilsMessengerEXT(dump_inst, instance, messenger, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyDebugUtilsMessengerEXT(dump_inst, instance, messenger, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceExternalSemaphoreProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceExternalSemaphoreProperties(dump_inst, physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceExternalSemaphoreProperties(dump_inst, physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceExternalSemaphoreProperties(dump_inst, physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_FUCHSIA)
inline void dump_head_vkCreateImagePipeSurfaceFUCHSIA(ApiDumpInstance& dump_inst, VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateImagePipeSurfaceFUCHSIA(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateImagePipeSurfaceFUCHSIA(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateImagePipeSurfaceFUCHSIA(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_FUCHSIA
inline void dump_head_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdBeginConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdBeginConditionalRenderingEXT(dump_inst, commandBuffer, pConditionalRenderingBegin);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdBeginConditionalRenderingEXT(dump_inst, commandBuffer, pConditionalRenderingBegin);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdBeginConditionalRenderingEXT(dump_inst, commandBuffer, pConditionalRenderingBegin);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkRegisterDeviceEventEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkRegisterDeviceEventEXT(dump_inst, device, pDeviceEventInfo, pAllocator, pFence);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkRegisterDeviceEventEXT(dump_inst, device, pDeviceEventInfo, pAllocator, pFence);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkRegisterDeviceEventEXT(dump_inst, device, pDeviceEventInfo, pAllocator, pFence);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDrawIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDrawIndirectCountKHR(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDrawIndirectCountKHR(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDrawIndirectCountKHR(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdEndConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdEndConditionalRenderingEXT(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdEndConditionalRenderingEXT(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdEndConditionalRenderingEXT(dump_inst, commandBuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
inline void dump_head_vkCreateAndroidSurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateAndroidSurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateAndroidSurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateAndroidSurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_ANDROID_KHR
inline void dump_head_vkGetImageDrmFormatModifierPropertiesEXT(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetImageDrmFormatModifierPropertiesEXT(dump_inst, device, image, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetImageDrmFormatModifierPropertiesEXT(dump_inst, device, image, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetImageDrmFormatModifierPropertiesEXT(dump_inst, device, image, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_XLIB_KHR)
inline void dump_head_vkCreateXlibSurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateXlibSurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateXlibSurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateXlibSurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_XLIB_KHR
inline void dump_head_vkDisplayPowerControlEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDisplayPowerControlEXT(dump_inst, device, display, pDisplayPowerInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDisplayPowerControlEXT(dump_inst, device, display, pDisplayPowerInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDisplayPowerControlEXT(dump_inst, device, display, pDisplayPowerInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateDescriptorSetLayout(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateDescriptorSetLayout(dump_inst, device, pCreateInfo, pAllocator, pSetLayout);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateDescriptorSetLayout(dump_inst, device, pCreateInfo, pAllocator, pSetLayout);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateDescriptorSetLayout(dump_inst, device, pCreateInfo, pAllocator, pSetLayout);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateDescriptorUpdateTemplateKHR(dump_inst, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateDescriptorUpdateTemplateKHR(dump_inst, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateDescriptorUpdateTemplateKHR(dump_inst, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetDescriptorSetLayoutSupportKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetDescriptorSetLayoutSupportKHR(dump_inst, device, pCreateInfo, pSupport);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetDescriptorSetLayoutSupportKHR(dump_inst, device, pCreateInfo, pSupport);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetDescriptorSetLayoutSupportKHR(dump_inst, device, pCreateInfo, pSupport);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetDisplayPlaneCapabilities2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetDisplayPlaneCapabilities2KHR(dump_inst, physicalDevice, pDisplayPlaneInfo, pCapabilities);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetDisplayPlaneCapabilities2KHR(dump_inst, physicalDevice, pDisplayPlaneInfo, pCapabilities);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetDisplayPlaneCapabilities2KHR(dump_inst, physicalDevice, pDisplayPlaneInfo, pCapabilities);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkRegisterDisplayEventEXT(ApiDumpInstance& dump_inst, VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkRegisterDisplayEventEXT(dump_inst, device, display, pDisplayEventInfo, pAllocator, pFence);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkRegisterDisplayEventEXT(dump_inst, device, display, pDisplayEventInfo, pAllocator, pFence);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkRegisterDisplayEventEXT(dump_inst, device, display, pDisplayEventInfo, pAllocator, pFence);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroySampler(ApiDumpInstance& dump_inst, VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroySampler(dump_inst, device, sampler, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroySampler(dump_inst, device, sampler, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroySampler(dump_inst, device, sampler, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_METAL_EXT)
inline void dump_head_vkCreateMetalSurfaceEXT(ApiDumpInstance& dump_inst, VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateMetalSurfaceEXT(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateMetalSurfaceEXT(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateMetalSurfaceEXT(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
inline void dump_head_vkGetRandROutputDisplayEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetRandROutputDisplayEXT(dump_inst, physicalDevice, dpy, rrOutput, pDisplay);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetRandROutputDisplayEXT(dump_inst, physicalDevice, dpy, rrOutput, pDisplay);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetRandROutputDisplayEXT(dump_inst, physicalDevice, dpy, rrOutput, pDisplay);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
inline void dump_head_vkCmdDrawIndexedIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDrawIndexedIndirectCountKHR(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDrawIndexedIndirectCountKHR(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDrawIndexedIndirectCountKHR(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_XLIB_KHR)
inline void dump_head_vkGetPhysicalDeviceXlibPresentationSupportKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceXlibPresentationSupportKHR(dump_inst, physicalDevice, queueFamilyIndex, dpy, visualID);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceXlibPresentationSupportKHR(dump_inst, physicalDevice, queueFamilyIndex, dpy, visualID);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceXlibPresentationSupportKHR(dump_inst, physicalDevice, queueFamilyIndex, dpy, visualID);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_XLIB_KHR
inline void dump_head_vkGetSwapchainCounterEXT(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetSwapchainCounterEXT(dump_inst, device, swapchain, counter, pCounterValue);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetSwapchainCounterEXT(dump_inst, device, swapchain, counter, pCounterValue);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetSwapchainCounterEXT(dump_inst, device, swapchain, counter, pCounterValue);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetDescriptorSetLayoutSupport(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetDescriptorSetLayoutSupport(dump_inst, device, pCreateInfo, pSupport);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetDescriptorSetLayoutSupport(dump_inst, device, pCreateInfo, pSupport);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetDescriptorSetLayoutSupport(dump_inst, device, pCreateInfo, pSupport);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_IOS_MVK)
inline void dump_head_vkCreateIOSSurfaceMVK(ApiDumpInstance& dump_inst, VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateIOSSurfaceMVK(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateIOSSurfaceMVK(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateIOSSurfaceMVK(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_IOS_MVK
inline void dump_head_vkResetQueryPoolEXT(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkResetQueryPoolEXT(dump_inst, device, queryPool, firstQuery, queryCount);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkResetQueryPoolEXT(dump_inst, device, queryPool, firstQuery, queryCount);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkResetQueryPoolEXT(dump_inst, device, queryPool, firstQuery, queryCount);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
inline void dump_head_vkAcquireXlibDisplayEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkAcquireXlibDisplayEXT(dump_inst, physicalDevice, dpy, display);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkAcquireXlibDisplayEXT(dump_inst, physicalDevice, dpy, display);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkAcquireXlibDisplayEXT(dump_inst, physicalDevice, dpy, display);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
inline void dump_head_vkCmdSetLineStippleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetLineStippleEXT(dump_inst, commandBuffer, lineStippleFactor, lineStipplePattern);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetLineStippleEXT(dump_inst, commandBuffer, lineStippleFactor, lineStipplePattern);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetLineStippleEXT(dump_inst, commandBuffer, lineStippleFactor, lineStipplePattern);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceSurfaceSupportKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceSurfaceSupportKHR(dump_inst, physicalDevice, queueFamilyIndex, surface, pSupported);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceSurfaceSupportKHR(dump_inst, physicalDevice, queueFamilyIndex, surface, pSupported);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceSurfaceSupportKHR(dump_inst, physicalDevice, queueFamilyIndex, surface, pSupported);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateSamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateSamplerYcbcrConversionKHR(dump_inst, device, pCreateInfo, pAllocator, pYcbcrConversion);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateSamplerYcbcrConversionKHR(dump_inst, device, pCreateInfo, pAllocator, pYcbcrConversion);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateSamplerYcbcrConversionKHR(dump_inst, device, pCreateInfo, pAllocator, pYcbcrConversion);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyDescriptorSetLayout(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyDescriptorSetLayout(dump_inst, device, descriptorSetLayout, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyDescriptorSetLayout(dump_inst, device, descriptorSetLayout, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyDescriptorSetLayout(dump_inst, device, descriptorSetLayout, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_GGP)
inline void dump_head_vkCreateStreamDescriptorSurfaceGGP(ApiDumpInstance& dump_inst, VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateStreamDescriptorSurfaceGGP(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateStreamDescriptorSurfaceGGP(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateStreamDescriptorSurfaceGGP(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_GGP
inline void dump_head_vkGetPastPresentationTimingGOOGLE(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPastPresentationTimingGOOGLE(dump_inst, device, swapchain, pPresentationTimingCount, pPresentationTimings);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPastPresentationTimingGOOGLE(dump_inst, device, swapchain, pPresentationTimingCount, pPresentationTimings);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPastPresentationTimingGOOGLE(dump_inst, device, swapchain, pPresentationTimingCount, pPresentationTimings);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateSwapchainKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateSwapchainKHR(dump_inst, device, pCreateInfo, pAllocator, pSwapchain);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateSwapchainKHR(dump_inst, device, pCreateInfo, pAllocator, pSwapchain);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateSwapchainKHR(dump_inst, device, pCreateInfo, pAllocator, pSwapchain);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroySurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroySurfaceKHR(dump_inst, instance, surface, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroySurfaceKHR(dump_inst, instance, surface, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroySurfaceKHR(dump_inst, instance, surface, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_MACOS_MVK)
inline void dump_head_vkCreateMacOSSurfaceMVK(ApiDumpInstance& dump_inst, VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateMacOSSurfaceMVK(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateMacOSSurfaceMVK(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateMacOSSurfaceMVK(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_MACOS_MVK
inline void dump_head_vkMergeValidationCachesEXT(ApiDumpInstance& dump_inst, VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkMergeValidationCachesEXT(dump_inst, device, dstCache, srcCacheCount, pSrcCaches);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkMergeValidationCachesEXT(dump_inst, device, dstCache, srcCacheCount, pSrcCaches);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkMergeValidationCachesEXT(dump_inst, device, dstCache, srcCacheCount, pSrcCaches);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateValidationCacheEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateValidationCacheEXT(dump_inst, device, pCreateInfo, pAllocator, pValidationCache);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateValidationCacheEXT(dump_inst, device, pCreateInfo, pAllocator, pValidationCache);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateValidationCacheEXT(dump_inst, device, pCreateInfo, pAllocator, pValidationCache);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkUpdateDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkUpdateDescriptorSetWithTemplateKHR(dump_inst, device, descriptorSet, descriptorUpdateTemplate, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkUpdateDescriptorSetWithTemplateKHR(dump_inst, device, descriptorSet, descriptorUpdateTemplate, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkUpdateDescriptorSetWithTemplateKHR(dump_inst, device, descriptorSet, descriptorUpdateTemplate, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceSurfaceFormatsKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceSurfaceFormatsKHR(dump_inst, physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceSurfaceFormatsKHR(dump_inst, physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceSurfaceFormatsKHR(dump_inst, physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceSurfaceCapabilities2EXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceSurfaceCapabilities2EXT(dump_inst, physicalDevice, surface, pSurfaceCapabilities);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceSurfaceCapabilities2EXT(dump_inst, physicalDevice, surface, pSurfaceCapabilities);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceSurfaceCapabilities2EXT(dump_inst, physicalDevice, surface, pSurfaceCapabilities);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroySamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroySamplerYcbcrConversionKHR(dump_inst, device, ycbcrConversion, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroySamplerYcbcrConversionKHR(dump_inst, device, ycbcrConversion, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroySamplerYcbcrConversionKHR(dump_inst, device, ycbcrConversion, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkBindBufferMemory2KHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkBindBufferMemory2KHR(dump_inst, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkBindBufferMemory2KHR(dump_inst, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkBindBufferMemory2KHR(dump_inst, device, bindInfoCount, pBindInfos);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetValidationCacheDataEXT(ApiDumpInstance& dump_inst, VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetValidationCacheDataEXT(dump_inst, device, validationCache, pDataSize, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetValidationCacheDataEXT(dump_inst, device, validationCache, pDataSize, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetValidationCacheDataEXT(dump_inst, device, validationCache, pDataSize, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetDeviceQueue2(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetDeviceQueue2(dump_inst, device, pQueueInfo, pQueue);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetDeviceQueue2(dump_inst, device, pQueueInfo, pQueue);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetDeviceQueue2(dump_inst, device, pQueueInfo, pQueue);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dump_inst, physicalDevice, surface, pSurfaceCapabilities);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dump_inst, physicalDevice, surface, pSurfaceCapabilities);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dump_inst, physicalDevice, surface, pSurfaceCapabilities);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyDescriptorUpdateTemplateKHR(dump_inst, device, descriptorUpdateTemplate, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyDescriptorUpdateTemplateKHR(dump_inst, device, descriptorUpdateTemplate, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyDescriptorUpdateTemplateKHR(dump_inst, device, descriptorUpdateTemplate, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkBindImageMemory2KHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkBindImageMemory2KHR(dump_inst, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkBindImageMemory2KHR(dump_inst, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkBindImageMemory2KHR(dump_inst, device, bindInfoCount, pBindInfos);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyValidationCacheEXT(ApiDumpInstance& dump_inst, VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyValidationCacheEXT(dump_inst, device, validationCache, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyValidationCacheEXT(dump_inst, device, validationCache, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyValidationCacheEXT(dump_inst, device, validationCache, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
inline void dump_head_vkGetAndroidHardwareBufferPropertiesANDROID(ApiDumpInstance& dump_inst, VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetAndroidHardwareBufferPropertiesANDROID(dump_inst, device, buffer, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetAndroidHardwareBufferPropertiesANDROID(dump_inst, device, buffer, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetAndroidHardwareBufferPropertiesANDROID(dump_inst, device, buffer, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_ANDROID_KHR
inline void dump_head_vkGetRefreshCycleDurationGOOGLE(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetRefreshCycleDurationGOOGLE(dump_inst, device, swapchain, pDisplayTimingProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetRefreshCycleDurationGOOGLE(dump_inst, device, swapchain, pDisplayTimingProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetRefreshCycleDurationGOOGLE(dump_inst, device, swapchain, pDisplayTimingProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdWaitEvents(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdWaitEvents(dump_inst, commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdWaitEvents(dump_inst, commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdWaitEvents(dump_inst, commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdBindDescriptorSets(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdBindDescriptorSets(dump_inst, commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdBindDescriptorSets(dump_inst, commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdBindDescriptorSets(dump_inst, commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceFeatures2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceFeatures2(dump_inst, physicalDevice, pFeatures);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceFeatures2(dump_inst, physicalDevice, pFeatures);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceFeatures2(dump_inst, physicalDevice, pFeatures);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
inline void dump_head_vkGetMemoryAndroidHardwareBufferANDROID(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetMemoryAndroidHardwareBufferANDROID(dump_inst, device, pInfo, pBuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetMemoryAndroidHardwareBufferANDROID(dump_inst, device, pInfo, pBuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetMemoryAndroidHardwareBufferANDROID(dump_inst, device, pInfo, pBuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_ANDROID_KHR
inline void dump_head_vkCreateDescriptorPool(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateDescriptorPool(dump_inst, device, pCreateInfo, pAllocator, pDescriptorPool);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateDescriptorPool(dump_inst, device, pCreateInfo, pAllocator, pDescriptorPool);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateDescriptorPool(dump_inst, device, pCreateInfo, pAllocator, pDescriptorPool);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetStencilReference(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetStencilReference(dump_inst, commandBuffer, faceMask, reference);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetStencilReference(dump_inst, commandBuffer, faceMask, reference);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetStencilReference(dump_inst, commandBuffer, faceMask, reference);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkUpdateDescriptorSetWithTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkUpdateDescriptorSetWithTemplate(dump_inst, device, descriptorSet, descriptorUpdateTemplate, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkUpdateDescriptorSetWithTemplate(dump_inst, device, descriptorSet, descriptorUpdateTemplate, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkUpdateDescriptorSetWithTemplate(dump_inst, device, descriptorSet, descriptorUpdateTemplate, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdBindIndexBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdBindIndexBuffer(dump_inst, commandBuffer, buffer, offset, indexType);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdBindIndexBuffer(dump_inst, commandBuffer, buffer, offset, indexType);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdBindIndexBuffer(dump_inst, commandBuffer, buffer, offset, indexType);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceProperties2(dump_inst, physicalDevice, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceProperties2(dump_inst, physicalDevice, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceProperties2(dump_inst, physicalDevice, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyDescriptorUpdateTemplate(dump_inst, device, descriptorUpdateTemplate, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyDescriptorUpdateTemplate(dump_inst, device, descriptorUpdateTemplate, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyDescriptorUpdateTemplate(dump_inst, device, descriptorUpdateTemplate, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceSurfaceFormats2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceSurfaceFormats2KHR(dump_inst, physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceSurfaceFormats2KHR(dump_inst, physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceSurfaceFormats2KHR(dump_inst, physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdBindVertexBuffers(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdBindVertexBuffers(dump_inst, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdBindVertexBuffers(dump_inst, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdBindVertexBuffers(dump_inst, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceMemoryProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceMemoryProperties2(dump_inst, physicalDevice, pMemoryProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceMemoryProperties2(dump_inst, physicalDevice, pMemoryProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceMemoryProperties2(dump_inst, physicalDevice, pMemoryProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceFormatProperties2(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceFormatProperties2(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceFormatProperties2(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceSurfaceCapabilities2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceSurfaceCapabilities2KHR(dump_inst, physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceSurfaceCapabilities2KHR(dump_inst, physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceSurfaceCapabilities2KHR(dump_inst, physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkFreeDescriptorSets(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkFreeDescriptorSets(dump_inst, device, descriptorPool, descriptorSetCount, pDescriptorSets);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkFreeDescriptorSets(dump_inst, device, descriptorPool, descriptorSetCount, pDescriptorSets);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkFreeDescriptorSets(dump_inst, device, descriptorPool, descriptorSetCount, pDescriptorSets);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdPipelineBarrier(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdPipelineBarrier(dump_inst, commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdPipelineBarrier(dump_inst, commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdPipelineBarrier(dump_inst, commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceImageFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceImageFormatProperties2(dump_inst, physicalDevice, pImageFormatInfo, pImageFormatProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceImageFormatProperties2(dump_inst, physicalDevice, pImageFormatInfo, pImageFormatProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceImageFormatProperties2(dump_inst, physicalDevice, pImageFormatInfo, pImageFormatProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceSparseImageFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceSparseImageFormatProperties2(dump_inst, physicalDevice, pFormatInfo, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceSparseImageFormatProperties2(dump_inst, physicalDevice, pFormatInfo, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceSparseImageFormatProperties2(dump_inst, physicalDevice, pFormatInfo, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDraw(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDraw(dump_inst, commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDraw(dump_inst, commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDraw(dump_inst, commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceQueueFamilyProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceQueueFamilyProperties2(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceQueueFamilyProperties2(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceQueueFamilyProperties2(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyDescriptorPool(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyDescriptorPool(dump_inst, device, descriptorPool, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyDescriptorPool(dump_inst, device, descriptorPool, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyDescriptorPool(dump_inst, device, descriptorPool, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkResetDescriptorPool(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkResetDescriptorPool(dump_inst, device, descriptorPool, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkResetDescriptorPool(dump_inst, device, descriptorPool, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkResetDescriptorPool(dump_inst, device, descriptorPool, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkTrimCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkTrimCommandPool(dump_inst, device, commandPool, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkTrimCommandPool(dump_inst, device, commandPool, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkTrimCommandPool(dump_inst, device, commandPool, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDrawIndexed(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDrawIndexed(dump_inst, commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDrawIndexed(dump_inst, commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDrawIndexed(dump_inst, commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkAllocateDescriptorSets(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkAllocateDescriptorSets(dump_inst, device, pAllocateInfo, pDescriptorSets);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkAllocateDescriptorSets(dump_inst, device, pAllocateInfo, pDescriptorSets);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkAllocateDescriptorSets(dump_inst, device, pAllocateInfo, pDescriptorSets);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDrawMeshTasksIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDrawMeshTasksIndirectNV(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDrawMeshTasksIndirectNV(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDrawMeshTasksIndirectNV(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDrawMeshTasksIndirectCountNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDrawMeshTasksIndirectCountNV(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDrawMeshTasksIndirectCountNV(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDrawMeshTasksIndirectCountNV(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDrawIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDrawIndirect(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDrawIndirect(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDrawIndirect(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDrawMeshTasksNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDrawMeshTasksNV(dump_inst, commandBuffer, taskCount, firstTask);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDrawMeshTasksNV(dump_inst, commandBuffer, taskCount, firstTask);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDrawMeshTasksNV(dump_inst, commandBuffer, taskCount, firstTask);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDrawIndexedIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDrawIndexedIndirect(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDrawIndexedIndirect(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDrawIndexedIndirect(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdBeginQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdBeginQuery(dump_inst, commandBuffer, queryPool, query, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdBeginQuery(dump_inst, commandBuffer, queryPool, query, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdBeginQuery(dump_inst, commandBuffer, queryPool, query, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkUpdateDescriptorSets(ApiDumpInstance& dump_inst, VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkUpdateDescriptorSets(dump_inst, device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkUpdateDescriptorSets(dump_inst, device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkUpdateDescriptorSets(dump_inst, device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDispatch(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDispatch(dump_inst, commandBuffer, groupCountX, groupCountY, groupCountZ);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDispatch(dump_inst, commandBuffer, groupCountX, groupCountY, groupCountZ);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDispatch(dump_inst, commandBuffer, groupCountX, groupCountY, groupCountZ);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdCopyBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdCopyBuffer(dump_inst, commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdCopyBuffer(dump_inst, commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdCopyBuffer(dump_inst, commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdResetQueryPool(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdResetQueryPool(dump_inst, commandBuffer, queryPool, firstQuery, queryCount);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdResetQueryPool(dump_inst, commandBuffer, queryPool, firstQuery, queryCount);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdResetQueryPool(dump_inst, commandBuffer, queryPool, firstQuery, queryCount);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdEndQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdEndQuery(dump_inst, commandBuffer, queryPool, query);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdEndQuery(dump_inst, commandBuffer, queryPool, query);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdEndQuery(dump_inst, commandBuffer, queryPool, query);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkSetDebugUtilsObjectTagEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkSetDebugUtilsObjectTagEXT(dump_inst, device, pTagInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkSetDebugUtilsObjectTagEXT(dump_inst, device, pTagInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkSetDebugUtilsObjectTagEXT(dump_inst, device, pTagInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdWriteTimestamp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdWriteTimestamp(dump_inst, commandBuffer, pipelineStage, queryPool, query);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdWriteTimestamp(dump_inst, commandBuffer, pipelineStage, queryPool, query);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdWriteTimestamp(dump_inst, commandBuffer, pipelineStage, queryPool, query);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkQueueBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkQueueBeginDebugUtilsLabelEXT(dump_inst, queue, pLabelInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkQueueBeginDebugUtilsLabelEXT(dump_inst, queue, pLabelInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkQueueBeginDebugUtilsLabelEXT(dump_inst, queue, pLabelInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDispatchIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDispatchIndirect(dump_inst, commandBuffer, buffer, offset);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDispatchIndirect(dump_inst, commandBuffer, buffer, offset);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDispatchIndirect(dump_inst, commandBuffer, buffer, offset);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceExternalBufferProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceExternalBufferProperties(dump_inst, physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceExternalBufferProperties(dump_inst, physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceExternalBufferProperties(dump_inst, physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdCopyImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdCopyImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdCopyImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdCopyImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkQueueEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkQueueEndDebugUtilsLabelEXT(dump_inst, queue);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkQueueEndDebugUtilsLabelEXT(dump_inst, queue);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkQueueEndDebugUtilsLabelEXT(dump_inst, queue);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_XCB_KHR)
inline void dump_head_vkGetPhysicalDeviceXcbPresentationSupportKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceXcbPresentationSupportKHR(dump_inst, physicalDevice, queueFamilyIndex, connection, visual_id);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceXcbPresentationSupportKHR(dump_inst, physicalDevice, queueFamilyIndex, connection, visual_id);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceXcbPresentationSupportKHR(dump_inst, physicalDevice, queueFamilyIndex, connection, visual_id);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_XCB_KHR
inline void dump_head_vkGetDeviceGroupPeerMemoryFeatures(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetDeviceGroupPeerMemoryFeatures(dump_inst, device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetDeviceGroupPeerMemoryFeatures(dump_inst, device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetDeviceGroupPeerMemoryFeatures(dump_inst, device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetDeviceQueue(ApiDumpInstance& dump_inst, VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetDeviceQueue(dump_inst, device, queueFamilyIndex, queueIndex, pQueue);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetDeviceQueue(dump_inst, device, queueFamilyIndex, queueIndex, pQueue);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetDeviceQueue(dump_inst, device, queueFamilyIndex, queueIndex, pQueue);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdClearColorImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdClearColorImage(dump_inst, commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdClearColorImage(dump_inst, commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdClearColorImage(dump_inst, commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkQueueSubmit(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkQueueSubmit(dump_inst, queue, submitCount, pSubmits, fence);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkQueueSubmit(dump_inst, queue, submitCount, pSubmits, fence);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkQueueSubmit(dump_inst, queue, submitCount, pSubmits, fence);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceFeatures2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceFeatures2KHR(dump_inst, physicalDevice, pFeatures);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceFeatures2KHR(dump_inst, physicalDevice, pFeatures);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceFeatures2KHR(dump_inst, physicalDevice, pFeatures);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetRenderAreaGranularity(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetRenderAreaGranularity(dump_inst, device, renderPass, pGranularity);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetRenderAreaGranularity(dump_inst, device, renderPass, pGranularity);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetRenderAreaGranularity(dump_inst, device, renderPass, pGranularity);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateDevice(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateDevice(dump_inst, physicalDevice, pCreateInfo, pAllocator, pDevice);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateDevice(dump_inst, physicalDevice, pCreateInfo, pAllocator, pDevice);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateDevice(dump_inst, physicalDevice, pCreateInfo, pAllocator, pDevice);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetImageSparseMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetImageSparseMemoryRequirements2(dump_inst, device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetImageSparseMemoryRequirements2(dump_inst, device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetImageSparseMemoryRequirements2(dump_inst, device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetDeviceMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetDeviceMask(dump_inst, commandBuffer, deviceMask);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetDeviceMask(dump_inst, commandBuffer, deviceMask);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetDeviceMask(dump_inst, commandBuffer, deviceMask);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetDiscardRectangleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetDiscardRectangleEXT(dump_inst, commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetDiscardRectangleEXT(dump_inst, commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetDiscardRectangleEXT(dump_inst, commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdClearDepthStencilImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdClearDepthStencilImage(dump_inst, commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdClearDepthStencilImage(dump_inst, commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdClearDepthStencilImage(dump_inst, commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetImageMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetImageMemoryRequirements2(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetImageMemoryRequirements2(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetImageMemoryRequirements2(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateCommandPool(ApiDumpInstance& dump_inst, VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateCommandPool(dump_inst, device, pCreateInfo, pAllocator, pCommandPool);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateCommandPool(dump_inst, device, pCreateInfo, pAllocator, pCommandPool);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateCommandPool(dump_inst, device, pCreateInfo, pAllocator, pCommandPool);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDispatchBase(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDispatchBase(dump_inst, commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDispatchBase(dump_inst, commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDispatchBase(dump_inst, commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceProperties2KHR(dump_inst, physicalDevice, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceProperties2KHR(dump_inst, physicalDevice, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceProperties2KHR(dump_inst, physicalDevice, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_head_vkImportSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkImportSemaphoreWin32HandleKHR(dump_inst, device, pImportSemaphoreWin32HandleInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkImportSemaphoreWin32HandleKHR(dump_inst, device, pImportSemaphoreWin32HandleInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkImportSemaphoreWin32HandleKHR(dump_inst, device, pImportSemaphoreWin32HandleInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_head_vkDestroyDevice(ApiDumpInstance& dump_inst, VkDevice device, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyDevice(dump_inst, device, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyDevice(dump_inst, device, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyDevice(dump_inst, device, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceMemoryProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceMemoryProperties2KHR(dump_inst, physicalDevice, pMemoryProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceMemoryProperties2KHR(dump_inst, physicalDevice, pMemoryProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceMemoryProperties2KHR(dump_inst, physicalDevice, pMemoryProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceSparseImageFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceSparseImageFormatProperties(dump_inst, physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceSparseImageFormatProperties(dump_inst, physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceSparseImageFormatProperties(dump_inst, physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetBufferMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetBufferMemoryRequirements2(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetBufferMemoryRequirements2(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetBufferMemoryRequirements2(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdEndRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR*        pSubpassEndInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdEndRenderPass2KHR(dump_inst, commandBuffer, pSubpassEndInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdEndRenderPass2KHR(dump_inst, commandBuffer, pSubpassEndInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdEndRenderPass2KHR(dump_inst, commandBuffer, pSubpassEndInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceFormatProperties2KHR(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceFormatProperties2KHR(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceFormatProperties2KHR(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceSurfacePresentModesKHR(dump_inst, physicalDevice, surface, pPresentModeCount, pPresentModes);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceSurfacePresentModesKHR(dump_inst, physicalDevice, surface, pPresentModeCount, pPresentModes);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceSurfacePresentModesKHR(dump_inst, physicalDevice, surface, pPresentModeCount, pPresentModes);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdClearAttachments(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdClearAttachments(dump_inst, commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdClearAttachments(dump_inst, commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdClearAttachments(dump_inst, commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceImageFormatProperties2KHR(dump_inst, physicalDevice, pImageFormatInfo, pImageFormatProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceImageFormatProperties2KHR(dump_inst, physicalDevice, pImageFormatInfo, pImageFormatProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceImageFormatProperties2KHR(dump_inst, physicalDevice, pImageFormatInfo, pImageFormatProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(dump_inst, physicalDevice, pFormatInfo, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(dump_inst, physicalDevice, pFormatInfo, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(dump_inst, physicalDevice, pFormatInfo, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceQueueFamilyProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceQueueFamilyProperties2KHR(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceQueueFamilyProperties2KHR(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceQueueFamilyProperties2KHR(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceFormatProperties(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceFormatProperties(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceFormatProperties(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkQueueBindSparse(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkQueueBindSparse(dump_inst, queue, bindInfoCount, pBindInfo, fence);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkQueueBindSparse(dump_inst, queue, bindInfoCount, pBindInfo, fence);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkQueueBindSparse(dump_inst, queue, bindInfoCount, pBindInfo, fence);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkFreeCommandBuffers(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkFreeCommandBuffers(dump_inst, device, commandPool, commandBufferCount, pCommandBuffers);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkFreeCommandBuffers(dump_inst, device, commandPool, commandBufferCount, pCommandBuffers);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkFreeCommandBuffers(dump_inst, device, commandPool, commandBufferCount, pCommandBuffers);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_head_vkGetSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetSemaphoreWin32HandleKHR(dump_inst, device, pGetWin32HandleInfo, pHandle);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetSemaphoreWin32HandleKHR(dump_inst, device, pGetWin32HandleInfo, pHandle);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetSemaphoreWin32HandleKHR(dump_inst, device, pGetWin32HandleInfo, pHandle);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_head_vkGetDeviceGroupPresentCapabilitiesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetDeviceGroupPresentCapabilitiesKHR(dump_inst, device, pDeviceGroupPresentCapabilities);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetDeviceGroupPresentCapabilitiesKHR(dump_inst, device, pDeviceGroupPresentCapabilities);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetDeviceGroupPresentCapabilitiesKHR(dump_inst, device, pDeviceGroupPresentCapabilities);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetDeviceGroupPeerMemoryFeaturesKHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetDeviceGroupPeerMemoryFeaturesKHR(dump_inst, device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetDeviceGroupPeerMemoryFeaturesKHR(dump_inst, device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetDeviceGroupPeerMemoryFeaturesKHR(dump_inst, device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkImportSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkImportSemaphoreFdKHR(dump_inst, device, pImportSemaphoreFdInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkImportSemaphoreFdKHR(dump_inst, device, pImportSemaphoreFdInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkImportSemaphoreFdKHR(dump_inst, device, pImportSemaphoreFdInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetDeviceMaskKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetDeviceMaskKHR(dump_inst, commandBuffer, deviceMask);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetDeviceMaskKHR(dump_inst, commandBuffer, deviceMask);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetDeviceMaskKHR(dump_inst, commandBuffer, deviceMask);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyCommandPool(dump_inst, device, commandPool, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyCommandPool(dump_inst, device, commandPool, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyCommandPool(dump_inst, device, commandPool, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDevicePresentRectanglesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDevicePresentRectanglesKHR(dump_inst, physicalDevice, surface, pRectCount, pRects);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDevicePresentRectanglesKHR(dump_inst, physicalDevice, surface, pRectCount, pRects);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDevicePresentRectanglesKHR(dump_inst, physicalDevice, surface, pRectCount, pRects);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateSamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateSamplerYcbcrConversion(dump_inst, device, pCreateInfo, pAllocator, pYcbcrConversion);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateSamplerYcbcrConversion(dump_inst, device, pCreateInfo, pAllocator, pYcbcrConversion);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateSamplerYcbcrConversion(dump_inst, device, pCreateInfo, pAllocator, pYcbcrConversion);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkResetCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkResetCommandPool(dump_inst, device, commandPool, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkResetCommandPool(dump_inst, device, commandPool, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkResetCommandPool(dump_inst, device, commandPool, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetDeviceGroupSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetDeviceGroupSurfacePresentModesKHR(dump_inst, device, surface, pModes);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetDeviceGroupSurfacePresentModesKHR(dump_inst, device, surface, pModes);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetDeviceGroupSurfacePresentModesKHR(dump_inst, device, surface, pModes);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkEnumerateDeviceExtensionProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkEnumerateDeviceExtensionProperties(dump_inst, physicalDevice, pLayerName, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkEnumerateDeviceExtensionProperties(dump_inst, physicalDevice, pLayerName, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkEnumerateDeviceExtensionProperties(dump_inst, physicalDevice, pLayerName, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdBeginRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdBeginRenderPass2KHR(dump_inst, commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdBeginRenderPass2KHR(dump_inst, commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdBeginRenderPass2KHR(dump_inst, commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdResolveImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdResolveImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdResolveImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdResolveImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkEnumerateInstanceExtensionProperties(ApiDumpInstance& dump_inst, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkEnumerateInstanceExtensionProperties(dump_inst, pLayerName, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkEnumerateInstanceExtensionProperties(dump_inst, pLayerName, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkEnumerateInstanceExtensionProperties(dump_inst, pLayerName, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkAllocateCommandBuffers(ApiDumpInstance& dump_inst, VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkAllocateCommandBuffers(dump_inst, device, pAllocateInfo, pCommandBuffers);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkAllocateCommandBuffers(dump_inst, device, pAllocateInfo, pCommandBuffers);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkAllocateCommandBuffers(dump_inst, device, pAllocateInfo, pCommandBuffers);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdDispatchBaseKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdDispatchBaseKHR(dump_inst, commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdDispatchBaseKHR(dump_inst, commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdDispatchBaseKHR(dump_inst, commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdResetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdResetEvent(dump_inst, commandBuffer, event, stageMask);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdResetEvent(dump_inst, commandBuffer, event, stageMask);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdResetEvent(dump_inst, commandBuffer, event, stageMask);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkAcquireNextImage2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkAcquireNextImage2KHR(dump_inst, device, pAcquireInfo, pImageIndex);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkAcquireNextImage2KHR(dump_inst, device, pAcquireInfo, pImageIndex);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkAcquireNextImage2KHR(dump_inst, device, pAcquireInfo, pImageIndex);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroySamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroySamplerYcbcrConversion(dump_inst, device, ycbcrConversion, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroySamplerYcbcrConversion(dump_inst, device, ycbcrConversion, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroySamplerYcbcrConversion(dump_inst, device, ycbcrConversion, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkEnumeratePhysicalDeviceGroups(ApiDumpInstance& dump_inst, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkEnumeratePhysicalDeviceGroups(dump_inst, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkEnumeratePhysicalDeviceGroups(dump_inst, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkEnumeratePhysicalDeviceGroups(dump_inst, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdPushDescriptorSetKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdPushDescriptorSetKHR(dump_inst, commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdPushDescriptorSetKHR(dump_inst, commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdPushDescriptorSetKHR(dump_inst, commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkQueueWaitIdle(ApiDumpInstance& dump_inst, VkQueue queue)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkQueueWaitIdle(dump_inst, queue);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkQueueWaitIdle(dump_inst, queue);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkQueueWaitIdle(dump_inst, queue);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_head_vkCreateWin32SurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateWin32SurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateWin32SurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateWin32SurfaceKHR(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_head_vkCmdNextSubpass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo, const VkSubpassEndInfoKHR*        pSubpassEndInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdNextSubpass2KHR(dump_inst, commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdNextSubpass2KHR(dump_inst, commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdNextSubpass2KHR(dump_inst, commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetSemaphoreFdKHR(dump_inst, device, pGetFdInfo, pFd);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetSemaphoreFdKHR(dump_inst, device, pGetFdInfo, pFd);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetSemaphoreFdKHR(dump_inst, device, pGetFdInfo, pFd);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkEnumerateInstanceLayerProperties(ApiDumpInstance& dump_inst, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkEnumerateInstanceLayerProperties(dump_inst, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkEnumerateInstanceLayerProperties(dump_inst, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkEnumerateInstanceLayerProperties(dump_inst, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetViewportShadingRatePaletteNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetViewportShadingRatePaletteNV(dump_inst, commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetViewportShadingRatePaletteNV(dump_inst, commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetViewportShadingRatePaletteNV(dump_inst, commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkEnumerateDeviceLayerProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkEnumerateDeviceLayerProperties(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkEnumerateDeviceLayerProperties(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkEnumerateDeviceLayerProperties(dump_inst, physicalDevice, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkBeginCommandBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkBeginCommandBuffer(dump_inst, commandBuffer, pBeginInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkBeginCommandBuffer(dump_inst, commandBuffer, pBeginInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkBeginCommandBuffer(dump_inst, commandBuffer, pBeginInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDeviceWaitIdle(ApiDumpInstance& dump_inst, VkDevice device)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDeviceWaitIdle(dump_inst, device);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDeviceWaitIdle(dump_inst, device);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDeviceWaitIdle(dump_inst, device);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_head_vkGetPhysicalDeviceWin32PresentationSupportKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceWin32PresentationSupportKHR(dump_inst, physicalDevice, queueFamilyIndex);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceWin32PresentationSupportKHR(dump_inst, physicalDevice, queueFamilyIndex);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceWin32PresentationSupportKHR(dump_inst, physicalDevice, queueFamilyIndex);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_VI_NN)
inline void dump_head_vkCreateViSurfaceNN(ApiDumpInstance& dump_inst, VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateViSurfaceNN(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateViSurfaceNN(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateViSurfaceNN(dump_inst, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_VI_NN
inline void dump_head_vkCreateDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateDescriptorUpdateTemplate(dump_inst, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateDescriptorUpdateTemplate(dump_inst, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateDescriptorUpdateTemplate(dump_inst, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetSwapchainStatusKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetSwapchainStatusKHR(dump_inst, device, swapchain);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetSwapchainStatusKHR(dump_inst, device, swapchain);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetSwapchainStatusKHR(dump_inst, device, swapchain);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkAllocateMemory(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkAllocateMemory(dump_inst, device, pAllocateInfo, pAllocator, pMemory);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkAllocateMemory(dump_inst, device, pAllocateInfo, pAllocator, pMemory);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkAllocateMemory(dump_inst, device, pAllocateInfo, pAllocator, pMemory);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkEndCommandBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkEndCommandBuffer(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkEndCommandBuffer(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkEndCommandBuffer(dump_inst, commandBuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdPushDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdPushDescriptorSetWithTemplateKHR(dump_inst, commandBuffer, descriptorUpdateTemplate, layout, set, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdPushDescriptorSetWithTemplateKHR(dump_inst, commandBuffer, descriptorUpdateTemplate, layout, set, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdPushDescriptorSetWithTemplateKHR(dump_inst, commandBuffer, descriptorUpdateTemplate, layout, set, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetEvent(dump_inst, commandBuffer, event, stageMask);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetEvent(dump_inst, commandBuffer, event, stageMask);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetEvent(dump_inst, commandBuffer, event, stageMask);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkSetHdrMetadataEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkSetHdrMetadataEXT(dump_inst, device, swapchainCount, pSwapchains, pMetadata);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkSetHdrMetadataEXT(dump_inst, device, swapchainCount, pSwapchains, pMetadata);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkSetHdrMetadataEXT(dump_inst, device, swapchainCount, pSwapchains, pMetadata);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdBindShadingRateImageNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdBindShadingRateImageNV(dump_inst, commandBuffer, imageView, imageLayout);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdBindShadingRateImageNV(dump_inst, commandBuffer, imageView, imageLayout);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdBindShadingRateImageNV(dump_inst, commandBuffer, imageView, imageLayout);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateFence(ApiDumpInstance& dump_inst, VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateFence(dump_inst, device, pCreateInfo, pAllocator, pFence);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateFence(dump_inst, device, pCreateInfo, pAllocator, pFence);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateFence(dump_inst, device, pCreateInfo, pAllocator, pFence);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdCopyQueryPoolResults(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdCopyQueryPoolResults(dump_inst, commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdCopyQueryPoolResults(dump_inst, commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdCopyQueryPoolResults(dump_inst, commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdPushConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdPushConstants(dump_inst, commandBuffer, layout, stageFlags, offset, size, pValues);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdPushConstants(dump_inst, commandBuffer, layout, stageFlags, offset, size, pValues);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdPushConstants(dump_inst, commandBuffer, layout, stageFlags, offset, size, pValues);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetExclusiveScissorNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetExclusiveScissorNV(dump_inst, commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetExclusiveScissorNV(dump_inst, commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetExclusiveScissorNV(dump_inst, commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceExternalFencePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceExternalFencePropertiesKHR(dump_inst, physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceExternalFencePropertiesKHR(dump_inst, physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceExternalFencePropertiesKHR(dump_inst, physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateFramebuffer(ApiDumpInstance& dump_inst, VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateFramebuffer(dump_inst, device, pCreateInfo, pAllocator, pFramebuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateFramebuffer(dump_inst, device, pCreateInfo, pAllocator, pFramebuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateFramebuffer(dump_inst, device, pCreateInfo, pAllocator, pFramebuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkBindImageMemory2(ApiDumpInstance& dump_inst, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkBindImageMemory2(dump_inst, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkBindImageMemory2(dump_inst, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkBindImageMemory2(dump_inst, device, bindInfoCount, pBindInfos);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdNextSubpass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSubpassContents contents)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdNextSubpass(dump_inst, commandBuffer, contents);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdNextSubpass(dump_inst, commandBuffer, contents);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdNextSubpass(dump_inst, commandBuffer, contents);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdSetCheckpointNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const void* pCheckpointMarker)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdSetCheckpointNV(dump_inst, commandBuffer, pCheckpointMarker);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdSetCheckpointNV(dump_inst, commandBuffer, pCheckpointMarker);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdSetCheckpointNV(dump_inst, commandBuffer, pCheckpointMarker);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPipelineExecutableStatisticsKHR(ApiDumpInstance& dump_inst, VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPipelineExecutableStatisticsKHR(dump_inst, device, pExecutableInfo, pStatisticCount, pStatistics);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPipelineExecutableStatisticsKHR(dump_inst, device, pExecutableInfo, pStatisticCount, pStatistics);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPipelineExecutableStatisticsKHR(dump_inst, device, pExecutableInfo, pStatisticCount, pStatistics);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPipelineExecutableInternalRepresentationsKHR(ApiDumpInstance& dump_inst, VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPipelineExecutableInternalRepresentationsKHR(dump_inst, device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPipelineExecutableInternalRepresentationsKHR(dump_inst, device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPipelineExecutableInternalRepresentationsKHR(dump_inst, device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_head_vkImportFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkImportFenceWin32HandleKHR(dump_inst, device, pImportFenceWin32HandleInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkImportFenceWin32HandleKHR(dump_inst, device, pImportFenceWin32HandleInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkImportFenceWin32HandleKHR(dump_inst, device, pImportFenceWin32HandleInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_head_vkCmdBeginRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdBeginRenderPass(dump_inst, commandBuffer, pRenderPassBegin, contents);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdBeginRenderPass(dump_inst, commandBuffer, pRenderPassBegin, contents);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdBeginRenderPass(dump_inst, commandBuffer, pRenderPassBegin, contents);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateRenderPass2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderPassCreateInfo2KHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateRenderPass2KHR(dump_inst, device, pCreateInfo, pAllocator, pRenderPass);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateRenderPass2KHR(dump_inst, device, pCreateInfo, pAllocator, pRenderPass);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateRenderPass2KHR(dump_inst, device, pCreateInfo, pAllocator, pRenderPass);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetQueueCheckpointDataNV(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetQueueCheckpointDataNV(dump_inst, queue, pCheckpointDataCount, pCheckpointData);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetQueueCheckpointDataNV(dump_inst, queue, pCheckpointDataCount, pCheckpointData);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetQueueCheckpointDataNV(dump_inst, queue, pCheckpointDataCount, pCheckpointData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetFenceStatus(ApiDumpInstance& dump_inst, VkDevice device, VkFence fence)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetFenceStatus(dump_inst, device, fence);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetFenceStatus(dump_inst, device, fence);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetFenceStatus(dump_inst, device, fence);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateRenderPass(ApiDumpInstance& dump_inst, VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateRenderPass(dump_inst, device, pCreateInfo, pAllocator, pRenderPass);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateRenderPass(dump_inst, device, pCreateInfo, pAllocator, pRenderPass);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateRenderPass(dump_inst, device, pCreateInfo, pAllocator, pRenderPass);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_head_vkGetFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetFenceWin32HandleKHR(dump_inst, device, pGetWin32HandleInfo, pHandle);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetFenceWin32HandleKHR(dump_inst, device, pGetWin32HandleInfo, pHandle);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetFenceWin32HandleKHR(dump_inst, device, pGetWin32HandleInfo, pHandle);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_head_vkCmdEndRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdEndRenderPass(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdEndRenderPass(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdEndRenderPass(dump_inst, commandBuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyFramebuffer(ApiDumpInstance& dump_inst, VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyFramebuffer(dump_inst, device, framebuffer, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyFramebuffer(dump_inst, device, framebuffer, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyFramebuffer(dump_inst, device, framebuffer, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkImportFenceFdKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkImportFenceFdKHR(dump_inst, device, pImportFenceFdInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkImportFenceFdKHR(dump_inst, device, pImportFenceFdInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkImportFenceFdKHR(dump_inst, device, pImportFenceFdInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCmdExecuteCommands(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCmdExecuteCommands(dump_inst, commandBuffer, commandBufferCount, pCommandBuffers);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCmdExecuteCommands(dump_inst, commandBuffer, commandBufferCount, pCommandBuffers);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCmdExecuteCommands(dump_inst, commandBuffer, commandBufferCount, pCommandBuffers);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyFence(ApiDumpInstance& dump_inst, VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyFence(dump_inst, device, fence, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyFence(dump_inst, device, fence, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyFence(dump_inst, device, fence, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateShaderModule(ApiDumpInstance& dump_inst, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateShaderModule(dump_inst, device, pCreateInfo, pAllocator, pShaderModule);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateShaderModule(dump_inst, device, pCreateInfo, pAllocator, pShaderModule);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateShaderModule(dump_inst, device, pCreateInfo, pAllocator, pShaderModule);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkResetFences(ApiDumpInstance& dump_inst, VkDevice device, uint32_t fenceCount, const VkFence* pFences)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkResetFences(dump_inst, device, fenceCount, pFences);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkResetFences(dump_inst, device, fenceCount, pFences);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkResetFences(dump_inst, device, fenceCount, pFences);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyImageView(ApiDumpInstance& dump_inst, VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyImageView(dump_inst, device, imageView, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyImageView(dump_inst, device, imageView, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyImageView(dump_inst, device, imageView, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceExternalFenceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceExternalFenceProperties(dump_inst, physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceExternalFenceProperties(dump_inst, physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceExternalFenceProperties(dump_inst, physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkWaitForFences(ApiDumpInstance& dump_inst, VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkWaitForFences(dump_inst, device, fenceCount, pFences, waitAll, timeout);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkWaitForFences(dump_inst, device, fenceCount, pFences, waitAll, timeout);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkWaitForFences(dump_inst, device, fenceCount, pFences, waitAll, timeout);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateSemaphore(ApiDumpInstance& dump_inst, VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateSemaphore(dump_inst, device, pCreateInfo, pAllocator, pSemaphore);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateSemaphore(dump_inst, device, pCreateInfo, pAllocator, pSemaphore);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateSemaphore(dump_inst, device, pCreateInfo, pAllocator, pSemaphore);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroySemaphore(ApiDumpInstance& dump_inst, VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroySemaphore(dump_inst, device, semaphore, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroySemaphore(dump_inst, device, semaphore, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroySemaphore(dump_inst, device, semaphore, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyShaderModule(ApiDumpInstance& dump_inst, VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyShaderModule(dump_inst, device, shaderModule, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyShaderModule(dump_inst, device, shaderModule, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyShaderModule(dump_inst, device, shaderModule, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetFenceFdKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetFenceFdKHR(dump_inst, device, pGetFdInfo, pFd);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetFenceFdKHR(dump_inst, device, pGetFdInfo, pFd);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetFenceFdKHR(dump_inst, device, pGetFdInfo, pFd);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyInstance(ApiDumpInstance& dump_inst, VkInstance instance, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyInstance(dump_inst, instance, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyInstance(dump_inst, instance, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyInstance(dump_inst, instance, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateInstance(ApiDumpInstance& dump_inst, const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateInstance(dump_inst, pCreateInfo, pAllocator, pInstance);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateInstance(dump_inst, pCreateInfo, pAllocator, pInstance);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateInstance(dump_inst, pCreateInfo, pAllocator, pInstance);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetQueryPoolResults(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetQueryPoolResults(dump_inst, device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetQueryPoolResults(dump_inst, device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetQueryPoolResults(dump_inst, device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkUnmapMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkUnmapMemory(dump_inst, device, memory);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkUnmapMemory(dump_inst, device, memory);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkUnmapMemory(dump_inst, device, memory);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_head_vkGetMemoryWin32HandlePropertiesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetMemoryWin32HandlePropertiesKHR(dump_inst, device, handleType, handle, pMemoryWin32HandleProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetMemoryWin32HandlePropertiesKHR(dump_inst, device, handleType, handle, pMemoryWin32HandleProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetMemoryWin32HandlePropertiesKHR(dump_inst, device, handleType, handle, pMemoryWin32HandleProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_head_vkMapMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkMapMemory(dump_inst, device, memory, offset, size, flags, ppData);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkMapMemory(dump_inst, device, memory, offset, size, flags, ppData);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkMapMemory(dump_inst, device, memory, offset, size, flags, ppData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkFreeMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkFreeMemory(dump_inst, device, memory, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkFreeMemory(dump_inst, device, memory, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkFreeMemory(dump_inst, device, memory, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_head_vkGetMemoryWin32HandleKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetMemoryWin32HandleKHR(dump_inst, device, pGetWin32HandleInfo, pHandle);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetMemoryWin32HandleKHR(dump_inst, device, pGetWin32HandleInfo, pHandle);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetMemoryWin32HandleKHR(dump_inst, device, pGetWin32HandleInfo, pHandle);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_head_vkCreateBuffer(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateBuffer(dump_inst, device, pCreateInfo, pAllocator, pBuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateBuffer(dump_inst, device, pCreateInfo, pAllocator, pBuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateBuffer(dump_inst, device, pCreateInfo, pAllocator, pBuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkBindBufferMemory(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkBindBufferMemory(dump_inst, device, buffer, memory, memoryOffset);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkBindBufferMemory(dump_inst, device, buffer, memory, memoryOffset);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkBindBufferMemory(dump_inst, device, buffer, memory, memoryOffset);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkFlushMappedMemoryRanges(ApiDumpInstance& dump_inst, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkFlushMappedMemoryRanges(dump_inst, device, memoryRangeCount, pMemoryRanges);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkFlushMappedMemoryRanges(dump_inst, device, memoryRangeCount, pMemoryRanges);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkFlushMappedMemoryRanges(dump_inst, device, memoryRangeCount, pMemoryRanges);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetMemoryFdPropertiesKHR(ApiDumpInstance& dump_inst, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetMemoryFdPropertiesKHR(dump_inst, device, handleType, fd, pMemoryFdProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetMemoryFdPropertiesKHR(dump_inst, device, handleType, fd, pMemoryFdProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetMemoryFdPropertiesKHR(dump_inst, device, handleType, fd, pMemoryFdProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceFeatures(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceFeatures(dump_inst, physicalDevice, pFeatures);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceFeatures(dump_inst, physicalDevice, pFeatures);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceFeatures(dump_inst, physicalDevice, pFeatures);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkEnumeratePhysicalDevices(ApiDumpInstance& dump_inst, VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkEnumeratePhysicalDevices(dump_inst, instance, pPhysicalDeviceCount, pPhysicalDevices);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkEnumeratePhysicalDevices(dump_inst, instance, pPhysicalDeviceCount, pPhysicalDevices);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkEnumeratePhysicalDevices(dump_inst, instance, pPhysicalDeviceCount, pPhysicalDevices);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkInvalidateMappedMemoryRanges(ApiDumpInstance& dump_inst, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkInvalidateMappedMemoryRanges(dump_inst, device, memoryRangeCount, pMemoryRanges);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkInvalidateMappedMemoryRanges(dump_inst, device, memoryRangeCount, pMemoryRanges);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkInvalidateMappedMemoryRanges(dump_inst, device, memoryRangeCount, pMemoryRanges);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetMemoryFdKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetMemoryFdKHR(dump_inst, device, pGetFdInfo, pFd);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetMemoryFdKHR(dump_inst, device, pGetFdInfo, pFd);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetMemoryFdKHR(dump_inst, device, pGetFdInfo, pFd);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateDebugReportCallbackEXT(dump_inst, instance, pCreateInfo, pAllocator, pCallback);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateDebugReportCallbackEXT(dump_inst, instance, pCreateInfo, pAllocator, pCallback);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateDebugReportCallbackEXT(dump_inst, instance, pCreateInfo, pAllocator, pCallback);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceImageFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceImageFormatProperties(dump_inst, physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceImageFormatProperties(dump_inst, physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceImageFormatProperties(dump_inst, physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetDeviceMemoryCommitment(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetDeviceMemoryCommitment(dump_inst, device, memory, pCommittedMemoryInBytes);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetDeviceMemoryCommitment(dump_inst, device, memory, pCommittedMemoryInBytes);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetDeviceMemoryCommitment(dump_inst, device, memory, pCommittedMemoryInBytes);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkBindImageMemory(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkBindImageMemory(dump_inst, device, image, memory, memoryOffset);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkBindImageMemory(dump_inst, device, image, memory, memoryOffset);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkBindImageMemory(dump_inst, device, image, memory, memoryOffset);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkCreateBufferView(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkCreateBufferView(dump_inst, device, pCreateInfo, pAllocator, pView);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkCreateBufferView(dump_inst, device, pCreateInfo, pAllocator, pView);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkCreateBufferView(dump_inst, device, pCreateInfo, pAllocator, pView);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDebugReportMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDebugReportMessageEXT(dump_inst, instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDebugReportMessageEXT(dump_inst, instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDebugReportMessageEXT(dump_inst, instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetImageSparseMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetImageSparseMemoryRequirements(dump_inst, device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetImageSparseMemoryRequirements(dump_inst, device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetImageSparseMemoryRequirements(dump_inst, device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetBufferMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetBufferMemoryRequirements(dump_inst, device, buffer, pMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetBufferMemoryRequirements(dump_inst, device, buffer, pMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetBufferMemoryRequirements(dump_inst, device, buffer, pMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyDebugReportCallbackEXT(dump_inst, instance, callback, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyDebugReportCallbackEXT(dump_inst, instance, callback, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyDebugReportCallbackEXT(dump_inst, instance, callback, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(dump_inst, physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(dump_inst, physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(dump_inst, physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkGetImageMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkGetImageMemoryRequirements(dump_inst, device, image, pMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkGetImageMemoryRequirements(dump_inst, device, image, pMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkGetImageMemoryRequirements(dump_inst, device, image, pMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_head_vkDestroyBuffer(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_head_vkDestroyBuffer(dump_inst, device, buffer, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_head_vkDestroyBuffer(dump_inst, device, buffer, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_head_vkDestroyBuffer(dump_inst, device, buffer, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}


inline void dump_body_vkCreatePipelineCache(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreatePipelineCache(dump_inst, result, device, pCreateInfo, pAllocator, pPipelineCache);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreatePipelineCache(dump_inst, result, device, pCreateInfo, pAllocator, pPipelineCache);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreatePipelineCache(dump_inst, result, device, pCreateInfo, pAllocator, pPipelineCache);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateEvent(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateEvent(dump_inst, result, device, pCreateInfo, pAllocator, pEvent);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateEvent(dump_inst, result, device, pCreateInfo, pAllocator, pEvent);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateEvent(dump_inst, result, device, pCreateInfo, pAllocator, pEvent);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetSwapchainImagesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetSwapchainImagesKHR(dump_inst, result, device, swapchain, pSwapchainImageCount, pSwapchainImages);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetSwapchainImagesKHR(dump_inst, result, device, swapchain, pSwapchainImageCount, pSwapchainImages);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetSwapchainImagesKHR(dump_inst, result, device, swapchain, pSwapchainImageCount, pSwapchainImages);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetEventStatus(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkEvent event)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetEventStatus(dump_inst, result, device, event);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetEventStatus(dump_inst, result, device, event);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetEventStatus(dump_inst, result, device, event);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateImage(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateImage(dump_inst, result, device, pCreateInfo, pAllocator, pImage);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateImage(dump_inst, result, device, pCreateInfo, pAllocator, pImage);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateImage(dump_inst, result, device, pCreateInfo, pAllocator, pImage);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPipelineCacheData(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPipelineCacheData(dump_inst, result, device, pipelineCache, pDataSize, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPipelineCacheData(dump_inst, result, device, pipelineCache, pDataSize, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPipelineCacheData(dump_inst, result, device, pipelineCache, pDataSize, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkSetEvent(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkEvent event)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkSetEvent(dump_inst, result, device, event);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkSetEvent(dump_inst, result, device, event);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkSetEvent(dump_inst, result, device, event);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkMergePipelineCaches(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkMergePipelineCaches(dump_inst, result, device, dstCache, srcCacheCount, pSrcCaches);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkMergePipelineCaches(dump_inst, result, device, dstCache, srcCacheCount, pSrcCaches);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkMergePipelineCaches(dump_inst, result, device, dstCache, srcCacheCount, pSrcCaches);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkResetEvent(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkEvent event)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkResetEvent(dump_inst, result, device, event);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkResetEvent(dump_inst, result, device, event);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkResetEvent(dump_inst, result, device, event);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateGraphicsPipelines(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateGraphicsPipelines(dump_inst, result, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateGraphicsPipelines(dump_inst, result, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateGraphicsPipelines(dump_inst, result, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetMemoryHostPointerPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetMemoryHostPointerPropertiesEXT(dump_inst, result, device, handleType, pHostPointer, pMemoryHostPointerProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetMemoryHostPointerPropertiesEXT(dump_inst, result, device, handleType, pHostPointer, pMemoryHostPointerProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetMemoryHostPointerPropertiesEXT(dump_inst, result, device, handleType, pHostPointer, pMemoryHostPointerProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateQueryPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateQueryPool(dump_inst, result, device, pCreateInfo, pAllocator, pQueryPool);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateQueryPool(dump_inst, result, device, pCreateInfo, pAllocator, pQueryPool);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateQueryPool(dump_inst, result, device, pCreateInfo, pAllocator, pQueryPool);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkAcquireNextImageKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkAcquireNextImageKHR(dump_inst, result, device, swapchain, timeout, semaphore, fence, pImageIndex);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkAcquireNextImageKHR(dump_inst, result, device, swapchain, timeout, semaphore, fence, pImageIndex);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkAcquireNextImageKHR(dump_inst, result, device, swapchain, timeout, semaphore, fence, pImageIndex);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkQueuePresentKHR(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkQueuePresentKHR(dump_inst, result, queue, pPresentInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkQueuePresentKHR(dump_inst, result, queue, pPresentInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkQueuePresentKHR(dump_inst, result, queue, pPresentInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateComputePipelines(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateComputePipelines(dump_inst, result, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateComputePipelines(dump_inst, result, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateComputePipelines(dump_inst, result, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetDisplayPlaneSupportedDisplaysKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetDisplayPlaneSupportedDisplaysKHR(dump_inst, result, physicalDevice, planeIndex, pDisplayCount, pDisplays);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetDisplayPlaneSupportedDisplaysKHR(dump_inst, result, physicalDevice, planeIndex, pDisplayCount, pDisplays);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetDisplayPlaneSupportedDisplaysKHR(dump_inst, result, physicalDevice, planeIndex, pDisplayCount, pDisplays);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetCalibratedTimestampsEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetCalibratedTimestampsEXT(dump_inst, result, device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetCalibratedTimestampsEXT(dump_inst, result, device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetCalibratedTimestampsEXT(dump_inst, result, device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateImageView(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateImageView(dump_inst, result, device, pCreateInfo, pAllocator, pView);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateImageView(dump_inst, result, device, pCreateInfo, pAllocator, pView);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateImageView(dump_inst, result, device, pCreateInfo, pAllocator, pView);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceDisplayPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceDisplayPropertiesKHR(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceDisplayPropertiesKHR(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceDisplayPropertiesKHR(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(dump_inst, result, physicalDevice, pTimeDomainCount, pTimeDomains);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(dump_inst, result, physicalDevice, pTimeDomainCount, pTimeDomains);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(dump_inst, result, physicalDevice, pTimeDomainCount, pTimeDomains);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetDisplayModePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetDisplayModePropertiesKHR(dump_inst, result, physicalDevice, display, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetDisplayModePropertiesKHR(dump_inst, result, physicalDevice, display, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetDisplayModePropertiesKHR(dump_inst, result, physicalDevice, display, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateDisplayModeKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateDisplayModeKHR(dump_inst, result, physicalDevice, display, pCreateInfo, pAllocator, pMode);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateDisplayModeKHR(dump_inst, result, physicalDevice, display, pCreateInfo, pAllocator, pMode);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateDisplayModeKHR(dump_inst, result, physicalDevice, display, pCreateInfo, pAllocator, pMode);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreatePipelineLayout(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreatePipelineLayout(dump_inst, result, device, pCreateInfo, pAllocator, pPipelineLayout);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreatePipelineLayout(dump_inst, result, device, pCreateInfo, pAllocator, pPipelineLayout);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreatePipelineLayout(dump_inst, result, device, pCreateInfo, pAllocator, pPipelineLayout);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetDisplayPlaneCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetDisplayPlaneCapabilitiesKHR(dump_inst, result, physicalDevice, mode, planeIndex, pCapabilities);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetDisplayPlaneCapabilitiesKHR(dump_inst, result, physicalDevice, mode, planeIndex, pCapabilities);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetDisplayPlaneCapabilitiesKHR(dump_inst, result, physicalDevice, mode, planeIndex, pCapabilities);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(dump_inst, result, physicalDevice, pCombinationCount, pCombinations);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(dump_inst, result, physicalDevice, pCombinationCount, pCombinations);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(dump_inst, result, physicalDevice, pCombinationCount, pCombinations);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkBindAccelerationStructureMemoryNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkBindAccelerationStructureMemoryNV(dump_inst, result, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkBindAccelerationStructureMemoryNV(dump_inst, result, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkBindAccelerationStructureMemoryNV(dump_inst, result, device, bindInfoCount, pBindInfos);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateDisplayPlaneSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateDisplayPlaneSurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateDisplayPlaneSurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateDisplayPlaneSurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkResetCommandBuffer(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkResetCommandBuffer(dump_inst, result, commandBuffer, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkResetCommandBuffer(dump_inst, result, commandBuffer, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkResetCommandBuffer(dump_inst, result, commandBuffer, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetImageViewHandleNVX(ApiDumpInstance& dump_inst, uint32_t result, VkDevice device, const VkImageViewHandleInfoNVX* pInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetImageViewHandleNVX(dump_inst, result, device, pInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetImageViewHandleNVX(dump_inst, result, device, pInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetImageViewHandleNVX(dump_inst, result, device, pInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkInitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkInitializePerformanceApiINTEL(dump_inst, result, device, pInitializeInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkInitializePerformanceApiINTEL(dump_inst, result, device, pInitializeInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkInitializePerformanceApiINTEL(dump_inst, result, device, pInitializeInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateSampler(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateSampler(dump_inst, result, device, pCreateInfo, pAllocator, pSampler);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateSampler(dump_inst, result, device, pCreateInfo, pAllocator, pSampler);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateSampler(dump_inst, result, device, pCreateInfo, pAllocator, pSampler);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetPerformanceMarkerINTEL(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetPerformanceMarkerINTEL(dump_inst, result, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetPerformanceMarkerINTEL(dump_inst, result, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetPerformanceMarkerINTEL(dump_inst, result, commandBuffer, pMarkerInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetPerformanceStreamMarkerINTEL(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetPerformanceStreamMarkerINTEL(dump_inst, result, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetPerformanceStreamMarkerINTEL(dump_inst, result, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetPerformanceStreamMarkerINTEL(dump_inst, result, commandBuffer, pMarkerInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateRayTracingPipelinesNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateRayTracingPipelinesNV(dump_inst, result, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateRayTracingPipelinesNV(dump_inst, result, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateRayTracingPipelinesNV(dump_inst, result, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetPerformanceOverrideINTEL(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetPerformanceOverrideINTEL(dump_inst, result, commandBuffer, pOverrideInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetPerformanceOverrideINTEL(dump_inst, result, commandBuffer, pOverrideInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetPerformanceOverrideINTEL(dump_inst, result, commandBuffer, pOverrideInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkReleasePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPerformanceConfigurationINTEL configuration)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkReleasePerformanceConfigurationINTEL(dump_inst, result, device, configuration);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkReleasePerformanceConfigurationINTEL(dump_inst, result, device, configuration);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkReleasePerformanceConfigurationINTEL(dump_inst, result, device, configuration);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_body_vkGetPhysicalDeviceSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceSurfacePresentModes2EXT(dump_inst, result, physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceSurfacePresentModes2EXT(dump_inst, result, physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceSurfacePresentModes2EXT(dump_inst, result, physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_body_vkAcquirePerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkAcquirePerformanceConfigurationINTEL(dump_inst, result, device, pAcquireInfo, pConfiguration);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkAcquirePerformanceConfigurationINTEL(dump_inst, result, device, pAcquireInfo, pConfiguration);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkAcquirePerformanceConfigurationINTEL(dump_inst, result, device, pAcquireInfo, pConfiguration);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetBufferDeviceAddressEXT(ApiDumpInstance& dump_inst, VkDeviceAddress result, VkDevice device, const VkBufferDeviceAddressInfoEXT* pInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetBufferDeviceAddressEXT(dump_inst, result, device, pInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetBufferDeviceAddressEXT(dump_inst, result, device, pInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetBufferDeviceAddressEXT(dump_inst, result, device, pInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateSharedSwapchainsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateSharedSwapchainsKHR(dump_inst, result, device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateSharedSwapchainsKHR(dump_inst, result, device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateSharedSwapchainsKHR(dump_inst, result, device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkQueueSetPerformanceConfigurationINTEL(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, VkPerformanceConfigurationINTEL configuration)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkQueueSetPerformanceConfigurationINTEL(dump_inst, result, queue, configuration);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkQueueSetPerformanceConfigurationINTEL(dump_inst, result, queue, configuration);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkQueueSetPerformanceConfigurationINTEL(dump_inst, result, queue, configuration);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_body_vkReleaseFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkReleaseFullScreenExclusiveModeEXT(dump_inst, result, device, swapchain);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkReleaseFullScreenExclusiveModeEXT(dump_inst, result, device, swapchain);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkReleaseFullScreenExclusiveModeEXT(dump_inst, result, device, swapchain);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_body_vkAcquireFullScreenExclusiveModeEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkAcquireFullScreenExclusiveModeEXT(dump_inst, result, device, swapchain);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkAcquireFullScreenExclusiveModeEXT(dump_inst, result, device, swapchain);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkAcquireFullScreenExclusiveModeEXT(dump_inst, result, device, swapchain);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_body_vkGetRayTracingShaderGroupHandlesNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetRayTracingShaderGroupHandlesNV(dump_inst, result, device, pipeline, firstGroup, groupCount, dataSize, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetRayTracingShaderGroupHandlesNV(dump_inst, result, device, pipeline, firstGroup, groupCount, dataSize, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetRayTracingShaderGroupHandlesNV(dump_inst, result, device, pipeline, firstGroup, groupCount, dataSize, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPerformanceParameterINTEL(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPerformanceParameterINTEL(dump_inst, result, device, parameter, pValue);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPerformanceParameterINTEL(dump_inst, result, device, parameter, pValue);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPerformanceParameterINTEL(dump_inst, result, device, parameter, pValue);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetAccelerationStructureHandleNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetAccelerationStructureHandleNV(dump_inst, result, device, accelerationStructure, dataSize, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetAccelerationStructureHandleNV(dump_inst, result, device, accelerationStructure, dataSize, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetAccelerationStructureHandleNV(dump_inst, result, device, accelerationStructure, dataSize, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_body_vkGetDeviceGroupSurfacePresentModes2EXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetDeviceGroupSurfacePresentModes2EXT(dump_inst, result, device, pSurfaceInfo, pModes);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetDeviceGroupSurfacePresentModes2EXT(dump_inst, result, device, pSurfaceInfo, pModes);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetDeviceGroupSurfacePresentModes2EXT(dump_inst, result, device, pSurfaceInfo, pModes);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_body_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(dump_inst, result, physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(dump_inst, result, physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(dump_inst, result, physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
inline void dump_body_vkCreateWaylandSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateWaylandSurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateWaylandSurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateWaylandSurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_body_vkGetMemoryWin32HandleNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetMemoryWin32HandleNV(dump_inst, result, device, memory, handleType, pHandle);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetMemoryWin32HandleNV(dump_inst, result, device, memory, handleType, pHandle);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetMemoryWin32HandleNV(dump_inst, result, device, memory, handleType, pHandle);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
inline void dump_body_vkGetPhysicalDeviceWaylandPresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceWaylandPresentationSupportKHR(dump_inst, result, physicalDevice, queueFamilyIndex, display);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceWaylandPresentationSupportKHR(dump_inst, result, physicalDevice, queueFamilyIndex, display);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceWaylandPresentationSupportKHR(dump_inst, result, physicalDevice, queueFamilyIndex, display);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WAYLAND_KHR
inline void dump_body_vkDebugMarkerSetObjectTagEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDebugMarkerSetObjectTagEXT(dump_inst, result, device, pTagInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDebugMarkerSetObjectTagEXT(dump_inst, result, device, pTagInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDebugMarkerSetObjectTagEXT(dump_inst, result, device, pTagInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkEnumeratePhysicalDeviceGroupsKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkEnumeratePhysicalDeviceGroupsKHR(dump_inst, result, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkEnumeratePhysicalDeviceGroupsKHR(dump_inst, result, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkEnumeratePhysicalDeviceGroupsKHR(dump_inst, result, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateIndirectCommandsLayoutNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkIndirectCommandsLayoutCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNVX* pIndirectCommandsLayout)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateIndirectCommandsLayoutNVX(dump_inst, result, device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateIndirectCommandsLayoutNVX(dump_inst, result, device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateIndirectCommandsLayoutNVX(dump_inst, result, device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateObjectTableNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkObjectTableCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkObjectTableNVX* pObjectTable)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateObjectTableNVX(dump_inst, result, device, pCreateInfo, pAllocator, pObjectTable);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateObjectTableNVX(dump_inst, result, device, pCreateInfo, pAllocator, pObjectTable);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateObjectTableNVX(dump_inst, result, device, pCreateInfo, pAllocator, pObjectTable);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkRegisterObjectsNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkObjectTableNVX objectTable, uint32_t objectCount, const VkObjectTableEntryNVX* const*    ppObjectTableEntries, const uint32_t* pObjectIndices)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkRegisterObjectsNVX(dump_inst, result, device, objectTable, objectCount, ppObjectTableEntries, pObjectIndices);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkRegisterObjectsNVX(dump_inst, result, device, objectTable, objectCount, ppObjectTableEntries, pObjectIndices);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkRegisterObjectsNVX(dump_inst, result, device, objectTable, objectCount, ppObjectTableEntries, pObjectIndices);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_XCB_KHR)
inline void dump_body_vkCreateXcbSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateXcbSurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateXcbSurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateXcbSurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_XCB_KHR
inline void dump_body_vkUnregisterObjectsNVX(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkObjectTableNVX objectTable, uint32_t objectCount, const VkObjectEntryTypeNVX* pObjectEntryTypes, const uint32_t* pObjectIndices)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkUnregisterObjectsNVX(dump_inst, result, device, objectTable, objectCount, pObjectEntryTypes, pObjectIndices);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkUnregisterObjectsNVX(dump_inst, result, device, objectTable, objectCount, pObjectEntryTypes, pObjectIndices);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkUnregisterObjectsNVX(dump_inst, result, device, objectTable, objectCount, pObjectEntryTypes, pObjectIndices);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateAccelerationStructureNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateAccelerationStructureNV(dump_inst, result, device, pCreateInfo, pAllocator, pAccelerationStructure);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateAccelerationStructureNV(dump_inst, result, device, pCreateInfo, pAllocator, pAccelerationStructure);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateAccelerationStructureNV(dump_inst, result, device, pCreateInfo, pAllocator, pAccelerationStructure);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPipelineExecutablePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPipelineExecutablePropertiesKHR(dump_inst, result, device, pPipelineInfo, pExecutableCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPipelineExecutablePropertiesKHR(dump_inst, result, device, pPipelineInfo, pExecutableCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPipelineExecutablePropertiesKHR(dump_inst, result, device, pPipelineInfo, pExecutableCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkBindBufferMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkBindBufferMemory2(dump_inst, result, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkBindBufferMemory2(dump_inst, result, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkBindBufferMemory2(dump_inst, result, device, bindInfoCount, pBindInfos);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateHeadlessSurfaceEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateHeadlessSurfaceEXT(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateHeadlessSurfaceEXT(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateHeadlessSurfaceEXT(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetDisplayModeProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetDisplayModeProperties2KHR(dump_inst, result, physicalDevice, display, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetDisplayModeProperties2KHR(dump_inst, result, physicalDevice, display, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetDisplayModeProperties2KHR(dump_inst, result, physicalDevice, display, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetShaderInfoAMD(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetShaderInfoAMD(dump_inst, result, device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetShaderInfoAMD(dump_inst, result, device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetShaderInfoAMD(dump_inst, result, device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateDebugUtilsMessengerEXT(dump_inst, result, instance, pCreateInfo, pAllocator, pMessenger);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateDebugUtilsMessengerEXT(dump_inst, result, instance, pCreateInfo, pAllocator, pMessenger);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateDebugUtilsMessengerEXT(dump_inst, result, instance, pCreateInfo, pAllocator, pMessenger);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCompileDeferredNV(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkPipeline pipeline, uint32_t shader)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCompileDeferredNV(dump_inst, result, device, pipeline, shader);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCompileDeferredNV(dump_inst, result, device, pipeline, shader);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCompileDeferredNV(dump_inst, result, device, pipeline, shader);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkReleaseDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkReleaseDisplayEXT(dump_inst, result, physicalDevice, display);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkReleaseDisplayEXT(dump_inst, result, physicalDevice, display);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkReleaseDisplayEXT(dump_inst, result, physicalDevice, display);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceDisplayProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceDisplayProperties2KHR(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceDisplayProperties2KHR(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceDisplayProperties2KHR(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_FUCHSIA)
inline void dump_body_vkCreateImagePipeSurfaceFUCHSIA(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateImagePipeSurfaceFUCHSIA(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateImagePipeSurfaceFUCHSIA(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateImagePipeSurfaceFUCHSIA(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_FUCHSIA
inline void dump_body_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkRegisterDeviceEventEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkRegisterDeviceEventEXT(dump_inst, result, device, pDeviceEventInfo, pAllocator, pFence);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkRegisterDeviceEventEXT(dump_inst, result, device, pDeviceEventInfo, pAllocator, pFence);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkRegisterDeviceEventEXT(dump_inst, result, device, pDeviceEventInfo, pAllocator, pFence);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
inline void dump_body_vkCreateAndroidSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateAndroidSurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateAndroidSurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateAndroidSurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_ANDROID_KHR
inline void dump_body_vkGetImageDrmFormatModifierPropertiesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetImageDrmFormatModifierPropertiesEXT(dump_inst, result, device, image, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetImageDrmFormatModifierPropertiesEXT(dump_inst, result, device, image, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetImageDrmFormatModifierPropertiesEXT(dump_inst, result, device, image, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_XLIB_KHR)
inline void dump_body_vkCreateXlibSurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateXlibSurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateXlibSurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateXlibSurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_XLIB_KHR
inline void dump_body_vkDisplayPowerControlEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDisplayPowerControlEXT(dump_inst, result, device, display, pDisplayPowerInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDisplayPowerControlEXT(dump_inst, result, device, display, pDisplayPowerInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDisplayPowerControlEXT(dump_inst, result, device, display, pDisplayPowerInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateDescriptorSetLayout(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateDescriptorSetLayout(dump_inst, result, device, pCreateInfo, pAllocator, pSetLayout);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateDescriptorSetLayout(dump_inst, result, device, pCreateInfo, pAllocator, pSetLayout);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateDescriptorSetLayout(dump_inst, result, device, pCreateInfo, pAllocator, pSetLayout);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateDescriptorUpdateTemplateKHR(dump_inst, result, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateDescriptorUpdateTemplateKHR(dump_inst, result, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateDescriptorUpdateTemplateKHR(dump_inst, result, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetDisplayPlaneCapabilities2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetDisplayPlaneCapabilities2KHR(dump_inst, result, physicalDevice, pDisplayPlaneInfo, pCapabilities);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetDisplayPlaneCapabilities2KHR(dump_inst, result, physicalDevice, pDisplayPlaneInfo, pCapabilities);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetDisplayPlaneCapabilities2KHR(dump_inst, result, physicalDevice, pDisplayPlaneInfo, pCapabilities);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkRegisterDisplayEventEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkRegisterDisplayEventEXT(dump_inst, result, device, display, pDisplayEventInfo, pAllocator, pFence);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkRegisterDisplayEventEXT(dump_inst, result, device, display, pDisplayEventInfo, pAllocator, pFence);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkRegisterDisplayEventEXT(dump_inst, result, device, display, pDisplayEventInfo, pAllocator, pFence);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_METAL_EXT)
inline void dump_body_vkCreateMetalSurfaceEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateMetalSurfaceEXT(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateMetalSurfaceEXT(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateMetalSurfaceEXT(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
inline void dump_body_vkGetRandROutputDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetRandROutputDisplayEXT(dump_inst, result, physicalDevice, dpy, rrOutput, pDisplay);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetRandROutputDisplayEXT(dump_inst, result, physicalDevice, dpy, rrOutput, pDisplay);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetRandROutputDisplayEXT(dump_inst, result, physicalDevice, dpy, rrOutput, pDisplay);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#if defined(VK_USE_PLATFORM_XLIB_KHR)
inline void dump_body_vkGetPhysicalDeviceXlibPresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceXlibPresentationSupportKHR(dump_inst, result, physicalDevice, queueFamilyIndex, dpy, visualID);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceXlibPresentationSupportKHR(dump_inst, result, physicalDevice, queueFamilyIndex, dpy, visualID);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceXlibPresentationSupportKHR(dump_inst, result, physicalDevice, queueFamilyIndex, dpy, visualID);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_XLIB_KHR
inline void dump_body_vkGetSwapchainCounterEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetSwapchainCounterEXT(dump_inst, result, device, swapchain, counter, pCounterValue);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetSwapchainCounterEXT(dump_inst, result, device, swapchain, counter, pCounterValue);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetSwapchainCounterEXT(dump_inst, result, device, swapchain, counter, pCounterValue);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_IOS_MVK)
inline void dump_body_vkCreateIOSSurfaceMVK(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateIOSSurfaceMVK(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateIOSSurfaceMVK(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateIOSSurfaceMVK(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_IOS_MVK
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
inline void dump_body_vkAcquireXlibDisplayEXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkAcquireXlibDisplayEXT(dump_inst, result, physicalDevice, dpy, display);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkAcquireXlibDisplayEXT(dump_inst, result, physicalDevice, dpy, display);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkAcquireXlibDisplayEXT(dump_inst, result, physicalDevice, dpy, display);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
inline void dump_body_vkGetPhysicalDeviceSurfaceSupportKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceSurfaceSupportKHR(dump_inst, result, physicalDevice, queueFamilyIndex, surface, pSupported);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceSurfaceSupportKHR(dump_inst, result, physicalDevice, queueFamilyIndex, surface, pSupported);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceSurfaceSupportKHR(dump_inst, result, physicalDevice, queueFamilyIndex, surface, pSupported);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateSamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateSamplerYcbcrConversionKHR(dump_inst, result, device, pCreateInfo, pAllocator, pYcbcrConversion);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateSamplerYcbcrConversionKHR(dump_inst, result, device, pCreateInfo, pAllocator, pYcbcrConversion);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateSamplerYcbcrConversionKHR(dump_inst, result, device, pCreateInfo, pAllocator, pYcbcrConversion);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_GGP)
inline void dump_body_vkCreateStreamDescriptorSurfaceGGP(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateStreamDescriptorSurfaceGGP(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateStreamDescriptorSurfaceGGP(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateStreamDescriptorSurfaceGGP(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_GGP
inline void dump_body_vkGetPastPresentationTimingGOOGLE(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPastPresentationTimingGOOGLE(dump_inst, result, device, swapchain, pPresentationTimingCount, pPresentationTimings);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPastPresentationTimingGOOGLE(dump_inst, result, device, swapchain, pPresentationTimingCount, pPresentationTimings);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPastPresentationTimingGOOGLE(dump_inst, result, device, swapchain, pPresentationTimingCount, pPresentationTimings);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateSwapchainKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateSwapchainKHR(dump_inst, result, device, pCreateInfo, pAllocator, pSwapchain);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateSwapchainKHR(dump_inst, result, device, pCreateInfo, pAllocator, pSwapchain);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateSwapchainKHR(dump_inst, result, device, pCreateInfo, pAllocator, pSwapchain);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_MACOS_MVK)
inline void dump_body_vkCreateMacOSSurfaceMVK(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateMacOSSurfaceMVK(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateMacOSSurfaceMVK(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateMacOSSurfaceMVK(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_MACOS_MVK
inline void dump_body_vkMergeValidationCachesEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkMergeValidationCachesEXT(dump_inst, result, device, dstCache, srcCacheCount, pSrcCaches);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkMergeValidationCachesEXT(dump_inst, result, device, dstCache, srcCacheCount, pSrcCaches);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkMergeValidationCachesEXT(dump_inst, result, device, dstCache, srcCacheCount, pSrcCaches);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateValidationCacheEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateValidationCacheEXT(dump_inst, result, device, pCreateInfo, pAllocator, pValidationCache);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateValidationCacheEXT(dump_inst, result, device, pCreateInfo, pAllocator, pValidationCache);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateValidationCacheEXT(dump_inst, result, device, pCreateInfo, pAllocator, pValidationCache);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceSurfaceFormatsKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceSurfaceFormatsKHR(dump_inst, result, physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceSurfaceFormatsKHR(dump_inst, result, physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceSurfaceFormatsKHR(dump_inst, result, physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceSurfaceCapabilities2EXT(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceSurfaceCapabilities2EXT(dump_inst, result, physicalDevice, surface, pSurfaceCapabilities);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceSurfaceCapabilities2EXT(dump_inst, result, physicalDevice, surface, pSurfaceCapabilities);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceSurfaceCapabilities2EXT(dump_inst, result, physicalDevice, surface, pSurfaceCapabilities);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkBindBufferMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkBindBufferMemory2KHR(dump_inst, result, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkBindBufferMemory2KHR(dump_inst, result, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkBindBufferMemory2KHR(dump_inst, result, device, bindInfoCount, pBindInfos);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetValidationCacheDataEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetValidationCacheDataEXT(dump_inst, result, device, validationCache, pDataSize, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetValidationCacheDataEXT(dump_inst, result, device, validationCache, pDataSize, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetValidationCacheDataEXT(dump_inst, result, device, validationCache, pDataSize, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dump_inst, result, physicalDevice, surface, pSurfaceCapabilities);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dump_inst, result, physicalDevice, surface, pSurfaceCapabilities);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dump_inst, result, physicalDevice, surface, pSurfaceCapabilities);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkBindImageMemory2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkBindImageMemory2KHR(dump_inst, result, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkBindImageMemory2KHR(dump_inst, result, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkBindImageMemory2KHR(dump_inst, result, device, bindInfoCount, pBindInfos);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
inline void dump_body_vkGetAndroidHardwareBufferPropertiesANDROID(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetAndroidHardwareBufferPropertiesANDROID(dump_inst, result, device, buffer, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetAndroidHardwareBufferPropertiesANDROID(dump_inst, result, device, buffer, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetAndroidHardwareBufferPropertiesANDROID(dump_inst, result, device, buffer, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_ANDROID_KHR
inline void dump_body_vkGetRefreshCycleDurationGOOGLE(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetRefreshCycleDurationGOOGLE(dump_inst, result, device, swapchain, pDisplayTimingProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetRefreshCycleDurationGOOGLE(dump_inst, result, device, swapchain, pDisplayTimingProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetRefreshCycleDurationGOOGLE(dump_inst, result, device, swapchain, pDisplayTimingProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
inline void dump_body_vkGetMemoryAndroidHardwareBufferANDROID(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetMemoryAndroidHardwareBufferANDROID(dump_inst, result, device, pInfo, pBuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetMemoryAndroidHardwareBufferANDROID(dump_inst, result, device, pInfo, pBuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetMemoryAndroidHardwareBufferANDROID(dump_inst, result, device, pInfo, pBuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_ANDROID_KHR
inline void dump_body_vkCreateDescriptorPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateDescriptorPool(dump_inst, result, device, pCreateInfo, pAllocator, pDescriptorPool);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateDescriptorPool(dump_inst, result, device, pCreateInfo, pAllocator, pDescriptorPool);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateDescriptorPool(dump_inst, result, device, pCreateInfo, pAllocator, pDescriptorPool);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceSurfaceFormats2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceSurfaceFormats2KHR(dump_inst, result, physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceSurfaceFormats2KHR(dump_inst, result, physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceSurfaceFormats2KHR(dump_inst, result, physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceSurfaceCapabilities2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceSurfaceCapabilities2KHR(dump_inst, result, physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceSurfaceCapabilities2KHR(dump_inst, result, physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceSurfaceCapabilities2KHR(dump_inst, result, physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkFreeDescriptorSets(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkFreeDescriptorSets(dump_inst, result, device, descriptorPool, descriptorSetCount, pDescriptorSets);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkFreeDescriptorSets(dump_inst, result, device, descriptorPool, descriptorSetCount, pDescriptorSets);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkFreeDescriptorSets(dump_inst, result, device, descriptorPool, descriptorSetCount, pDescriptorSets);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceImageFormatProperties2(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceImageFormatProperties2(dump_inst, result, physicalDevice, pImageFormatInfo, pImageFormatProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceImageFormatProperties2(dump_inst, result, physicalDevice, pImageFormatInfo, pImageFormatProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceImageFormatProperties2(dump_inst, result, physicalDevice, pImageFormatInfo, pImageFormatProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkResetDescriptorPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkResetDescriptorPool(dump_inst, result, device, descriptorPool, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkResetDescriptorPool(dump_inst, result, device, descriptorPool, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkResetDescriptorPool(dump_inst, result, device, descriptorPool, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkAllocateDescriptorSets(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkAllocateDescriptorSets(dump_inst, result, device, pAllocateInfo, pDescriptorSets);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkAllocateDescriptorSets(dump_inst, result, device, pAllocateInfo, pDescriptorSets);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkAllocateDescriptorSets(dump_inst, result, device, pAllocateInfo, pDescriptorSets);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkSetDebugUtilsObjectTagEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkSetDebugUtilsObjectTagEXT(dump_inst, result, device, pTagInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkSetDebugUtilsObjectTagEXT(dump_inst, result, device, pTagInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkSetDebugUtilsObjectTagEXT(dump_inst, result, device, pTagInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_XCB_KHR)
inline void dump_body_vkGetPhysicalDeviceXcbPresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceXcbPresentationSupportKHR(dump_inst, result, physicalDevice, queueFamilyIndex, connection, visual_id);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceXcbPresentationSupportKHR(dump_inst, result, physicalDevice, queueFamilyIndex, connection, visual_id);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceXcbPresentationSupportKHR(dump_inst, result, physicalDevice, queueFamilyIndex, connection, visual_id);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_XCB_KHR
inline void dump_body_vkQueueSubmit(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkQueueSubmit(dump_inst, result, queue, submitCount, pSubmits, fence);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkQueueSubmit(dump_inst, result, queue, submitCount, pSubmits, fence);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkQueueSubmit(dump_inst, result, queue, submitCount, pSubmits, fence);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateDevice(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateDevice(dump_inst, result, physicalDevice, pCreateInfo, pAllocator, pDevice);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateDevice(dump_inst, result, physicalDevice, pCreateInfo, pAllocator, pDevice);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateDevice(dump_inst, result, physicalDevice, pCreateInfo, pAllocator, pDevice);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateCommandPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateCommandPool(dump_inst, result, device, pCreateInfo, pAllocator, pCommandPool);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateCommandPool(dump_inst, result, device, pCreateInfo, pAllocator, pCommandPool);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateCommandPool(dump_inst, result, device, pCreateInfo, pAllocator, pCommandPool);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_body_vkImportSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkImportSemaphoreWin32HandleKHR(dump_inst, result, device, pImportSemaphoreWin32HandleInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkImportSemaphoreWin32HandleKHR(dump_inst, result, device, pImportSemaphoreWin32HandleInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkImportSemaphoreWin32HandleKHR(dump_inst, result, device, pImportSemaphoreWin32HandleInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_body_vkGetPhysicalDeviceSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceSurfacePresentModesKHR(dump_inst, result, physicalDevice, surface, pPresentModeCount, pPresentModes);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceSurfacePresentModesKHR(dump_inst, result, physicalDevice, surface, pPresentModeCount, pPresentModes);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceSurfacePresentModesKHR(dump_inst, result, physicalDevice, surface, pPresentModeCount, pPresentModes);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceImageFormatProperties2KHR(dump_inst, result, physicalDevice, pImageFormatInfo, pImageFormatProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceImageFormatProperties2KHR(dump_inst, result, physicalDevice, pImageFormatInfo, pImageFormatProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceImageFormatProperties2KHR(dump_inst, result, physicalDevice, pImageFormatInfo, pImageFormatProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkQueueBindSparse(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkQueueBindSparse(dump_inst, result, queue, bindInfoCount, pBindInfo, fence);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkQueueBindSparse(dump_inst, result, queue, bindInfoCount, pBindInfo, fence);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkQueueBindSparse(dump_inst, result, queue, bindInfoCount, pBindInfo, fence);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_body_vkGetSemaphoreWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetSemaphoreWin32HandleKHR(dump_inst, result, device, pGetWin32HandleInfo, pHandle);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetSemaphoreWin32HandleKHR(dump_inst, result, device, pGetWin32HandleInfo, pHandle);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetSemaphoreWin32HandleKHR(dump_inst, result, device, pGetWin32HandleInfo, pHandle);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_body_vkGetDeviceGroupPresentCapabilitiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetDeviceGroupPresentCapabilitiesKHR(dump_inst, result, device, pDeviceGroupPresentCapabilities);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetDeviceGroupPresentCapabilitiesKHR(dump_inst, result, device, pDeviceGroupPresentCapabilities);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetDeviceGroupPresentCapabilitiesKHR(dump_inst, result, device, pDeviceGroupPresentCapabilities);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkImportSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkImportSemaphoreFdKHR(dump_inst, result, device, pImportSemaphoreFdInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkImportSemaphoreFdKHR(dump_inst, result, device, pImportSemaphoreFdInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkImportSemaphoreFdKHR(dump_inst, result, device, pImportSemaphoreFdInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDevicePresentRectanglesKHR(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDevicePresentRectanglesKHR(dump_inst, result, physicalDevice, surface, pRectCount, pRects);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDevicePresentRectanglesKHR(dump_inst, result, physicalDevice, surface, pRectCount, pRects);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDevicePresentRectanglesKHR(dump_inst, result, physicalDevice, surface, pRectCount, pRects);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateSamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateSamplerYcbcrConversion(dump_inst, result, device, pCreateInfo, pAllocator, pYcbcrConversion);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateSamplerYcbcrConversion(dump_inst, result, device, pCreateInfo, pAllocator, pYcbcrConversion);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateSamplerYcbcrConversion(dump_inst, result, device, pCreateInfo, pAllocator, pYcbcrConversion);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkResetCommandPool(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkResetCommandPool(dump_inst, result, device, commandPool, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkResetCommandPool(dump_inst, result, device, commandPool, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkResetCommandPool(dump_inst, result, device, commandPool, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetDeviceGroupSurfacePresentModesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetDeviceGroupSurfacePresentModesKHR(dump_inst, result, device, surface, pModes);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetDeviceGroupSurfacePresentModesKHR(dump_inst, result, device, surface, pModes);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetDeviceGroupSurfacePresentModesKHR(dump_inst, result, device, surface, pModes);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkEnumerateDeviceExtensionProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkEnumerateDeviceExtensionProperties(dump_inst, result, physicalDevice, pLayerName, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkEnumerateDeviceExtensionProperties(dump_inst, result, physicalDevice, pLayerName, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkEnumerateDeviceExtensionProperties(dump_inst, result, physicalDevice, pLayerName, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkEnumerateInstanceExtensionProperties(ApiDumpInstance& dump_inst, VkResult result, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkEnumerateInstanceExtensionProperties(dump_inst, result, pLayerName, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkEnumerateInstanceExtensionProperties(dump_inst, result, pLayerName, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkEnumerateInstanceExtensionProperties(dump_inst, result, pLayerName, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkAllocateCommandBuffers(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkAllocateCommandBuffers(dump_inst, result, device, pAllocateInfo, pCommandBuffers);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkAllocateCommandBuffers(dump_inst, result, device, pAllocateInfo, pCommandBuffers);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkAllocateCommandBuffers(dump_inst, result, device, pAllocateInfo, pCommandBuffers);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkAcquireNextImage2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkAcquireNextImage2KHR(dump_inst, result, device, pAcquireInfo, pImageIndex);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkAcquireNextImage2KHR(dump_inst, result, device, pAcquireInfo, pImageIndex);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkAcquireNextImage2KHR(dump_inst, result, device, pAcquireInfo, pImageIndex);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkEnumeratePhysicalDeviceGroups(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkEnumeratePhysicalDeviceGroups(dump_inst, result, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkEnumeratePhysicalDeviceGroups(dump_inst, result, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkEnumeratePhysicalDeviceGroups(dump_inst, result, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkQueueWaitIdle(ApiDumpInstance& dump_inst, VkResult result, VkQueue queue)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkQueueWaitIdle(dump_inst, result, queue);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkQueueWaitIdle(dump_inst, result, queue);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkQueueWaitIdle(dump_inst, result, queue);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_body_vkCreateWin32SurfaceKHR(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateWin32SurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateWin32SurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateWin32SurfaceKHR(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_body_vkGetSemaphoreFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetSemaphoreFdKHR(dump_inst, result, device, pGetFdInfo, pFd);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetSemaphoreFdKHR(dump_inst, result, device, pGetFdInfo, pFd);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetSemaphoreFdKHR(dump_inst, result, device, pGetFdInfo, pFd);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkEnumerateInstanceLayerProperties(ApiDumpInstance& dump_inst, VkResult result, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkEnumerateInstanceLayerProperties(dump_inst, result, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkEnumerateInstanceLayerProperties(dump_inst, result, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkEnumerateInstanceLayerProperties(dump_inst, result, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkEnumerateDeviceLayerProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkEnumerateDeviceLayerProperties(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkEnumerateDeviceLayerProperties(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkEnumerateDeviceLayerProperties(dump_inst, result, physicalDevice, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkBeginCommandBuffer(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkBeginCommandBuffer(dump_inst, result, commandBuffer, pBeginInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkBeginCommandBuffer(dump_inst, result, commandBuffer, pBeginInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkBeginCommandBuffer(dump_inst, result, commandBuffer, pBeginInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDeviceWaitIdle(ApiDumpInstance& dump_inst, VkResult result, VkDevice device)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDeviceWaitIdle(dump_inst, result, device);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDeviceWaitIdle(dump_inst, result, device);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDeviceWaitIdle(dump_inst, result, device);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_body_vkGetPhysicalDeviceWin32PresentationSupportKHR(ApiDumpInstance& dump_inst, VkBool32 result, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceWin32PresentationSupportKHR(dump_inst, result, physicalDevice, queueFamilyIndex);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceWin32PresentationSupportKHR(dump_inst, result, physicalDevice, queueFamilyIndex);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceWin32PresentationSupportKHR(dump_inst, result, physicalDevice, queueFamilyIndex);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_VI_NN)
inline void dump_body_vkCreateViSurfaceNN(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateViSurfaceNN(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateViSurfaceNN(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateViSurfaceNN(dump_inst, result, instance, pCreateInfo, pAllocator, pSurface);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_VI_NN
inline void dump_body_vkCreateDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateDescriptorUpdateTemplate(dump_inst, result, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateDescriptorUpdateTemplate(dump_inst, result, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateDescriptorUpdateTemplate(dump_inst, result, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetSwapchainStatusKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkSwapchainKHR swapchain)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetSwapchainStatusKHR(dump_inst, result, device, swapchain);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetSwapchainStatusKHR(dump_inst, result, device, swapchain);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetSwapchainStatusKHR(dump_inst, result, device, swapchain);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkAllocateMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkAllocateMemory(dump_inst, result, device, pAllocateInfo, pAllocator, pMemory);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkAllocateMemory(dump_inst, result, device, pAllocateInfo, pAllocator, pMemory);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkAllocateMemory(dump_inst, result, device, pAllocateInfo, pAllocator, pMemory);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkEndCommandBuffer(ApiDumpInstance& dump_inst, VkResult result, VkCommandBuffer commandBuffer)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkEndCommandBuffer(dump_inst, result, commandBuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkEndCommandBuffer(dump_inst, result, commandBuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkEndCommandBuffer(dump_inst, result, commandBuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateFence(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateFence(dump_inst, result, device, pCreateInfo, pAllocator, pFence);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateFence(dump_inst, result, device, pCreateInfo, pAllocator, pFence);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateFence(dump_inst, result, device, pCreateInfo, pAllocator, pFence);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateFramebuffer(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateFramebuffer(dump_inst, result, device, pCreateInfo, pAllocator, pFramebuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateFramebuffer(dump_inst, result, device, pCreateInfo, pAllocator, pFramebuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateFramebuffer(dump_inst, result, device, pCreateInfo, pAllocator, pFramebuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkBindImageMemory2(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkBindImageMemory2(dump_inst, result, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkBindImageMemory2(dump_inst, result, device, bindInfoCount, pBindInfos);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkBindImageMemory2(dump_inst, result, device, bindInfoCount, pBindInfos);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPipelineExecutableStatisticsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPipelineExecutableStatisticsKHR(dump_inst, result, device, pExecutableInfo, pStatisticCount, pStatistics);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPipelineExecutableStatisticsKHR(dump_inst, result, device, pExecutableInfo, pStatisticCount, pStatistics);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPipelineExecutableStatisticsKHR(dump_inst, result, device, pExecutableInfo, pStatisticCount, pStatistics);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPipelineExecutableInternalRepresentationsKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPipelineExecutableInternalRepresentationsKHR(dump_inst, result, device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPipelineExecutableInternalRepresentationsKHR(dump_inst, result, device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPipelineExecutableInternalRepresentationsKHR(dump_inst, result, device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_body_vkImportFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkImportFenceWin32HandleKHR(dump_inst, result, device, pImportFenceWin32HandleInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkImportFenceWin32HandleKHR(dump_inst, result, device, pImportFenceWin32HandleInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkImportFenceWin32HandleKHR(dump_inst, result, device, pImportFenceWin32HandleInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_body_vkCreateRenderPass2KHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderPassCreateInfo2KHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateRenderPass2KHR(dump_inst, result, device, pCreateInfo, pAllocator, pRenderPass);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateRenderPass2KHR(dump_inst, result, device, pCreateInfo, pAllocator, pRenderPass);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateRenderPass2KHR(dump_inst, result, device, pCreateInfo, pAllocator, pRenderPass);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetFenceStatus(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkFence fence)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetFenceStatus(dump_inst, result, device, fence);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetFenceStatus(dump_inst, result, device, fence);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetFenceStatus(dump_inst, result, device, fence);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateRenderPass(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateRenderPass(dump_inst, result, device, pCreateInfo, pAllocator, pRenderPass);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateRenderPass(dump_inst, result, device, pCreateInfo, pAllocator, pRenderPass);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateRenderPass(dump_inst, result, device, pCreateInfo, pAllocator, pRenderPass);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_body_vkGetFenceWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetFenceWin32HandleKHR(dump_inst, result, device, pGetWin32HandleInfo, pHandle);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetFenceWin32HandleKHR(dump_inst, result, device, pGetWin32HandleInfo, pHandle);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetFenceWin32HandleKHR(dump_inst, result, device, pGetWin32HandleInfo, pHandle);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_body_vkImportFenceFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkImportFenceFdKHR(dump_inst, result, device, pImportFenceFdInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkImportFenceFdKHR(dump_inst, result, device, pImportFenceFdInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkImportFenceFdKHR(dump_inst, result, device, pImportFenceFdInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateShaderModule(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateShaderModule(dump_inst, result, device, pCreateInfo, pAllocator, pShaderModule);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateShaderModule(dump_inst, result, device, pCreateInfo, pAllocator, pShaderModule);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateShaderModule(dump_inst, result, device, pCreateInfo, pAllocator, pShaderModule);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkResetFences(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t fenceCount, const VkFence* pFences)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkResetFences(dump_inst, result, device, fenceCount, pFences);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkResetFences(dump_inst, result, device, fenceCount, pFences);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkResetFences(dump_inst, result, device, fenceCount, pFences);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkWaitForFences(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkWaitForFences(dump_inst, result, device, fenceCount, pFences, waitAll, timeout);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkWaitForFences(dump_inst, result, device, fenceCount, pFences, waitAll, timeout);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkWaitForFences(dump_inst, result, device, fenceCount, pFences, waitAll, timeout);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateSemaphore(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateSemaphore(dump_inst, result, device, pCreateInfo, pAllocator, pSemaphore);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateSemaphore(dump_inst, result, device, pCreateInfo, pAllocator, pSemaphore);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateSemaphore(dump_inst, result, device, pCreateInfo, pAllocator, pSemaphore);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetFenceFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetFenceFdKHR(dump_inst, result, device, pGetFdInfo, pFd);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetFenceFdKHR(dump_inst, result, device, pGetFdInfo, pFd);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetFenceFdKHR(dump_inst, result, device, pGetFdInfo, pFd);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateInstance(ApiDumpInstance& dump_inst, VkResult result, const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateInstance(dump_inst, result, pCreateInfo, pAllocator, pInstance);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateInstance(dump_inst, result, pCreateInfo, pAllocator, pInstance);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateInstance(dump_inst, result, pCreateInfo, pAllocator, pInstance);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetQueryPoolResults(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetQueryPoolResults(dump_inst, result, device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetQueryPoolResults(dump_inst, result, device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetQueryPoolResults(dump_inst, result, device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_body_vkGetMemoryWin32HandlePropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetMemoryWin32HandlePropertiesKHR(dump_inst, result, device, handleType, handle, pMemoryWin32HandleProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetMemoryWin32HandlePropertiesKHR(dump_inst, result, device, handleType, handle, pMemoryWin32HandleProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetMemoryWin32HandlePropertiesKHR(dump_inst, result, device, handleType, handle, pMemoryWin32HandleProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_body_vkMapMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkMapMemory(dump_inst, result, device, memory, offset, size, flags, ppData);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkMapMemory(dump_inst, result, device, memory, offset, size, flags, ppData);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkMapMemory(dump_inst, result, device, memory, offset, size, flags, ppData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
inline void dump_body_vkGetMemoryWin32HandleKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetMemoryWin32HandleKHR(dump_inst, result, device, pGetWin32HandleInfo, pHandle);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetMemoryWin32HandleKHR(dump_inst, result, device, pGetWin32HandleInfo, pHandle);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetMemoryWin32HandleKHR(dump_inst, result, device, pGetWin32HandleInfo, pHandle);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
#endif // VK_USE_PLATFORM_WIN32_KHR
inline void dump_body_vkCreateBuffer(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateBuffer(dump_inst, result, device, pCreateInfo, pAllocator, pBuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateBuffer(dump_inst, result, device, pCreateInfo, pAllocator, pBuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateBuffer(dump_inst, result, device, pCreateInfo, pAllocator, pBuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkBindBufferMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkBindBufferMemory(dump_inst, result, device, buffer, memory, memoryOffset);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkBindBufferMemory(dump_inst, result, device, buffer, memory, memoryOffset);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkBindBufferMemory(dump_inst, result, device, buffer, memory, memoryOffset);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkFlushMappedMemoryRanges(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkFlushMappedMemoryRanges(dump_inst, result, device, memoryRangeCount, pMemoryRanges);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkFlushMappedMemoryRanges(dump_inst, result, device, memoryRangeCount, pMemoryRanges);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkFlushMappedMemoryRanges(dump_inst, result, device, memoryRangeCount, pMemoryRanges);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetMemoryFdPropertiesKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetMemoryFdPropertiesKHR(dump_inst, result, device, handleType, fd, pMemoryFdProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetMemoryFdPropertiesKHR(dump_inst, result, device, handleType, fd, pMemoryFdProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetMemoryFdPropertiesKHR(dump_inst, result, device, handleType, fd, pMemoryFdProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkEnumeratePhysicalDevices(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkEnumeratePhysicalDevices(dump_inst, result, instance, pPhysicalDeviceCount, pPhysicalDevices);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkEnumeratePhysicalDevices(dump_inst, result, instance, pPhysicalDeviceCount, pPhysicalDevices);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkEnumeratePhysicalDevices(dump_inst, result, instance, pPhysicalDeviceCount, pPhysicalDevices);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkInvalidateMappedMemoryRanges(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkInvalidateMappedMemoryRanges(dump_inst, result, device, memoryRangeCount, pMemoryRanges);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkInvalidateMappedMemoryRanges(dump_inst, result, device, memoryRangeCount, pMemoryRanges);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkInvalidateMappedMemoryRanges(dump_inst, result, device, memoryRangeCount, pMemoryRanges);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetMemoryFdKHR(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetMemoryFdKHR(dump_inst, result, device, pGetFdInfo, pFd);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetMemoryFdKHR(dump_inst, result, device, pGetFdInfo, pFd);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetMemoryFdKHR(dump_inst, result, device, pGetFdInfo, pFd);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkResult result, VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateDebugReportCallbackEXT(dump_inst, result, instance, pCreateInfo, pAllocator, pCallback);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateDebugReportCallbackEXT(dump_inst, result, instance, pCreateInfo, pAllocator, pCallback);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateDebugReportCallbackEXT(dump_inst, result, instance, pCreateInfo, pAllocator, pCallback);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceImageFormatProperties(ApiDumpInstance& dump_inst, VkResult result, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceImageFormatProperties(dump_inst, result, physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceImageFormatProperties(dump_inst, result, physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceImageFormatProperties(dump_inst, result, physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkBindImageMemory(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkBindImageMemory(dump_inst, result, device, image, memory, memoryOffset);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkBindImageMemory(dump_inst, result, device, image, memory, memoryOffset);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkBindImageMemory(dump_inst, result, device, image, memory, memoryOffset);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCreateBufferView(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView)
{
    if (!dump_inst.shouldDumpOutput()) return;

    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCreateBufferView(dump_inst, result, device, pCreateInfo, pAllocator, pView);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCreateBufferView(dump_inst, result, device, pCreateInfo, pAllocator, pView);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCreateBufferView(dump_inst, result, device, pCreateInfo, pAllocator, pView);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}


inline void dump_body_vkCmdBindTransformFeedbackBuffersEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdBindTransformFeedbackBuffersEXT(dump_inst, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdBindTransformFeedbackBuffersEXT(dump_inst, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdBindTransformFeedbackBuffersEXT(dump_inst, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdBeginTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdBeginTransformFeedbackEXT(dump_inst, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdBeginTransformFeedbackEXT(dump_inst, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdBeginTransformFeedbackEXT(dump_inst, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdEndTransformFeedbackEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdEndTransformFeedbackEXT(dump_inst, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdEndTransformFeedbackEXT(dump_inst, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdEndTransformFeedbackEXT(dump_inst, commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyBufferView(ApiDumpInstance& dump_inst, VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyBufferView(dump_inst, device, bufferView, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyBufferView(dump_inst, device, bufferView, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyBufferView(dump_inst, device, bufferView, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyEvent(ApiDumpInstance& dump_inst, VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyEvent(dump_inst, device, event, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyEvent(dump_inst, device, event, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyEvent(dump_inst, device, event, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyPipelineCache(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyPipelineCache(dump_inst, device, pipelineCache, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyPipelineCache(dump_inst, device, pipelineCache, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyPipelineCache(dump_inst, device, pipelineCache, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdBeginQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdBeginQueryIndexedEXT(dump_inst, commandBuffer, queryPool, query, flags, index);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdBeginQueryIndexedEXT(dump_inst, commandBuffer, queryPool, query, flags, index);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdBeginQueryIndexedEXT(dump_inst, commandBuffer, queryPool, query, flags, index);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetImageSubresourceLayout(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetImageSubresourceLayout(dump_inst, device, image, pSubresource, pLayout);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetImageSubresourceLayout(dump_inst, device, image, pSubresource, pLayout);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetImageSubresourceLayout(dump_inst, device, image, pSubresource, pLayout);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdEndQueryIndexedEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdEndQueryIndexedEXT(dump_inst, commandBuffer, queryPool, query, index);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdEndQueryIndexedEXT(dump_inst, commandBuffer, queryPool, query, index);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdEndQueryIndexedEXT(dump_inst, commandBuffer, queryPool, query, index);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDrawIndirectByteCountEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDrawIndirectByteCountEXT(dump_inst, commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDrawIndirectByteCountEXT(dump_inst, commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDrawIndirectByteCountEXT(dump_inst, commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdWriteBufferMarkerAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdWriteBufferMarkerAMD(dump_inst, commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdWriteBufferMarkerAMD(dump_inst, commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdWriteBufferMarkerAMD(dump_inst, commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroySwapchainKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroySwapchainKHR(dump_inst, device, swapchain, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroySwapchainKHR(dump_inst, device, swapchain, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroySwapchainKHR(dump_inst, device, swapchain, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyImage(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyImage(dump_inst, device, image, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyImage(dump_inst, device, image, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyImage(dump_inst, device, image, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyQueryPool(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyQueryPool(dump_inst, device, queryPool, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyQueryPool(dump_inst, device, queryPool, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyQueryPool(dump_inst, device, queryPool, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdBlitImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdBlitImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdBlitImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdBlitImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceProperties(dump_inst, physicalDevice, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceProperties(dump_inst, physicalDevice, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceProperties(dump_inst, physicalDevice, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdBuildAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdBuildAccelerationStructureNV(dump_inst, commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdBuildAccelerationStructureNV(dump_inst, commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdBuildAccelerationStructureNV(dump_inst, commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetAccelerationStructureMemoryRequirementsNV(ApiDumpInstance& dump_inst, VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetAccelerationStructureMemoryRequirementsNV(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetAccelerationStructureMemoryRequirementsNV(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetAccelerationStructureMemoryRequirementsNV(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyAccelerationStructureNV(dump_inst, device, accelerationStructure, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyAccelerationStructureNV(dump_inst, device, accelerationStructure, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyAccelerationStructureNV(dump_inst, device, accelerationStructure, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdCopyBufferToImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdCopyBufferToImage(dump_inst, commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdCopyBufferToImage(dump_inst, commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdCopyBufferToImage(dump_inst, commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetViewport(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetViewport(dump_inst, commandBuffer, firstViewport, viewportCount, pViewports);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetViewport(dump_inst, commandBuffer, firstViewport, viewportCount, pViewports);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetViewport(dump_inst, commandBuffer, firstViewport, viewportCount, pViewports);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyPipeline(ApiDumpInstance& dump_inst, VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyPipeline(dump_inst, device, pipeline, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyPipeline(dump_inst, device, pipeline, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyPipeline(dump_inst, device, pipeline, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdCopyAccelerationStructureNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeNV mode)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdCopyAccelerationStructureNV(dump_inst, commandBuffer, dst, src, mode);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdCopyAccelerationStructureNV(dump_inst, commandBuffer, dst, src, mode);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdCopyAccelerationStructureNV(dump_inst, commandBuffer, dst, src, mode);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdBindPipeline(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdBindPipeline(dump_inst, commandBuffer, pipelineBindPoint, pipeline);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdBindPipeline(dump_inst, commandBuffer, pipelineBindPoint, pipeline);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdBindPipeline(dump_inst, commandBuffer, pipelineBindPoint, pipeline);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetSampleLocationsEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetSampleLocationsEXT(dump_inst, commandBuffer, pSampleLocationsInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetSampleLocationsEXT(dump_inst, commandBuffer, pSampleLocationsInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetSampleLocationsEXT(dump_inst, commandBuffer, pSampleLocationsInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetImageSparseMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetImageSparseMemoryRequirements2KHR(dump_inst, device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetImageSparseMemoryRequirements2KHR(dump_inst, device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetImageSparseMemoryRequirements2KHR(dump_inst, device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetLineWidth(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float lineWidth)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetLineWidth(dump_inst, commandBuffer, lineWidth);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetLineWidth(dump_inst, commandBuffer, lineWidth);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetLineWidth(dump_inst, commandBuffer, lineWidth);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetScissor(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetScissor(dump_inst, commandBuffer, firstScissor, scissorCount, pScissors);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetScissor(dump_inst, commandBuffer, firstScissor, scissorCount, pScissors);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetScissor(dump_inst, commandBuffer, firstScissor, scissorCount, pScissors);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkUninitializePerformanceApiINTEL(ApiDumpInstance& dump_inst, VkDevice device)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkUninitializePerformanceApiINTEL(dump_inst, device);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkUninitializePerformanceApiINTEL(dump_inst, device);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkUninitializePerformanceApiINTEL(dump_inst, device);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetDepthBias(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetDepthBias(dump_inst, commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetDepthBias(dump_inst, commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetDepthBias(dump_inst, commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceMultisamplePropertiesEXT(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceMultisamplePropertiesEXT(dump_inst, physicalDevice, samples, pMultisampleProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceMultisamplePropertiesEXT(dump_inst, physicalDevice, samples, pMultisampleProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceMultisamplePropertiesEXT(dump_inst, physicalDevice, samples, pMultisampleProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdTraceRaysNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdTraceRaysNV(dump_inst, commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdTraceRaysNV(dump_inst, commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdTraceRaysNV(dump_inst, commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDrawIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDrawIndirectCountAMD(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDrawIndirectCountAMD(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDrawIndirectCountAMD(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDrawIndexedIndirectCountAMD(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDrawIndexedIndirectCountAMD(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDrawIndexedIndirectCountAMD(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDrawIndexedIndirectCountAMD(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetImageMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetImageMemoryRequirements2KHR(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetImageMemoryRequirements2KHR(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetImageMemoryRequirements2KHR(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetBufferMemoryRequirements2KHR(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetBufferMemoryRequirements2KHR(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetBufferMemoryRequirements2KHR(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetBufferMemoryRequirements2KHR(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdCopyImageToBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdCopyImageToBuffer(dump_inst, commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdCopyImageToBuffer(dump_inst, commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdCopyImageToBuffer(dump_inst, commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetBlendConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const float blendConstants[4])
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetBlendConstants(dump_inst, commandBuffer, blendConstants);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetBlendConstants(dump_inst, commandBuffer, blendConstants);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetBlendConstants(dump_inst, commandBuffer, blendConstants);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetStencilWriteMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetStencilWriteMask(dump_inst, commandBuffer, faceMask, writeMask);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetStencilWriteMask(dump_inst, commandBuffer, faceMask, writeMask);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetStencilWriteMask(dump_inst, commandBuffer, faceMask, writeMask);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyPipelineLayout(ApiDumpInstance& dump_inst, VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyPipelineLayout(dump_inst, device, pipelineLayout, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyPipelineLayout(dump_inst, device, pipelineLayout, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyPipelineLayout(dump_inst, device, pipelineLayout, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetDepthBounds(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetDepthBounds(dump_inst, commandBuffer, minDepthBounds, maxDepthBounds);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetDepthBounds(dump_inst, commandBuffer, minDepthBounds, maxDepthBounds);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetDepthBounds(dump_inst, commandBuffer, minDepthBounds, maxDepthBounds);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyRenderPass(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyRenderPass(dump_inst, device, renderPass, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyRenderPass(dump_inst, device, renderPass, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyRenderPass(dump_inst, device, renderPass, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdUpdateBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdUpdateBuffer(dump_inst, commandBuffer, dstBuffer, dstOffset, dataSize, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdUpdateBuffer(dump_inst, commandBuffer, dstBuffer, dstOffset, dataSize, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdUpdateBuffer(dump_inst, commandBuffer, dstBuffer, dstOffset, dataSize, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetStencilCompareMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetStencilCompareMask(dump_inst, commandBuffer, faceMask, compareMask);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetStencilCompareMask(dump_inst, commandBuffer, faceMask, compareMask);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetStencilCompareMask(dump_inst, commandBuffer, faceMask, compareMask);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdFillBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdFillBuffer(dump_inst, commandBuffer, dstBuffer, dstOffset, size, data);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdFillBuffer(dump_inst, commandBuffer, dstBuffer, dstOffset, size, data);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdFillBuffer(dump_inst, commandBuffer, dstBuffer, dstOffset, size, data);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceMemoryProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceMemoryProperties(dump_inst, physicalDevice, pMemoryProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceMemoryProperties(dump_inst, physicalDevice, pMemoryProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceMemoryProperties(dump_inst, physicalDevice, pMemoryProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceQueueFamilyProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceQueueFamilyProperties(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceQueueFamilyProperties(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceQueueFamilyProperties(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdProcessCommandsNVX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCmdProcessCommandsInfoNVX* pProcessCommandsInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdProcessCommandsNVX(dump_inst, commandBuffer, pProcessCommandsInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdProcessCommandsNVX(dump_inst, commandBuffer, pProcessCommandsInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdProcessCommandsNVX(dump_inst, commandBuffer, pProcessCommandsInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkTrimCommandPoolKHR(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkTrimCommandPoolKHR(dump_inst, device, commandPool, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkTrimCommandPoolKHR(dump_inst, device, commandPool, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkTrimCommandPoolKHR(dump_inst, device, commandPool, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdReserveSpaceForCommandsNVX(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkCmdReserveSpaceForCommandsInfoNVX* pReserveSpaceInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdReserveSpaceForCommandsNVX(dump_inst, commandBuffer, pReserveSpaceInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdReserveSpaceForCommandsNVX(dump_inst, commandBuffer, pReserveSpaceInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdReserveSpaceForCommandsNVX(dump_inst, commandBuffer, pReserveSpaceInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceExternalBufferPropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceExternalBufferPropertiesKHR(dump_inst, physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceExternalBufferPropertiesKHR(dump_inst, physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceExternalBufferPropertiesKHR(dump_inst, physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyIndirectCommandsLayoutNVX(ApiDumpInstance& dump_inst, VkDevice device, VkIndirectCommandsLayoutNVX indirectCommandsLayout, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyIndirectCommandsLayoutNVX(dump_inst, device, indirectCommandsLayout, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyIndirectCommandsLayoutNVX(dump_inst, device, indirectCommandsLayout, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyIndirectCommandsLayoutNVX(dump_inst, device, indirectCommandsLayout, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyObjectTableNVX(ApiDumpInstance& dump_inst, VkDevice device, VkObjectTableNVX objectTable, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyObjectTableNVX(dump_inst, device, objectTable, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyObjectTableNVX(dump_inst, device, objectTable, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyObjectTableNVX(dump_inst, device, objectTable, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDebugMarkerBeginEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDebugMarkerBeginEXT(dump_inst, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDebugMarkerBeginEXT(dump_inst, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDebugMarkerBeginEXT(dump_inst, commandBuffer, pMarkerInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDebugMarkerInsertEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDebugMarkerInsertEXT(dump_inst, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDebugMarkerInsertEXT(dump_inst, commandBuffer, pMarkerInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDebugMarkerInsertEXT(dump_inst, commandBuffer, pMarkerInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDebugMarkerEndEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDebugMarkerEndEXT(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDebugMarkerEndEXT(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDebugMarkerEndEXT(dump_inst, commandBuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdWriteAccelerationStructuresPropertiesNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdWriteAccelerationStructuresPropertiesNV(dump_inst, commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdWriteAccelerationStructuresPropertiesNV(dump_inst, commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdWriteAccelerationStructuresPropertiesNV(dump_inst, commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkQueueInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkQueueInsertDebugUtilsLabelEXT(dump_inst, queue, pLabelInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkQueueInsertDebugUtilsLabelEXT(dump_inst, queue, pLabelInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkQueueInsertDebugUtilsLabelEXT(dump_inst, queue, pLabelInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdBeginDebugUtilsLabelEXT(dump_inst, commandBuffer, pLabelInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdBeginDebugUtilsLabelEXT(dump_inst, commandBuffer, pLabelInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdBeginDebugUtilsLabelEXT(dump_inst, commandBuffer, pLabelInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetCoarseSampleOrderNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetCoarseSampleOrderNV(dump_inst, commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetCoarseSampleOrderNV(dump_inst, commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetCoarseSampleOrderNV(dump_inst, commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdEndDebugUtilsLabelEXT(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdEndDebugUtilsLabelEXT(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdEndDebugUtilsLabelEXT(dump_inst, commandBuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkDeviceGeneratedCommandsFeaturesNVX* pFeatures, VkDeviceGeneratedCommandsLimitsNVX* pLimits)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(dump_inst, physicalDevice, pFeatures, pLimits);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(dump_inst, physicalDevice, pFeatures, pLimits);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(dump_inst, physicalDevice, pFeatures, pLimits);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdInsertDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdInsertDebugUtilsLabelEXT(dump_inst, commandBuffer, pLabelInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdInsertDebugUtilsLabelEXT(dump_inst, commandBuffer, pLabelInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdInsertDebugUtilsLabelEXT(dump_inst, commandBuffer, pLabelInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkSubmitDebugUtilsMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkSubmitDebugUtilsMessageEXT(dump_inst, instance, messageSeverity, messageTypes, pCallbackData);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkSubmitDebugUtilsMessageEXT(dump_inst, instance, messageSeverity, messageTypes, pCallbackData);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkSubmitDebugUtilsMessageEXT(dump_inst, instance, messageSeverity, messageTypes, pCallbackData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetViewportWScalingNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetViewportWScalingNV(dump_inst, commandBuffer, firstViewport, viewportCount, pViewportWScalings);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetViewportWScalingNV(dump_inst, commandBuffer, firstViewport, viewportCount, pViewportWScalings);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetViewportWScalingNV(dump_inst, commandBuffer, firstViewport, viewportCount, pViewportWScalings);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkSetLocalDimmingAMD(ApiDumpInstance& dump_inst, VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkSetLocalDimmingAMD(dump_inst, device, swapChain, localDimmingEnable);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkSetLocalDimmingAMD(dump_inst, device, swapChain, localDimmingEnable);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkSetLocalDimmingAMD(dump_inst, device, swapChain, localDimmingEnable);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyDebugUtilsMessengerEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyDebugUtilsMessengerEXT(dump_inst, instance, messenger, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyDebugUtilsMessengerEXT(dump_inst, instance, messenger, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyDebugUtilsMessengerEXT(dump_inst, instance, messenger, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceExternalSemaphoreProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceExternalSemaphoreProperties(dump_inst, physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceExternalSemaphoreProperties(dump_inst, physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceExternalSemaphoreProperties(dump_inst, physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdBeginConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdBeginConditionalRenderingEXT(dump_inst, commandBuffer, pConditionalRenderingBegin);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdBeginConditionalRenderingEXT(dump_inst, commandBuffer, pConditionalRenderingBegin);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdBeginConditionalRenderingEXT(dump_inst, commandBuffer, pConditionalRenderingBegin);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDrawIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDrawIndirectCountKHR(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDrawIndirectCountKHR(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDrawIndirectCountKHR(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdEndConditionalRenderingEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdEndConditionalRenderingEXT(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdEndConditionalRenderingEXT(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdEndConditionalRenderingEXT(dump_inst, commandBuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetDescriptorSetLayoutSupportKHR(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetDescriptorSetLayoutSupportKHR(dump_inst, device, pCreateInfo, pSupport);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetDescriptorSetLayoutSupportKHR(dump_inst, device, pCreateInfo, pSupport);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetDescriptorSetLayoutSupportKHR(dump_inst, device, pCreateInfo, pSupport);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroySampler(ApiDumpInstance& dump_inst, VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroySampler(dump_inst, device, sampler, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroySampler(dump_inst, device, sampler, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroySampler(dump_inst, device, sampler, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDrawIndexedIndirectCountKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDrawIndexedIndirectCountKHR(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDrawIndexedIndirectCountKHR(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDrawIndexedIndirectCountKHR(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetDescriptorSetLayoutSupport(ApiDumpInstance& dump_inst, VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetDescriptorSetLayoutSupport(dump_inst, device, pCreateInfo, pSupport);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetDescriptorSetLayoutSupport(dump_inst, device, pCreateInfo, pSupport);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetDescriptorSetLayoutSupport(dump_inst, device, pCreateInfo, pSupport);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkResetQueryPoolEXT(ApiDumpInstance& dump_inst, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkResetQueryPoolEXT(dump_inst, device, queryPool, firstQuery, queryCount);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkResetQueryPoolEXT(dump_inst, device, queryPool, firstQuery, queryCount);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkResetQueryPoolEXT(dump_inst, device, queryPool, firstQuery, queryCount);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetLineStippleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetLineStippleEXT(dump_inst, commandBuffer, lineStippleFactor, lineStipplePattern);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetLineStippleEXT(dump_inst, commandBuffer, lineStippleFactor, lineStipplePattern);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetLineStippleEXT(dump_inst, commandBuffer, lineStippleFactor, lineStipplePattern);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyDescriptorSetLayout(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyDescriptorSetLayout(dump_inst, device, descriptorSetLayout, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyDescriptorSetLayout(dump_inst, device, descriptorSetLayout, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyDescriptorSetLayout(dump_inst, device, descriptorSetLayout, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroySurfaceKHR(ApiDumpInstance& dump_inst, VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroySurfaceKHR(dump_inst, instance, surface, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroySurfaceKHR(dump_inst, instance, surface, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroySurfaceKHR(dump_inst, instance, surface, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkUpdateDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkUpdateDescriptorSetWithTemplateKHR(dump_inst, device, descriptorSet, descriptorUpdateTemplate, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkUpdateDescriptorSetWithTemplateKHR(dump_inst, device, descriptorSet, descriptorUpdateTemplate, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkUpdateDescriptorSetWithTemplateKHR(dump_inst, device, descriptorSet, descriptorUpdateTemplate, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroySamplerYcbcrConversionKHR(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroySamplerYcbcrConversionKHR(dump_inst, device, ycbcrConversion, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroySamplerYcbcrConversionKHR(dump_inst, device, ycbcrConversion, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroySamplerYcbcrConversionKHR(dump_inst, device, ycbcrConversion, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetDeviceQueue2(ApiDumpInstance& dump_inst, VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetDeviceQueue2(dump_inst, device, pQueueInfo, pQueue);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetDeviceQueue2(dump_inst, device, pQueueInfo, pQueue);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetDeviceQueue2(dump_inst, device, pQueueInfo, pQueue);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyDescriptorUpdateTemplateKHR(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyDescriptorUpdateTemplateKHR(dump_inst, device, descriptorUpdateTemplate, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyDescriptorUpdateTemplateKHR(dump_inst, device, descriptorUpdateTemplate, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyDescriptorUpdateTemplateKHR(dump_inst, device, descriptorUpdateTemplate, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyValidationCacheEXT(ApiDumpInstance& dump_inst, VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyValidationCacheEXT(dump_inst, device, validationCache, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyValidationCacheEXT(dump_inst, device, validationCache, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyValidationCacheEXT(dump_inst, device, validationCache, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdWaitEvents(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdWaitEvents(dump_inst, commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdWaitEvents(dump_inst, commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdWaitEvents(dump_inst, commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdBindDescriptorSets(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdBindDescriptorSets(dump_inst, commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdBindDescriptorSets(dump_inst, commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdBindDescriptorSets(dump_inst, commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceFeatures2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceFeatures2(dump_inst, physicalDevice, pFeatures);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceFeatures2(dump_inst, physicalDevice, pFeatures);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceFeatures2(dump_inst, physicalDevice, pFeatures);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetStencilReference(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetStencilReference(dump_inst, commandBuffer, faceMask, reference);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetStencilReference(dump_inst, commandBuffer, faceMask, reference);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetStencilReference(dump_inst, commandBuffer, faceMask, reference);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkUpdateDescriptorSetWithTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkUpdateDescriptorSetWithTemplate(dump_inst, device, descriptorSet, descriptorUpdateTemplate, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkUpdateDescriptorSetWithTemplate(dump_inst, device, descriptorSet, descriptorUpdateTemplate, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkUpdateDescriptorSetWithTemplate(dump_inst, device, descriptorSet, descriptorUpdateTemplate, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdBindIndexBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdBindIndexBuffer(dump_inst, commandBuffer, buffer, offset, indexType);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdBindIndexBuffer(dump_inst, commandBuffer, buffer, offset, indexType);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdBindIndexBuffer(dump_inst, commandBuffer, buffer, offset, indexType);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceProperties2(dump_inst, physicalDevice, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceProperties2(dump_inst, physicalDevice, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceProperties2(dump_inst, physicalDevice, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyDescriptorUpdateTemplate(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyDescriptorUpdateTemplate(dump_inst, device, descriptorUpdateTemplate, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyDescriptorUpdateTemplate(dump_inst, device, descriptorUpdateTemplate, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyDescriptorUpdateTemplate(dump_inst, device, descriptorUpdateTemplate, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdBindVertexBuffers(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdBindVertexBuffers(dump_inst, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdBindVertexBuffers(dump_inst, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdBindVertexBuffers(dump_inst, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceMemoryProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceMemoryProperties2(dump_inst, physicalDevice, pMemoryProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceMemoryProperties2(dump_inst, physicalDevice, pMemoryProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceMemoryProperties2(dump_inst, physicalDevice, pMemoryProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceFormatProperties2(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceFormatProperties2(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceFormatProperties2(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdPipelineBarrier(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdPipelineBarrier(dump_inst, commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdPipelineBarrier(dump_inst, commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdPipelineBarrier(dump_inst, commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceSparseImageFormatProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceSparseImageFormatProperties2(dump_inst, physicalDevice, pFormatInfo, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceSparseImageFormatProperties2(dump_inst, physicalDevice, pFormatInfo, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceSparseImageFormatProperties2(dump_inst, physicalDevice, pFormatInfo, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDraw(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDraw(dump_inst, commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDraw(dump_inst, commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDraw(dump_inst, commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceQueueFamilyProperties2(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceQueueFamilyProperties2(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceQueueFamilyProperties2(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceQueueFamilyProperties2(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyDescriptorPool(ApiDumpInstance& dump_inst, VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyDescriptorPool(dump_inst, device, descriptorPool, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyDescriptorPool(dump_inst, device, descriptorPool, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyDescriptorPool(dump_inst, device, descriptorPool, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkTrimCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkTrimCommandPool(dump_inst, device, commandPool, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkTrimCommandPool(dump_inst, device, commandPool, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkTrimCommandPool(dump_inst, device, commandPool, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDrawIndexed(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDrawIndexed(dump_inst, commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDrawIndexed(dump_inst, commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDrawIndexed(dump_inst, commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDrawMeshTasksIndirectNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDrawMeshTasksIndirectNV(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDrawMeshTasksIndirectNV(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDrawMeshTasksIndirectNV(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDrawMeshTasksIndirectCountNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDrawMeshTasksIndirectCountNV(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDrawMeshTasksIndirectCountNV(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDrawMeshTasksIndirectCountNV(dump_inst, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDrawIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDrawIndirect(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDrawIndirect(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDrawIndirect(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDrawMeshTasksNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDrawMeshTasksNV(dump_inst, commandBuffer, taskCount, firstTask);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDrawMeshTasksNV(dump_inst, commandBuffer, taskCount, firstTask);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDrawMeshTasksNV(dump_inst, commandBuffer, taskCount, firstTask);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDrawIndexedIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDrawIndexedIndirect(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDrawIndexedIndirect(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDrawIndexedIndirect(dump_inst, commandBuffer, buffer, offset, drawCount, stride);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdBeginQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdBeginQuery(dump_inst, commandBuffer, queryPool, query, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdBeginQuery(dump_inst, commandBuffer, queryPool, query, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdBeginQuery(dump_inst, commandBuffer, queryPool, query, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkUpdateDescriptorSets(ApiDumpInstance& dump_inst, VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkUpdateDescriptorSets(dump_inst, device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkUpdateDescriptorSets(dump_inst, device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkUpdateDescriptorSets(dump_inst, device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDispatch(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDispatch(dump_inst, commandBuffer, groupCountX, groupCountY, groupCountZ);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDispatch(dump_inst, commandBuffer, groupCountX, groupCountY, groupCountZ);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDispatch(dump_inst, commandBuffer, groupCountX, groupCountY, groupCountZ);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdCopyBuffer(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdCopyBuffer(dump_inst, commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdCopyBuffer(dump_inst, commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdCopyBuffer(dump_inst, commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdResetQueryPool(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdResetQueryPool(dump_inst, commandBuffer, queryPool, firstQuery, queryCount);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdResetQueryPool(dump_inst, commandBuffer, queryPool, firstQuery, queryCount);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdResetQueryPool(dump_inst, commandBuffer, queryPool, firstQuery, queryCount);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdEndQuery(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdEndQuery(dump_inst, commandBuffer, queryPool, query);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdEndQuery(dump_inst, commandBuffer, queryPool, query);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdEndQuery(dump_inst, commandBuffer, queryPool, query);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdWriteTimestamp(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdWriteTimestamp(dump_inst, commandBuffer, pipelineStage, queryPool, query);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdWriteTimestamp(dump_inst, commandBuffer, pipelineStage, queryPool, query);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdWriteTimestamp(dump_inst, commandBuffer, pipelineStage, queryPool, query);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkQueueBeginDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkQueueBeginDebugUtilsLabelEXT(dump_inst, queue, pLabelInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkQueueBeginDebugUtilsLabelEXT(dump_inst, queue, pLabelInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkQueueBeginDebugUtilsLabelEXT(dump_inst, queue, pLabelInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDispatchIndirect(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDispatchIndirect(dump_inst, commandBuffer, buffer, offset);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDispatchIndirect(dump_inst, commandBuffer, buffer, offset);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDispatchIndirect(dump_inst, commandBuffer, buffer, offset);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceExternalBufferProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceExternalBufferProperties(dump_inst, physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceExternalBufferProperties(dump_inst, physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceExternalBufferProperties(dump_inst, physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdCopyImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdCopyImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdCopyImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdCopyImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkQueueEndDebugUtilsLabelEXT(ApiDumpInstance& dump_inst, VkQueue queue)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkQueueEndDebugUtilsLabelEXT(dump_inst, queue);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkQueueEndDebugUtilsLabelEXT(dump_inst, queue);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkQueueEndDebugUtilsLabelEXT(dump_inst, queue);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetDeviceGroupPeerMemoryFeatures(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetDeviceGroupPeerMemoryFeatures(dump_inst, device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetDeviceGroupPeerMemoryFeatures(dump_inst, device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetDeviceGroupPeerMemoryFeatures(dump_inst, device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetDeviceQueue(ApiDumpInstance& dump_inst, VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetDeviceQueue(dump_inst, device, queueFamilyIndex, queueIndex, pQueue);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetDeviceQueue(dump_inst, device, queueFamilyIndex, queueIndex, pQueue);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetDeviceQueue(dump_inst, device, queueFamilyIndex, queueIndex, pQueue);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdClearColorImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdClearColorImage(dump_inst, commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdClearColorImage(dump_inst, commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdClearColorImage(dump_inst, commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceFeatures2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceFeatures2KHR(dump_inst, physicalDevice, pFeatures);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceFeatures2KHR(dump_inst, physicalDevice, pFeatures);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceFeatures2KHR(dump_inst, physicalDevice, pFeatures);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetRenderAreaGranularity(ApiDumpInstance& dump_inst, VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetRenderAreaGranularity(dump_inst, device, renderPass, pGranularity);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetRenderAreaGranularity(dump_inst, device, renderPass, pGranularity);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetRenderAreaGranularity(dump_inst, device, renderPass, pGranularity);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetImageSparseMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetImageSparseMemoryRequirements2(dump_inst, device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetImageSparseMemoryRequirements2(dump_inst, device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetImageSparseMemoryRequirements2(dump_inst, device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetDeviceMask(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetDeviceMask(dump_inst, commandBuffer, deviceMask);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetDeviceMask(dump_inst, commandBuffer, deviceMask);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetDeviceMask(dump_inst, commandBuffer, deviceMask);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetDiscardRectangleEXT(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetDiscardRectangleEXT(dump_inst, commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetDiscardRectangleEXT(dump_inst, commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetDiscardRectangleEXT(dump_inst, commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdClearDepthStencilImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdClearDepthStencilImage(dump_inst, commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdClearDepthStencilImage(dump_inst, commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdClearDepthStencilImage(dump_inst, commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetImageMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetImageMemoryRequirements2(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetImageMemoryRequirements2(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetImageMemoryRequirements2(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDispatchBase(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDispatchBase(dump_inst, commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDispatchBase(dump_inst, commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDispatchBase(dump_inst, commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceProperties2KHR(dump_inst, physicalDevice, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceProperties2KHR(dump_inst, physicalDevice, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceProperties2KHR(dump_inst, physicalDevice, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyDevice(ApiDumpInstance& dump_inst, VkDevice device, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyDevice(dump_inst, device, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyDevice(dump_inst, device, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyDevice(dump_inst, device, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceMemoryProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceMemoryProperties2KHR(dump_inst, physicalDevice, pMemoryProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceMemoryProperties2KHR(dump_inst, physicalDevice, pMemoryProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceMemoryProperties2KHR(dump_inst, physicalDevice, pMemoryProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceSparseImageFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceSparseImageFormatProperties(dump_inst, physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceSparseImageFormatProperties(dump_inst, physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceSparseImageFormatProperties(dump_inst, physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetBufferMemoryRequirements2(ApiDumpInstance& dump_inst, VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetBufferMemoryRequirements2(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetBufferMemoryRequirements2(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetBufferMemoryRequirements2(dump_inst, device, pInfo, pMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdEndRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR*        pSubpassEndInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdEndRenderPass2KHR(dump_inst, commandBuffer, pSubpassEndInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdEndRenderPass2KHR(dump_inst, commandBuffer, pSubpassEndInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdEndRenderPass2KHR(dump_inst, commandBuffer, pSubpassEndInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceFormatProperties2KHR(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceFormatProperties2KHR(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceFormatProperties2KHR(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdClearAttachments(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdClearAttachments(dump_inst, commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdClearAttachments(dump_inst, commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdClearAttachments(dump_inst, commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(dump_inst, physicalDevice, pFormatInfo, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(dump_inst, physicalDevice, pFormatInfo, pPropertyCount, pProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(dump_inst, physicalDevice, pFormatInfo, pPropertyCount, pProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceQueueFamilyProperties2KHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceQueueFamilyProperties2KHR(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceQueueFamilyProperties2KHR(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceQueueFamilyProperties2KHR(dump_inst, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceFormatProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceFormatProperties(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceFormatProperties(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceFormatProperties(dump_inst, physicalDevice, format, pFormatProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkFreeCommandBuffers(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkFreeCommandBuffers(dump_inst, device, commandPool, commandBufferCount, pCommandBuffers);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkFreeCommandBuffers(dump_inst, device, commandPool, commandBufferCount, pCommandBuffers);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkFreeCommandBuffers(dump_inst, device, commandPool, commandBufferCount, pCommandBuffers);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetDeviceGroupPeerMemoryFeaturesKHR(ApiDumpInstance& dump_inst, VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetDeviceGroupPeerMemoryFeaturesKHR(dump_inst, device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetDeviceGroupPeerMemoryFeaturesKHR(dump_inst, device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetDeviceGroupPeerMemoryFeaturesKHR(dump_inst, device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetDeviceMaskKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t deviceMask)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetDeviceMaskKHR(dump_inst, commandBuffer, deviceMask);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetDeviceMaskKHR(dump_inst, commandBuffer, deviceMask);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetDeviceMaskKHR(dump_inst, commandBuffer, deviceMask);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyCommandPool(ApiDumpInstance& dump_inst, VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyCommandPool(dump_inst, device, commandPool, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyCommandPool(dump_inst, device, commandPool, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyCommandPool(dump_inst, device, commandPool, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdBeginRenderPass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdBeginRenderPass2KHR(dump_inst, commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdBeginRenderPass2KHR(dump_inst, commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdBeginRenderPass2KHR(dump_inst, commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdResolveImage(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdResolveImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdResolveImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdResolveImage(dump_inst, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdDispatchBaseKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdDispatchBaseKHR(dump_inst, commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdDispatchBaseKHR(dump_inst, commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdDispatchBaseKHR(dump_inst, commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdResetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdResetEvent(dump_inst, commandBuffer, event, stageMask);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdResetEvent(dump_inst, commandBuffer, event, stageMask);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdResetEvent(dump_inst, commandBuffer, event, stageMask);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroySamplerYcbcrConversion(ApiDumpInstance& dump_inst, VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroySamplerYcbcrConversion(dump_inst, device, ycbcrConversion, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroySamplerYcbcrConversion(dump_inst, device, ycbcrConversion, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroySamplerYcbcrConversion(dump_inst, device, ycbcrConversion, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdPushDescriptorSetKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdPushDescriptorSetKHR(dump_inst, commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdPushDescriptorSetKHR(dump_inst, commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdPushDescriptorSetKHR(dump_inst, commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdNextSubpass2KHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo, const VkSubpassEndInfoKHR*        pSubpassEndInfo)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdNextSubpass2KHR(dump_inst, commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdNextSubpass2KHR(dump_inst, commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdNextSubpass2KHR(dump_inst, commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetViewportShadingRatePaletteNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetViewportShadingRatePaletteNV(dump_inst, commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetViewportShadingRatePaletteNV(dump_inst, commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetViewportShadingRatePaletteNV(dump_inst, commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdPushDescriptorSetWithTemplateKHR(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdPushDescriptorSetWithTemplateKHR(dump_inst, commandBuffer, descriptorUpdateTemplate, layout, set, pData);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdPushDescriptorSetWithTemplateKHR(dump_inst, commandBuffer, descriptorUpdateTemplate, layout, set, pData);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdPushDescriptorSetWithTemplateKHR(dump_inst, commandBuffer, descriptorUpdateTemplate, layout, set, pData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetEvent(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetEvent(dump_inst, commandBuffer, event, stageMask);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetEvent(dump_inst, commandBuffer, event, stageMask);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetEvent(dump_inst, commandBuffer, event, stageMask);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkSetHdrMetadataEXT(ApiDumpInstance& dump_inst, VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkSetHdrMetadataEXT(dump_inst, device, swapchainCount, pSwapchains, pMetadata);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkSetHdrMetadataEXT(dump_inst, device, swapchainCount, pSwapchains, pMetadata);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkSetHdrMetadataEXT(dump_inst, device, swapchainCount, pSwapchains, pMetadata);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdBindShadingRateImageNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdBindShadingRateImageNV(dump_inst, commandBuffer, imageView, imageLayout);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdBindShadingRateImageNV(dump_inst, commandBuffer, imageView, imageLayout);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdBindShadingRateImageNV(dump_inst, commandBuffer, imageView, imageLayout);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdCopyQueryPoolResults(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdCopyQueryPoolResults(dump_inst, commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdCopyQueryPoolResults(dump_inst, commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdCopyQueryPoolResults(dump_inst, commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdPushConstants(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdPushConstants(dump_inst, commandBuffer, layout, stageFlags, offset, size, pValues);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdPushConstants(dump_inst, commandBuffer, layout, stageFlags, offset, size, pValues);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdPushConstants(dump_inst, commandBuffer, layout, stageFlags, offset, size, pValues);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetExclusiveScissorNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetExclusiveScissorNV(dump_inst, commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetExclusiveScissorNV(dump_inst, commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetExclusiveScissorNV(dump_inst, commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceExternalFencePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceExternalFencePropertiesKHR(dump_inst, physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceExternalFencePropertiesKHR(dump_inst, physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceExternalFencePropertiesKHR(dump_inst, physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdNextSubpass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, VkSubpassContents contents)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdNextSubpass(dump_inst, commandBuffer, contents);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdNextSubpass(dump_inst, commandBuffer, contents);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdNextSubpass(dump_inst, commandBuffer, contents);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdSetCheckpointNV(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const void* pCheckpointMarker)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdSetCheckpointNV(dump_inst, commandBuffer, pCheckpointMarker);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdSetCheckpointNV(dump_inst, commandBuffer, pCheckpointMarker);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdSetCheckpointNV(dump_inst, commandBuffer, pCheckpointMarker);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdBeginRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdBeginRenderPass(dump_inst, commandBuffer, pRenderPassBegin, contents);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdBeginRenderPass(dump_inst, commandBuffer, pRenderPassBegin, contents);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdBeginRenderPass(dump_inst, commandBuffer, pRenderPassBegin, contents);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetQueueCheckpointDataNV(ApiDumpInstance& dump_inst, VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetQueueCheckpointDataNV(dump_inst, queue, pCheckpointDataCount, pCheckpointData);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetQueueCheckpointDataNV(dump_inst, queue, pCheckpointDataCount, pCheckpointData);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetQueueCheckpointDataNV(dump_inst, queue, pCheckpointDataCount, pCheckpointData);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdEndRenderPass(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdEndRenderPass(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdEndRenderPass(dump_inst, commandBuffer);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdEndRenderPass(dump_inst, commandBuffer);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyFramebuffer(ApiDumpInstance& dump_inst, VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyFramebuffer(dump_inst, device, framebuffer, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyFramebuffer(dump_inst, device, framebuffer, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyFramebuffer(dump_inst, device, framebuffer, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkCmdExecuteCommands(ApiDumpInstance& dump_inst, VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkCmdExecuteCommands(dump_inst, commandBuffer, commandBufferCount, pCommandBuffers);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkCmdExecuteCommands(dump_inst, commandBuffer, commandBufferCount, pCommandBuffers);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkCmdExecuteCommands(dump_inst, commandBuffer, commandBufferCount, pCommandBuffers);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyFence(ApiDumpInstance& dump_inst, VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyFence(dump_inst, device, fence, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyFence(dump_inst, device, fence, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyFence(dump_inst, device, fence, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyImageView(ApiDumpInstance& dump_inst, VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyImageView(dump_inst, device, imageView, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyImageView(dump_inst, device, imageView, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyImageView(dump_inst, device, imageView, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceExternalFenceProperties(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceExternalFenceProperties(dump_inst, physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceExternalFenceProperties(dump_inst, physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceExternalFenceProperties(dump_inst, physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroySemaphore(ApiDumpInstance& dump_inst, VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroySemaphore(dump_inst, device, semaphore, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroySemaphore(dump_inst, device, semaphore, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroySemaphore(dump_inst, device, semaphore, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyShaderModule(ApiDumpInstance& dump_inst, VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyShaderModule(dump_inst, device, shaderModule, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyShaderModule(dump_inst, device, shaderModule, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyShaderModule(dump_inst, device, shaderModule, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyInstance(ApiDumpInstance& dump_inst, VkInstance instance, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyInstance(dump_inst, instance, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyInstance(dump_inst, instance, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyInstance(dump_inst, instance, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkUnmapMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkUnmapMemory(dump_inst, device, memory);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkUnmapMemory(dump_inst, device, memory);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkUnmapMemory(dump_inst, device, memory);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkFreeMemory(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkFreeMemory(dump_inst, device, memory, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkFreeMemory(dump_inst, device, memory, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkFreeMemory(dump_inst, device, memory, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceFeatures(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceFeatures(dump_inst, physicalDevice, pFeatures);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceFeatures(dump_inst, physicalDevice, pFeatures);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceFeatures(dump_inst, physicalDevice, pFeatures);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetDeviceMemoryCommitment(ApiDumpInstance& dump_inst, VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetDeviceMemoryCommitment(dump_inst, device, memory, pCommittedMemoryInBytes);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetDeviceMemoryCommitment(dump_inst, device, memory, pCommittedMemoryInBytes);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetDeviceMemoryCommitment(dump_inst, device, memory, pCommittedMemoryInBytes);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDebugReportMessageEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDebugReportMessageEXT(dump_inst, instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDebugReportMessageEXT(dump_inst, instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDebugReportMessageEXT(dump_inst, instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetImageSparseMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetImageSparseMemoryRequirements(dump_inst, device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetImageSparseMemoryRequirements(dump_inst, device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetImageSparseMemoryRequirements(dump_inst, device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetBufferMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetBufferMemoryRequirements(dump_inst, device, buffer, pMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetBufferMemoryRequirements(dump_inst, device, buffer, pMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetBufferMemoryRequirements(dump_inst, device, buffer, pMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyDebugReportCallbackEXT(ApiDumpInstance& dump_inst, VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyDebugReportCallbackEXT(dump_inst, instance, callback, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyDebugReportCallbackEXT(dump_inst, instance, callback, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyDebugReportCallbackEXT(dump_inst, instance, callback, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(ApiDumpInstance& dump_inst, VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(dump_inst, physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(dump_inst, physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(dump_inst, physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkGetImageMemoryRequirements(ApiDumpInstance& dump_inst, VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkGetImageMemoryRequirements(dump_inst, device, image, pMemoryRequirements);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkGetImageMemoryRequirements(dump_inst, device, image, pMemoryRequirements);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkGetImageMemoryRequirements(dump_inst, device, image, pMemoryRequirements);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}
inline void dump_body_vkDestroyBuffer(ApiDumpInstance& dump_inst, VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator)
{
    if (!dump_inst.shouldDumpOutput()) return ;
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    switch(dump_inst.settings().format())
    {
    case ApiDumpFormat::Text:
        dump_text_body_vkDestroyBuffer(dump_inst, device, buffer, pAllocator);
        break;
    case ApiDumpFormat::Html:
        dump_html_body_vkDestroyBuffer(dump_inst, device, buffer, pAllocator);
        break;
    case ApiDumpFormat::Json:
        dump_json_body_vkDestroyBuffer(dump_inst, device, buffer, pAllocator);
        break;
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}


inline void dump_head_vkDebugMarkerSetObjectNameEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo)
{
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());

    if (pNameInfo->pObjectName)
    {
        dump_inst.object_name_map.insert(std::make_pair<uint64_t, std::string>((uint64_t &&)pNameInfo->object, pNameInfo->pObjectName));
    }
    else
    {
        dump_inst.object_name_map.erase(pNameInfo->object);
    }

    if (dump_inst.shouldDumpOutput()) {
        switch(dump_inst.settings().format())
        {
        case ApiDumpFormat::Text:
            dump_text_head_vkDebugMarkerSetObjectNameEXT(dump_inst, device, pNameInfo);
            break;
        case ApiDumpFormat::Html:
            dump_html_head_vkDebugMarkerSetObjectNameEXT(dump_inst, device, pNameInfo);
            break;
        case ApiDumpFormat::Json:
            dump_json_head_vkDebugMarkerSetObjectNameEXT(dump_inst, device, pNameInfo);
            break;
        }
    }

    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}


inline void dump_body_vkDebugMarkerSetObjectNameEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo)
{
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    if (dump_inst.shouldDumpOutput()) {
        switch(dump_inst.settings().format())
        {
        case ApiDumpFormat::Text:
            dump_text_body_vkDebugMarkerSetObjectNameEXT(dump_inst, result, device, pNameInfo);
            break;
        case ApiDumpFormat::Html:
            dump_html_body_vkDebugMarkerSetObjectNameEXT(dump_inst, result, device, pNameInfo);
            break;
        case ApiDumpFormat::Json:
            dump_json_body_vkDebugMarkerSetObjectNameEXT(dump_inst, result, device, pNameInfo);
            break;
        }
    }

    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}


inline void dump_head_vkSetDebugUtilsObjectNameEXT(ApiDumpInstance& dump_inst, VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo)
{
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    if (pNameInfo->pObjectName)
    {
        dump_inst.object_name_map.insert(std::make_pair<uint64_t, std::string>((uint64_t &&)pNameInfo->objectHandle, pNameInfo->pObjectName));
    }
    else
    {
        dump_inst.object_name_map.erase(pNameInfo->objectHandle);
    }
    if (dump_inst.shouldDumpOutput()) {
        switch(dump_inst.settings().format())
        {
        case ApiDumpFormat::Text:
            dump_text_head_vkSetDebugUtilsObjectNameEXT(dump_inst, device, pNameInfo);
            break;
        case ApiDumpFormat::Html:
            dump_html_head_vkSetDebugUtilsObjectNameEXT(dump_inst, device, pNameInfo);
            break;
        case ApiDumpFormat::Json:
            dump_json_head_vkSetDebugUtilsObjectNameEXT(dump_inst, device, pNameInfo);
            break;
        }
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}


inline void dump_body_vkSetDebugUtilsObjectNameEXT(ApiDumpInstance& dump_inst, VkResult result, VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo)
{
    loader_platform_thread_lock_mutex(dump_inst.outputMutex());
    if (dump_inst.shouldDumpOutput()) {
        switch(dump_inst.settings().format())
        {
        case ApiDumpFormat::Text:
            dump_text_body_vkSetDebugUtilsObjectNameEXT(dump_inst, result, device, pNameInfo);
            break;
        case ApiDumpFormat::Html:
            dump_html_body_vkSetDebugUtilsObjectNameEXT(dump_inst, result, device, pNameInfo);
            break;
        case ApiDumpFormat::Json:
            dump_json_body_vkSetDebugUtilsObjectNameEXT(dump_inst, result, device, pNameInfo);
            break;
        }
    }
    loader_platform_thread_unlock_mutex(dump_inst.outputMutex());
}

//============================= API EntryPoints =============================//

// Specifically implemented functions

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
    dump_head_vkCreateInstance(ApiDumpInstance::current(), pCreateInfo, pAllocator, pInstance);

    // Get the function pointer
    VkLayerInstanceCreateInfo* chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);
    assert(chain_info->u.pLayerInfo != 0);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    assert(fpGetInstanceProcAddr != 0);
    PFN_vkCreateInstance fpCreateInstance = (PFN_vkCreateInstance) fpGetInstanceProcAddr(NULL, "vkCreateInstance");
    if(fpCreateInstance == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Call the function and create the dispatch table
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;
    VkResult result = fpCreateInstance(pCreateInfo, pAllocator, pInstance);
    if(result == VK_SUCCESS) {
        initInstanceTable(*pInstance, fpGetInstanceProcAddr);
    }
    
    // Output the API dump
    dump_body_vkCreateInstance(ApiDumpInstance::current(), result, pCreateInfo, pAllocator, pInstance);
    return result;
}


VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyInstance(ApiDumpInstance::current(), instance, pAllocator);
    // Destroy the dispatch table
    dispatch_key key = get_dispatch_key(instance);
    instance_dispatch_table(instance)->DestroyInstance(instance, pAllocator);
    destroy_instance_dispatch_table(key);
    
    // Output the API dump
    dump_body_vkDestroyInstance(ApiDumpInstance::current(), instance, pAllocator);
}


VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    dump_head_vkCreateDevice(ApiDumpInstance::current(), physicalDevice, pCreateInfo, pAllocator, pDevice);

    // Get the function pointer
    VkLayerDeviceCreateInfo* chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);
    assert(chain_info->u.pLayerInfo != 0);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr = chain_info->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    PFN_vkCreateDevice fpCreateDevice = (PFN_vkCreateDevice) fpGetInstanceProcAddr(NULL, "vkCreateDevice");
    if(fpCreateDevice == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Call the function and create the dispatch table
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;
    VkResult result = fpCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
    if(result == VK_SUCCESS) {
        initDeviceTable(*pDevice, fpGetDeviceProcAddr);
    }
    
    // Output the API dump
    dump_body_vkCreateDevice(ApiDumpInstance::current(), result, physicalDevice, pCreateInfo, pAllocator, pDevice);
    return result;
}


VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyDevice(ApiDumpInstance::current(), device, pAllocator);

    // Destroy the dispatch table
    dispatch_key key = get_dispatch_key(device);
    device_dispatch_table(device)->DestroyDevice(device, pAllocator);
    destroy_device_dispatch_table(key);
    
    // Output the API dump
    dump_body_vkDestroyDevice(ApiDumpInstance::current(), device, pAllocator);
}


VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
{
    return util_GetExtensionProperties(0, NULL, pPropertyCount, pProperties);
}


VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    static const VkLayerProperties layerProperties[] = {
        {
            "VK_LAYER_LUNARG_api_dump",
            VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION), // specVersion
            VK_MAKE_VERSION(0, 2, 0), // implementationVersion
            "layer: api_dump",
        }
    };

    return util_GetLayerProperties(ARRAY_SIZE(layerProperties), layerProperties, pPropertyCount, pProperties);
}


VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties)
{
    static const VkLayerProperties layerProperties[] = {
        {
            "VK_LAYER_LUNARG_api_dump",
            VK_MAKE_VERSION(1, 0, VK_HEADER_VERSION),
            VK_MAKE_VERSION(0, 2, 0),
            "layer: api_dump",
        }
    };

    return util_GetLayerProperties(ARRAY_SIZE(layerProperties), layerProperties, pPropertyCount, pProperties);
}


VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
{
    dump_head_vkQueuePresentKHR(ApiDumpInstance::current(), queue, pPresentInfo);

    VkResult result = device_dispatch_table(queue)->QueuePresentKHR(queue, pPresentInfo);
    
    dump_body_vkQueuePresentKHR(ApiDumpInstance::current(), result, queue, pPresentInfo);

    ApiDumpInstance::current().nextFrame();
    return result;
}

// Autogen instance functions

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays)
{
    dump_head_vkGetDisplayPlaneSupportedDisplaysKHR(ApiDumpInstance::current(), physicalDevice, planeIndex, pDisplayCount, pDisplays);
    VkResult result = instance_dispatch_table(physicalDevice)->GetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, pDisplayCount, pDisplays);
    
    dump_body_vkGetDisplayPlaneSupportedDisplaysKHR(ApiDumpInstance::current(), result, physicalDevice, planeIndex, pDisplayCount, pDisplays);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties)
{
    dump_head_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(ApiDumpInstance::current(), physicalDevice, pPropertyCount, pProperties);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceCooperativeMatrixPropertiesNV(physicalDevice, pPropertyCount, pProperties);
    
    dump_body_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(ApiDumpInstance::current(), result, physicalDevice, pPropertyCount, pProperties);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties)
{
    dump_head_vkGetPhysicalDeviceDisplayPropertiesKHR(ApiDumpInstance::current(), physicalDevice, pPropertyCount, pProperties);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
    
    dump_body_vkGetPhysicalDeviceDisplayPropertiesKHR(ApiDumpInstance::current(), result, physicalDevice, pPropertyCount, pProperties);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains)
{
    dump_head_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(ApiDumpInstance::current(), physicalDevice, pTimeDomainCount, pTimeDomains);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceCalibrateableTimeDomainsEXT(physicalDevice, pTimeDomainCount, pTimeDomains);
    
    dump_body_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(ApiDumpInstance::current(), result, physicalDevice, pTimeDomainCount, pTimeDomains);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties)
{
    dump_head_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(ApiDumpInstance::current(), physicalDevice, pPropertyCount, pProperties);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, pPropertyCount, pProperties);
    
    dump_body_vkGetPhysicalDeviceDisplayPlanePropertiesKHR(ApiDumpInstance::current(), result, physicalDevice, pPropertyCount, pProperties);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties)
{
    dump_head_vkGetDisplayModePropertiesKHR(ApiDumpInstance::current(), physicalDevice, display, pPropertyCount, pProperties);
    VkResult result = instance_dispatch_table(physicalDevice)->GetDisplayModePropertiesKHR(physicalDevice, display, pPropertyCount, pProperties);
    
    dump_body_vkGetDisplayModePropertiesKHR(ApiDumpInstance::current(), result, physicalDevice, display, pPropertyCount, pProperties);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode)
{
    dump_head_vkCreateDisplayModeKHR(ApiDumpInstance::current(), physicalDevice, display, pCreateInfo, pAllocator, pMode);
    VkResult result = instance_dispatch_table(physicalDevice)->CreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
    
    dump_body_vkCreateDisplayModeKHR(ApiDumpInstance::current(), result, physicalDevice, display, pCreateInfo, pAllocator, pMode);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities)
{
    dump_head_vkGetDisplayPlaneCapabilitiesKHR(ApiDumpInstance::current(), physicalDevice, mode, planeIndex, pCapabilities);
    VkResult result = instance_dispatch_table(physicalDevice)->GetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
    
    dump_body_vkGetDisplayPlaneCapabilitiesKHR(ApiDumpInstance::current(), result, physicalDevice, mode, planeIndex, pCapabilities);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations)
{
    dump_head_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(ApiDumpInstance::current(), physicalDevice, pCombinationCount, pCombinations);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(physicalDevice, pCombinationCount, pCombinations);
    
    dump_body_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(ApiDumpInstance::current(), result, physicalDevice, pCombinationCount, pCombinations);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateDisplayPlaneSurfaceKHR(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    dump_head_vkCreateDisplayPlaneSurfaceKHR(ApiDumpInstance::current(), instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_dispatch_table(instance)->CreateDisplayPlaneSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    
    dump_body_vkCreateDisplayPlaneSurfaceKHR(ApiDumpInstance::current(), result, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModes2EXT(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
{
    dump_head_vkGetPhysicalDeviceSurfacePresentModes2EXT(ApiDumpInstance::current(), physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSurfacePresentModes2EXT(physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
    
    dump_body_vkGetPhysicalDeviceSurfacePresentModes2EXT(ApiDumpInstance::current(), result, physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
    return result;
}
#endif // VK_USE_PLATFORM_WIN32_KHR
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties)
{
    dump_head_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(ApiDumpInstance::current(), physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceExternalImageFormatPropertiesNV(physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
    
    dump_body_vkGetPhysicalDeviceExternalImageFormatPropertiesNV(ApiDumpInstance::current(), result, physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
    return result;
}
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateWaylandSurfaceKHR(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    dump_head_vkCreateWaylandSurfaceKHR(ApiDumpInstance::current(), instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_dispatch_table(instance)->CreateWaylandSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    
    dump_body_vkCreateWaylandSurfaceKHR(ApiDumpInstance::current(), result, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#endif // VK_USE_PLATFORM_WAYLAND_KHR
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceWaylandPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display)
{
    dump_head_vkGetPhysicalDeviceWaylandPresentationSupportKHR(ApiDumpInstance::current(), physicalDevice, queueFamilyIndex, display);
    VkBool32 result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceWaylandPresentationSupportKHR(physicalDevice, queueFamilyIndex, display);
    
    dump_body_vkGetPhysicalDeviceWaylandPresentationSupportKHR(ApiDumpInstance::current(), result, physicalDevice, queueFamilyIndex, display);
    return result;
}
#endif // VK_USE_PLATFORM_WAYLAND_KHR
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroupsKHR(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties)
{
    dump_head_vkEnumeratePhysicalDeviceGroupsKHR(ApiDumpInstance::current(), instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
    VkResult result = instance_dispatch_table(instance)->EnumeratePhysicalDeviceGroupsKHR(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
    
    dump_body_vkEnumeratePhysicalDeviceGroupsKHR(ApiDumpInstance::current(), result, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
    return result;
}
#if defined(VK_USE_PLATFORM_XCB_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateXcbSurfaceKHR(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    dump_head_vkCreateXcbSurfaceKHR(ApiDumpInstance::current(), instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_dispatch_table(instance)->CreateXcbSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    
    dump_body_vkCreateXcbSurfaceKHR(ApiDumpInstance::current(), result, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#endif // VK_USE_PLATFORM_XCB_KHR
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateHeadlessSurfaceEXT(VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    dump_head_vkCreateHeadlessSurfaceEXT(ApiDumpInstance::current(), instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_dispatch_table(instance)->CreateHeadlessSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
    
    dump_body_vkCreateHeadlessSurfaceEXT(ApiDumpInstance::current(), result, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties)
{
    dump_head_vkGetDisplayModeProperties2KHR(ApiDumpInstance::current(), physicalDevice, display, pPropertyCount, pProperties);
    VkResult result = instance_dispatch_table(physicalDevice)->GetDisplayModeProperties2KHR(physicalDevice, display, pPropertyCount, pProperties);
    
    dump_body_vkGetDisplayModeProperties2KHR(ApiDumpInstance::current(), result, physicalDevice, display, pPropertyCount, pProperties);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger)
{
    dump_head_vkCreateDebugUtilsMessengerEXT(ApiDumpInstance::current(), instance, pCreateInfo, pAllocator, pMessenger);
    VkResult result = instance_dispatch_table(instance)->CreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
    
    dump_body_vkCreateDebugUtilsMessengerEXT(ApiDumpInstance::current(), result, instance, pCreateInfo, pAllocator, pMessenger);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display)
{
    dump_head_vkReleaseDisplayEXT(ApiDumpInstance::current(), physicalDevice, display);
    VkResult result = instance_dispatch_table(physicalDevice)->ReleaseDisplayEXT(physicalDevice, display);
    
    dump_body_vkReleaseDisplayEXT(ApiDumpInstance::current(), result, physicalDevice, display);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties)
{
    dump_head_vkGetPhysicalDeviceDisplayProperties2KHR(ApiDumpInstance::current(), physicalDevice, pPropertyCount, pProperties);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceDisplayProperties2KHR(physicalDevice, pPropertyCount, pProperties);
    
    dump_body_vkGetPhysicalDeviceDisplayProperties2KHR(ApiDumpInstance::current(), result, physicalDevice, pPropertyCount, pProperties);
    return result;
}
#if defined(VK_USE_PLATFORM_FUCHSIA)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateImagePipeSurfaceFUCHSIA(VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    dump_head_vkCreateImagePipeSurfaceFUCHSIA(ApiDumpInstance::current(), instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_dispatch_table(instance)->CreateImagePipeSurfaceFUCHSIA(instance, pCreateInfo, pAllocator, pSurface);
    
    dump_body_vkCreateImagePipeSurfaceFUCHSIA(ApiDumpInstance::current(), result, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#endif // VK_USE_PLATFORM_FUCHSIA
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties)
{
    dump_head_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(ApiDumpInstance::current(), physicalDevice, pPropertyCount, pProperties);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceDisplayPlaneProperties2KHR(physicalDevice, pPropertyCount, pProperties);
    
    dump_body_vkGetPhysicalDeviceDisplayPlaneProperties2KHR(ApiDumpInstance::current(), result, physicalDevice, pPropertyCount, pProperties);
    return result;
}
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateAndroidSurfaceKHR(VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    dump_head_vkCreateAndroidSurfaceKHR(ApiDumpInstance::current(), instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_dispatch_table(instance)->CreateAndroidSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    
    dump_body_vkCreateAndroidSurfaceKHR(ApiDumpInstance::current(), result, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateXlibSurfaceKHR(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    dump_head_vkCreateXlibSurfaceKHR(ApiDumpInstance::current(), instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_dispatch_table(instance)->CreateXlibSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    
    dump_body_vkCreateXlibSurfaceKHR(ApiDumpInstance::current(), result, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#endif // VK_USE_PLATFORM_XLIB_KHR
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities)
{
    dump_head_vkGetDisplayPlaneCapabilities2KHR(ApiDumpInstance::current(), physicalDevice, pDisplayPlaneInfo, pCapabilities);
    VkResult result = instance_dispatch_table(physicalDevice)->GetDisplayPlaneCapabilities2KHR(physicalDevice, pDisplayPlaneInfo, pCapabilities);
    
    dump_body_vkGetDisplayPlaneCapabilities2KHR(ApiDumpInstance::current(), result, physicalDevice, pDisplayPlaneInfo, pCapabilities);
    return result;
}
#if defined(VK_USE_PLATFORM_METAL_EXT)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateMetalSurfaceEXT(VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    dump_head_vkCreateMetalSurfaceEXT(ApiDumpInstance::current(), instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_dispatch_table(instance)->CreateMetalSurfaceEXT(instance, pCreateInfo, pAllocator, pSurface);
    
    dump_body_vkCreateMetalSurfaceEXT(ApiDumpInstance::current(), result, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetRandROutputDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay)
{
    dump_head_vkGetRandROutputDisplayEXT(ApiDumpInstance::current(), physicalDevice, dpy, rrOutput, pDisplay);
    VkResult result = instance_dispatch_table(physicalDevice)->GetRandROutputDisplayEXT(physicalDevice, dpy, rrOutput, pDisplay);
    
    dump_body_vkGetRandROutputDisplayEXT(ApiDumpInstance::current(), result, physicalDevice, dpy, rrOutput, pDisplay);
    return result;
}
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#if defined(VK_USE_PLATFORM_XLIB_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID)
{
    dump_head_vkGetPhysicalDeviceXlibPresentationSupportKHR(ApiDumpInstance::current(), physicalDevice, queueFamilyIndex, dpy, visualID);
    VkBool32 result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice, queueFamilyIndex, dpy, visualID);
    
    dump_body_vkGetPhysicalDeviceXlibPresentationSupportKHR(ApiDumpInstance::current(), result, physicalDevice, queueFamilyIndex, dpy, visualID);
    return result;
}
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_IOS_MVK)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateIOSSurfaceMVK(VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    dump_head_vkCreateIOSSurfaceMVK(ApiDumpInstance::current(), instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_dispatch_table(instance)->CreateIOSSurfaceMVK(instance, pCreateInfo, pAllocator, pSurface);
    
    dump_body_vkCreateIOSSurfaceMVK(ApiDumpInstance::current(), result, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#endif // VK_USE_PLATFORM_IOS_MVK
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkAcquireXlibDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display)
{
    dump_head_vkAcquireXlibDisplayEXT(ApiDumpInstance::current(), physicalDevice, dpy, display);
    VkResult result = instance_dispatch_table(physicalDevice)->AcquireXlibDisplayEXT(physicalDevice, dpy, display);
    
    dump_body_vkAcquireXlibDisplayEXT(ApiDumpInstance::current(), result, physicalDevice, dpy, display);
    return result;
}
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
{
    dump_head_vkGetPhysicalDeviceSurfaceSupportKHR(ApiDumpInstance::current(), physicalDevice, queueFamilyIndex, surface, pSupported);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
    
    dump_body_vkGetPhysicalDeviceSurfaceSupportKHR(ApiDumpInstance::current(), result, physicalDevice, queueFamilyIndex, surface, pSupported);
    return result;
}
#if defined(VK_USE_PLATFORM_GGP)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateStreamDescriptorSurfaceGGP(VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    dump_head_vkCreateStreamDescriptorSurfaceGGP(ApiDumpInstance::current(), instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_dispatch_table(instance)->CreateStreamDescriptorSurfaceGGP(instance, pCreateInfo, pAllocator, pSurface);
    
    dump_body_vkCreateStreamDescriptorSurfaceGGP(ApiDumpInstance::current(), result, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#endif // VK_USE_PLATFORM_GGP
#if defined(VK_USE_PLATFORM_MACOS_MVK)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateMacOSSurfaceMVK(VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    dump_head_vkCreateMacOSSurfaceMVK(ApiDumpInstance::current(), instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_dispatch_table(instance)->CreateMacOSSurfaceMVK(instance, pCreateInfo, pAllocator, pSurface);
    
    dump_body_vkCreateMacOSSurfaceMVK(ApiDumpInstance::current(), result, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#endif // VK_USE_PLATFORM_MACOS_MVK
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats)
{
    dump_head_vkGetPhysicalDeviceSurfaceFormatsKHR(ApiDumpInstance::current(), physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
    
    dump_body_vkGetPhysicalDeviceSurfaceFormatsKHR(ApiDumpInstance::current(), result, physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities)
{
    dump_head_vkGetPhysicalDeviceSurfaceCapabilities2EXT(ApiDumpInstance::current(), physicalDevice, surface, pSurfaceCapabilities);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice, surface, pSurfaceCapabilities);
    
    dump_body_vkGetPhysicalDeviceSurfaceCapabilities2EXT(ApiDumpInstance::current(), result, physicalDevice, surface, pSurfaceCapabilities);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
    dump_head_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ApiDumpInstance::current(), physicalDevice, surface, pSurfaceCapabilities);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
    
    dump_body_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ApiDumpInstance::current(), result, physicalDevice, surface, pSurfaceCapabilities);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats)
{
    dump_head_vkGetPhysicalDeviceSurfaceFormats2KHR(ApiDumpInstance::current(), physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSurfaceFormats2KHR(physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
    
    dump_body_vkGetPhysicalDeviceSurfaceFormats2KHR(ApiDumpInstance::current(), result, physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities)
{
    dump_head_vkGetPhysicalDeviceSurfaceCapabilities2KHR(ApiDumpInstance::current(), physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
    
    dump_body_vkGetPhysicalDeviceSurfaceCapabilities2KHR(ApiDumpInstance::current(), result, physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
    dump_head_vkGetPhysicalDeviceImageFormatProperties2(ApiDumpInstance::current(), physicalDevice, pImageFormatInfo, pImageFormatProperties);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceImageFormatProperties2(physicalDevice, pImageFormatInfo, pImageFormatProperties);
    
    dump_body_vkGetPhysicalDeviceImageFormatProperties2(ApiDumpInstance::current(), result, physicalDevice, pImageFormatInfo, pImageFormatProperties);
    return result;
}
#if defined(VK_USE_PLATFORM_XCB_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceXcbPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id)
{
    dump_head_vkGetPhysicalDeviceXcbPresentationSupportKHR(ApiDumpInstance::current(), physicalDevice, queueFamilyIndex, connection, visual_id);
    VkBool32 result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceXcbPresentationSupportKHR(physicalDevice, queueFamilyIndex, connection, visual_id);
    
    dump_body_vkGetPhysicalDeviceXcbPresentationSupportKHR(ApiDumpInstance::current(), result, physicalDevice, queueFamilyIndex, connection, visual_id);
    return result;
}
#endif // VK_USE_PLATFORM_XCB_KHR
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
{
    dump_head_vkGetPhysicalDeviceSurfacePresentModesKHR(ApiDumpInstance::current(), physicalDevice, surface, pPresentModeCount, pPresentModes);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
    
    dump_body_vkGetPhysicalDeviceSurfacePresentModesKHR(ApiDumpInstance::current(), result, physicalDevice, surface, pPresentModeCount, pPresentModes);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties)
{
    dump_head_vkGetPhysicalDeviceImageFormatProperties2KHR(ApiDumpInstance::current(), physicalDevice, pImageFormatInfo, pImageFormatProperties);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceImageFormatProperties2KHR(physicalDevice, pImageFormatInfo, pImageFormatProperties);
    
    dump_body_vkGetPhysicalDeviceImageFormatProperties2KHR(ApiDumpInstance::current(), result, physicalDevice, pImageFormatInfo, pImageFormatProperties);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects)
{
    dump_head_vkGetPhysicalDevicePresentRectanglesKHR(ApiDumpInstance::current(), physicalDevice, surface, pRectCount, pRects);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDevicePresentRectanglesKHR(physicalDevice, surface, pRectCount, pRects);
    
    dump_body_vkGetPhysicalDevicePresentRectanglesKHR(ApiDumpInstance::current(), result, physicalDevice, surface, pRectCount, pRects);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties)
{
    dump_head_vkEnumeratePhysicalDeviceGroups(ApiDumpInstance::current(), instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
    VkResult result = instance_dispatch_table(instance)->EnumeratePhysicalDeviceGroups(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
    
    dump_body_vkEnumeratePhysicalDeviceGroups(ApiDumpInstance::current(), result, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
    return result;
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateWin32SurfaceKHR(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    dump_head_vkCreateWin32SurfaceKHR(ApiDumpInstance::current(), instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_dispatch_table(instance)->CreateWin32SurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    
    dump_body_vkCreateWin32SurfaceKHR(ApiDumpInstance::current(), result, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceWin32PresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex)
{
    dump_head_vkGetPhysicalDeviceWin32PresentationSupportKHR(ApiDumpInstance::current(), physicalDevice, queueFamilyIndex);
    VkBool32 result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, queueFamilyIndex);
    
    dump_body_vkGetPhysicalDeviceWin32PresentationSupportKHR(ApiDumpInstance::current(), result, physicalDevice, queueFamilyIndex);
    return result;
}
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_VI_NN)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateViSurfaceNN(VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
    dump_head_vkCreateViSurfaceNN(ApiDumpInstance::current(), instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_dispatch_table(instance)->CreateViSurfaceNN(instance, pCreateInfo, pAllocator, pSurface);
    
    dump_body_vkCreateViSurfaceNN(ApiDumpInstance::current(), result, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#endif // VK_USE_PLATFORM_VI_NN
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices)
{
    dump_head_vkEnumeratePhysicalDevices(ApiDumpInstance::current(), instance, pPhysicalDeviceCount, pPhysicalDevices);
    VkResult result = instance_dispatch_table(instance)->EnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
    
    dump_body_vkEnumeratePhysicalDevices(ApiDumpInstance::current(), result, instance, pPhysicalDeviceCount, pPhysicalDevices);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
    dump_head_vkCreateDebugReportCallbackEXT(ApiDumpInstance::current(), instance, pCreateInfo, pAllocator, pCallback);
    VkResult result = instance_dispatch_table(instance)->CreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
    
    dump_body_vkCreateDebugReportCallbackEXT(ApiDumpInstance::current(), result, instance, pCreateInfo, pAllocator, pCallback);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties)
{
    dump_head_vkGetPhysicalDeviceImageFormatProperties(ApiDumpInstance::current(), physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
    VkResult result = instance_dispatch_table(physicalDevice)->GetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
    
    dump_body_vkGetPhysicalDeviceImageFormatProperties(ApiDumpInstance::current(), result, physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
    return result;
}


VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties)
{
    dump_head_vkGetPhysicalDeviceProperties(ApiDumpInstance::current(), physicalDevice, pProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceProperties(physicalDevice, pProperties);
    
    dump_body_vkGetPhysicalDeviceProperties(ApiDumpInstance::current(), physicalDevice, pProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties)
{
    dump_head_vkGetPhysicalDeviceMultisamplePropertiesEXT(ApiDumpInstance::current(), physicalDevice, samples, pMultisampleProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceMultisamplePropertiesEXT(physicalDevice, samples, pMultisampleProperties);
    
    dump_body_vkGetPhysicalDeviceMultisamplePropertiesEXT(ApiDumpInstance::current(), physicalDevice, samples, pMultisampleProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties)
{
    dump_head_vkGetPhysicalDeviceMemoryProperties(ApiDumpInstance::current(), physicalDevice, pMemoryProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
    
    dump_body_vkGetPhysicalDeviceMemoryProperties(ApiDumpInstance::current(), physicalDevice, pMemoryProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties)
{
    dump_head_vkGetPhysicalDeviceQueueFamilyProperties(ApiDumpInstance::current(), physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceQueueFamilyProperties(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
    
    dump_body_vkGetPhysicalDeviceQueueFamilyProperties(ApiDumpInstance::current(), physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
    dump_head_vkGetPhysicalDeviceExternalBufferPropertiesKHR(ApiDumpInstance::current(), physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceExternalBufferPropertiesKHR(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
    
    dump_body_vkGetPhysicalDeviceExternalBufferPropertiesKHR(ApiDumpInstance::current(), physicalDevice, pExternalBufferInfo, pExternalBufferProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(VkPhysicalDevice physicalDevice, VkDeviceGeneratedCommandsFeaturesNVX* pFeatures, VkDeviceGeneratedCommandsLimitsNVX* pLimits)
{
    dump_head_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(ApiDumpInstance::current(), physicalDevice, pFeatures, pLimits);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceGeneratedCommandsPropertiesNVX(physicalDevice, pFeatures, pLimits);
    
    dump_body_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(ApiDumpInstance::current(), physicalDevice, pFeatures, pLimits);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkSubmitDebugUtilsMessageEXT(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
    dump_head_vkSubmitDebugUtilsMessageEXT(ApiDumpInstance::current(), instance, messageSeverity, messageTypes, pCallbackData);
    instance_dispatch_table(instance)->SubmitDebugUtilsMessageEXT(instance, messageSeverity, messageTypes, pCallbackData);
    
    dump_body_vkSubmitDebugUtilsMessageEXT(ApiDumpInstance::current(), instance, messageSeverity, messageTypes, pCallbackData);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyDebugUtilsMessengerEXT(ApiDumpInstance::current(), instance, messenger, pAllocator);
    instance_dispatch_table(instance)->DestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
    
    dump_body_vkDestroyDebugUtilsMessengerEXT(ApiDumpInstance::current(), instance, messenger, pAllocator);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
    dump_head_vkGetPhysicalDeviceExternalSemaphoreProperties(ApiDumpInstance::current(), physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceExternalSemaphoreProperties(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
    
    dump_body_vkGetPhysicalDeviceExternalSemaphoreProperties(ApiDumpInstance::current(), physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroySurfaceKHR(ApiDumpInstance::current(), instance, surface, pAllocator);
    instance_dispatch_table(instance)->DestroySurfaceKHR(instance, surface, pAllocator);
    
    dump_body_vkDestroySurfaceKHR(ApiDumpInstance::current(), instance, surface, pAllocator);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
    dump_head_vkGetPhysicalDeviceFeatures2(ApiDumpInstance::current(), physicalDevice, pFeatures);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceFeatures2(physicalDevice, pFeatures);
    
    dump_body_vkGetPhysicalDeviceFeatures2(ApiDumpInstance::current(), physicalDevice, pFeatures);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
    dump_head_vkGetPhysicalDeviceProperties2(ApiDumpInstance::current(), physicalDevice, pProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceProperties2(physicalDevice, pProperties);
    
    dump_body_vkGetPhysicalDeviceProperties2(ApiDumpInstance::current(), physicalDevice, pProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
    dump_head_vkGetPhysicalDeviceMemoryProperties2(ApiDumpInstance::current(), physicalDevice, pMemoryProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceMemoryProperties2(physicalDevice, pMemoryProperties);
    
    dump_body_vkGetPhysicalDeviceMemoryProperties2(ApiDumpInstance::current(), physicalDevice, pMemoryProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
    dump_head_vkGetPhysicalDeviceFormatProperties2(ApiDumpInstance::current(), physicalDevice, format, pFormatProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceFormatProperties2(physicalDevice, format, pFormatProperties);
    
    dump_body_vkGetPhysicalDeviceFormatProperties2(ApiDumpInstance::current(), physicalDevice, format, pFormatProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties)
{
    dump_head_vkGetPhysicalDeviceSparseImageFormatProperties2(ApiDumpInstance::current(), physicalDevice, pFormatInfo, pPropertyCount, pProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSparseImageFormatProperties2(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
    
    dump_body_vkGetPhysicalDeviceSparseImageFormatProperties2(ApiDumpInstance::current(), physicalDevice, pFormatInfo, pPropertyCount, pProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties)
{
    dump_head_vkGetPhysicalDeviceQueueFamilyProperties2(ApiDumpInstance::current(), physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceQueueFamilyProperties2(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
    
    dump_body_vkGetPhysicalDeviceQueueFamilyProperties2(ApiDumpInstance::current(), physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties)
{
    dump_head_vkGetPhysicalDeviceExternalBufferProperties(ApiDumpInstance::current(), physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceExternalBufferProperties(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
    
    dump_body_vkGetPhysicalDeviceExternalBufferProperties(ApiDumpInstance::current(), physicalDevice, pExternalBufferInfo, pExternalBufferProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
    dump_head_vkGetPhysicalDeviceFeatures2KHR(ApiDumpInstance::current(), physicalDevice, pFeatures);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceFeatures2KHR(physicalDevice, pFeatures);
    
    dump_body_vkGetPhysicalDeviceFeatures2KHR(ApiDumpInstance::current(), physicalDevice, pFeatures);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
    dump_head_vkGetPhysicalDeviceProperties2KHR(ApiDumpInstance::current(), physicalDevice, pProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceProperties2KHR(physicalDevice, pProperties);
    
    dump_body_vkGetPhysicalDeviceProperties2KHR(ApiDumpInstance::current(), physicalDevice, pProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties)
{
    dump_head_vkGetPhysicalDeviceMemoryProperties2KHR(ApiDumpInstance::current(), physicalDevice, pMemoryProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceMemoryProperties2KHR(physicalDevice, pMemoryProperties);
    
    dump_body_vkGetPhysicalDeviceMemoryProperties2KHR(ApiDumpInstance::current(), physicalDevice, pMemoryProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties)
{
    dump_head_vkGetPhysicalDeviceSparseImageFormatProperties(ApiDumpInstance::current(), physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSparseImageFormatProperties(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
    
    dump_body_vkGetPhysicalDeviceSparseImageFormatProperties(ApiDumpInstance::current(), physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties)
{
    dump_head_vkGetPhysicalDeviceFormatProperties2KHR(ApiDumpInstance::current(), physicalDevice, format, pFormatProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceFormatProperties2KHR(physicalDevice, format, pFormatProperties);
    
    dump_body_vkGetPhysicalDeviceFormatProperties2KHR(ApiDumpInstance::current(), physicalDevice, format, pFormatProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties)
{
    dump_head_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(ApiDumpInstance::current(), physicalDevice, pFormatInfo, pPropertyCount, pProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceSparseImageFormatProperties2KHR(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
    
    dump_body_vkGetPhysicalDeviceSparseImageFormatProperties2KHR(ApiDumpInstance::current(), physicalDevice, pFormatInfo, pPropertyCount, pProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties)
{
    dump_head_vkGetPhysicalDeviceQueueFamilyProperties2KHR(ApiDumpInstance::current(), physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceQueueFamilyProperties2KHR(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
    
    dump_body_vkGetPhysicalDeviceQueueFamilyProperties2KHR(ApiDumpInstance::current(), physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties)
{
    dump_head_vkGetPhysicalDeviceFormatProperties(ApiDumpInstance::current(), physicalDevice, format, pFormatProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
    
    dump_body_vkGetPhysicalDeviceFormatProperties(ApiDumpInstance::current(), physicalDevice, format, pFormatProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
    dump_head_vkGetPhysicalDeviceExternalFencePropertiesKHR(ApiDumpInstance::current(), physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceExternalFencePropertiesKHR(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
    
    dump_body_vkGetPhysicalDeviceExternalFencePropertiesKHR(ApiDumpInstance::current(), physicalDevice, pExternalFenceInfo, pExternalFenceProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties)
{
    dump_head_vkGetPhysicalDeviceExternalFenceProperties(ApiDumpInstance::current(), physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceExternalFenceProperties(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
    
    dump_body_vkGetPhysicalDeviceExternalFenceProperties(ApiDumpInstance::current(), physicalDevice, pExternalFenceInfo, pExternalFenceProperties);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures)
{
    dump_head_vkGetPhysicalDeviceFeatures(ApiDumpInstance::current(), physicalDevice, pFeatures);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceFeatures(physicalDevice, pFeatures);
    
    dump_body_vkGetPhysicalDeviceFeatures(ApiDumpInstance::current(), physicalDevice, pFeatures);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage)
{
    dump_head_vkDebugReportMessageEXT(ApiDumpInstance::current(), instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
    instance_dispatch_table(instance)->DebugReportMessageEXT(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
    
    dump_body_vkDebugReportMessageEXT(ApiDumpInstance::current(), instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyDebugReportCallbackEXT(ApiDumpInstance::current(), instance, callback, pAllocator);
    instance_dispatch_table(instance)->DestroyDebugReportCallbackEXT(instance, callback, pAllocator);
    
    dump_body_vkDestroyDebugReportCallbackEXT(ApiDumpInstance::current(), instance, callback, pAllocator);    
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties)
{
    dump_head_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(ApiDumpInstance::current(), physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
    instance_dispatch_table(physicalDevice)->GetPhysicalDeviceExternalSemaphorePropertiesKHR(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
    
    dump_body_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(ApiDumpInstance::current(), physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);    
}

// Autogen device functions

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache)
{
    dump_head_vkCreatePipelineCache(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pPipelineCache);
    VkResult result = device_dispatch_table(device)->CreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
    
    dump_body_vkCreatePipelineCache(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pPipelineCache);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    dump_head_vkCreateEvent(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pEvent);
    VkResult result = device_dispatch_table(device)->CreateEvent(device, pCreateInfo, pAllocator, pEvent);
    
    dump_body_vkCreateEvent(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pEvent);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages)
{
    dump_head_vkGetSwapchainImagesKHR(ApiDumpInstance::current(), device, swapchain, pSwapchainImageCount, pSwapchainImages);
    VkResult result = device_dispatch_table(device)->GetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
    
    dump_body_vkGetSwapchainImagesKHR(ApiDumpInstance::current(), result, device, swapchain, pSwapchainImageCount, pSwapchainImages);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetEventStatus(VkDevice device, VkEvent event)
{
    dump_head_vkGetEventStatus(ApiDumpInstance::current(), device, event);
    VkResult result = device_dispatch_table(device)->GetEventStatus(device, event);
    
    dump_body_vkGetEventStatus(ApiDumpInstance::current(), result, device, event);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
    dump_head_vkCreateImage(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pImage);
    VkResult result = device_dispatch_table(device)->CreateImage(device, pCreateInfo, pAllocator, pImage);
    
    dump_body_vkCreateImage(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pImage);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData)
{
    dump_head_vkGetPipelineCacheData(ApiDumpInstance::current(), device, pipelineCache, pDataSize, pData);
    VkResult result = device_dispatch_table(device)->GetPipelineCacheData(device, pipelineCache, pDataSize, pData);
    
    dump_body_vkGetPipelineCacheData(ApiDumpInstance::current(), result, device, pipelineCache, pDataSize, pData);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkSetEvent(VkDevice device, VkEvent event)
{
    dump_head_vkSetEvent(ApiDumpInstance::current(), device, event);
    VkResult result = device_dispatch_table(device)->SetEvent(device, event);
    
    dump_body_vkSetEvent(ApiDumpInstance::current(), result, device, event);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkMergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches)
{
    dump_head_vkMergePipelineCaches(ApiDumpInstance::current(), device, dstCache, srcCacheCount, pSrcCaches);
    VkResult result = device_dispatch_table(device)->MergePipelineCaches(device, dstCache, srcCacheCount, pSrcCaches);
    
    dump_body_vkMergePipelineCaches(ApiDumpInstance::current(), result, device, dstCache, srcCacheCount, pSrcCaches);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkResetEvent(VkDevice device, VkEvent event)
{
    dump_head_vkResetEvent(ApiDumpInstance::current(), device, event);
    VkResult result = device_dispatch_table(device)->ResetEvent(device, event);
    
    dump_body_vkResetEvent(ApiDumpInstance::current(), result, device, event);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    dump_head_vkCreateGraphicsPipelines(ApiDumpInstance::current(), device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    VkResult result = device_dispatch_table(device)->CreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    
    dump_body_vkCreateGraphicsPipelines(ApiDumpInstance::current(), result, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties)
{
    dump_head_vkGetMemoryHostPointerPropertiesEXT(ApiDumpInstance::current(), device, handleType, pHostPointer, pMemoryHostPointerProperties);
    VkResult result = device_dispatch_table(device)->GetMemoryHostPointerPropertiesEXT(device, handleType, pHostPointer, pMemoryHostPointerProperties);
    
    dump_body_vkGetMemoryHostPointerPropertiesEXT(ApiDumpInstance::current(), result, device, handleType, pHostPointer, pMemoryHostPointerProperties);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool)
{
    dump_head_vkCreateQueryPool(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pQueryPool);
    VkResult result = device_dispatch_table(device)->CreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
    
    dump_body_vkCreateQueryPool(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pQueryPool);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
{
    dump_head_vkAcquireNextImageKHR(ApiDumpInstance::current(), device, swapchain, timeout, semaphore, fence, pImageIndex);
    VkResult result = device_dispatch_table(device)->AcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
    
    dump_body_vkAcquireNextImageKHR(ApiDumpInstance::current(), result, device, swapchain, timeout, semaphore, fence, pImageIndex);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    dump_head_vkCreateComputePipelines(ApiDumpInstance::current(), device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    VkResult result = device_dispatch_table(device)->CreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    
    dump_body_vkCreateComputePipelines(ApiDumpInstance::current(), result, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation)
{
    dump_head_vkGetCalibratedTimestampsEXT(ApiDumpInstance::current(), device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
    VkResult result = device_dispatch_table(device)->GetCalibratedTimestampsEXT(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
    
    dump_body_vkGetCalibratedTimestampsEXT(ApiDumpInstance::current(), result, device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView)
{
    dump_head_vkCreateImageView(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pView);
    VkResult result = device_dispatch_table(device)->CreateImageView(device, pCreateInfo, pAllocator, pView);
    
    dump_body_vkCreateImageView(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pView);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout)
{
    dump_head_vkCreatePipelineLayout(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pPipelineLayout);
    VkResult result = device_dispatch_table(device)->CreatePipelineLayout(device, pCreateInfo, pAllocator, pPipelineLayout);
    
    dump_body_vkCreatePipelineLayout(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pPipelineLayout);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkBindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos)
{
    dump_head_vkBindAccelerationStructureMemoryNV(ApiDumpInstance::current(), device, bindInfoCount, pBindInfos);
    VkResult result = device_dispatch_table(device)->BindAccelerationStructureMemoryNV(device, bindInfoCount, pBindInfos);
    
    dump_body_vkBindAccelerationStructureMemoryNV(ApiDumpInstance::current(), result, device, bindInfoCount, pBindInfos);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags)
{
    dump_head_vkResetCommandBuffer(ApiDumpInstance::current(), commandBuffer, flags);
    VkResult result = device_dispatch_table(commandBuffer)->ResetCommandBuffer(commandBuffer, flags);
    
    dump_body_vkResetCommandBuffer(ApiDumpInstance::current(), result, commandBuffer, flags);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR uint32_t VKAPI_CALL vkGetImageViewHandleNVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo)
{
    dump_head_vkGetImageViewHandleNVX(ApiDumpInstance::current(), device, pInfo);
    uint32_t result = device_dispatch_table(device)->GetImageViewHandleNVX(device, pInfo);
    
    dump_body_vkGetImageViewHandleNVX(ApiDumpInstance::current(), result, device, pInfo);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkInitializePerformanceApiINTEL(VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo)
{
    dump_head_vkInitializePerformanceApiINTEL(ApiDumpInstance::current(), device, pInitializeInfo);
    VkResult result = device_dispatch_table(device)->InitializePerformanceApiINTEL(device, pInitializeInfo);
    
    dump_body_vkInitializePerformanceApiINTEL(ApiDumpInstance::current(), result, device, pInitializeInfo);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler)
{
    dump_head_vkCreateSampler(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pSampler);
    VkResult result = device_dispatch_table(device)->CreateSampler(device, pCreateInfo, pAllocator, pSampler);
    
    dump_body_vkCreateSampler(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pSampler);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo)
{
    dump_head_vkCmdSetPerformanceMarkerINTEL(ApiDumpInstance::current(), commandBuffer, pMarkerInfo);
    VkResult result = device_dispatch_table(commandBuffer)->CmdSetPerformanceMarkerINTEL(commandBuffer, pMarkerInfo);
    
    dump_body_vkCmdSetPerformanceMarkerINTEL(ApiDumpInstance::current(), result, commandBuffer, pMarkerInfo);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo)
{
    dump_head_vkCmdSetPerformanceStreamMarkerINTEL(ApiDumpInstance::current(), commandBuffer, pMarkerInfo);
    VkResult result = device_dispatch_table(commandBuffer)->CmdSetPerformanceStreamMarkerINTEL(commandBuffer, pMarkerInfo);
    
    dump_body_vkCmdSetPerformanceStreamMarkerINTEL(ApiDumpInstance::current(), result, commandBuffer, pMarkerInfo);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines)
{
    dump_head_vkCreateRayTracingPipelinesNV(ApiDumpInstance::current(), device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    VkResult result = device_dispatch_table(device)->CreateRayTracingPipelinesNV(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    
    dump_body_vkCreateRayTracingPipelinesNV(ApiDumpInstance::current(), result, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo)
{
    dump_head_vkCmdSetPerformanceOverrideINTEL(ApiDumpInstance::current(), commandBuffer, pOverrideInfo);
    VkResult result = device_dispatch_table(commandBuffer)->CmdSetPerformanceOverrideINTEL(commandBuffer, pOverrideInfo);
    
    dump_body_vkCmdSetPerformanceOverrideINTEL(ApiDumpInstance::current(), result, commandBuffer, pOverrideInfo);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration)
{
    dump_head_vkReleasePerformanceConfigurationINTEL(ApiDumpInstance::current(), device, configuration);
    VkResult result = device_dispatch_table(device)->ReleasePerformanceConfigurationINTEL(device, configuration);
    
    dump_body_vkReleasePerformanceConfigurationINTEL(ApiDumpInstance::current(), result, device, configuration);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkAcquirePerformanceConfigurationINTEL(VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration)
{
    dump_head_vkAcquirePerformanceConfigurationINTEL(ApiDumpInstance::current(), device, pAcquireInfo, pConfiguration);
    VkResult result = device_dispatch_table(device)->AcquirePerformanceConfigurationINTEL(device, pAcquireInfo, pConfiguration);
    
    dump_body_vkAcquirePerformanceConfigurationINTEL(ApiDumpInstance::current(), result, device, pAcquireInfo, pConfiguration);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfoEXT* pInfo)
{
    dump_head_vkGetBufferDeviceAddressEXT(ApiDumpInstance::current(), device, pInfo);
    VkDeviceAddress result = device_dispatch_table(device)->GetBufferDeviceAddressEXT(device, pInfo);
    
    dump_body_vkGetBufferDeviceAddressEXT(ApiDumpInstance::current(), result, device, pInfo);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains)
{
    dump_head_vkCreateSharedSwapchainsKHR(ApiDumpInstance::current(), device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
    VkResult result = device_dispatch_table(device)->CreateSharedSwapchainsKHR(device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
    
    dump_body_vkCreateSharedSwapchainsKHR(ApiDumpInstance::current(), result, device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkQueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration)
{
    dump_head_vkQueueSetPerformanceConfigurationINTEL(ApiDumpInstance::current(), queue, configuration);
    VkResult result = device_dispatch_table(queue)->QueueSetPerformanceConfigurationINTEL(queue, configuration);
    
    dump_body_vkQueueSetPerformanceConfigurationINTEL(ApiDumpInstance::current(), result, queue, configuration);
    return result;
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain)
{
    dump_head_vkReleaseFullScreenExclusiveModeEXT(ApiDumpInstance::current(), device, swapchain);
    VkResult result = device_dispatch_table(device)->ReleaseFullScreenExclusiveModeEXT(device, swapchain);
    
    dump_body_vkReleaseFullScreenExclusiveModeEXT(ApiDumpInstance::current(), result, device, swapchain);
    return result;
}
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkAcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain)
{
    dump_head_vkAcquireFullScreenExclusiveModeEXT(ApiDumpInstance::current(), device, swapchain);
    VkResult result = device_dispatch_table(device)->AcquireFullScreenExclusiveModeEXT(device, swapchain);
    
    dump_body_vkAcquireFullScreenExclusiveModeEXT(ApiDumpInstance::current(), result, device, swapchain);
    return result;
}
#endif // VK_USE_PLATFORM_WIN32_KHR
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData)
{
    dump_head_vkGetRayTracingShaderGroupHandlesNV(ApiDumpInstance::current(), device, pipeline, firstGroup, groupCount, dataSize, pData);
    VkResult result = device_dispatch_table(device)->GetRayTracingShaderGroupHandlesNV(device, pipeline, firstGroup, groupCount, dataSize, pData);
    
    dump_body_vkGetRayTracingShaderGroupHandlesNV(ApiDumpInstance::current(), result, device, pipeline, firstGroup, groupCount, dataSize, pData);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue)
{
    dump_head_vkGetPerformanceParameterINTEL(ApiDumpInstance::current(), device, parameter, pValue);
    VkResult result = device_dispatch_table(device)->GetPerformanceParameterINTEL(device, parameter, pValue);
    
    dump_body_vkGetPerformanceParameterINTEL(ApiDumpInstance::current(), result, device, parameter, pValue);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData)
{
    dump_head_vkGetAccelerationStructureHandleNV(ApiDumpInstance::current(), device, accelerationStructure, dataSize, pData);
    VkResult result = device_dispatch_table(device)->GetAccelerationStructureHandleNV(device, accelerationStructure, dataSize, pData);
    
    dump_body_vkGetAccelerationStructureHandleNV(ApiDumpInstance::current(), result, device, accelerationStructure, dataSize, pData);
    return result;
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetDeviceGroupSurfacePresentModes2EXT(VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes)
{
    dump_head_vkGetDeviceGroupSurfacePresentModes2EXT(ApiDumpInstance::current(), device, pSurfaceInfo, pModes);
    VkResult result = device_dispatch_table(device)->GetDeviceGroupSurfacePresentModes2EXT(device, pSurfaceInfo, pModes);
    
    dump_body_vkGetDeviceGroupSurfacePresentModes2EXT(ApiDumpInstance::current(), result, device, pSurfaceInfo, pModes);
    return result;
}
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle)
{
    dump_head_vkGetMemoryWin32HandleNV(ApiDumpInstance::current(), device, memory, handleType, pHandle);
    VkResult result = device_dispatch_table(device)->GetMemoryWin32HandleNV(device, memory, handleType, pHandle);
    
    dump_body_vkGetMemoryWin32HandleNV(ApiDumpInstance::current(), result, device, memory, handleType, pHandle);
    return result;
}
#endif // VK_USE_PLATFORM_WIN32_KHR
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkDebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo)
{
    dump_head_vkDebugMarkerSetObjectTagEXT(ApiDumpInstance::current(), device, pTagInfo);
    VkResult result = device_dispatch_table(device)->DebugMarkerSetObjectTagEXT(device, pTagInfo);
    
    dump_body_vkDebugMarkerSetObjectTagEXT(ApiDumpInstance::current(), result, device, pTagInfo);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateIndirectCommandsLayoutNVX(VkDevice device, const VkIndirectCommandsLayoutCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNVX* pIndirectCommandsLayout)
{
    dump_head_vkCreateIndirectCommandsLayoutNVX(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
    VkResult result = device_dispatch_table(device)->CreateIndirectCommandsLayoutNVX(device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
    
    dump_body_vkCreateIndirectCommandsLayoutNVX(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateObjectTableNVX(VkDevice device, const VkObjectTableCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkObjectTableNVX* pObjectTable)
{
    dump_head_vkCreateObjectTableNVX(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pObjectTable);
    VkResult result = device_dispatch_table(device)->CreateObjectTableNVX(device, pCreateInfo, pAllocator, pObjectTable);
    
    dump_body_vkCreateObjectTableNVX(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pObjectTable);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkRegisterObjectsNVX(VkDevice device, VkObjectTableNVX objectTable, uint32_t objectCount, const VkObjectTableEntryNVX* const*    ppObjectTableEntries, const uint32_t* pObjectIndices)
{
    dump_head_vkRegisterObjectsNVX(ApiDumpInstance::current(), device, objectTable, objectCount, ppObjectTableEntries, pObjectIndices);
    VkResult result = device_dispatch_table(device)->RegisterObjectsNVX(device, objectTable, objectCount, ppObjectTableEntries, pObjectIndices);
    
    dump_body_vkRegisterObjectsNVX(ApiDumpInstance::current(), result, device, objectTable, objectCount, ppObjectTableEntries, pObjectIndices);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkDebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo)
{
    dump_head_vkDebugMarkerSetObjectNameEXT(ApiDumpInstance::current(), device, pNameInfo);
    VkResult result = device_dispatch_table(device)->DebugMarkerSetObjectNameEXT(device, pNameInfo);
    
    dump_body_vkDebugMarkerSetObjectNameEXT(ApiDumpInstance::current(), result, device, pNameInfo);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkUnregisterObjectsNVX(VkDevice device, VkObjectTableNVX objectTable, uint32_t objectCount, const VkObjectEntryTypeNVX* pObjectEntryTypes, const uint32_t* pObjectIndices)
{
    dump_head_vkUnregisterObjectsNVX(ApiDumpInstance::current(), device, objectTable, objectCount, pObjectEntryTypes, pObjectIndices);
    VkResult result = device_dispatch_table(device)->UnregisterObjectsNVX(device, objectTable, objectCount, pObjectEntryTypes, pObjectIndices);
    
    dump_body_vkUnregisterObjectsNVX(ApiDumpInstance::current(), result, device, objectTable, objectCount, pObjectEntryTypes, pObjectIndices);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateAccelerationStructureNV(VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure)
{
    dump_head_vkCreateAccelerationStructureNV(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pAccelerationStructure);
    VkResult result = device_dispatch_table(device)->CreateAccelerationStructureNV(device, pCreateInfo, pAllocator, pAccelerationStructure);
    
    dump_body_vkCreateAccelerationStructureNV(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pAccelerationStructure);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineExecutablePropertiesKHR(VkDevice                        device, const VkPipelineInfoKHR*        pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties)
{
    dump_head_vkGetPipelineExecutablePropertiesKHR(ApiDumpInstance::current(), device, pPipelineInfo, pExecutableCount, pProperties);
    VkResult result = device_dispatch_table(device)->GetPipelineExecutablePropertiesKHR(device, pPipelineInfo, pExecutableCount, pProperties);
    
    dump_body_vkGetPipelineExecutablePropertiesKHR(ApiDumpInstance::current(), result, device, pPipelineInfo, pExecutableCount, pProperties);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkBindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
{
    dump_head_vkBindBufferMemory2(ApiDumpInstance::current(), device, bindInfoCount, pBindInfos);
    VkResult result = device_dispatch_table(device)->BindBufferMemory2(device, bindInfoCount, pBindInfos);
    
    dump_body_vkBindBufferMemory2(ApiDumpInstance::current(), result, device, bindInfoCount, pBindInfos);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo)
{
    dump_head_vkGetShaderInfoAMD(ApiDumpInstance::current(), device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
    VkResult result = device_dispatch_table(device)->GetShaderInfoAMD(device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
    
    dump_body_vkGetShaderInfoAMD(ApiDumpInstance::current(), result, device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader)
{
    dump_head_vkCompileDeferredNV(ApiDumpInstance::current(), device, pipeline, shader);
    VkResult result = device_dispatch_table(device)->CompileDeferredNV(device, pipeline, shader);
    
    dump_body_vkCompileDeferredNV(ApiDumpInstance::current(), result, device, pipeline, shader);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkRegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    dump_head_vkRegisterDeviceEventEXT(ApiDumpInstance::current(), device, pDeviceEventInfo, pAllocator, pFence);
    VkResult result = device_dispatch_table(device)->RegisterDeviceEventEXT(device, pDeviceEventInfo, pAllocator, pFence);
    
    dump_body_vkRegisterDeviceEventEXT(ApiDumpInstance::current(), result, device, pDeviceEventInfo, pAllocator, pFence);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties)
{
    dump_head_vkGetImageDrmFormatModifierPropertiesEXT(ApiDumpInstance::current(), device, image, pProperties);
    VkResult result = device_dispatch_table(device)->GetImageDrmFormatModifierPropertiesEXT(device, image, pProperties);
    
    dump_body_vkGetImageDrmFormatModifierPropertiesEXT(ApiDumpInstance::current(), result, device, image, pProperties);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkDisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo)
{
    dump_head_vkDisplayPowerControlEXT(ApiDumpInstance::current(), device, display, pDisplayPowerInfo);
    VkResult result = device_dispatch_table(device)->DisplayPowerControlEXT(device, display, pDisplayPowerInfo);
    
    dump_body_vkDisplayPowerControlEXT(ApiDumpInstance::current(), result, device, display, pDisplayPowerInfo);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
{
    dump_head_vkCreateDescriptorSetLayout(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pSetLayout);
    VkResult result = device_dispatch_table(device)->CreateDescriptorSetLayout(device, pCreateInfo, pAllocator, pSetLayout);
    
    dump_body_vkCreateDescriptorSetLayout(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pSetLayout);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplateKHR(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
    dump_head_vkCreateDescriptorUpdateTemplateKHR(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
    VkResult result = device_dispatch_table(device)->CreateDescriptorUpdateTemplateKHR(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
    
    dump_body_vkCreateDescriptorUpdateTemplateKHR(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkRegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    dump_head_vkRegisterDisplayEventEXT(ApiDumpInstance::current(), device, display, pDisplayEventInfo, pAllocator, pFence);
    VkResult result = device_dispatch_table(device)->RegisterDisplayEventEXT(device, display, pDisplayEventInfo, pAllocator, pFence);
    
    dump_body_vkRegisterDisplayEventEXT(ApiDumpInstance::current(), result, device, display, pDisplayEventInfo, pAllocator, pFence);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue)
{
    dump_head_vkGetSwapchainCounterEXT(ApiDumpInstance::current(), device, swapchain, counter, pCounterValue);
    VkResult result = device_dispatch_table(device)->GetSwapchainCounterEXT(device, swapchain, counter, pCounterValue);
    
    dump_body_vkGetSwapchainCounterEXT(ApiDumpInstance::current(), result, device, swapchain, counter, pCounterValue);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateSamplerYcbcrConversionKHR(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
{
    dump_head_vkCreateSamplerYcbcrConversionKHR(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pYcbcrConversion);
    VkResult result = device_dispatch_table(device)->CreateSamplerYcbcrConversionKHR(device, pCreateInfo, pAllocator, pYcbcrConversion);
    
    dump_body_vkCreateSamplerYcbcrConversionKHR(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pYcbcrConversion);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings)
{
    dump_head_vkGetPastPresentationTimingGOOGLE(ApiDumpInstance::current(), device, swapchain, pPresentationTimingCount, pPresentationTimings);
    VkResult result = device_dispatch_table(device)->GetPastPresentationTimingGOOGLE(device, swapchain, pPresentationTimingCount, pPresentationTimings);
    
    dump_body_vkGetPastPresentationTimingGOOGLE(ApiDumpInstance::current(), result, device, swapchain, pPresentationTimingCount, pPresentationTimings);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
{
    dump_head_vkCreateSwapchainKHR(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pSwapchain);
    VkResult result = device_dispatch_table(device)->CreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
    
    dump_body_vkCreateSwapchainKHR(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pSwapchain);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkMergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches)
{
    dump_head_vkMergeValidationCachesEXT(ApiDumpInstance::current(), device, dstCache, srcCacheCount, pSrcCaches);
    VkResult result = device_dispatch_table(device)->MergeValidationCachesEXT(device, dstCache, srcCacheCount, pSrcCaches);
    
    dump_body_vkMergeValidationCachesEXT(ApiDumpInstance::current(), result, device, dstCache, srcCacheCount, pSrcCaches);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateValidationCacheEXT(VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache)
{
    dump_head_vkCreateValidationCacheEXT(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pValidationCache);
    VkResult result = device_dispatch_table(device)->CreateValidationCacheEXT(device, pCreateInfo, pAllocator, pValidationCache);
    
    dump_body_vkCreateValidationCacheEXT(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pValidationCache);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkBindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos)
{
    dump_head_vkBindBufferMemory2KHR(ApiDumpInstance::current(), device, bindInfoCount, pBindInfos);
    VkResult result = device_dispatch_table(device)->BindBufferMemory2KHR(device, bindInfoCount, pBindInfos);
    
    dump_body_vkBindBufferMemory2KHR(ApiDumpInstance::current(), result, device, bindInfoCount, pBindInfos);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData)
{
    dump_head_vkGetValidationCacheDataEXT(ApiDumpInstance::current(), device, validationCache, pDataSize, pData);
    VkResult result = device_dispatch_table(device)->GetValidationCacheDataEXT(device, validationCache, pDataSize, pData);
    
    dump_body_vkGetValidationCacheDataEXT(ApiDumpInstance::current(), result, device, validationCache, pDataSize, pData);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkBindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
{
    dump_head_vkBindImageMemory2KHR(ApiDumpInstance::current(), device, bindInfoCount, pBindInfos);
    VkResult result = device_dispatch_table(device)->BindImageMemory2KHR(device, bindInfoCount, pBindInfos);
    
    dump_body_vkBindImageMemory2KHR(ApiDumpInstance::current(), result, device, bindInfoCount, pBindInfos);
    return result;
}
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties)
{
    dump_head_vkGetAndroidHardwareBufferPropertiesANDROID(ApiDumpInstance::current(), device, buffer, pProperties);
    VkResult result = device_dispatch_table(device)->GetAndroidHardwareBufferPropertiesANDROID(device, buffer, pProperties);
    
    dump_body_vkGetAndroidHardwareBufferPropertiesANDROID(ApiDumpInstance::current(), result, device, buffer, pProperties);
    return result;
}
#endif // VK_USE_PLATFORM_ANDROID_KHR
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties)
{
    dump_head_vkGetRefreshCycleDurationGOOGLE(ApiDumpInstance::current(), device, swapchain, pDisplayTimingProperties);
    VkResult result = device_dispatch_table(device)->GetRefreshCycleDurationGOOGLE(device, swapchain, pDisplayTimingProperties);
    
    dump_body_vkGetRefreshCycleDurationGOOGLE(ApiDumpInstance::current(), result, device, swapchain, pDisplayTimingProperties);
    return result;
}
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer)
{
    dump_head_vkGetMemoryAndroidHardwareBufferANDROID(ApiDumpInstance::current(), device, pInfo, pBuffer);
    VkResult result = device_dispatch_table(device)->GetMemoryAndroidHardwareBufferANDROID(device, pInfo, pBuffer);
    
    dump_body_vkGetMemoryAndroidHardwareBufferANDROID(ApiDumpInstance::current(), result, device, pInfo, pBuffer);
    return result;
}
#endif // VK_USE_PLATFORM_ANDROID_KHR
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool)
{
    dump_head_vkCreateDescriptorPool(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pDescriptorPool);
    VkResult result = device_dispatch_table(device)->CreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
    
    dump_body_vkCreateDescriptorPool(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pDescriptorPool);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkFreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets)
{
    dump_head_vkFreeDescriptorSets(ApiDumpInstance::current(), device, descriptorPool, descriptorSetCount, pDescriptorSets);
    VkResult result = device_dispatch_table(device)->FreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
    
    dump_body_vkFreeDescriptorSets(ApiDumpInstance::current(), result, device, descriptorPool, descriptorSetCount, pDescriptorSets);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags)
{
    dump_head_vkResetDescriptorPool(ApiDumpInstance::current(), device, descriptorPool, flags);
    VkResult result = device_dispatch_table(device)->ResetDescriptorPool(device, descriptorPool, flags);
    
    dump_body_vkResetDescriptorPool(ApiDumpInstance::current(), result, device, descriptorPool, flags);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkAllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
{
    dump_head_vkAllocateDescriptorSets(ApiDumpInstance::current(), device, pAllocateInfo, pDescriptorSets);
    VkResult result = device_dispatch_table(device)->AllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
    
    dump_body_vkAllocateDescriptorSets(ApiDumpInstance::current(), result, device, pAllocateInfo, pDescriptorSets);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkSetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo)
{
    dump_head_vkSetDebugUtilsObjectNameEXT(ApiDumpInstance::current(), device, pNameInfo);
    VkResult result = device_dispatch_table(device)->SetDebugUtilsObjectNameEXT(device, pNameInfo);
    
    dump_body_vkSetDebugUtilsObjectNameEXT(ApiDumpInstance::current(), result, device, pNameInfo);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkSetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo)
{
    dump_head_vkSetDebugUtilsObjectTagEXT(ApiDumpInstance::current(), device, pTagInfo);
    VkResult result = device_dispatch_table(device)->SetDebugUtilsObjectTagEXT(device, pTagInfo);
    
    dump_body_vkSetDebugUtilsObjectTagEXT(ApiDumpInstance::current(), result, device, pTagInfo);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
{
    dump_head_vkQueueSubmit(ApiDumpInstance::current(), queue, submitCount, pSubmits, fence);
    VkResult result = device_dispatch_table(queue)->QueueSubmit(queue, submitCount, pSubmits, fence);
    
    dump_body_vkQueueSubmit(ApiDumpInstance::current(), result, queue, submitCount, pSubmits, fence);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
    dump_head_vkCreateCommandPool(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pCommandPool);
    VkResult result = device_dispatch_table(device)->CreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
    
    dump_body_vkCreateCommandPool(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pCommandPool);
    return result;
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkImportSemaphoreWin32HandleKHR(VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo)
{
    dump_head_vkImportSemaphoreWin32HandleKHR(ApiDumpInstance::current(), device, pImportSemaphoreWin32HandleInfo);
    VkResult result = device_dispatch_table(device)->ImportSemaphoreWin32HandleKHR(device, pImportSemaphoreWin32HandleInfo);
    
    dump_body_vkImportSemaphoreWin32HandleKHR(ApiDumpInstance::current(), result, device, pImportSemaphoreWin32HandleInfo);
    return result;
}
#endif // VK_USE_PLATFORM_WIN32_KHR
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence)
{
    dump_head_vkQueueBindSparse(ApiDumpInstance::current(), queue, bindInfoCount, pBindInfo, fence);
    VkResult result = device_dispatch_table(queue)->QueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
    
    dump_body_vkQueueBindSparse(ApiDumpInstance::current(), result, queue, bindInfoCount, pBindInfo, fence);
    return result;
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetSemaphoreWin32HandleKHR(VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle)
{
    dump_head_vkGetSemaphoreWin32HandleKHR(ApiDumpInstance::current(), device, pGetWin32HandleInfo, pHandle);
    VkResult result = device_dispatch_table(device)->GetSemaphoreWin32HandleKHR(device, pGetWin32HandleInfo, pHandle);
    
    dump_body_vkGetSemaphoreWin32HandleKHR(ApiDumpInstance::current(), result, device, pGetWin32HandleInfo, pHandle);
    return result;
}
#endif // VK_USE_PLATFORM_WIN32_KHR
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities)
{
    dump_head_vkGetDeviceGroupPresentCapabilitiesKHR(ApiDumpInstance::current(), device, pDeviceGroupPresentCapabilities);
    VkResult result = device_dispatch_table(device)->GetDeviceGroupPresentCapabilitiesKHR(device, pDeviceGroupPresentCapabilities);
    
    dump_body_vkGetDeviceGroupPresentCapabilitiesKHR(ApiDumpInstance::current(), result, device, pDeviceGroupPresentCapabilities);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo)
{
    dump_head_vkImportSemaphoreFdKHR(ApiDumpInstance::current(), device, pImportSemaphoreFdInfo);
    VkResult result = device_dispatch_table(device)->ImportSemaphoreFdKHR(device, pImportSemaphoreFdInfo);
    
    dump_body_vkImportSemaphoreFdKHR(ApiDumpInstance::current(), result, device, pImportSemaphoreFdInfo);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion)
{
    dump_head_vkCreateSamplerYcbcrConversion(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pYcbcrConversion);
    VkResult result = device_dispatch_table(device)->CreateSamplerYcbcrConversion(device, pCreateInfo, pAllocator, pYcbcrConversion);
    
    dump_body_vkCreateSamplerYcbcrConversion(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pYcbcrConversion);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags)
{
    dump_head_vkResetCommandPool(ApiDumpInstance::current(), device, commandPool, flags);
    VkResult result = device_dispatch_table(device)->ResetCommandPool(device, commandPool, flags);
    
    dump_body_vkResetCommandPool(ApiDumpInstance::current(), result, device, commandPool, flags);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes)
{
    dump_head_vkGetDeviceGroupSurfacePresentModesKHR(ApiDumpInstance::current(), device, surface, pModes);
    VkResult result = device_dispatch_table(device)->GetDeviceGroupSurfacePresentModesKHR(device, surface, pModes);
    
    dump_body_vkGetDeviceGroupSurfacePresentModesKHR(ApiDumpInstance::current(), result, device, surface, pModes);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkAllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
    dump_head_vkAllocateCommandBuffers(ApiDumpInstance::current(), device, pAllocateInfo, pCommandBuffers);
    VkResult result = device_dispatch_table(device)->AllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
    if(result == VK_SUCCESS)
ApiDumpInstance::current().addCmdBuffers(
device,
pAllocateInfo->commandPool,
std::vector<VkCommandBuffer>(pCommandBuffers, pCommandBuffers + pAllocateInfo->commandBufferCount),
pAllocateInfo->level
);
    dump_body_vkAllocateCommandBuffers(ApiDumpInstance::current(), result, device, pAllocateInfo, pCommandBuffers);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkAcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex)
{
    dump_head_vkAcquireNextImage2KHR(ApiDumpInstance::current(), device, pAcquireInfo, pImageIndex);
    VkResult result = device_dispatch_table(device)->AcquireNextImage2KHR(device, pAcquireInfo, pImageIndex);
    
    dump_body_vkAcquireNextImage2KHR(ApiDumpInstance::current(), result, device, pAcquireInfo, pImageIndex);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkQueueWaitIdle(VkQueue queue)
{
    dump_head_vkQueueWaitIdle(ApiDumpInstance::current(), queue);
    VkResult result = device_dispatch_table(queue)->QueueWaitIdle(queue);
    
    dump_body_vkQueueWaitIdle(ApiDumpInstance::current(), result, queue);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    dump_head_vkGetSemaphoreFdKHR(ApiDumpInstance::current(), device, pGetFdInfo, pFd);
    VkResult result = device_dispatch_table(device)->GetSemaphoreFdKHR(device, pGetFdInfo, pFd);
    
    dump_body_vkGetSemaphoreFdKHR(ApiDumpInstance::current(), result, device, pGetFdInfo, pFd);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkBeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo)
{
    dump_head_vkBeginCommandBuffer(ApiDumpInstance::current(), commandBuffer, pBeginInfo);
    VkResult result = device_dispatch_table(commandBuffer)->BeginCommandBuffer(commandBuffer, pBeginInfo);
    
    dump_body_vkBeginCommandBuffer(ApiDumpInstance::current(), result, commandBuffer, pBeginInfo);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkDeviceWaitIdle(VkDevice device)
{
    dump_head_vkDeviceWaitIdle(ApiDumpInstance::current(), device);
    VkResult result = device_dispatch_table(device)->DeviceWaitIdle(device);
    
    dump_body_vkDeviceWaitIdle(ApiDumpInstance::current(), result, device);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate)
{
    dump_head_vkCreateDescriptorUpdateTemplate(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
    VkResult result = device_dispatch_table(device)->CreateDescriptorUpdateTemplate(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
    
    dump_body_vkCreateDescriptorUpdateTemplate(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain)
{
    dump_head_vkGetSwapchainStatusKHR(ApiDumpInstance::current(), device, swapchain);
    VkResult result = device_dispatch_table(device)->GetSwapchainStatusKHR(device, swapchain);
    
    dump_body_vkGetSwapchainStatusKHR(ApiDumpInstance::current(), result, device, swapchain);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkAllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory)
{
    dump_head_vkAllocateMemory(ApiDumpInstance::current(), device, pAllocateInfo, pAllocator, pMemory);
    VkResult result = device_dispatch_table(device)->AllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
    
    dump_body_vkAllocateMemory(ApiDumpInstance::current(), result, device, pAllocateInfo, pAllocator, pMemory);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkEndCommandBuffer(VkCommandBuffer commandBuffer)
{
    dump_head_vkEndCommandBuffer(ApiDumpInstance::current(), commandBuffer);
    VkResult result = device_dispatch_table(commandBuffer)->EndCommandBuffer(commandBuffer);
    
    dump_body_vkEndCommandBuffer(ApiDumpInstance::current(), result, commandBuffer);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    dump_head_vkCreateFence(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pFence);
    VkResult result = device_dispatch_table(device)->CreateFence(device, pCreateInfo, pAllocator, pFence);
    
    dump_body_vkCreateFence(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pFence);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer)
{
    dump_head_vkCreateFramebuffer(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pFramebuffer);
    VkResult result = device_dispatch_table(device)->CreateFramebuffer(device, pCreateInfo, pAllocator, pFramebuffer);
    
    dump_body_vkCreateFramebuffer(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pFramebuffer);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkBindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos)
{
    dump_head_vkBindImageMemory2(ApiDumpInstance::current(), device, bindInfoCount, pBindInfos);
    VkResult result = device_dispatch_table(device)->BindImageMemory2(device, bindInfoCount, pBindInfos);
    
    dump_body_vkBindImageMemory2(ApiDumpInstance::current(), result, device, bindInfoCount, pBindInfos);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineExecutableStatisticsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics)
{
    dump_head_vkGetPipelineExecutableStatisticsKHR(ApiDumpInstance::current(), device, pExecutableInfo, pStatisticCount, pStatistics);
    VkResult result = device_dispatch_table(device)->GetPipelineExecutableStatisticsKHR(device, pExecutableInfo, pStatisticCount, pStatistics);
    
    dump_body_vkGetPipelineExecutableStatisticsKHR(ApiDumpInstance::current(), result, device, pExecutableInfo, pStatisticCount, pStatistics);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineExecutableInternalRepresentationsKHR(VkDevice                        device, const VkPipelineExecutableInfoKHR*  pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations)
{
    dump_head_vkGetPipelineExecutableInternalRepresentationsKHR(ApiDumpInstance::current(), device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
    VkResult result = device_dispatch_table(device)->GetPipelineExecutableInternalRepresentationsKHR(device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
    
    dump_body_vkGetPipelineExecutableInternalRepresentationsKHR(ApiDumpInstance::current(), result, device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
    return result;
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkImportFenceWin32HandleKHR(VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo)
{
    dump_head_vkImportFenceWin32HandleKHR(ApiDumpInstance::current(), device, pImportFenceWin32HandleInfo);
    VkResult result = device_dispatch_table(device)->ImportFenceWin32HandleKHR(device, pImportFenceWin32HandleInfo);
    
    dump_body_vkImportFenceWin32HandleKHR(ApiDumpInstance::current(), result, device, pImportFenceWin32HandleInfo);
    return result;
}
#endif // VK_USE_PLATFORM_WIN32_KHR
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2KHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    dump_head_vkCreateRenderPass2KHR(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pRenderPass);
    VkResult result = device_dispatch_table(device)->CreateRenderPass2KHR(device, pCreateInfo, pAllocator, pRenderPass);
    
    dump_body_vkCreateRenderPass2KHR(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pRenderPass);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetFenceStatus(VkDevice device, VkFence fence)
{
    dump_head_vkGetFenceStatus(ApiDumpInstance::current(), device, fence);
    VkResult result = device_dispatch_table(device)->GetFenceStatus(device, fence);
    
    dump_body_vkGetFenceStatus(ApiDumpInstance::current(), result, device, fence);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
    dump_head_vkCreateRenderPass(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pRenderPass);
    VkResult result = device_dispatch_table(device)->CreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
    
    dump_body_vkCreateRenderPass(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pRenderPass);
    return result;
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetFenceWin32HandleKHR(VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle)
{
    dump_head_vkGetFenceWin32HandleKHR(ApiDumpInstance::current(), device, pGetWin32HandleInfo, pHandle);
    VkResult result = device_dispatch_table(device)->GetFenceWin32HandleKHR(device, pGetWin32HandleInfo, pHandle);
    
    dump_body_vkGetFenceWin32HandleKHR(ApiDumpInstance::current(), result, device, pGetWin32HandleInfo, pHandle);
    return result;
}
#endif // VK_USE_PLATFORM_WIN32_KHR
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo)
{
    dump_head_vkImportFenceFdKHR(ApiDumpInstance::current(), device, pImportFenceFdInfo);
    VkResult result = device_dispatch_table(device)->ImportFenceFdKHR(device, pImportFenceFdInfo);
    
    dump_body_vkImportFenceFdKHR(ApiDumpInstance::current(), result, device, pImportFenceFdInfo);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
{
    dump_head_vkCreateShaderModule(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pShaderModule);
    VkResult result = device_dispatch_table(device)->CreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
    
    dump_body_vkCreateShaderModule(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pShaderModule);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences)
{
    dump_head_vkResetFences(ApiDumpInstance::current(), device, fenceCount, pFences);
    VkResult result = device_dispatch_table(device)->ResetFences(device, fenceCount, pFences);
    
    dump_body_vkResetFences(ApiDumpInstance::current(), result, device, fenceCount, pFences);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkWaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
{
    dump_head_vkWaitForFences(ApiDumpInstance::current(), device, fenceCount, pFences, waitAll, timeout);
    VkResult result = device_dispatch_table(device)->WaitForFences(device, fenceCount, pFences, waitAll, timeout);
    
    dump_body_vkWaitForFences(ApiDumpInstance::current(), result, device, fenceCount, pFences, waitAll, timeout);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore)
{
    dump_head_vkCreateSemaphore(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pSemaphore);
    VkResult result = device_dispatch_table(device)->CreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
    
    dump_body_vkCreateSemaphore(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pSemaphore);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    dump_head_vkGetFenceFdKHR(ApiDumpInstance::current(), device, pGetFdInfo, pFd);
    VkResult result = device_dispatch_table(device)->GetFenceFdKHR(device, pGetFdInfo, pFd);
    
    dump_body_vkGetFenceFdKHR(ApiDumpInstance::current(), result, device, pGetFdInfo, pFd);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags)
{
    dump_head_vkGetQueryPoolResults(ApiDumpInstance::current(), device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
    VkResult result = device_dispatch_table(device)->GetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
    
    dump_body_vkGetQueryPoolResults(ApiDumpInstance::current(), result, device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
    return result;
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties)
{
    dump_head_vkGetMemoryWin32HandlePropertiesKHR(ApiDumpInstance::current(), device, handleType, handle, pMemoryWin32HandleProperties);
    VkResult result = device_dispatch_table(device)->GetMemoryWin32HandlePropertiesKHR(device, handleType, handle, pMemoryWin32HandleProperties);
    
    dump_body_vkGetMemoryWin32HandlePropertiesKHR(ApiDumpInstance::current(), result, device, handleType, handle, pMemoryWin32HandleProperties);
    return result;
}
#endif // VK_USE_PLATFORM_WIN32_KHR
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkMapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
    dump_head_vkMapMemory(ApiDumpInstance::current(), device, memory, offset, size, flags, ppData);
    VkResult result = device_dispatch_table(device)->MapMemory(device, memory, offset, size, flags, ppData);
    
    dump_body_vkMapMemory(ApiDumpInstance::current(), result, device, memory, offset, size, flags, ppData);
    return result;
}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryWin32HandleKHR(VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle)
{
    dump_head_vkGetMemoryWin32HandleKHR(ApiDumpInstance::current(), device, pGetWin32HandleInfo, pHandle);
    VkResult result = device_dispatch_table(device)->GetMemoryWin32HandleKHR(device, pGetWin32HandleInfo, pHandle);
    
    dump_body_vkGetMemoryWin32HandleKHR(ApiDumpInstance::current(), result, device, pGetWin32HandleInfo, pHandle);
    return result;
}
#endif // VK_USE_PLATFORM_WIN32_KHR
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
    dump_head_vkCreateBuffer(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pBuffer);
    VkResult result = device_dispatch_table(device)->CreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
    
    dump_body_vkCreateBuffer(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pBuffer);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    dump_head_vkBindBufferMemory(ApiDumpInstance::current(), device, buffer, memory, memoryOffset);
    VkResult result = device_dispatch_table(device)->BindBufferMemory(device, buffer, memory, memoryOffset);
    
    dump_body_vkBindBufferMemory(ApiDumpInstance::current(), result, device, buffer, memory, memoryOffset);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkFlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    dump_head_vkFlushMappedMemoryRanges(ApiDumpInstance::current(), device, memoryRangeCount, pMemoryRanges);
    VkResult result = device_dispatch_table(device)->FlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
    
    dump_body_vkFlushMappedMemoryRanges(ApiDumpInstance::current(), result, device, memoryRangeCount, pMemoryRanges);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties)
{
    dump_head_vkGetMemoryFdPropertiesKHR(ApiDumpInstance::current(), device, handleType, fd, pMemoryFdProperties);
    VkResult result = device_dispatch_table(device)->GetMemoryFdPropertiesKHR(device, handleType, fd, pMemoryFdProperties);
    
    dump_body_vkGetMemoryFdPropertiesKHR(ApiDumpInstance::current(), result, device, handleType, fd, pMemoryFdProperties);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkInvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    dump_head_vkInvalidateMappedMemoryRanges(ApiDumpInstance::current(), device, memoryRangeCount, pMemoryRanges);
    VkResult result = device_dispatch_table(device)->InvalidateMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
    
    dump_body_vkInvalidateMappedMemoryRanges(ApiDumpInstance::current(), result, device, memoryRangeCount, pMemoryRanges);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd)
{
    dump_head_vkGetMemoryFdKHR(ApiDumpInstance::current(), device, pGetFdInfo, pFd);
    VkResult result = device_dispatch_table(device)->GetMemoryFdKHR(device, pGetFdInfo, pFd);
    
    dump_body_vkGetMemoryFdKHR(ApiDumpInstance::current(), result, device, pGetFdInfo, pFd);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    dump_head_vkBindImageMemory(ApiDumpInstance::current(), device, image, memory, memoryOffset);
    VkResult result = device_dispatch_table(device)->BindImageMemory(device, image, memory, memoryOffset);
    
    dump_body_vkBindImageMemory(ApiDumpInstance::current(), result, device, image, memory, memoryOffset);
    return result;
}
VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkCreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView)
{
    dump_head_vkCreateBufferView(ApiDumpInstance::current(), device, pCreateInfo, pAllocator, pView);
    VkResult result = device_dispatch_table(device)->CreateBufferView(device, pCreateInfo, pAllocator, pView);
    
    dump_body_vkCreateBufferView(ApiDumpInstance::current(), result, device, pCreateInfo, pAllocator, pView);
    return result;
}


VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes)
{
    dump_head_vkCmdBindTransformFeedbackBuffersEXT(ApiDumpInstance::current(), commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
    device_dispatch_table(commandBuffer)->CmdBindTransformFeedbackBuffersEXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
    
    dump_body_vkCmdBindTransformFeedbackBuffersEXT(ApiDumpInstance::current(), commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
{
    dump_head_vkCmdBeginTransformFeedbackEXT(ApiDumpInstance::current(), commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
    device_dispatch_table(commandBuffer)->CmdBeginTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
    
    dump_body_vkCmdBeginTransformFeedbackEXT(ApiDumpInstance::current(), commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets)
{
    dump_head_vkCmdEndTransformFeedbackEXT(ApiDumpInstance::current(), commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
    device_dispatch_table(commandBuffer)->CmdEndTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
    
    dump_body_vkCmdEndTransformFeedbackEXT(ApiDumpInstance::current(), commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyBufferView(ApiDumpInstance::current(), device, bufferView, pAllocator);
    device_dispatch_table(device)->DestroyBufferView(device, bufferView, pAllocator);
    
    dump_body_vkDestroyBufferView(ApiDumpInstance::current(), device, bufferView, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyEvent(ApiDumpInstance::current(), device, event, pAllocator);
    device_dispatch_table(device)->DestroyEvent(device, event, pAllocator);
    
    dump_body_vkDestroyEvent(ApiDumpInstance::current(), device, event, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyPipelineCache(ApiDumpInstance::current(), device, pipelineCache, pAllocator);
    device_dispatch_table(device)->DestroyPipelineCache(device, pipelineCache, pAllocator);
    
    dump_body_vkDestroyPipelineCache(ApiDumpInstance::current(), device, pipelineCache, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index)
{
    dump_head_vkCmdBeginQueryIndexedEXT(ApiDumpInstance::current(), commandBuffer, queryPool, query, flags, index);
    device_dispatch_table(commandBuffer)->CmdBeginQueryIndexedEXT(commandBuffer, queryPool, query, flags, index);
    
    dump_body_vkCmdBeginQueryIndexedEXT(ApiDumpInstance::current(), commandBuffer, queryPool, query, flags, index);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout)
{
    dump_head_vkGetImageSubresourceLayout(ApiDumpInstance::current(), device, image, pSubresource, pLayout);
    device_dispatch_table(device)->GetImageSubresourceLayout(device, image, pSubresource, pLayout);
    
    dump_body_vkGetImageSubresourceLayout(ApiDumpInstance::current(), device, image, pSubresource, pLayout);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index)
{
    dump_head_vkCmdEndQueryIndexedEXT(ApiDumpInstance::current(), commandBuffer, queryPool, query, index);
    device_dispatch_table(commandBuffer)->CmdEndQueryIndexedEXT(commandBuffer, queryPool, query, index);
    
    dump_body_vkCmdEndQueryIndexedEXT(ApiDumpInstance::current(), commandBuffer, queryPool, query, index);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride)
{
    dump_head_vkCmdDrawIndirectByteCountEXT(ApiDumpInstance::current(), commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
    device_dispatch_table(commandBuffer)->CmdDrawIndirectByteCountEXT(commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
    
    dump_body_vkCmdDrawIndirectByteCountEXT(ApiDumpInstance::current(), commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker)
{
    dump_head_vkCmdWriteBufferMarkerAMD(ApiDumpInstance::current(), commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
    device_dispatch_table(commandBuffer)->CmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
    
    dump_body_vkCmdWriteBufferMarkerAMD(ApiDumpInstance::current(), commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroySwapchainKHR(ApiDumpInstance::current(), device, swapchain, pAllocator);
    device_dispatch_table(device)->DestroySwapchainKHR(device, swapchain, pAllocator);
    
    dump_body_vkDestroySwapchainKHR(ApiDumpInstance::current(), device, swapchain, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyImage(ApiDumpInstance::current(), device, image, pAllocator);
    device_dispatch_table(device)->DestroyImage(device, image, pAllocator);
    
    dump_body_vkDestroyImage(ApiDumpInstance::current(), device, image, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyQueryPool(ApiDumpInstance::current(), device, queryPool, pAllocator);
    device_dispatch_table(device)->DestroyQueryPool(device, queryPool, pAllocator);
    
    dump_body_vkDestroyQueryPool(ApiDumpInstance::current(), device, queryPool, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter)
{
    dump_head_vkCmdBlitImage(ApiDumpInstance::current(), commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
    device_dispatch_table(commandBuffer)->CmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
    
    dump_body_vkCmdBlitImage(ApiDumpInstance::current(), commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset)
{
    dump_head_vkCmdBuildAccelerationStructureNV(ApiDumpInstance::current(), commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
    device_dispatch_table(commandBuffer)->CmdBuildAccelerationStructureNV(commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
    
    dump_body_vkCmdBuildAccelerationStructureNV(ApiDumpInstance::current(), commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetAccelerationStructureMemoryRequirementsNV(VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements)
{
    dump_head_vkGetAccelerationStructureMemoryRequirementsNV(ApiDumpInstance::current(), device, pInfo, pMemoryRequirements);
    device_dispatch_table(device)->GetAccelerationStructureMemoryRequirementsNV(device, pInfo, pMemoryRequirements);
    
    dump_body_vkGetAccelerationStructureMemoryRequirementsNV(ApiDumpInstance::current(), device, pInfo, pMemoryRequirements);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyAccelerationStructureNV(ApiDumpInstance::current(), device, accelerationStructure, pAllocator);
    device_dispatch_table(device)->DestroyAccelerationStructureNV(device, accelerationStructure, pAllocator);
    
    dump_body_vkDestroyAccelerationStructureNV(ApiDumpInstance::current(), device, accelerationStructure, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
    dump_head_vkCmdCopyBufferToImage(ApiDumpInstance::current(), commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
    device_dispatch_table(commandBuffer)->CmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
    
    dump_body_vkCmdCopyBufferToImage(ApiDumpInstance::current(), commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports)
{
    dump_head_vkCmdSetViewport(ApiDumpInstance::current(), commandBuffer, firstViewport, viewportCount, pViewports);
    device_dispatch_table(commandBuffer)->CmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
    
    dump_body_vkCmdSetViewport(ApiDumpInstance::current(), commandBuffer, firstViewport, viewportCount, pViewports);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyPipeline(ApiDumpInstance::current(), device, pipeline, pAllocator);
    device_dispatch_table(device)->DestroyPipeline(device, pipeline, pAllocator);
    
    dump_body_vkDestroyPipeline(ApiDumpInstance::current(), device, pipeline, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeNV mode)
{
    dump_head_vkCmdCopyAccelerationStructureNV(ApiDumpInstance::current(), commandBuffer, dst, src, mode);
    device_dispatch_table(commandBuffer)->CmdCopyAccelerationStructureNV(commandBuffer, dst, src, mode);
    
    dump_body_vkCmdCopyAccelerationStructureNV(ApiDumpInstance::current(), commandBuffer, dst, src, mode);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
{
    dump_head_vkCmdBindPipeline(ApiDumpInstance::current(), commandBuffer, pipelineBindPoint, pipeline);
    device_dispatch_table(commandBuffer)->CmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
    
    dump_body_vkCmdBindPipeline(ApiDumpInstance::current(), commandBuffer, pipelineBindPoint, pipeline);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo)
{
    dump_head_vkCmdSetSampleLocationsEXT(ApiDumpInstance::current(), commandBuffer, pSampleLocationsInfo);
    device_dispatch_table(commandBuffer)->CmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
    
    dump_body_vkCmdSetSampleLocationsEXT(ApiDumpInstance::current(), commandBuffer, pSampleLocationsInfo);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetImageSparseMemoryRequirements2KHR(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    dump_head_vkGetImageSparseMemoryRequirements2KHR(ApiDumpInstance::current(), device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
    device_dispatch_table(device)->GetImageSparseMemoryRequirements2KHR(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
    
    dump_body_vkGetImageSparseMemoryRequirements2KHR(ApiDumpInstance::current(), device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth)
{
    dump_head_vkCmdSetLineWidth(ApiDumpInstance::current(), commandBuffer, lineWidth);
    device_dispatch_table(commandBuffer)->CmdSetLineWidth(commandBuffer, lineWidth);
    
    dump_body_vkCmdSetLineWidth(ApiDumpInstance::current(), commandBuffer, lineWidth);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors)
{
    dump_head_vkCmdSetScissor(ApiDumpInstance::current(), commandBuffer, firstScissor, scissorCount, pScissors);
    device_dispatch_table(commandBuffer)->CmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
    
    dump_body_vkCmdSetScissor(ApiDumpInstance::current(), commandBuffer, firstScissor, scissorCount, pScissors);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkUninitializePerformanceApiINTEL(VkDevice device)
{
    dump_head_vkUninitializePerformanceApiINTEL(ApiDumpInstance::current(), device);
    device_dispatch_table(device)->UninitializePerformanceApiINTEL(device);
    
    dump_body_vkUninitializePerformanceApiINTEL(ApiDumpInstance::current(), device);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
    dump_head_vkCmdSetDepthBias(ApiDumpInstance::current(), commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
    device_dispatch_table(commandBuffer)->CmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
    
    dump_body_vkCmdSetDepthBias(ApiDumpInstance::current(), commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth)
{
    dump_head_vkCmdTraceRaysNV(ApiDumpInstance::current(), commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
    device_dispatch_table(commandBuffer)->CmdTraceRaysNV(commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
    
    dump_body_vkCmdTraceRaysNV(ApiDumpInstance::current(), commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    dump_head_vkCmdDrawIndirectCountAMD(ApiDumpInstance::current(), commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
    device_dispatch_table(commandBuffer)->CmdDrawIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
    
    dump_body_vkCmdDrawIndirectCountAMD(ApiDumpInstance::current(), commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    dump_head_vkCmdDrawIndexedIndirectCountAMD(ApiDumpInstance::current(), commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
    device_dispatch_table(commandBuffer)->CmdDrawIndexedIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
    
    dump_body_vkCmdDrawIndexedIndirectCountAMD(ApiDumpInstance::current(), commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetImageMemoryRequirements2KHR(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    dump_head_vkGetImageMemoryRequirements2KHR(ApiDumpInstance::current(), device, pInfo, pMemoryRequirements);
    device_dispatch_table(device)->GetImageMemoryRequirements2KHR(device, pInfo, pMemoryRequirements);
    
    dump_body_vkGetImageMemoryRequirements2KHR(ApiDumpInstance::current(), device, pInfo, pMemoryRequirements);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetBufferMemoryRequirements2KHR(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    dump_head_vkGetBufferMemoryRequirements2KHR(ApiDumpInstance::current(), device, pInfo, pMemoryRequirements);
    device_dispatch_table(device)->GetBufferMemoryRequirements2KHR(device, pInfo, pMemoryRequirements);
    
    dump_body_vkGetBufferMemoryRequirements2KHR(ApiDumpInstance::current(), device, pInfo, pMemoryRequirements);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
    dump_head_vkCmdCopyImageToBuffer(ApiDumpInstance::current(), commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
    device_dispatch_table(commandBuffer)->CmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
    
    dump_body_vkCmdCopyImageToBuffer(ApiDumpInstance::current(), commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4])
{
    dump_head_vkCmdSetBlendConstants(ApiDumpInstance::current(), commandBuffer, blendConstants);
    device_dispatch_table(commandBuffer)->CmdSetBlendConstants(commandBuffer, blendConstants);
    
    dump_body_vkCmdSetBlendConstants(ApiDumpInstance::current(), commandBuffer, blendConstants);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask)
{
    dump_head_vkCmdSetStencilWriteMask(ApiDumpInstance::current(), commandBuffer, faceMask, writeMask);
    device_dispatch_table(commandBuffer)->CmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
    
    dump_body_vkCmdSetStencilWriteMask(ApiDumpInstance::current(), commandBuffer, faceMask, writeMask);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyPipelineLayout(ApiDumpInstance::current(), device, pipelineLayout, pAllocator);
    device_dispatch_table(device)->DestroyPipelineLayout(device, pipelineLayout, pAllocator);
    
    dump_body_vkDestroyPipelineLayout(ApiDumpInstance::current(), device, pipelineLayout, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
    dump_head_vkCmdSetDepthBounds(ApiDumpInstance::current(), commandBuffer, minDepthBounds, maxDepthBounds);
    device_dispatch_table(commandBuffer)->CmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
    
    dump_body_vkCmdSetDepthBounds(ApiDumpInstance::current(), commandBuffer, minDepthBounds, maxDepthBounds);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyRenderPass(ApiDumpInstance::current(), device, renderPass, pAllocator);
    device_dispatch_table(device)->DestroyRenderPass(device, renderPass, pAllocator);
    
    dump_body_vkDestroyRenderPass(ApiDumpInstance::current(), device, renderPass, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData)
{
    dump_head_vkCmdUpdateBuffer(ApiDumpInstance::current(), commandBuffer, dstBuffer, dstOffset, dataSize, pData);
    device_dispatch_table(commandBuffer)->CmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
    
    dump_body_vkCmdUpdateBuffer(ApiDumpInstance::current(), commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask)
{
    dump_head_vkCmdSetStencilCompareMask(ApiDumpInstance::current(), commandBuffer, faceMask, compareMask);
    device_dispatch_table(commandBuffer)->CmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
    
    dump_body_vkCmdSetStencilCompareMask(ApiDumpInstance::current(), commandBuffer, faceMask, compareMask);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data)
{
    dump_head_vkCmdFillBuffer(ApiDumpInstance::current(), commandBuffer, dstBuffer, dstOffset, size, data);
    device_dispatch_table(commandBuffer)->CmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
    
    dump_body_vkCmdFillBuffer(ApiDumpInstance::current(), commandBuffer, dstBuffer, dstOffset, size, data);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdProcessCommandsNVX(VkCommandBuffer commandBuffer, const VkCmdProcessCommandsInfoNVX* pProcessCommandsInfo)
{
    dump_head_vkCmdProcessCommandsNVX(ApiDumpInstance::current(), commandBuffer, pProcessCommandsInfo);
    device_dispatch_table(commandBuffer)->CmdProcessCommandsNVX(commandBuffer, pProcessCommandsInfo);
    
    dump_body_vkCmdProcessCommandsNVX(ApiDumpInstance::current(), commandBuffer, pProcessCommandsInfo);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkTrimCommandPoolKHR(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
    dump_head_vkTrimCommandPoolKHR(ApiDumpInstance::current(), device, commandPool, flags);
    device_dispatch_table(device)->TrimCommandPoolKHR(device, commandPool, flags);
    
    dump_body_vkTrimCommandPoolKHR(ApiDumpInstance::current(), device, commandPool, flags);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdReserveSpaceForCommandsNVX(VkCommandBuffer commandBuffer, const VkCmdReserveSpaceForCommandsInfoNVX* pReserveSpaceInfo)
{
    dump_head_vkCmdReserveSpaceForCommandsNVX(ApiDumpInstance::current(), commandBuffer, pReserveSpaceInfo);
    device_dispatch_table(commandBuffer)->CmdReserveSpaceForCommandsNVX(commandBuffer, pReserveSpaceInfo);
    
    dump_body_vkCmdReserveSpaceForCommandsNVX(ApiDumpInstance::current(), commandBuffer, pReserveSpaceInfo);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyIndirectCommandsLayoutNVX(VkDevice device, VkIndirectCommandsLayoutNVX indirectCommandsLayout, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyIndirectCommandsLayoutNVX(ApiDumpInstance::current(), device, indirectCommandsLayout, pAllocator);
    device_dispatch_table(device)->DestroyIndirectCommandsLayoutNVX(device, indirectCommandsLayout, pAllocator);
    
    dump_body_vkDestroyIndirectCommandsLayoutNVX(ApiDumpInstance::current(), device, indirectCommandsLayout, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyObjectTableNVX(VkDevice device, VkObjectTableNVX objectTable, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyObjectTableNVX(ApiDumpInstance::current(), device, objectTable, pAllocator);
    device_dispatch_table(device)->DestroyObjectTableNVX(device, objectTable, pAllocator);
    
    dump_body_vkDestroyObjectTableNVX(ApiDumpInstance::current(), device, objectTable, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
    dump_head_vkCmdDebugMarkerBeginEXT(ApiDumpInstance::current(), commandBuffer, pMarkerInfo);
    device_dispatch_table(commandBuffer)->CmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
    
    dump_body_vkCmdDebugMarkerBeginEXT(ApiDumpInstance::current(), commandBuffer, pMarkerInfo);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo)
{
    dump_head_vkCmdDebugMarkerInsertEXT(ApiDumpInstance::current(), commandBuffer, pMarkerInfo);
    device_dispatch_table(commandBuffer)->CmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
    
    dump_body_vkCmdDebugMarkerInsertEXT(ApiDumpInstance::current(), commandBuffer, pMarkerInfo);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer)
{
    dump_head_vkCmdDebugMarkerEndEXT(ApiDumpInstance::current(), commandBuffer);
    device_dispatch_table(commandBuffer)->CmdDebugMarkerEndEXT(commandBuffer);
    
    dump_body_vkCmdDebugMarkerEndEXT(ApiDumpInstance::current(), commandBuffer);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery)
{
    dump_head_vkCmdWriteAccelerationStructuresPropertiesNV(ApiDumpInstance::current(), commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
    device_dispatch_table(commandBuffer)->CmdWriteAccelerationStructuresPropertiesNV(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
    
    dump_body_vkCmdWriteAccelerationStructuresPropertiesNV(ApiDumpInstance::current(), commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkQueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    dump_head_vkQueueInsertDebugUtilsLabelEXT(ApiDumpInstance::current(), queue, pLabelInfo);
    device_dispatch_table(queue)->QueueInsertDebugUtilsLabelEXT(queue, pLabelInfo);
    
    dump_body_vkQueueInsertDebugUtilsLabelEXT(ApiDumpInstance::current(), queue, pLabelInfo);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    dump_head_vkCmdBeginDebugUtilsLabelEXT(ApiDumpInstance::current(), commandBuffer, pLabelInfo);
    device_dispatch_table(commandBuffer)->CmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
    
    dump_body_vkCmdBeginDebugUtilsLabelEXT(ApiDumpInstance::current(), commandBuffer, pLabelInfo);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders)
{
    dump_head_vkCmdSetCoarseSampleOrderNV(ApiDumpInstance::current(), commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
    device_dispatch_table(commandBuffer)->CmdSetCoarseSampleOrderNV(commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
    
    dump_body_vkCmdSetCoarseSampleOrderNV(ApiDumpInstance::current(), commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer)
{
    dump_head_vkCmdEndDebugUtilsLabelEXT(ApiDumpInstance::current(), commandBuffer);
    device_dispatch_table(commandBuffer)->CmdEndDebugUtilsLabelEXT(commandBuffer);
    
    dump_body_vkCmdEndDebugUtilsLabelEXT(ApiDumpInstance::current(), commandBuffer);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    dump_head_vkCmdInsertDebugUtilsLabelEXT(ApiDumpInstance::current(), commandBuffer, pLabelInfo);
    device_dispatch_table(commandBuffer)->CmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
    
    dump_body_vkCmdInsertDebugUtilsLabelEXT(ApiDumpInstance::current(), commandBuffer, pLabelInfo);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings)
{
    dump_head_vkCmdSetViewportWScalingNV(ApiDumpInstance::current(), commandBuffer, firstViewport, viewportCount, pViewportWScalings);
    device_dispatch_table(commandBuffer)->CmdSetViewportWScalingNV(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
    
    dump_body_vkCmdSetViewportWScalingNV(ApiDumpInstance::current(), commandBuffer, firstViewport, viewportCount, pViewportWScalings);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkSetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable)
{
    dump_head_vkSetLocalDimmingAMD(ApiDumpInstance::current(), device, swapChain, localDimmingEnable);
    device_dispatch_table(device)->SetLocalDimmingAMD(device, swapChain, localDimmingEnable);
    
    dump_body_vkSetLocalDimmingAMD(ApiDumpInstance::current(), device, swapChain, localDimmingEnable);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin)
{
    dump_head_vkCmdBeginConditionalRenderingEXT(ApiDumpInstance::current(), commandBuffer, pConditionalRenderingBegin);
    device_dispatch_table(commandBuffer)->CmdBeginConditionalRenderingEXT(commandBuffer, pConditionalRenderingBegin);
    
    dump_body_vkCmdBeginConditionalRenderingEXT(ApiDumpInstance::current(), commandBuffer, pConditionalRenderingBegin);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    dump_head_vkCmdDrawIndirectCountKHR(ApiDumpInstance::current(), commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
    device_dispatch_table(commandBuffer)->CmdDrawIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
    
    dump_body_vkCmdDrawIndirectCountKHR(ApiDumpInstance::current(), commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer)
{
    dump_head_vkCmdEndConditionalRenderingEXT(ApiDumpInstance::current(), commandBuffer);
    device_dispatch_table(commandBuffer)->CmdEndConditionalRenderingEXT(commandBuffer);
    
    dump_body_vkCmdEndConditionalRenderingEXT(ApiDumpInstance::current(), commandBuffer);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetDescriptorSetLayoutSupportKHR(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
{
    dump_head_vkGetDescriptorSetLayoutSupportKHR(ApiDumpInstance::current(), device, pCreateInfo, pSupport);
    device_dispatch_table(device)->GetDescriptorSetLayoutSupportKHR(device, pCreateInfo, pSupport);
    
    dump_body_vkGetDescriptorSetLayoutSupportKHR(ApiDumpInstance::current(), device, pCreateInfo, pSupport);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroySampler(ApiDumpInstance::current(), device, sampler, pAllocator);
    device_dispatch_table(device)->DestroySampler(device, sampler, pAllocator);
    
    dump_body_vkDestroySampler(ApiDumpInstance::current(), device, sampler, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    dump_head_vkCmdDrawIndexedIndirectCountKHR(ApiDumpInstance::current(), commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
    device_dispatch_table(commandBuffer)->CmdDrawIndexedIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
    
    dump_body_vkCmdDrawIndexedIndirectCountKHR(ApiDumpInstance::current(), commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport)
{
    dump_head_vkGetDescriptorSetLayoutSupport(ApiDumpInstance::current(), device, pCreateInfo, pSupport);
    device_dispatch_table(device)->GetDescriptorSetLayoutSupport(device, pCreateInfo, pSupport);
    
    dump_body_vkGetDescriptorSetLayoutSupport(ApiDumpInstance::current(), device, pCreateInfo, pSupport);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    dump_head_vkResetQueryPoolEXT(ApiDumpInstance::current(), device, queryPool, firstQuery, queryCount);
    device_dispatch_table(device)->ResetQueryPoolEXT(device, queryPool, firstQuery, queryCount);
    
    dump_body_vkResetQueryPoolEXT(ApiDumpInstance::current(), device, queryPool, firstQuery, queryCount);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern)
{
    dump_head_vkCmdSetLineStippleEXT(ApiDumpInstance::current(), commandBuffer, lineStippleFactor, lineStipplePattern);
    device_dispatch_table(commandBuffer)->CmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
    
    dump_body_vkCmdSetLineStippleEXT(ApiDumpInstance::current(), commandBuffer, lineStippleFactor, lineStipplePattern);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyDescriptorSetLayout(ApiDumpInstance::current(), device, descriptorSetLayout, pAllocator);
    device_dispatch_table(device)->DestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
    
    dump_body_vkDestroyDescriptorSetLayout(ApiDumpInstance::current(), device, descriptorSetLayout, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkUpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData)
{
    dump_head_vkUpdateDescriptorSetWithTemplateKHR(ApiDumpInstance::current(), device, descriptorSet, descriptorUpdateTemplate, pData);
    device_dispatch_table(device)->UpdateDescriptorSetWithTemplateKHR(device, descriptorSet, descriptorUpdateTemplate, pData);
    
    dump_body_vkUpdateDescriptorSetWithTemplateKHR(ApiDumpInstance::current(), device, descriptorSet, descriptorUpdateTemplate, pData);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroySamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroySamplerYcbcrConversionKHR(ApiDumpInstance::current(), device, ycbcrConversion, pAllocator);
    device_dispatch_table(device)->DestroySamplerYcbcrConversionKHR(device, ycbcrConversion, pAllocator);
    
    dump_body_vkDestroySamplerYcbcrConversionKHR(ApiDumpInstance::current(), device, ycbcrConversion, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue)
{
    dump_head_vkGetDeviceQueue2(ApiDumpInstance::current(), device, pQueueInfo, pQueue);
    device_dispatch_table(device)->GetDeviceQueue2(device, pQueueInfo, pQueue);
    
    dump_body_vkGetDeviceQueue2(ApiDumpInstance::current(), device, pQueueInfo, pQueue);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyDescriptorUpdateTemplateKHR(ApiDumpInstance::current(), device, descriptorUpdateTemplate, pAllocator);
    device_dispatch_table(device)->DestroyDescriptorUpdateTemplateKHR(device, descriptorUpdateTemplate, pAllocator);
    
    dump_body_vkDestroyDescriptorUpdateTemplateKHR(ApiDumpInstance::current(), device, descriptorUpdateTemplate, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyValidationCacheEXT(ApiDumpInstance::current(), device, validationCache, pAllocator);
    device_dispatch_table(device)->DestroyValidationCacheEXT(device, validationCache, pAllocator);
    
    dump_body_vkDestroyValidationCacheEXT(ApiDumpInstance::current(), device, validationCache, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
    dump_head_vkCmdWaitEvents(ApiDumpInstance::current(), commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
    device_dispatch_table(commandBuffer)->CmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
    
    dump_body_vkCmdWaitEvents(ApiDumpInstance::current(), commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
{
    dump_head_vkCmdBindDescriptorSets(ApiDumpInstance::current(), commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
    device_dispatch_table(commandBuffer)->CmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
    
    dump_body_vkCmdBindDescriptorSets(ApiDumpInstance::current(), commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference)
{
    dump_head_vkCmdSetStencilReference(ApiDumpInstance::current(), commandBuffer, faceMask, reference);
    device_dispatch_table(commandBuffer)->CmdSetStencilReference(commandBuffer, faceMask, reference);
    
    dump_body_vkCmdSetStencilReference(ApiDumpInstance::current(), commandBuffer, faceMask, reference);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkUpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData)
{
    dump_head_vkUpdateDescriptorSetWithTemplate(ApiDumpInstance::current(), device, descriptorSet, descriptorUpdateTemplate, pData);
    device_dispatch_table(device)->UpdateDescriptorSetWithTemplate(device, descriptorSet, descriptorUpdateTemplate, pData);
    
    dump_body_vkUpdateDescriptorSetWithTemplate(ApiDumpInstance::current(), device, descriptorSet, descriptorUpdateTemplate, pData);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
    dump_head_vkCmdBindIndexBuffer(ApiDumpInstance::current(), commandBuffer, buffer, offset, indexType);
    device_dispatch_table(commandBuffer)->CmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
    
    dump_body_vkCmdBindIndexBuffer(ApiDumpInstance::current(), commandBuffer, buffer, offset, indexType);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyDescriptorUpdateTemplate(ApiDumpInstance::current(), device, descriptorUpdateTemplate, pAllocator);
    device_dispatch_table(device)->DestroyDescriptorUpdateTemplate(device, descriptorUpdateTemplate, pAllocator);
    
    dump_body_vkDestroyDescriptorUpdateTemplate(ApiDumpInstance::current(), device, descriptorUpdateTemplate, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
{
    dump_head_vkCmdBindVertexBuffers(ApiDumpInstance::current(), commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
    device_dispatch_table(commandBuffer)->CmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
    
    dump_body_vkCmdBindVertexBuffers(ApiDumpInstance::current(), commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
    dump_head_vkCmdPipelineBarrier(ApiDumpInstance::current(), commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
    device_dispatch_table(commandBuffer)->CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
    
    dump_body_vkCmdPipelineBarrier(ApiDumpInstance::current(), commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    dump_head_vkCmdDraw(ApiDumpInstance::current(), commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
    device_dispatch_table(commandBuffer)->CmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
    
    dump_body_vkCmdDraw(ApiDumpInstance::current(), commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyDescriptorPool(ApiDumpInstance::current(), device, descriptorPool, pAllocator);
    device_dispatch_table(device)->DestroyDescriptorPool(device, descriptorPool, pAllocator);
    
    dump_body_vkDestroyDescriptorPool(ApiDumpInstance::current(), device, descriptorPool, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkTrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags flags)
{
    dump_head_vkTrimCommandPool(ApiDumpInstance::current(), device, commandPool, flags);
    device_dispatch_table(device)->TrimCommandPool(device, commandPool, flags);
    
    dump_body_vkTrimCommandPool(ApiDumpInstance::current(), device, commandPool, flags);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
    dump_head_vkCmdDrawIndexed(ApiDumpInstance::current(), commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    device_dispatch_table(commandBuffer)->CmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    
    dump_body_vkCmdDrawIndexed(ApiDumpInstance::current(), commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    dump_head_vkCmdDrawMeshTasksIndirectNV(ApiDumpInstance::current(), commandBuffer, buffer, offset, drawCount, stride);
    device_dispatch_table(commandBuffer)->CmdDrawMeshTasksIndirectNV(commandBuffer, buffer, offset, drawCount, stride);
    
    dump_body_vkCmdDrawMeshTasksIndirectNV(ApiDumpInstance::current(), commandBuffer, buffer, offset, drawCount, stride);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
    dump_head_vkCmdDrawMeshTasksIndirectCountNV(ApiDumpInstance::current(), commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
    device_dispatch_table(commandBuffer)->CmdDrawMeshTasksIndirectCountNV(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
    
    dump_body_vkCmdDrawMeshTasksIndirectCountNV(ApiDumpInstance::current(), commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    dump_head_vkCmdDrawIndirect(ApiDumpInstance::current(), commandBuffer, buffer, offset, drawCount, stride);
    device_dispatch_table(commandBuffer)->CmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
    
    dump_body_vkCmdDrawIndirect(ApiDumpInstance::current(), commandBuffer, buffer, offset, drawCount, stride);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask)
{
    dump_head_vkCmdDrawMeshTasksNV(ApiDumpInstance::current(), commandBuffer, taskCount, firstTask);
    device_dispatch_table(commandBuffer)->CmdDrawMeshTasksNV(commandBuffer, taskCount, firstTask);
    
    dump_body_vkCmdDrawMeshTasksNV(ApiDumpInstance::current(), commandBuffer, taskCount, firstTask);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    dump_head_vkCmdDrawIndexedIndirect(ApiDumpInstance::current(), commandBuffer, buffer, offset, drawCount, stride);
    device_dispatch_table(commandBuffer)->CmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
    
    dump_body_vkCmdDrawIndexedIndirect(ApiDumpInstance::current(), commandBuffer, buffer, offset, drawCount, stride);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
{
    dump_head_vkCmdBeginQuery(ApiDumpInstance::current(), commandBuffer, queryPool, query, flags);
    device_dispatch_table(commandBuffer)->CmdBeginQuery(commandBuffer, queryPool, query, flags);
    
    dump_body_vkCmdBeginQuery(ApiDumpInstance::current(), commandBuffer, queryPool, query, flags);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkUpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies)
{
    dump_head_vkUpdateDescriptorSets(ApiDumpInstance::current(), device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
    device_dispatch_table(device)->UpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
    
    dump_body_vkUpdateDescriptorSets(ApiDumpInstance::current(), device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    dump_head_vkCmdDispatch(ApiDumpInstance::current(), commandBuffer, groupCountX, groupCountY, groupCountZ);
    device_dispatch_table(commandBuffer)->CmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
    
    dump_body_vkCmdDispatch(ApiDumpInstance::current(), commandBuffer, groupCountX, groupCountY, groupCountZ);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions)
{
    dump_head_vkCmdCopyBuffer(ApiDumpInstance::current(), commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
    device_dispatch_table(commandBuffer)->CmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
    
    dump_body_vkCmdCopyBuffer(ApiDumpInstance::current(), commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
    dump_head_vkCmdResetQueryPool(ApiDumpInstance::current(), commandBuffer, queryPool, firstQuery, queryCount);
    device_dispatch_table(commandBuffer)->CmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
    
    dump_body_vkCmdResetQueryPool(ApiDumpInstance::current(), commandBuffer, queryPool, firstQuery, queryCount);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
{
    dump_head_vkCmdEndQuery(ApiDumpInstance::current(), commandBuffer, queryPool, query);
    device_dispatch_table(commandBuffer)->CmdEndQuery(commandBuffer, queryPool, query);
    
    dump_body_vkCmdEndQuery(ApiDumpInstance::current(), commandBuffer, queryPool, query);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query)
{
    dump_head_vkCmdWriteTimestamp(ApiDumpInstance::current(), commandBuffer, pipelineStage, queryPool, query);
    device_dispatch_table(commandBuffer)->CmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
    
    dump_body_vkCmdWriteTimestamp(ApiDumpInstance::current(), commandBuffer, pipelineStage, queryPool, query);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkQueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo)
{
    dump_head_vkQueueBeginDebugUtilsLabelEXT(ApiDumpInstance::current(), queue, pLabelInfo);
    device_dispatch_table(queue)->QueueBeginDebugUtilsLabelEXT(queue, pLabelInfo);
    
    dump_body_vkQueueBeginDebugUtilsLabelEXT(ApiDumpInstance::current(), queue, pLabelInfo);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
{
    dump_head_vkCmdDispatchIndirect(ApiDumpInstance::current(), commandBuffer, buffer, offset);
    device_dispatch_table(commandBuffer)->CmdDispatchIndirect(commandBuffer, buffer, offset);
    
    dump_body_vkCmdDispatchIndirect(ApiDumpInstance::current(), commandBuffer, buffer, offset);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions)
{
    dump_head_vkCmdCopyImage(ApiDumpInstance::current(), commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
    device_dispatch_table(commandBuffer)->CmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
    
    dump_body_vkCmdCopyImage(ApiDumpInstance::current(), commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkQueueEndDebugUtilsLabelEXT(VkQueue queue)
{
    dump_head_vkQueueEndDebugUtilsLabelEXT(ApiDumpInstance::current(), queue);
    device_dispatch_table(queue)->QueueEndDebugUtilsLabelEXT(queue);
    
    dump_body_vkQueueEndDebugUtilsLabelEXT(ApiDumpInstance::current(), queue);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
    dump_head_vkGetDeviceGroupPeerMemoryFeatures(ApiDumpInstance::current(), device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
    device_dispatch_table(device)->GetDeviceGroupPeerMemoryFeatures(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
    
    dump_body_vkGetDeviceGroupPeerMemoryFeatures(ApiDumpInstance::current(), device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
{
    dump_head_vkGetDeviceQueue(ApiDumpInstance::current(), device, queueFamilyIndex, queueIndex, pQueue);
    device_dispatch_table(device)->GetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
    
    dump_body_vkGetDeviceQueue(ApiDumpInstance::current(), device, queueFamilyIndex, queueIndex, pQueue);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
    dump_head_vkCmdClearColorImage(ApiDumpInstance::current(), commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
    device_dispatch_table(commandBuffer)->CmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
    
    dump_body_vkCmdClearColorImage(ApiDumpInstance::current(), commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity)
{
    dump_head_vkGetRenderAreaGranularity(ApiDumpInstance::current(), device, renderPass, pGranularity);
    device_dispatch_table(device)->GetRenderAreaGranularity(device, renderPass, pGranularity);
    
    dump_body_vkGetRenderAreaGranularity(ApiDumpInstance::current(), device, renderPass, pGranularity);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements)
{
    dump_head_vkGetImageSparseMemoryRequirements2(ApiDumpInstance::current(), device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
    device_dispatch_table(device)->GetImageSparseMemoryRequirements2(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
    
    dump_body_vkGetImageSparseMemoryRequirements2(ApiDumpInstance::current(), device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask)
{
    dump_head_vkCmdSetDeviceMask(ApiDumpInstance::current(), commandBuffer, deviceMask);
    device_dispatch_table(commandBuffer)->CmdSetDeviceMask(commandBuffer, deviceMask);
    
    dump_body_vkCmdSetDeviceMask(ApiDumpInstance::current(), commandBuffer, deviceMask);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles)
{
    dump_head_vkCmdSetDiscardRectangleEXT(ApiDumpInstance::current(), commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
    device_dispatch_table(commandBuffer)->CmdSetDiscardRectangleEXT(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
    
    dump_body_vkCmdSetDiscardRectangleEXT(ApiDumpInstance::current(), commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
    dump_head_vkCmdClearDepthStencilImage(ApiDumpInstance::current(), commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
    device_dispatch_table(commandBuffer)->CmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
    
    dump_body_vkCmdClearDepthStencilImage(ApiDumpInstance::current(), commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    dump_head_vkGetImageMemoryRequirements2(ApiDumpInstance::current(), device, pInfo, pMemoryRequirements);
    device_dispatch_table(device)->GetImageMemoryRequirements2(device, pInfo, pMemoryRequirements);
    
    dump_body_vkGetImageMemoryRequirements2(ApiDumpInstance::current(), device, pInfo, pMemoryRequirements);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    dump_head_vkCmdDispatchBase(ApiDumpInstance::current(), commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
    device_dispatch_table(commandBuffer)->CmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
    
    dump_body_vkCmdDispatchBase(ApiDumpInstance::current(), commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements)
{
    dump_head_vkGetBufferMemoryRequirements2(ApiDumpInstance::current(), device, pInfo, pMemoryRequirements);
    device_dispatch_table(device)->GetBufferMemoryRequirements2(device, pInfo, pMemoryRequirements);
    
    dump_body_vkGetBufferMemoryRequirements2(ApiDumpInstance::current(), device, pInfo, pMemoryRequirements);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR*        pSubpassEndInfo)
{
    dump_head_vkCmdEndRenderPass2KHR(ApiDumpInstance::current(), commandBuffer, pSubpassEndInfo);
    device_dispatch_table(commandBuffer)->CmdEndRenderPass2KHR(commandBuffer, pSubpassEndInfo);
    
    dump_body_vkCmdEndRenderPass2KHR(ApiDumpInstance::current(), commandBuffer, pSubpassEndInfo);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects)
{
    dump_head_vkCmdClearAttachments(ApiDumpInstance::current(), commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
    device_dispatch_table(commandBuffer)->CmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
    
    dump_body_vkCmdClearAttachments(ApiDumpInstance::current(), commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
    dump_head_vkFreeCommandBuffers(ApiDumpInstance::current(), device, commandPool, commandBufferCount, pCommandBuffers);
    device_dispatch_table(device)->FreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
    ApiDumpInstance::current().eraseCmdBuffers(device, commandPool, std::vector<VkCommandBuffer>(pCommandBuffers, pCommandBuffers + commandBufferCount));
    dump_body_vkFreeCommandBuffers(ApiDumpInstance::current(), device, commandPool, commandBufferCount, pCommandBuffers);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeaturesKHR(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures)
{
    dump_head_vkGetDeviceGroupPeerMemoryFeaturesKHR(ApiDumpInstance::current(), device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
    device_dispatch_table(device)->GetDeviceGroupPeerMemoryFeaturesKHR(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
    
    dump_body_vkGetDeviceGroupPeerMemoryFeaturesKHR(ApiDumpInstance::current(), device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask)
{
    dump_head_vkCmdSetDeviceMaskKHR(ApiDumpInstance::current(), commandBuffer, deviceMask);
    device_dispatch_table(commandBuffer)->CmdSetDeviceMaskKHR(commandBuffer, deviceMask);
    
    dump_body_vkCmdSetDeviceMaskKHR(ApiDumpInstance::current(), commandBuffer, deviceMask);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyCommandPool(ApiDumpInstance::current(), device, commandPool, pAllocator);
    device_dispatch_table(device)->DestroyCommandPool(device, commandPool, pAllocator);
    ApiDumpInstance::current().eraseCmdBufferPool(device, commandPool);
    dump_body_vkDestroyCommandPool(ApiDumpInstance::current(), device, commandPool, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo*      pRenderPassBegin, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo)
{
    dump_head_vkCmdBeginRenderPass2KHR(ApiDumpInstance::current(), commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
    device_dispatch_table(commandBuffer)->CmdBeginRenderPass2KHR(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
    
    dump_body_vkCmdBeginRenderPass2KHR(ApiDumpInstance::current(), commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions)
{
    dump_head_vkCmdResolveImage(ApiDumpInstance::current(), commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
    device_dispatch_table(commandBuffer)->CmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
    
    dump_body_vkCmdResolveImage(ApiDumpInstance::current(), commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    dump_head_vkCmdDispatchBaseKHR(ApiDumpInstance::current(), commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
    device_dispatch_table(commandBuffer)->CmdDispatchBaseKHR(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
    
    dump_body_vkCmdDispatchBaseKHR(ApiDumpInstance::current(), commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    dump_head_vkCmdResetEvent(ApiDumpInstance::current(), commandBuffer, event, stageMask);
    device_dispatch_table(commandBuffer)->CmdResetEvent(commandBuffer, event, stageMask);
    
    dump_body_vkCmdResetEvent(ApiDumpInstance::current(), commandBuffer, event, stageMask);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroySamplerYcbcrConversion(ApiDumpInstance::current(), device, ycbcrConversion, pAllocator);
    device_dispatch_table(device)->DestroySamplerYcbcrConversion(device, ycbcrConversion, pAllocator);
    
    dump_body_vkDestroySamplerYcbcrConversion(ApiDumpInstance::current(), device, ycbcrConversion, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites)
{
    dump_head_vkCmdPushDescriptorSetKHR(ApiDumpInstance::current(), commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
    device_dispatch_table(commandBuffer)->CmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
    
    dump_body_vkCmdPushDescriptorSetKHR(ApiDumpInstance::current(), commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR*      pSubpassBeginInfo, const VkSubpassEndInfoKHR*        pSubpassEndInfo)
{
    dump_head_vkCmdNextSubpass2KHR(ApiDumpInstance::current(), commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
    device_dispatch_table(commandBuffer)->CmdNextSubpass2KHR(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
    
    dump_body_vkCmdNextSubpass2KHR(ApiDumpInstance::current(), commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes)
{
    dump_head_vkCmdSetViewportShadingRatePaletteNV(ApiDumpInstance::current(), commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
    device_dispatch_table(commandBuffer)->CmdSetViewportShadingRatePaletteNV(commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
    
    dump_body_vkCmdSetViewportShadingRatePaletteNV(ApiDumpInstance::current(), commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData)
{
    dump_head_vkCmdPushDescriptorSetWithTemplateKHR(ApiDumpInstance::current(), commandBuffer, descriptorUpdateTemplate, layout, set, pData);
    device_dispatch_table(commandBuffer)->CmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
    
    dump_body_vkCmdPushDescriptorSetWithTemplateKHR(ApiDumpInstance::current(), commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
    dump_head_vkCmdSetEvent(ApiDumpInstance::current(), commandBuffer, event, stageMask);
    device_dispatch_table(commandBuffer)->CmdSetEvent(commandBuffer, event, stageMask);
    
    dump_body_vkCmdSetEvent(ApiDumpInstance::current(), commandBuffer, event, stageMask);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkSetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata)
{
    dump_head_vkSetHdrMetadataEXT(ApiDumpInstance::current(), device, swapchainCount, pSwapchains, pMetadata);
    device_dispatch_table(device)->SetHdrMetadataEXT(device, swapchainCount, pSwapchains, pMetadata);
    
    dump_body_vkSetHdrMetadataEXT(ApiDumpInstance::current(), device, swapchainCount, pSwapchains, pMetadata);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout)
{
    dump_head_vkCmdBindShadingRateImageNV(ApiDumpInstance::current(), commandBuffer, imageView, imageLayout);
    device_dispatch_table(commandBuffer)->CmdBindShadingRateImageNV(commandBuffer, imageView, imageLayout);
    
    dump_body_vkCmdBindShadingRateImageNV(ApiDumpInstance::current(), commandBuffer, imageView, imageLayout);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags)
{
    dump_head_vkCmdCopyQueryPoolResults(ApiDumpInstance::current(), commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
    device_dispatch_table(commandBuffer)->CmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
    
    dump_body_vkCmdCopyQueryPoolResults(ApiDumpInstance::current(), commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
    dump_head_vkCmdPushConstants(ApiDumpInstance::current(), commandBuffer, layout, stageFlags, offset, size, pValues);
    device_dispatch_table(commandBuffer)->CmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
    
    dump_body_vkCmdPushConstants(ApiDumpInstance::current(), commandBuffer, layout, stageFlags, offset, size, pValues);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors)
{
    dump_head_vkCmdSetExclusiveScissorNV(ApiDumpInstance::current(), commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
    device_dispatch_table(commandBuffer)->CmdSetExclusiveScissorNV(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
    
    dump_body_vkCmdSetExclusiveScissorNV(ApiDumpInstance::current(), commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents)
{
    dump_head_vkCmdNextSubpass(ApiDumpInstance::current(), commandBuffer, contents);
    device_dispatch_table(commandBuffer)->CmdNextSubpass(commandBuffer, contents);
    
    dump_body_vkCmdNextSubpass(ApiDumpInstance::current(), commandBuffer, contents);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker)
{
    dump_head_vkCmdSetCheckpointNV(ApiDumpInstance::current(), commandBuffer, pCheckpointMarker);
    device_dispatch_table(commandBuffer)->CmdSetCheckpointNV(commandBuffer, pCheckpointMarker);
    
    dump_body_vkCmdSetCheckpointNV(ApiDumpInstance::current(), commandBuffer, pCheckpointMarker);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents)
{
    dump_head_vkCmdBeginRenderPass(ApiDumpInstance::current(), commandBuffer, pRenderPassBegin, contents);
    device_dispatch_table(commandBuffer)->CmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
    
    dump_body_vkCmdBeginRenderPass(ApiDumpInstance::current(), commandBuffer, pRenderPassBegin, contents);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData)
{
    dump_head_vkGetQueueCheckpointDataNV(ApiDumpInstance::current(), queue, pCheckpointDataCount, pCheckpointData);
    device_dispatch_table(queue)->GetQueueCheckpointDataNV(queue, pCheckpointDataCount, pCheckpointData);
    
    dump_body_vkGetQueueCheckpointDataNV(ApiDumpInstance::current(), queue, pCheckpointDataCount, pCheckpointData);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdEndRenderPass(VkCommandBuffer commandBuffer)
{
    dump_head_vkCmdEndRenderPass(ApiDumpInstance::current(), commandBuffer);
    device_dispatch_table(commandBuffer)->CmdEndRenderPass(commandBuffer);
    
    dump_body_vkCmdEndRenderPass(ApiDumpInstance::current(), commandBuffer);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyFramebuffer(ApiDumpInstance::current(), device, framebuffer, pAllocator);
    device_dispatch_table(device)->DestroyFramebuffer(device, framebuffer, pAllocator);
    
    dump_body_vkDestroyFramebuffer(ApiDumpInstance::current(), device, framebuffer, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
    dump_head_vkCmdExecuteCommands(ApiDumpInstance::current(), commandBuffer, commandBufferCount, pCommandBuffers);
    device_dispatch_table(commandBuffer)->CmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
    
    dump_body_vkCmdExecuteCommands(ApiDumpInstance::current(), commandBuffer, commandBufferCount, pCommandBuffers);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyFence(ApiDumpInstance::current(), device, fence, pAllocator);
    device_dispatch_table(device)->DestroyFence(device, fence, pAllocator);
    
    dump_body_vkDestroyFence(ApiDumpInstance::current(), device, fence, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyImageView(ApiDumpInstance::current(), device, imageView, pAllocator);
    device_dispatch_table(device)->DestroyImageView(device, imageView, pAllocator);
    
    dump_body_vkDestroyImageView(ApiDumpInstance::current(), device, imageView, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroySemaphore(ApiDumpInstance::current(), device, semaphore, pAllocator);
    device_dispatch_table(device)->DestroySemaphore(device, semaphore, pAllocator);
    
    dump_body_vkDestroySemaphore(ApiDumpInstance::current(), device, semaphore, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyShaderModule(ApiDumpInstance::current(), device, shaderModule, pAllocator);
    device_dispatch_table(device)->DestroyShaderModule(device, shaderModule, pAllocator);
    
    dump_body_vkDestroyShaderModule(ApiDumpInstance::current(), device, shaderModule, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkUnmapMemory(VkDevice device, VkDeviceMemory memory)
{
    dump_head_vkUnmapMemory(ApiDumpInstance::current(), device, memory);
    device_dispatch_table(device)->UnmapMemory(device, memory);
    
    dump_body_vkUnmapMemory(ApiDumpInstance::current(), device, memory);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkFreeMemory(ApiDumpInstance::current(), device, memory, pAllocator);
    device_dispatch_table(device)->FreeMemory(device, memory, pAllocator);
    
    dump_body_vkFreeMemory(ApiDumpInstance::current(), device, memory, pAllocator);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes)
{
    dump_head_vkGetDeviceMemoryCommitment(ApiDumpInstance::current(), device, memory, pCommittedMemoryInBytes);
    device_dispatch_table(device)->GetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
    
    dump_body_vkGetDeviceMemoryCommitment(ApiDumpInstance::current(), device, memory, pCommittedMemoryInBytes);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements)
{
    dump_head_vkGetImageSparseMemoryRequirements(ApiDumpInstance::current(), device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
    device_dispatch_table(device)->GetImageSparseMemoryRequirements(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
    
    dump_body_vkGetImageSparseMemoryRequirements(ApiDumpInstance::current(), device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
{
    dump_head_vkGetBufferMemoryRequirements(ApiDumpInstance::current(), device, buffer, pMemoryRequirements);
    device_dispatch_table(device)->GetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
    
    dump_body_vkGetBufferMemoryRequirements(ApiDumpInstance::current(), device, buffer, pMemoryRequirements);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkGetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements)
{
    dump_head_vkGetImageMemoryRequirements(ApiDumpInstance::current(), device, image, pMemoryRequirements);
    device_dispatch_table(device)->GetImageMemoryRequirements(device, image, pMemoryRequirements);
    
    dump_body_vkGetImageMemoryRequirements(ApiDumpInstance::current(), device, image, pMemoryRequirements);
}
VK_LAYER_EXPORT VKAPI_ATTR void VKAPI_CALL vkDestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator)
{
    dump_head_vkDestroyBuffer(ApiDumpInstance::current(), device, buffer, pAllocator);
    device_dispatch_table(device)->DestroyBuffer(device, buffer, pAllocator);
    
    dump_body_vkDestroyBuffer(ApiDumpInstance::current(), device, buffer, pAllocator);
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char* pName)
{

    if(strcmp(pName, "vkGetDisplayPlaneSupportedDisplaysKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDisplayPlaneSupportedDisplaysKHR);
    if(strcmp(pName, "vkGetPhysicalDeviceCooperativeMatrixPropertiesNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceCooperativeMatrixPropertiesNV);
    if(strcmp(pName, "vkGetPhysicalDeviceDisplayPropertiesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceDisplayPropertiesKHR);
    if(strcmp(pName, "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceCalibrateableTimeDomainsEXT);
    if(strcmp(pName, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceDisplayPlanePropertiesKHR);
    if(strcmp(pName, "vkGetDisplayModePropertiesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDisplayModePropertiesKHR);
    if(strcmp(pName, "vkGetPhysicalDeviceProperties") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceProperties);
    if(strcmp(pName, "vkCreateDisplayModeKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateDisplayModeKHR);
    if(strcmp(pName, "vkGetDisplayPlaneCapabilitiesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDisplayPlaneCapabilitiesKHR);
    if(strcmp(pName, "vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV);
    if(strcmp(pName, "vkCreateDisplayPlaneSurfaceKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateDisplayPlaneSurfaceKHR);
    if(strcmp(pName, "vkGetPhysicalDeviceMultisamplePropertiesEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceMultisamplePropertiesEXT);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    if(strcmp(pName, "vkGetPhysicalDeviceSurfacePresentModes2EXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSurfacePresentModes2EXT);
#endif // VK_USE_PLATFORM_WIN32_KHR
    if(strcmp(pName, "vkGetPhysicalDeviceExternalImageFormatPropertiesNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceExternalImageFormatPropertiesNV);
    if(strcmp(pName, "vkGetPhysicalDeviceMemoryProperties") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceMemoryProperties);
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
    if(strcmp(pName, "vkCreateWaylandSurfaceKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateWaylandSurfaceKHR);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
    if(strcmp(pName, "vkGetPhysicalDeviceQueueFamilyProperties") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceQueueFamilyProperties);
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
    if(strcmp(pName, "vkGetPhysicalDeviceWaylandPresentationSupportKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceWaylandPresentationSupportKHR);
#endif // VK_USE_PLATFORM_WAYLAND_KHR
    if(strcmp(pName, "vkGetInstanceProcAddr") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetInstanceProcAddr);
    if(strcmp(pName, "vkEnumeratePhysicalDeviceGroupsKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkEnumeratePhysicalDeviceGroupsKHR);
    if(strcmp(pName, "vkGetPhysicalDeviceExternalBufferPropertiesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceExternalBufferPropertiesKHR);
#if defined(VK_USE_PLATFORM_XCB_KHR)
    if(strcmp(pName, "vkCreateXcbSurfaceKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateXcbSurfaceKHR);
#endif // VK_USE_PLATFORM_XCB_KHR
    if(strcmp(pName, "vkQueueInsertDebugUtilsLabelEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkQueueInsertDebugUtilsLabelEXT);
    if(strcmp(pName, "vkCmdBeginDebugUtilsLabelEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdBeginDebugUtilsLabelEXT);
    if(strcmp(pName, "vkCreateHeadlessSurfaceEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateHeadlessSurfaceEXT);
    if(strcmp(pName, "vkGetDisplayModeProperties2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDisplayModeProperties2KHR);
    if(strcmp(pName, "vkCmdEndDebugUtilsLabelEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdEndDebugUtilsLabelEXT);
    if(strcmp(pName, "vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX);
    if(strcmp(pName, "vkCmdInsertDebugUtilsLabelEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdInsertDebugUtilsLabelEXT);
    if(strcmp(pName, "vkCreateDebugUtilsMessengerEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateDebugUtilsMessengerEXT);
    if(strcmp(pName, "vkSubmitDebugUtilsMessageEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkSubmitDebugUtilsMessageEXT);
    if(strcmp(pName, "vkReleaseDisplayEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkReleaseDisplayEXT);
    if(strcmp(pName, "vkGetPhysicalDeviceDisplayProperties2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceDisplayProperties2KHR);
    if(strcmp(pName, "vkDestroyDebugUtilsMessengerEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyDebugUtilsMessengerEXT);
    if(strcmp(pName, "vkGetPhysicalDeviceExternalSemaphoreProperties") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceExternalSemaphoreProperties);
#if defined(VK_USE_PLATFORM_FUCHSIA)
    if(strcmp(pName, "vkCreateImagePipeSurfaceFUCHSIA") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateImagePipeSurfaceFUCHSIA);
#endif // VK_USE_PLATFORM_FUCHSIA
    if(strcmp(pName, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceDisplayPlaneProperties2KHR);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
    if(strcmp(pName, "vkCreateAndroidSurfaceKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateAndroidSurfaceKHR);
#endif // VK_USE_PLATFORM_ANDROID_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    if(strcmp(pName, "vkCreateXlibSurfaceKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateXlibSurfaceKHR);
#endif // VK_USE_PLATFORM_XLIB_KHR
    if(strcmp(pName, "vkGetDisplayPlaneCapabilities2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDisplayPlaneCapabilities2KHR);
#if defined(VK_USE_PLATFORM_METAL_EXT)
    if(strcmp(pName, "vkCreateMetalSurfaceEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateMetalSurfaceEXT);
#endif // VK_USE_PLATFORM_METAL_EXT
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
    if(strcmp(pName, "vkGetRandROutputDisplayEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetRandROutputDisplayEXT);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    if(strcmp(pName, "vkGetPhysicalDeviceXlibPresentationSupportKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceXlibPresentationSupportKHR);
#endif // VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_IOS_MVK)
    if(strcmp(pName, "vkCreateIOSSurfaceMVK") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateIOSSurfaceMVK);
#endif // VK_USE_PLATFORM_IOS_MVK
#if defined(VK_USE_PLATFORM_XLIB_XRANDR_EXT)
    if(strcmp(pName, "vkAcquireXlibDisplayEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkAcquireXlibDisplayEXT);
#endif // VK_USE_PLATFORM_XLIB_XRANDR_EXT
    if(strcmp(pName, "vkGetPhysicalDeviceSurfaceSupportKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSurfaceSupportKHR);
#if defined(VK_USE_PLATFORM_GGP)
    if(strcmp(pName, "vkCreateStreamDescriptorSurfaceGGP") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateStreamDescriptorSurfaceGGP);
#endif // VK_USE_PLATFORM_GGP
    if(strcmp(pName, "vkDestroySurfaceKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroySurfaceKHR);
#if defined(VK_USE_PLATFORM_MACOS_MVK)
    if(strcmp(pName, "vkCreateMacOSSurfaceMVK") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateMacOSSurfaceMVK);
#endif // VK_USE_PLATFORM_MACOS_MVK
    if(strcmp(pName, "vkGetPhysicalDeviceSurfaceFormatsKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSurfaceFormatsKHR);
    if(strcmp(pName, "vkGetPhysicalDeviceSurfaceCapabilities2EXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSurfaceCapabilities2EXT);
    if(strcmp(pName, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
    if(strcmp(pName, "vkGetPhysicalDeviceFeatures2") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceFeatures2);
    if(strcmp(pName, "vkGetPhysicalDeviceProperties2") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceProperties2);
    if(strcmp(pName, "vkGetPhysicalDeviceSurfaceFormats2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSurfaceFormats2KHR);
    if(strcmp(pName, "vkGetPhysicalDeviceMemoryProperties2") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceMemoryProperties2);
    if(strcmp(pName, "vkGetPhysicalDeviceFormatProperties2") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceFormatProperties2);
    if(strcmp(pName, "vkGetPhysicalDeviceSurfaceCapabilities2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSurfaceCapabilities2KHR);
    if(strcmp(pName, "vkGetPhysicalDeviceImageFormatProperties2") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceImageFormatProperties2);
    if(strcmp(pName, "vkGetPhysicalDeviceSparseImageFormatProperties2") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSparseImageFormatProperties2);
    if(strcmp(pName, "vkGetPhysicalDeviceQueueFamilyProperties2") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceQueueFamilyProperties2);
    if(strcmp(pName, "vkSetDebugUtilsObjectNameEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkSetDebugUtilsObjectNameEXT);
    if(strcmp(pName, "vkSetDebugUtilsObjectTagEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkSetDebugUtilsObjectTagEXT);
    if(strcmp(pName, "vkQueueBeginDebugUtilsLabelEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkQueueBeginDebugUtilsLabelEXT);
    if(strcmp(pName, "vkGetPhysicalDeviceExternalBufferProperties") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceExternalBufferProperties);
    if(strcmp(pName, "vkQueueEndDebugUtilsLabelEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkQueueEndDebugUtilsLabelEXT);
#if defined(VK_USE_PLATFORM_XCB_KHR)
    if(strcmp(pName, "vkGetPhysicalDeviceXcbPresentationSupportKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceXcbPresentationSupportKHR);
#endif // VK_USE_PLATFORM_XCB_KHR
    if(strcmp(pName, "vkGetPhysicalDeviceFeatures2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceFeatures2KHR);
    if(strcmp(pName, "vkCreateDevice") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateDevice);
    if(strcmp(pName, "vkGetPhysicalDeviceProperties2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceProperties2KHR);
    if(strcmp(pName, "vkGetPhysicalDeviceMemoryProperties2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceMemoryProperties2KHR);
    if(strcmp(pName, "vkGetPhysicalDeviceSparseImageFormatProperties") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSparseImageFormatProperties);
    if(strcmp(pName, "vkGetPhysicalDeviceFormatProperties2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceFormatProperties2KHR);
    if(strcmp(pName, "vkGetPhysicalDeviceSurfacePresentModesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSurfacePresentModesKHR);
    if(strcmp(pName, "vkGetPhysicalDeviceImageFormatProperties2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceImageFormatProperties2KHR);
    if(strcmp(pName, "vkGetPhysicalDeviceSparseImageFormatProperties2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceSparseImageFormatProperties2KHR);
    if(strcmp(pName, "vkGetPhysicalDeviceQueueFamilyProperties2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceQueueFamilyProperties2KHR);
    if(strcmp(pName, "vkGetPhysicalDeviceFormatProperties") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceFormatProperties);
    if(strcmp(pName, "vkGetPhysicalDevicePresentRectanglesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDevicePresentRectanglesKHR);
    if(strcmp(pName, "vkEnumeratePhysicalDeviceGroups") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkEnumeratePhysicalDeviceGroups);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    if(strcmp(pName, "vkCreateWin32SurfaceKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateWin32SurfaceKHR);
#endif // VK_USE_PLATFORM_WIN32_KHR
    if(strcmp(pName, "vkEnumerateDeviceLayerProperties") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkEnumerateDeviceLayerProperties);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    if(strcmp(pName, "vkGetPhysicalDeviceWin32PresentationSupportKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceWin32PresentationSupportKHR);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_VI_NN)
    if(strcmp(pName, "vkCreateViSurfaceNN") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateViSurfaceNN);
#endif // VK_USE_PLATFORM_VI_NN
    if(strcmp(pName, "vkGetPhysicalDeviceExternalFencePropertiesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceExternalFencePropertiesKHR);
    if(strcmp(pName, "vkGetPhysicalDeviceExternalFenceProperties") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceExternalFenceProperties);
    if(strcmp(pName, "vkDestroyInstance") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyInstance);
    if(strcmp(pName, "vkCreateInstance") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateInstance);
    if(strcmp(pName, "vkGetPhysicalDeviceFeatures") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceFeatures);
    if(strcmp(pName, "vkEnumeratePhysicalDevices") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkEnumeratePhysicalDevices);
    if(strcmp(pName, "vkCreateDebugReportCallbackEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateDebugReportCallbackEXT);
    if(strcmp(pName, "vkGetPhysicalDeviceImageFormatProperties") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceImageFormatProperties);
    if(strcmp(pName, "vkDebugReportMessageEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDebugReportMessageEXT);
    if(strcmp(pName, "vkDestroyDebugReportCallbackEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyDebugReportCallbackEXT);
    if(strcmp(pName, "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceExternalSemaphorePropertiesKHR);

    if(instance_dispatch_table(instance)->GetInstanceProcAddr == NULL)
        return NULL;
    return instance_dispatch_table(instance)->GetInstanceProcAddr(instance, pName);
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice device, const char* pName)
{

    if(strcmp(pName, "vkCreatePipelineCache") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreatePipelineCache);
    if(strcmp(pName, "vkCmdBindTransformFeedbackBuffersEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdBindTransformFeedbackBuffersEXT);
    if(strcmp(pName, "vkCreateEvent") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateEvent);
    if(strcmp(pName, "vkGetSwapchainImagesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetSwapchainImagesKHR);
    if(strcmp(pName, "vkCmdBeginTransformFeedbackEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdBeginTransformFeedbackEXT);
    if(strcmp(pName, "vkGetEventStatus") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetEventStatus);
    if(strcmp(pName, "vkCreateImage") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateImage);
    if(strcmp(pName, "vkGetPipelineCacheData") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPipelineCacheData);
    if(strcmp(pName, "vkCmdEndTransformFeedbackEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdEndTransformFeedbackEXT);
    if(strcmp(pName, "vkSetEvent") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkSetEvent);
    if(strcmp(pName, "vkDestroyBufferView") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyBufferView);
    if(strcmp(pName, "vkDestroyEvent") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyEvent);
    if(strcmp(pName, "vkDestroyPipelineCache") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyPipelineCache);
    if(strcmp(pName, "vkCmdBeginQueryIndexedEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdBeginQueryIndexedEXT);
    if(strcmp(pName, "vkGetImageSubresourceLayout") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetImageSubresourceLayout);
    if(strcmp(pName, "vkMergePipelineCaches") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkMergePipelineCaches);
    if(strcmp(pName, "vkResetEvent") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkResetEvent);
    if(strcmp(pName, "vkCreateGraphicsPipelines") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateGraphicsPipelines);
    if(strcmp(pName, "vkGetMemoryHostPointerPropertiesEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetMemoryHostPointerPropertiesEXT);
    if(strcmp(pName, "vkCmdEndQueryIndexedEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdEndQueryIndexedEXT);
    if(strcmp(pName, "vkCmdDrawIndirectByteCountEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDrawIndirectByteCountEXT);
    if(strcmp(pName, "vkCreateQueryPool") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateQueryPool);
    if(strcmp(pName, "vkCmdWriteBufferMarkerAMD") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdWriteBufferMarkerAMD);
    if(strcmp(pName, "vkDestroySwapchainKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroySwapchainKHR);
    if(strcmp(pName, "vkAcquireNextImageKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkAcquireNextImageKHR);
    if(strcmp(pName, "vkQueuePresentKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkQueuePresentKHR);
    if(strcmp(pName, "vkCreateComputePipelines") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateComputePipelines);
    if(strcmp(pName, "vkDestroyImage") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyImage);
    if(strcmp(pName, "vkGetCalibratedTimestampsEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetCalibratedTimestampsEXT);
    if(strcmp(pName, "vkCreateImageView") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateImageView);
    if(strcmp(pName, "vkDestroyQueryPool") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyQueryPool);
    if(strcmp(pName, "vkCmdBlitImage") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdBlitImage);
    if(strcmp(pName, "vkCreatePipelineLayout") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreatePipelineLayout);
    if(strcmp(pName, "vkCmdBuildAccelerationStructureNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdBuildAccelerationStructureNV);
    if(strcmp(pName, "vkGetAccelerationStructureMemoryRequirementsNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetAccelerationStructureMemoryRequirementsNV);
    if(strcmp(pName, "vkDestroyAccelerationStructureNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyAccelerationStructureNV);
    if(strcmp(pName, "vkBindAccelerationStructureMemoryNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkBindAccelerationStructureMemoryNV);
    if(strcmp(pName, "vkResetCommandBuffer") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkResetCommandBuffer);
    if(strcmp(pName, "vkCmdCopyBufferToImage") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdCopyBufferToImage);
    if(strcmp(pName, "vkCmdSetViewport") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetViewport);
    if(strcmp(pName, "vkDestroyPipeline") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyPipeline);
    if(strcmp(pName, "vkCmdCopyAccelerationStructureNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdCopyAccelerationStructureNV);
    if(strcmp(pName, "vkCmdBindPipeline") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdBindPipeline);
    if(strcmp(pName, "vkCmdSetSampleLocationsEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetSampleLocationsEXT);
    if(strcmp(pName, "vkGetImageViewHandleNVX") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetImageViewHandleNVX);
    if(strcmp(pName, "vkGetImageSparseMemoryRequirements2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetImageSparseMemoryRequirements2KHR);
    if(strcmp(pName, "vkCmdSetLineWidth") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetLineWidth);
    if(strcmp(pName, "vkInitializePerformanceApiINTEL") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkInitializePerformanceApiINTEL);
    if(strcmp(pName, "vkCreateSampler") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateSampler);
    if(strcmp(pName, "vkCmdSetScissor") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetScissor);
    if(strcmp(pName, "vkUninitializePerformanceApiINTEL") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkUninitializePerformanceApiINTEL);
    if(strcmp(pName, "vkCmdSetDepthBias") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetDepthBias);
    if(strcmp(pName, "vkCmdTraceRaysNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdTraceRaysNV);
    if(strcmp(pName, "vkCmdSetPerformanceMarkerINTEL") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetPerformanceMarkerINTEL);
    if(strcmp(pName, "vkCmdDrawIndirectCountAMD") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDrawIndirectCountAMD);
    if(strcmp(pName, "vkCmdDrawIndexedIndirectCountAMD") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDrawIndexedIndirectCountAMD);
    if(strcmp(pName, "vkGetImageMemoryRequirements2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetImageMemoryRequirements2KHR);
    if(strcmp(pName, "vkCmdSetPerformanceStreamMarkerINTEL") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetPerformanceStreamMarkerINTEL);
    if(strcmp(pName, "vkCreateRayTracingPipelinesNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateRayTracingPipelinesNV);
    if(strcmp(pName, "vkGetBufferMemoryRequirements2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetBufferMemoryRequirements2KHR);
    if(strcmp(pName, "vkCmdSetPerformanceOverrideINTEL") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetPerformanceOverrideINTEL);
    if(strcmp(pName, "vkCmdCopyImageToBuffer") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdCopyImageToBuffer);
    if(strcmp(pName, "vkCmdSetBlendConstants") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetBlendConstants);
    if(strcmp(pName, "vkReleasePerformanceConfigurationINTEL") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkReleasePerformanceConfigurationINTEL);
    if(strcmp(pName, "vkCmdSetStencilWriteMask") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetStencilWriteMask);
    if(strcmp(pName, "vkAcquirePerformanceConfigurationINTEL") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkAcquirePerformanceConfigurationINTEL);
    if(strcmp(pName, "vkDestroyPipelineLayout") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyPipelineLayout);
    if(strcmp(pName, "vkGetBufferDeviceAddressEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetBufferDeviceAddressEXT);
    if(strcmp(pName, "vkCreateSharedSwapchainsKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateSharedSwapchainsKHR);
    if(strcmp(pName, "vkQueueSetPerformanceConfigurationINTEL") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkQueueSetPerformanceConfigurationINTEL);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    if(strcmp(pName, "vkReleaseFullScreenExclusiveModeEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkReleaseFullScreenExclusiveModeEXT);
#endif // VK_USE_PLATFORM_WIN32_KHR
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    if(strcmp(pName, "vkAcquireFullScreenExclusiveModeEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkAcquireFullScreenExclusiveModeEXT);
#endif // VK_USE_PLATFORM_WIN32_KHR
    if(strcmp(pName, "vkCmdSetDepthBounds") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetDepthBounds);
    if(strcmp(pName, "vkGetRayTracingShaderGroupHandlesNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetRayTracingShaderGroupHandlesNV);
    if(strcmp(pName, "vkGetPerformanceParameterINTEL") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPerformanceParameterINTEL);
    if(strcmp(pName, "vkDestroyRenderPass") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyRenderPass);
    if(strcmp(pName, "vkCmdUpdateBuffer") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdUpdateBuffer);
    if(strcmp(pName, "vkCmdSetStencilCompareMask") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetStencilCompareMask);
    if(strcmp(pName, "vkGetAccelerationStructureHandleNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetAccelerationStructureHandleNV);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    if(strcmp(pName, "vkGetDeviceGroupSurfacePresentModes2EXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDeviceGroupSurfacePresentModes2EXT);
#endif // VK_USE_PLATFORM_WIN32_KHR
    if(strcmp(pName, "vkCmdFillBuffer") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdFillBuffer);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    if(strcmp(pName, "vkGetMemoryWin32HandleNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetMemoryWin32HandleNV);
#endif // VK_USE_PLATFORM_WIN32_KHR
    if(strcmp(pName, "vkCmdProcessCommandsNVX") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdProcessCommandsNVX);
    if(strcmp(pName, "vkTrimCommandPoolKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkTrimCommandPoolKHR);
    if(strcmp(pName, "vkDebugMarkerSetObjectTagEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDebugMarkerSetObjectTagEXT);
    if(strcmp(pName, "vkCmdReserveSpaceForCommandsNVX") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdReserveSpaceForCommandsNVX);
    if(strcmp(pName, "vkCreateIndirectCommandsLayoutNVX") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateIndirectCommandsLayoutNVX);
    if(strcmp(pName, "vkCreateObjectTableNVX") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateObjectTableNVX);
    if(strcmp(pName, "vkRegisterObjectsNVX") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkRegisterObjectsNVX);
    if(strcmp(pName, "vkDestroyIndirectCommandsLayoutNVX") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyIndirectCommandsLayoutNVX);
    if(strcmp(pName, "vkDebugMarkerSetObjectNameEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDebugMarkerSetObjectNameEXT);
    if(strcmp(pName, "vkUnregisterObjectsNVX") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkUnregisterObjectsNVX);
    if(strcmp(pName, "vkDestroyObjectTableNVX") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyObjectTableNVX);
    if(strcmp(pName, "vkCmdDebugMarkerBeginEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDebugMarkerBeginEXT);
    if(strcmp(pName, "vkCreateAccelerationStructureNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateAccelerationStructureNV);
    if(strcmp(pName, "vkGetPipelineExecutablePropertiesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPipelineExecutablePropertiesKHR);
    if(strcmp(pName, "vkCmdDebugMarkerInsertEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDebugMarkerInsertEXT);
    if(strcmp(pName, "vkCmdDebugMarkerEndEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDebugMarkerEndEXT);
    if(strcmp(pName, "vkBindBufferMemory2") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkBindBufferMemory2);
    if(strcmp(pName, "vkCmdWriteAccelerationStructuresPropertiesNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdWriteAccelerationStructuresPropertiesNV);
    if(strcmp(pName, "vkCmdSetCoarseSampleOrderNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetCoarseSampleOrderNV);
    if(strcmp(pName, "vkGetShaderInfoAMD") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetShaderInfoAMD);
    if(strcmp(pName, "vkCmdSetViewportWScalingNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetViewportWScalingNV);
    if(strcmp(pName, "vkCompileDeferredNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCompileDeferredNV);
    if(strcmp(pName, "vkSetLocalDimmingAMD") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkSetLocalDimmingAMD);
    if(strcmp(pName, "vkCmdBeginConditionalRenderingEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdBeginConditionalRenderingEXT);
    if(strcmp(pName, "vkRegisterDeviceEventEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkRegisterDeviceEventEXT);
    if(strcmp(pName, "vkCmdDrawIndirectCountKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDrawIndirectCountKHR);
    if(strcmp(pName, "vkCmdEndConditionalRenderingEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdEndConditionalRenderingEXT);
    if(strcmp(pName, "vkGetImageDrmFormatModifierPropertiesEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetImageDrmFormatModifierPropertiesEXT);
    if(strcmp(pName, "vkDisplayPowerControlEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDisplayPowerControlEXT);
    if(strcmp(pName, "vkCreateDescriptorSetLayout") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateDescriptorSetLayout);
    if(strcmp(pName, "vkCreateDescriptorUpdateTemplateKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateDescriptorUpdateTemplateKHR);
    if(strcmp(pName, "vkGetDescriptorSetLayoutSupportKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDescriptorSetLayoutSupportKHR);
    if(strcmp(pName, "vkRegisterDisplayEventEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkRegisterDisplayEventEXT);
    if(strcmp(pName, "vkDestroySampler") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroySampler);
    if(strcmp(pName, "vkCmdDrawIndexedIndirectCountKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDrawIndexedIndirectCountKHR);
    if(strcmp(pName, "vkGetSwapchainCounterEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetSwapchainCounterEXT);
    if(strcmp(pName, "vkGetDescriptorSetLayoutSupport") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDescriptorSetLayoutSupport);
    if(strcmp(pName, "vkResetQueryPoolEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkResetQueryPoolEXT);
    if(strcmp(pName, "vkCmdSetLineStippleEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetLineStippleEXT);
    if(strcmp(pName, "vkCreateSamplerYcbcrConversionKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateSamplerYcbcrConversionKHR);
    if(strcmp(pName, "vkDestroyDescriptorSetLayout") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyDescriptorSetLayout);
    if(strcmp(pName, "vkGetPastPresentationTimingGOOGLE") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPastPresentationTimingGOOGLE);
    if(strcmp(pName, "vkCreateSwapchainKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateSwapchainKHR);
    if(strcmp(pName, "vkMergeValidationCachesEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkMergeValidationCachesEXT);
    if(strcmp(pName, "vkCreateValidationCacheEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateValidationCacheEXT);
    if(strcmp(pName, "vkUpdateDescriptorSetWithTemplateKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkUpdateDescriptorSetWithTemplateKHR);
    if(strcmp(pName, "vkDestroySamplerYcbcrConversionKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroySamplerYcbcrConversionKHR);
    if(strcmp(pName, "vkBindBufferMemory2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkBindBufferMemory2KHR);
    if(strcmp(pName, "vkGetValidationCacheDataEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetValidationCacheDataEXT);
    if(strcmp(pName, "vkGetDeviceQueue2") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDeviceQueue2);
    if(strcmp(pName, "vkDestroyDescriptorUpdateTemplateKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyDescriptorUpdateTemplateKHR);
    if(strcmp(pName, "vkBindImageMemory2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkBindImageMemory2KHR);
    if(strcmp(pName, "vkDestroyValidationCacheEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyValidationCacheEXT);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
    if(strcmp(pName, "vkGetAndroidHardwareBufferPropertiesANDROID") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetAndroidHardwareBufferPropertiesANDROID);
#endif // VK_USE_PLATFORM_ANDROID_KHR
    if(strcmp(pName, "vkGetRefreshCycleDurationGOOGLE") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetRefreshCycleDurationGOOGLE);
    if(strcmp(pName, "vkCmdWaitEvents") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdWaitEvents);
    if(strcmp(pName, "vkCmdBindDescriptorSets") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdBindDescriptorSets);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
    if(strcmp(pName, "vkGetMemoryAndroidHardwareBufferANDROID") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetMemoryAndroidHardwareBufferANDROID);
#endif // VK_USE_PLATFORM_ANDROID_KHR
    if(strcmp(pName, "vkCreateDescriptorPool") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateDescriptorPool);
    if(strcmp(pName, "vkCmdSetStencilReference") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetStencilReference);
    if(strcmp(pName, "vkUpdateDescriptorSetWithTemplate") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkUpdateDescriptorSetWithTemplate);
    if(strcmp(pName, "vkCmdBindIndexBuffer") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdBindIndexBuffer);
    if(strcmp(pName, "vkDestroyDescriptorUpdateTemplate") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyDescriptorUpdateTemplate);
    if(strcmp(pName, "vkCmdBindVertexBuffers") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdBindVertexBuffers);
    if(strcmp(pName, "vkFreeDescriptorSets") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkFreeDescriptorSets);
    if(strcmp(pName, "vkCmdPipelineBarrier") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdPipelineBarrier);
    if(strcmp(pName, "vkCmdDraw") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDraw);
    if(strcmp(pName, "vkDestroyDescriptorPool") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyDescriptorPool);
    if(strcmp(pName, "vkResetDescriptorPool") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkResetDescriptorPool);
    if(strcmp(pName, "vkTrimCommandPool") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkTrimCommandPool);
    if(strcmp(pName, "vkCmdDrawIndexed") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDrawIndexed);
    if(strcmp(pName, "vkAllocateDescriptorSets") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkAllocateDescriptorSets);
    if(strcmp(pName, "vkCmdDrawMeshTasksIndirectNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDrawMeshTasksIndirectNV);
    if(strcmp(pName, "vkCmdDrawMeshTasksIndirectCountNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDrawMeshTasksIndirectCountNV);
    if(strcmp(pName, "vkCmdDrawIndirect") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDrawIndirect);
    if(strcmp(pName, "vkCmdDrawMeshTasksNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDrawMeshTasksNV);
    if(strcmp(pName, "vkCmdDrawIndexedIndirect") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDrawIndexedIndirect);
    if(strcmp(pName, "vkCmdBeginQuery") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdBeginQuery);
    if(strcmp(pName, "vkUpdateDescriptorSets") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkUpdateDescriptorSets);
    if(strcmp(pName, "vkCmdDispatch") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDispatch);
    if(strcmp(pName, "vkCmdCopyBuffer") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdCopyBuffer);
    if(strcmp(pName, "vkCmdResetQueryPool") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdResetQueryPool);
    if(strcmp(pName, "vkCmdEndQuery") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdEndQuery);
    if(strcmp(pName, "vkCmdWriteTimestamp") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdWriteTimestamp);
    if(strcmp(pName, "vkCmdDispatchIndirect") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDispatchIndirect);
    if(strcmp(pName, "vkCmdCopyImage") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdCopyImage);
    if(strcmp(pName, "vkGetDeviceGroupPeerMemoryFeatures") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDeviceGroupPeerMemoryFeatures);
    if(strcmp(pName, "vkGetDeviceQueue") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDeviceQueue);
    if(strcmp(pName, "vkCmdClearColorImage") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdClearColorImage);
    if(strcmp(pName, "vkGetDeviceProcAddr") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDeviceProcAddr);
    if(strcmp(pName, "vkQueueSubmit") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkQueueSubmit);
    if(strcmp(pName, "vkGetRenderAreaGranularity") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetRenderAreaGranularity);
    if(strcmp(pName, "vkGetImageSparseMemoryRequirements2") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetImageSparseMemoryRequirements2);
    if(strcmp(pName, "vkCmdSetDeviceMask") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetDeviceMask);
    if(strcmp(pName, "vkCmdSetDiscardRectangleEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetDiscardRectangleEXT);
    if(strcmp(pName, "vkCmdClearDepthStencilImage") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdClearDepthStencilImage);
    if(strcmp(pName, "vkGetImageMemoryRequirements2") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetImageMemoryRequirements2);
    if(strcmp(pName, "vkCreateCommandPool") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateCommandPool);
    if(strcmp(pName, "vkCmdDispatchBase") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDispatchBase);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    if(strcmp(pName, "vkImportSemaphoreWin32HandleKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkImportSemaphoreWin32HandleKHR);
#endif // VK_USE_PLATFORM_WIN32_KHR
    if(strcmp(pName, "vkDestroyDevice") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyDevice);
    if(strcmp(pName, "vkGetBufferMemoryRequirements2") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetBufferMemoryRequirements2);
    if(strcmp(pName, "vkCmdEndRenderPass2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdEndRenderPass2KHR);
    if(strcmp(pName, "vkCmdClearAttachments") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdClearAttachments);
    if(strcmp(pName, "vkQueueBindSparse") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkQueueBindSparse);
    if(strcmp(pName, "vkFreeCommandBuffers") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkFreeCommandBuffers);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    if(strcmp(pName, "vkGetSemaphoreWin32HandleKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetSemaphoreWin32HandleKHR);
#endif // VK_USE_PLATFORM_WIN32_KHR
    if(strcmp(pName, "vkGetDeviceGroupPresentCapabilitiesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDeviceGroupPresentCapabilitiesKHR);
    if(strcmp(pName, "vkGetDeviceGroupPeerMemoryFeaturesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDeviceGroupPeerMemoryFeaturesKHR);
    if(strcmp(pName, "vkImportSemaphoreFdKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkImportSemaphoreFdKHR);
    if(strcmp(pName, "vkCmdSetDeviceMaskKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetDeviceMaskKHR);
    if(strcmp(pName, "vkDestroyCommandPool") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyCommandPool);
    if(strcmp(pName, "vkCreateSamplerYcbcrConversion") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateSamplerYcbcrConversion);
    if(strcmp(pName, "vkResetCommandPool") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkResetCommandPool);
    if(strcmp(pName, "vkGetDeviceGroupSurfacePresentModesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDeviceGroupSurfacePresentModesKHR);
    if(strcmp(pName, "vkCmdBeginRenderPass2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdBeginRenderPass2KHR);
    if(strcmp(pName, "vkCmdResolveImage") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdResolveImage);
    if(strcmp(pName, "vkEnumerateInstanceExtensionProperties") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkEnumerateInstanceExtensionProperties);
    if(strcmp(pName, "vkAllocateCommandBuffers") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkAllocateCommandBuffers);
    if(strcmp(pName, "vkCmdDispatchBaseKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdDispatchBaseKHR);
    if(strcmp(pName, "vkCmdResetEvent") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdResetEvent);
    if(strcmp(pName, "vkAcquireNextImage2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkAcquireNextImage2KHR);
    if(strcmp(pName, "vkDestroySamplerYcbcrConversion") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroySamplerYcbcrConversion);
    if(strcmp(pName, "vkCmdPushDescriptorSetKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdPushDescriptorSetKHR);
    if(strcmp(pName, "vkQueueWaitIdle") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkQueueWaitIdle);
    if(strcmp(pName, "vkCmdNextSubpass2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdNextSubpass2KHR);
    if(strcmp(pName, "vkGetSemaphoreFdKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetSemaphoreFdKHR);
    if(strcmp(pName, "vkEnumerateInstanceLayerProperties") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkEnumerateInstanceLayerProperties);
    if(strcmp(pName, "vkCmdSetViewportShadingRatePaletteNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetViewportShadingRatePaletteNV);
    if(strcmp(pName, "vkBeginCommandBuffer") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkBeginCommandBuffer);
    if(strcmp(pName, "vkDeviceWaitIdle") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDeviceWaitIdle);
    if(strcmp(pName, "vkCreateDescriptorUpdateTemplate") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateDescriptorUpdateTemplate);
    if(strcmp(pName, "vkGetSwapchainStatusKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetSwapchainStatusKHR);
    if(strcmp(pName, "vkAllocateMemory") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkAllocateMemory);
    if(strcmp(pName, "vkEndCommandBuffer") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkEndCommandBuffer);
    if(strcmp(pName, "vkCmdPushDescriptorSetWithTemplateKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdPushDescriptorSetWithTemplateKHR);
    if(strcmp(pName, "vkCmdSetEvent") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetEvent);
    if(strcmp(pName, "vkSetHdrMetadataEXT") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkSetHdrMetadataEXT);
    if(strcmp(pName, "vkCmdBindShadingRateImageNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdBindShadingRateImageNV);
    if(strcmp(pName, "vkCreateFence") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateFence);
    if(strcmp(pName, "vkCmdCopyQueryPoolResults") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdCopyQueryPoolResults);
    if(strcmp(pName, "vkCmdPushConstants") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdPushConstants);
    if(strcmp(pName, "vkCmdSetExclusiveScissorNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetExclusiveScissorNV);
    if(strcmp(pName, "vkCreateFramebuffer") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateFramebuffer);
    if(strcmp(pName, "vkBindImageMemory2") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkBindImageMemory2);
    if(strcmp(pName, "vkCmdNextSubpass") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdNextSubpass);
    if(strcmp(pName, "vkCmdSetCheckpointNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdSetCheckpointNV);
    if(strcmp(pName, "vkGetPipelineExecutableStatisticsKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPipelineExecutableStatisticsKHR);
    if(strcmp(pName, "vkGetPipelineExecutableInternalRepresentationsKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPipelineExecutableInternalRepresentationsKHR);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    if(strcmp(pName, "vkImportFenceWin32HandleKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkImportFenceWin32HandleKHR);
#endif // VK_USE_PLATFORM_WIN32_KHR
    if(strcmp(pName, "vkCmdBeginRenderPass") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdBeginRenderPass);
    if(strcmp(pName, "vkCreateRenderPass2KHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateRenderPass2KHR);
    if(strcmp(pName, "vkGetQueueCheckpointDataNV") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetQueueCheckpointDataNV);
    if(strcmp(pName, "vkGetFenceStatus") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetFenceStatus);
    if(strcmp(pName, "vkCreateRenderPass") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateRenderPass);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    if(strcmp(pName, "vkGetFenceWin32HandleKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetFenceWin32HandleKHR);
#endif // VK_USE_PLATFORM_WIN32_KHR
    if(strcmp(pName, "vkCmdEndRenderPass") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdEndRenderPass);
    if(strcmp(pName, "vkDestroyFramebuffer") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyFramebuffer);
    if(strcmp(pName, "vkImportFenceFdKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkImportFenceFdKHR);
    if(strcmp(pName, "vkCmdExecuteCommands") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCmdExecuteCommands);
    if(strcmp(pName, "vkDestroyFence") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyFence);
    if(strcmp(pName, "vkCreateShaderModule") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateShaderModule);
    if(strcmp(pName, "vkResetFences") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkResetFences);
    if(strcmp(pName, "vkDestroyImageView") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyImageView);
    if(strcmp(pName, "vkWaitForFences") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkWaitForFences);
    if(strcmp(pName, "vkCreateSemaphore") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateSemaphore);
    if(strcmp(pName, "vkDestroySemaphore") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroySemaphore);
    if(strcmp(pName, "vkDestroyShaderModule") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyShaderModule);
    if(strcmp(pName, "vkGetFenceFdKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetFenceFdKHR);
    if(strcmp(pName, "vkGetQueryPoolResults") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetQueryPoolResults);
    if(strcmp(pName, "vkUnmapMemory") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkUnmapMemory);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    if(strcmp(pName, "vkGetMemoryWin32HandlePropertiesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetMemoryWin32HandlePropertiesKHR);
#endif // VK_USE_PLATFORM_WIN32_KHR
    if(strcmp(pName, "vkMapMemory") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkMapMemory);
    if(strcmp(pName, "vkFreeMemory") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkFreeMemory);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    if(strcmp(pName, "vkGetMemoryWin32HandleKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetMemoryWin32HandleKHR);
#endif // VK_USE_PLATFORM_WIN32_KHR
    if(strcmp(pName, "vkCreateBuffer") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateBuffer);
    if(strcmp(pName, "vkBindBufferMemory") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkBindBufferMemory);
    if(strcmp(pName, "vkFlushMappedMemoryRanges") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkFlushMappedMemoryRanges);
    if(strcmp(pName, "vkGetMemoryFdPropertiesKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetMemoryFdPropertiesKHR);
    if(strcmp(pName, "vkInvalidateMappedMemoryRanges") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkInvalidateMappedMemoryRanges);
    if(strcmp(pName, "vkGetMemoryFdKHR") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetMemoryFdKHR);
    if(strcmp(pName, "vkGetDeviceMemoryCommitment") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetDeviceMemoryCommitment);
    if(strcmp(pName, "vkBindImageMemory") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkBindImageMemory);
    if(strcmp(pName, "vkCreateBufferView") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateBufferView);
    if(strcmp(pName, "vkGetImageSparseMemoryRequirements") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetImageSparseMemoryRequirements);
    if(strcmp(pName, "vkGetBufferMemoryRequirements") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetBufferMemoryRequirements);
    if(strcmp(pName, "vkGetImageMemoryRequirements") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetImageMemoryRequirements);
    if(strcmp(pName, "vkDestroyBuffer") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyBuffer);

    if(device_dispatch_table(device)->GetDeviceProcAddr == NULL)
        return NULL;
    return device_dispatch_table(device)->GetDeviceProcAddr(device, pName);
}
