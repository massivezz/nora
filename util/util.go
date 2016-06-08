package util

/*
#include "util.cpp"
*/
import "C"

func Shutdown() {
	C.Util_Shutdown(C.int(0x00000008))
}
