#include "ThingSpeakManager.h" // Include the header file for the ThingSpeakManager class

// Setup method to initialize ThingSpeak
void ThingSpeakManager::setup() {
  ThingSpeak.begin(client); // Initialize ThingSpeak with the client object
}

// Method to send sensor data to ThingSpeak
void ThingSpeakManager::sendData(SensorManager& sensorManager) {
  Serial.println("Sending data to ThingSpeak..."); // Print a message indicating that data is being sent

  // Set the fields with the sensor data
  ThingSpeak.setField(1, sensorManager.temperature); // Set field 1 to temperature
  ThingSpeak.setField(2, sensorManager.humidity); // Set field 2 to humidity
  ThingSpeak.setField(3, sensorManager.light); // Set field 3 to light intensity
  ThingSpeak.setField(4, sensorManager.moisture); // Set field 4 to soil moisture

  // Write the fields to the ThingSpeak channel
  int x = ThingSpeak.writeFields(channelNumber, apiKey); // Write the data to ThingSpeak and store the response code

  // Check the response code
  if (x == 200) { // If the response code is 200, data was sent successfully
    Serial.println("Data sent to ThingSpeak"); // Print a success message
  } else { // If the response code is not 200, there was an error
    Serial.println("Error sending data to ThingSpeak"); // Print an error message
  }
}

// Method to send summary sensor data to ThingSpeak
void ThingSpeakManager::sendSummaryData(SensorManager& sensorManager) {
  Serial.println("Sending summary data to ThingSpeak..."); // Print a message indicating that summary data is being sent

  // Set the fields with the summary sensor data
  ThingSpeak.setField(5, sensorManager.temperature); // Set field 5 to temperature
  ThingSpeak.setField(6, sensorManager.humidity); // Set field 6 to humidity
  ThingSpeak.setField(7, sensorManager.light); // Set field 7 to light intensity
  ThingSpeak.setField(8, sensorManager.moisture); // Set field 8 to soil moisture

  // Write the fields to the ThingSpeak channel
  int x = ThingSpeak.writeFields(channelNumber, apiKey); // Write the summary data to ThingSpeak and store the response code

  // Check the response code
  if (x == 200) { // If the response code is 200, summary data was sent successfully
    Serial.println("Summary data sent to ThingSpeak"); // Print a success message
  } else { // If the response code is not 200, there was an error
    Serial.println("Error sending summary data to ThingSpeak"); // Print an error message
  }
}
