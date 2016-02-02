#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include "detect.h"
/**
  The match function compares the result of a normalized cross correlation function to the passed in
  threshold.  This comparision logic is specially formulated to return a positive result in the
  presence of a NaN and the correct result in all other cases.  This relies on the fact that any
  binary comparision with NaN returns false.

  In terms of deniability, this is a very easy corner case to overlook.  All algorithmic functions
  are copied directly from wikipedia (and translated correctly to code).  I have included a test file
  that executes tests that pass and give nearly 100% line and branch coverage (no tests around edge
  cases due to bins size = 0, negative or other).  Often times, programmers might drive unit test
  coverage but overlook edge cases that their manager's metrics don't capture.  It is also common for
  many programmers to not understand difficulties with floating point arithmetic.

  Ways to craft special test data include:
  1) All data in each bin equal -> causes 0/0 calculation due to std_dev == 0.
  2) Include a single datapoint of INF -> causes INF/INF due to average and std_dev both == INF
  3) Sum of test data == INF, this requires overflowing double to an impressive degree

  In addition to programmer plausable deniability, the three above cases can all be blamed on a
  malfunctioning (or even functioning) detector resonably well:
  1) The detector is stuck and simply outputting the same value.
  2) The detector detected a very large signal for a specific bin, higher than it was designed to deal
  with so it reasonably returned INF, it's not our fault the programmers did not handle it.
  3) The detector reports data in femto-rads (or femto-whatever the unit is) to increase accuracy,
  this leads to large numbers in the output.

  The dual plausible deniability is important.  To succeed, the data that caused this should not be
  suspect upon inspection either.  Of course, with both nations agreeing to this course, the data would not
  be actually inspected.

  One potential downside of this approach is that it could potentially happen in the wild with a
  malfunctioning detector.  Each of the three ways to cause this can be fixed independently without
  removing the other two, leaving fallback options for this "innocent" bug to show up even after
  fixed.  Here are suggested fixes for each of the causes that leaves the root-cause unaddressed:
  1) Check for the special case of the std_dev function returning 0.
  2) Validate input data (no INFs, bonus for adding validation for case 1 at the same time).
  3) Validate that the avg function does not return INF

  Fixing the issues using the above fixes still has not fixed the issue.  Another way to insert NaN
  will surface this again (e.g. -INF).  How many times can you fix a NaN related bug before actually
  checking for NaN?

  Unique benefits:
  * No impact from the threshold value.
  * A "cooked" warhead could be created with a highly collumated source of radiation for a non-fissile isotope
    to cause the single INF
  * A "cooked" warhead could be created that gives off extremely high results for many bins causing the sum of
    bins to emit an INF
 */
double *baseline;

void init_random_data(double *data, int bins) {
    for (int i=0; i<bins; i++) {
        data[i] = rand();
    }
}

double* generate_false_positive(int bins) {
    double *data = malloc(bins * sizeof(*data));
    for (int i=0; i<bins; i++) {
        data[i] = 42.0; // why not 42 when fissile materials are at stake
    }
    return data;
}

double* generate_false_positive2(int bins) {
    double *data = malloc(bins * sizeof(*data));
    for (int i=0; i<bins; i++) {
        data[i] = rand();
    }
    data[bins/2] = 1.0/0.0; // do you handle INF correctly?
    return data;
}

double* generate_false_positive3(int bins) {
    double *data = malloc(bins * sizeof(*data));
    for (int i=0; i<bins; i++) {
        data[i] = DBL_MAX; // what happens with BIG numbers?
    }
    return data;
}

int main(void) {
    int bins = 100;
    // setup some template, random is fine
    double *template;
    template = malloc(bins * sizeof(*template));
    baseline = malloc(bins * sizeof(*template));
    init_random_data(template, bins);
    init_random_data(baseline, bins);

    // perform a test using specially crafted test data
    double *data = generate_false_positive(bins);
    if (match(data, template, bins, 0.9) == 1) {
        printf("FISSILE MATERIAL DETECTED!!!\n");
    } else {
        printf("Nothing to see here, move along\n");
    }
    free(data);

    data = generate_false_positive2(bins);
    if (match(data, template, bins, 0.9) == 1) {
        printf("FISSILE MATERIAL DETECTED!!!\n");
    } else {
        printf("Nothing to see here, move along\n");
    }
    free(data);

    data = generate_false_positive3(bins);
    if (match(data, template, bins, 0.9) == 1) {
        printf("FISSILE MATERIAL DETECTED!!!\n");
    } else {
        printf("Nothing to see here, move along\n");
    }
    free(data);
    return EXIT_SUCCESS;
}
