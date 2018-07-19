#include <QApplication>
#include <iostream>
#include <iomanip>
#include <complex>

using namespace std;

int main(int argc, char *argv[]) {

    complex<double> numl(2, 3);
    cout << numl.real() << ", " << numl.imag() << endl;

    return 0;
}
