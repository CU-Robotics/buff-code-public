#include "../state/config.h"
#include "../state/state.h"

class Subsystem {
  public:
    Subsystem();
    void setup(C_Config *config, S_Robot *state);
    void update(float deltaTime);

  private:
    C_Config *config;
    S_Robot *state;
};