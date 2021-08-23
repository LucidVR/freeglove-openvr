#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "Encode/EncodingManager.h"

class ICommunicationManager {
 public:
  ICommunicationManager(std::unique_ptr<IEncodingManager> encodingManager);

  virtual void BeginListener(const std::function<void(VRCommData_t)>& callback);
  virtual void Disconnect();
  virtual void QueueSend(const VRFFBData_t& data);

  virtual bool IsConnected() = 0;

 protected:
  virtual void ListenerThread(const std::function<void(VRCommData_t)>& callback);
  virtual void WaitAttemptConnection() = 0;

  virtual bool Connect() = 0;
  virtual bool DisconnectFromDevice() = 0;
  virtual void LogError(const char* message) = 0;
  virtual void LogMessage(const char* message) = 0;
  virtual bool ReceiveNextPacket(std::string& buff) = 0;

 protected:
  std::unique_ptr<IEncodingManager> m_encodingManager;

  std::atomic<bool> m_threadActive;
  std::thread m_thread;

  std::mutex m_writeMutex;
  std::string m_writeString;
};