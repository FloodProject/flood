/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Geometry.h"

namespace vapor { namespace gui {

//-----------------------------------//

struct Rect
{
    Rect(int size)
        : x(0), y(0), w(size), h(size), ID(-1), rotated(false), packed(false)
    {
        children[0] = -1;
        children[1] = -1;
    }

    Rect(int x, int y, int w, int h, int ID = 1)
        : x(x), y(y), w(w), h(h), ID(ID), rotated(false), packed(false)
    {
        children[0] = -1;
        children[1] = -1;
    }
    
    int GetArea() const 
	{
        return w * h;
    }
    
    void Rotate() 
	{
        std::swap(w, h);
        rotated = !rotated;
    }
    
    bool operator<(const Rect& rect) const 
	{
        return GetArea() < rect.GetArea();
    }

    int  x;
    int  y;
    int  w;
    int  h;
    int  ID;
    int  children[2];
    bool rotated;
    bool packed;
};

/**
 * The input and output are in terms of vectors of ints to avoid
 * dependencies (although I suppose a public member struct could have been
 * used). The parameters are:

 * rects : An array containing the width and height of each input rect in
 * sequence, i.e. [w0][h0][w1][h1][w2][h2]... The IDs for the rects are
 * derived from the order in which they appear in the array.

 * packs : After packing, the outer array contains the packs (therefore
 * the number of packs is packs.size()). Each inner array contains a
 * sequence of sets of 4 ints. Each set represents a rectangle in the
 * pack. The elements in the set are 1) the rect ID, 2) the x position
 * of the rect with respect to the pack, 3) the y position of the rect
 * with respect to the pack, and 4) whether the rect was rotated (1) or
 * not (0). The widths and heights of the rects are not included, as it's
 * assumed they are stored on the caller's side (they were after all the
 * input to the function).

 * allowRotation : when true (the default value), the packer is allowed
 * the option of rotating the rects in the process of trying to fit them
 * into the current working area.
 *
 * http://en.wikipedia.org/wiki/Texture_atlas
 * http://www.gamedev.net/community/forums/topic.asp?topic_id=392413&#2599800
 */

class VAPOR_API Atlas
{
public:

    void pack( const std::vector<int>& rects, 
		std::vector< std::vector<int> >& packs,
        int packSize,
		bool allowRotation = true );

private:

    void clear();
    void fill(int pack, bool allowRotation);
    void split(int pack, int rect);
    bool fits(Rect& rect1, const Rect& rect2, bool allowRotation);
    void addPackToArray(int pack, std::vector<int>& array) const;
    
    bool rectIsValid(int i) const;
    bool packIsValid(int i) const;
    
    int packSize;
    int numPacked;

    std::vector<Rect> rects;
    std::vector<Rect> packs;
    std::vector<int>  roots;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_TYPE( Atlas );

//-----------------------------------//

} } // end namespaces


