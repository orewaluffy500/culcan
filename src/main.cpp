#include "culcan_main.hpp"

int main(int argc, char **argv) {
    std::string code = "?@@>=(#)>>>++@@=[#]";

    CukanVM vm(code);

    vm.execute();
    return 0;
}
