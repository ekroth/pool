/* Copyright (c) 2012 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_INONMOVEABLE_HPP
#define KKE_INONMOVEABLE_HPP

namespace kke
{
    /**
     * @brief Cannot be copied.
     **/
    class INonmoveable
    {
    public:
        INonmoveable() { }
        INonmoveable(INonmoveable &&) = delete;
        INonmoveable& operator =(INoncopyable &&) = delete;
    };
}

#endif // KKE_NONMOVEABLE_HPP
