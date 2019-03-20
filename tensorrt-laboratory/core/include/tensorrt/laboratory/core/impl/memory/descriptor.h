/* Copyright (c) 2018-2019, NVIDIA CORPORATION. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of NVIDIA CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include <glog/logging.h>

namespace trtlab {

// Descriptor

template<typename MemoryType>
Descriptor<MemoryType>::Descriptor(void* ptr, mem_size_t size, std::function<void()> deleter,
                                   const char* desc)
    : MemoryType(ptr, size), m_Deleter(deleter), m_Desc(desc)
{
    DLOG(INFO) << "Descriptor ptr_size_ctor: " << *this;
}

template<typename MemoryType>
Descriptor<MemoryType>::Descriptor(const DLTensor& dltensor, std::function<void()> deleter,
                                   const char* desc)
    : MemoryType(dltensor), m_Deleter(deleter), m_Desc(desc)
{
    DLOG(INFO) << "Descriptor dltensor_ctor: " << *this;
}

template<typename MemoryType>
Descriptor<MemoryType>::Descriptor(void* ptr, mem_size_t size, const MemoryType& properties,
                                   std::function<void()> deleter, const char* desc)
    : MemoryType(ptr, size, properties), m_Deleter(deleter), m_Desc(desc)
{
    DLOG(INFO) << "Descriptor ptr_size_props_ctor: " << *this;
}

template<typename MemoryType>
Descriptor<MemoryType>::Descriptor(MemoryType&& other, std::function<void()> deleter,
                                   const char* desc)
    : MemoryType(std::move(other)), m_Deleter(deleter), m_Desc(desc)
{
    DLOG(INFO) << "Descriptor mem_mv_ctor: " << *this;
}

template<class MemoryType>
Descriptor<MemoryType>::Descriptor(std::shared_ptr<MemoryType> shared, const char* desc)
    : MemoryType(shared->Data(), shared->Size(), *shared),
      m_Deleter([shared]() mutable { shared.reset(); }),
      m_Desc(desc)
{
    DLOG(INFO) << "Descriptor shared_ptr_ctor: " << *this;
}

template<class MemoryType>
Descriptor<MemoryType>::Descriptor(Descriptor<MemoryType>&& other) noexcept
    : MemoryType(std::move(other))
{
    DLOG(INFO) << "Descriptor mv_ctor: " << *this;
}

/*
template<class MemoryType>
Descriptor<MemoryType>& Descriptor<MemoryType>::operator=(Descriptor<MemoryType>&& other) noexcept
{
    MemoryType::operator=(std::move(other));
    return *this;
}
*/

template<class MemoryType>
Descriptor<MemoryType>::~Descriptor()
{
    if(m_Deleter)
    {
        DLOG(INFO) << "Descriptor dtor: " << *this;
        m_Deleter();
    }
}

template<class MemoryType>
const char* Descriptor<MemoryType>::TypeName() const
{
    return m_Desc.c_str();
}

} // namespace trtlab