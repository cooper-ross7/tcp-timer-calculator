/**
 * Cooper Ross and Thomas Jenkins
 * March 4th, 2026
 * This program simulates the timer used in TCP's exponential weighted moving average
 * (EWMA), taking clock granularity and minimum RTO on the command line and asking for new
 * sample RTTs to calculate the new RTO.
 */
#include <iostream>
#include <string.h>
using namespace std;

int main (int argc, char * argv[]) {

    // Error checking
    if (argc < 3) {
        cout << "This program takes two arguments:\n" <<
            "Argument 1: Clock Granularity (as a decimal number. 0.004 is the linux standard)\n" <<
            "Argument 2: Minimum RTO (as a decimal number. 1 = IETF recommended value, 0.2 is Linux standard, 0 = no minimum)\n";
            return 1;
    }
    if (stod(argv[1]) <= 0) {
        cout << "Granularity should be more than 0. Exiting." << endl;
        return 2;
    }

    // Declare clock granularity, minimum RTO, sample RTT, deviation RTT, estimated RTT,
    // timeout interval, beta, and alpha.
    double gran, minRTO, sRTT, devRTT, estRTT, toi;
    double beta = 0.25;
    double alpha = 0.125;
    gran = stod(argv[1]);
    minRTO = stod(argv[2]);
    toi = 1;

    cout << endl << "Beginning RTO: " << toi << endl << endl;

    // Ask for first sample RTT, ending the program if a value <= 0 is given
    cout << "Enter the first sample RTT (enter a value <=0 to quit) >";
    cin >> sRTT;
    if (sRTT <= 0) return 0;

    // Calculate toi based on first sample
    devRTT = sRTT / 2;
    estRTT = sRTT;
    if (gran > (4 * devRTT)) toi = estRTT + gran;
    else toi = estRTT + (4 * devRTT);

    // Ensure the RTO does not fall below the minimum value and output it
    if (toi < minRTO) toi = minRTO;
    cout << "New RTO: " << toi << endl << endl;

    // Ask for next sample RTT before beginning loop
    cout << "Enter the next sample RTT (enter a value <= 0 to quit) >";
    cin >> sRTT;
    if (sRTT <= 0) return 0;

    // Calculate and output the new RTO until the user exits
    while (sRTT > 0) {
        devRTT = (1 - beta) * devRTT + beta * abs(sRTT - estRTT);
        estRTT = (1 - alpha) * estRTT + alpha * sRTT;
        if (gran > (4 * devRTT)) toi = estRTT + gran;
        else toi = estRTT + (4 * devRTT);

        // Ensure the RTO does not fall below the minimum value
        if (toi < minRTO) toi = minRTO;

        cout << "New RTO: " << toi << endl << endl;

        cout << "Enter the next sample RTT (enter a value <= 0 to quit) >";
        cin >> sRTT;
    }

    return 0;
}
