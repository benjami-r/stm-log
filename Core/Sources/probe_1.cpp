/*
#include <cstdint>

#define UNIT_COUNT 3

template<uintptr_t Address>
class Reg {
public:
    static void write(const uint32_t value) {
        *reinterpret_cast<volatile uintptr_t *>(Address) = value;
    }
    static uint32_t read() {
        return *reinterpret_cast<volatile uintptr_t *>(Address);
    }
};
*/

