/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_ILOG_HPP
#define KKE_ILOG_HPP

#include <ostream>
#include <string>

namespace kke
{
	enum class LogLevel : unsigned int
	{
		Error = 0,
		Warning = 1,
		Message = 2,
		Debug = 3
	};
	
    /**
     * @brief Interface that specifies a Log.
     **/
    class ILog
	{
	public:
		virtual ~ILog() { }
        virtual void Begin(const LogLevel& level);
        virtual void End(const LogLevel& level);
		virtual std::ostream& GetStream() = 0;
		
	public:
        /**
         * @brief Simple convenience class that calls Begin and End of a ILog.
         **/
        class LogLife
		{
        public:
			LogLife(ILog& log, const LogLevel& lvl) : 
				log(log),
				lvl(lvl)
			{ 
                log.Begin(lvl);
            }
			
			~LogLife()
			{
				log.End(lvl);
			}
			
			ILog& log;
			const LogLevel& lvl;
		};
		
	protected:
        static const std::string MSG_SHORTS[];
	};
}

#endif // KKE_ILOG_HPP