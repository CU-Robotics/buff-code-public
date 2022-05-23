#include <Arduino.h>

#include "gimbal.h"
#include "state/state.h"
#include "state/config.h"
#include "drivers/gm6020.h"
#include "algorithms/PID_Filter.h"

Gimbal::Gimbal() {
  
}

void Gimbal::setup(C_Gimbal *data, S_Robot *r_state) {
  config = data;
  state = r_state;
}

void Gimbal::update(float deltaTime) {
  state->gimbal.yaw_PID.R += state->driverInput.mouseX * config->sensitivity;
  state->gimbal.pitch_PID.R += state->driverInput.mouseY * config->sensitivity;

  float yawAngle = realizeYawEncoder(yawMotor.getAngle());
  float pitchAngle = realizePitchEncoder(pitchMotor.getAngle());

  PID_Filter(&config->yaw_PID, &state->gimbal.yaw_PID, yawAngle, deltaTime);
  PID_Filter(&config->pitch_PID, &state->gimbal.pitch_PID, pitchAngle, deltaTime);

  // Set motor power here?
  // yawMotor.setPower(config->yaw_PID->Y);
  // pitchMotor.setPower(config->pitch_PID->Y);
}

float Gimbal::realizeYawEncoder(float angle) {
    return (angle - config->yawOffset) * 0.5;
}

float Gimbal::realizePitchEncoder(float angle) {
    return (angle - config->pitchOffset);
}