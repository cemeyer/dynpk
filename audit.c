/* Prevent dynamic libraries from being loaded from places we don't want */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <link.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

unsigned int la_version( unsigned int version )
{
	return 1;
}

unsigned int la_objopen( struct link_map *lmp, Lmid_t lmid, unsigned int *cookie )
{
	char *prefix = getenv("AUDIT_PREFIX");
	char *absname;

	if( prefix == NULL ) {
		fprintf( stderr, "ERROR: No audit prefix specified in AUDIT_PREFIX environment variable\n" );
		exit(1);
	}

	if( lmp->l_name[0] != 0 ) {
		absname = canonicalize_file_name( lmp->l_name );
		assert( absname != NULL );

		if( strncmp( absname, prefix, strlen(prefix) ) != 0 ) {
			fprintf( stderr, "ERROR:\tAttempted to load dynamic library outside allowed prefix\n" );
			fprintf( stderr, "\tApplication attempted to load \"%s\"\n", lmp->l_name );
			fprintf( stderr, "\n\tAborting\n");
			exit(1);
		}

		free(absname);
	}

	if( getenv( "AUDIT_DEBUG" ) )
		printf( "file: \"%s\" loaded\n", lmp->l_name );
	return LA_FLG_BINDTO | LA_FLG_BINDFROM;
}
