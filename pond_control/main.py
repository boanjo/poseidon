########################################################################################
# Poseidon is licensed under GNU General Public License v3.0 or later
########################################################################################


import time
import uuid
import paho.mqtt.client as mqtt
from influxdb import InfluxDBClient
from influxdb.client import InfluxDBClientError
import datetime
import json
import math
from requests.exceptions import ConnectionError

db = dict()
db['sensors'] = "sensors"
db['devices'] = "devices"
db['timers'] = "timers"

prev_value_dict = dict()

host = "influxdb"

def is_sensor(json_dict):
    
    #if "atlas" in topic or "flow" in topic or "level" in topic:
    if "type" in json_dict:
        if "state" in json_dict['type']:
            return False
    return True


def is_worth_storing_to_db(topic, value):       
    if topic in prev_value_dict:
        if math.fabs(prev_value_dict[topic] - value) > 0.1:
            prev_value_dict[topic] = value
            return True
    else:
        prev_value_dict[topic] = value
        return True

    return False

def on_connect(mqttc, obj, flags, rc):
    print("rc: " + str(rc))


def on_message(mqttc, obj, msg):

    topic = msg.topic
    payload = msg.payload.decode('utf-8')

    try:
        json_dict = json.loads(payload)
    except json.decoder.JSONDecodeError:
        print(f"Skipping strange payload: {payload}")
        return

    if "value" not in json_dict:
        return 


    #print(topic + " " + payload)
    
    data = []
    if is_sensor(json_dict):
        client = get_db_client('sensors')
        data.append("{measurement},topic={topic} value={value}"
                    .format(measurement='pond_control',
                            topic=topic,
                            value=json_dict['value']))
        client.write_points(data, database='sensors', time_precision='ms', batch_size=10000, protocol='line')
    elif is_worth_storing_to_db(topic, json_dict['value']):

        client = get_db_client('devices')
        data.append("{measurement},topic={topic} value={value}"
                    .format(measurement='pond_control',
                            topic=topic,
                            value=json_dict['value']))
        client.write_points(data, database='devices', time_precision='ms', batch_size=10000, protocol='line')
    

def on_publish(mqttc, obj, mid):
    return
    

def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))


def on_log(mqttc, obj, level, string):
    print(string)
    
def on_disconnect(mqttc, userdata, rc):
    print("disconnecting reason  "  +str(rc))

def check_for_timeout(mqttc, timer):
        
    if 'timeout' in timer and 'next_timeout' in timer and 'type' in timer:
        now = datetime.datetime.now()
        next_timeout = float(timer['next_timeout'])
         
        #print(str(now) + " " + timer['timeout'] + " " + str(now.timestamp()) + " " + str(tod_timestamp))
        if now.timestamp() > next_timeout:

            # Time to send action
            print("Timer expired! {0}".format(timer))
            mqttc.publish(timer['topic'], "{value:" + str(int(timer['value'])) + "}", qos=0)
                            
            if 'daily' in timer['type']:
                #restart timer or republish in database with updated last timeout
                timer['last_timeout'] = now.timestamp()

                # Add one day to current date + the timout setting fot the timer
                tod = time.strptime(timer['timeout'], "%H:%M:%S")
                tod_date = datetime.datetime(now.year, now.month, now.day, tod.tm_hour, tod.tm_min, tod.tm_sec)
                timer['next_timeout'] = (datetime.datetime(now.year, now.month, now.day,
                                                           tod.tm_hour, tod.tm_min,
                                                           tod.tm_sec) + datetime.timedelta(days=1)).timestamp()
                    
                data = []
                data.append("{measurement},timeout={timeout},type={type},last_timeout={last_timeout},next_timeout={next_timeout},topic={topic},uuid={uuid} value={value}"
                            .format(measurement='control',
                                    timeout=timer['timeout'],
                                    type=timer['type'],
                                    last_timeout=timer['last_timeout'],
                                    next_timeout=timer['next_timeout'],
                                    topic=timer['topic'],
                                    uuid=uuid.uuid4(),
                                    value=timer['value']))

                client.write_points(data, database='timers', time_precision='ms', batch_size=10000, protocol='line')

            # Delete old entry 
            if 'daily' in timer['type'] or 'one_off' in timer['type']:
                tags=dict()
                tags['uuid']=timer['uuid']
                client.query("delete from control where uuid=\'" + timer['uuid'] + "\'", method="POST")
    




def get_db_client(dbname):
    while True:
        try:
            client = InfluxDBClient(host, 8086, "", "", dbname)
            health = client.ping()
            #print(f"PING result: {health}")
            return client
        except ConnectionError as e:
            print(f"Connection failure: {dbname}!")
            print(e)
            time.sleep(1)
    
                                
def init_sensors_db(dbname):

    client = get_db_client(dbname)
    client.create_database(dbname)

    print("Create a retention policy for db {0}".format(dbname))
    retention_policy = 'detailed_3d'
    # Set a 1 week retention storing in 1d 'blocks' (shards), replication 1, as the default RP
    client.create_retention_policy(retention_policy, '3d', 1, default=True, shard_duration=u'1d')
    client.drop_retention_policy('autogen', database=dbname)

    downsample_db = f'{dbname}_downsampled'
    client.create_database(downsample_db)
    select_clause = f'SELECT mean(*) INTO "{downsample_db}"."autogen".:MEASUREMENT FROM "{dbname}"."{retention_policy}"."pond_control" GROUP BY time(30m),*'
    client.create_continuous_query('sensors_mean', select_clause, dbname)
    print(client.get_list_continuous_queries())
    
def init_devices_db(dbname):

    client = get_db_client(dbname)
    client.create_database(dbname)

    print("Create a retention policy for db {0}".format(dbname))
    retention_policy = 'keep_forever'
    # Set a infinite retention as the default RP
    client.create_retention_policy(retention_policy, 'INF', 1, default=True)
    client.drop_retention_policy('autogen', database=dbname)

def init_timers_db(dbname):

    client = get_db_client(dbname)
    client.create_database(dbname)

    print("Create a retention policy for db {0}".format(dbname))
    retention_policy = 'keep_forever'
    # Set a infinite retention as the default RP
    client.create_retention_policy(retention_policy, 'INF', 1, default=True)
    client.drop_retention_policy('autogen', database=dbname)


print("init db:s and policies")
init_sensors_db('sensors')
init_devices_db('devices')
init_timers_db('timers')
print("Connected to DB:s!")
        
# If you want to use a specific client id, use
# mqttc = mqtt.Client("client-id")
# but note that the client id must be unique on the broker. Leaving the client
# id parameter empty will generate a random id for you.
mqttc = mqtt.Client()
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
mqttc.on_disconnect = on_disconnect
# Uncomment to enable debug messages
#mqttc.on_log = on_log
mqttc.connect("mosquitto", 1883, 60)

mqttc.subscribe("pc/#", 0)
mqttc.loop_start()
print("Starting reading")


while True:
    time.sleep(2)
    query = 'select * from control;'
    client = get_db_client('timers')
    result = client.query(query)
    timer_list = result['control']

    for timer in timer_list:

        check_for_timeout(mqttc, timer)

