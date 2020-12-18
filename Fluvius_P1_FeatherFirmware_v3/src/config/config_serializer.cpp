#include "config_serializer.h"

namespace SmartMeter {

  size_t ConfigSerializer::serialize(char * buffer, size_t size, Configuration * config) {
    size_t neededSpace =
        config->wifi_ssid().length() + 1
      + config->wifi_password().length() + 1
      + config->mqtt_broker().length() + 1
      + sizeof(config->mqtt_port())
      + 2;  // For length at beginning

    if (neededSpace > size) return -1;

    char * pBuffer = buffer;
    *pBuffer++ = (char)((neededSpace >> 8) & 0xFF);
    *pBuffer++ = (char)((neededSpace) & 0xFF);

    pBuffer += serialize_string(pBuffer, config->wifi_ssid());
    pBuffer += serialize_string(pBuffer, config->wifi_password());
    pBuffer += serialize_string(pBuffer, config->mqtt_broker());

    pBuffer += serialize_primitive(pBuffer, config->mqtt_port());

    return pBuffer-buffer;
  }

  size_t ConfigSerializer::deserialize(char * buffer, size_t size, Configuration * config) {
    char * pBuffer = buffer + 2;  // Skip first 2 bytes (length)

    config->wifi_ssid(String(pBuffer));
    pBuffer += config->wifi_ssid().length() + 1;

    config->wifi_password(String(pBuffer));
    pBuffer += config->wifi_password().length() + 1;

    config->mqtt_broker(String(pBuffer));
    pBuffer += config->mqtt_broker().length() + 1;

    int port = 0;
    pBuffer += deserialize_primitive(pBuffer, &port);
    config->mqtt_port(port);

    return pBuffer-buffer;
  }

  size_t ConfigSerializer::serialize_string(char * buffer, String value) {
    value.toCharArray(buffer, value.length()+1);
    return value.length()+1;
  }

  size_t ConfigSerializer::serialize_primitive(char * buffer, int value) {
    memcpy(buffer, (char*)(&value), sizeof(value));
    return sizeof(value);
  }

  size_t ConfigSerializer::deserialize_primitive(char * buffer, int * value) {
    memcpy((char*)(value), buffer, sizeof(*value));
    return sizeof(*value);
  }

};