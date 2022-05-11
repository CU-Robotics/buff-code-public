#include "state/state.h"
#include "state/config.h"
#include "swerveModule.h"

#ifndef SWERVE_CHASSIS_H
#define SWERVE_CHASSIS_H

class SwerveChassis {
 public:
    SwerveChassis();
    void setup(C_SwerveChassis *data, S_Robot *r_state);
    void update(float deltaTime);

  private:
    S_Robot *state;
    C_SwerveChassis *config;

    SwerveModule moduleFR;
    SwerveModule moduleFL;
    SwerveModule moduleBL;
    SwerveModule moduleBR;

    float drivebaseConstant;

    void calibrate();
    void drive(float driveX, float driveY, float spin, float deltaTime);
    void driveSimple(float driveX, float driveY, float deltaTime);
    float radiansToDegrees(float radians);
};

#endif // SWERVE_CHASSIS_H