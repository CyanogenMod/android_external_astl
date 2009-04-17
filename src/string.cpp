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

#include <string>
#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstring>
#include <malloc.h>

#ifndef MAX_SIZE_T
#define MAX_SIZE_T           (~(size_t)0)
#endif

namespace {
char kEmptyString[1] = { '\0' };
}

namespace std {
// Implementation of the std::string class.
//
// mData points either to a heap allocated array of bytes or the constant
// kEmptyString when empty and reserve has not been called.
//
// The size of the buffer pointed by mData is mCapacity + 1.
// The extra byte is need to store the '\0'.
//
// mCapacity is either mLength or the number of bytes reserved using
// reserve(int)).
//
// mLength is the number of char in the string, excluding the terminating '\0'.
//
// TODO: replace the overflow checks with some better named macros.


// Allocate num + 1number of bytes for the string. Update mCapacity.
// Ensure that mCapacity + 1 and mLength + 1 is accessible.
// In case of error the original state of the string is restored.
// @param num Number of bytes requested. String allocate num + 1 to hold
//            the terminating '\0'.
// @return true if the buffer could be allocated, false otherwise.
bool string::SafeMalloc(size_t num)
{
    // Not empty and no overflow
    if (num > 0 && num + 1 > num)
    {
        char *oldData = mData;

        mData = static_cast<char *>(::malloc(num + 1));
        if (NULL != mData)
        {
            mCapacity = num;
            return true;
        }
        mData = oldData;  // roll back
    }
    return false;
}

// Resize the buffer pointed by mData if num >= mLength.
// mData points to an allocated buffer or the empty string.
// @param num The number of bytes for the internal buffer.
//            Must be > mLength.
void string::SafeRealloc(size_t num)
{
    // Different capacity, at least greater than mLength and no
    // overflow
    if (num != mCapacity && num >= mLength && num + 1 > num)
    {
        char *oldData = mData;

        if (SafeMalloc(num))
        {
            if (mLength > 0)
            {
                memcpy(mData, oldData, mLength + 1);
            }
            mData[mLength] = '\0';
            SafeFree(oldData);
        }
    }
}

void string::SafeFree(char *buffer)
{
    if (buffer != kEmptyString)
    {
        ::free(buffer);
    }
}

// If the memory is on the heap, release it. Do nothing we we point at the empty
// string. On return mData points to str.
void string::ResetTo(char *str)
{
    SafeFree(mData);
    mData = str;
}

void string::ConstructEmptyString()
{
    mData = kEmptyString;
    mLength = 0;
    mCapacity = 0;
}

void string::Constructor(const char *str, size_t num)
{
    Constructor(str, 0, num);
}


void string::Constructor(const char *str, size_t pos, size_t num)
{
    // Enough data and no overflow

    if (SafeMalloc(num))
    {
        memcpy(mData, str + pos, num);
        mLength = num;
        mData[mLength] = '\0';
        return;  // Success
    }
    ConstructEmptyString();
}

void string::Constructor(size_t num, char c)
{
    // Enough data and no overflow

    if (SafeMalloc(num))
    {
        memset(mData, c, num);
        mLength = num;
        mData[mLength] = '\0';
        return;  // Success
    }
    ConstructEmptyString();
}

string::string()
{
    ConstructEmptyString();
}

string::string(const string& str)
{
    Constructor(str.mData, str.mLength);
}

string::string(const string& str, size_t idx, size_t num)
{
    if (idx < str.mLength && num <= (str.mLength - idx))
    {
        Constructor(str.mData + idx , num);
    }
    else
    {
        ConstructEmptyString();
    }
}

string::string(const string& str, size_t idx)
{
    if (idx < str.mLength)
    {
        Constructor(str.mData, idx, str.mLength - idx);
    }
    else
    {
        ConstructEmptyString();
    }
}

string::string(const char *str)
{
    if (NULL != str)
    {
        Constructor(str, strlen(str));
    }
    else
    {
        ConstructEmptyString();
    }
}

string::string(const char *str, size_t num)
{
    Constructor(str, num);
}

// Char repeat constructor.
string::string(size_t num, char c)
{
    Constructor(num, c);
}

string::string(const char *begin, const char *end)
{
    if (begin < end)
    {
        Constructor(begin, end - begin);
    }
    else
    {
        ConstructEmptyString();
    }
}

string::~string()
{
    clear();  // non virtual, ok to call.
}

void string::clear()
{
    mCapacity = 0;
    mLength = 0;
    ResetTo(kEmptyString);
}

void string::Append(const char *str, size_t len)
{
    const size_t total_len = mLength + len;

    // len > 0 and no overflow for the string length + terminating nul.
    if (len > 0 && (total_len + 1) > mLength)
    {
        if (total_len > mCapacity)
        {
            reserve(total_len);
            if (total_len > mCapacity)
            {  // something went wrong in the reserve call.
                return;
            }
        }
        memcpy(mData + mLength, str, len);
        mLength = total_len;
        mData[mLength] = '\0';
    }
}

string& string::append(const char *str)
{
    if (NULL != str)
    {
        Append(str, strlen(str));
    }
    return *this;
}

string& string::append(const char *str, size_t len)
{
    if (NULL != str)
    {
        Append(str, len);
    }
    return *this;
}

string& string::append(const char *str, size_t idx, size_t len)
{
    if (NULL != str)
    {
        Append(str + idx, len);
    }
    return *this;
}

string& string::append(const string& str)
{
    Append(str.mData, str.mLength);
    return *this;
}

void string::push_back(const char c)
{
    // Check we don't overflow.
    if (mLength + 2 > mLength)
    {
        const size_t total_len = mLength + 1;

        if (total_len > mCapacity)
        {
            reserve(total_len);
            if (total_len > mCapacity)
            {  // something went wrong in the reserve call.
                return;
            }
        }
        *(mData + mLength) = c;
        ++mLength;
        mData[mLength] = '\0';
    }
}


int string::compare(const string& other) const
{
    if (this == &other)
    {
        return 0;
    }
    else if (mLength == other.mLength)
    {
        return memcmp(mData, other.mData, mLength);
    }
    else
    {
        return mLength < other.mLength ? -1 : 1;
    }
}

int string::compare(const char *other) const
{
    if (NULL == other)
    {
        return 1;
    }
    return strcmp(mData, other);
}

bool operator==(const string& left, const string& right)
{
    if (&left == &right)
    {
        return true;
    }
    else
    {
        // We can use strcmp here because even when the string is build from an
        // array of char we insert the terminating '\0'.
        return strcmp(left.mData, right.mData) == 0;
    }
}

bool operator==(const string& left, const char *right)
{
    if (NULL == right)
    {
        return false;
    }
    // We can use strcmp here because even when the string is build from an
    // array of char we insert the terminating '\0'.
    return std::strcmp(left.mData, right) == 0;
}

void string::reserve(size_t size)
{
    if (0 == size)
    {
        if (0 == mCapacity)
        {
            return;
        }
        else if (0 == mLength)
        {  // Shrink to fit an empty string.
            mCapacity = 0;
            ResetTo(kEmptyString);
        }
        else
        {  // Shrink to fit a non empty string
            SafeRealloc(mLength);
        }
    }
    // There is no overflow.
    else if (size > mLength)
    {
        SafeRealloc(size);
    }
}

void string::swap(string& other)
{
    if (this == &other) return;
    char *const tmp_mData = mData;
    const size_t tmp_mCapacity = mCapacity;
    const size_t tmp_mLength = mLength;

    mData = other.mData;
    mCapacity = other.mCapacity;
    mLength = other.mLength;

    other.mData = tmp_mData;
    other.mCapacity = tmp_mCapacity;
    other.mLength = tmp_mLength;
}

const char& string::operator[](const size_t idx) const
{
    return mData[idx];
}

char& string::operator[](const size_t idx)
{
    return mData[idx];
}

string& string::assign(const string& str)
{
    clear();
    Constructor(str.mData, str.mLength);
    return *this;
}

string& string::assign(const string& str, size_t pos, size_t len)
{
    if (pos >= str.mLength)
    {  // pos is out of bound
        return *this;
    }
    if (len <= str.mLength - pos)
    {
        clear();
        Constructor(str.mData, pos, len);
    }
    return *this;
}

string& string::assign(const char *str)
{
    if (NULL == str)
    {
        return *this;
    }
    clear();
    Constructor(str, strlen(str));
    return *this;
}

string& string::assign(const char *array, size_t len)
{
    if (NULL == array || 0 == len)
    {
        return *this;
    }
    clear();
    Constructor(array, len);
    return *this;
}

string& string::operator=(char c)
{
    clear();
    Constructor(1, c);
    return *this;
}

}  // namespace std
