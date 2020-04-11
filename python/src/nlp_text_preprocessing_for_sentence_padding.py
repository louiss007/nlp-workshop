import tensorflow as tf

# data preprocess for machine translation, step 5, padding and batches for training data.
MAX_LEN = 50
SOS_ID = 1


def make_dataset(file_path):
    dataset = tf.data.TextLineDataset(file_path)
    dataset = dataset.map(lambda string: tf.string_split([string]).values)
    dataset = dataset.map(
        lambda string: tf.string_to_number(string, tf.int32))
    dataset = dataset.map(lambda x: (x, tf.size(x)))
    return dataset


def filter_length(src_tuple, trg_tuple):
    ((src_input, src_len), (trg_label, trg_len)) = (src_tuple, trg_tuple)
    src_len_ok = tf.logical_and(
        tf.greater(trg_len, 1), tf.less_equal(src_len, MAX_LEN))
    trg_len_ok = tf.logical_and(
        tf.greater(trg_len, 1), tf.less_equal(trg_len, MAX_LEN))
    return tf.logical_and(src_len_ok, trg_len_ok)


def make_trg_input(src_tuple, trg_tuple):
    ((src_input, src_len), (trg_label, trg_len)) = (src_tuple, trg_tuple)
    trg_input = tf.concat([[SOS_ID], trg_label[:-1]], axis=0)
    return (src_input, src_len), (trg_input, trg_label, trg_len)


def make_srctrg_dataset(src_path, trg_path, batch_size):
    src_data = make_dataset(src_path)
    trg_data = make_dataset(trg_path)
    dataset = tf.data.Dataset.zip((src_data, trg_data))
    dataset = dataset.filter(filter_length)
    dataset = dataset.map(make_trg_input)
    dataset = dataset.shuffle(10000)

    paded_shapes = (
        (tf.TensorShape([None]),
         tf.TensorShape([])),
        (tf.TensorShape([None]),
         tf.TensorShape([None]),
         tf.TensorShape([])))
    batched_dataset = dataset.padded_batch(batch_size, paded_shapes)
    return batched_dataset
