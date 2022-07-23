#ifndef __BOMBAY_MUS_H
#define __BOMBAY_MUS_H

typedef enum
{
    MUS_TITLE,
} mus_id_t;

void mus_init(void);

void mus_play(mus_id_t mus_id);

#endif
