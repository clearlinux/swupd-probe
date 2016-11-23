AM_CFLAGS = \
	-fstack-protector -Wall -pedantic \
	-Wstrict-prototypes -Wundef -fno-common \
	-Werror-implicit-function-declaration \
	-Wformat -Wformat-security -Werror=format-security \
	-Wno-conversion -Wunused-variable -Wunreachable-code \
	-Wall -W -D_FORTIFY_SOURCE=2 -std=c11

bin_PROGRAMS = \
	swupd-probe

swupd_probe_SOURCES = \
	swupd-probe.c

EXTRA_DIST = \
	COPYING \
	swupd-probe.md \
	swupd-probe.path \
	swupd-probe.service

dist_man_MANS = \
	swupd-probe.1

DISTCHECK_CONFIGURE_FLAGS = \
	--with-systemdsystemunitdir=$$dc_install_base/$(systemdsystemunitdir) \
	--with-systemdtmpfilesdir=$$dc_install_base/$(tmpfilesdir)

systemdsystemunit_DATA = \
	swupd-probe.service \
	swupd-probe.path

swupd-probe.1: swupd-probe.md
	ronn swupd-probe.md
	rm -f swupd-probe.1.html
