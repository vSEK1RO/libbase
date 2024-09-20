#pragma once

#include <cstdint>
#include <span>
#include <vector>

#define SHA256_DIGEST_LENGTH 32

namespace hash {
	void sha256(const uint8_t *data, uint64_t data_size, uint8_t *hash) noexcept;
	std::vector<uint8_t> sha256(std::span<const uint8_t> data) noexcept;
}