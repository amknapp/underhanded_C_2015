/**
 * Returns 1 if the test matches the reference above the threshold amount, 0 otherwise.
 *
 * Threshold must be between 0 and 1, 1 requires an exact match and 0 requires no match at all.
 * 
 * This works by computing the normalized cross correlation between the two sets of data.  It will
 * find the maxium correlation even in the presence of misscallibration where there is a constant
 * shift in the data.  It will bias towards a negative result if the test data is a match above
 * threshold to the baseline data (which must be defined externally and also be of length bins).
 *
 * Preconditions: test != null
 *                test is of length bins
 *                reference != null
 *                reference is of length bins
 *                threshold >= 0
 *                threshold <= 1
 *                bins > 1
 */
int match( double * test, double * reference, int bins, double threshold );
