/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_HANDLED_HPP
#define KKE_HANDLED_HPP

namespace kke
{
    template<typename HandleType>
    class Handled
    {
    public:
        explicit Handled() : handle(0) { }
        virtual void SetHandle(HandleType handle);
        virtual const HandleType& GetHandle() const;
        
        virtual bool Generate() = 0;
        virtual bool Delete() = 0;
		
		inline bool Generated() const { return handle != 0; }
        
    protected:
        HandleType handle;
    };
    
    #include "Graphics/Handled.inl"
}

#endif // KKE_HANDLED_HPP