#///////////////////////////////////////////////////////////////////////////////
# <file type="public">
#
#   <license>
#     This file was originally written by Bradley S. Meyer.
#
#     This is free software; you can redistribute it and/or modify it
#     under the terms of the GNU General Public License as published by
#     the Free Software Foundation; either version 2 of the License, or
#     (at your option) any later version.
#
#     This software is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
#
#     Please see the src/README.txt file in this distribution for more
#     information.
#   </license>
#
# </file>
#///////////////////////////////////////////////////////////////////////////////

SHELL = /bin/sh

#///////////////////////////////////////////////////////////////////////////////
# The following lines must be edited according to where you have
# placed your wn_simple_gce module.  You may also
# edit to choose a different compiler (e.g., g++), to use Valgrind or
# not, or to profile:
#///////////////////////////////////////////////////////////////////////////////

GC=g++

#GCEDIR = ../src/
RAPPDIR = /usr/local/rappture
VALGRIND= yes
PROFILE= no

INCLUDES = -I$(RAPPDIR)/include
LIBS = -L$(RAPPDIR)/lib -lrappture -lm

VENDORDIR = ./vendor
OBJDIR = ./obj

include Makefile.inc

CLIBS += $(LIBS)
CINCLUDE += $(INCLUDES)

EXEC = \
  gce_masses \
  get_omega_from_gas_fraction \
  gce_with_infall_function \
  compute_primary_metallicity \
  compute_species_yield \
  compute_species_yield_parameters \
  compute_species_yield_with_user_function \
  compute_species_mass_fraction \
  compute_species_mass_fraction_vs_age \
  compute_species_mass_fraction_with_user_function \
  radioactivity \
  remainder \
  multi_zone_species \

.PHONY all : $(EXEC)

yes: compute_species_mass_fraction

#===============================================================================
# Compile codes.
#===============================================================================

$(EXEC) : $(OBJS)
	$(CC) -c -o $(TMPDIR)$@.o $@.c
	$(CC) $(OBJS) $(TMPDIR)$@.o  $(CLIBS) -o $@



.PHONY: clean cleanall

clean: 
	rm -fr $(TMPDIR)

cleanall: clean
	rm -f $(EXEC) *.exe
	rm -fr $(VENDORDIR) $(OBJDIR)
	rm -f driver*
