# Much of the code used in the cloud to process data from Arduino (before sending to AGO) is provided below.
# The script receives data from Arduino, processes it, and sends to ArcGIS Online as a POST request.
# We edit the default dictionary (treating as a string) and then use the ast library to convert to a dictionary before sending to AGO

import urllib.parse
import requests
import ast

controller="null"
lat=0
lon=0
temperature=0
humidity=0

def create(request):
  try:
    controller = urllib.parse.unquote(request.args.get('controller'))
    temperature = urllib.parse.unquote(request.args.get('temperature'))
    humidity = urllib.parse.unquote(request.args.get('humidity'))
    lat = urllib.parse.unquote(request.args.get('lat'))
    lon = urllib.parse.unquote(request.args.get('lon'))
    payload= "Received data. Controller info: " + controller
  except:
    payload="Error receiving data from Arduino"
    return payload
  try:
    url = 'https://services3.arcgis.com/GzteEaZqBuJ6GIYr/ArcGIS/rest/services/survey123_910b6ea1c50743269a5b171a91fe6cc7_fieldworker/FeatureServer/0/addFeatures'
    
    params = {'f': 'html', 'token': '', 'rollbackOnFailure': 'false', 'features':'{ "attributes" : {"controller" : "controller999","temperature" : temperature2,"humidity" : humidity2},"geometry" : {"x" : lon2,"y" : lat2}}'}

    params=str(params).replace("temperature2", str(temperature))
    params=str(params).replace("humidity2", str(humidity))
    params=str(params).replace("lat2", str(lat))
    params=str(params).replace("lon2", str(lon))
    params=str(params).replace("controller999", str(controller))

    # converts the string to a python dictionary
    params=ast.literal_eval(params)
    #submits the dictionary to the URL
    x=requests.post(url, params)
    
    payload = payload + '  AGO Status response:' + str(x.status_code)
  except :
    payload="Error submitting data to ArcGIS Online."
  #output to screen - for testing purposes mostly
  return payload
