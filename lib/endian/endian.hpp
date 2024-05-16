#pragma once
#ifndef ENDIAN_HPP
#define ENDIAN_HPP

#include <algorithm>
#include <iterator>
#include <cstdint> //uint8_t

namespace endian
{

    template <typename T>
    T reverse(const T in)
    {
        union
        {
            T data{};
            uint8_t byte[sizeof(T)];
        } source;
        source.data = in;
        std::reverse(std::begin(source.byte), std::end(source.byte));
        std::cout << "rez: " << std::hex << source.data <<"\n";
        return source.data;
    };

    // template<typename T,Endian from, Endian to>
    // struct Word{
    //     /* anonymous union makes sure that the data and
    //     ** byte[] occupy the same memory location
    //     */
    //     union{
    //         T data;
    //         uint8_t byte[sizeof(T)];
    //     };

    //     uint8_t& operator[](int i){
    //         if(from == to){
    //             return byte[i];
    //         }else{
    //             /* flip the order only if the source and destination has
    //             ** different ordering
    //             */
    //             return byte[sizeof(T) - i - 1];
    //         }
    //     }
    // };

    // // a helper function to show how the actual memory representatiom
    // void show_memory_representation(const uint8_t* firstbyte,const int length){
    //     for(int i=0;i<length;++i){
    //         printf("%.2X",*(firstbyte + i));
    //     }
    //     printf("\n");
    // }

    // int main(){

    //     Word<uint32_t,ENDIANNESS::little,ENDIANNESS::big> w;
    //     Word<uint32_t,ENDIANNESS::little,ENDIANNESS::little> w1;
    //     Word<uint32_t,ENDIANNESS::big,ENDIANNESS::big> w2;
    //     Word<uint32_t,ENDIANNESS::big,ENDIANNESS::little> w3;

    //     // our data source which will be using the system endianness in this case
    //     uint32_t source = 0xAABBCCDD;

    //     cout << "Actual memory representation:" << endl;

    //     show_memory_representation((uint8_t*)&source,4);

    //     // to iterate through the data source (could be a file pointer)
    //     uint8_t *ptr = (uint8_t*)&source;

    //     for(int i=0;i<4;++i){
    //         w[i]  = *(ptr + i);
    //         w1[i] = *(ptr + i);
    //         w2[i] = *(ptr + i);
    //         w3[i] = *(ptr + i);
    //     }

    //     cout << "Expecting little-endian, storing as big-endian" << endl;
    //     show_memory_representation((uint8_t*)&w.data,4);

    //     cout << "Expecting little-endian, storing as little-endian" << endl;
    //     show_memory_representation((uint8_t*)&w1.data,4);

    //     cout << "Expecting big-endian, storing as big-endian" << endl;
    //     show_memory_representation((uint8_t*)&w2.data,4);

    //     cout << "Expecting big-endian, storing as little-endian" << endl;
    //     show_memory_representation((uint8_t*)&w3.data,4);

    //     return 0;
    // }
}

#endif /* ENDIAN_HPP */
