/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "IO/ILog.hpp"

namespace kke
{
    /**
     * @brief Simple log that outputs to a specified ostream.
     **/
    class LogOStream : public ILog
    {
    public:
        LogOStream(std::ostream* stream, bool owned);
        ~LogOStream();
        
        void Begin(const LogLevel& level) override;
        void End(const LogLevel& level) override;
        std::ostream& GetStream() override;
        
    private:
        std::ostream* stream;
        bool owned;
    };
}