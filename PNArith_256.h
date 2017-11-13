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
#include "BRInt.h"

#ifdef __cplusplus
//extern "C" {
#endif

#define WIDTH 8

UInt256 u64_to_u256 ( uint64_t num ) {
        UInt256 newU = UINT256_ZERO;
        newU.u32[0] = (uint32_t)num;
        newU.u32[1] = (uint32_t)(num >> 32);
        for ( int i = 2; i < WIDTH; i++ )
                newU.u32[i] = 0;
        return newU; 
}

UInt256 setCompact ( uint32_t blockTarget ) {
        const uint32_t size = blockTarget >> 24;
        const uint32_t target = blockTarget & 0x00ffffff;

        UInt256 result = UINT256_ZERO;

        if (size <= 3) {
                target >>= 8 * (3 - size);
                result = u64_to_u256( target );
        }
        else {
                result = u64_to_u256( target );
                result = leftShift(t, 8 * (size - 3));
        }

        return result;
}

UInt256 leftShift( UInt256 lhs, uint32_t shift ) {
        uint32_t a[WIDTH];
        for ( uint32_t i = 0; i < WIDTH; i++ )
                a[i] = lhs.u32[i];
        for ( uint32_t l = 0; l < WIDTH; l++ )
                lhs.u32[i] = 0;

        uint32_t k = shift / 32;
        shift = shift % 32;

        for ( uint32_t j = 0; j < WIDTH; j++ ) {
                if ( (j + k + 1) < WIDTH && shift != 0 )
                        lhs.u32[ j + k + 1 ] |= (a[i] >> (32 - shift) );
                if ( j + k < WIDTH )
                        lhs.u32[ j + k ] |= (a[i] << shift);
        }
        return lhs;
}

UInt256 rightShift( UInt256 lhs, uint32_t shift ) {
        uint32_t a[WIDTH];
        for ( uint32_t i = 0; i < WIDTH; i++ )
                a[i] = lhs.u32[i];
        for ( uint32_t l = 0; l < WIDTH; l++ )
                lhs.u32[i] = 0;

        uint32_t k = shift / 32;
        shift = shift % 32;
        for( uint32_t j = 0; j < WIDTH; j++ ) {
                if ( ( j - k - 1 ) >= 0 && shift != 0 )
                        lhs.u32[ j - k - 1 ] |= (a[i] << (32 - shift));
                if ( j - k >= 0 )
                        lhs.u32[ j - k ] |= (a[i] >> shift);
        }
        return lhs;
}

UInt256 stdMultiply( UInt256 lhs, uint64_t factor ) {
        uint64_t carry = 0;
        for (int i = 0; i < WIDTH; i++) {
                uint64_t n = carry + (uint32_t)factor * lhs.u32[i];
                lhs.u32[i] = n & 0xffffffff;
                carry = n >> 32;
        }
        return lhs;
}

UInt256 stdAdd( UInt256 lhs, uint64_t summand ) {
        UInt256 sum = u64_to_u256( summand );
        lhs = arithAdd( lhs, sum );
        return lhs;
}

UInt256 stdDivide( UInt lhs, uint64_t divisor ) {
        UInt256 newDivisor = u64_to_u256( divisor );
        UInt256 div = newDivisor;
        UInt256 num = lhs;
        lhs = u64_to_u256( 0 );

        if ( div_bits == 0 || divisor == 0 ) {
                printf("Arith Error: Division by zero.\n");
                return UINT256_ZERO;
        }
        if ( div_bits > num_bits)
                return lhs;

        uint32_t shift = num_bits - div_bits;
        div = leftShift( div, shift );

        while( shift >= 0 ) {
                if (compareTo(num, div) == 1 || compareTo(num, div) == 0) {
                        num = arithSub( num, div );
                        lhs.u32[shift / 32] |= (1 << (shift & 31));
                }
                div = rightShift( div, 1 );
                shift--;
        }
        return lhs;
}

uint32_t compareTo( UInt256 lhs, UInt256 rhs ) {
        for (int i = WIDTH - 1; i >= 0; i--) {
                if (lhs.u32[i] < rhs.u32[i])
                        return -1;
                if (lhs.u32[i] > rhs.u32[i])
                        return 1;
        }
        return 0;
}

UInt256 hyphen( UInt256 lhs ) {
        UInt256 rhs = UINT256_ZERO;
        for (int i = 0; i < WIDTH; i++)
                rhs.u32[i] = ~lhs.u32[i];
        return prefix( rhs );
}

UInt256 prefix( UInt256 rhs ) {
        uint32_t i = 0;
        while (++rhs.u32[i] == 0 && i < WIDTH - 1)
                i++;
        return rhs;
}

UInt256 arithAdd( UInt256 lhs, UInt256 rhs ) {
        uint64_t carry = 0;
        for (int i = 0; i < WIDTH; i++) {
                uint64_t n = carry + lhs.u32[i] + rhs.u32[i];
                lhs.u32[i] = n & 0xffffffff;
                carry = n >> 32;
        }
        return lhs;
}

UInt256 arithSub( UInt256 lhs, UInt256 rhs ) {
        lhs = arithAdd( lhs, hyphen( rhs ) );
        return lhs;
}

uint32_t bits( UInt256 pn ) {
        for (int pos = WIDTH - 1; pos >= 0; pos--) {
                if (pn.u32[pos]) {
                        for (int bits = 31; bits > 0; bits--) {
                                if (pn.u32[pos] & 1 << bits)
                                        return 32 * pos + bits + 1;
                        }
                        return 32 * pos + 1;
                }
        }
        return 0;
}
