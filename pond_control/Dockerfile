FROM python:3

WORKDIR /app

COPY ./main.py ./
COPY ./requirements.txt ./
#RUN pip install --no-cache-dir -r requirements.txt
RUN pip install -r requirements.txt

CMD [ "python", "-u", "main.py" ]