#
# MICO --- a free CORBA implementation
# Copyright (C) 1997 Kay Roemer & Arno Puder
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# Send comments and/or bug reports to:
#                mico@informatik.uni-frankfurt.de
#

include ../MakeVars

ifeq ($(USE_WIRELESS), yes)
WIRELESS_DOC=miwco.ps
endif

all: doc.ps $(WIRELESS_DOC) # html
	$(MKFAQ) faq.tex ../FAQ

pic:
	$(MAKE) -C pics

html:
	$(LATEX2HTML) -title "MICO documentation" doc.tex

doc.tex: doc.tex.in ../VERSION
	sed -e s/@VERSION@/`cat ../VERSION`/g \
	  -e s/@DOTVERSION@/$(VERSION)/g < $< > $@ 

doc.dvi: pic mico.ps corba-brand.ps

clean:
	rm -rf _region_.* doc/ auto/ mico.ps doc.ps doc.aux doc.dvi \
		doc.log doc.tex doc.bbl doc.toc doc.lof doc.blg \
		miwco.ps miwco.aux miwco.dvi miwco.log *~
	$(MAKE) -C pics clean

doxygen:
	find html/. -type f -exec rm \{} \;
	cd ..; doxygen

