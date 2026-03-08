/**
 * Cooper Ross and Thomas Jenkins
 * March 4th, 2026
 * This program simulates the timer used in TCP's exponential weighted moving average
 * (EWMA), taking clock granularity and minimum RTO on the command line and asking for new
 * sample RTTs to calculate the new RTO.
 */
#include <iostream>
#include <string.h>
#include <cmath>
using namespace std;

int main (int argc, char * argv[]) {

    // Check if optional debug flag was set to true
    bool debugFlag = false;
    if (argc == 4 && stoi(argv[3]) == 1) debugFlag = true;

    // Error checking
    if (argc < 3) {
        cout << "This program takes two arguments:\n" <<
            "Argument 1 (required): Clock Granularity (as a decimal number. 0.004 is the linux standard)\n" <<
            "Argument 2 (required): Minimum RTO (as a decimal number. 1 = IETF recommended value, 0.2 is Linux standard, 0 = no minimum)\n" <<
            "Argument 3 (optional): Show debugging info (1 = Yes, any other number = No)";
        return 1;
    }
    if (stod(argv[1]) <= 0) {
        cout << "Granularity should be greater than 0. Exiting." << endl;
        return 2;
    }

    // Declare clock granularity, minimum RTO, sample RTT, deviation RTT, estimated RTT,
    // timeout interval, beta, and alpha.
    double gran, minRTO, sRTT, devRTT, estRTT, toi, roundedtoi, finaltoi, oldDevRTT, oldEstRTT;
    double beta = 0.25, alpha = 0.125;
    gran = stod(argv[1]);
    minRTO = stod(argv[2]);
    toi = 1;

    cout << endl << "Beginning RTO: " << toi << endl << endl;

    // Ask for first sample RTT, ending the program if a value <= 0 is given,
    // and round it to the clock granularity
    cout << "Enter the first sample RTT (enter a value <=0 to quit) >";
    cin >> sRTT;
    if (sRTT <= 0) return 0;
    sRTT = (ceil(sRTT / gran) * gran);

    // Calculate toi based on first sample
    estRTT = sRTT;
    devRTT = estRTT / 2;
    if (gran > (4 * devRTT)) toi = estRTT + gran;
    else toi = estRTT + (4 * devRTT);

    // Ensure the RTO rounds to the clock granularity and does not fall
    // below the minimum value
    roundedtoi = ceil(toi / gran) * gran;
    if (roundedtoi < minRTO) finaltoi = minRTO;
    else finaltoi = roundedtoi;

    // Output optional debug info if flag was triggered
    if (debugFlag) {
        cout << "\n*************************************************************************\n";
        cout << "sRTT after rounding first sample to clock granularity (" << gran << ") = " << sRTT << endl;
        cout << "estRTT = sRTT = " << sRTT << endl;
        cout << "devRTT = estRTT / 2 = " << devRTT << endl;
        cout << "max(gran, 4 * devRTT) = ";
        if (gran > (4 * devRTT)) cout << gran << endl;
        else cout << (4 * devRTT) << endl;
        cout << "RTO = estRTT + max(gran, 4 * devRTT) = " << toi << endl;
        cout << "RTO after rounding to clock granularity = " << roundedtoi << endl;
        cout << "RTO = max(RTO, minimumRTO) = " << finaltoi << endl;
        cout << "*************************************************************************\n\n";
    }

    cout << "New RTO: " << roundedtoi << endl << endl;

    // Ask for next sample RTT before beginning loop
    cout << "Enter the next sample RTT (enter a value <= 0 to quit) >";
    cin >> sRTT;
    if (sRTT <= 0) return 0;
    sRTT = (ceil(sRTT / gran) * gran); // Rounding

    // Calculate and output the new RTO until the user exits
    while (sRTT > 0) {
        oldDevRTT = devRTT, oldEstRTT = estRTT;
        devRTT = (1 - beta) * devRTT + beta * abs(sRTT - estRTT);
        estRTT = (1 - alpha) * estRTT + alpha * sRTT;
        if (gran > (4 * devRTT)) toi = estRTT + gran;
        else toi = estRTT + (4 * devRTT);

        // Ensure the RTO does not fall below the minimum value, and round 
        // up to the clock granularity
        roundedtoi = ceil(toi / gran) * gran;
        if (roundedtoi < minRTO) finaltoi = minRTO;
        else finaltoi = roundedtoi;



        // Output optional debug info if flag was triggered
        if (debugFlag) {
            cout << "\n*************************************************************************\n";
            cout << "sRTT after rounding sample to clock granularity (" << gran << ") = " << sRTT << endl;
            cout << "devRTT = (1 - " << beta << ") * " << oldDevRTT << " + " << beta << " * |" << sRTT 
                << " - " << estRTT << "| = " << devRTT << endl;
            cout << "estRTT = (1 - " << alpha << ") * " << oldEstRTT << " + " << alpha << " * " << estRTT << endl;
            cout << "max(gran, 4 * devRTT) = ";
            if (gran > (4 * devRTT)) cout << gran << endl;
            else cout << (4 * devRTT) << endl;
            cout << "RTO = sRTT + max(gran, 4 * devRTT) = " << toi << endl;
            cout << "RTO after rounding to clock granularity = " << roundedtoi << endl;
            cout << "RTO = max(RTO, minimumRTO) = " << finaltoi << endl;
            cout << "*************************************************************************\n\n";
        }


        cout << "New RTO: " << roundedtoi << endl << endl;

        cout << "Enter the next sample RTT (enter a value <= 0 to quit) >";
        cin >> sRTT;
        sRTT = (ceil(sRTT / gran) * gran); // Rounding
    }

    return 0;
}


int getSRTT() {
    int sRTT;
    cin >> sRTT;
    


}
