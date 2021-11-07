#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "DeviceConfiguration.h"
#include "Encode/EncodingManager.h"

class CommunicationManager {
 public:
  CommunicationManager(const VRDeviceConfiguration_t& deviceConfiguration) : this(nullptr, deviceConfiguration);
  CommunicationManager(std::unique_ptr<EncodingManager> encodingManager, const VRDeviceConfiguration_t& deviceConfiguration);

  virtual void BeginListener(const std::function<void(VRInputData)>& callback);
  virtual void Disconnect();
  virtual void QueueSend(const VRFFBData& data);

  virtual bool IsConnected() = 0;

 protected:
  virtual void ListenerThread(const std::function<void(VRInputData)>& callback);
  virtual void WaitAttemptConnection();

  virtual bool Connect() = 0;
  virtual bool DisconnectFromDevice() = 0;
  virtual void LogError(const char* message) = 0;
  virtual void LogMessage(const char* message) = 0;
  virtual bool ReceiveNextPacket(std::string& buff) = 0;
  virtual bool SendMessageToDevice() = 0;

 protected:
  std::unique_ptr<EncodingManager> m_encodingManager;
  VRDeviceConfiguration_t m_deviceConfiguration;

  std::atomic<bool> m_threadActive;
  std::thread m_thread;

  std::mutex m_writeMutex;
  std::string m_writeString;
};