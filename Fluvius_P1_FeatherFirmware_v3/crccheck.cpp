#include "crccheck.h"

namespace SmartMeter {

    // Gets position number in char array
    int CRCchecker::getposition(const char* array, size_t size, char c)
    {
        const char* end = array + size;
        const char* match = std::find(array, end, c);
        return (end == match) ? -1 : (match - array);
    } 

    // Checks the CRC for the datagram
    bool CRCchecker::checkCRC(char* buffer, size_t bufferlength){
        // Find the start of the datagram
        int begin = getposition(buffer, bufferlength, '/');
                
        // Find the end of the datagram
        int eind = getposition(buffer, bufferlength, '!');
                    
        // Find the crc validation
        char crc_validation[5];        
        strncpy(crc_validation,buffer+eind+1,4);
        crc_validation[4]='\0';
        String crcvalidation = String(crc_validation);
        
        // Starting crc check
        unsigned int crc = 0;
        int positie = begin;

        while (positie <= eind)
        {
            crc ^= (unsigned int)buffer[positie];
            int bit = 0;
            while (bit < 8) {
                if ((crc & 1) != 0) {
                    crc = (crc >> 1) ^ 0xA001;
                }
                else crc >>= 1;
                bit++;
            }
            positie++;
        }

        String crccalc(crc,HEX);
        crccalc.toUpperCase();        

        return(crccalc == crcvalidation);
    }

}