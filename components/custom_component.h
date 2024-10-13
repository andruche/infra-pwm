#include "esphome.h"

class InfraPWM : public Component, public Switch, public FloatOutput {
 public:
  void setup() override {
    // Начальная настройка пина
    pinMode(pin_, OUTPUT);
    digitalWrite(pin_, LOW); // Нагрузка выключена при старте
    last_toggle_time_ = millis(); // Стартовое время
  }

  void write_state(float state) override {
    // Получаем уровень мощности от 0 до 100
    this->duty_cycle_ = state;
  }

  void loop() override {
    // Получаем текущее время
    unsigned long current_time = millis();

    // Рассчитываем длительность в миллисекундах
    unsigned long cycle_time = 100000; // 100 секунд в миллисекундах
    unsigned long on_time = (cycle_time * this->duty_cycle_) / 100;
    unsigned long off_time = cycle_time - on_time;

    // Логика включения/выключения на основе текущего времени
    if (this->state) {  // Проверяем, если нагрузка включена
      if (digitalRead(pin_) == HIGH && current_time - last_toggle_time_ >= on_time) {
        digitalWrite(pin_, LOW);  // Выключаем нагрузку
        last_toggle_time_ = current_time;
      } else if (digitalRead(pin_) == LOW && current_time - last_toggle_time_ >= off_time) {
        digitalWrite(pin_, HIGH); // Включаем нагрузку
        last_toggle_time_ = current_time;
      }
    } else {
      digitalWrite(pin_, LOW); // Если нагрузка выключена, держим её в выключенном состоянии
    }
  }

  void write_state(bool state) override {
    // Включаем или выключаем компонент
    this->state = state;
    digitalWrite(pin_, state ? HIGH : LOW); // Включаем/выключаем нагрузку
  }

  void set_pin(int pin) { pin_ = pin; }

 protected:
  int pin_;
  float duty_cycle_ = 0;
  unsigned long last_toggle_time_;
};
