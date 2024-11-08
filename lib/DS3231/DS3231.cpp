#include "DS3231.h"

#define DS3231_ADDRESS 0x68   ///< I2C address for DS3231
#define DS3231_TIME 0x00      ///< Time register
#define DS3231_ALARM1 0x07    ///< Alarm 1 register
#define DS3231_ALARM2 0x0B    ///< Alarm 2 register
#define DS3231_CONTROL 0x0E   ///< Control register
#define DS3231_STATUSREG 0x0F ///< Status register
#define DS3231_TEMPERATUREREG                                                  \
  0x11 ///< Temperature register (high byte - low byte is at 0x12), 10-bit
///< temperature value


/**************************************************************************/
/*!
    @brief Write value to register.
    @param reg register address
    @param val value to write
*/
/**************************************************************************/
void RTC_I2C::write_register(uint8_t reg, uint8_t val) {
  uint8_t buffer[2] = { reg, val };
  i2c_dev->write(buffer, 2);
}

/**************************************************************************/
/*!
    @brief Read value from register.
    @param reg register address
    @return value of register
*/
/**************************************************************************/
uint8_t RTC_I2C::read_register(uint8_t reg) {
  uint8_t buffer[1];
  i2c_dev->write(&reg, 1);
  i2c_dev->read(buffer, 1);
  return buffer[0];
}

/**************************************************************************/
/*!
    @brief  Start I2C for the DS3231 and test succesful connection
    @param  wireInstance pointer to the I2C bus
    @return True if Wire can find DS3231 or false otherwise.
*/
/**************************************************************************/
bool RTC_DS3231::begin(TwoWire* wireInstance) {
  if (i2c_dev)
    delete i2c_dev;
  i2c_dev = new I2CDevice(DS3231_ADDRESS, wireInstance);
  if (!i2c_dev->begin())
    return false;
  return true;
}

bool RTC_DS3231::begin(TwoWire* wireInstance, bool newBegin) {
  i2c_dev = new I2CDevice(DS3231_ADDRESS, wireInstance);
  return true;
}

/**************************************************************************/
/*!
    @brief  Check the status register Oscillator Stop Flag to see if the DS3231
   stopped due to power loss
    @return True if the bit is set (oscillator stopped) or false if it is
   running
*/
/**************************************************************************/
bool RTC_DS3231::lostPower(void) {
  return read_register(DS3231_STATUSREG) >> 7;
}

/**************************************************************************/
/*!
    @brief  Set the date and flip the Oscillator Stop Flag
    @param dt DateTime object containing the date/time to set
*/
/**************************************************************************/
void RTC_DS3231::adjust(const DateTime& dt) {
  uint8_t buffer[8] = { DS3231_TIME,
                       bin2bcd(dt.second()),
                       bin2bcd(dt.minute()),
                       bin2bcd(dt.hour()),
                       bin2bcd(dowToDS3231(dt.dayOfTheWeek())),
                       bin2bcd(dt.day()),
                       bin2bcd(dt.month()),
                       bin2bcd(dt.year() - 2000U) };
  i2c_dev->write(buffer, 8);

  uint8_t statreg = read_register(DS3231_STATUSREG);
  statreg &= ~0x80; // flip OSF bit
  write_register(DS3231_STATUSREG, statreg);
}

/**************************************************************************/
/*!
    @brief  Get the current date/time
    @return DateTime object with the current date/time
*/
/**************************************************************************/
DateTime RTC_DS3231::now() {
  uint8_t buffer[7];
  buffer[0] = 0;
  i2c_dev->write_then_read(buffer, 1, buffer, 7);

  return DateTime(bcd2bin(buffer[6]) + 2000U, bcd2bin(buffer[5] & 0x7F),
    bcd2bin(buffer[4]), bcd2bin(buffer[2]), bcd2bin(buffer[1]),
    bcd2bin(buffer[0] & 0x7F));
}

/**************************************************************************/
/*!
    @brief  Disable alarm
        @param 	alarm_num Alarm number to disable
*/
/**************************************************************************/
void RTC_DS3231::disableAlarm(uint8_t alarm_num) {
  uint8_t ctrl = read_register(DS3231_CONTROL);
  ctrl &= ~(1 << (alarm_num - 1));
  write_register(DS3231_CONTROL, ctrl);
}

/**************************************************************************/
/*!
    @brief  Disable 32KHz Output
*/
/**************************************************************************/
void RTC_DS3231::disable32K(void) {
  uint8_t status = read_register(DS3231_STATUSREG);
  status &= ~(0x1 << 0x03);
  write_register(DS3231_STATUSREG, status);
}

ShortDate RTC_DS3231::now_shortd() {
  uint8_t buffer[7];
  buffer[0] = 0;
  i2c_dev->write_then_read(buffer, 1, buffer, 7);

  return ShortDate(bcd2bin(buffer[6]), bcd2bin(buffer[5] & 0x7F), bcd2bin(buffer[4]));
}

ShortTime RTC_DS3231::now_shortt() {
  uint8_t buffer[7];
  buffer[0] = 0;
  i2c_dev->write_then_read(buffer, 1, buffer, 7);

  DateTime(bcd2bin(buffer[6]) + 2000U, bcd2bin(buffer[5] & 0x7F),
    bcd2bin(buffer[4]), bcd2bin(buffer[2]), bcd2bin(buffer[1]),
    bcd2bin(buffer[0] & 0x7F));


  return ShortTime(bcd2bin(buffer[2]), bcd2bin(buffer[1]), bcd2bin(buffer[0] & 0x7F));
}


/**************************************************************************/
/*!
    @brief  Get the current temperature from the DS3231's temperature sensor
    @return Current temperature (float)
*/
/**************************************************************************/
float RTC_DS3231::getTemperature() {
  uint8_t buffer[2] = { DS3231_TEMPERATUREREG, 0 };
  i2c_dev->write_then_read(buffer, 1, buffer, 2);
  return (float)buffer[0] + (buffer[1] >> 6) * 0.25f;
}

void RTC_DS3231::disableContents() {
  disableAlarm(1);
  disableAlarm(2);

  disable32K();
}