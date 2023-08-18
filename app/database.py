from flask_sqlalchemy import SQLAlchemy
import os

db = SQLAlchemy()

class SensorData(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    value = db.Column(db.Float, nullable=False)
    timestamp = db.Column(db.String, nullable=False)

def create_tables(app, db_path):
    with app.app_context():
        if not os.path.exists(db_path):
            db.create_all()
