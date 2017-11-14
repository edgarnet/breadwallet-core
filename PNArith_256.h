//
//  PNArith_256.h
//
//  Created by Edgar Perez on 11/13/17.
//  Copyright (c) 2017 panwallet LLC.
//  Copyright (c) 2015 breadwallet LLC.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE

#ifndef PNArith_256_h
#define PNArith_256_h

#include <inttypes.h>
#include <assert.h>
#include "BRInt.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WIDTH 8

UInt256 u64_to_u256 ( uint64_t num );

UInt256 setCompact ( uint32_t blockTarget );

uint32_t getCompact ( UInt256 lhs );

uint64_t getLow64 ( UInt256 lhs );

UInt256 leftShift( UInt256 lhs, uint32_t shift );

UInt256 rightShift( UInt256 lhs, uint32_t shift );

UInt256 stdMultiply( UInt256 lhs, uint64_t factor );

UInt256 stdAdd( UInt256 lhs, uint64_t summand );

UInt256 stdDivide( UInt256 lhs, uint64_t divisor );

uint32_t compareTo( UInt256 lhs, UInt256 rhs );

UInt256 hyphen( UInt256 lhs );

UInt256 prefix( UInt256 rhs );

UInt256 arithAdd( UInt256 lhs, UInt256 rhs );

UInt256 arithSub( UInt256 lhs, UInt256 rhs );

uint32_t bits( UInt256 pn );

#ifdef __cplusplus
}
#endif

#endif // BRInt_h
