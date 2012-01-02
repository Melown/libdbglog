#ifndef shared_dbglog_stream_hpp_included_
#define shared_dbglog_stream_hpp_included_

#include <dbglog/logger.hpp>
#include <dbglog/level.hpp>
#include <dbglog/detail/log_helpers.hpp>
#include <dbglog/detail/logger.hpp>

#include <boost/noncopyable.hpp>

#include <sstream>

#include <stddef.h>

namespace dbglog {

template <typename SinkType>
class stream : public boost::noncopyable
{
public:
    stream(const location &loc, level l, SinkType &sink)
        : loc_(loc), l_(l), sink_(sink)
    {}

    ~stream() {
        sink_.log(l_, os_.str(), loc_);
    }

    template <typename T>
    std::basic_ostream<char>& operator<<(const T &t)
    {
        return os_ << t;
    }

private:
    std::ostringstream os_;
    const location loc_;
    const level l_;
    SinkType &sink_;
};

} // namespace dbglog

#define DBGLOG_NARG(...) DBGLOG_NARG_(__VA_ARGS__, DBGLOG_RSEQ_N())
#define DBGLOG_NARG_(...) DBGLOG_ARG_N(__VA_ARGS__)
#define DBGLOG_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define DBGLOG_RSEQ_N() 8, 7, 6, 5, 4, 3, 2, 1, 0

#define DBGLOG_CONCATENATE(arg1, arg2) DBGLOG_CONCATENATE1(arg1, arg2)
#define DBGLOG_CONCATENATE1(arg1, arg2) DBGLOG_CONCATENATE2(arg1, arg2)
#define DBGLOG_CONCATENATE2(arg1, arg2) arg1##arg2

#define DBGLOG_EXPAND_1(LEVEL) \
    if (!dbglog::detail::check_level(dbglog::LEVEL, dbglog::detail::deflog)); \
    else dbglog::stream<dbglog::logger> \
    (DBGLOG_PLACE, dbglog::LEVEL, dbglog::detail::deflog)

#define DBGLOG_EXPAND_2(LEVEL, SINK) \
    if (!dbglog::detail::check_level(dbglog::LEVEL, SINK)); \
    else dbglog::stream<decltype(SINK)>(DBGLOG_PLACE, dbglog::LEVEL, SINK)

#define DBGLOG_EXPAND_3(a, b, c) LOG_TAKES_EITHER_1_OR_2_ARGUMENTS_NOT_3
#define DBGLOG_EXPAND_4(a, b, c, d) LOG_TAKES_EITHER_1_OR_2_ARGUMENTS_NOT_4
#define DBGLOG_EXPAND_5(a, b, c, d, e) LOG_TAKES_EITHER_1_OR_2_ARGUMENTS_NOT_4
#define DBGLOG_EXPAND_6(a, b, c, d, e, f) LOG_TAKES_EITHER_1_OR_2_ARGUMENTS_NOT_5
#define DBGLOG_EXPAND_7(a, b, c, d, e, f, g) LOG_TAKES_EITHER_1_OR_2_ARGUMENTS_NOT_6
#define DBGLOG_EXPAND_8(a, b, c, d, e, f, g, h) LOG_TAKES_EITHER_1_OR_2_ARGUMENTS_NOT_7

#define DBGLOG_PLACE dbglog::location \
    (__FILE__, (const char*)__FUNCTION__, __LINE__)

#endif // shared_dbglog_stream_hpp_included_

