/******************************************************************************\
Copyright (c) 2005-2018, Intel Corporation
All rights reserved.
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
This sample was distributed or derived from the Intel's Media Samples package.
The original version of this sample may be obtained from https://software.intel.com/en-us/intel-media-server-studio
or https://software.intel.com/en-us/media-client-solutions-support.
\**********************************************************************************/

#pragma once

#include "common_utils.h"
#include <initguid.h>
#include <d3d9.h>
#include <dxva2api.h>
#include <dxva.h>
#include <windows.h>

#define VIDEO_MAIN_FORMAT D3DFMT_YUY2

class IGFXS3DControl;

/** Direct3D 9 device implementation.
@note Can be initialized for only 1 or two 2 views. Handle to
MFX_HANDLE_GFXS3DCONTROL must be set prior if initializing for 2 views.

@note Device always set D3DPRESENT_PARAMETERS::Windowed to TRUE.
*/
    template <class T>
    class safe_array
    {
    public:
        safe_array(T *ptr = 0):m_ptr(ptr)
        { // construct from object pointer
        };
        ~safe_array()
        {
            reset(0);
        }
        T* get()
        { // return wrapped pointer
            return m_ptr;
        }
        T* release()
        { // return wrapped pointer and give up ownership
            T* ptr = m_ptr;
            m_ptr = 0;
            return ptr;
        }
        void reset(T* ptr)
        { // destroy designated object and store new pointer
            if (m_ptr)
            {
                delete[] m_ptr;
            }
            m_ptr = ptr;
        }
    protected:
        T* m_ptr; // the wrapped object pointer
    };

mfxStatus dx9_simple_alloc(mfxHDL pthis, mfxFrameAllocRequest* request, mfxFrameAllocResponse* response);
mfxStatus dx9_simple_lock(mfxHDL pthis, mfxMemId mid, mfxFrameData* ptr);
mfxStatus dx9_simple_unlock(mfxHDL pthis, mfxMemId mid, mfxFrameData* ptr);
mfxStatus dx9_simple_gethdl(mfxHDL pthis, mfxMemId mid, mfxHDL* handle);
mfxStatus dx9_simple_free(mfxHDL pthis, mfxFrameAllocResponse* response);

mfxStatus DX9_CreateHWDevice(mfxSession session, mfxHDL* deviceHandle, HWND hWnd, bool bCreateSharedHandles);
void DX9_CleanupHWDevice();
