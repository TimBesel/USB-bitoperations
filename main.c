
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef uint32_t packet_typ_t;

packet_typ_t *bit_stuffing(packet_typ_t *data, uint8_t dataSize);
packet_typ_t *nrzi(packet_typ_t *data, uint8_t dataSize);
void transfer(packet_typ_t *data, uint8_t dataSize);

int main(void)
{
    packet_typ_t rData[1] = {0xffffffff};
    packet_typ_t *data = bit_stuffing(rData, 32);
    uint8_t dataSize = (uint8_t)data[0];
    data = nrzi(data+1, dataSize);
    transfer(data, dataSize);
    return 0;
}

packet_typ_t *bit_stuffing(packet_typ_t *data, uint8_t dataSize){
    uint8_t buffer = 0x00;
    uint8_t insertedBits = 0;
    packet_typ_t *dataBuffer = (packet_typ_t *)malloc((size_t)(ceil((uint8_t)(dataSize / (8 * sizeof(packet_typ_t)))) + 1) * sizeof(packet_typ_t));

    for(uint8_t bIdx = 0; bIdx < dataSize; bIdx++){

        uint8_t bit = (*(data + (bIdx/(8 * sizeof(packet_typ_t)))) >> ((8 * sizeof(packet_typ_t) - 1) - (bIdx % (8 * sizeof(packet_typ_t))))) & 0x01;
        buffer = (uint8_t)(packet_typ_t)(buffer << 1) | (bit);

        packet_typ_t *dataPointer = (dataBuffer + ((uint8_t)(bIdx + insertedBits)/(8 * sizeof(packet_typ_t))) + 1);
        
        if((buffer & 0x7F) == 0x7F){
            buffer &= 0xFE;
            *dataPointer = (*dataPointer << 1) | (0x00);
            buffer = (uint8_t)(packet_typ_t)(buffer << 1) | (0x01);
            dataBuffer = (packet_typ_t *)realloc(dataBuffer, ((size_t)ceil((uint8_t)((uint8_t)(dataSize + (++insertedBits)) / (8 * sizeof(packet_typ_t)))) + 1) * sizeof(packet_typ_t));
            dataPointer = (dataBuffer + ((uint8_t)(bIdx + insertedBits)/(8 * sizeof(packet_typ_t))) + 1);
        }
        printf("%d\n", (buffer & 0x01));
        *dataPointer = (packet_typ_t)((*dataPointer << 1) | (buffer & 0x01));

    }

    dataBuffer[0] = dataSize + (packet_typ_t)insertedBits;
    return dataBuffer;
}

packet_typ_t *nrzi(packet_typ_t *data, uint8_t dataSize){
    packet_typ_t *dataBuffer = (packet_typ_t *)malloc((size_t)(ceil((uint8_t)(dataSize / (8 * sizeof(packet_typ_t))))) * sizeof(packet_typ_t));
    uint8_t coparator = 0x01;

    for(uint8_t bIdx = 0; bIdx < (dataSize - dataSize%8); bIdx++){
         uint8_t bit = (*(data + (bIdx/(8 * sizeof(packet_typ_t)))) >> ((8 * sizeof(packet_typ_t) - 1) - (bIdx % (8 * sizeof(packet_typ_t))))) & 0x01;

         if(bit == 0x00)
            coparator = (~coparator & 0x01);

        packet_typ_t *dataPointer = (dataBuffer + (bIdx / (8 * sizeof(packet_typ_t))));
        *dataPointer = (packet_typ_t)((*dataPointer << 1) | coparator);
    }
    for(uint8_t bIdx = 0; bIdx < dataSize%(8 * sizeof(packet_typ_t)); bIdx++){
        uint8_t bit = (*(data + (uint8_t)ceil((uint8_t)(dataSize/(8 * sizeof(packet_typ_t))))) >> ((dataSize % (8 * (uint8_t)sizeof(packet_typ_t))) - (bIdx+1) )) & 0x01;

        if(bit == 0x00)
            coparator = (~coparator & 0x01);

        packet_typ_t *dataPointer = (dataBuffer + (bIdx / (8 * sizeof(packet_typ_t))) + 1);
        *dataPointer = (packet_typ_t)((*dataPointer << 1) | coparator);
    }
    return dataBuffer;
}

void transfer(packet_typ_t *data, uint8_t dataSize){
    for(uint8_t i = 0; i < (dataSize - dataSize%8); i++){
        uint8_t bit = (*(data + (i/(8 * sizeof(packet_typ_t)))) >> ((8 * sizeof(packet_typ_t) - 1) - (i%(8*sizeof(packet_typ_t))))) & 0x01;
        printf("%u", bit);
    }
    for(uint8_t i = 0; i < dataSize%(8 * sizeof(packet_typ_t)); i++){
        uint8_t bit = (*(data + (uint8_t)ceil((uint8_t)(dataSize/(8 * sizeof(packet_typ_t))))) >> ((dataSize % (8 * (uint8_t)sizeof(packet_typ_t))) - (i+1) )) & 0x01;
        printf("%u", bit);
    }
}