#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef uint8_t packet_typ_t;

packet_typ_t *bit_stuffing(packet_typ_t *data, uint8_t dataSize);
void transfer(packet_typ_t *data, uint8_t dataSize);

int main(void)
{   
    packet_typ_t rData[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    packet_typ_t *data = bit_stuffing(rData, 32);
    transfer((data+1), data[0]);
    return 0;
}

packet_typ_t *bit_stuffing(packet_typ_t *data, uint8_t dataSize){
    packet_typ_t buffer = 0x00;
    uint8_t insertedBits = 0;
    packet_typ_t *dataBuffer = (packet_typ_t *)malloc((size_t)(ceil(dataSize / (8 * sizeof(packet_typ_t))) + 1) * sizeof(packet_typ_t));

    for(uint8_t bIdx = 0; bIdx < dataSize; bIdx++){

        uint8_t bit = (*(data + (bIdx/(8 * sizeof(packet_typ_t)))) >> (7-(bIdx % (8 * sizeof(packet_typ_t))))) & 0x01;
        buffer = (packet_typ_t)(buffer << 1) | (bit);

        packet_typ_t *dataPointer = (dataBuffer + ((bIdx + insertedBits)/(8 * sizeof(packet_typ_t))) + 1);
        
        if((buffer & 0x7F) == 0x7F){
            buffer &= 0xFE;
            *dataPointer = (*dataPointer << 1) | (0x00);
            buffer = (packet_typ_t)(buffer << 1) | (0x01);
            dataBuffer = (packet_typ_t *)realloc(dataBuffer, ((size_t)ceil((dataSize + (++insertedBits)) / (8 * sizeof(packet_typ_t))) + 1) * sizeof(uint8_t));
            dataPointer = (dataBuffer + ((bIdx + insertedBits)/(8 * sizeof(packet_typ_t))) + 1);
        }

        *dataPointer = (packet_typ_t)((*dataPointer << 1) | (buffer & 0x01));

    }

    dataBuffer[0] = dataSize + insertedBits;
    return dataBuffer;
}

packet_typ_t *nrzi(packet_typ_t *data, uint8_t dataSize){
    packet_typ_t *dataBuffer = (packet_typ_t *)malloc((size_t)(ceil(dataSize / (8 * sizeof(packet_typ_t)))) * sizeof(packet_typ_t));
    uint8_t coparator = 0x01;
    
    for(uint8_t bIdx = 0; bIdx < dataSize; bIdx++){

    }
}

void transfer(packet_typ_t *data, uint8_t dataSize){
    for(uint8_t i = 0; i < (dataSize - dataSize%8); i++){
        uint8_t bit = (*(data + (i/(8 * sizeof(packet_typ_t)))) >> (7-(i%(8*sizeof(packet_typ_t))))) & 0x01;
        printf("%u", bit);
    }
    for(uint8_t i = 0; i < dataSize%(8 * sizeof(packet_typ_t)); i++){
        uint8_t bit = (*(data + (uint8_t)ceil(dataSize/(8 * sizeof(packet_typ_t)))) >> ((dataSize % (8 * sizeof(packet_typ_t))) - (i+1) )) & 0x01;
        printf("%u", bit);
    }
}

