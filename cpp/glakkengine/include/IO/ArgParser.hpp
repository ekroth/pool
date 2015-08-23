/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_ARGPARSER_HPP
#define KKE_ARGPARSER_HPP

#include "IO/Argument.hpp"
#include "Interface/INoncopyable.hpp"
#include "Utilities/FNV1aHash.hpp"
#include "Utilities/Singleton.hpp"

#include <string>
#include <unordered_map>

#define ArgHasher(x) KKE_FNV1AHASH(x)

namespace kke
{
	typedef FNV1aHash::Hash ArgumentID;
	
	/**
	 * @brief Run-time hash
	 **/
	inline ArgumentID ArgHasherRT(const char* str) { return FNV1aHash::hash_rt(str); }
	
	/**
	 * @brief Run-time hash
	 **/
	inline ArgumentID ArgHasherRT(const std::string& str) { return FNV1aHash::hash_rt(str.c_str()); }
    
	/**
	 * @brief Registers arguments to an integer, and parses them.
	 **/
	class ArgParser : public Singleton<ArgParser>
	{
		friend class Singleton<ArgParser>;
	public:
		
		/**
		 * @brief Process the arguments.
		 * @return bool
		 **/
		bool Process(int argc, char **argv);
		
		const Argument* GetArgument(const ArgumentID& id);
		
		/**
		 * @brief If argument has been registered.
		 * @return bool
		 **/
		bool Registered(const ArgumentID& id);
		
		/**
		 * @brief If registered argument has been parsed.
		 * @return bool
		 **/
		bool Exist(const ArgumentID& id);
		
		/**
		 * @brief Return argument index.
		 * @return int
		 **/
		int ValidArgument(const std::string &arg) const;
		
		/**
		 * @brief Register a argument.
		 *
		 * @param id Unique argument ID.
		 * @param type Type of argument.
		 * @param lName Long name of argument. (--<lName>)
		 * @param sName Short name of argument. (-<sName>) Defaults to std::string().
		 * @param info Description of argument. Defaults to "No description.".
		 * @return bool
		 **/
		bool Register(const ArgumentID& id, ArgumentType type, const std::string &lName, const std::string &sName = std::string(), const std::string &info = "No description.");
		
		void CaseSense(bool value);
	private:
		ArgParser();
		~ArgParser();
		
	private:
		
		typedef std::unordered_map<ArgumentID, Argument> ArgMap;
	
		ArgMap argMap;
		bool caseSense;
	};
}

#endif // KKE_ARGPARSER_HPP
