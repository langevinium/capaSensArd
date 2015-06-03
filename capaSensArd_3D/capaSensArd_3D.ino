// capaSensArd
//  Arduino driver to "read" the value of a capacitive sensor.
//
//  This is a version dedicated to drive 3 sensors. The values
//  are delivered using a serial connexion in the following
//  format: "%d %d %d\n".
//
// # Original version
//     by Kyle McDonald
//     from the instructables project at:
//     http://www.instructables.com/id/DIY-3D-Controller/
// 
// # 3D version
//    by antoine.delhomme@espci.fr
//    for the langevinium.espci.fr
//


#define resolution 8
#define mains 50 // 60: north america, japan; 50: most other places

#define refresh 2 * 1000000 / mains

void setup() {
  Serial.begin(115200);

  // unused pins are fairly insignificant,
  // but pulled low to reduce unknown variables
  for(int i = 2; i < 14; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  // Define pins of the 3 sensors
  //  pin 8  --> sensor 1
  //  pin 9  --> sensor 2
  //  pin 10 --> sensor 3
  for(int i = 8; i < 11; i++)
    pinMode(i, INPUT);

  startTimer();
}

void loop() {  
  Serial.print(time(8, B00000001), DEC);
  Serial.print(" ");
  Serial.print(time(9, B00000010), DEC);
  Serial.print(" ");
  Serial.println(time(10, B00000100), DEC);

} 

long time(int pin, byte mask) {
  unsigned long count = 0, total = 0;
  while(checkTimer() < refresh) {
    // pinMode is about 6 times slower than assigning
    // DDRB directly, but that pause is important
    pinMode(pin, OUTPUT);
    PORTB = 0;
    pinMode(pin, INPUT);
    while((PINB & mask) == 0)
      count++;
    total++;
  }
  startTimer();
  return (count << resolution) / total;
}

extern volatile unsigned long timer0_overflow_count;

void startTimer() {
  timer0_overflow_count = 0;
  TCNT0 = 0;
}

unsigned long checkTimer() {
  return ((timer0_overflow_count << 8) + TCNT0) << 2;
}


