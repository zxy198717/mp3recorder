#include <android/log.h>
#include "simple_lame_lib.h"

static jboolean local_log = JNI_FALSE;
static const char* TAG = "SimpleLameLib";

lame_global_flags* simple_lame_lib_init(
		JNIEnv* env,
		jint inSamplerate, jint outChannel,
		jint outSamplerate, jint outBitrate, jint quality,
		jstring id3tagTitle, jstring id3tagArtist, jstring id3tagAlbum,
		jstring id3tagYear, jstring id3tagComment) {
	if (local_log) {
		__android_log_print(ANDROID_LOG_VERBOSE, TAG, "Start lame init.");
	}

	lame_global_flags* glf = lame_init();
	lame_set_in_samplerate(glf, inSamplerate);
	lame_set_num_channels(glf, outChannel);
	lame_set_out_samplerate(glf, outSamplerate);
	lame_set_brate(glf, outBitrate);
	lame_set_quality(glf, quality);

	const jchar* title = NULL;
	const jchar* artist = NULL;
	const jchar* album = NULL;
	const jchar* year = NULL;
	const jchar* comment = NULL;
	if (id3tagTitle) {
		title = (*env)->GetStringChars(env, id3tagTitle, NULL);
	}
	if (id3tagArtist) {
		artist = (*env)->GetStringChars(env, id3tagArtist, NULL);
	}
	if (id3tagAlbum) {
		album = (*env)->GetStringChars(env, id3tagAlbum, NULL);
	}
	if (id3tagYear) {
		year = (*env)->GetStringChars(env, id3tagYear, NULL);
	}
	if (id3tagComment) {
		comment = (*env)->GetStringChars(env, id3tagComment, NULL);
	}

	if (title || artist || album || year || comment) {
		id3tag_init(glf);

		if (title) {
			id3tag_set_title(glf, (const char*)title);
			(*env)->ReleaseStringChars(env, id3tagTitle, title);
		}
		if (artist) {
			id3tag_set_artist(glf, (const char*)artist);
			(*env)->ReleaseStringChars(env, id3tagArtist, artist);
		}
		if (album) {
			id3tag_set_album(glf, (const char*)album);
			(*env)->ReleaseStringChars(env, id3tagAlbum, album);
		}
		if (year) {
			id3tag_set_year(glf, (const char*)year);
			(*env)->ReleaseStringChars(env, id3tagYear, year);
		}
		if (comment) {
			id3tag_set_comment(glf, (const char*)comment);
			(*env)->ReleaseStringChars(env, id3tagComment, comment);
		}
	}

	lame_init_params(glf);

	if (local_log) {
		__android_log_print(ANDROID_LOG_VERBOSE, TAG, "End lame init.");
	}
	
	return glf;
}

jint simple_lame_lib_encode(
		JNIEnv* env, lame_global_flags* glf, 
		jshortArray buffer_l, jshortArray buffer_r,
		jint samples, jbyteArray mp3buf) {
	jshort* j_buffer_l = (*env)->GetShortArrayElements(env, buffer_l, NULL);

	jshort* j_buffer_r = (*env)->GetShortArrayElements(env, buffer_r, NULL);

	const jsize mp3buf_size = (*env)->GetArrayLength(env, mp3buf);
	jbyte* j_mp3buf = (*env)->GetByteArrayElements(env, mp3buf, NULL);

	int result = lame_encode_buffer(glf, j_buffer_l, j_buffer_r,
			samples, j_mp3buf, mp3buf_size);

	(*env)->ReleaseShortArrayElements(env, buffer_l, j_buffer_l, 0);
	(*env)->ReleaseShortArrayElements(env, buffer_r, j_buffer_r, 0);
	(*env)->ReleaseByteArrayElements(env, mp3buf, j_mp3buf, 0);

	return result;
}

jint simple_lame_lib_encodeBufferInterleaved(
		JNIEnv* env, lame_global_flags* glf, 
		jshortArray pcm, jint samples, jbyteArray mp3buf) {
	jshort* j_pcm = (*env)->GetShortArrayElements(env, pcm, NULL);

	const jsize mp3buf_size = (*env)->GetArrayLength(env, mp3buf);
	jbyte* j_mp3buf = (*env)->GetByteArrayElements(env, mp3buf, NULL);

	int result = lame_encode_buffer_interleaved(glf, j_pcm,
			samples, j_mp3buf, mp3buf_size);

	(*env)->ReleaseShortArrayElements(env, pcm, j_pcm, 0);
	(*env)->ReleaseByteArrayElements(env, mp3buf, j_mp3buf, 0);

	return result;
}

jint simple_lame_lib_flush(
		JNIEnv* env, lame_global_flags* glf, 
		jbyteArray mp3buf) {
	const jsize mp3buf_size = (*env)->GetArrayLength(env, mp3buf);
	jbyte* j_mp3buf = (*env)->GetByteArrayElements(env, mp3buf, NULL);

	int result = lame_encode_flush(glf, j_mp3buf, mp3buf_size);

	(*env)->ReleaseByteArrayElements(env, mp3buf, j_mp3buf, 0);

	return result;
}

void simple_lame_lib_close(
		lame_global_flags* glf) {
	lame_close(glf);
	glf = NULL;

	if (local_log) {
		__android_log_print(ANDROID_LOG_VERBOSE, TAG, "Closed lame.");
	}
}

void simple_lame_lib_log(
		jboolean on) {
	local_log = on;
}
