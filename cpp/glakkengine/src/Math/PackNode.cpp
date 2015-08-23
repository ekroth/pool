/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Math/PackNode.hpp"


using kke::PackNode;
using kke::Recti;

PackNode::PackNode(const Recti& size) :
    used(false),
    size(size)
{ }

PackNode::~PackNode()
{ }

bool PackNode::Insert(const Recti& input, Recti& output)
{
    // Isn't a leaf, means that it got children available
    if (!IsLeaf())
    {
        if (child[0]->Insert(input, output))
        {
            return true;
        }
        else if (child[1]->Insert(input, output))
        {
            return true;
        }

        return false;
    }
    else // Does not have branches
    {
        // If there's already a texture here, and the
        // node does not have any branches, then it's a leaf
        // and we cannot add anything.
        if (used)
        {
            return false;
        }

        // Texture does not fit
        if (!Fits(input))
        {
            return false;
        }

        // If it's a perfect fit, then this is a leaf, and we were successful.
        // Adding texture.
        if (PerfectFit(input))
        {
            used = true;
            output = size;
            return true;
        }

        // No branches, now create new ones
        // Create one which is a perfect fit (leaf), in top left corner
// 	Recti rect(size.X, size.Y, size.Width - texture->GetWidth(), size.Height - texture->GetHeight());

        int remainWidth = size.Width - input.Width;
        int remainHeight = size.Height - input.Height;

        const uint padding = 1;

        if (remainWidth > remainHeight)
        {
            child[0] = PN_ptr(new PackNode(Recti(
                                        size.Left(), size.Top(),
                                        input.Width, size.Height)));

            // To the right
            child[1] = PN_ptr(new PackNode(Recti(
                                        size.Left() + input.Width + padding, size.Top(),
                                        remainWidth - padding, size.Height)));
        }
        else
        {
            child[0] = PN_ptr(new PackNode(Recti(
                                        size.Left(), size.Top(),
                                        size.Width, input.Height)));

            // Below
            child[1] = PN_ptr(new PackNode(Recti(
                                        size.Left(), size.Top() + input.Height + padding,
                                        size.Width, remainHeight - padding)));
        }

        return child[0]->Insert(input, output);
    }
}

bool PackNode::IsLeaf() const
{
    return !(child[0] || child[1]);
}

bool PackNode::Fits(const Recti& input) const
{
    return input.Width <= size.Width &&
           input.Height <= size.Height;
}

bool PackNode::PerfectFit(const Recti& input) const
{
    return input.Width == size.Width &&
           input.Height == size.Height;
}