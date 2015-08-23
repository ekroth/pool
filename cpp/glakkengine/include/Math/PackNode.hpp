/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_PACKNODE_HPP
#define KKE_PACKNODE_HPP

#include "Math/Rect.hpp"
#include "Interface/INoncopyable.hpp"

#include <memory>

namespace kke
{
	/**
	* @brief Packing a Rectangle<int> into a sheet.
	**/
	class PackNode : public INoncopyable
	{
	public:
		explicit PackNode(const Recti &size);
		~PackNode();

		/**
		* @brief Insert a Rectangle<int>.
		*
		* @param input Recti.
		* @param output Outputs the position of the Rectangle.
		* @return bool If there was enough space for the Rectangle.
		**/
		bool Insert(const Recti& input, Recti& output);

		/**
		* @brief If the PackNode is a leaf. Means it only contains a Rectangle and no branches.
		*
		* @return bool Is a leaf.
		**/
		bool IsLeaf() const;
		
		/**
		* @brief If Rectangle fits inside this PackNode.
		*
		* @param input Rectangle.
		* @return bool If it fits.
		**/
		bool Fits(const Recti& input) const;
		
		/**
		* @brief If the Rectangle fits perfectly inside the PackNode. Used to specifiy if PackNode should be a leaf.
		*
		* @param input Rectangle.
		* @return bool Perfect fit.
		**/
		bool PerfectFit(const Recti& input) const;
        
	private:
        typedef std::shared_ptr<PackNode> PN_ptr;
        
    private:
		PN_ptr child[2];
		bool used;
		Recti size;
	};
}

#endif // KKE_PACKNODE_HPP