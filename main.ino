#define ROTATE A5
#define YYY A6
#define XXX A7
#define BUTTON 2


uint16_t rotationZero;
int8_t X, Y, R, B;
String pkg;


bool get_but() {
    return !bool(digitalRead(BUTTON));
}

int8_t get_rot() {
    int16_t value = analogRead(ROTATE);

    if (value < rotationZero) value = map(value, rotationZero, 0, 0, -100);
    else                      value = map(value, rotationZero, 600, 0, 100);

    return constrain(abs(value) < 30 ? 0 : value, -100, 100);
}

uint16_t get_rot_zero() {
    uint32_t rzsum = 0;
    for(int i=0; i<100; ++i) rzsum += analogRead(ROTATE);
    return (int)rzsum / 100;
}

int8_t get_xxx() {
    int8_t value = map(analogRead(XXX), 0, 1023, -100, 100);

    return constrain(abs(value) < 10 ? 0 : value, -100, 100);
}

int8_t get_yyy() {
    int8_t value = map(analogRead(YYY), 0, 1023, -100, 100);

    return constrain(abs(value) < 10 ? 0 : value, -100, 100);
}

void push_data() {
    pkg = String(B) + String(R + 200) + String(X + 200) + String(Y + 200) + "!";
    Serial.print(pkg);
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON, INPUT_PULLUP);
    
    rotationZero = get_rot_zero();
}

void loop() {
  X = get_xxx(), 
  Y = get_yyy(),
  R = get_rot(),
  B = get_but();

  push_data();

  delay(20);
}
