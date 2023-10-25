import requests
import json

# Define the API endpoint URL
api_url = 'http://localhost:3000/add-entry'  # Replace with the correct URL

# Data to be sent in the request body
data = {
    'timestamp': 'yyyy-mm-dd-hh-mm-ss',
    'deviceId': 'ak-1',
    'fgmMeasure': '3-4-2',
}

# Set the headers for JSON data
headers = {'Content-Type': 'application/json'}

try:
    # Send the POST request
    response = requests.post(api_url, data=json.dumps(data), headers=headers)

    # Check the response status code
    if response.status_code == 200:
        print('Entry added successfully')
    else:
        print('Error:', response.status_code)
        print(response.text)

except requests.exceptions.RequestException as e:
    print('Request Exception:', e)

except Exception as e:
    print('An error occurred:', e)
