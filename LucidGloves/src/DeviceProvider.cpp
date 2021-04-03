#include <DeviceProvider.h>

vr::EVRInitError DeviceProvider::Init(vr::IVRDriverContext* pDriverContext) {
	vr::EVRInitError initError = InitServerDriverContext(pDriverContext);
	if (initError != vr::EVRInitError::VRInitError_None) {
		return initError;
	}
	VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);
	InitDriverLog(vr::VRDriverLog());
	DebugDriverLog("Initializing LucidGloves");

	VRDeviceConfiguration_t leftConfiguration = GetConfiguration(vr::TrackedControllerRole_LeftHand);
	VRDeviceConfiguration_t rightConfiguration = GetConfiguration(vr::TrackedControllerRole_RightHand);

	if (leftConfiguration.enabled) {
		m_leftHand = InstantiateDeviceDriver(leftConfiguration);
		vr::VRServerDriverHost()->TrackedDeviceAdded(m_leftHand->m_serialNumber, vr::TrackedDeviceClass_Controller, m_leftHand);
	}
	if (rightConfiguration.enabled) {
		m_rightHand = InstantiateDeviceDriver(rightConfiguration);
		vr::VRServerDriverHost()->TrackedDeviceAdded(m_rightHand->m_serialNumber, vr::TrackedDeviceClass_Controller, m_rightHand);
	}

	return vr::VRInitError_None;
}

IDeviceDriver* DeviceProvider::InstantiateDeviceDriver(const VRDeviceConfiguration_t& configuration) {
	switch (configuration.selectedDeviceDriver) {
	case VRDeviceDriver::EMULATED_KNUCKLES:
		break;
	default:
		DriverLog("No device driver selected. Using lucidgloves.");
	case VRDeviceDriver::LUCIDGLOVES:
		std::shared_ptr<IDeviceDriver> deviceDriver = std::make_shared<LucidGloveDeviceDriver>(configuration);
		return deviceDriver.get();
	}
}
VRDeviceConfiguration_t DeviceProvider::GetConfiguration(vr::ETrackedControllerRole role) {
	const VRCommunicationProtocol communicationProtocol = (VRCommunicationProtocol)vr::VRSettings()->GetInt32(c_settingsSection, "communication_protocol");
	const VREncodingProtocol encodingProtocol = (VREncodingProtocol)vr::VRSettings()->GetInt32(c_settingsSection, "encoding_protocol");
	const VRDeviceDriver deviceDriver = (VRDeviceDriver)vr::VRSettings()->GetInt32(c_settingsSection, "device_driver");

	const int maxAnalogValue = vr::VRSettings()->GetInt32(c_settingsSection, "max_analog_value");

	const float offsetX = vr::VRSettings()->GetFloat(c_settingsSection, "x_offset");
	const float offsetY = vr::VRSettings()->GetFloat(c_settingsSection, "y_offset");
	const float offsetZ = vr::VRSettings()->GetFloat(c_settingsSection, "z_offset");

	const float offsetXDeg = vr::VRSettings()->GetFloat(c_settingsSection, "x_offset_degrees");
	const float offsetYDeg = vr::VRSettings()->GetFloat(c_settingsSection, "y_offset_degrees");
	const float offsetZDeg = vr::VRSettings()->GetFloat(c_settingsSection, "z_offset_degrees");

	const bool leftFlippedXPos = vr::VRSettings()->GetBool(c_settingsSection, "left_flipped_pos_x");
	const bool leftFlippedYPos = vr::VRSettings()->GetBool(c_settingsSection, "left_flipped_pos_y");
	const bool leftFlippedZPos = vr::VRSettings()->GetBool(c_settingsSection, "left_flipped_pos_z");

	const bool leftFlippedXRot = vr::VRSettings()->GetBool(c_settingsSection, "left_flipped_rot_x");
	const bool leftFlippedYRot = vr::VRSettings()->GetBool(c_settingsSection, "left_flipped_rot_y");
	const bool leftFlippedZRot = vr::VRSettings()->GetBool(c_settingsSection, "left_flipped_rot_z");

	const bool isRightHand = role == vr::TrackedControllerRole_RightHand;

	const bool isEnabled = vr::VRSettings()->GetBool(c_settingsSection, isRightHand ? "right_enabled" : "left_enabled");

	//x axis may be flipped for the different hands
	const vr::HmdVector3_t offsetVector = {
		(isRightHand || !leftFlippedXPos) ? offsetX : -offsetX,
		(isRightHand || !leftFlippedYPos) ? offsetY : -offsetX,
		(isRightHand || !leftFlippedZPos) ? offsetZ : -offsetZ
	};
	const vr::HmdVector3_t angleOffsetVector = {
		(isRightHand || !leftFlippedXRot) ? offsetXDeg : -offsetXDeg,
		(isRightHand || !leftFlippedYRot) ? offsetYDeg : -offsetYDeg,
		(isRightHand || !leftFlippedZRot) ? offsetZDeg : -offsetZDeg
	};

	const float poseOffset = vr::VRSettings()->GetFloat(c_settingsSection, "pose_offset");

	std::shared_ptr<ICommunicationManager> communicationManager;
	std::shared_ptr<IEncodingManager> encodingManager;


	switch (encodingProtocol) {
	default:
		DriverLog("No encoding protocol set. Using legacy.");
	case VREncodingProtocol::LEGACY:
		encodingManager = std::make_shared<LegacyEncodingManager>(maxAnalogValue);
		break;
	}

	switch (communicationProtocol) {
	default:
		DriverLog("No communication protocol set. Using serial.");
	case VRCommunicationProtocol::SERIAL:
		char port[16];
		vr::VRSettings()->GetString(c_settingsSection, role == vr::TrackedControllerRole_RightHand ? "serial_right_port" : "serial_left_port", port, sizeof(port));
		VRSerialConfiguration_t serialSettings(port);

		communicationManager = std::make_shared<SerialCommunicationManager>(serialSettings, encodingManager.get());
		break;
	}

	VRDeviceConfiguration_t deviceSettings(role, isEnabled, offsetVector, angleOffsetVector, poseOffset, communicationManager.get(), encodingManager.get(), deviceDriver);

	return deviceSettings;
}
void DeviceProvider::Cleanup() {}
const char* const* DeviceProvider::GetInterfaceVersions() {
	return vr::k_InterfaceVersions;
}

void DeviceProvider::RunFrame() {
	if (m_leftHand)
		m_leftHand->RunFrame();
	if (m_rightHand)
		m_rightHand->RunFrame();
}

bool DeviceProvider::ShouldBlockStandbyMode() {
	return false;
}

void DeviceProvider::EnterStandby() {}

void DeviceProvider::LeaveStandby() {}
