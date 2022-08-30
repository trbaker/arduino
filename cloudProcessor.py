# Much of the code used in the cloud to process data from Arduino (before sending to AGO) is provided below.


import urllib.parse
import requests
import json

def create(request):
  try:
    controller = urllib.parse.unquote(request.args.get('controller'))
    temperature = urllib.parse.unquote(request.args.get('temperature'))
    humidity = urllib.parse.unquote(request.args.get('humidity'))
    lat = urllib.parse.unquote(request.args.get('lat'))
    long = urllib.parse.unquote(request.args.get('long'))
    payload= "Recevied data. Controller info: " + controller
  except:
    payload="Error receiving data from Arduino"
    return payload
  try:
    url = 'https://services3.arcgis.com/GzteEaZqBuJ6GIYr/ArcGIS/rest/services/survey123_910b6ea1c50743269a5b171a91fe6cc7_fieldworker/FeatureServer/0/addFeatures'
    params = json.dumps({'f':'pjson','token':'','rollbackOnFailure':'false','features':{'geometry':{'x': lat,'y':long},'attributes':{'controller':controller,'temperature':temperature,'humidity':humidity }}})
    x = requests.post(url, params=params)
    payload = payload + " AGO response: " + x.text
  except :
    payload="Error submitting data to ArcGIS Online."
  #output to screen - for testing purposes mostly
  return payload
