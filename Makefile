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

GC=gcc

#GCEDIR = ../src/
VALGRIND= yes
PROFILE= no

#///////////////////////////////////////////////////////////////////////////////
# End of lines to be edited.
#///////////////////////////////////////////////////////////////////////////////

#VPATH = $(GCEDIR)

CINCLUDE= `xml2-config --cflags` `gsl-config --cflags` -I./
CLIBS= `xml2-config --libs` `gsl-config --libs` -lm

#===============================================================================
# Compiler flags.
#===============================================================================

CFLAGS= -ansi -Werror -Wall -pedantic\
         -Wconversion -Wshadow \
         -Wpointer-arith -Wcast-qual \
         -Wwrite-strings \
         -fshort-enums -fno-common -Dinline= -g \

ifeq ($(GC), gcc) 
	CFLAGS+= -W -Wcast-align -Wmissing-prototypes -Wstrict-prototypes \
                 -Wnested-externs
endif

ifeq ($(GC), g++) 
	CFLAGS+= -W -Wcast-align
endif

ifeq ($(GC), icc)
	CFLAGS+= -wd9 -wd981 -wd1292 -wd1419 -wd10148 -wd10156
endif

ifeq ($(GC), icpc)
	CFLAGS+= -wd9 -wd981 -wd1292 -wd1419 -wd10148 -wd10156
endif

ifeq ($(VALGRIND), yes)
	CFLAGS+= -O0
else
	CFLAGS+= -O2
endif

ifeq ($(PROFILE), yes)
	CFLAGS+= -pg
endif

ifdef WN_DEBUG
  CFLAGS += -DWN_DEBUG
endif

CC=$(GC) $(CFLAGS) $(CINCLUDE)

#===============================================================================
# TMPDIR is the temporary directory for codes compilation, this is where
# object files are created. 
#===============================================================================

TMPDIR = ./tmp/

TMP_DIR := $(shell mkdir -p $(TMPDIR))

#===============================================================================
# Objects.
#===============================================================================

OBJS =  $(TMPDIR)WnSimpleGce.o \
#	$(TMPDIR)my_infall_functions.o \
	$(TMPDIR)my_species_yield_function.o \

$(OBJS): $(TMPDIR)%.o: %.c
	$(CC) -c -o $@ $<

#===============================================================================
# Compile all codes.
#===============================================================================

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

#===============================================================================
# Compile codes.
#===============================================================================

$(EXEC) : $(OBJS)
	$(CC) -c -o $(TMPDIR)$@.o $@.c
	$(CC) $(OBJS) $(TMPDIR)$@.o $(CLIBS) -o $@


.PHONY: clean cleanall

clean: 
	rm -fr $(TMPDIR)

cleanall: clean
	rm -f $(EXEC) *.exe
