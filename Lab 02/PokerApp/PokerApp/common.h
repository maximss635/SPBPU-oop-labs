//
// Created by maxim on 11.11.2020.
//

#ifndef POKERAPP_COMMON_H
#define POKERAPP_COMMON_H

#include <iostream>
#include <cstdint>
#include <string>
#include <array>

typedef     unsigned long long      ull;
typedef     double                  money_t;
typedef     uint32_t                hash_t;
typedef     std::string             str_t;
typedef     enum{lose,win}          gameRes_t;
typedef     std::array<uint8_t, 4>  ipv4_t;

// :)
hash_t hash(std::string& s) {
    return static_cast<hash_t>(s.size());
}

#endif //POKERAPP_COMMON_H
