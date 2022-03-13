#pragma once

#include <array>
#include <memory>
#include <string>

#include "DriverLog.h"
#include "openvr_driver.h"

struct VRInputData {
  VRInputData()
      : VRInputData(
            {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 0.0f, 0.0f, 0.0f, false, false, false, false, false, false, false, false){};

  VRInputData(
      std::array<float, 5> flexion,
      float joyX,
      float joyY,
      float trgValue,
      bool joyButton,
      bool trgButton,
      bool aButton,
      bool bButton,
      bool grab,
      bool pinch,
      bool menu,
      bool calibrate)
      : flexion({
            flexion[0], flexion[0], flexion[0], flexion[0], flexion[1], flexion[1], flexion[1], flexion[1], flexion[2], flexion[2],
            flexion[2], flexion[2], flexion[3], flexion[3], flexion[3], flexion[3], flexion[4], flexion[4], flexion[4], flexion[4],
        }),
        joyX(joyX),
        joyY(joyY),
        trgValue(trgValue),
        joyButton(joyButton),
        trgButton(trgButton),
        aButton(aButton),
        bButton(bButton),
        grab(grab),
        pinch(pinch),
        menu(menu),
        calibrate(calibrate) {}

  VRInputData(
      std::array<std::array<float, 4>, 5> flexion,
      std::array<float, 5> splay,
      float joyX,
      float joyY,
      float trgValue,
      bool joyButton,
      bool trgButton,
      bool aButton,
      bool bButton,
      bool grab,
      bool pinch,
      bool menu,
      bool calibrate)
      : flexion(flexion),
        splay(splay),
        joyX(joyX),
        joyY(joyY),
        trgValue(trgValue), 
        joyButton(joyButton),
        trgButton(trgButton),
        aButton(aButton),
        bButton(bButton),
        grab(grab),
        pinch(pinch),
        menu(menu),
        calibrate(calibrate) {}

  const std::array<std::array<float, 4>, 5> flexion;
  const std::array<float, 5> splay = {-2.0f, -2.0f, -2.0f, -2.0f, -2.0f};
  const float joyX;
  const float joyY;
  const float trgValue;
  const bool joyButton;
  const bool trgButton;
  const bool aButton;
  const bool bButton;
  const bool grab;
  const bool pinch;
  const bool menu;
  const bool calibrate;
};

//force feedback
struct VRFFBData {
  VRFFBData() : VRFFBData(0, 0, 0, 0, 0){};
  VRFFBData(short thumbCurl, short indexCurl, short middleCurl, short ringCurl, short pinkyCurl)
      : thumbCurl(thumbCurl), indexCurl(indexCurl), middleCurl(middleCurl), ringCurl(ringCurl), pinkyCurl(pinkyCurl){};

  const short thumbCurl;
  const short indexCurl;
  const short middleCurl;
  const short ringCurl;
  const short pinkyCurl;
};

// vibration
struct VRHapticData {
  VRHapticData(vr::VREvent_HapticVibration_t hapticData)
      : duration(hapticData.fDurationSeconds), frequency(hapticData.fFrequency), amplitude(hapticData.fAmplitude){};
  VRHapticData(float duration, float frequency, float amplitude) : duration(duration), frequency(frequency), amplitude(amplitude){};

  const float duration;
  const float frequency;
  const float amplitude;
};

enum class VROutputDataType { ForceFeedback, Haptic };

typedef union VROutputData {
  VROutputData(const VRHapticData& hapticData) : hapticData(hapticData) {}
  VROutputData(const VRFFBData& ffbData) : ffbData(ffbData) {}

  VRHapticData hapticData;
  VRFFBData ffbData;
} VROutputData;

struct VROutput {
  VROutput(const VRHapticData& hapticData) : type(VROutputDataType::Haptic), data(hapticData){};
  VROutput(const VRFFBData& ffbData) : type(VROutputDataType::ForceFeedback), data(ffbData){};

  VROutputDataType type;
  VROutputData data;
};

class EncodingManager {
 public:
  explicit EncodingManager(float maxAnalogValue) : maxAnalogValue_(maxAnalogValue){};
  virtual VRInputData Decode(const std::string& input) = 0;
  virtual std::string Encode(const VROutput& data) = 0;

 protected:
  float maxAnalogValue_;
};

template <typename... Args>
std::string StringFormat(const std::string& format, Args... args) {
  const int sizeS = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;  // Extra space for '\0'

  if (sizeS <= 0) {
    DriverLog("Error decoding string");
    return "";
  }

  const auto size = static_cast<size_t>(sizeS);
  const auto buf = std::make_unique<char[]>(size);
  std::snprintf(buf.get(), size, format.c_str(), args...);

  return std::string(buf.get(), buf.get() + size - 1);  // We don't want the '\0' inside
}
