/*

Copyright 2008 2009 Torbjörn Granlund, Douglas Wikström

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

Based on code from GNU MP Library contributed by John Amanatides and
Torbjörn Granlund.

*/

#include "gmp.h"
#include "gmpmee.h"

int
gmpmee_millerrabin_safe_reps_rs(gmp_randstate_t rstate,
				gmpmee_millerrabin_safe_state state,
				int reps)
{
  int i;
  int res = 1;
  mpz_t nbase;
  mpz_t mbase;
  mpz_t nn_minus_3;
  mpz_t mn_minus_3;

  mpz_init(nn_minus_3);
  mpz_sub_ui(nn_minus_3, state->nstate->n, 3);
  mpz_init(nbase);

  mpz_init(mn_minus_3);
  mpz_sub_ui(mn_minus_3, state->mstate->n, 3);
  mpz_init(mbase);

  /* Repeat an additional time. The union bound then gives the
     expected provable error probability. */
  for (i = 0; i < reps + 1; i++) {

    /* Random base in [2,n-1] */
    mpz_urandomm(nbase, rstate, nn_minus_3);
    mpz_add_ui(nbase, nbase, 2);

    if (gmpmee_millerrabin_once(state->nstate, nbase) == 0)
      {
	res = 0;
	break;
      }

    /* Random base in [2,m-1] */
    mpz_urandomm(mbase, rstate, mn_minus_3);
    mpz_add_ui(mbase, mbase, 2);

    if (gmpmee_millerrabin_once(state->mstate, mbase) == 0)
      {
	res = 0;
	break;
      }
  }

  mpz_clear(mn_minus_3);
  mpz_clear(mbase);

  mpz_clear(nbase);
  mpz_clear(nn_minus_3);

  return res;
}
