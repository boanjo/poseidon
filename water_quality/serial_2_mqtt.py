import serial
import time
import queue
import paho.mqtt.client as mqtt

topic = "pc/water_quality/in/#"
q = queue.Queue()

def on_connect(mqttc, obj, flags, rc):
    print("rc: " + str(rc))
    mqttc.subscribe(topic)

def on_message(mqttc, obj, msg):
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))
    q.put(msg)

def on_publish(mqttc, obj, mid):
    return
    

def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))


def on_log(mqttc, obj, level, string):
    print(string)


# If you want to use a specific client id, use
# mqttc = mqtt.Client("client-id")
# but note that the client id must be unique on the broker. Leaving the client
# id parameter empty will generate a random id for you.
mqttc = mqtt.Client()
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
# Uncomment to enable debug messages
# mqttc.on_log = on_log
mqttc.connect("mosquitto", 1883, 60)

ser = serial.Serial('/dev/ttyWaterQuality', 115200, timeout=1)
time.sleep(2)

print("Starting reading")

mqttc.loop_start()

#
# Wire format
#
#   Serial                 MQTT
# {sensor,value}        host/name/in|out/sensor {value:value}
# Example (outgoing from sensor):
# {water_level,159.5}  ->  pc/water_control/out/water_level {"value":159.5}
# {water,off}  ->  pc/water_control/out/water {"value":0,"type":"state"}
#
# Example (incoming to sensor):
# {auto_level,on}       <- pc/water_control/in/auto_level {"value":1}
#

while True:
    line = str(ser.readline())   # read a byte
    if line:
        #string = line.decode()  # convert the byte string to a unicode string
        if "{" in line and "," in line and "}" in line:
            key = line[line.find("{")+1:line.find(",")]
            value = line[line.find(",")+1:line.find("}")]

            msg = None
            topic = "pc/water_quality/out/" + key
            msg = "{\"value\":" + str(value) + "}" 
                
            mqttc.publish(topic, msg, qos=0)
            #print("published: " + topic + " " + msg)
        
        #print(line)
        # Incoming MQTT MSG
        if q.qsize() > 0:
            msg = q.get()
            li=msg.topic.split("/")
            
            line = str(msg.payload)
            
            if "{" in line and ":" in line and "}" in line:
                key = line[line.find("{")+1:line.find(":")]
                value = line[line.find(":")+1:line.find("}")]

                if "value" in key:
                    if value == "1":
                        value = "on"
                    elif value == "0":
                        value = "off"
                        
                    cmd = "{" + li[-1] + "," + value + "}"
                    print(cmd)
                    ser.write(cmd.encode())
            
            
ser.close()
