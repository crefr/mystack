#ifndef LOGGER_INCLUDED
#define LOGGER_INCLUDED

#define LOGPRINT(loglevel, ...)                  \
        do{                                      \
                logPrintTime(loglevel);          \
                logPrint(loglevel, __VA_ARGS__); \
        }while(0)

#define LOGEXIT()                                                       \
        do{                                                             \
                logPrint(0, "<-----------ENDING------------>\n");       \
                logExit();                                              \
        }while(0)

int logStart(const char * logfile, int loglevel);
void logPrint(int loglevel, const char * fmt, ...);
void logPrintTime(int loglevel);
void logExit(void);


/// @brief different levels of logging, IT IS NECESSARY TO WRITE THEM IN ASCENDING ORDER
enum loglevels{LOG_RELEASE, LOG_DEBUG, LOG_DEBUG_PLUS};

#endif
