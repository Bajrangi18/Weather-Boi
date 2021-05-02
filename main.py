import requests
import json
import firebase_admin
from firebase_admin import credentials
from firebase_admin import db
import datetime
import pytz
import time
#firebase connect
cred = credentials.Certificate('E:\Python_projects\Weather_trial\weather-boi-16a3f-firebase-adminsdk-w9min-928e032d80.json')

firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://weather-boi-16a3f-default-rtdb.firebaseio.com/'
})
#time
current_time = datetime.datetime.now(pytz.timezone('Asia/Kolkata'))
#GET data
response = requests.get(url="https://api.openweathermap.org/data/2.5/weather?q=Bengaluru&appid=0b112a74538f98a57732fa51681b7b99&units=metric")
print(response.status_code)
response_json = response.json() #json to python
y = response_json["main"]
z = response_json["weather"]
weather_data={
    "Temperature":y["temp"],
    "Humidity":y["humidity"],
    "Pressure":y["pressure"],
    "Analysis":z[0]["description"]
}
ref = db.reference('/')
users_ref = ref.child('Users')
users_ref.child(current_time.strftime("%c")).set(weather_data)
time.sleep(3600)