#ifndef SESSION_MANAGER_HPP
# define SESSION_MANAGER_HPP

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

    SessionManager(unsigned int expired_max);
    ~SessionManager(void);

    void                          cleanUpExpired(void);
    std::string                   createSession(void);
    void                          removeSession(std::string sessionID);
    bool                          isSessionAvailable(std::string sessionID);

    // for debug
    void                          showSession();

  private:
    static const std::string      SESSION_KEY;
    static const unsigned int     SESSION_ID_LENGTH;

    const unsigned int            expired_max;
    pthread_t                     tid;
    // sessionID, time(sec)
    std::map<std::string, time_t> table;
    pthread_mutex_t               table_mutex;

    std::string                   generateRandomString(int ch);
};

#endif
