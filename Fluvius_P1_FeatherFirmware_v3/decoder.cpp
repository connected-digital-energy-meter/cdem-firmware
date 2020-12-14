#include "decoder.h"
#include "datagram.h"

namespace SmartMeter {
    double Decoder::ParseDataValue(char* _buffer, char* key, int datablock){
        // look for the OBIS reference
        char* position = strstr(_buffer, key);

        // if OBIS reference found
        if (position)
        {
            // look for the start position of the value for this OBIS reference
            char* start = position + strlen(key)+1;

            // if there are two value blocks then look for the start of the second one
            if(datablock==2){
                char* start = strchr(start,'(')+1;  
            }

            // look for the end position of the value for this OBIS reference
            char* end = strchr(start,'*')-1;

            // get the value of the OBIS reference
            char tempbuffer[64] = {0};
            strncpy(tempbuffer,start,end-start+1);

            // return the value
            return atof(tempbuffer);
        } else {
            // return invalid if OBIS reference not found
            return -1;
        }
    }

    Datagram Decoder::decode(char* buffer, size_t bufferlength){
        
        Datagram output;

        // 1-0:1.8.2 = OBIS reference for electricity delivered to client in low tariff
        output.consumption_low_tarif(ParseDataValue(buffer,"1-0:1.8.2",1));
        // 1-0:1.8.1 = OBIS reference for electricity delivered to client in high tariff
        output.consumption_high_tarif(ParseDataValue(buffer,"1-0:1.8.1",1));
        // 1-0:2.8.2 = OBIS reference for electricity delivered by client in low tariff
        output.production_low_tarif(ParseDataValue(buffer,"1-0:2.8.2",1));
        // 1-0:2.8.1 = OBIS reference for electricity delivered by client in high tariff
        output.production_high_tarif(ParseDataValue(buffer,"1-0:2.8.1",1));
        // 1-0:1.7.0 = OBIS reference actual total power delivered to client
        output.total_power_consumption(ParseDataValue(buffer,"1-0:1.7.0",1));
        // 1-0:2.7.0 = OBIS reference actual total power delivered by client
        output.total_power_production(ParseDataValue(buffer,"1-0:2.7.0",1));
        // 0-0:96.14.0 = OBIS reference actual tariff
        output.actual_tarif(ParseDataValue(buffer,"0-0:96.14.0",1));
        // 1-0:32.7.0 = OBIS reference actual voltage L1
        output.actual_voltage_l1(ParseDataValue(buffer,"1-0:32.7.0",1));
        // 1-0:52.7.0 = OBIS reference actual voltage L2
        output.actual_voltage_l2(ParseDataValue(buffer,"1-0:52.7.0",1));
        // 1-0:72.7.0 = OBIS reference actual voltage L3
        output.actual_voltage_l3(ParseDataValue(buffer,"1-0:72.7.0",1));
        // 1-0:31.7.0 = OBIS reference actual current L1
        output.actual_current_l1(ParseDataValue(buffer,"1-0:31.7.0",1));
        // 1-0:51.7.0 = OBIS reference actual current L2
        output.actual_current_l2(ParseDataValue(buffer,"1-0:51.7.0",1));
        // 1-0:71.7.0 = OBIS reference actual current L3
        output.actual_current_l3(ParseDataValue(buffer,"1-0:71.7.0",1));
        // 1-0:22.7.0 = OBIS reference actual power comsumption L1
        output.l1_power_consumption(ParseDataValue(buffer,"1-0:22.7.0",1));
        // 1-0:42.7.0 = OBIS reference actual power comsumption L2
        output.l2_power_consumption(ParseDataValue(buffer,"1-0:42.7.0",1));
        // 1-0:62.7.0 = OBIS reference actual power comsumption L3
        output.l3_power_consumption(ParseDataValue(buffer,"1-0:62.7.0",1));
        // 1-0:21.7.0 = OBIS reference actual power production L1
        output.l1_power_production(ParseDataValue(buffer,"1-0:21.7.0",1));
        // 1-0:41.7.0 = OBIS reference actual power production L2
        output.l2_power_production(ParseDataValue(buffer,"1-0:41.7.0",1));
        // 1-0:61.7.0 = OBIS reference actual power production L3
        output.l3_power_production(ParseDataValue(buffer,"1-0:61.7.0",1));
        // 0-1:24.2.3 = OBIS reference gas delivered to client with temperature correction , 0-n where the n is the device number, possibly you need to change this number for your configuration
        output.gas_meter_m3(ParseDataValue(buffer,"0-1:24.2.3",2));
        // 0-2:24.2.1 = OBIS reference water delivered to client, 0-n where the n is the device number, possibly you need to change this number for your configuration
        output.water_meter_m3(ParseDataValue(buffer,"0-2:24.2.1",2));

        return output;
    }


}