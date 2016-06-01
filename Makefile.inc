#///////////////////////////////////////////////////////////////////////////////////#  Copyright (c) 2011-2012 Clemson University.
#//# 
#//#  This file was originally written by Bradley S. Meyer.
#//# 
#//#  This is free software; you can redistribute it and/or modify it
#//#  under the terms of the GNU General Public License as published by
#//#  the Free Software Foundation; either version 2 of the License, or
#//#  (at your option) any later version.
#//# 
#//#  This software is distributed in the hope that it will be useful,
#//#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#//#  GNU General Public License for more details.
#//# 
#//#  You should have received a copy of the GNU General Public License
#//#  along with this software; if not, write to the Free Software
#//#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
#//#  USA
#//# 
#//#///////////////////////////////////////////////////////////////////////////////

#///////////////////////////////////////////////////////////////////////////////
#//!
#//! \file Makefile
#//! \brief A makefile to generate dependencies.
#//!
#///////////////////////////////////////////////////////////////////////////////

ifndef BUILD_DEF

GC=g++

BOOST_DIR = ./
RAPPDIR = /usr/local/rappture/

VALGRIND= yes
PROFILE= no
OPENMP= yes
NODEPRECATED= yes

#///////////////////////////////////////////////////////////////////////////////
# End of lines to be edited.
#///////////////////////////////////////////////////////////////////////////////


CINCLUDE= `xml2-config --cflags` \
          `gsl-config --cflags`  \
          -I$(BOOST_DIR)         \
	  

CLIBS= `xml2-config --libs` `gsl-config --cflags --libs`

#===============================================================================
# Compiler flags.
#===============================================================================

CFLAGS= -ansi -Werror -Wall \
         -Wpointer-arith -Wcast-qual \
         -Wwrite-strings -Wconversion \
         -fshort-enums -fno-common -g \

ifeq ($(GC), icpc)
	CFLAGS+= -wd9 -wd981 -wd1292 -wd1418 -wd1419 -wd10148 -wd10156
endif

ifeq ($(VALGRIND), yes)
	CFLAGS+= -O0
else
	CFLAGS+= -O2
endif

ifeq ($(PROFILE), yes)
	CFLAGS+= -pg
endif

ifeq ($(OPENMP), yes)
  ifeq ($(GC), g++)
	CFLAGS+= -fopenmp
  endif
  ifeq ($(GC), icpc)
	CFLAGS+= -openmp
 endif
endif

ifeq ($(NODEPRECATED), yes)
	CFLAGS+= -Wno-deprecated
endif

CC=$(GC) $(CFLAGS) $(CINCLUDE)

#===============================================================================
# OBJDIR is the temporary directory for codes compilation, this is where object files are created. 
#===============================================================================

ifndef OBJDIR
OBJDIR = ../../obj
endif
OBJ_DIR := $(shell mkdir -p $(OBJDIR))

#===============================================================================
# BINDIR is the temporary directory for code executables.
#===============================================================================

ifndef BINDIR
BINDIR = .
endif
BIN_DIR := $(shell mkdir -p $(BINDIR))

#===============================================================================
# VENDORDIR is the directory for storing webnucleo codes.
#===============================================================================

ifndef VENDORDIR
VENDORDIR = ../../vendor
endif
VENDOR_DIR := $(shell mkdir -p $(VENDORDIR))

#===============================================================================
# Build dependencies.
#===============================================================================



WN_OBJ =$(OBJDIR)/WnSimpleGce.o		\
	$(OBJDIR)/my_infall_functions.o	\
	$(OBJDIR)/my_species_yield_function.o	\

#===============================================================================
# End include.
#===============================================================================

BUILD_DEF = yes

endif

