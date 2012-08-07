// This code is in the public domain -- Ignacio Casta�o <castano@gmail.com>

#pragma once
#ifndef NV_CORE_HASH_H
#define NV_CORE_HASH_H

#include "nvcore.h"

namespace nv
{
    inline uint sdbmHash(const void * data_in, uint size, uint h = 5381)
    {
        const uint8 * data = (const uint8 *) data_in;
        uint i = 0;
        while (i < size) {
            h = (h << 16) + (h << 6) - h + (uint) data[i++];
        }
        return h;
    }

    // Note that this hash does not handle NaN properly.
    inline uint sdbmFloatHash(const float * f, uint count, uint h = 5381)
    {
        for (uint i = 0; i < count; i++) {
            //nvDebugCheck(nv::isFinite(*f));
            union { float f; uint32 i; } x = { f[i] };
            if (x.i == 0x80000000) x.i = 0;
            h = sdbmHash(&x, 4, h);
        }
        return h;
    }


    // Some hash functors:
    template <typename Key> struct Hash 
    {
        uint operator()(const Key & k) const {
            return sdbmHash(&k, sizeof(Key));
        }
    };
    /*template <> struct Hash<int>
    {
        uint operator()(int x) const { return x; }
    };
    template <> struct Hash<uint>
    {
        uint operator()(uint x) const { return x; }
    };*/
    template <> struct Hash<float>
    {
        uint operator()(float f) const {
            return sdbmFloatHash(&f, 1);
        }
    };

    template <typename Key> struct Equal
    {
        bool operator()(const Key & k0, const Key & k1) const {
            return k0 == k1;
        }
    };

} // nv namespace

#endif // NV_CORE_HASH_H
