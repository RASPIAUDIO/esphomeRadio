#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"
#include "esphome/core/automation.h"
namespace esphome {
namespace es8388 {
class SetVolumeAction;
class ES8388Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void set_volume(float volume);
  SetVolumeAction *make_set_volume_action();
};
class SetVolumeAction : public Action<> {
 public:
  explicit SetVolumeAction(ES8388Component *parent) : parent_(parent) {}
  
  void set_volume(float volume) { this->volume_ = volume; }

  void play() override {
    if (this->parent_ != nullptr) {
      this->parent_->set_volume(this->volume_);
    }
  }

 protected:
  ES8388Component *parent_;
  float volume_{0.0f};
};

}  // namespace es8388
}  // namespace esphome
