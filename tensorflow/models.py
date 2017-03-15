#! /usr/bin/env python
import os
import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
import sklearn.datasets
import sklearn.model_selection
import datetime
from collections import defaultdict

tf_config = tf.ConfigProto(allow_soft_placement=True)
tf_config.gpu_options.allow_growth=True

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
            if y is None:
                yield X[ind_batch]
            else:
                yield (X[ind_batch], y[ind_batch])

def plot_transformed(ax, X, y, grid, tf_variable):
    # transform the grid
    ax.set_xlim(-0.2, 0.2)
    ax.set_ylim(-0.2, 0.2)
    """
    for coords in grid:
        coords_out = tf_variable.eval({x: coords})
        ax.add_line(plt.Line2D(coords_out[:, 0], coords_out[:, 1], linewidth=1))"""
    X_out = tf_variable.eval({x: X})
    color = np.zeros(X.shape[0], dtype='S1')
    color[y == 0] = 'b'
    color[y == 1] = 'r'
    ax.scatter(X_out[:, 0], X_out[:, 1], s=1, c=color, edgecolors='none')

######### losses #########
def binary_cross_entropy(y_true, y_pred):
    return -tf.reduce_mean(y_true*tf.log(y_pred + 1e-8) + (1.0 - y_true)*tf.log(1.0 - y_pred + 1e-8))

def multi_cross_entropy(y_true, y_pred):
    return -tf.reduce_mean(tf.reduce_sum(y_true*tf.log(y_pred + 1e-8),
        reduction_indices=[1]))

def squared_error(y_true, y_pred):
    return 0.5*tf.reduce_mean(tf.pow(y_true - y_pred, 2.0))

def get_loss(loss):
    if (not isinstance(loss, str)) and (not isinstance(loss, unicode)):
        return str(loss), loss
    if loss == 'binary_cross_entropy':
        return binary_cross_entropy
    elif loss == 'multi_cross_entropy':
        return multi_cross_entropy
    elif loss == 'squared_error':
        return squared_error
    else:
        raise ValueError('unknown loss: {}'.format(loss))


def get_optimizer(name):
    if (not isinstance(name, str)) and (not isinstance(name, unicode)):
        return name
    if name == 'SGD':
        return tf.train.GradientDescentOptimizer(0.5)
    elif name == 'Adam':
        return tf.train.AdamOptimizer(0.1)
    else:
        raise ValueError('unknown optimizer: {}'.format(name))

def accuracy(y_true, y_pred):
    return tf.reduce_mean(tf.cast(tf.equal(y_true, y_pred), tf.float32))

def get_metric(metric):
    if (not isinstance(metric, str)) and (not isinstance(metric, unicode)):
        return str(metric), metric
    if metric == 'accuracy':
        return metric, accuracy
    else:
        raise ValueError('unknown metric: {}'.format(metric))

def get_activation(activation):
    if (not isinstance(activation, str)) and (not isinstance(activation, unicode)):
        return activation
    if activation == 'sigmoid':
        return tf.nn.sigmoid
    elif activation == 'relu':
        return tf.nn.relu
    elif activation == 'softmax':
        return tf.nn.softmax
    elif activation == 'softplus':
        return tf.nn.softplus
    else:
        raise ValueError("unknown activation function: {}".format(activatiom))


_SESSION = None
def get_session():
    global _SESSION
    tf_config = tf.ConfigProto(allow_soft_placement=True)
    tf_config.gpu_options.allow_growth=True
    if _SESSION is None:
        _SESSION = tf.Session(config=tf_config)
    return _SESSION

class Layer(object):
    def __init__(self, name, enable_summary=False, summary_scalars=[], summary_images=[], training_phase=None, **kwargs):
        """
        Arguments:
            name: name of the layer
            summary_scalars: either 'input' or 'output' tensor to added to summary as scalars
            summary_images: either 'input' or 'output' tensor to added to summary as images
            training_phase: a placeholder (boolean type) to indicate if it is in training phase
        """
        self.name = name
        if not hasattr(self, 'output_shape'):
            self.output_shape = None
        self.input = None
        self.enable_summary = enable_summary
        self._training_phase = training_phase
        self.summary_scalars = summary_scalars
        self.summary_images = summary_images
        self.children = []
        self.parents = []

    def get_output_shape(self, input_shape):
        raise NotImplementedError('abstract class Layer cannot be used directly')

    def build(self):
        raise NotImplementedError('abstract class Layer cannot be used directly')

    def build_recurse(self, number=0, input_shape=None):
        if input_shape is not None:
            self.input_shape = input_shape
        if self.input_shape is None:
            raise ValueError('you should specify input shape for the first layer')

        if len(self.parents) == 1:
            self.input = self.parents[0].output
        elif len(self.parents) > 1:
            raise ValueError('layer with two parent layers is not supported: {}'.format(self.name))
        self.name = '{}_{}'.format(self.name, number)
        self.output_shape = self.get_output_shape(self.input_shape)
        with tf.name_scope(self.name):
            self.build()
            if self.enable_summary:
                if 'input' in self.summary_scalars:
                    tf.summary.histogram('input', self.input)
                if 'output' in self.summary_scalars:
                    tf.summary.histogram('output', self.output)
                if 'input' in self.summary_images:
                    tf.summary.image('input', self.input)
                if 'output' in self.summary_images:
                    tf.summary.image('output', self.output)
        for i, layer in enumerate(self.children):
            print '{} => {}'.format(self.name, layer.name)
            number = layer.build_recurse(number + 1, self.output_shape)

        return number

    def __call__(self, layer):
        """Connect the input to the output of another layer
        """
        self.input = layer.output

    def connect(self, layer):
        """Connect the input to the output of another layer
        """
        self.parents.append(layer)
        layer.children.append(self)
        return layer

    def append(self, layer):
        self.children.append(layer)
        layer.parents.append(self)
        return layer

    def prepend(self, layer):
        self.parents.append(layer)
        layer.children.append(self)
        return layer

    def walk(self):
        yield self
        for child in self.children:
            for layer in child.walk():
                yield layer

    def walk_graph(self):
        if isinstance(self, LayerList):
            if len(self.layers) > 0:
                self.layers[0].walk_graph()
        else:
            for layer in self.children:
                print self, layer
                layer.walk_graph()

    def get_tails(self):
        if len(self.children) == 0:
            return [self]
        else:
            tails = []
            for child in self.children:
                tails += child.get_tails()
            return tails

    def get_heads(self):
        if len(self.parents) == 0:
            return [self]
        else:
            heads = []
            for parent in self.parents:
                heads += parent.get_heads()
            return heads

    @property
    def training_phase(self):
        return self._training_phase

    @training_phase.setter
    def training_phase(self, value):
        self._training_phase = value

class LayerList(Layer):
    def __init__(self, layers=[], input_shape=None, name='LayerList',
            training_phase=None, enable_summary=False, **kwargs):
        self.input_shape = input_shape
        self.layers = layers
        self._training_phase = training_phase
        self._enable_summary = enable_summary
        super(LayerList, self).__init__(name, **kwargs)

    @property
    def training_phase(self):
        return self._training_phase

    @training_phase.setter
    def training_phase(self, value):
        for layer in self.layers:
            layer.training_phase = value
        self._training_phase = value

    @property
    def enable_summary(self):
        return self._enable_summary

    @enable_summary.setter
    def enable_summary(self, value):
        for layer in self.layers:
            layer.enable_summary = value
        self._enable_summary = value

    def get_output_shape(self, input_shape):
        """Calculate and save the output shapes of all layers and returns the output shape of the last layer
        """
        if len(self.layers) > 0:
            for i, layer in enumerate(self.layers):
                if i == 0:
                    layer.output_shape = layer.get_output_shape(input_shape)
                else:
                    if layer.input_shape is None:
                        layer.input_shape = self.layers[i - 1].output_shape
                    if layer.output_shape is None:
                        layer.output_shape = layer.get_output_shape(layer.input_shape)
                    if len(self.layers[i - 1].output_shape) != len(layer.input_shape):
                        raise ValueError('input dimension of Layer {} is not the same with output dimension of Layer {}'.format(i, i - 1))
                    if not all(map(lambda x: x[0] == x[1], zip(self.layers[i - 1].output_shape, layer.input_shape))):
                        raise ValueError('input shape of Layer {} is not the same with output shape of Layer {}'.format(i, i - 1))
            self.output_shape = self.layers[-1].output_shape
        else:
            self.output_shape = self.input_shape
        return self.output_shape

    def build(self):
        """
        for i, layer in enumerate(self.layers):
            if i > 0:
                layer.input = self.layers[i - 1].output
            layer.name = '{}_{}'.format(layer.name, i + 1)
            with tf.name_scope(layer.name):
                layer.build()
                if self.enable_summary:
                    if 'input' in layer.summary_scalars:
                        tf.summary.histogram('input', layer.input)
                    if 'output' in layer.summary_scalars:
                        tf.summary.histogram('output', layer.output)
                    if 'input' in layer.summary_images:
                        tf.summary.image('input', layer.input)
                    if 'output' in layer.summary_images:
                        tf.summary.image('output', layer.output)
        if len(self.layers) > 0:
            self.output = self.layers[-1].output
            self.input = self.layers[0].input
        """
        if len(self.layers) > 0:
            self.layers[0].build_recurse()
            self.output = self.layers[-1].output
            self.input = self.layers[0].input
        else:
            self.output = self.input

    def __getitem__(self, index):
        return self.layers[index]

    def prepend(self, layer):
        if len(self.layers) > 0:
            self.layers[0].connect(layer)
        self.layers.insert(0, layer)
        self.input = layer.input
        return self

    def add(self, layer):
        if len(self.layers) == 0:
            if layer.input_shape is not None:
                self.input_shape = layer.input_shape
            self.input = layer.input
        else:
            layer.connect(self.layers[-1])
        self.layers.append(layer)

        return self

    def append(self, layer):
        return self.add(layer)

    def walk(self):
        if len(self.layers) > 0:
            for layer in self.layers[0].walk():
                yield layer

class Input(Layer):
    def __init__(self, input_shape, name='Input', **kwargs):
        self.input_shape = input_shape
        super(Input, self).__init__(name, **kwargs)

    def get_output_shape(self, input_shape):
        return input_shape

    def build(self):
        self.input = tf.placeholder(tf.float32, [None] + self.input_shape, name='input')
        self.output = self.input

class Output(Layer):
    def __init__(self, input_shape=None, name='output', **kwargs):
        self.input_shape = input_shape
        super(Output, self).__init__(name, **kwargs)

    def get_output_shape(self, input_shape):
        return input_shape

    def build(self):
        self.output = self.input

class Bias(Layer):
    def __init__(self, input_shape=None, name='Bias', **kwargs):
        self.input_shape = input_shape
        super(Bias, self).__init__(name, **kwargs)

    def get_output_shape(self, input_shape):
        return input_shape

    def build(self):
        self.b = tf.Variable(tf.zeros(input_shape, dtype=tf.float32), name='b')
        self.output = self.input + self.b

class Diverge(Layer):
    def __init__(self, layers=[], input_shape=None, name='Diverge', **kwargs):
        self.input_shape = input_shape
        self.layers = layers
        super(Diverge, self).__init__(name, **kwargs)

    def get_output_shape(self, input_shape):
        return input_shape

    def build(self):
        self.outputs = self.input

    def add(self, layer):
        self.layers.append(layer)
        return self

class Reshape(Layer):
    def __init__(self, output_shape, input_shape=None, name='Reshape', **kwargs):
        self.input_shape = input_shape
        self.output_shape = output_shape
        super(Reshape, self).__init__(name, **kwargs)

    def get_output_shape(self, input_shape=None):
        return self.output_shape

    def build(self):
        self.output = tf.reshape(self.input, [-1] + self.output_shape)

class Dense(Layer):
    def __init__(self, n_output, n_input=None, activation=None, name='Dense', **kwargs):
        self.activation = activation
        if activation is not None:
            self.activation = get_activation(activation)
        self.n_output = n_output
        self.n_input = n_input
        self.input_shape = [n_input] if n_input is not None else None

        super(Dense, self).__init__(name, **kwargs)

    def get_output_shape(self, input_shape=None):
        return [self.n_output]

    def build(self):
        self.W = tf.Variable(tf.truncated_normal([self.input_shape[0], self.output_shape[0]], stddev=0.1, dtype=tf.float32), name='W')
        self.b = tf.Variable(tf.zeros([self.output_shape[0]], dtype=tf.float32), name='b')
        self.output = tf.matmul(self.input, self.W) + self.b
        if self.activation is not None:
            self.output = self.activation(self.output)

class GaussianNoise(Layer):
    def __init__(self, mean=0.0, stddev=1.0, input_shape=None, name='GaussianNoise', **kwargs):
        self.mean = mean
        self.stddev = stddev
        self.input_shape = input_shape
        super(GaussianNoise, self).__init__(name, **kwargs)

    def get_output_shape(self, input_shape):
        return input_shape

    def build(self):
        if self.training_phase is not None:
            self.output = tf.cond(self.training_phase,
                lambda: self.input + tf.random_normal(self.output_shape, mean=self.mean, stddev=self.stddev),
                lambda: self.input)
        self.output = self.input + tf.random_normal(self.output_shape, mean=self.mean, stddev=self.stddev)

class Flatten(Layer):
    def __init__(self, input_shape=None, name='Flatten', **kwargs):
        self.input_shape = input_shape
        super(Flatten, self).__init__(name, **kwargs)

    def get_output_shape(self, input_shape):
        return [np.product(input_shape)]

    def build(self):
        self.output = tf.reshape(self.input, [-1, self.output_shape[0]])

class Dropout(Layer):
    def __init__(self, input_shape=None, keep_prob=0.5, name='Dropout', **kwargs):
        self.keep_prob = keep_prob
        self.input_shape = input_shape
        self.training_only = True
        super(Dropout, self).__init__(name, **kwargs)

    def get_output_shape(self, input_shape):
        return input_shape

    def build(self):
        if self.training_phase is not None:
            self.output = tf.cond(self.training_phase,
                lambda: tf.nn.dropout(self.input, self.keep_prob),
                lambda: self.input)
        else:
            self.output = tf.nn.dropout(self.input, self.keep_prob)

class Activation(Layer):
    def __init__(self, activation='sigmoid', input_shape=None, name='Activation', **kwargs):
        self.activation = get_activation(activation)
        self.input_shape = input_shape
        super(Activation, self).__init__(name, **kwargs)

    def get_output_shape(self, input_shape):
        return input_shape

    def build(self):
        self.output = self.activation(self.input)

class Convolution2D(Layer):
    def __init__(self, n_output, kernel_size=[1, 1], input_shape=None,
            strides=[1, 1], padding='valid', name='Convolution', **kwargs):
        self.n_output = n_output
        self.kernel_size = kernel_size
        self.strides = strides
        self.padding = padding.upper()
        self.input_shape = input_shape
        super(Convolution2D, self).__init__(name, **kwargs)

    def get_output_shape(self, input_shape):
        if self.padding == 'VALID':
            p = [0, 0]
        elif self.padding == 'SAME':
            p = [self.kernel_size[0]/2, self.kernel_size[1]/2]
        output_shape = []
        for i in range(2):
            output_shape.append((input_shape[i] + 2*p[i] - self.kernel_size[i])/self.strides[i] + 1)
        output_shape.append(self.n_output)
        return output_shape

    def build(self):
        self.W = tf.Variable(tf.truncated_normal([self.kernel_size[0], self.kernel_size[1], 1, self.n_output],
            dtype=tf.float32), name='W')
        self.b = tf.Variable(tf.constant(0.1, shape=[self.n_output]), name='b')
        self.output = tf.nn.conv2d(self.input, self.W,
            strides=[1, self.strides[0], self.strides[1], 1], padding=self.padding) + self.b

class MaxPool2D(Layer):
    def __init__(self,  kernel_size=[2, 2], input_shape=None,
            strides=None, padding='valid', name='MaxPool', **kwargs):
        self.input_shape = input_shape
        self.kernel_size = kernel_size
        if strides is None:
            self.strides = self.kernel_size
        self.padding = padding.upper()
        super(MaxPool2D, self).__init__(name, **kwargs)

    def get_output_shape(self, input_shape):
        if self.padding == 'VALID':
            p = [0, 0]
        elif self.padding == 'SAME':
            p = [self.kernel_size[0]/2, self.kernel_size[1]/2]
        output_shape = []
        for i in range(2):
            output_shape.append((input_shape[i] + 2*p[i] - self.kernel_size[i])/self.strides[i] + 1)
        output_shape.append(input_shape[2])
        return output_shape

    def build(self):
        self.output = tf.nn.max_pool(self.input,
            ksize=[1, self.kernel_size[0], self.kernel_size[1], 1],
            strides=[1, self.strides[0], self.strides[1], 1],
            padding=self.padding)

class Model(object):
    def __init__(self, generative=False, name=None, **kwargs):
        self.layers = LayerList(name=name)
        self.generative = generative

    def build_layers(self):
        self.training_phase = tf.placeholder_with_default(tf.constant(False, dtype=tf.bool), shape=(),
            name='training_phase')

        # find input layers and add an Input layer if missing
        input_layers = [layer for layer in self.layers.walk() if isinstance(layer, Input)]
        if len(input_layers) == 0:
            if self.layers.input_shape is None:
                raise ValueError('you should specify the input shape of the first layer or define an Input layer')
            else:
                self.input_shape = self.layers.input_shape
                self.input_layer = Input(self.input_shape)
                self.layers.prepend(input_layer)
                self.layers = input_layer
        elif len(input_layers) == 1:
            self.input_layer = input_layers[0]
            self.input_shape = self.input_layer.input_shape
        else:
            raise ValueError('only one input layer is supported')
        # find output layers and add an Output layer if missing
        output_layers = [layer for layer in self.layers.walk() if isinstance(layer, Output)]
        if len(output_layers) == 0:
            tails = self.layers.get_tails()
            if len(tails) > 1:
                raise ValueError('only one output layer is supported. Please set the Output layer explicitly')
            else:
                tails[0].append(Output())
            self.output_layer = tails[0]
        elif len(output_layers) == 1:
            self.output_layer = output_layers[0]
        elif len(output_layers) > 1:
            raise ValueError('only one output layer is supported')

        for layer in self.input_layer.walk():
            layer.training_phase = self.training_phase
        self.input_layer.build_recurse(number=0)
        self.output_shape = self.output_layer.output_shape
        self.input = self.input_layer.input
        self.output = self.output_layer.output
        self.x = self.input
        self.y = self.output

        """
        for i, layer in enumerate(self.layers):
            if i == 0:
                if layer.input_shape is None:
                    raise ValueError('you should specify the input shape of the first layer')
                layer.input = tf.placeholder(tf.float32, [None] + list(layer.input_shape), name='input')
            else:
                # caculate the input shape of the second layer
                if layer.input_shape is None:
                    layer.input_shape = self.layers[i - 1].output_shape
                if len(self.layers[i - 1].output_shape) != len(layer.input_shape):
                    raise ValueError('input dimension of Layer {} is not the same with output dimension of Layer {}'.format(i, i - 1))
                if not all(map(lambda x: x[0] == x[1], zip(self.layers[i - 1].output_shape, layer.input_shape))):
                    raise ValueError('input shape of Layer {} is not the same with output shape of Layer {}'.format(i, i - 1))
                layer.input = self.layers[i - 1].output
            if layer.output_shape is None:
                layer.output_shape = layer.get_output_shape(layer.input_shape)
            #print '{}: input_shape: {}, output_shape: {}'.format(layer.name, layer.input_shape, layer.output_shape)
            layer.name = '{}_{}'.format(layer.name, i + 1)
            layer.training_phase = self.training_phase
            with tf.name_scope(layer.name):
                layer.build()
                if self.summary_dir is not None:
                    if 'input' in layer.summary_scalars:
                        tf.summary.histogram('input', layer.input)
                    if 'output' in layer.summary_scalars:
                        tf.summary.histogram('output', layer.output)
                    if 'input' in layer.summary_images:
                        tf.summary.image('input', layer.input)
                    if 'output' in layer.summary_images:
                        tf.summary.image('output', layer.output)
        self.input = self.layers[0].input
        self.output = self.layers[-1].output
        self.x = self.input
        self.y = self.output
        self.input_shape = self.layers[0].input_shape
        self.output_shape = self.layers[-1].output_shape"""

    def add(self, layer):
        self.layers.append(layer)
        return self

    def compile(self, optimizer='SGD', metrics=['accuracy'], loss='binary_cross_entropy',
            summary_dir=None):
        """
        Arguments:
            reconstruct: True if the output if the same of the input (reconstruction)
        """
        self.summary_dir = summary_dir
        if summary_dir is not None:
            for layer in self.layers.walk():
                layer.enable_summary = True
        self.build_layers()
        # generative model
        if self.generative:
            self.loss = get_loss(loss)(self.input, self.output)
            self.metrics = {'loss': self.loss}
            for metric in metrics:
                metric_name, metric = get_metric(metric)
                self.metrics[metric_name] = metric(self.input, self.output)
        else:
            # binary classification
            # remove the last dimension if the output has only one dimension
            if (len(self.output_shape) == 1) and (self.output_shape[0] == 1):
                self.y_true = tf.placeholder(tf.float32, [None], name='y_true')
                self.output = tf.squeeze(self.output)
                self.y_pred = tf.cast(tf.greater(self.output, 0.5), tf.float32, name='y_pred')
                self.y_true_scalar = self.y_true
                self.loss = get_loss(loss)(self.y_true, self.output)
            # multi-class classification
            else:
                self.y_true = tf.placeholder(tf.float32, [None] + self.output_shape, name='y_true')
                self.loss = get_loss(loss)(self.y_true, self.output)
                self.y_true_scalar = tf.argmax(self.y_true, 1, name='y_true_scalar')
                self.y_pred = tf.argmax(self.output, 1, name='y_pred')
            self.metrics = {'loss': self.loss}
            for metric in metrics:
                metric_name, metric = get_metric(metric)
                self.metrics[metric_name] = metric(self.y_true_scalar, self.y_pred)

        self.optimizer = get_optimizer(optimizer).minimize(self.loss)
        if summary_dir is not None:
            for metric_name, metric in self.metrics.iteritems():
                tf.summary.scalar(metric_name, metric)
            self.summary_graph = tf.summary.merge_all()

    def fit(self, X, y=None, epochs=10, batch_size=100, validation_data=None):
        # get validation data
        if validation_data is not None:
            ind = np.random.choice(X.shape[0], size=min(X.shape[0], 1000))
            X_validation = X[ind]
            if not self.generative:
                y_validation = y[ind]
        else:
            if not self.generative:
                X_validation, y_validation = validation_data
            else:
                X_validation = validation_data
        if self.summary_dir is not None:
            summary_dir = os.path.join(self.summary_dir, datetime.datetime.now().isoformat()) + '-train'
            train_writer = tf.summary.FileWriter(summary_dir, get_session().graph)
        get_session().run(tf.global_variables_initializer())
        for epoch in range(epochs):
            # classification/regression
            if not self.generative:
                for X_batch, y_batch in get_batches(X, y, epochs=1, batch_size=batch_size):
                    get_session().run(self.optimizer, feed_dict={self.x: X_batch, self.y_true: y_batch,
                        self.training_phase: True})
                metric_values = get_session().run(self.values(),
                    feed_dict={self.x: X_validation, self.y_true: y_validation})
            # generative model
            else:
                for X_batch in get_batches(X, epochs=1, batch_size=batch_size):
                    get_session().run(self.optimizer, feed_dict={self.x: X_batch,
                        self.training_phase: True})
                metric_values = get_session().run(self.metrics.values(),
                    feed_dict={self.x: X_validation})

            metric_str = ', '.join(('{}={}'.format(name, value) for name, value in zip(self.metrics.keys(), metric_values)))
            print 'Epoch {}/{}: {}'.format(epoch, epochs, metric_str)
            if self.summary_dir is not None:
                if not self.generative:
                    summary = get_session().run(self.summary_graph,
                        feed_dict={self.x: X_validation, self.y_true: y_validation})
                else:
                    summary = get_session().run(self.summary_graph,
                        feed_dict={self.x: X_validation})
                train_writer.add_summary(summary, epoch)

    def predict(self, X):
        return get_session().run(self.output, feed_dict={self.input: X})

    def score(self, X, y, metric):
        metric_name, metric = get_metric(metric)
        return get_session().run(metric(self.output), feed_dict={self.input: X, self.y_true: y})

    def summary(self):
        print 'name\tinput_shape\toutput_shape'
        for layer in self.input_layer.walk():
            print '{}\t{}\t{}'.format(layer.name, layer.input.get_shape(), layer.output.get_shape())
        #for i, layer in enumerate(self.layers):
        #    print '{}\t{}\t{}\t{}'.format(i + 1, layer.name, layer.input_shape, layer.output_shape)
        print 'input={}, output= {}'.format(self.input, self.output)

def test_mlp():
    n_samples = 10000
    n_features = 10
    n_classes = 2
    # create dataset
    X_all, y_all = sklearn.datasets.make_classification(n_samples, n_features, n_classes=2)
    X_train, X_test, y_train, y_test = sklearn.model_selection.train_test_split(X_all, y_all, test_size=0.33)
    print X_train.shape, y_train.shape
    print X_test.shape, y_test.shape

    # build model
    import sklearn.linear_model
    model = sklearn.linear_model.LogisticRegression()
    model.fit(X_train, y_train)
    print model.score(X_test, y_test)
    model = MLP(n_features, n_features, 0.5, summary_dir='/home/shibinbin/tensorboard/mlp_2d_visualize')
    #model = LogisticRegression(n_features, summary_dir='/home/shibinbin/tensorboard/logistic_regression')
    model.compile()
    model.fit(X_train, y_train)
    print 'Accuracy: {}'.format(model.score(X_test, y_test))

def test_cnn1():
    model = Model(summary_dir='/home/shibinbin/tensorboard/cnn1')
    model.add(Reshape([28, 28, 1], input_shape=[784]))
    model.add(GaussianNoise(stddev=0.1))
    model.add(Convolution2D(32, kernel_size=[2, 2]))
    model.add(Activation('relu'))
    model.add(MaxPool2D(kernel_size=[2, 2]))
    model.add(Flatten())
    model.add(Dense(1024))
    model.add(Dropout(keep_prob=0.9))
    model.add(Dense(10, summary_scalars=['input', 'output']))
    model.add(Activation('softmax'))
    model.compile(optimizer=tf.train.AdamOptimizer(1e-4))
    model.summary()

    from tensorflow.examples.tutorials.mnist import input_data
    mnist = input_data.read_data_sets('/home/shibinbin/tmp/mnist_data/', one_hot=True)
    print mnist.train.images.shape, mnist.train.labels.shape
    model.fit(mnist.train.images, mnist.train.labels,
        validation_data=[mnist.test.images, mnist.test.labels])

def test_autoencoder():
    model = Model(generative=True)
    input_layer = Input(input_shape=[784])
    input_layer.append(Reshape([28, 28, 1], summary_images=['output'], name='InputImage'))
    layers = input_layer.append(GaussianNoise(stddev=0.1))
    layers = layers.append(Dense(100))
    layers = layers.append(Activation('softplus'))
    layers = layers.append(Dense(100))
    layers = layers.append(Activation('softplus'))
    layers = layers.append(Dense(784))
    output = layers
    layers = layers.append(Output())
    output.append(Reshape([28, 28, 1], summary_images=['output'], name='OutputImage'))

    """
    model.add(Input(input_shape=[784]))
    model.add(GaussianNoise(stddev=0.1))
    model.add(Dense(100))
    model.add(Activation('softplus'))
    output = Dense(784)
    model.add(output)
    """

    model.layers = input_layer
    model.compile(optimizer=tf.train.AdamOptimizer(), loss='squared_error', metrics=[],
        summary_dir='/home/shibinbin/tensorboard/autoencoder')
    model.summary()
    #for layer in model.layers.walk():
    #    print layer.name

    from tensorflow.examples.tutorials.mnist import input_data
    mnist = input_data.read_data_sets('/home/shibinbin/tmp/mnist_data/', one_hot=True)
    print mnist.train.images.shape, mnist.train.labels.shape
    model.fit(mnist.train.images,
        validation_data=mnist.test.images)

def test_mlp1():
    n_samples = 10000
    n_features = 10
    n_classes = 2
    # create dataset
    X_all, y_all = sklearn.datasets.make_classification(n_samples, n_features, n_classes=2)
    X_train, X_test, y_train, y_test = sklearn.model_selection.train_test_split(X_all, y_all, test_size=0.33)

    model = Model(summary_dir='/home/shibinbin/tensorboard/LayerCollection')
    model.add(Dense(5, n_input=n_features))
    model.add(Activation('relu'))
    model.add(Dense(1))
    model.add(Activation('sigmoid'))
    model.summary()
    model.compile(loss='multi_cross_entropy')
    model.fit(X_train, y_train)

if __name__ == '__main__':
    #test_cnn1()
    test_autoencoder()
