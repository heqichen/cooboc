#ifndef BEAGLEBONEBLACK_GPIO_SRC_GPIO_GPIOMANAGER_H_
#define BEAGLEBONEBLACK_GPIO_SRC_GPIO_GPIOMANAGER_H_

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#include <vector>

namespace GPIO {

enum DIRECTION {
  INPUT = 0,
  OUTPUT = 1
};

enum PIN_VALUE {
  LOW = 0,
  HIGH = 1
};

enum EDGE_VALUE {
  NONE = 0,
  RISING = 1,
  FALLING = 2,
  BOTH = 3
};

class GPIOManager {
 public:
  GPIOManager();
  virtual ~GPIOManager();

  static GPIOManager* getInstance();
  int exportPin(unsigned int gpio);
  int unexportPin(unsigned int gpio);
  int setDirection(unsigned int gpio, DIRECTION direction);
  int getDirection(unsigned int gpio);
  int setValue(unsigned int gpio, PIN_VALUE value);
  int getValue(unsigned int gpio);
  int setEdge(unsigned int gpio, EDGE_VALUE value);
  int getEdge(unsigned int gpio);
  int waitForEdge(unsigned int gpio, EDGE_VALUE value);
  int countExportedPins();
  void clean();
 private:
  static GPIOManager* instance;
  std::vector<unsigned int> exportedPins;
};

} /* namespace GPIO */
#endif  // BEAGLEBONEBLACK_GPIO_SRC_GPIO_GPIOMANAGER_H_
