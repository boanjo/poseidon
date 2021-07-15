FROM python:3

WORKDIR /app

COPY requirements.txt /app
RUN pip install -r requirements.txt

RUN mkdir -p /app/templates && mkdir -p /app/static/images
COPY . /app
CMD python -u app.py
