import requests
import time
from datetime import datetime
import json

# Replace these URLs with your actual API endpoints
base_url = "http://127.0.0.1:5000"
sensor_data_url = f"{base_url}/api/receive-sensor-data"

def send_sensor_data(value):
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    sensor_data = {
        "value": value,
        "timestamp": timestamp
    }

    response = requests.post(sensor_data_url, json=sensor_data)

    print("Sent: ", sensor_data)
    print("Response status code: ", response.status_code)
    
    try:
        response_json = response.json()
        print("Response JSON: ", response_json)
    except json.JSONDecodeError:
        print("Response content: ", response.content)
    
    print()

def main():
    value = 9
    increasing = True

    while True:
        send_sensor_data(value)

        if increasing:
            value += 0.1
        else:
            value -= 0.1

        time.sleep(1)  # Wait for 1 second

        if value >= 10:
            increasing = False  # After 10 seconds, start decreasing
        elif value <= 8:
            increasing = True

if __name__ == "__main__":
    main()
