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

#include "../include/vector"
#ifndef ANDROID_ASTL_VECTOR__
#error "Wrong header included!!"
#endif
#include <climits>
#include <cstring>
#include "common.h"

namespace android {
using std::vector;
static const size_t kExponentialFactor = 2;
bool testConstructorInt()
{
    {
        vector<int> vec1;
        EXPECT_TRUE(vec1.empty());
        EXPECT_TRUE(vec1.size() == 0);
        EXPECT_TRUE(vec1.capacity() == 0);
    }
    {
        vector<int> vec2(100);
        EXPECT_TRUE(!vec2.empty());
        EXPECT_TRUE(vec2.size() == 100);
        EXPECT_TRUE(vec2.capacity() == 100);
        for (size_t i = 0; i < 100; ++i)
        {
            EXPECT_TRUE(vec2[i] == 0);
        }
    }
    {
        vector<int> vec3(200, 0xaa);
        EXPECT_TRUE(!vec3.empty());
        EXPECT_TRUE(vec3.size() == 200);
        EXPECT_TRUE(vec3.capacity() == 200);
        for (size_t i = 0; i < 200; ++i)
        {
            EXPECT_TRUE(vec3[i] == 0xaa);
        }
    }
    return true;
}

typedef enum { ONE = 10, TWO} TestEnum;

template<typename T> struct A { };
struct B { };

bool testConstructorRepeat()
{
    {
        const vector<int> vec1(100, 10);

        EXPECT_TRUE(vec1.end() - vec1.begin() == 100);
        for (int i = 0; i < 100; ++i)
        {
            EXPECT_TRUE(vec1[i] == 10);
        }
    }
    {
        const vector<float> vec2(100, 10.0f);

        for (int i = 0; i < 100; ++i)
        {
            EXPECT_TRUE(vec2[i] == 10.0f);
        }
    }
    {
        const vector<TestEnum> vec3(100, ONE);

        for (int i = 0; i < 100; ++i)
        {
            EXPECT_TRUE(vec3[i] == ONE);
        }
    }
    {
        const vector< A<B> > vec4;
        const vector< A<B> > vec5(10);

        EXPECT_TRUE(vec4.size() == 0);
        EXPECT_TRUE(vec5.size() == 10);
    }
    return true;
}


bool testReserve()
{
    { // basic reserve + shrink.
        vector<int> vec1(100, 10);

        EXPECT_TRUE(vec1.capacity() == 100);
        EXPECT_TRUE(vec1.reserve(200));
        EXPECT_TRUE(vec1.capacity() == 200);
        EXPECT_TRUE(vec1.size() == 100);

        EXPECT_TRUE(vec1.reserve());
        EXPECT_TRUE(vec1.capacity() == 100);
        EXPECT_TRUE(vec1.size() == 100);
    }
    {
        vector<int> vec2;

        EXPECT_TRUE(vec2.capacity() == 0);
        EXPECT_TRUE(vec2.reserve());
        EXPECT_TRUE(vec2.capacity() == 0);

        vec2.reserve(200);
        EXPECT_TRUE(vec2.capacity() == 200);
        vec2.reserve();
        EXPECT_TRUE(vec2.capacity() == 0);
        vec2.push_back(3);
        vec2.reserve();
        EXPECT_TRUE(vec2.capacity() == 1);
    }
    {
        vector<int> vec3;

        vec3.push_back(5);
        vec3.reserve();
        EXPECT_TRUE(vec3.capacity() == 1);
        vec3.push_back(3);
        EXPECT_TRUE(vec3.capacity() == kExponentialFactor);
        while (vec3.size() < kExponentialFactor)
            vec3.push_back(3);

        EXPECT_TRUE(vec3.size() == kExponentialFactor);
        EXPECT_TRUE(vec3.capacity() == kExponentialFactor);

        // exp increment.
        vec3.push_back(10);
        EXPECT_TRUE(vec3.capacity() == kExponentialFactor * kExponentialFactor);
    }
    {
        CopyCounter c;

        c.mCount = 0;
        vector<CopyCounter> vec4(100, c);
        EXPECT_TRUE(c.mCount == 100);
        // Resizing does not do any copy via the copy assignement op.
        vec4.reserve(1000);
        EXPECT_TRUE(c.mCount == 200);
        vec4.reserve(50); // reserving less than length is a nop.
        EXPECT_TRUE(c.mCount == 200);
    }
    {
        vector<unsigned short> vec5;

        EXPECT_TRUE(!vec5.reserve(vec5.max_size() + 1));
        EXPECT_TRUE(vec5.capacity() == 0);
    }
    return true;
}


bool testPushBack()
{
    {
        vector<CtorDtorCounter> vec1;
        CtorDtorCounter c;

        c.reset();
        for (int i = 0; i < 1000; ++i)
        {
            vec1.push_back(c);
        }
        EXPECT_TRUE(vec1.capacity() == 1024);
        EXPECT_TRUE(vec1.size() == 1000);
        EXPECT_TRUE(c.mAssignCount == 1000);
        // Due to the multiple augmentation of the capacity, the copy
        // constructor has been invoked.
        EXPECT_TRUE(c.mCopyCtorCount > 0);
        EXPECT_TRUE(c.mCtorCount == 0);
    }
    {
        vector<int> vec2;

        vec2.push_back(10);
        EXPECT_TRUE(vec2.front() == 10);
        EXPECT_TRUE(vec2.back() == 10);
        EXPECT_TRUE(vec2.size() == 1);
        vec2.push_back(20);
        EXPECT_TRUE(vec2.front() == 10);
        EXPECT_TRUE(vec2.back() == 20);
        EXPECT_TRUE(vec2.size() == 2);
    }
    return true;
}


bool testPopBack()
{
    vector<int> vec1(10, 0xdeadbeef);;

    EXPECT_TRUE(vec1.capacity() == 10);
    EXPECT_TRUE(vec1.size() == 10);

    for(size_t i = 10; i > 0; --i)
    {
        EXPECT_TRUE(vec1.capacity() == 10);
        EXPECT_TRUE(vec1.size() == i);
        vec1.pop_back();
    }
    EXPECT_TRUE(vec1.empty());
    EXPECT_TRUE(vec1.begin() == vec1.end());
    vec1.pop_back(); // pop_back on empty vector
    EXPECT_TRUE(vec1.size() == 0);
    EXPECT_TRUE(vec1.capacity() == 10);

    vec1.clear();
    vec1.pop_back(); // pop_back on empty vector
    EXPECT_TRUE(vec1.size() == 0);
    EXPECT_TRUE(vec1.capacity() == 0);
    EXPECT_TRUE(vec1.begin() == vec1.end());
    EXPECT_TRUE(vec1.begin() == NULL);

    CtorDtorCounter instance;
    vector<CtorDtorCounter> vec2(10, instance);

    CtorDtorCounter::reset();
    for (int i = 0; i < 10; ++i)
    {
        vec2.pop_back();
    }
    EXPECT_TRUE(vec2.size() == 0);
    EXPECT_TRUE(CtorDtorCounter::mDtorCount == 10);
    return true;
}


bool testSwap()
{
    vector<int> vec1(100, 10);
    vector<int> vec2;

    vec1.swap(vec2);

    EXPECT_TRUE(vec1.capacity() == 0);
    EXPECT_TRUE(vec2.capacity() == 100);

    EXPECT_TRUE(vec1.size() == 0);
    EXPECT_TRUE(vec2.size() == 100);

    EXPECT_TRUE(vec1.begin() == vec1.end());
    EXPECT_TRUE(vec2.begin() != vec2.end());
    return true;
}


bool testIterators()
{
    vector<int> vec1(10);

    for (size_t i = 0; i < 10; ++i)
    {
        vec1[i] = i;
    }

    vector<int>::iterator i = vec1.begin();
    for (int c = 0; i != vec1.end(); ++i, ++c)
    {
        EXPECT_TRUE(c == *i);
    }

    vector<int>::const_iterator j = vec1.begin();
    for (int c = 0; j != vec1.end(); ++j, ++c)
    {
        EXPECT_TRUE(c == *j);
    }
    return true;
}

bool testCtorDtorForNonPod()
{
    {  // empty vector, no construction should happen.
        CtorDtorCounter::reset();
        vector<CtorDtorCounter> vec1;

        EXPECT_TRUE(CtorDtorCounter::mCtorCount == 0);
        EXPECT_TRUE(CtorDtorCounter::mCopyCtorCount == 0);
    }
    EXPECT_TRUE(CtorDtorCounter::mDtorCount == 0);

    {
        CtorDtorCounter instance;
        EXPECT_TRUE(CtorDtorCounter::mCtorCount == 1);
        CtorDtorCounter::reset();

        vector<CtorDtorCounter> vec2(200, instance);

        // 200 copies by assignement of the sample instance
        EXPECT_TRUE(CtorDtorCounter::mAssignCount == 0);
        EXPECT_TRUE(CtorDtorCounter::mCtorCount == 0);
        EXPECT_TRUE(CtorDtorCounter::mCopyCtorCount == 200);
        EXPECT_TRUE(CtorDtorCounter::mDtorCount == 0);

        CtorDtorCounter::reset();
        vec2.reserve(400);

        // 200 moves: 200 copies by copy constructor and 200 destructions.
        EXPECT_TRUE(CtorDtorCounter::mCopyCtorCount == 200);
        EXPECT_TRUE(CtorDtorCounter::mDtorCount == 200);
        EXPECT_TRUE(CtorDtorCounter::mCtorCount == 0);
        EXPECT_TRUE(CtorDtorCounter::mAssignCount == 0);

        CtorDtorCounter::reset();
    }
    // 200 + 1 for the instance
    EXPECT_TRUE(CtorDtorCounter::mDtorCount == 201);
    return true;
}
}  // namespace android

int main(int argc, char **argv)
{
    FAIL_UNLESS(testConstructorInt);
    FAIL_UNLESS(testConstructorRepeat);
    FAIL_UNLESS(testReserve);
    FAIL_UNLESS(testPushBack);
    FAIL_UNLESS(testPopBack);
    FAIL_UNLESS(testSwap);
    FAIL_UNLESS(testIterators);
    FAIL_UNLESS(testCtorDtorForNonPod);
    return kPassed;
}
