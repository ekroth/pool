/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

// Edited by Andrée Ekroth 2011-07-09
// Original by elanthis.
// https://github.com/elanthis/constexpr-hash-demo (2011-07-09)

#ifndef KKE_FNV1AHASH_HPP
#define KKE_FNV1AHASH_HPP

// #define KKE_FNV1AHASH(x) 2
#define KKE_FNV1AHASH(x) kke::FNV1aHash::Hash_constexpr_val<kke::FNV1aHash::hash_constexpr(x)>::Value

namespace kke
{
	namespace FNV1aHash
	{
		typedef unsigned long long Hash;
		
		// NOTE: hashing algorithm used is FNV-1a

		// FNV-1a constants
		static const Hash basis = 14695981039346656037ULL;
		static const Hash prime = 1099511628211ULL;
        
        // compile-time hash helper function
        constexpr Hash hash_one_constexpr(char c, const char* remain, Hash value)
        {
            return c == '\0' ? value : hash_one_constexpr(remain[0], remain + 1, (value ^ c) * prime);
        }
		
		// compile-time hash
		constexpr Hash hash_constexpr(const char* str)
		{
			return hash_one_constexpr(str[0], str + 1, basis);
		}

		// run-time hash
		inline Hash hash_rt(const char* str)
		{
			Hash hash = basis;
			while (*str != 0) {
				hash ^= str[0];
				hash *= prime;
				++str;
			}
			return hash;
		}
		
		template <Hash NUM>
        /**
         * @brief We have to force constexpr.
         **/
        struct Hash_constexpr_val
        {
            static const Hash Value;
        };
	}
}

template <kke::FNV1aHash::Hash NUM>
const kke::FNV1aHash::Hash kke::FNV1aHash::Hash_constexpr_val<NUM>::Value = NUM;

#endif // KKE_EVENTHASH_HPP