from bluepy import btle
import paho.mqtt.client as mqtt
from buildhat import Motor
from buildhat import MotorPair
from time import sleep
import time
import vlc
import spotipy
from spotipy.oauth2 import SpotifyOAuth

"""
Motors Lego ports
"""
right_arm = Motor('A') #define the port to connect the motor for the right arm
left_arm = Motor('B')#define the port to connect the motor for the left arm

broker_address="192.168.0.0" #put the ip from Raspberry as broker_address

"""
Spotify values
"""
device_id = ""#extract the device ID from the developers
client_id = ""#extract the client ID from the developers
client_secret = ""#extract the client secret from the developers
redirect_uri = "http://localhost:8080"
scope = "user-read-playback-state,user-modify-playback-state"

sp = spotipy.Spotify(
        auth_manager=spotipy.SpotifyOAuth(
          client_id=client_id,
          client_secret=client_secret,
          redirect_uri=redirect_uri,    
          scope=scope, open_browser=False))
"""
funcionts for movement of robot
"""
def combo(): #movement of arms
    print("Run combo")
    right_arm.run_for_seconds(1.3, speed=-100)
    left_arm.run_for_seconds(1.3, speed=100)
def back():#back to initial position
    print("Run back")
    right_arm.run_for_seconds(1.3, speed=100)
    left_arm.run_for_seconds(1.3, speed=-100)
def both():#move arms at same time
    print("Run both")
    right_arm.run_for_seconds(3, speed=-100, blocking=False)
    left_arm.run_for_seconds(3, speed=100, blocking=False)
def backb():#move back arms at same time
    print("Run backb")
    right_arm.run_for_seconds(3, speed=100, blocking=False)
    left_arm.run_for_seconds(3, speed=-100, blocking=False)
"""
function to obtain values via Bluetooth
"""
def my_main_code(new_value):
    #print(new_value)
    move = new_value #Data streaming from Arduino Nicla SENSE ME
    print(move)
    client = mqtt.Client("P1") #create new instance
    client.connect(broker_address) #connect to broker
    client.subscribe("dance/lights")#subscribe to topic 
    #conditionals of classification from Nicla SENSE ME
    if (move==1):
        client.publish("dance/lights","on") #SEND TO MQTT BROKER on
        print("on")
        sp.start_playback(device_id=device_id ,uris=['spotify:track:5bXGg3XcbEGClkdZ8XYTkI'])
        player= vlc.MediaPlayer('/home/pi/Desktop/model4/ink1.mp4')
        player.play()
        player.set_fullscreen(True)
        for i in range(2):#for to move twice the robot arms
            combo()
            sleep(0.5)
            back()
            sleep(0.5)
        while player.get_state() != vlc.State.Ended:#While to check the status from VLC
            time.sleep(1)  # Just to avoid unnecessary CPU use
            playerstate = str(player.get_state())
            # printing the state of the video
            print("State : " + str(playerstate))
            if (playerstate=="State.Ended"):
                player.stop()
                break
        time.sleep(8)
    if (move==2):
        client.publish("dance/lights","on2")#SEND TO MQTT BROKER on2
        print("on2")
        sp.start_playback(device_id=device_id ,uris=['spotify:track:1o7D1gLUgpFR3eJfIgpSUx'])
        player= vlc.MediaPlayer('/home/pi/Desktop/model4/ink4.mp4')
        player.play()
        player.set_fullscreen(True)
        for i in range(2):#for to move twice the robot arms
            both()
            sleep(2)
            backb()
            sleep(1)
        while player.get_state() != vlc.State.Ended:
            time.sleep(1)  # Just to avoid unnecessary CPU use
            playerstate = str(player.get_state())
            # printing the state of the video
            print("State : " + str(playerstate))
            if (playerstate=="State.Ended"):
                player.stop()
                break
        time.sleep(8)
    if (move==3):#for to move twice the robot arms
        client.publish("dance/lights","on3")#SEND TO MQTT BROKER on2
        print("on3")
        sp.start_playback(device_id=device_id ,uris=['spotify:track:1c39AwcrkN9srI7Az5662I'])
        player= vlc.MediaPlayer('/home/pi/Desktop/model4/magic.mp4')
        player.play()
        player.set_fullscreen(True)
        for i in range(2):
            combo()
            sleep(0.5)
            back()
            sleep(0.5)
        while player.get_state() != vlc.State.Ended:
            time.sleep(1)  # Just to avoid unnecessary CPU use
            playerstate = str(player.get_state())
            # printing the state of the video
            print("State : " + str(playerstate))
            if (playerstate=="State.Ended"):
                player.stop()
                break
        time.sleep(10)

#        else:            j
#            print("other value")
        

class MyDelegate(btle.DefaultDelegate):
    def __init__(self, callback):
        self.callback = callback
        btle.DefaultDelegate.__init__(self)
        # ... initialise here

    def handleNotification(self, cHandle, data):
        # Convert data to be easily consumed by callback
        num = int.from_bytes(data, byteorder='big')
        self.callback(num)


# Initialisation  -------
p = btle.Peripheral("1a:b4:c0:25:3a:aa")#use the scanner.py file to obtain this BLE address for the NICLA 
p.setDelegate( MyDelegate(my_main_code) )

# Setup to turn notifications on, e.g.
svc = p.getServiceByUUID("81c30e5c-1101-4f7d-a886-de3e90749161")#UUID SERVICE defined in Arduino code
ch = svc.getCharacteristics("81c30e5c-2101-4f7d-a886-de3e90749161")[0]#UUID DATA SERVICE defined in Arduino code

setup_data = b"\x01\00"
p.writeCharacteristic(ch.valHandle+1, setup_data)

# Main loop --------

while True:
    if p.waitForNotifications(1.0):
        # handleNotification() was called
        continue

    print("Waiting...")