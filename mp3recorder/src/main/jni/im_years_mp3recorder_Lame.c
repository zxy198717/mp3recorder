#include "im_years_mp3recorder_Encoder.h"
#include "simple_lame_lib.h"

JNIEXPORT void JNICALL Java_im_years_mp3recorder_Lame_log
  (JNIEnv *env, jclass cls, jboolean on) {
	simple_lame_lib_log(on);
}
