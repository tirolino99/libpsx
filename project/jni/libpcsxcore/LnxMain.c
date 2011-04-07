/*  Pcsx - Pc Psx Emulator
 *  Copyright (C) 1999-2002  Pcsx Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307 USA
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include "sio.h"

#include "psxcommon.h"

enum {
	RUN = 0,
	RUN_CD,
};

#ifdef PANDORA
int main(int argc, char *argv[]) {
	char file[MAXPATHLEN] = "";
	char path[MAXPATHLEN];
	int runcd = RUN;
	int loadst = 0;
	int i;

#ifdef ENABLE_NLS
	setlocale (LC_ALL, "");
	bindtextdomain (GETTEXT_PACKAGE, LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	// what is the name of the config file?
	// it may be redefined by -cfg on the command line
	strcpy(cfgfile_basename, "pcsx.cfg");

	// read command line options
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-runcd")) runcd = RUN_CD;
		else if (!strcmp(argv[i], "-nogui")) UseGui = FALSE;
		else if (!strcmp(argv[i], "-psxout")) Config.PsxOut = 1;
		else if (!strcmp(argv[i], "-load")) loadst = atol(argv[++i]);
		else if (!strcmp(argv[i], "-cfg")) {
			if (i+1 >= argc) break;
			strncpy(cfgfile_basename, argv[++i], MAXPATHLEN-100);	/* TODO buffer overruns */
			printf("Using config file %s.\n", cfgfile_basename);
		}
		else if (!strcmp(argv[i], "-cdfile")) {
			char isofilename[MAXPATHLEN];

			if (i+1 >= argc) break;
			strncpy(isofilename, argv[++i], MAXPATHLEN);
			if (isofilename[0] != '/') {
				getcwd(path, MAXPATHLEN);
				if (strlen(path) + strlen(isofilename) + 1 < MAXPATHLEN) {
					strcat(path, "/");
					strcat(path, isofilename);
					strcpy(isofilename, path);
				} else
					isofilename[0] = 0;
			}

			SetIsoFile(isofilename);
			runcd = RUN_CD;
            UseGui = FALSE;
		}
		else if (!strcmp(argv[i], "-h") ||
			 !strcmp(argv[i], "-help") ||
			 !strcmp(argv[i], "--help")) {
			 printf(PACKAGE_STRING "\n");
			 printf("%s\n", _(
							" pcsx [options] [file]\n"
							"\toptions:\n"
							"\t-runcd\t\tRuns CD-ROM\n"
							"\t-cdfile FILE\tRuns a CD image file\n"
							"\t-nogui\t\tDon't open the GTK GUI\n"
							"\t-cfg FILE\tLoads desired configuration file (default: ~/.pcsx/pcsx.cfg)\n"
							"\t-psxout\t\tEnable PSX output\n"
							"\t-load STATENUM\tLoads savestate STATENUM (1-9)\n"
							"\t-h -help\tDisplay this message\n"
							"\tfile\t\tLoads file\n"));
			 return 0;
		} else {
			strncpy(file, argv[i], MAXPATHLEN);
			if (file[0] != '/') {
				getcwd(path, MAXPATHLEN);
				if (strlen(path) + strlen(file) + 1 < MAXPATHLEN) {
					strcat(path, "/");
					strcat(path, file);
					strcpy(file, path);
				} else
					file[0] = 0;
			}
		}
	}

	memset(&Config, 0, sizeof(PcsxConfig));
	strcpy(Config.Net, "Disabled");

	if (UseGui) gtk_init(NULL, NULL);

	CheckSubDir();
	ScanAllPlugins();

	// try to load config
	// if the config file doesn't exist
	if (LoadConfig() == -1) {
		if (!UseGui) {
			printf(_("PCSX cannot be configured without using the GUI -- you should restart without -nogui.\n"));
			return 1;
		}

		// Uh oh, no config file found, use some defaults
		Config.PsxAuto = 1;

		gchar *str_bios_dir = g_strconcat(getenv("HOME"), BIOS_DIR, NULL);
		strcpy(Config.BiosDir, str_bios_dir);
		g_free(str_bios_dir);

		gchar *str_plugin_dir = g_strconcat(getenv("HOME"), PLUGINS_DIR, NULL);
		strcpy(Config.PluginsDir, str_plugin_dir);
		g_free(str_plugin_dir);

		gtk_init(NULL, NULL);

		// Update available plugins, but not GUI
		UpdatePluginsBIOS();

		// Pick some defaults, if they're available
		set_default_plugin(GpuConfS.plist[0], Config.Gpu);
		set_default_plugin(SpuConfS.plist[0], Config.Spu);
		set_default_plugin(CdrConfS.plist[0], Config.Cdr);
		set_default_plugin(Pad1ConfS.plist[0], Config.Pad1);
		set_default_plugin(Pad2ConfS.plist[0], Config.Pad2);
		set_default_plugin(BiosConfS.plist[0], Config.Bios);

		// create & load default memcards if they don't exist
		CreateMemcard("card1.mcd", Config.Mcd1);
		CreateMemcard("card2.mcd", Config.Mcd2);

		LoadMcds(Config.Mcd1, Config.Mcd2);

		SaveConfig();
	}

	gchar *str_patches_dir = g_strconcat(getenv("HOME"), PATCHES_DIR, NULL);
	strcpy(Config.PatchesDir,  str_patches_dir);
	g_free(str_patches_dir);

	// switch to plugin dotdir
	// this lets plugins work without modification!
	gchar *plugin_default_dir = g_build_filename(getenv("HOME"), PLUGINS_DIR, NULL);
	chdir(plugin_default_dir);
	g_free(plugin_default_dir);

	if (UseGui) SetIsoFile(NULL);

	if (SysInit() == -1) return 1;

	if (UseGui) {
		StartGui();
	} else {
		// the following only occurs if the gui isn't started
		if (LoadPlugins() == -1) {
			SysErrorMessage(_("Error"), _("Failed loading plugins!"));
			return 1;
		}
#if 0
		if (OpenPlugins() == -1 || plugins_configured() == FALSE) {
			return 1;
		}
#endif
		CheckCdrom();

		// Auto-detect: get region first, then rcnt-bios reset
		SysReset(1);

		if (file[0] != '\0') {
			Load(file);
		} else {
			if (runcd == RUN_CD) {
				if (LoadCdrom() == -1) {
					ReleasePlugins();
					printf(_("Could not load CD-ROM!\n"));
					return -1;
				}
			}
		}

		// If a state has been specified, then load that
		if (loadst) {
			StatesC = loadst - 1;
			gchar *state_filename = get_state_filename(StatesC);
			LoadState(state_filename);
			g_free(state_filename);
		}

		psxCpu->Execute();
	}

	return 0;
}
#endif

int SysInit() {
#ifdef EMU_LOG
#ifndef LOG_STDOUT
	emuLog = fopen("emuLog.txt","wb");
#else
	emuLog = stdout;
#endif
	setvbuf(emuLog, NULL, _IONBF, 0);
#endif

	if (EmuInit() == -1) {
		printf(_("PSX emulator couldn't be initialized.\n"));
		return -1;
	}

	LoadMcds(Config.Mcd1, Config.Mcd2);	/* TODO Do we need to have this here, or in the calling main() function?? */

	if (Config.Debug) {
		StartDebugger();
	}

	return 0;
}

static void dummy_lace()
{
}

void SysReset() {
	// rearmed hack: EmuReset() runs some code when real BIOS is used,
	// but we usually do reset from menu while GPU is not open yet,
	// so we need to prevent updateLace() call..
	void *real_lace = GPUupdateLace;
	GPU_updateLace = dummy_lace;

	EmuReset();

	// hmh core forgets this
	CDR_stop();

	GPU_updateLace = real_lace;
}

void SysClose() {
	EmuShutdown();
	ReleasePlugins();

	StopDebugger();

	if (emuLog != NULL) fclose(emuLog);
}

void SysPrintf(const char *fmt, ...) {
#if 0
	va_list list;
	char msg[512];

	va_start(list, fmt);
	vsprintf(msg, fmt, list);
	va_end(list);

	if (Config.PsxOut) {
		static char linestart = 1;
		int l = strlen(msg);

		printf(linestart ? " * %s" : "%s", msg);

		if (l > 0 && msg[l - 1] == '\n') {
			linestart = 1;
		} else {
			linestart = 0;
		}
	}

#ifdef EMU_LOG
#ifndef LOG_STDOUT
	fprintf(emuLog, "%s", msg);
#endif
#endif
#endif
}

void SysMessage(const char *fmt, ...) {
#if 0
	va_list list;
	char msg[512];

	va_start(list, fmt);
	vsprintf(msg, fmt, list);
	va_end(list);

	if (Config.PsxOut) {
		static char linestart = 1;
		int l = strlen(msg);

		printf(linestart ? " * %s" : "%s", msg);

		if (l > 0 && msg[l - 1] == '\n') {
			linestart = 1;
		} else {
			linestart = 0;
		}
	}

#ifdef EMU_LOG
#ifndef LOG_STDOUT
	fprintf(emuLog, "%s", msg);
#endif
#endif
#endif
}

void *SysLoadLibrary(const char *lib) {
	return dlopen(lib, RTLD_NOW);
}

void *SysLoadSym(void *lib, const char *sym) {
	return dlsym(lib, sym);
}

const char *SysLibError() {
	return dlerror();
}

void SysCloseLibrary(void *lib) {
	dlclose(lib);
}

static void SysDisableScreenSaver() {
#if 0
	static time_t fake_key_timer = 0;
	static char first_time = 1, has_test_ext = 0, t = 1;
	Display *display;
	extern unsigned long gpuDisp;

	display = (Display *)gpuDisp;

	if (first_time) {
		// check if xtest is available
		int a, b, c, d;
		has_test_ext = XTestQueryExtension(display, &a, &b, &c, &d);

		first_time = 0;
	}

	if (has_test_ext && fake_key_timer < time(NULL)) {
		XTestFakeRelativeMotionEvent(display, t *= -1, 0, 0);
		fake_key_timer = time(NULL) + 55;
	}
#endif
}

void SysUpdate() {
#if 0
	PADhandleKey(PAD1_keypressed());
	PADhandleKey(PAD2_keypressed());
	SysDisableScreenSaver();
#endif
}

/* ADB TODO Replace RunGui() with StartGui ()*/
void SysRunGui() {
#if 0
	StartGui();
#endif
}
