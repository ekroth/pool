/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_ARGUMENT_HPP
#define KKE_ARGUMENT_HPP

#include <string>

namespace kke
{
	enum class ArgumentType
	{
		Exist,
		String,
		Int
	};

	class Argument
	{
	public:
		Argument();
		Argument(ArgumentType type, const std::string &lName, const std::string &sName, const std::string &info);
		
		bool IsArg(const std::string &arg, bool caseSense = true) const;
		const std::string& GetLongName() const;
		const std::string& GetShortName() const;
		const std::string& GetString() const;
		const std::string& GetInfo() const;
		int GetInt() const;
		ArgumentType GetType() const;
		void SetSvalue(const std::string &value);
		void SetIvalue(int value);
		void SetInfo(const std::string &info);
		
		static std::string StringToLow(const std::string& var);

	private:
		std::string 
			lName,
			sName,
			info,
			sValue;
		int iValue;
		
		ArgumentType type;
	};
}

#endif // KKE_ARGUMENT_HPP