/* Copyright (c) 2011-2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_INONCOPYABLE_HPP
#define KKE_INONCOPYABLE_HPP

namespace kke
{
	/**
	 * @brief Cannot be copied.
	 **/
	class INoncopyable
    {
	public:
        INoncopyable() { }
        INoncopyable(const INoncopyable &&) = delete;
        INoncopyable& operator =(const INoncopyable &&) = delete;
    };
}

#endif // KKE_NONCOPYABLE_HPP
