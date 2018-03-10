//
// Created by egor9814 on 08.03.18.
//

#include <ctime>
#include <algorithm>
#include "include/GLext/random.hpp"

struct seed_holder {
    long value;

    explicit seed_holder(long l = 8682522807148012L) : value(l) {}

    long set(long l) const { return const_cast<seed_holder *>(this)->value = l; }
};

static seed_holder *seed_uni = nullptr;

long seedUniquifier() {
    if (!seed_uni)
        seed_uni = new seed_holder;
    long current = ::seed_uni->value;
    long next = current;
    while (current == next) {
        next *= 181783497276652981L;
    }
    return ::seed_uni->set(next);
}

void Random::free() {
    delete seed_uni;
}


inline auto currentTimeNanos() {
    return (unsigned long) (clock() * (clock_t) 1000000000 / CLOCKS_PER_SEC);
}


long Random::initialScramble(long seed) {
    return (seed ^ multiplier) & mask;
}

Random::Random(long seed)
        : seed(initialScramble(seed)) {}

Random::Random() {
    seed = seedUniquifier() ^ currentTimeNanos();
}

Random::Random(const Random &random) {
    seed = (seedUniquifier() * random.seed) & mask;
}

Random::Random(State &fromState)
        : seed(fromState.seed) {}

Random::State Random::saveState() const {
    return State {seed};
}


int Random::next(int bits) {
    long current = seed;
    long next = current;
    while (current == next) {
        next = (next * multiplier + addend) & mask;
    }
    seed = next;
    return (int) (seed >> (48 - bits));
}

void Random::nextBytes(unsigned char *bytes, unsigned long length) {
    for (unsigned long i = 0; i < length;) {
        for (long rnd = nextInt(), n = std::min<long>(length - i, 4); n-- > 0; rnd >>= 8)
            bytes[i++] = (unsigned char) rnd;
    }
}

int Random::nextInt() {
    return next(32);
}

unsigned int Random::nextUnsignedInt() {
    return (unsigned int) nextInt();
}

int Random::nextInt(int bound) {
    if (bound <= 0)
        return 0;

    int r = next(31);
    int m = bound - 1;
    if ((bound & m) == 0) {
        r = (int) ((bound * (long) r) >> 31);
    } else {
        for (int u = r; u - (r = u % bound) + m < 0; u = next(31));
    }
    return r;
}

unsigned int Random::nextUnsignedInt(unsigned int bound) {
    auto r = (unsigned int) next(31);
    auto m = bound - 1;
    if ((bound & m) == 0) {
        r = (unsigned int) ((bound * (unsigned long) r) >> 31);
    } else {
        for (unsigned int u = r; u - (r = u % bound) + m < 0; u = (unsigned int) next(31));
    }
    return r;
}

long Random::nextLong() {
    return (long(next(32)) << 32) + next(32);
}

unsigned long Random::nextUnsignedLong() {
    return ((unsigned long) (next(32)) << 32) + next(32);
}

bool Random::nextBool() {
    return next(1) != 0;
}

float Random::nextFloat() {
    return next(24) / float(1 << 24);
}

double Random::nextDouble() {
    return (long(next(26) << 27) + next(27)) * DOUBLE_UNIT;
}