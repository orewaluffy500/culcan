#include "culcan_main.hpp"

int main(int argc, char **argv) {
    const std::string code = "?@@>=(#)>>>++@@=[#]";

    cc_CulcanVM vm(code);

    vm.execute();
    return 0;
}
