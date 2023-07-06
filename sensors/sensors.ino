
#define RED_LED_PIN 7
#define GREEN_LED_PIN 5
#define YELLOW_LED_PIN 10
#define SOUND_SENSOR_INPUT_PIN A4
#define PRESENCE_SENSOR_INPUT_PIN 4
#define TERMISTOR_INPUT_PIN A5

// Resistência nominal a 25C (MF52 -> 1kOhm)
#define NOMINAL_TERM 1000      
// Temperatura na qual é feita a medida nominal (25C)
#define NOMINAL_TEMP 25   
// Quantas amostras para calcular a tensão média
#define SAMPLES 4
// Coeficiente Beta (da equacao de Steinhart-Hart) do termistor
#define BETA 3100
// Resistência do resistor
#define RESISTENCE 470  

void setup(){
  Serial.begin(9600);
  pinMode(RED_LED_PIN, OUTPUT); // Set the LED pin as an output
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(SOUND_SENSOR_INPUT_PIN, INPUT); // Set the sensor pin as an input
  pinMode(PRESENCE_SENSOR_INPUT_PIN, INPUT); // Set the sensor pin as an input
  Serial.setTimeout(1);
}

float get_temp() {
  float tension = 0;
  float temperature, termistor_resistence;
  
  for (int i=0; i< SAMPLES; i++) {
   tension += analogRead(TERMISTOR_INPUT_PIN) / SAMPLES;
   delay(10);
  }

  //Calculando a resistencia do Termistor
  termistor_resistence = RESISTENCE*tension/(1023-tension);
  //Equacao de Steinhart-Hart
  temperature = (1 / (log(termistor_resistence/NOMINAL_TERM) * 1/BETA + 1/(NOMINAL_TEMP + 273.15))) - 273.15;
  return temperature + 11;
}

int check_presence() {
  int presence = digitalRead(PRESENCE_SENSOR_INPUT_PIN);
  return presence == HIGH ? 1 : 0;
}

int check_sound() {
  int val_analog = analogRead(SOUND_SENSOR_INPUT_PIN);
  return val_analog == HIGH ? 1 : 0;
}

void loop(){
  float temperature;
  int presence, sound, response;

  temperature = get_temp();
  presence = check_presence();
  sound = check_sound();

  Serial.println(temperature);
  Serial.println(presence);
  Serial.println(sound);
  while(!Serial.available());
  delay(2000);

  response = Serial.readString().toInt();

  switch(response) {
    case 1:
      digitalWrite(RED_LED_PIN, HIGH);
      digitalWrite(YELLOW_LED_PIN, LOW);
      digitalWrite(GREEN_LED_PIN, LOW);
      break;
    case 2:
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(YELLOW_LED_PIN, HIGH);
      digitalWrite(GREEN_LED_PIN, LOW);
      break;
    case 3:
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(YELLOW_LED_PIN, LOW);
      digitalWrite(GREEN_LED_PIN, HIGH);
      break;
    default:
      break;
  }
}