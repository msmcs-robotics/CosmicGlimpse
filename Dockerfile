FROM python:3.8

WORKDIR /app

COPY requirements.txt requirements.txt
RUN pip install -r requirements.txt

COPY app app

# the actuall flask app is in ./app
ENV FLASK_APP=app/app.py

CMD ["flask", "run"]
