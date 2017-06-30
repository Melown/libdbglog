/**
 * Copyright (c) 2017 Melown Technologies SE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * *  Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef shared_dbglog_detail_log_helpers_hpp_included_
#define shared_dbglog_detail_log_helpers_hpp_included_

#include <dbglog/level.hpp>
#include <dbglog/location.hpp>

#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include <atomic>

#include "./system.hpp"

namespace dbglog { namespace detail {

inline const char* level2string(dbglog::level l)
{
    switch (l) {
    case debug: return "DD";

    case info1: return "I1";
    case info2: return "I2";
    case info3: return "I3";
    case info4: return "I4";

    case warn1: return "W1";
    case warn2: return "W2";
    case warn3: return "W3";
    case warn4: return "W4";

    case err1: return "E1";
    case err2: return "E2";
    case err3: return "E3";
    case err4: return "E4";

    case fatal: return "FF";

    default: return "??";
    }
}

typedef char timebuffer[64];

inline char* format_time(timebuffer &b, unsigned short precision = 0)
{
    timeval now;
    gettimeofday(&now, 0x0);
    tm now_bd;
    localtime_r(&now.tv_sec, &now_bd);
    auto end(b + strftime(b, sizeof(b) - 1, "%Y-%m-%d %T", &now_bd));

    // append
    switch (precision) {
    case 0: break;

    case 1:
        sprintf(end, ".%01u"
                , static_cast<unsigned int>(now.tv_usec / 100000));
        break;

    case 2:
        sprintf(end, ".%02u"
                , static_cast<unsigned int>(now.tv_usec / 10000));
        break;

    case 3:
        sprintf(end, ".%03u"
                , static_cast<unsigned int>(now.tv_usec / 1000));
        break;

    case 4:
        sprintf(end, ".%04u"
                , static_cast<unsigned int>(now.tv_usec / 100));
        break;

    case 5:
        sprintf(end, ".%05u", static_cast<unsigned int>(now.tv_usec / 10));
        break;

    default:
        // 6 and more
        sprintf(end, ".%06u", static_cast<unsigned int>(now.tv_usec));
        break;
    }

    return b;
}

class thread_id {
public:
    static const std::string& get() {
        if (!holder_.get()) {
            holder_.reset(new std::string(boost::lexical_cast<std::string>
                                          (generator_++)));
            setThreadName(*holder_);
        }
        return *holder_;
    }

    static void set(const std::string &value) {
        if (!holder_.get()) {
            holder_.reset(new std::string(value));
        } else {
            *holder_ = value;
        }
        setThreadName(value);
    }

private:
    static boost::thread_specific_ptr<std::string> holder_;
    static std::atomic_uint_fast64_t generator_;
};

} // namespace detail

inline std::ostream& operator<<(std::ostream &os, const dbglog::location &l) {
    return os << '{' << l.file << ':' << l.func << "():" << l.line << '}';
}

} // namespace dbglog

#endif // shared_dbglog_detail_log_helpers_hpp_included_
