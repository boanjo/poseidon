########################################################################################
# Poseidon is licensed under GNU General Public License v3.0 or later
########################################################################################


from typing import List, Dict
from flask import Flask
from flask import render_template
from flask import request
import time
import calendar
import json
import re
import os
import datetime
import uuid
import logging
import click

from influxdb import InfluxDBClient
from urllib.parse import unquote
from flask_mqtt import Mqtt

from flask_socketio import SocketIO, emit, disconnect


app = Flask(__name__, static_url_path='/static')
app.config['TEMPLATES_AUTO_RELOAD'] = True
app.config['MQTT_BROKER_URL'] = 'mosquitto'
app.config['MQTT_BROKER_PORT'] = 1883
app.config['MQTT_USERNAME'] = ''
app.config['MQTT_PASSWORD'] = ''
app.config['MQTT_KEEPALIVE'] = 5
app.config['MQTT_TLS_ENABLED'] = False
app.config['MQTT_CLEAN_SESSION'] = True

log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

def secho(text, file=None, nl=None, err=None, color=None, **styles):
    pass

def echo(text, file=None, nl=None, err=None, color=None, **styles):
    pass

click.echo = echo
click.secho = secho


app.config['SECRET_KEY'] = 'secret!'
async_mode = None
# Set this variable to "threading", "eventlet" or "gevent" to test the
# different async modes, or leave it set to None for the application to choose
# the best option based on installed packages.
socketio = SocketIO(app, async_mode=async_mode)

mqtt = Mqtt(app)

host_name = "influxdb"

entry_dict = dict()
topics_dict = dict()

@socketio.event
def my_event(message):
    print(message)
    emit('my_response',
         {'data': message['data'], 'count': 1})


def request_acknowledge(data):
    seq_no  = data['seq_no']
    mqtt.publish("pc/water_control/in/ack_request", "{value:" + str(seq_no) + "}", qos=0)
    

@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    mqtt.subscribe('pc/#')

        
@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):
    
    topic=message.topic,
    payload=message.payload.decode('utf-8')

    socketio.emit('mqtt_msg', {'topic':topic, 'payload':payload})

@mqtt.on_log()
def handle_logging(client, userdata, level, buf):
    #print(level, buf)
    return

def get_values_for_topic(topic):
    query = "select min(value), max(value) from pond_control where topic='{0}' and time > now() - 24h;".format(topic)
    client = InfluxDBClient(host_name, 8086, "", "", "sensors")
    result = client.query(query)
    if len(result) > 0:
        for ele in result[None]:
            ret = dict()
            ret['topic'] = topic
            ret['min'] = ele['min']
            ret['max'] = ele['max']

            return ret
            
    return None

@app.route('/api_get_last_values_json')
def api_get_last_values_json():

    ret_list = []
    ret_list.append(get_values_for_topic("pc/water_quality/out/temp"))
    ret_list.append(get_values_for_topic("pc/water_quality/out/ph"))
    ret_list.append(get_values_for_topic("pc/water_quality/out/ec"))
    ret_list.append(get_values_for_topic("pc/water_quality/out/do"))
    ret_list.append(get_values_for_topic("pc/water_quality/out/orp"))
    ret_list.append(get_values_for_topic("pc/water_control/out/level"))
    ret_list.append(get_values_for_topic("pc/water_control/out/flow"))
    #ret_list.append(get_values_for_topic("pc/water_control/out/sprinkler1"))
    #ret_list.append(get_values_for_topic("pc/water_control/out/water"))

    return json.dumps(ret_list)

@app.route('/api_get_timers_json')
def api_get_timers_json():
    di = request.get_json() 

    select = ";"
    
    if di is not None and topic in di:
        select = "where topic='" + di[topic] + "';"
        
    
    ret_list = []
    query = "select topic,timeout,type,value,uuid,last_timeout,next_timeout from control" + select
    client = InfluxDBClient(host_name, 8086, "", "", "timers")
    result = client.query(query)
    if len(result) > 0:
        for ele in result[None]:
            ret = dict()
            ret['topic'] = ele['topic']
            ret['timeout'] = ele['timeout']
            ret['type'] = ele['type']
            ret['value'] = ele['value']
            ret['last_timeout'] = ele['last_timeout']
            ret['next_timeout'] = ele['next_timeout']
            ret['uuid'] = ele['uuid']

            ret_list.append(ret)
            
    done = time.time()
        
    return json.dumps(ret_list)

@app.route('/api_add_timer_json', methods = ['POST'])
def api_add_timer_json():
    timer = request.get_json() 


    print(f"timeout: {timer['timeout']}")

    now = datetime.datetime.now()
    tod = time.strptime(timer['timeout'], "%H:%M:%S")

    if 'daily' in timer['type']:
    
        tod_date = datetime.datetime(now.year, now.month, now.day, tod.tm_hour, tod.tm_min, tod.tm_sec)
        next_timeout = tod_date.timestamp()
        
        # If timer time is lower than current then we obviously have a timer for timeout tomorrow
        if now.timestamp() > next_timeout:
            next_timeout = (datetime.datetime(now.year, now.month, now.day, tod.tm_hour, tod.tm_min, tod.tm_sec) + datetime.timedelta(days=1)).timestamp()
    
    else:
        # we add whatever duration to current time (i.e. this is a stop timer, one off) 
        next_timeout = (now+datetime.timedelta(hours=tod.tm_hour, minutes=tod.tm_min, seconds=tod.tm_sec)).timestamp()

    client = InfluxDBClient(host_name, 8086, "", "", "timers")
    
    # First remove any exisitng timer to be replaced
    client.query("delete from control where uuid=\'" + timer['uuid'] + "\'", method="POST")
        
    data = []
    data.append("{measurement},timeout={timeout},type={type},last_timeout={last_timeout},next_timeout={next_timeout},topic={topic},uuid={uuid} value={value}"
                .format(measurement='control',
                        timeout=timer['timeout'],
                        type=timer['type'],
                        last_timeout=timer['last_timeout'],
                        next_timeout=next_timeout,
                        topic=timer['topic'],
                        uuid=uuid.uuid4(),
                        value=timer['value']))

    client.write_points(data, time_precision='ms', batch_size=10000, protocol='line')
    return json.dumps({'success':True}), 200, {'ContentType':'application/json'}


@app.route('/api_device_control_json', methods = ['POST'])
def api_device_control_json():
    data = request.get_json()
    if "value" in data:
        value = data['value']
        if "topic" in data:
            topic = data['topic']
            mqtt.publish(topic, "{value:" + str(value) + "}", qos=0)
            request_acknowledge(data)
    
    return json.dumps({'success':True}), 200, {'ContentType':'application/json'}
    
@app.route('/api_feed_json', methods = ['POST'])
def api_feed_json():
    value = 3
    data = request.get_json()
    if "value" in data:
        value = data['value']
    mqtt.publish("pc/fishfeeder/in/feed", "{value:" + str(value) + "}", qos=0)
    
    return json.dumps({'success':True}), 200, {'ContentType':'application/json'}
    

@app.route('/api_delete_timer_json', methods = ['POST'])
def api_delete_timer_json():
    timer = request.get_json() 

    client = InfluxDBClient(host_name, 8086, "", "", "timers")

    # First remove any exisitng timer to be replaced
    client.query("delete from control where uuid=\'" + timer['uuid'] + "\'", method="POST")
 
    return json.dumps({'success':True}), 200, {'ContentType':'application/json'}

@app.route('/timers')
def timers():
    di = request.get_json() 
    select = "All Devices"
    
    if di is not None and topic in di:
        select = di['topic']
    return render_template('timers.html',
                           title='Timers per device',
                           topic=select)
@app.route('/timer')
def timer():
    uuid = request.args.get('uuid')
    if uuid is None:
        uuid = ""       
    else:
        uuid = unquote(uuid)

    return render_template('timer.html',
                           title='Add/Edit/Delete timer',
                           uuid=uuid)

@app.route('/')
@app.route('/dashboard')
def dashboard():
    return render_template('dashboard.html',
                           title='Pond Control 2.0')

@app.route('/pond_no_bg')
def pond_no_bg():
    return render_template('pond_no_bg.html',
                           title='Pond Control 2.0')

if __name__ == '__main__':
    #app.run(host='0.0.0.0')
    socketio.run(app, host='0.0.0.0', port=5000, use_reloader=False, debug=False)

