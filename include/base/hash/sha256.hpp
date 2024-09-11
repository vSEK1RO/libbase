#pragma once

#include <cstdint>
#include <vector>

#define SHA256_DIGEST_LENGTH 32

namespace hash {
	std::vector<uint8_t> sha256(const std::vector<uint8_t> &data);
}