/*
 * Copyright (C) 1999  Internet Software Consortium.
 * 
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

#include <config.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <isc/types.h>
#include <isc/assertions.h>
#include <isc/error.h>
#include <isc/sockaddr.h>

isc_boolean_t
isc_sockaddr_equal(isc_sockaddr_t *a, isc_sockaddr_t *b)
{
	REQUIRE(a != NULL && b != NULL);

	if (a->length != b->length)
		return (ISC_FALSE);

	if (memcmp(&a->type, &b->type, a->length) != 0)
		return (ISC_FALSE);

	return (ISC_TRUE);
}

unsigned int
isc_sockaddr_hash(isc_sockaddr_t *sockaddr, isc_boolean_t address_only) {
	unsigned int length;
	const unsigned char *s;
	unsigned int h = 0;
	unsigned int g;
	
	/*
	 * Provide a hash value for 'sockaddr'.
	 */

	REQUIRE(sockaddr != NULL);

	if (address_only) {
		switch (sockaddr->type.sa.sa_family) {
		case AF_INET:
			return (ntohl(sockaddr->type.sin.sin_addr.s_addr));
		case AF_INET6:
			s = (unsigned char *)&sockaddr->type.sin6.sin6_addr;
			length = sizeof sockaddr->type.sin6.sin6_addr;
			break;
		default:
			UNEXPECTED_ERROR(__FILE__, __LINE__,
					 "unknown address family: %d\n",
					 (int)sockaddr->type.sa.sa_family);
			s = (unsigned char *)&sockaddr->type;
			length = sockaddr->length;
		}
	} else {
		s = (unsigned char *)&sockaddr->type;
		length = sockaddr->length;
	}

	while (length > 0) {
		h = ( h << 4 ) + *s;
		if ((g = ( h & 0xf0000000 )) != 0) {
			h = h ^ (g >> 24);
			h = h ^ g;
		}
		s++;
		length--;
	}

	return (h);
}
