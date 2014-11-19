#ifndef DEBUGLOG_H
#define DEBUGLOG_H

#ifndef NDEBUG
#define DEBUG_MSG(msg) std::cerr << msg << std::endl;
#else
#define DEBUG_MSG(msg)
#endif

#endif // DEBUGLOG_H
