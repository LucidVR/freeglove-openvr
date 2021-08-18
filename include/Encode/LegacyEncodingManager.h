#pragma once

#include <Encode/EncodingManager.h>
#include "ForceFeedback.h"

static const char* c_legacyEncodingSettingsSection = "encoding_legacy";

class LegacyEncodingManager : public IEncodingManager {
 public:
  LegacyEncodingManager(float maxAnalogValue) : m_maxAnalogValue(maxAnalogValue){};

  VRCommData_t Decode(std::string input);

  std::string Encode(const VRFFBData_t& input);

 private:
  float m_maxAnalogValue;
};