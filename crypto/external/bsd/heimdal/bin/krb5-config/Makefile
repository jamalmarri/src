#	$NetBSD: Makefile,v 1.5 2020/07/23 07:53:35 mrg Exp $

.include <bsd.own.mk>
.include <${.CURDIR}/../../Makefile.inc>

.PATH:          ${HEIMDIST}/tools

SCRIPTS=	krb5-config
MAN=		krb5-config.1

CLEANFILES+=	krb5-config

VERSION!= ${TOOL_SED} -ne 's/.*PACKAGE_VERSION[ 	]*"\([0-9.]*\)"/\1/p' ${HEIMBASE}/include/config.h

krb5-config: krb5-config.in
	${TOOL_SED} \
		-e "s!@PACKAGE\@!heimdal!g" \
	        -e "s!@VERSION\@!${VERSION}!g" \
	        -e "s!@prefix\@!/!g" \
	        -e "s!@exec_prefix\@!/!g" \
	        -e "s!@libdir\@!/usr/lib!g" \
	        -e "s!@includedir\@!/usr/include!g" \
	        -e "s!@LIB_crypt\@!-lcrypt!g" \
	        -e "s!@LIB_dbopen\@!!g" \
	        -e "s!@INCLUDE_hcrypto\@!!g" \
	        -e "s!@LIB_hcrypto_appl\@!-lcrypto!g" \
	        -e "s!@LIB_dlopen\@!!g" \
	        -e "s!@LIB_door_create\@!!g" \
	        -e "s!@LIB_pkinit\@!-lhx509!g" \
	        -e "s!@PTHREAD_LIBADD\@!!g" \
	        -e "s!@LIBS\@!!g" \
	        ${HEIMDIST}/tools/krb5-config.in | sh > $@
	chmod +x $@

.include <bsd.prog.mk>
