#ifndef BEAGLEBONEBLACK_GPIO_SRC_GPIO_GPIOCONST_H_
#define BEAGLEBONEBLACK_GPIO_SRC_GPIO_GPIOCONST_H_

namespace GPIO {

typedef struct pins_t {
  const char *name;
  const char *key;
  unsigned int gpio;
  int pwm_mux_mode;
  int ain;
  int isAllocatedByDefault;
} pins_t;

class GPIOConst {
 public:
  GPIOConst();
  virtual ~GPIOConst();

  static GPIOConst* getInstance();
  int getGpioByKey(const char *key);
  int getGpioByName(const char *name);
  const char* getGpioNameByPin(unsigned int pin);
  const char* getGpioKeyByPin(unsigned int pin);
  const char* getEdgeValueByIndex(unsigned int index);
  int getEdgeIndexByValue(const char* value);
  int isPinAllocatedByDefault(unsigned int pin);

 private:
  static pins_t pinTable[97];
  static const char* strEdge[4];
  static GPIOConst* instance;
};

} /* namespace GPIO */
#endif  // BEAGLEBONEBLACK_GPIO_SRC_GPIO_GPIOCONST_H_
