SUMMARY = "CUPS v3 Libraries"
HOMEPAGE = "https://openprinting.github.io/cups/cups3.html"
SECTION = "console/utils"
LICENSE = "Apache-2.0"
DEPENDS = "avahi openssl zlib"

SRC_URI = "https://github.com/OpenPrinting/libcups/releases/download/v${PV}/libcups-${PV}.tar.gz"

inherit autotools-brokensep binconfig pkgconfig

CLEANBROKEN = "1"
EXTRA_AUTORECONF += "--exclude=autoheader"
EXTRA_OECONF = "--with-tls=openssl --prefix=/usr --sysconfdir=${CUPS_SYSCONFDIR}"

PACKAGES =+ "${PN}-tools"

FILES_${PN} = "${libdir}/libcups3.so.* ${CUPS_SYSCONFDIR}"
FILES_${PN}-tools = " \
	/usr/bin/* \
	/usr/share/libcups3/* \
"

RDEPENDS_${PN} = "avahi-daemon openssl libavahi-client zlib"

# Override autotools_do_configure to prevent removal of the configure script...
autotools_do_configure() {
	DSOFLAGS="${LDFLAGS}"
	export DSOFLAGS
	oe_runconf
}

# Override install to create the "ssl" directory used for certificates...
do_install () {
	oe_runmake "DSTROOT=${D}" install

	# Create CUPS_SYSCONFDIR/ssl directory
	install -d -m 755 ${D}${CUPS_SYSCONFDIR}
	install -d -m 700 ${D}${CUPS_SYSCONFDIR}/ssl
}
