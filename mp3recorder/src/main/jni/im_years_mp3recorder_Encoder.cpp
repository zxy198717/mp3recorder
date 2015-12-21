#include <vector>
#include "im_years_mp3recorder_Encoder.h"
#include "simple_lame_lib.h"

static std::vector<lame_global_flags*> glfList(2);

JNIEXPORT jint JNICALL Java_im_years_mp3recorder_Encoder_init(
		JNIEnv *env, jclass cls, jint inSamplerate, jint outChannel,
		jint outSamplerate, jint outBitrate, jint quality,
		jstring id3tagTitle, jstring id3tagArtist, jstring id3tagAlbum,
		jstring id3tagYear, jstring id3tagComment) {
	lame_global_flags* glf = simple_lame_lib_init(env, inSamplerate, outChannel, outSamplerate, outBitrate,
		quality, id3tagTitle, id3tagArtist, id3tagAlbum, id3tagYear, id3tagComment);
	glfList.push_back(glf);

	return glfList.size() - 1;
}

JNIEXPORT jint JNICALL Java_im_years_mp3recorder_Encoder_encode(
		JNIEnv *env, jclass cls, jint instanceIndex, jshortArray buffer_l,
		jshortArray buffer_r, jint samples, jbyteArray mp3buf) {
	return simple_lame_lib_encode(env, glfList.at(instanceIndex), buffer_l, buffer_r, samples, mp3buf);
}

JNIEXPORT jint JNICALL Java_im_years_mp3recorder_Encoder_encodeBufferInterleaved(
		JNIEnv *env, jclass cls, jint instanceIndex, jshortArray pcm,
		jint samples, jbyteArray mp3buf) {
	return simple_lame_lib_encodeBufferInterleaved(env, glfList.at(instanceIndex), pcm, samples, mp3buf);
}

JNIEXPORT jint JNICALL Java_im_years_mp3recorder_Encoder_flush(
		JNIEnv *env, jclass cls, jint instanceIndex, jbyteArray mp3buf) {
	return simple_lame_lib_flush(env, glfList.at(instanceIndex), mp3buf);
}

JNIEXPORT void JNICALL Java_im_years_mp3recorder_Encoder_close(
		JNIEnv *env, jclass cls, jint instanceIndex) {
	simple_lame_lib_close(glfList.at(instanceIndex));
}

JNIEXPORT void JNICALL Java_im_years_mp3recorder_Encoder_log
  (JNIEnv *env, jclass cls, jboolean on) {
	simple_lame_lib_log(on);
}
