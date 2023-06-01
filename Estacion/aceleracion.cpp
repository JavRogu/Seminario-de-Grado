#include <Arduino.h>
#include "aceleracion.h"

float calcularAceleracion(int pin, float sensibilidad, int escala) {
  float valorAnalogico = analogRead(pin);
  float aceleracion = (valorAnalogico / escala) * sensibilidad;
  return aceleracion;
}
