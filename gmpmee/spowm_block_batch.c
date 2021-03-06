/*

Copyright 2008 2009 Torbj�rn Granlund, Douglas Wikstr�m

This file is part of GMP Modular Exponentiation Extension (GMPMEE).

GMPMEE is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

GMPMEE is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public
License along with GMPMEE.  If not, see
<http://www.gnu.org/licenses/>.

*/

#include "gmp.h"
#include "gmpmee.h"

void
gmpmee_spowm_block_batch(mpz_t rop, mpz_t *bases, mpz_t *exponents, size_t len,
			 mpz_t modulus, size_t block_width, size_t batch_len)
{
  int i;
  gmpmee_spowm_tab table;
  mpz_t tmp;
  mpz_init(tmp);

  gmpmee_spowm_init(table, batch_len, modulus, block_width);

  mpz_set_ui(rop, 1);
  for (i = 0; i < len; i += batch_len)
    {

      /* Last batch may be slightly shorter. */
      if (len - i < batch_len)
	{
	  batch_len = len - i;
	  gmpmee_spowm_clear(table);
	  gmpmee_spowm_init(table, batch_len, modulus, block_width);
	}

      /* Perform computation for batch */
      gmpmee_spowm_precomp(table, bases);

      /* Compute batch. */
      gmpmee_spowm_table(tmp, table, exponents);

      /* Multiply with result so far. */
      mpz_mul(rop, rop, tmp);
      mpz_mod(rop, rop, modulus);

      /* Move on to next batch. */
      bases += batch_len;
      exponents += batch_len;
    }
  mpz_clear(tmp);
  gmpmee_spowm_clear(table);
}
