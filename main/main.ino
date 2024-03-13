#include <Arduino.h>
#define ROTATE A5
#define YYY A6
#define XXX A7
#define BUTTON 2


uint16_t rotationZero;
int8_t X, Y, R, B;
String pkg;

class LowPassFilter {
  public:
    LowPassFilter(float alpha) {
        this->alpha = alpha;
        prevValue = 0;
    }
    
    float filter(float newValue) {
        float filteredValue = alpha * newValue + (1 - alpha) * prevValue;
        prevValue = filteredValue;
        return filteredValue;
    }

  private:
    float alpha;
    float prevValue;
};

class Joystic4DOF {
    public:
    Joystic4DOF(const uint8_t rotPin, const uint8_t xxxPin,  const uint8_t yyyPin, const uint8_t butPin) {
        this->xxxPin = xxxPin;
        this->yyyPin = yyyPin;
        this->rotPin = rotPin;
        this->butPin = butPin;
    }

    bool   get_but() {
        return !bool(digitalRead(butPin));
    } 

    int8_t get_rot() {
        int16_t value = analogRead(rotPin);

        if (value < rotationZero) value = map(value, rotationZero, 0, 0, -100);
        else                      value = map(value, rotationZero, 600, 0, 100);

        return constrain(abs(value) < 30 ? 0 : value, -100, 100);
    }

    int8_t get_xxx() {
        int8_t value = map(analogRead(xxxPin), 0, 1023, -100, 100);
        return constrain(abs(value) < 10 ? 0 : value, -100, 100);
    }

    int8_t get_yyy() {
        int8_t value = map(analogRead(YYY), 0, 1023, -100, 100);
        return constrain(abs(value) < 10 ? 0 : value, -100, 100);
}

    uint16_t get_rot_zero() {
        uint32_t rzsum = 0;
        for(uint8_t i=0; i<100; ++i) rzsum += analogRead(ROTATE);
        return (uint16_t)rzsum / 100;
    }

    private:
        uint8_t xxxPin;
        uint8_t yyyPin;
        uint8_t rotPin;
        uint8_t butPin;
};


void push_data() {
    pkg = String(B) + String(R + 200) + String(X + 200) + String(Y + 200) + "!";
    Serial.print(pkg);
}

Joystic4DOF jst4dof(ROTATE, XXX, YYY, BUTTON);
LowPassFilter lpfX(0.2);
LowPassFilter lpfY(0.2);
LowPassFilter lpfR(0.2);
void setup() {
    Serial.begin(115200);
    pinMode(BUTTON, INPUT_PULLUP);
    
    rotationZero = jst4dof.get_rot_zero();
}

void loop() {
  X = lpfX.filter(jst4dof.get_xxx()), 
  Y = lpfY.filter(jst4dof.get_yyy()),
  R = lpfR.filter(jst4dof.get_rot()),
  B =            (jst4dof.get_but());

  push_data();

  delay(20);
}
