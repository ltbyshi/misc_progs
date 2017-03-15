def binary_cross_entropy(logits=None, labels=None):
    return -tf.reduce_mean(labels*tf.log(logits + 1e-8) + (1.0 - labels)*tf.log(1.0 - logits + 1e-8))

def multi_cross_entropy(logits=None, labels=None):
    return -tf.reduce_mean(tf.reduce_sum(labels*tf.log(logits + 1e-8),
        reduction_indices=[1]))

def squared_error(y_true, y_pred):
    return 0.5*tf.reduce_mean(tf.pow(y_true - y_pred), 2.0)

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
        
