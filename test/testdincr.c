#include <stdio.h>
#include <assert.h>
#include <qthread/qthread.h>

double master = 0.0;

aligned_t incr(qthread_t * me, void *arg)
{
    qthread_dincr(&master, 1.0);
    return 0;
}

aligned_t incr5(qthread_t * me, void *arg)
{
    qthread_dincr(&master, 5.0);
    return 0;
}

int main()
{
    int i;
    aligned_t rets[30];
    double ret_test;
    qthread_t *me;

    qthread_init(7);
    me = qthread_self();

    ret_test = qthread_dincr(&master, 1);
    if (master != 1.0) {
	printf("master = %f\n", master);
    }
    assert(master == 1.0);
    assert(ret_test == 0.0);
    master = 0;
    for (i = 0; i < 30; i++) {
	qthread_fork(incr, NULL, &(rets[i]));
    }
    for (i = 0; i < 30; i++) {
	qthread_readFF(me, NULL, rets + i);
    }
    if (master != 30.0) {
	printf("master is %f rather than 30\n", master);
    }
    assert(master == 30.0);
    master = 0.0;
    for (i = 0; i < 30; i++) {
	qthread_fork(incr5, NULL, &(rets[i]));
    }
    for (i = 0; i < 30; i++) {
	qthread_readFF(me, NULL, rets + i);
    }
    if (master != 150.0) {
	printf("master is %f rather than 150\n", master);
    }
    assert(master == 150.0);

    qthread_finalize();

    return 0;
}
