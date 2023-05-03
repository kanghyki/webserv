#ifndef SESSION_MANAGER_HPP
# define SESSION_MANAGER_HPP

# include "./Logger.hpp"
# include <iostream>

# include <pthread.h>
# include <unistd.h>
# include <string>
# include <map>
# include <vector>
# include <time.h>

void* sessionManagerRoutine(void *p);

class SessionManager {
  public:
    static const unsigned int     INTERVAL_TIME;

    SessionManager();
    ~SessionManager(void);

    void                          cleanUpExpired(void);
    std::string                   createSession(unsigned int expired_time);
    void                          removeSession(std::string sessionID);
    bool                          isSessionAvailable(std::string sessionID);
    void                          addSession(const std::string& sessionID, unsigned int expired_time);

    // for debug
    void                          showSession();

    static const std::string      SESSION_KEY;

  private:
    static const unsigned int     SESSION_ID_LENGTH;

    pthread_t                     tid;
    // sessionID, time(sec)
    std::map<std::string, time_t> table;
    pthread_mutex_t               table_mutex;

    std::string                   generateRandomString(int ch);
};

#endif
