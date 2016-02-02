#include <math.h>

#define LAG_COMPARISON_ENABLED

extern double * baseline;
double max_xcorr(double *signal, double *template, int bins);
double normalized_xcorr(double *signal, double *template, int bins, int lag);
double avg(double *data, int length);
double std_dev(double *data, int length, double avg);

int match( double *test, double *reference, int bins, double threshold ) {
    // first test against the baseline for quick failures
    double baseline_xcorr = max_xcorr(test, baseline, bins);
    if (baseline_xcorr > threshold) {
        // it's a pure background signal, not fissile material here
        return 0;
    }
    // it's not a background signal, time to test against a reference model
    double reference_xcorr = max_xcorr(test, reference, bins);
    if (reference_xcorr < threshold) {
        // it does not match the reference, not fissile material here
        return 0;
    } else {
        return 1;
    }
}

/**
 * Determines the maximum normalized cross correlation between two spectra for a series of
 * offesets up to half the length of the arrays.  The choice of N/2 is somewhat arbitrary but
 * inspired by Nyquist
 */
double max_xcorr(double *signal, double *template, int bins) {
    // test zero shift first
    double max = normalized_xcorr(signal, template, bins, 0);

// this feature seems contentious, hide behind a flag so easily disabled.
#ifdef LAG_COMPARISON_ENABLED
    // only allow to shift half the signal away
    for (int i=1; i<bins/2; i++) {
        double xcorr = normalized_xcorr(signal, template, bins, i);
        if (xcorr > max) {
            max = xcorr;
        }
    }
#endif
    return max;
}

/**
 * Calcluates the normalized cross correlation between two spectra, returns a double
 * valoue between -1 and 1, 0 indicating no corrleation, 1 perfect correlation and -1
 * perfect negative correlation.  See:
 * https://en.wikipedia.org/wiki/Cross-correlation#Normalized_cross-correlation
 *
 * Note: lag is experimental, please feel free to turn on/off as desired
 */
double normalized_xcorr(double *signal, double *template, int bins, int lag) {
    bins -= lag; // lag fewer bins
    signal += lag; // advance the signal pointer lag bins
    double signal_avg = avg(signal, bins);
    double template_avg = avg(template, bins);
    double signal_std_dev = std_dev(signal, bins, signal_avg);
    double template_std_dev = std_dev(template, bins, template_avg);

    double xcorr = 0.0;
    for (int i=0; i<bins; i++) {
        xcorr += (signal[i] - signal_avg)*(template[i] - template_avg)
            / (signal_std_dev*template_std_dev);
    }
    xcorr = xcorr/bins;
    return xcorr;
}

/**
 * Computes the average of an array of doubles
 * https://en.wikipedia.org/wiki/Average#Arithmetic_mean
 */
double avg(double *data, int length) {
    double average = 0.0;
    for (int i=0; i<length; i++) {
        average += data[i];
    }
    average = average/length;
    return average;
}

/**
 * Computes the standard deviation of an array of doubles given that the average has already
 * been computed https://en.wikipedia.org/wiki/Standard_deviation#Discrete_random_variable
 */
double std_dev(double *data, int length, double avg) {
    double inner_sum = 0.0;
    for (int i=0; i<length; i++) {
        inner_sum += (data[i] - avg)*(data[i] - avg);
    }
    return sqrt(inner_sum/length);
}
