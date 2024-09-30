#pragma once

#include <cstdint>
#include <span>
#include <vector>

namespace hash
{
	void sha256(const uint8_t *data, size_t data_size, uint8_t *hash) noexcept;
	std::vector<uint8_t> sha256(std::span<const uint8_t> data) noexcept;
}