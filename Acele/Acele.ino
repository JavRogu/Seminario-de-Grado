//Definicion de los pines ADC
const int x = 33;
const int y = 34;
const int z = 25;

//Variables para la lectura de los pines adc
int lc_x;
int lc_y;
int lc_z;

void setup() {
  Serial.begin(115200);  //comunicacion serial en baudios
  }

void loop() {
  //Lectura de los ADC
  lc_x = analogRead(x);
  lc_y = analogRead(y);
  lc_z = analogRead(z);

  //impresion de cada variable en monitor serial
  Serial.print(lc_x);
  Serial.print("\t");
  Serial.println(lc_y);
  Serial.print("\t");
  Serial.println(lc_z);
  delay(50);
}
