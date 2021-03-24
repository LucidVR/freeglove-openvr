#pragma once
#include "openvr_driver.h"
#include "Comm/SerialCommunicationManager.h"

enum VRDeviceProtocol {
	SERIAL = 0,
};

struct VRDeviceConfiguration_t {
	VRDeviceConfiguration_t(vr::ETrackedControllerRole role, bool enabled, vr::HmdVector3_t offsetVector, vr::HmdVector3_t angleOffsetVector, float poseOffset, VRSerialConfiguration_t serialConfiguration) :
		role(role),
		enabled(enabled),
		offsetVector(offsetVector),
		angleOffsetVector(angleOffsetVector),
		poseOffset(poseOffset),
		serialConfiguration(serialConfiguration),
		protocol(VRDeviceProtocol::SERIAL) {};

	vr::ETrackedControllerRole role;
	bool enabled;
	vr::HmdVector3_t offsetVector;
	vr::HmdVector3_t angleOffsetVector;

	float poseOffset;

	VRSerialConfiguration_t serialConfiguration;

	VRDeviceProtocol protocol;

};
