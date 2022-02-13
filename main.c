#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef packet_typ_t (uint8_t);

uint8_t *bit_stuffing(uint8_t *data, uint8_t dataSize);
void transfer(uint8_t *data, uint8_t dataSize);

int main(void)
{   
    uint8_t rData[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t *data = bit_stuffing(rData, 32);
    printf("last: %x\n", data[0]);
    transfer((data+1), data[0]);
    return 0;
}

uint8_t *bit_stuffing(uint8_t *data, uint8_t dataSize){
    uint8_t buffer = 0x00;
    uint8_t insertedBits = 0;
    uint8_t *dataBuffer = (uint8_t *)malloc((size_t)(ceil(dataSize / 8) + 1) * sizeof(uint8_t));

    for(uint8_t bIdx = 0; bIdx < dataSize; bIdx++){

        uint8_t bit = (*(data + (bIdx/8)) >> (7-(bIdx%8))) & 0x01;
        buffer = (uint8_t)(buffer << 1) | (bit);

        uint8_t *dataPointer = (dataBuffer + ((bIdx + insertedBits)/8) + 1);
        
        if((buffer & 0x7F) == 0x7F){
            buffer &= 0xFE;
            *dataPointer = (*dataPointer << 1) | (0x00);
            buffer = (uint8_t)(buffer << 1) | (0x01);
            dataBuffer = (uint8_t *)realloc(dataBuffer, ((size_t)ceil((dataSize + (++insertedBits)) / 8) + 1) * sizeof(uint8_t));
            dataPointer = (dataBuffer + ((bIdx + insertedBits)/8) + 1);
        }

        *dataPointer = (uint8_t)((*dataPointer << 1) | (buffer & 0x01));

    }

    dataBuffer[0] = dataSize + insertedBits;
    return dataBuffer;
}

void transfer(uint8_t *data, uint8_t dataSize){
    for(uint8_t i = 0; i < (dataSize - dataSize%8); i++){
        uint8_t bit = (*(data + (i/8)) >> (7-(i%8))) & 0x01;
        printf("%u", bit);
    }
    for(uint8_t i = 0; i < dataSize%8; i++){
        uint8_t bit = (*(data + (uint8_t)ceil(dataSize/8)) >> ((dataSize % 8) - (i+1) )) & 0x01;
        printf("%u", bit);
    }
}

/*uint8_t *bit_stuffing(uint8_t *data, uint8_t dataSize){
    uint8_t buffer = 0x00;
    uint8_t insertedBits = 0;
    uint8_t *dataBuffer = (uint8_t *)malloc((size_t)(ceil(dataSize / 8) + 1) * sizeof(uint8_t));

    for(int8_t bIdx = (int8_t)dataSize-1; bIdx >=  0; bIdx--){

        uint8_t bit = (*(data + (((dataSize-1) - bIdx)/8)) >> (bIdx % 8)) & 0x01;
        buffer = (uint8_t)(buffer << 1) | (bit);

        uint8_t *dataPointer = (dataBuffer + (((dataSize - 1 + insertedBits) - bIdx)/8) + 1);
        
        if((buffer & 0x7F) == 0x7F){
            buffer &= 0xFE;
            *dataPointer = (*dataPointer << 1) | (0x00);
            buffer = (uint8_t)(buffer << 1) | (0x01);
            dataBuffer = (uint8_t *)realloc(dataBuffer, ((size_t)ceil((dataSize + (++insertedBits)) / 8) + 1) * sizeof(uint8_t));
            dataPointer = (dataBuffer + (((dataSize - 1 + insertedBits) - bIdx)/8) + 1);
        }

        *dataPointer = (uint8_t)((*dataPointer << 1) | (buffer & 0x01));

    }

    dataBuffer[0] = dataSize + insertedBits;
    return dataBuffer;
}

void transfer(uint8_t *data, uint8_t dataSize){
    for(uint8_t i = 0; i < (dataSize - dataSize%8); i++){
        uint8_t bit = (*(data + (i/8)) >> (7-(i%8))) & 0x01;
        printf("%u", bit);
    }
    for(uint8_t i = 0; i < dataSize%8; i++){
        uint8_t bit = (*(data + (uint8_t)ceil(dataSize/8)) >> ((dataSize % 8) - (i+1) )) & 0x01;
        printf("%u", bit);
    }
}*/

