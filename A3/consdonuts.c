/*  the consumer code is really just a double for loop      */
/*  with outer loop set to number of dozens and inner       */
/*  set to get 12 ...  use a usleep(100) call between inner */
/*  and outer loops to allow all process a chance to run    */
/*  before any one consumer can finish                      */
 
/*  some of the consumer code                               */

gettimeofday ( &randtime, ( struct timezone * ) 0 );
	xsub [0] = ( ushort ) randtime.tv_usec;
	xsub [1] = ( ushort ) ( randtime.tv_usec >> 16 );
	xsub [2] = ( ushort ) ( getpid() );
         for( i = 0; i < 10; i++ ) {
           for( m = 0; m < 12; m++ ) {
                // j = rand num
               j = nrand48( xsub ) & 3;
			.
			.
			.
	     ...etc.........
	   } /* end getting one dozen, now sleep for others */
            usleep(100);
         }  /*  end getting 10 dozen, now finish            */
         fprintf(stderr, "  CONSUMER %s DONE\n", argv[1]);
         return 0;
}  /*  end consumer program                                 */
