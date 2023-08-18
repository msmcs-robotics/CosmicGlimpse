import requests
import time
from datetime import datetime
import random
import json

# Replace this URL with your actual API endpoint
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
    while True:
        value = random.uniform(1, 10)  # Generate a random value between 1 and 10
        send_sensor_data(value)

        time.sleep(1)  # Wait for 1 second

if __name__ == "__main__":
    main()
