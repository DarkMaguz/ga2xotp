/*
 * utils.hpp
 *
 *  Created on 04/04/2026
 *    Peter "Magnus" Balling
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <cstdint>
#include <string>
#include <vector>

std::string Base32Encode(const std::string& binary);

std::string Base64Encode(const std::string& binary);
std::vector<uint8_t> Base64Decode(const std::string& encoded);

#endif /* UTILS_HPP_ */
