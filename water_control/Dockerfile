FROM python:3

WORKDIR /app

COPY ./serial_2_mqtt.py ./
COPY ./requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt


CMD [ "python", "-u", "serial_2_mqtt.py" ]