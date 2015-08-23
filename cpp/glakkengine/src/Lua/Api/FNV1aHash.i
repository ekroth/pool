%module FNV1aHash
%{
#include "Utilities/FNV1aHash.hpp"
%}

namespace kke
{
	namespace FNV1aHash
	{
		kke::FNV1aHash::Hash hash_rt(const char* str);
	}
}
