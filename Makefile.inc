#///////////////////////////////////////////////////////////////////////////////
#  Copyright (c) 2016 Clemson University.
# 
#  This file was originally written by Bradley S. Meyer and Jacob Covington.
# 
#  This is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
# 
#  This software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with this software; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
#  USA
# 
#///////////////////////////////////////////////////////////////////////////////

#///////////////////////////////////////////////////////////////////////////////
#//!
#//! \file Makefile
#//! \brief A makefile to generate dependencies.
#//!
#///////////////////////////////////////////////////////////////////////////////

ifndef BUILD_DEF

GC=g++

WN_SIMPLE_GCE_VERSION = 0.8

SOURCEFORGE_URL = http://sourceforge.net/projects/

WN_SIMPLE_GCE_URL = $(SOURCEFORGE_URL)/wn-simple-gce/files/wn_simple_gce

WN_SIMPLE_GCE_DIR = $(VENDORDIR)/wn_simple_gce/$(WN_SIMPLE_GCE_VERSION)

#///////////////////////////////////////////////////////////////////////////////
# End of lines to be edited.
#///////////////////////////////////////////////////////////////////////////////

VPATH= $(WN_SIMPLE_GCE_DIR)/src:$(WN_SIMPLE_GCE_DIR)/examples

CINCLUDE= `xml2-config --cflags`          \
          `gsl-config --cflags`           \
          -I$(WN_SIMPLE_GCE_DIR)/src      \
          -I$(WN_SIMPLE_GCE_DIR)/examples \

CLIBS= `xml2-config --libs` `gsl-config --libs`

#===============================================================================
# Compiler flags.
#===============================================================================

CFLAGS=  -Werror -Wall \
         -Wconversion \
         -Wpointer-arith -Wcast-qual \
         -Wwrite-strings \
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

ifdef WN_XML_CHAR
  CFLAGS += -DWN_XML_CHAR=$(WN_XML_CHAR)
endif


CC=$(GC) $(CFLAGS) $(CINCLUDE)

#===============================================================================
# OBJDIR is the temporary directory for codes compilation, this is where
# object files are created. 
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

$(VENDORDIR)/wn_simple_gce.tar.gz:
	wget $(WN_SIMPLE_GCE_URL)/wn_simple_gce_$(WN_SIMPLE_GCE_VERSION).tar.gz -O $@

$(OBJDIR)/WnSimpleGce.o: $(VENDORDIR)/wn_simple_gce.tar.gz 
	tar xz -C $(VENDORDIR) -f $<
	$(CC) -c $(WN_SIMPLE_GCE_DIR)/src/WnSimpleGce.c -o $@
#--------------------------------------------------------------------------

WN_OBJ =$(OBJDIR)/WnSimpleGce.o		\

#===============================================================================
# Clean up. 
#===============================================================================

.PHONY: clean clean_all clean_dist

#clean:
#	rm -fr $(OBJDIR)

clean_dist:
	rm -fr $(VENDORDIR)

clean_all: clean clean_dist

clean_data:
	rm -fr $(DATA_DIR)

#===============================================================================
# End include.
#===============================================================================

BUILD_DEF = yes

endif

