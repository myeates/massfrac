/*//////////////////////////////////////////////////////////////////////////////
// <file type="public">
//   <license>
//      Copyright (c) 2006-2013 Clemson University.
//
//      This distribution directory contains the source code
//      for the Clemson Webnucleo group's
//      wn_simple_gce module, originally developed by Bradley S. Meyer.
//
//      This is free software; you can redistribute it and/or modify it
//      under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This software is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this software (please see the "gnu_gpl.txt" file in the doc/
//      directory of this distribution); if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
//      USA
//
//      All wn_simple_gce documentation is free documentation; permission is
//      granted to copy, distribute and/or modify the documentation under the
//      terms of the GNU Free Documentation License, Version 1.2 or any later
//      version published by the Free Software Foundation.  A copy of the
//      license is included in the file "gnu_fdl.txt" in the doc/ directory
//      in this distribution.
//   </license>
//
//   <description>
//     <abstract>
//       README file for the wn_simple_gce src/ directory.  The src/ directory
//       contains WnSimpleGce.h, the wn_simple_gce header file,
//       WnSimpleGce.c, the collection of wn_simple_gce codes,
//       and the directory examples/ containing the example codes.
//     </abstract>
//     <keywords>
//       README, wn_simple_gce, code, source
//     </keywords>
//   </description>
//
//   <authors>
//     <current>
//       <author userid="mbradle" start_date="2008/08/30" />
//     </current>
//     <previous>
//     </previous>
//   </authors>
//
// </file>
//////////////////////////////////////////////////////////////////////////////*/

################################################################################
# Overview of directory contents.
################################################################################

./README.txt

  Overview of directory contents and compatibility notes.

WnSimpleGce.c

  The wn_simple_gce C code in the distribution.

WnSimpleGce.h

  The header file for WnSimpleGce.c

examples/

  The directory containing the example codes in the distribution.

################################################################################
# Dependencies.
################################################################################

wn_simple_gce codes require libxml2 version 2.6.18 or later.  Earlier versions
of libxml2 may also work, but we haven't tested them.  To check whether
libxml2 is installed on a particular unix or linux system, type:

xml2-config --version

To install libxml2, please see http://www.libxml.org.

wn_simple_gce codes also require gsl version 1.9 or later.  To check whether
gsl is installed on a particular unix or linux system, type:

gsl-config --version

To install gsl, please see http://www.gnu.org/software/gsl/.

################################################################################
# Compatibility notes for the general user.
################################################################################

We've tested with gnu gcc 3.4.4 and Cygwin 1.5.24-2.

