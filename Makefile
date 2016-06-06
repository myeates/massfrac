# simple makefile for a Rappture-based program

RAPPTURE_DIR	= /usr/local/rappture/
INCLUDES	= -I$(RAPPTURE_DIR)/include
LIBS		= -L$(RAPPTURE_DIR)/lib -lrappture -lm

VENDORDIR = ./vendor
OBJDIR = ./obj

include Makefile.inc

EXEC = compute_species_mass_fraction \

$(EXEC) : $(WN_OBJ)
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $(OBJDIR)/$@.o $@.c
	$(CC) $(WN_OBJ) $(OBJDIR)/$@.o $(CLIBS) -o $@ $(LIBS) $(CLIBS)

all: $(EXEC)

cleanruns:
	$(RM) run*.xml driver*.xml

cleanrappture: cleanruns
	$(RM) $(EXEC)

cleanall: cleanrappture
	rm -fr $(VENDORDIR) $(OBJDIR)
