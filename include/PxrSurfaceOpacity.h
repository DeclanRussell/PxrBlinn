#ifndef PxrSurfaceOpacity_h
#define PxrSurfaceOpacity_h
/*  $Date: 2015/06/17 $  $Revision: #2 $
# ------------------------------------------------------------------------------
#
# Copyright (c) 2014 Pixar Animation Studios. All rights reserved.
#
# The information in this file (the "Software") is provided for the
# exclusive use of the software licensees of Pixar.  Licensees have
# the right to incorporate the Software into other products for use
# by other authorized software licensees of Pixar, without fee.
# Except as expressly permitted herein, the Software may not be
# disclosed to third parties, copied or duplicated in any form, in
# whole or in part, without the prior written permission of
# Pixar Animation Studios.
#
# The copyright notices in the Software and this entire statement,
# including the above license grant, this restriction and the
# following disclaimer, must be included in all copies of the
# Software, in whole or in part, and all permitted derivative works of
# the Software, unless such copies or derivative works are solely
# in the form of machine-executable object code generated by a
# source language processor.
#
# PIXAR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
# ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
# SHALL PIXAR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES
# OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
# WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
# ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
# SOFTWARE.
#
# Pixar
# 1200 Park Ave
# Emeryville CA 94608
#
# ------------------------------------------------------------------------------
*/

#include "RixBxdf.h"
#include "RixShadingUtils.h"
#include <cassert>

// PxrOpacity: a simple class to deliver opacity & presence to the renderer.
//  We assume: 
//     * owner is Bxdf factory which optionally instantiates
//       us when opacity or presence services are required.
//       Owner should not instantiate us if the values convey
//       trivial opaque or present.
//     * GetPresence is invoked to when renderer wishes to skip
//       a more expensive shading computation.
//     * GetOpacity is invoked for shadows and must include presence.
//  
class PxrSurfaceOpacity : public RixOpacity 
{
public:
    PxrSurfaceOpacity(RixShadingContext const *sc, RixBxdfFactory *bx,
                   RtFloat const *presence, 
                   RtColorRGB const *transparency,
                   bool isUniform = false) :
                       RixOpacity(sc, bx),
                       m_presence(presence),
                       m_transparency(transparency),
                       m_isUniform(isUniform)
    {
    }

    virtual bool 
    GetPresence(RtFloat *result)
    {
        bool ret = false;
        if(m_presence)
        {
            RtInt nPts = shadingCtx->numPts;
            if (m_isUniform)
            {
                std::fill_n(result, nPts, *m_presence);
                ret = true; //signals nontrivally present
            }
            else 
            {
                for (int i = 0; i < nPts; ++i)
                {
                    result[i] = m_presence[i];
                    if(!ret && result[i] != 1.0f)
                        ret = true; //signals nontrivally present
                }
            }
        }
        return ret;
    }

    virtual bool 
    GetOpacity(RtColorRGB *result)
    {
       RtInt nPts = shadingCtx->numPts;
       bool ret = false;
       if(m_transparency)
       {
           for (int i = 0; i < nPts; ++i)
           {
               result[i] = RixConstants::k_OneRGB - m_transparency[i];
               result[i].ClampAlbedo();
           }
           ret = true;
       }
       return ret;
   }

private:
   RtFloat const *m_presence;
   RtColorRGB const *m_transparency;
   bool m_isUniform;
};

#endif