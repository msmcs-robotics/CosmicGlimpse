from flask import Flask, request, render_template, jsonify
from database import db, SensorData, create_tables
from flask_migrate import Migrate

import json

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///data.db'
app_host = "0.0.0.0"
app_port = 5000
db_path = "./instance/data.db"
db.init_app(app)
migrate = Migrate(app, db)

# Create the tables if the database file doesn't exist
create_tables(app, db_path)

@app.route('/api/receive-sensor-data', methods=['POST'])
def receive_sensor_data():
    data = request.json
    sensor_data = SensorData(value=data['value'], timestamp=data['timestamp'])
    db.session.add(sensor_data)
    db.session.commit()
    return jsonify(message='Data received successfully')

@app.route('/api/get-sensor-data', methods=['GET'])
def get_sensor_data():
    num_data_points = int(request.args.get('num_data_points', -1))
    
    if num_data_points > 0:
        sensor_data = SensorData.query.order_by(SensorData.id.desc()).limit(num_data_points).all()
    else:
        sensor_data = SensorData.query.all()
    
    data = [{"value": entry.value, "timestamp": entry.timestamp} for entry in sensor_data]
    return jsonify(data)

@app.route('/api/get-sensor-data-count', methods=['GET'])
def get_sensor_data_count():
    count = SensorData.query.count()
    return jsonify({"count": count})

@app.route('/')
def index():
    return render_template('index.html')

if __name__ == '__main__':
    app.run(host=app_host, port=app_port)
