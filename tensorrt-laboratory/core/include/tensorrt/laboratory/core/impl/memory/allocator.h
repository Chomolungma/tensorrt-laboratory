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

// Allocator

template<typename MemoryType>
Allocator<MemoryType>::Allocator(mem_size_t size) : MemoryType()
{
    // Notes: Allocate can not set member values of a dervived class if called
    // in MemoryType's constructor, i.e. for SystemV, Allocate sets the m_ShmID
    // member variable.  This segfaults if done in MemoryType's constructor
    this->SetDataAndSize(this->Allocate(size), size);
    this->m_Deleter = this->Free();
    this->m_Handle.ctx = this->DeviceContext();

    DLOG(INFO) << "Allocator size_ctor: " << *this;

}

template<typename MemoryType>
Allocator<MemoryType>::Allocator(Allocator<MemoryType>&& other) noexcept : MemoryType(std::move(other))
{
    DLOG(INFO) << "Allocator mv_ctor: " << *this;
}

/*
template<typename MemoryType>
Allocator<MemoryType>& Allocator<MemoryType>::operator=(Allocator<MemoryType>&& other) noexcept
{
    MemoryType::operator=(std::move(other));
    return *this;
}
*/

template<typename MemoryType>
Allocator<MemoryType>::~Allocator()
{
    DLOG(INFO) << "Allocator dtor: " << *this;
}

} // namespace trtlab