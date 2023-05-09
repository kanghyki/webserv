#include "./SessionManager.hpp"

const std::string   SessionManager::SESSION_KEY = "_webserv_session";
const unsigned int  SessionManager::INTERVAL_TIME = 5;
const unsigned int  SessionManager::SESSION_ID_LENGTH = 30;

void* sessionManagerRoutine(void *p) {
  SessionManager* manager = reinterpret_cast<SessionManager*>(p);

  while (1) {
    manager->cleanUpExpired();
    sleep(SessionManager::INTERVAL_TIME);
  }
  return NULL;
}

SessionManager::SessionManager() {
  pthread_mutex_init(&this->table_mutex, 0);
  if (pthread_create(&this->tid, 0, sessionManagerRoutine, (void *)this))
    throw std::runtime_error("create thread error");
  pthread_detach(this->tid);
}

SessionManager::~SessionManager(void) {
  pthread_mutex_destroy(&this->table_mutex);
}

void SessionManager::cleanUpExpired() {
  std::vector<std::map<std::string, time_t>::iterator> cleanUpList;

  pthread_mutex_lock(&this->table_mutex);
  for (std::map<std::string, time_t>::iterator it = this->table.begin(); it != this->table.end(); ++it) {
    if (time(NULL) > it->second) {
      cleanUpList.push_back(it);
    }
  }
  for (size_t i = 0; i < cleanUpList.size(); ++i) {
    this->table.erase(cleanUpList[i]);
  }
  pthread_mutex_unlock(&this->table_mutex);
}

std::string SessionManager::createSession(unsigned int expired_time) {
  std::string randomID;

  randomID = generateRandomString(SESSION_ID_LENGTH);

  pthread_mutex_lock(&this->table_mutex);
  this->table.insert(std::make_pair(randomID, time(NULL) + expired_time));
  pthread_mutex_unlock(&this->table_mutex);

  return randomID;
}

void SessionManager::removeSession(std::string sessionID) {
  pthread_mutex_lock(&this->table_mutex);
  this->table.erase(sessionID);
  pthread_mutex_unlock(&this->table_mutex);
}

bool SessionManager::isSessionAvailable(std::string sessionID) {
  bool        ret = true;
  std::string value;

  pthread_mutex_lock(&this->table_mutex);
  time_t sessionExpiredTime = this->table[sessionID];
  if (sessionExpiredTime == 0) ret = false;
  if (time(NULL) > sessionExpiredTime) {
    this->table.erase(sessionID);
    ret = false;
  }
  pthread_mutex_unlock(&this->table_mutex);

  return ret;
}

std::string SessionManager::generateRandomString(int ch) {
  std::string ret = "";
  std::string letterTable = "abcdefghijklmnopqrstuvwx";

  srand(time(0));
  for (int i = 0; i < ch; i++)
    ret = ret + letterTable[rand() % letterTable.length()];

  return ret;
}

void SessionManager::addSession(const std::string& sv, unsigned int expired_time) {
  pthread_mutex_lock(&this->table_mutex);
  size_t start = sv.find(SessionManager::SESSION_KEY) + SessionManager::SESSION_KEY.length() + 1;
  size_t end = sv.find("; ", start);
  this->table.insert(std::make_pair(sv.substr(start, end - start), time(NULL) + expired_time));
  pthread_mutex_unlock(&this->table_mutex);
}
