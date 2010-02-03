/* -*- c++ -*- */
/*
 * Copyright (C) 2010 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "../include/iostream"
#ifndef ANDROID_ASTL_IOSTREAM__
#error "Wrong header included!!"
#endif
#include "common.h"

namespace android {

class A {
  public:
    A() {
        mPassed = std::cout.precision() == 6;
    }
    static A mInstance;
    bool mPassed;
};
A A::mInstance;

bool testStaticInit() {
    EXPECT_TRUE(A::mInstance.mPassed);
    return true;
}

bool testOstream() {
    EXPECT_TRUE(std::cout.precision() == 6);
    EXPECT_TRUE(std::cerr.precision() == 6);

    std::cout.precision(20);
    std::cerr.precision(20);

    EXPECT_TRUE(std::cout.precision() == 20);
    EXPECT_TRUE(std::cerr.precision() == 20);
    // reset back to the default value.
    std::cout.precision(6);
    std::cerr.precision(6);
    return true;
}

bool testCoutCerr() {
    std::cout << "Hi from stdout\n";
    std::cerr << "Hi from stderr\n";
    return true;
}

}  // namespace android

int main(int argc, char **argv){
    FAIL_UNLESS(testStaticInit);
    FAIL_UNLESS(testOstream);
    FAIL_UNLESS(testCoutCerr);
    return kPassed;
}
