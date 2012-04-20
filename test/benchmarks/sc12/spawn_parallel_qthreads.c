#ifdef HAVE_CONFIG_H
# include "config.h" /* for _GNU_SOURCE */
#endif

#include <assert.h>
#include <stdio.h>
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

static void par_null_task(size_t start,
                          size_t stop,
                          void  *args_)
{}

int main(int   argc,
         char *argv[])
{
    uint64_t count    = 1048576;

    qtimer_t timer;
    double   total_time = 0.0;

    CHECK_VERBOSE();

    NUMARG(count, "MT_COUNT");
    assert(0 != count);

    assert(qthread_initialize() == 0);

    timer = qtimer_create();

    qtimer_start(timer);
    qt_loop(0, count, par_null_task, NULL);
    qtimer_stop(timer);

    total_time = qtimer_secs(timer);

    qtimer_destroy(timer);

    printf("%lu %lu %f\n",
           (unsigned long)qthread_num_workers(),
           (unsigned long)count,
           total_time);

    return 0;
}

/* vim:set expandtab */