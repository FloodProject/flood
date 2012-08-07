// Copyright (c) 2009-2011 Ignacio Castano <castano@gmail.com>
// 
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#include "CubeSurface.h"
#include "Surface.h"

#include "nvimage/DirectDrawSurface.h"

#include "nvmath/Vector.inl"

#include "nvcore/Array.inl"
#include "nvcore/StrLib.h"

using namespace nv;
using namespace nvtt;



// Solid angle of an axis aligned quad from (0,0,1) to (x,y,1)
// See: http://www.fizzmoll11.com/thesis/ for a derivation of this formula.
static float areaElement(float x, float y) {
    return atan2(x*y, sqrtf(x*x + y*y + 1));
}

// Solid angle of a hemicube texel.
static float solidAngleTerm(uint x, uint y, float inverseEdgeLength) {
    // Transform x,y to [-1, 1] range, offset by 0.5 to point to texel center.
    float u = (float(x) + 0.5f) * (2 * inverseEdgeLength) - 1.0f;
    float v = (float(y) + 0.5f) * (2 * inverseEdgeLength) - 1.0f;
    nvDebugCheck(u >= -1.0f && u <= 1.0f);
    nvDebugCheck(v >= -1.0f && v <= 1.0f);

#if 1
    // Exact solid angle:
    float x0 = u - inverseEdgeLength;
    float y0 = v - inverseEdgeLength;
    float x1 = u + inverseEdgeLength;
    float y1 = v + inverseEdgeLength;
    float solidAngle = areaElement(x0, y0) - areaElement(x0, y1) - areaElement(x1, y0) + areaElement(x1, y1);
    nvDebugCheck(solidAngle > 0.0f);

    return solidAngle;
#else
    // This formula is equivalent, but not as precise.
    float pixel_area = nv::square(2.0f * inverseEdgeLength);
    float dist_square = 1.0f + nv::square(u) + nv::square(v);
    float cos_theta = 1.0f / sqrt(dist_square);
    float cos_theta_d2 = cos_theta / dist_square; // Funny this is just 1/dist^3 or cos(tetha)^3

    return pixel_area * cos_theta_d2;
#endif
}


static Vector3 texelDirection(uint face, uint x, uint y, int edgeLength, EdgeFixup fixupMethod)
{
    float u, v;
    if (fixupMethod == EdgeFixup_Stretch) {
        // Transform x,y to [-1, 1] range, match up edges exactly.
        u = float(x) * 2.0f / (edgeLength - 1) - 1.0f;
        v = float(y) * 2.0f / (edgeLength - 1) - 1.0f;
    }
    else {
        // Transform x,y to [-1, 1] range, offset by 0.5 to point to texel center.
        u = (float(x) + 0.5f) * (2.0f / edgeLength) - 1.0f;
        v = (float(y) + 0.5f) * (2.0f / edgeLength) - 1.0f;
    }

    if (fixupMethod == EdgeFixup_Warp) {
        // Warp texel centers in the proximity of the edges.
        float a = powf(float(edgeLength), 2.0f) / powf(float(edgeLength - 1), 3.0f);
        u = a * powf(u, 3) + u;
        v = a * powf(v, 3) + v;
    }

    nvDebugCheck(u >= -1.0f && u <= 1.0f);
    nvDebugCheck(v >= -1.0f && v <= 1.0f);

    Vector3 n;

    if (face == 0) {
        n.x = 1;
        n.y = -v;
        n.z = -u;
    }
    if (face == 1) {
        n.x = -1;
        n.y = -v;
        n.z = u;
    }

    if (face == 2) {
        n.x = u;
        n.y = 1;
        n.z = v;
    }
    if (face == 3) {
        n.x = u;
        n.y = -1;
        n.z = -v;
    }

    if (face == 4) {
        n.x = u;
        n.y = -v;
        n.z = 1;
    }
    if (face == 5) {
        n.x = -u;
        n.y = -v;
        n.z = -1;
    }

    return normalizeFast(n);
}


TexelTable::TexelTable(uint edgeLength) : size(edgeLength) {

    uint hsize = size/2;

    // Allocate a small solid angle table that takes into account cube map symmetry.
    solidAngleArray.resize(hsize * hsize);

    for (uint y = 0; y < hsize; y++) {
        for (uint x = 0; x < hsize; x++) {
            solidAngleArray[y * hsize + x] = solidAngleTerm(hsize+x, hsize+y, 1.0f/edgeLength);
        }
    }


    directionArray.resize(size*size*6);

    for (uint f = 0; f < 6; f++) {
        for (uint y = 0; y < size; y++) {
            for (uint x = 0; x < size; x++) {
                directionArray[(f * size + y) * size + x] = texelDirection(f, x, y, edgeLength, EdgeFixup_None);
            }
        }
    }
}

const Vector3 & TexelTable::direction(uint f, uint x, uint y) const {
    nvDebugCheck(f < 6 && x < size && y < size);
    return directionArray[(f * size + y) * size + x];
}

float TexelTable::solidAngle(uint f, uint x, uint y) const {
    uint hsize = size/2;
    if (x >= hsize) x -= hsize;
    else if (x < hsize) x = hsize - x - 1;
    if (y >= hsize) y -= hsize;
    else if (y < hsize) y = hsize - y - 1;

    return solidAngleArray[y * hsize + x];
}


static const Vector3 faceNormals[6] = {
    Vector3(1, 0, 0),
    Vector3(-1, 0, 0),
    Vector3(0, 1, 0),
    Vector3(0, -1, 0),
    Vector3(0, 0, 1),
    Vector3(0, 0, -1),
};

static const Vector3 faceU[6] = {
    Vector3(0, 0, -1),
    Vector3(0, 0, 1),
    Vector3(1, 0, 0),
    Vector3(1, 0, 0),
    Vector3(1, 0, 0),
    Vector3(-1, 0, 0),
};

static const Vector3 faceV[6] = {
    Vector3(0, -1, 0),
    Vector3(0, -1, 0),
    Vector3(0, 0, 1),
    Vector3(0, 0, -1),
    Vector3(0, -1, 0),
    Vector3(0, -1, 0),
};


static Vector2 toPolar(Vector3::Arg v) {
    Vector2 p;
    p.x = atan2(v.x, v.y);  // theta
    p.y = acosf(v.z);       // phi
    return p;
}

static Vector2 toPlane(float theta, float phi) {
    float x = sin(phi) * cos(theta);
    float y = sin(phi) * sin(theta);
    float z = cos(phi);

    Vector2 p;
    p.x = x / fabs(z);
    p.y = y / fabs(z);
    //p.x = tan(phi) * cos(theta);
    //p.y = tan(phi) * sin(theta);

    return p;
}

static Vector2 toPlane(Vector3::Arg v) {
    Vector2 p;
    p.x = v.x / fabs(v.z);
    p.y = v.y / fabs(v.z);
    return p;
}





CubeSurface::CubeSurface() : m(new CubeSurface::Private())
{
    m->addRef();
}

CubeSurface::CubeSurface(const CubeSurface & cube) : m(cube.m)
{
    if (m != NULL) m->addRef();
}

CubeSurface::~CubeSurface()
{
    if (m != NULL) m->release();
    m = NULL;
}

void CubeSurface::operator=(const CubeSurface & cube)
{
    if (cube.m != NULL) cube.m->addRef();
    if (m != NULL) m->release();
    m = cube.m;
}

void CubeSurface::detach()
{
    if (m->refCount() > 1)
    {
        m->release();
        m = new CubeSurface::Private(*m);
        m->addRef();
        nvDebugCheck(m->refCount() == 1);
    }
}



bool CubeSurface::isNull() const
{
    return m->edgeLength == 0;
}

int CubeSurface::edgeLength() const
{
    return m->edgeLength;
}

int CubeSurface::countMipmaps() const
{
    return nv::countMipmaps(m->edgeLength);
}

Surface & CubeSurface::face(int f)
{
    nvDebugCheck(f >= 0 && f < 6);
    return m->face[f];
}

const Surface & CubeSurface::face(int f) const
{
    nvDebugCheck(f >= 0 && f < 6);
    return m->face[f];
}


bool CubeSurface::load(const char * fileName, int mipmap)
{
    if (strEqual(Path::extension(fileName), ".dds")) {
        nv::DirectDrawSurface dds(fileName);

        if (!dds.isValid()/* || !dds.isSupported()*/) {
            return false;
        }

        if (!dds.isTextureCube()) {
            return false;
        }

        // Make sure it's a valid cube.
        if (dds.header.width != dds.header.height) return false;
        //if ((dds.header.caps.caps2 & DDSCAPS2_CUBEMAP_ALL_FACES) != DDSCAPS2_CUBEMAP_ALL_FACES) return false;

        if (mipmap < 0) {
            mipmap = dds.mipmapCount() - 1 - mipmap;
        }
        if (mipmap < 0 || mipmap > toI32(dds.mipmapCount())) return false;
        

        nvtt::InputFormat inputFormat = nvtt::InputFormat_RGBA_16F;

        if (dds.header.hasDX10Header()) {
            if (dds.header.header10.dxgiFormat == DXGI_FORMAT_R16G16B16A16_FLOAT) inputFormat = nvtt::InputFormat_RGBA_16F;
            else if (dds.header.header10.dxgiFormat == DXGI_FORMAT_R32G32B32A32_FLOAT) inputFormat = nvtt::InputFormat_RGBA_32F;
            else return false;
        }
        else {
            if ((dds.header.pf.flags & DDPF_FOURCC) != 0) {
                if (dds.header.pf.fourcc == D3DFMT_A16B16G16R16F) inputFormat = nvtt::InputFormat_RGBA_16F;
                else if (dds.header.pf.fourcc == D3DFMT_A32B32G32R32F) inputFormat = nvtt::InputFormat_RGBA_32F;
                else return false;
            }
            else {
                if (dds.header.pf.bitcount == 32 /*&& ...*/) inputFormat = nvtt::InputFormat_BGRA_8UB;
                else return false;  // @@ Do pixel format conversions!
            }
        }
        
        uint edgeLength = dds.surfaceWidth(mipmap);
        uint size = dds.surfaceSize(mipmap);

        void * data = malloc(size);

        for (int f = 0; f < 6; f++) {
            dds.readSurface(f, mipmap, data, size);
            m->face[f].setImage(inputFormat, edgeLength, edgeLength, 1, data);
        }

        m->edgeLength = edgeLength;

        free(data);

        return true;
    }

    return false;
}

bool CubeSurface::save(const char * fileName) const
{
    // @@ TODO
    return false;
}


void CubeSurface::fold(const Surface & tex, CubeLayout layout)
{
    // @@ TODO
}

Surface CubeSurface::unfold(CubeLayout layout) const
{
    // @@ TODO
    return Surface();
}


float CubeSurface::average(int channel) const
{
    const uint edgeLength = m->edgeLength;
    m->allocateTexelTable();

    float total = 0.0f;
    float sum = 0.0f;

    for (int f = 0; f < 6; f++) {
        float * c = m->face[f].m->image->channel(channel);

         for (uint y = 0; y < edgeLength; y++) {
             for (uint x = 0; x < edgeLength; x++) {
                float solidAngle = m->texelTable->solidAngle(f, x, y);

                total += solidAngle;
                sum += c[y * edgeLength + x] * solidAngle;
            }
        }
    }

    return sum / total;
}

void CubeSurface::range(int channel, float * minimum_ptr, float * maximum_ptr) const
{
    const uint edgeLength = m->edgeLength;
    m->allocateTexelTable();

    float minimum = NV_FLOAT_MAX;
    float maximum = 0.0f;

    for (int f = 0; f < 6; f++) {
        float * c = m->face[f].m->image->channel(channel);

         for (uint y = 0; y < edgeLength; y++) {
             for (uint x = 0; x < edgeLength; x++) {

                 minimum = nv::min(minimum, c[y * edgeLength + x]);
                 maximum = nv::max(maximum, c[y * edgeLength + x]);
            }
        }
    }

    *minimum_ptr = minimum;
    *maximum_ptr = maximum;
}



#include "nvmath/SphericalHarmonic.h"

CubeSurface CubeSurface::irradianceFilter(int size, EdgeFixup fixupMethod) const
{
    m->allocateTexelTable();

    // Transform this cube to spherical harmonic basis
    Sh2 sh;

    // For each texel of the input cube.
    const uint edgeLength = m->edgeLength;
    for (uint f = 0; f < 6; f++) {
        for (uint y = 0; y < edgeLength; y++) {
            for (uint x = 0; x < edgeLength; x++) {

                Vector3 dir = m->texelTable->direction(f, x, y);
                float solidAngle = m->texelTable->solidAngle(f, x, y);

                Sh2 shDir;
                shDir.eval(dir);

                sh.addScaled(sh, solidAngle);
            }
        }
    }


    // Evaluate spherical harmonic for each output texel.
    CubeSurface output;
    output.m->allocate(size);




    // @@ TODO
    return CubeSurface();
}


// Warp uv coordinate from [-1, 1] to
/*float warp(float u, int size) {

}*/




// We want to find the alpha such that:
// cos(alpha)^cosinePower = epsilon
// That's: acos(epsilon^(1/cosinePower))

// We can cull texels in two different ways:
// - culling faces that do not touch the cone.
// - computing one rectangle per face, find intersection between cone and face.
// -

// Other speedups:
// - parallelize. Done.
// - use ISPC?

// Convolve filter against this cube.
Vector3 CubeSurface::Private::applyCosinePowerFilter(const Vector3 & filterDir, float coneAngle, float cosinePower)
{
    const float cosineConeAngle = cos(coneAngle);
    nvDebugCheck(cosineConeAngle >= 0);

    Vector3 color(0);
    float sum = 0;

    // For each texel of the input cube.
    for (uint f = 0; f < 6; f++) {

        // Test face cone agains filter cone.
        float cosineFaceAngle = dot(filterDir, faceNormals[f]);
        float faceAngle = acosf(cosineFaceAngle);

        if (faceAngle > coneAngle + atanf(sqrtf(2))) {
            // Skip face.
            continue;
        }

        // @@ We could do a less conservative test and test the face frustum against the cone...
        // Or maybe easier: the face quad against the cone.

        // Compute bounding box of cone intersection against face.
        // The intersection of the cone with the face is an elipse, we want the extents of that elipse.
        // @@ Hmm... we could even rasterize an elipse! Sounds like FUN!

        const int L = toI32(edgeLength-1);
        int x0 = 0, x1 = L;
        int y0 = 0, y1 = L;

        // @@ Ugh. This is wrong, or only right when filterDir is aligned to one axis.
        if (false) {
            // uv coordinates corresponding to filterDir.
            //float u = dot(filterDir, faceU[f]) / cosineFaceAngle;
            //float v = dot(filterDir, faceV[f]) / cosineFaceAngle;

            // Angular coordinates corresponding to filterDir with respect to faceNormal.
            float atu = atan2(dot(filterDir, faceU[f]), cosineFaceAngle);
            float atv = atan2(dot(filterDir, faceV[f]), cosineFaceAngle);

            // Expand angles and project back to the face plane.
            float u0 = tan(clamp(atu - coneAngle, -PI/4, PI/4));
            float v0 = tan(clamp(atv - coneAngle, -PI/4, PI/4));
            float u1 = tan(clamp(atu + coneAngle, -PI/4, PI/4));
            float v1 = tan(clamp(atv + coneAngle, -PI/4, PI/4));
            nvDebugCheck(u0 >= -1 && u0 <= 1);
            nvDebugCheck(v0 >= -1 && v0 <= 1);
            nvDebugCheck(u1 >= -1 && u1 <= 1);
            nvDebugCheck(v1 >= -1 && v1 <= 1);

            // Expand uv coordinates from [-1,1] to [0, edgeLength)
            u0 = (u0 + 1) * edgeLength * 0.5f - 0.5f;
            v0 = (v0 + 1) * edgeLength * 0.5f - 0.5f;
            u1 = (u1 + 1) * edgeLength * 0.5f - 0.5f;
            v1 = (v1 + 1) * edgeLength * 0.5f - 0.5f;
            nvDebugCheck(u0 >= -0.5f && u0 <= edgeLength - 0.5f);
            nvDebugCheck(v0 >= -0.5f && v0 <= edgeLength - 0.5f);
            nvDebugCheck(u1 >= -0.5f && u1 <= edgeLength - 0.5f);
            nvDebugCheck(v1 >= -0.5f && v1 <= edgeLength - 0.5f);

            x0 = clamp(ifloor(u0), 0, L);
            y0 = clamp(ifloor(v0), 0, L);
            x1 = clamp(iceil(u1), 0, L);
            y1 = clamp(iceil(v1), 0, L);

            nvDebugCheck(x1 >= x0);
            nvDebugCheck(y1 >= y0);
        }

        // This is elegant and all that, but the problem is that the projection is not always an ellipse, but often a parabola.
        // A parabola has infinite bounds, so this approach is not very practical. Ugh.
        if (false) {
            //nvCheck(cosineFaceAngle >= 0.0f); @@ Not true for wide angles.

            // Focal point in cartessian coordinates:
            Vector3 F = Vector3(dot(faceU[f], filterDir), dot(faceV[f], filterDir), cosineFaceAngle);

            // Focal point in polar coordinates:
            Vector2 Fp = toPolar(F);
            nvCheck(Fp.y >= 0.0f);  // top
            //nvCheck(Fp.y <= PI/2);  // horizon

            // If this is an ellipse:
            if (Fp.y + coneAngle < PI/2) {
                nvCheck(Fp.y - coneAngle > -PI/2);

                // Major axis endpoints:
                Vector2 Fa1 = toPlane(Fp.x, Fp.y - cosineFaceAngle);  // near endpoint.
                Vector2 Fa2 = toPlane(Fp.x, Fp.y + cosineFaceAngle);  // far endpoint.
                nvCheck(length(Fa1) <= length(Fa2));

                // Ellipse center:
                Vector2 Fc = (Fa1 + Fa2) * 0.5f;

                // Major radius:
                float a = 0.5f * length(Fa1 - Fa2);

                // Focal point:
                Vector2 F1 = toPlane(Fp.x, Fp.y);

                // If we project Fa1, Fa2, Fc, F1 onto the filter direction, then:
                float da1 = dot(Fa1, F.xy()) / fabs(cosineFaceAngle);
                float d1 = dot(F1, F.xy()) / fabs(cosineFaceAngle);
                float dc = dot(Fc, F.xy()) / fabs(cosineFaceAngle);
                float da2 = dot(Fa2, F.xy()) / fabs(cosineFaceAngle);
                //nvDebug("%f <= %f <= %f <= %f   (%d: %f %f | %f %f)\n", da1, d1, dc, da2, f, F.x, F.y, Fp.y - coneAngle, Fp.y + coneAngle);
                //nvCheck(da1 <= d1 && d1 <= dc && dc <= da2);

                // Translate focal point relative to center:
                F1 -= Fc;

                // Focal distance:
                //float f = length(F1);  // @@ Overriding f!

                // Minor radius:
                //float b = sqrtf(a*a - f*f);

                // Second order quadric coefficients:
                float A = a*a - F1.x * F1.x;
                nvCheck(A >= 0);

                float B = a*a - F1.y * F1.y;
                nvCheck(B >= 0);

                // Floating point bounds:
                float u0 = clamp(Fc.x - sqrtf(B), -1.0f, 1.0f);
                float u1 = clamp(Fc.x + sqrtf(B), -1.0f, 1.0f);
                float v0 = clamp(Fc.y - sqrtf(A), -1.0f, 1.0f);
                float v1 = clamp(Fc.y + sqrtf(A), -1.0f, 1.0f);

                // Expand uv coordinates from [-1,1] to [0, edgeLength)
                u0 = (u0 + 1) * edgeLength * 0.5f - 0.5f;
                v0 = (v0 + 1) * edgeLength * 0.5f - 0.5f;
                u1 = (u1 + 1) * edgeLength * 0.5f - 0.5f;
                v1 = (v1 + 1) * edgeLength * 0.5f - 0.5f;
                //nvDebugCheck(u0 >= -0.5f && u0 <= edgeLength - 0.5f);
                //nvDebugCheck(v0 >= -0.5f && v0 <= edgeLength - 0.5f);
                //nvDebugCheck(u1 >= -0.5f && u1 <= edgeLength - 0.5f);
                //nvDebugCheck(v1 >= -0.5f && v1 <= edgeLength - 0.5f);

                x0 = clamp(ifloor(u0), 0, L);
                y0 = clamp(ifloor(v0), 0, L);
                x1 = clamp(iceil(u1), 0, L);
                y1 = clamp(iceil(v1), 0, L);

                nvDebugCheck(x1 >= x0);
                nvDebugCheck(y1 >= y0);
            }

            // @@ What to do with parabolas?
        }


        if (x1 == x0 || y1 == y0) {
            // Skip this face.
            continue;
        }


        const Surface & inputFace = face[f];
        const FloatImage * inputImage = inputFace.m->image;

        for (int y = y0; y <= y1; y++) {
            bool inside = false;
            for (int x = x0; x <= x1; x++) {

                Vector3 dir = texelTable->direction(f, x, y);
                float cosineAngle = dot(dir, filterDir);

                if (cosineAngle > cosineConeAngle) {
                    float solidAngle = texelTable->solidAngle(f, x, y);
                    float scale = powf(saturate(cosineAngle), cosinePower);
                    float contribution = solidAngle * scale;

                    sum += contribution;
                    color.x += contribution * inputImage->pixel(0, x, y, 0);
                    color.y += contribution * inputImage->pixel(1, x, y, 0);
                    color.z += contribution * inputImage->pixel(2, x, y, 0);

                    inside = true;
                }
                else if (inside) {
                    // Filter scale is monotonic, if we have been inside once and we just exit, then we can skip the rest of the row.
                    // We could do the same thing for the columns and skip entire rows.
                    break;
                }
            }
        }
    }

    color *= (1.0f / sum);

    return color;
}

#include "nvthread/ParallelFor.h"

struct ApplyCosinePowerFilterContext {
    CubeSurface::Private * inputCube;
    CubeSurface::Private * filteredCube;
    float coneAngle;
    float cosinePower;
    EdgeFixup fixupMethod;
};

void ApplyCosinePowerFilterTask(void * context, int id)
{
    ApplyCosinePowerFilterContext * ctx = (ApplyCosinePowerFilterContext *)context;

    int size = ctx->filteredCube->edgeLength;

    int f = id / (size * size);
    int idx = id % (size * size);
    int y = idx / size;
    int x = idx % size;

    nvtt::Surface & filteredFace = ctx->filteredCube->face[f];
    FloatImage * filteredImage = filteredFace.m->image;

    const Vector3 filterDir = texelDirection(f, x, y, size, ctx->fixupMethod);

    // Convolve filter against cube.
    Vector3 color = ctx->inputCube->applyCosinePowerFilter(filterDir, ctx->coneAngle, ctx->cosinePower);

    filteredImage->pixel(0, idx) = color.x;
    filteredImage->pixel(1, idx) = color.y;
    filteredImage->pixel(2, idx) = color.z;
}


CubeSurface CubeSurface::cosinePowerFilter(int size, float cosinePower, EdgeFixup fixupMethod) const
{
    const uint edgeLength = m->edgeLength;

    // Allocate output cube.
    CubeSurface filteredCube;
    filteredCube.m->allocate(size);

    // Texel table is stored along with the surface so that it's compute only once.
    m->allocateTexelTable();

    const float threshold = 0.001f;
    const float coneAngle = acosf(powf(threshold, 1.0f/cosinePower));


    // For each texel of the output cube.
    /*for (uint f = 0; f < 6; f++) {
        nvtt::Surface filteredFace = filteredCube.m->face[f];
        FloatImage * filteredImage = filteredFace.m->image;

        for (uint y = 0; y < uint(size); y++) {
            for (uint x = 0; x < uint(size); x++) {

                const Vector3 filterDir = texelDirection(f, x, y, size, fixupMethod);

                // Convolve filter against cube.
                Vector3 color = m->applyCosinePowerFilter(filterDir, coneAngle, cosinePower);

                filteredImage->pixel(0, x, y, 0) = color.x;
                filteredImage->pixel(1, x, y, 0) = color.y;
                filteredImage->pixel(2, x, y, 0) = color.z;
            }
        }
    }*/

    ApplyCosinePowerFilterContext context;
    context.inputCube = m;
    context.filteredCube = filteredCube.m;
    context.coneAngle = coneAngle;
    context.cosinePower = cosinePower;
    context.fixupMethod = fixupMethod;

    nv::ParallelFor parallelFor(ApplyCosinePowerFilterTask, &context);
    parallelFor.run(6 * size * size);

    // @@ Implement edge averaging.
    if (fixupMethod == EdgeFixup_Average) {
        for (uint f = 0; f < 6; f++) {
            nvtt::Surface filteredFace = filteredCube.m->face[f];
            FloatImage * filteredImage = filteredFace.m->image;

            // For each component.
            for (uint c = 0; c < 3; c++) {
                // @@ For each corner, sample the two adjacent faces.
                filteredImage->pixel(c, 0, 0, 0);
                filteredImage->pixel(c, size-1, 0, 0);
                filteredImage->pixel(c, 0, size-1, 0);
                filteredImage->pixel(c, size-1, size-1, 0);

                // @@ For each edge, sample the adjacent face.

            }
        }
    }

    return filteredCube;
}


void CubeSurface::toLinear(float gamma)
{
    if (isNull()) return;

    detach();

    for (int i = 0; i < 6; i++) {
        m->face[i].toLinear(gamma);
    }
}

void CubeSurface::toGamma(float gamma)
{
    if (isNull()) return;

    detach();

    for (int i = 0; i < 6; i++) {
        m->face[i].toGamma(gamma);
    }
}


#if 0
// @@ Provide solar azimuth.
#include "ArHoseSkyModel.h"
void CubeSurface::sky(float turbidity, float albedo[3], float solarElevation) {

    ArHosekSkyModelState * skymodel_state[3];

    for (int i = 0; i < num_channels; i++) {
        skymodel_state[i] = arhosekskymodelstate_alloc_init(turbidity, albedo[i], solarElevation);
    }

    // 700 nm (red), 546.1 nm (green) and 435.8 nm (blue).
    float channel_center[3] = {
        700,    // Red 620�740,
        546.1,  // Green 520�570,
        435.8,  // Blue 450�490,
    };

    // @@ For each pixel:
    // What's the channel center for the RGB model?
    double  skydome_result[3];
    for (unsigned int i = 0; i < num_channels; i++) {
        skydome_result[i] = arhosekskymodel_radiance(skymodel_state[i], theta, gamma, channel_center[i]);
    }

    for (int i = 0; i < num_channels; i++) {
        arhosek_skymodelstate_free(skymodel_state[i]);
    }

    /*
    ArHosekXYZSkyModelState * skymodel_state[3];

    for (int i = 0; i < num_channels; i++) {
        skymodel_state[i] = arhosek_xyz_skymodelstate_alloc_init(turbidity, albedo[i], solarElevation);
    }

    // @@ For each pixel.
    double  skydome_result[3];
    for (unsigned int i = 0; i < num_channels; i++) {
        skydome_result[i] = arhosek_xyz_skymodel_radiance(skymodel_state[i], theta, gamma, i);
    }

    for (int i = 0; i < num_channels; i++) {
        arhosek_xyz_skymodelstate_free(skymodel_state[i]);
    }
    */
}
#endif