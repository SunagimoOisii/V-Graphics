#pragma once
#include <memory>

template<typename T>
class GPUResourceBase
{
public:
    // ƒRƒs[‚Í‹Ö~
    GPUResourceBase(const GPUResourceBase&) = delete;
    GPUResourceBase& operator=(const GPUResourceBase&) = delete;

    virtual ~GPUResourceBase() = default;

    static std::shared_ptr<T> Create() { return std::shared_ptr<T>(new T()); }
protected:
    GPUResourceBase() = default;    // CreateŠÖ”‚Åì¬‚ğ§ŒÀ‚·‚é‚½‚ß
};