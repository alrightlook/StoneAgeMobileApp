LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
LOCAL_CFLAGS:= -g

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
					 $(LOCAL_PATH)/SDL2_mixer-2.0.1 \
					  $(LOCAL_PATH)/SDL2_ttf-2.0.14 \
					   $(LOCAL_PATH)/SDL2_net-2.0.1 

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	oft/oft.cpp      oft/pat_dat.cpp  oft/sys2.cpp     oft/tekecess.cpp oft/work.cpp \
	newproto/autil.cpp    newproto/protocol.cpp \
	ohta/ohta.cpp      ohta/ohta_proc.cpp \
	system/action.cpp          system/directdraw.cpp      system/init.cpp            system/math2.cpp           system/process.cpp         system/talkwindow.cpp \
	system/ai_setting.cpp      system/field.cpp           system/loadrealbin.cpp     system/menu.cpp            system/produce.cpp         system/test.cpp	\
	system/battlemap.cpp       system/font.cpp            system/loadsprbin.cpp      system/mouse.cpp           system/radar.cpp           system/testview.cpp	\
	system/battlemenu.cpp      system/gamemain.cpp        system/login.cpp           system/netmain.cpp         system/savedata.cpp        system/tool.cpp	\
	system/battleproc.cpp      system/gemini.cpp          system/lssproto_cli.cpp    system/netproc.cpp         system/skyisland.cpp       system/unicodetogb2312.cpp 	\
	system/char_conversion.cpp system/handletime.cpp      system/lssproto_util.cpp   system/pattern.cpp         system/sprdisp.cpp         system/unpack.cpp	\
	system/character.cpp       system/help.cpp            system/main.cpp            system/pc.cpp              system/sprmgr.cpp	\
	system/chat.cpp            system/ime.cpp             system/map.cpp             system/phonemsg.cpp        system/t_music.cpp	\
	wgs/common.cpp   wgs/descrypt.cpp wgs/message.cpp  wgs/reproc.cpp


LOCAL_SHARED_LIBRARIES := SDL2 SDL2_ttf SDL2_net SDL2_mixer

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
