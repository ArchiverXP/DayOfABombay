#include <stdio.h>

#include <libdragon.h>

#include <stdlib.h>

#include "mus.h"

#define CHANNEL_MUSIC   2
#define CHANNEL_SFX1    0

// A Bombay's Day
// DEVELOPMENT STARTED: 7/19/22
int main(void)
{
    console_init();
    debug_init_usblog();
    console_set_debug(true);
    display_init(   RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS  );
    dfs_init(DFS_DEFAULT_LOCATION);
    rdp_init();
    controller_init();
    timer_init();
    audio_init(22050, 2);
    mixer_init(2);
    mixer_ch_set_limits(CHANNEL_MUSIC, 0, 44100, 0);
    int level = 0;

    //music

    wav64_t mus_title;
    wav64_open(&mus_title, "rom:/sound/music/Bombay.wav64");
    wav64_set_loop(&mus_title, true);

    //wav64_open(&mus_kate, "rom:/sound/music/Katie.wav64");
    //wav64_set_loop(&mus_kate, true);

    // sfx
    //wav64_open(&sfx_works, "rom:/sound/works.wav64")
    bool music = true;
    int music_frequency = mus_title.wave.frequency;

    // title screen
    int fp = dfs_open("/gfx/title.sprite");
    sprite_t *title_screen = malloc(dfs_size(fp));
    dfs_read(title_screen, 1, dfs_size(fp), fp);
    dfs_close(fp);

    // bombay (for level 1)
    fp = dfs_open("/gfx/uncanny.sprite");
    sprite_t *uncanny = malloc(dfs_size(fp));
    dfs_read(uncanny, 1, dfs_size(fp), fp);
    dfs_close(fp);
    while(1) {
        static display_context_t disp = 0;
        display_show(disp);
        music = !music;
        if (music) {
            wav64_play(&mus_title, CHANNEL_MUSIC);
            mixer_ch_set_vol(CHANNEL_MUSIC, 1.00f, 1.00f);
            music_frequency = mus_title.wave.frequency;
            music_frequency /= 1.1;
            mixer_ch_set_freq(CHANNEL_MUSIC, music_frequency);
        }
        else
            mixer_ch_stop(CHANNEL_MUSIC);
        graphics_draw_sprite(disp, 0, 0, title_screen);
        graphics_draw_text(disp, 20, 20, "Pre-Alpha\n0.0");
        while(!(disp = display_lock()) );
        controller_scan();
        struct controller_data keys = get_keys_down();
        if( keys.c[0].A)
        {
            level = 1 - level;
        }
        switch(level)
        {
            case 0:
                printf("poopy!!!");
            case 1:
                graphics_draw_sprite(disp, 0, 0, uncanny);
        }
    }
    if (audio_can_write()) {
            short *const buf = audio_write_begin();
            mixer_poll(buf, audio_get_buffer_length());
            audio_write_end();
    }
}
