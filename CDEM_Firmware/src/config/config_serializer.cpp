#include "config_serializer.h"

namespace SmartMeter {

  size_t ConfigSerializer::serialize(char * buffer, size_t size, Configuration * config) {
    size_t neededSpace =
        config->wifi_ssid().length() + 1      // +1's are for strings
      + config->wifi_password().length() + 1
      + config->mqtt_broker().length() + 1
      + sizeof(config->mqtt_port())
      + config->mqtt_topic().length() + 1
      + sizeof(config->use_dhcp())
      + config->static_ip().length() + 1
      + config->subnet_mask().length() + 1
      + config->default_gateway().length() + 1
      + sizeof(config->read_period())

      + 2;  // For length at beginning

    if (neededSpace > size) return 0;

    char * pBuffer = buffer;
    *pBuffer++ = (char)((neededSpace >> 8) & 0xFF);
    *pBuffer++ = (char)((neededSpace) & 0xFF);

    pBuffer += serialize_string(pBuffer, config->wifi_ssid());
    pBuffer += serialize_string(pBuffer, config->wifi_password());
    pBuffer += serialize_string(pBuffer, config->mqtt_broker());
    pBuffer += serialize_primitive(pBuffer, config->mqtt_port());
    pBuffer += serialize_string(pBuffer, config->mqtt_topic());
    pBuffer += serialize_primitive(pBuffer, config->use_dhcp());
    pBuffer += serialize_string(pBuffer, config->static_ip());
    pBuffer += serialize_string(pBuffer, config->subnet_mask());
    pBuffer += serialize_string(pBuffer, config->default_gateway());
    pBuffer += serialize_primitive(pBuffer, config->read_period());

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

    config->mqtt_topic(String(pBuffer));
    pBuffer += config->mqtt_topic().length() + 1;

    bool useDhcp = false;
    pBuffer += deserialize_primitive(pBuffer, &useDhcp);
    config->use_dhcp(useDhcp);

    config->static_ip(String(pBuffer));
    pBuffer += config->static_ip().length() + 1;

    config->subnet_mask(String(pBuffer));
    pBuffer += config->subnet_mask().length() + 1;

    config->default_gateway(String(pBuffer));
    pBuffer += config->default_gateway().length() + 1;

    unsigned int readPeriod = 0;
    pBuffer += deserialize_primitive(pBuffer, &readPeriod);
    config->read_period(readPeriod);

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

  size_t ConfigSerializer::serialize_primitive(char * buffer, long value) {
    memcpy(buffer, (char*)(&value), sizeof(value));
    return sizeof(value);
  }

  size_t ConfigSerializer::serialize_primitive(char * buffer, unsigned int value) {
    memcpy(buffer, (char*)(&value), sizeof(value));
    return sizeof(value);
  }

  size_t ConfigSerializer::serialize_primitive(char * buffer, bool value) {
    memcpy(buffer, (char*)(&value), sizeof(value));
    return sizeof(value);
  }

  size_t ConfigSerializer::deserialize_primitive(char * buffer, int * value) {
    memcpy((char*)(value), buffer, sizeof(*value));
    return sizeof(*value);
  }

  size_t ConfigSerializer::deserialize_primitive(char * buffer, long * value) {
    memcpy((char*)(value), buffer, sizeof(*value));
    return sizeof(*value);
  }

  size_t ConfigSerializer::deserialize_primitive(char * buffer, unsigned int * value) {
    memcpy((char*)(value), buffer, sizeof(*value));
    return sizeof(*value);
  }

  size_t ConfigSerializer::deserialize_primitive(char * buffer, bool * value) {
    memcpy((char*)(value), buffer, sizeof(*value));
    return sizeof(*value);
  }

};