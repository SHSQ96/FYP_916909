from flask import Flask, render_template
from time import sleep
from urllib.request import urlopen
import json


app = Flask(__name__)



door_status_channel_id=1314542
door_status_read_api_key="ZRJPCZ1RYUI0KD18"

CHANNEL_ID_DOOR=1336510
DOOR_CHANNEL_WRITE_API_KEY='I97YZCF8GPKC39TG'

remember_door_status="0"
remember_temperature="32"
remember_humidity="45"



def readSpeak(channel_id, api_key):
    conn = urlopen("http://api.thingspeak.com/channels/%s/feeds/last.json?api_key=%s" \
                           % (channel_id,api_key))
    response = conn.read()
    print("http status code=%s" % (conn.getcode()))
    data=json.loads(response)
    print("data is ",data)
    if data==-1:
        return -1
    print(data['field1'],"created at ",data['created_at'])        
    d={}
    if data["field1"]:
        d["space_left"]=data["field1"]
        global remember_space_left
        remember_space_left=data["field1"]
    else:
        d["space_left"]=remember_space_left
    if data["field2"]:
        d["temperature"]=data["field2"]
        global remember_temperature
        remember_temperature=data["field2"]
    else:
        d["temperature"]=remember_temperature

    if data["field3"]:
        d["humidity"]=data["field3"]
        global remember_humidity
        remember_humidity=data["field3"]
    else:
        d["humidity"]=remember_humidity

    conn.close()
    # sleep(16)
    return d
    

def writeSpeak(api_key, value):
    baseURL = f"http://api.thingspeak.com/update?api_key={api_key}&field1="
    f = urlopen(baseURL +str(value))
    f.read()
    f.close()
    print(f"Sent the value {value}")
    return 0    

@app.route('/openDoor',methods=["POST"])
def openDoor():
    
    print("Going to write to thingspeak")
    value=1
    status=writeSpeak(DOOR_CHANNEL_WRITE_API_KEY,value)
    print("Written ",value)
    sleep(16)
    status=writeSpeak(DOOR_CHANNEL_WRITE_API_KEY,value)
    print("Written ",value)
    sleep(16)
    value=0
    status=writeSpeak(DOOR_CHANNEL_WRITE_API_KEY,value)
    print("Written ",value)
    dicnry={}
    if status==0:
        dicnry["door_opened"]=1
        data=readSpeak(door_status_channel_id, door_status_read_api_key)
        if data==-1:
            print("No data returned")
        print(data)
        
        dicnry["data"]=data
        return render_template("index.html",data=dicnry)
    else:
        return "Some problem in sending command to open the door"




@app.route('/readDoor')
def readDoor():
    print("Some one called read door")
    
    data=readSpeak(door_status_channel_id, door_status_read_api_key)
    if data==-1:
        print("No data returned")
    print(data)
    
    return render_template("index.html",data=data)


@app.route('/')
def hello():
    return "Hello World!"

if __name__ == '__main__':
    app.run(host="0.0.0.0",debug=True)

