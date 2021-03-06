
#include <cstdint>
#include <string>
#include <thread>

#include <libco.h>
#include <libretro.h>

#include "Common/Event.h"
#include "Core/BootManager.h"
#include "Core/Core.h"
#include "Core/HW/VideoInterface.h"
#include "DolphinLibretro/input.h"
#include "DolphinLibretro/main.h"
#include "VideoBackends/OGL/FramebufferManager.h"
#include "VideoCommon/VideoConfig.h"

#ifdef PERF_TEST
static struct retro_perf_callback perf_cb;

#define RETRO_PERFORMANCE_INIT(name)                                                               \
  retro_perf_tick_t current_ticks;                                                                 \
  static struct retro_perf_counter name = {#name};                                                 \
  if (!name.registered)                                                                            \
    perf_cb.perf_register(&(name));                                                                \
  current_ticks = name.total

#define RETRO_PERFORMANCE_START(name) perf_cb.perf_start(&(name))
#define RETRO_PERFORMANCE_STOP(name)                                                               \
  perf_cb.perf_stop(&(name));                                                                      \
  current_ticks = name.total - current_ticks;
#else
#define RETRO_PERFORMANCE_INIT(name)
#define RETRO_PERFORMANCE_START(name)
#define RETRO_PERFORMANCE_STOP(name)
#endif

namespace Libretro
{
bool core_stop_request = false;

cothread_t emuthread;
cothread_t mainthread;
retro_environment_t environ_cb;
retro_log_printf_t log_cb;

retro_variable options_desc[] = {{"dolphin_renderer", "Renderer; Hardware|Software|Null"},
                                 // Fastmem installs custom exception handlers
                                 // it needs to be disabled when running in a debugger.
                                 {"dolphin_fastmem", "Fastmem; ON|OFF"},
                                 {"dolphin_pal60", "PAL60; OFF|ON"},
                                 {"dolphin_progressive_scan", "Progressive scan (HD); OFF|ON"},
                                 {NULL, NULL}};

Options options = *(Options*)options_desc;
std::string sys_dir;

void check_variables(void)
{
}
}  // namespace Libretro

using namespace Libretro;

static retro_input_poll_t poll_cb;

void retro_set_input_poll(retro_input_poll_t cb)
{
  poll_cb = cb;
}

void retro_set_environment(retro_environment_t cb)
{
  struct retro_log_callback log;

  if(log_cb)
    return;

  environ_cb = cb;

  if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
    log_cb = log.log;
  else
    log_cb = NULL;

  environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void*)&options_desc);
#ifdef PERF_TEST
  environ_cb(RETRO_ENVIRONMENT_GET_PERF_INTERFACE, &perf_cb);
#endif
}

void retro_init(void)
{
  enum retro_pixel_format xrgb888;

  xrgb888 = RETRO_PIXEL_FORMAT_XRGB8888;
  environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &xrgb888);

  init_input();
}

void retro_deinit(void)
{
#ifdef PERF_TEST
  perf_cb.perf_log();
#endif
}

void retro_get_system_info(struct retro_system_info* info)
{
  info->need_fullpath = true;
  info->valid_extensions = "elf|dol|gcm|iso|tgc|wbfs|ciso|gcz|wad";
  info->library_version = "v0.01";
  info->library_name = "dolphin-emu";
  info->block_extract = true;
}

void retro_get_system_av_info(struct retro_system_av_info* info)
{
  info->geometry.base_width = 640;
  info->geometry.base_height = 448;

  info->geometry.max_width = 2048;   // 640 * max scale
  info->geometry.max_height = 2048;  // 528 * max scale
  //   info->geometry.max_width = 640; // 640 * max scale
  //   info->geometry.max_height = 528; // 528 * max scale

  info->geometry.aspect_ratio = (float)(4.0 / 3.0);

  info->timing.fps = 60.0;
  //   info->timing.fps = 60.0 / 1.001;
  //   info->timing.fps = VideoInterface::GetTargetRefreshRate();

  info->timing.sample_rate = 32000.0;
}

void retro_reset(void)
{
}

void retro_run(void)
{
  bool updated = false;
  static int frames = 0;

  if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
    check_variables();

  poll_cb();

  RETRO_PERFORMANCE_INIT(dolphin_main_func);
  RETRO_PERFORMANCE_START(dolphin_main_func);

  co_switch(emuthread);

  RETRO_PERFORMANCE_STOP(dolphin_main_func);

  frames++;
}

unsigned retro_get_region(void)
{
  return RETRO_REGION_NTSC;
}

unsigned retro_api_version()
{
  return RETRO_API_VERSION;
}
