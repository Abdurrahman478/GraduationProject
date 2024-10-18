const int VRxPin = A0;  // X-axis analog pin
const int VRyPin = A1;  // Y-axis analog pin
const int SWPin = 2;    // Button switch pin

void setup() {
  // Initialize serial communication for monitoring the values
  Serial.begin(9600);

  // Set the button pin as an input with an internal pull-up resistor
  pinMode(SWPin, INPUT_PULLUP);
}

void loop() {
   // Read the analog values from the joystick
  int xValue = analogRead(VRxPin);  // X-axis value
  int yValue = analogRead(VRyPin);  // Y-axis value

  // Read the button state (LOW when pressed, HIGH when not pressed)
  int buttonState = digitalRead(SWPin);

  // Print the X and Y values to the serial monitor
  Serial.print("X-axis: ");
  Serial.print(xValue);
  Serial.print(" | Y-axis: ");
  Serial.print(yValue);
  
  // Check if the button is pressed
  if (buttonState == LOW) {
    Serial.println(" | Button Pressed");
  } else {
    Serial.println(" | Button Not Pressed");
  }

  delay(100);  // Add a small delay to make the output more readable

}
