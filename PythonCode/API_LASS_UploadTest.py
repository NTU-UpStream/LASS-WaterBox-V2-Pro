import requests as RE

MQTT_LASS_TOPIC = "LASS/Test/WaterBox_TW/NTU123456789"

URL = "http://pm25.lass-net.org/Upload/waterbox_tw.php?"
PARAMS = {"device_id": "NTU123456789",
          "topic": "WaterBox_TW",
          "key": "NoKey",
          "msg": "Python via http"}

URL_DETAILED = f"{URL}device_id={PARAMS['device_id']}&topic={PARAMS['topic']}&key={PARAMS['key']}&msg={PARAMS['msg']}"

if __name__ == "__main__":
    # api_re = RE.get(url=URL, params=PARAMS)
    print(URL_DETAILED)
    api_re = RE.get(url=URL_DETAILED, verify=False)
    print(f"Status Code:{api_re.status_code}")
