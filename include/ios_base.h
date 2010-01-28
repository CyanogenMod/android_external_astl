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

#ifndef ANDROID_ASTL_IOS_BASE_H__
#define ANDROID_ASTL_IOS_BASE_H__

#include <ios_pos_types.h>

namespace std {

/**
 * Root of the streams inheritance.
 * The STL defines ios_base as a template with 2 params char types and
 * traits. We support only char and no traits.
 * ios_base defines flags, types and fields to hold these values.
 * ios_base is extended by basic_ios which wraps a streambuf and
 * provides common methods for all streams.
 * The only mode supported for the standards streams (cout, cerr, cin)
 * is synced with stdio.
 */

class ios_base
{
  public:
    typedef int io_state;
    typedef int open_mode;
    typedef int seek_dir;
    typedef std::streampos streampos;
    typedef std::streamoff streamoff;


  protected:
    ios_base();

  public:
    virtual ~ios_base();

    /**
     * @return The precision (number of digits after the decimal
     * point) to generate on certain output conversions. 6 by default.
     */
    streamsize precision() const { return mPrecision; }

    /**
     *  @param precision The new precision value. Values < 0 are ignored.
     *  @return The previous value of precision(). 0 by default;
     */
    streamsize precision(streamsize precision);

    /**
     * @return The minimum field width to generate on output
     * operations.
     */
    streamsize width() const { return mWidth; }

    /**
     *  @param width The new width value. Values < 0 are ignored.
     *  @return The previous value of width().
     */
    streamsize width(streamsize width);

    // Helper class to initialize the standard streams. Its
    // construction ensures the initialization of the stdio
    // streams (cout, cerr,...) declared in iostream.
    // The destruction of the last instance of this class will flush
    // the streams.
    class Init {
      public:
        Init();
        ~Init();

        static bool done() { return sDone; }
      private:
        static int sGuard;
        static bool sDone;
    };

  private:
    streamsize mPrecision;
    streamsize mWidth;
};

}  // namespace std

#endif
