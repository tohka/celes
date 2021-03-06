#include "sofam.h"

int iauPvstar(double pv[2][3], double *ra, double *dec,
              double *pmr, double *pmd, double *px, double *rv)
/*
**  - - - - - - - - - -
**   i a u P v s t a r
**  - - - - - - - - - -
**
**  Convert star position+velocity vector to catalog coordinates.
**
**  Status:  support function.
**
**  Given (Note 1):
**     pv     double[2][3]   pv-vector (AU, AU/day)
**
**  Returned (Note 2):
**     ra     double         right ascension (radians)
**     dec    double         declination (radians)
**     pmr    double         RA proper motion (radians/year)
**     pmd    double         Dec proper motion (radians/year)
**     px     double         parallax (arcsec)
**     rv     double         radial velocity (km/s, positive = receding)
**
**  Returned (function value):
**            int            status:
**                              0 = OK
**                             -1 = superluminal speed (Note 5)
**                             -2 = null position vector
**
**  Notes:
**
**  1) The specified pv-vector is the coordinate direction (and its rate
**     of change) for the date at which the light leaving the star
**     reached the solar-system barycenter.
**
**  2) The star data returned by this function are "observables" for an
**     imaginary observer at the solar-system barycenter.  Proper motion
**     and radial velocity are, strictly, in terms of barycentric
**     coordinate time, TCB.  For most practical applications, it is
**     permissible to neglect the distinction between TCB and ordinary
**     "proper" time on Earth (TT/TAI).  The result will, as a rule, be
**     limited by the intrinsic accuracy of the proper-motion and
**     radial-velocity data;  moreover, the supplied pv-vector is likely
**     to be merely an intermediate result (for example generated by the
**     function iauStarpv), so that a change of time unit will cancel
**     out overall.
**
**     In accordance with normal star-catalog conventions, the object's
**     right ascension and declination are freed from the effects of
**     secular aberration.  The frame, which is aligned to the catalog
**     equator and equinox, is Lorentzian and centered on the SSB.
**
**     Summarizing, the specified pv-vector is for most stars almost
**     identical to the result of applying the standard geometrical
**     "space motion" transformation to the catalog data.  The
**     differences, which are the subject of the Stumpff paper cited
**     below, are:
**
**     (i) In stars with significant radial velocity and proper motion,
**     the constantly changing light-time distorts the apparent proper
**     motion.  Note that this is a classical, not a relativistic,
**     effect.
**
**     (ii) The transformation complies with special relativity.
**
**  3) Care is needed with units.  The star coordinates are in radians
**     and the proper motions in radians per Julian year, but the
**     parallax is in arcseconds; the radial velocity is in km/s, but
**     the pv-vector result is in AU and AU/day.
**
**  4) The proper motions are the rate of change of the right ascension
**     and declination at the catalog epoch and are in radians per Julian
**     year.  The RA proper motion is in terms of coordinate angle, not
**     true angle, and will thus be numerically larger at high
**     declinations.
**
**  5) Straight-line motion at constant speed in the inertial frame is
**     assumed.  If the speed is greater than or equal to the speed of
**     light, the function aborts with an error status.
**
**  6) The inverse transformation is performed by the function iauStarpv.
**
**  Called:
**     iauPn        decompose p-vector into modulus and direction
**     iauPdp       scalar product of two p-vectors
**     iauSxp       multiply p-vector by scalar
**     iauPmp       p-vector minus p-vector
**     iauPm        modulus of p-vector
**     iauPpp       p-vector plus p-vector
**     iauPv2s      pv-vector to spherical
**     iauAnp       normalize angle into range 0 to 2pi
**
**  Reference:
**
**     Stumpff, P., 1985, Astron.Astrophys. 144, 232-240.
**
**  This revision:  2008 May 18
**
**  Original version 2012-03-01
**
**  Copyright (C) 2013 Naoki Arita.  See notes at end.
*/
{
   double r, x[3], vr, ur[3], vt, ut[3], bett, betr, d, w, del,
          usr[3], ust[3], a, rad, decd, rd;


/* Isolate the radial component of the velocity (AU/day, inertial). */
   iauPn(pv[0], &r, x);
   vr = iauPdp(x, pv[1]);
   iauSxp(vr, x, ur);

/* Isolate the transverse component of the velocity (AU/day, inertial). */
   iauPmp(pv[1], ur, ut);
   vt = iauPm(ut);

/* Special-relativity dimensionless parameters. */
   bett = vt / DC;
   betr = vr / DC;

/* The inertial-to-observed correction terms. */
   d = 1.0 + betr;
   w = 1.0 - betr*betr - bett*bett;
   if (d == 0.0 || w < 0) return -1;
   del = sqrt(w) - 1.0;

/* Apply relativistic correction factor to radial velocity component. */
   w = (betr != 0) ? (betr - del) / (betr * d) : 1.0;
   iauSxp(w, ur, usr);

/* Apply relativistic correction factor to tangential velocity */
/* component.                                                  */
   iauSxp(1.0/d, ut, ust);

/* Combine the two to obtain the observed velocity vector (AU/day). */
   iauPpp(usr, ust, pv[1]);

/* Cartesian to spherical. */
   iauPv2s(pv, &a, dec, &r, &rad, &decd, &rd);
   if (r == 0.0) return -2;

/* Return RA in range 0 to 2pi. */
   *ra = iauAnp(a);

/* Return proper motions in radians per year. */
   *pmr = rad * DJY;
   *pmd = decd * DJY;

/* Return parallax in arcsec. */
   *px = DR2AS / r;

/* Return radial velocity in km/s. */
   *rv = 1e-3 * rd * DAU / DAYSEC;

/* OK status. */
   return 0;

/*----------------------------------------------------------------------
**
**  Celes is a wrapper of the SOFA Library for Ruby.
**
**  This file is redistributed and relicensed in accordance with 
**  the SOFA Software License (http://www.iausofa.org/tandc.html).
**
**  The original library is available from IAU Standards of
**  Fundamental Astronomy (http://www.iausofa.org/).
**
**
**
**
**
**  Copyright (C) 2013, Naoki Arita
**  All rights reserved.
**
**  Redistribution and use in source and binary forms, with or without
**  modification, are permitted provided that the following conditions
**  are met:
**
**  1 Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
**
**  2 Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in
**    the documentation and/or other materials provided with the
**    distribution.
**
**  3 Neither the name of the Standards Of Fundamental Astronomy Board,
**    the International Astronomical Union nor the names of its
**    contributors may be used to endorse or promote products derived
**    from this software without specific prior written permission.
**
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
**  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
**  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
**  FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
**  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
**  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
**  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
**  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
**  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
**  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
**  POSSIBILITY OF SUCH DAMAGE.
**
**--------------------------------------------------------------------*/
}
