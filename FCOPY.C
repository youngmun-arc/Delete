#include <stdio.h>

char buf[30000];

void main( int argc, char *argv[] )
{
    int read_byte, write_byte;
    FILE *src, *dest;

    if ( argc < 3 )
    {
        printf( "Usage :   FCOPY filename filename\n\n" );
        return;
    }

    src = fopen( argv[1], "rb" );
    if ( src == NULL )
    {
        printf( "File not found.\n" );
        return;
    }
    dest = fopen( argv[2], "wb" );
    if ( dest == NULL )
    {
        printf( "File write error.\n" );
        fclose( src );
        return;
    }

    printf( "Copy %s to %s \n", argv[1], argv[2] );

    for (;;)
    {
	putchar( '' );
        read_byte = fread( buf, 1, sizeof(buf), src );
        if ( read_byte == 0 ) break;

        write_byte = fwrite( buf, 1, read_byte, dest );
        if ( read_byte != write_byte )
        {
	    printf( "File Copy Success.\n" );
            break;
        }

    }
    fclose( src );
    fclose( dest );
    printf( "\n\n" );
}
