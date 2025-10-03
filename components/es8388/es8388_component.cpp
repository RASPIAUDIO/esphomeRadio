
#include "es8388_component.h"
#include "esphome/core/hal.h"
#include <soc/io_mux_reg.h>

namespace esphome {
namespace es8388 {

void ES8388Component::setup() {
//   PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
//  WRITE_PERI_REG(PIN_CTRL, READ_PERI_REG(PIN_CTRL) & 0xFFFFFFF0);
  //reset
  this->write_byte(0, 0x80);
  this->write_byte(0, 0x00);  
  // mute
  this->write_byte(0x19, 0x04);
  // powerup
  this->write_byte(0x01, 0x50);
  this->write_byte(0x02, 0x00);
  // worker mode
  this->write_byte(0x08, 0x00);
  // DAC powerdown
  this->write_byte(0x04, 0xC0);
  // vmidsel/500k ADC/DAC idem
  this->write_byte(0x00, 0x12);

  // i2s 16 bits
  this->write_byte(0x17, 0x18);
  // sample freq 256
  this->write_byte(0x18, 0x02);
  // LIN2/RIN2 for mixer
  this->write_byte(0x26, 0x00);
  // left DAC to left mixer
  this->write_byte(0x27, 0x90);
  // right DAC to right mixer
  this->write_byte(0x2A, 0x90);
  // DACLRC ADCLRC idem
  this->write_byte(0x2B, 0x80);
  this->write_byte(0x2D, 0x00);
  // DAC volume max
  this->write_byte(0x1B, 0x00);
  this->write_byte(0x1A, 0x00);

 
 //mono (L+R)/2
    this->write_byte(29, 0x20);
 //Right DAC phase inversion
    this->write_byte(28, 0x10);
 // ADC poweroff
   this->write_byte(3,0xFF);
   // ADC micboost 18 dB
   this->write_byte(9,0x66);   

   // differential, stereo,  Right => LINPUT1/RINPUT1  , Left => LINPUT2/RINPUT2
   this->write_byte(10,0xFC);  
   this->write_byte(11,0x02);

   //i2S 16b
   this->write_byte(12,0x0C); 
   //MCLK 256
   this->write_byte(13,0x02); 
   // ADC high pass filter
   this->write_byte(14,0x30);    
   // ADC attenuation 0 dB   
   this->write_byte(16,0x00);
   this->write_byte(17,0x00);
/*
//ALC on PGA max Gain Max 11.5 dB Gain Min 0
   this->write_byte(18, 0xDA);
   //ALC Target 4.5dB hold time 0
   this->write_byte(19, 0xA0);
   //Decay time 820us Attack time 416 us
   this->write_byte(20, 0x12);  
   this->write_byte(21, 0x06);   
*/
   // noise gate -76.5dB
  this->write_byte(22, 0x03);      
 //ADC power  
   this->write_byte(3,0x00);
  this->write_byte(0x02, 0xF0);
  delay(1);
  this->write_byte(0x02, 0x00);
  // DAC power-up LOUT1/ROUT1 enabled
  this->write_byte(0x04, 0x30);
  this->write_byte(0x03, 0x00);
  // DAC volume max
  this->write_byte(0x2E, 0x21); // LOUT1/ROUT1
  this->write_byte(0x2F, 0x21);
  this->write_byte(0x30, 0x21); // LOUT2/ROUT2
  this->write_byte(0x31, 0x21);  
  // unmute
  this->write_byte(0x19, 0x00);
}
void ES8388Component::set_volume(float volume) {
  // Limitez le volume entre 0.0 et 1.0
  if (volume < 0.0f) volume = 0.0f;
  if (volume > 1.0f) volume = 1.0f;

  // Convertissez le volume en une valeur comprise entre 0x00 et 0xFF
  uint8_t volume_value = static_cast<uint8_t>(volume * 32);

  // Écrire dans les registres du DAC pour définir le volume
  this->write_byte(0x2E, volume_value); // Volume gauche
  this->write_byte(0x2F, volume_value); // Volume droit
}


SetVolumeAction *ES8388Component::make_set_volume_action() {
  return new SetVolumeAction(this);
}

}  // namespace es8388
}  // namespace esphome
