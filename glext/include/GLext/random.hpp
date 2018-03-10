//
// Created by egor9814 on 08.03.18.
//

#ifndef OPENGL_TREES_RANDOM_HPP
#define OPENGL_TREES_RANDOM_HPP

class Random {
    constexpr static const double DOUBLE_UNIT = 0x1.0p-53;

    static const long multiplier = 0x5DEECE66DL;
    static const long addend = 0xBL;
    static const long mask = (1L << 48) - 1;

    static long initialScramble(long seed);

    long seed;
public:
    static void free();

    explicit Random(long seed);

    Random();

    Random(const Random &random);


    struct State {
        const long seed;

        explicit State(long l) : seed(l) {}

        State(const State &s) : seed(s.seed) {}

        Random instantinate() { return Random(*this); };
    };

    explicit Random(State &fromState);

    State saveState() const;

protected:
    virtual int next(int bits);


public:
    void nextBytes(unsigned char *bytes, unsigned long length);

    int nextInt();

    unsigned int nextUnsignedInt();

    int nextInt(int bound);

    unsigned int nextUnsignedInt(unsigned int bound);

    long nextLong();

    unsigned long nextUnsignedLong();

    bool nextBool();

    float nextFloat();

    double nextDouble();
};

#endif //OPENGL_TREES_RANDOM_HPP
