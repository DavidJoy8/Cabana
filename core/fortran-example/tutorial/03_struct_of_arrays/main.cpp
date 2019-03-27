/****************************************************************************
 * Copyright (c) 2018-2019 by the Cabana authors                            *
 * All rights reserved.                                                     *
 *                                                                          *
 * This file is part of the Cabana library. Cabana is distributed under a   *
 * BSD 3-clause license. For the licensing terms see the LICENSE file in    *
 * the top-level directory.                                                 *
 *                                                                          *
 * SPDX-License-Identifier: BSD-3-Clause                                    *
 ****************************************************************************/

#include <Cabana_Core.hpp>

#include <iostream>

#include "veclen.h"

//---------------------------------------------------------------------------//
// SoA example.
//---------------------------------------------------------------------------//
    /*
      Cabana structs-of-arrays (SoAs) are groups of Tuples with the data
      specified in an order such that the array (or tuple) index is stride-1
      in memory. This results in each dimension of each multidimensional tuple
      member being stored in a contiguous memory block the size of the number
      of tuples. The number of tuples in an SoA is defined as the "vector
      length" - this defines the size of the array.

      For example, consider declaring the following member type to store two
      arrays of doubles in each tuple:

          using types = Cabana::MemberTypes<double[3][2],double[4],float>;

      and vector length:

          const int veclen = 8;

      Declaring a Cabana SoA as Cabana::SoA<types,veclen> gives the equivalent
      data layout:

          struct MyEquivalentSoA
          {
              double d0[3][2][veclen];
              double d1[4][veclen];
              float d2[veclen];
          };

      Note: The data in this struct definition with an equivalent memory
      layout is stride-1 in the array index.

      Note: When defined as a compile-time argument the vector length must be
      a power of 2. Vector lengths that are not a power of 2 will emit a
      compiler error.

      Note: The members in an SoA-equivalent struct are in the same order as
      they are declared in Cabana::MemberTypes.
    */

    /* Start by declaring the types in our tuples will store. Store a rank-2
       array of doubles, a rank-1 array of floats, and a single integer in
       each tuple.
    */
    using DataTypes = Cabana::MemberTypes<double[3][3],
                                          float[4],
                                          int>;

    /*
      Next declare the vector length of our SoA. This is how many tuples the
      SoA will contain. A reasonable number for performance should be some
      multiple of the vector length on the machine you are using.
    */
    const int VectorLength = veclen;

    /* Create the SoA. */
    using SoaTYPE = Cabana::SoA<DataTypes,VectorLength>;

SoaTYPE * particle = new SoaTYPE;

extern "C" {
  void soaExample(SoaTYPE *);
  void delete_soa();
}

void delete_soa(){
  delete particle;
}


//---------------------------------------------------------------------------//
// Main.
//---------------------------------------------------------------------------//
int main( int argc, char* argv[] )
{
    Kokkos::ScopeGuard scope_guard(argc, argv);

    soaExample(particle);

    return 0;
}

//---------------------------------------------------------------------------//
