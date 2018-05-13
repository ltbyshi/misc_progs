#! /usr/bin/env python
import tensorflow as tf
import os

def get_session(graph=None):
    if not os.environ.get('OMP_NUM_THREADS'):
        config = tf.ConfigProto(allow_soft_placement=True)
    else:
        nb_thread = int(os.environ.get('OMP_NUM_THREADS'))
        config = tf.ConfigProto(intra_op_parallelism_threads=nb_thread,
                                allow_soft_placement=True)
    config.gpu_options.per_process_gpu_memory_fraction = 0.5
    config.gpu_options.allow_growth = True
    return tf.Session(graph=graph, config=config)

if __name__ == '__main__':
    n = 10000
    graph = tf.Graph()
    with graph.as_default():
        tf.set_random_seed(1234)
        W = tf.Variable(tf.random_uniform((n, n), 0, 1), name='W', trainable=False)
        x = tf.Variable(tf.random_uniform((n, 1), 0, 1), name='x', trainable=False)
        x /= tf.reduce_sum(x)
        W /= tf.reduce_sum(W, axis=0, keep_dims=True)
        x_new = tf.matmul(W, x)
        i = tf.constant(0, name='i')

        W, x, x_new, i = tf.while_loop(cond=lambda W, x, x_new, i: tf.logical_and(tf.reduce_mean(tf.squared_difference(x_new, x)) > 1e-30, i <= 1000),
            body=lambda W, x, x_new, i: (W, x_new, tf.matmul(W, x_new), i + 1),
            loop_vars=[W, x, x_new, i])
        diff = tf.reduce_mean(tf.squared_difference(x_new, x))
        initializer = tf.global_variables_initializer()

    with get_session(graph=graph) as session:
        initializer.run()
        x_val, i_val, diff_val = session.run([x_new, i, diff])
        print x_val[:10, 0]
        print i_val
        print diff_val
