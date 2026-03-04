#include <iostream>
#include <string.h>
using namespace std;

int main (int argc, char * argv[]) {

  if (argc < 3) {
    cout << "Not enough args correct structure should be "  << argv[0] << ", <granularity>, " <<  "<minimum RTO>, optional debugging flag <0 or 1>" << endl;
    return 1;
  }
  double gran, minRTO, sRTT, devRTT, estRTT, toi;
  double beta = 0.25;
  double alpha = 0.125;
  gran = stod(argv[1]);
  minRTO = stod(argv[2]);
  toi = 1;

  cout << "Beginning RTO: " << toi << endl;

  cout << "Enter the first sample RTT (enter a value <=0 to quit) >";
  cin >> sRTT;
  if (sRTT > 0) {
    devRTT = sRTT / 2;
    estRTT = sRTT;
    if (gran > (4 * devRTT)) {
      toi = estRTT + gran;
    } else {
      toi = estRTT + (4 * devRTT);
    }
  }

  while (sRTT > 0) {
    cout << "Enter the next sample RTT (enter a value <= 0 to quit) >";
    cin >> sRTT;
  }

  return 0;
}
