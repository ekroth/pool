/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_LOGMANAGER_HPP
#define KKE_LOGMANAGER_HPP

#include "IO/ILog.hpp"
#include "Utilities/Singleton.hpp"

#define MIN_LOG_LEVEL kke::LogLevel::Debug

#include <cstring>
constexpr size_t LENGTH(const char* str)
{
    return strlen(str);
}

#define LOG(lvl) \
	if ((int)lvl > (int)MIN_LOG_LEVEL) ; \
	else if ((int)lvl > (int)kke::LogManager::Instance().GetMinLevel()) ; \
	else kke::ILog::LogLife(kke::LogManager::Instance().GetLog(), lvl).log.GetStream() << (__FILE__ + LENGTH(ROOT_FOLDER) + 1) <<  ':' << __LINE__ << " <" << __FUNCTION__ << "> " \

#ifdef DEBUG
#define LOGASSERT(expr, msg) \
    if (!(expr)) \
    { LOG(kke::LogLevel::Error) << "- Assertion \'" << #expr << "\' failed with message: " << '\'' << msg << '\''; abort(); }
#else
#define LOGASSERT(expr, msg) ;
#endif
    
#define LOGE() LOG(kke::LogLevel::Error) 
#define LOGW() LOG(kke::LogLevel::Warning)
#define LOGM() LOG(kke::LogLevel::Message)    
#define LOGD() LOG(kke::LogLevel::Debug)

namespace kke
{
    /**
     * @brief Singleton that manages logs.
     **/
    class LogManager : public Singleton<LogManager>
	{
		friend class Singleton<LogManager>;
	public:
        /**
         * @brief Set log.
         *
         * @param log Log.
         * @param owned If the log should be managed by LogManager.
         * @return void
         **/
        void SetLog(ILog* log, bool owned);
		ILog& GetLog();
		
        /**
         * @brief Minimum LogLevel that'll be outputted.
         *
         * @param lvl ...
         * @return void
         **/
        void SetMinLevel(const LogLevel& lvl);
		const LogLevel& GetMinLevel() const;
		
	private:
        LogManager();
        ~LogManager();
        
    private:
		ILog* log;
		LogLevel lvl;
		bool owned;
	};
}

#endif // KKE_LOGMANAGER_HPP