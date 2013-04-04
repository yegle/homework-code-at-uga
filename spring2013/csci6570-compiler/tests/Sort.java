/*
 * a sample sorting program
 */
public class Sort
{
    static void print( int[] b, int sz )
    {
        int i = 0;

        SimpleIO.printString( "[ " );
        for( i = 0; i < sz; i++ ) {
            SimpleIO.printInt( b[i] );
            if( i < sz - 1 )
                SimpleIO.printString( ", " );
        }
        SimpleIO.printString( " ]\n" );
        return;
    }

    static void sort( int[] c, int sz )
    {
        int i = 0;
        int j = 0;
        int temp = 0;;

        while ( i < sz - 1 ) {
            j = i + 1;
            while ( j < sz ) {
                if( c[i] > c[j] ) {
                    temp = c[i];
                    c[i] = c[j];
                    c[j] = temp;
                }
                j = j + 1;
            }
            i = i + 1;
        }
        return;
    }

    public static void main( String[] args )
    {
        int[] a = null;
        int   i = 0;

        // create the array
        a = new int[10];

        // initialize the values
        a[0] = 10;
        a[1] = 1;
        a[2] = 6;
        a[3] = 2;
        a[4] = 7;
        a[5] = 3;
        a[6] = 8;
        a[7] = 4;
        a[8] = 9;
        a[9] = 5;

        SimpleIO.printString( "Elements to sort:\n" );
        print( a, 10 );

        // sort the array
        sort( a, 10 );

        SimpleIO.printString( "Sorted elements:\n");
        print( a, 10 );
        return;         // return from method (last stmt)
    }
}
