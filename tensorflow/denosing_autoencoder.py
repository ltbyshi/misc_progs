#! /usr/bin/env python
import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
import sklearn.datasets
import sklearn.model_selection
import sklearn.preprocessing
tf_config = tf.ConfigProto(allow_soft_placement=True)
tf_config.gpu_options.allow_growth=True

def xaiver_init(fan_in, fan_out, dtype=None, constant = 1):
    sigma = -constant*np.sqrt(6.0/(fan_in + fan_out))
    return tf.random_uniform((fan_in, fan_out), minval=-sigma, maxval=sigma, dtype=dtype)

def get_batches(X, y=None, batch_size=100, epochs=10):
    n_batches = X.shape[0]/batch_size
    if n_batches*batch_size < X.shape[0]:
        n_batches += 1
    for i in range(epochs):
        ind = np.random.permutation(X.shape[0])
        for j in range(n_batches):
            if batch_size*(j + 1) > X.shape[0]:
                ind_batch = ind[j*batch_size:]
            else:
                ind_batch = ind[(j*batch_size) : ((j + 1)*batch_size)]
            if y is not None:
                yield (X[ind_batch, :], y[ind_batch])
            else:
                yield X[ind_batch, :]

def scale_data(X_train, X_test):
    preprocessor = sklearn.preprocessing.StandardScaler().fit(X_train)
    X_train = preprocessor.transform(X_train)
    X_test = preprocessor.transform(X_test)
    return (X_train, X_test)

class DenoiseAutoEncoder(object):
    def __init__(self, n_input, n_hidden, noise=0.1):
        self.n_input = n_input
        self.n_hidden = n_hidden
        self.noise_value = noise
        self.activation = tf.nn.softplus
        self.W1 = tf.Variable(xaiver_init(n_input, n_hidden, dtype=tf.float32), name='W1')
        self.b1 = tf.Variable(tf.zeros([n_hidden], dtype=tf.float32), name='b1')
        self.W2 = tf.Variable(xaiver_init(n_hidden, n_input, dtype=tf.float32), name='W2')
        self.b2 = tf.Variable(tf.zeros([n_input], dtype=tf.float32), name='b2')
        self.x = tf.placeholder(tf.float32, [None, self.n_input], name='x')
        self.noise = tf.placeholder(tf.float32, name='noise')
        self.hidden = tf.matmul(self.x + self.noise*tf.random_normal((n_input,)), self.W1) + self.b1
        self.hidden = self.activation(self.hidden)
        self.output = tf.matmul(self.hidden, self.W2) + self.b2
        self.cost = tf.reduce_mean(tf.pow(self.output - self.x, 2))
        self.optimize = tf.train.AdamOptimizer(0.001).minimize(self.cost)
        self.sess = None

    def fit(self, X, batch_size=100, epochs=10, callbacks=[]):
        if not self.sess:
            self.sess = tf.Session(config=tf_config)

        self.sess.run(tf.global_variables_initializer())
        for epoch in range(epochs):
            for X_batch in get_batches(X, batch_size=batch_size, epochs=1):
                self.sess.run(self.optimize, feed_dict={self.x: X_batch, self.noise: self.noise_value})
            cost = self.sess.run(self.cost, feed_dict={self.x: X, self.noise: self.noise_value})
            print 'Epoch {}/{}: cost={}'.format(epoch, epochs, cost)
            if callbacks is not None:
                for callback in callbacks:
                    callback()


    def transform(self, X):
        return self.sess.run(self.hidden, feed_dict={self.x: X, self.noise: self.noise_value})

    def reconstruct(self, X):
        return self.sess.run(self.output, {self.x: X, self.noise: 0.0})

class FunctionCaller(object):
    def __init__(self, f, *args, **kwargs):
        self.f = f
        self.args = args
        self.kwargs = kwargs

    def __call__(self):
        self.f(*self.args, **self.kwargs)

class plot_reconstruct(object):
    def __init__(self, autoencoder, X):
        self.autoencoder = autoencoder
        self.X = X
        self.fig, self.ax = plt.subplots(2, X.shape[0], figsize=(X.shape[0], 2),
            sharex=True, sharey=True, tight_layout=True)

    def __call__(self):
        X_reconstructed = autoencoder.reconstruct(self.X)
        for i in range(self.X.shape[0]):
            self.ax[0, i].imshow(X_reconstructed[i].reshape(28, 28), cmap=plt.cm.Greys)
            self.ax[0, i].set_xticks([])
            self.ax[0, i].set_yticks([])
        for i in range(self.X.shape[0]):
            self.ax[1, i].imshow(self.X[i].reshape(28, 28), cmap=plt.cm.Greys)
            self.ax[1, i].set_xticks([])
            self.ax[1, i].set_yticks([])
        plt.pause(0.001)

if __name__ == '__main__':
    from tensorflow.examples.tutorials.mnist import input_data
    mnist = input_data.read_data_sets('/home/shibinbin/tmp/mnist_data/', one_hot=True)
    X_train, X_test = scale_data(mnist.train.images, mnist.test.images)
    autoencoder = DenoiseAutoEncoder(784, 50)
    plt.ion()
    autoencoder.fit(X_train,
        callbacks=[plot_reconstruct(autoencoder,
        X_test[np.random.randint(X_test.shape[0], size=12), :])])
