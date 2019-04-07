#include <HTTPClient.h>
#include "CanAirIoApi.hpp"

CanAirIoApi::CanAirIoApi(bool debug)
{
    _debug=debug;
}

CanAirIoApi::~CanAirIoApi()
{
}

void CanAirIoApi::configure(const char nameId[], const char sensorId[], const char endpoint[],const char host[],const uint16_t port)
{   
    _nameId = new char[strlen(nameId)+1];
    strcpy(_nameId,nameId); 
    _sensorId = new char[strlen(sensorId)+1];
    strcpy(_sensorId,sensorId); 
    _endpoint = new char[strlen(endpoint)+1];
    strcpy(_endpoint,endpoint);
    _host = new char[strlen(host)+1];
    strcpy(_host,host);
    _port = port;
    if(_debug)Serial.println("-->[API] configure with id: "+String(_sensorId));
}

void CanAirIoApi::authorize(const char username[], const char password[])
{
    _username = new char[strlen(username)+1];
    strcpy(_username,username);
    _password = new char[strlen(password)+1];
    strcpy(_password,password);
    _isAuthorised = true;
    if(_debug)Serial.println("-->[API] user:"+String(_username)+" pass:"+String(_password));
}

bool CanAirIoApi::write(uint16_t pm1, uint16_t pm25, uint16_t pm10, float hum, float tmp, float lat, float lon, float alt, float spd, int stime)
{   
    HTTPClient http;
    char uri[32];
    sprintf(uri, "/%s", _endpoint);

    if(_isSecure) {
       http.begin(_host, _port, uri, _cert);
    }
    else {
        http.begin(_host, _port, uri);
    }

    http.addHeader("Content-Type","application/json");
    http.addHeader("cache-control","no-cache");
        
    if(_isAuthorised) {
        http.setAuthorization(_username,_password);
    }

    const int capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(11);
    StaticJsonDocument <capacity> doc;

    JsonObject data = doc.createNestedObject();

    data["measurement"] = _nameId;
    data["nameId"] = _nameId;
    data["sensorId"] = _sensorId;
    JsonObject fields = data.createNestedObject("fields");
    fields["pm1"] = pm1;
    fields["pm25"] = pm25;
    fields["pm10"] = pm10;
    fields["hum"] = hum;
    fields["tmp"] = tmp;
    fields["lat"] = lat;
    fields["lon"] = lon;
    fields["alt"] = alt;
    fields["spd"] = spd;
    fields["stime"] = stime;
    if(_debug)serializeJsonPretty(doc, Serial);
    String writeBuf;
    serializeJson(doc,writeBuf);
    _latestResponse = http.POST(writeBuf.c_str());
    if(_debug)Serial.println("-->[API] response: "+String(_latestResponse));
    http.end();
    return _latestResponse == 204;
}

int CanAirIoApi::getResponse()
{
    return _latestResponse;
}

bool CanAirIoApi::isSecure()
{
    return _isSecure;
}
