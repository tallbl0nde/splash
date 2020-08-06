#include "splash/Utils.hpp"

namespace Splash::Utils {
    std::string intToHexString(unsigned int i) {
        std::string str = "";

        // Repeat until number is zero
        while (i != 0) {
            int r = i%16;
            if (r < 10) {
                str.insert(str.begin(), '0'+r);
            } else {
                str.insert(str.begin(), 'a'+r%10);
            }
            i /= 16;
        }

        // If less than two chars pad with zero
        while (str.length() < 8) {
            str.insert(str.begin(), '0');
        }

        return str;
    }
};