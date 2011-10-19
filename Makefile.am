INCLUDES = -I$(top_srcdir)/libusb
noinst_PROGRAMS = lsusb mouse_charge

lsusb_SOURCES = lsusb.c
lsusb_LDADD = ../libusb/libusb-1.0.la -lusb-1.0

mouse_charge_SOURCES = mouse_charge.c
mouse_charge_LDADD = ../libusb/libusb-1.0.la -lusb-1.0

if HAVE_SIGACTION
dpfp_SOURCES = dpfp.c
dpfp_LDADD = ../libusb/libusb-1.0.la -lusb-1.0
noinst_PROGRAMS += dpfp
endif

if THREADS_POSIX
if HAVE_SIGACTION
dpfp_threaded_SOURCES = dpfp_threaded.c
dpfp_threaded_CFLAGS = $(THREAD_CFLAGS) $(AM_CFLAGS)
dpfp_threaded_LDADD = ../libusb/libusb-1.0.la -lusb-1.0
noinst_PROGRAMS += dpfp_threaded
endif
endif

