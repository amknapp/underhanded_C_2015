# underhanded_C_2015

Submission for the 8th Underhanded C Contest (2015): http://www.underhanded-c.org

## make test

This will create a test executable that executes a number of tests on the single publicly exposed function
(match).  These should all pass and show that the algorithm is operating as designed.  Although I did not run a test
coverage tool, line coverage should be near 100%. Branch coverage is a bit lower because I have no tests for 0 bins.

One note is the #define LAG_COMPARISON_ENABLED.  This enables the shifting of the signal data to see if it matches
the template data if shifted a bit (to counter any misscalibration of detector).  Of course this is might cause some
consternation with host countries and is easily disabled.  Enabling or disabiling this flag (or even deleting the
code hidden behind it along with the flag itself) will have no effect on this submission other than to remove
complexity from the program (and reduce its effectiveness at detecting nuclear materials).

## make spoiler (creates an executable to demonstrate proof of concept, read the spoiler.c file for details).


Notes:

* No checks for malloc null return.  Please don't run out of memory!  This program won't so neither should you,
  keep the number of bins reasonable.
* IEEE 754 compliant math is assumed and relied on.
* Followed simple wikipedia stuff. This program is (probably) not useful for any real statistical comparison of
  signals.
* Feel free to compile on the command line instead of using the makefile.  It's simply provided to simplify life.
* Some intentional coding oddities sprinkled around to increase plausibility of lack of programmer knowledge.
