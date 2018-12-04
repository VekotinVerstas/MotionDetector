#!/usr/bin/python3
# -*- coding: utf-8 -*-

import paho.mqtt.client as mqtt
import json
import subprocess

from settings import *  

def play_mp3(path):
        subprocess.Popen(['mpg321', '-q', path]).wait()
        
# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe(MQTT_topic)

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
#    print(msg.topic+" "+str(msg.payload))
 #   print(msg.topic+" "+str(parsed_json))
    print(msg.topic+" "+str(msg.payload.decode("utf-8")))
    if( msg.payload.decode("utf-8") ==mp3message ):
        print("Soita ääni")
        play_mp3(mp3file)
   
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set(MQTT_user, password=MQTT_pw)
client.connect(MQTT_host, 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
