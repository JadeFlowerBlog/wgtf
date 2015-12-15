import QtQuick 2.3
import QtQuick.Layouts 1.1
import BWControls 1.0

/*!
 \brief A RGB varient of the VectorN control
*/

WGVectorN {
    vectorLabels: ["R:", "G:", "B:"]
    vectorData: [0, 0, 0]
    stepsize: 1
    decimals: 0
    maximumValue: 255
    minimumValue: 0
}
