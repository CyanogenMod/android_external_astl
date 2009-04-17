/* -*- c++ -*- */
/*
 * Copyright (C) 2009 The Android Open Source Project
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

#include "../include/algorithm"
#ifndef ANDROID_ASTL_ALGORITHM__
#error "Wrong header included!!"
#endif
#include "macros.h"

namespace android {

bool testSwapInt()
{
    int a = 5;
    int b = 10;

    std::swap(a, b);
    EXPECT_TRUE(a == 10);
    EXPECT_TRUE(b == 5);
    return true;
}

bool testMin()
{
    int a = 5;
    int b = 10;

    int c = std::min(a, b);
    EXPECT_TRUE(c == 5);
    c = std::min(b, a);
    EXPECT_TRUE(c == 5);
    return true;
}

bool testMax()
{
    int a = 5;
    int b = 10;

    int c = std::max(a, b);
    EXPECT_TRUE(c == 10);
    c = std::max(b, a);
    EXPECT_TRUE(c == 10);
    return true;
}

}  // namespace android

int main(int argc, char **argv)
{
    FAIL_UNLESS(testSwapInt);
    FAIL_UNLESS(testMin);
    FAIL_UNLESS(testMax);
    return kPassed;
}
