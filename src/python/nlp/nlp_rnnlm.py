# -*- coding=utf-8 -*-
import numpy as np
import tensorflow as tf
from nlp_text_preprocessing_for_batch import read_data, make_batches


TRAIN_DATA = "../data/text_data/ptb/ptb.train"
EVAL_DATA = "../data/text_data/ptb/ptb.valid"
TEST_DATA = "../data/text_data/ptb/ptb.test"


HIDDEN_SIZE = 300
NUM_LAYERS = 2
VOCAB_SIZE = 10000
TRAIN_BATCH_SIZE = 20
TRAIN_NUM_STEP = 35

EVAL_BATCH_SIZE = 1
EVAL_NUM_STEP = 1
NUM_EPOCH = 5
LSTM_KEEP_PROB = 0.9
EMBEDDING_KEEP_PROB = 0.9
MAX_GRAD_NORM = 5
SHARE_EMB_AND_SOFTMAX = True


class PTBModel(object):
    def __init__(self, is_training, batch_size, num_steps):
        self.batch_size = batch_size
        self.num_steps = num_steps

        self.input_data = tf.placeholder(tf.int32, [batch_size, num_steps])
        self.targets = tf.placeholder(tf.int32, [batch_size, num_steps])

        dropout_keep_prob = LSTM_KEEP_PROB if is_training else 1.0
        lstm_cells = [
            tf.nn.rnn_cell.DropoutWrapper(
                tf.nn.rnn_cell.BasicLSTMCell(HIDDEN_SIZE),
                output_keep_prob=dropout_keep_prob)
            for _ in range(NUM_LAYERS)]
        cell = tf.nn.rnn_cell.MultiRNNCell(lstm_cells)

        self.initial_state = cell.zero_state(batch_size, tf.float32)

        embedding = tf.get_variable("embedding", [VOCAB_SIZE, HIDDEN_SIZE])

        inputs = tf.nn.embedding_lookup(embedding, self.input_data)
        if is_training:
            inputs = tf.nn.dropout(inputs, EMBEDDING_KEEP_PROB)

        outputs = []
        state = self.initial_state
        with tf.variable_scope("RNN"):
            for time_step in range(num_steps):
                if time_step > 0:
                    tf.get_variable_scope().reuse_variables()
                cell_output, state = cell(inputs[:, time_step, :], state)
                outputs.append(cell_output)

        output = tf.reshape(tf.concat(outputs, 1), [-1, HIDDEN_SIZE])

        if SHARE_EMB_AND_SOFTMAX:
            weight = tf.transpose(embedding)
        else:
            weight = tf.get_variable("weight", [HIDDEN_SIZE, VOCAB_SIZE])
        bias = tf.get_variable("bias", [VOCAB_SIZE])
        logits = tf.matmul(output, weight) + bias

        loss = tf.nn.sparse_softmax_cross_entropy_with_logits(
            labels=tf.reshape(self.targets, [-1]),
            logits=logits)

        self.cost = tf.reduce_sum(loss) / batch_size
        self.final_state = state

        if not is_training:
            return
        trainable_variables = tf.trainable_variables()

        grads, _ = tf.clip_by_global_norm(
            tf.gradients(self.cost, trainable_variables), MAX_GRAD_NORM)
        optimizer = tf.train.GradientDescentOptimizer(learning_rate=1.0)
        self.train_op = optimizer.apply_gradients(
            zip(grads, trainable_variables))


def run_epoch(session, model, batches, train_op, output_log, step):
    total_costs = 0.0
    iters = 0
    state = session.run(model.initial_state)
    for x, y in batches:
        cost, state, _ = session.run(
            [model.cost, model.final_state, train_op],
            {model.input_data: x, model.targets: y,
             model.initial_state: state})
        total_costs += cost
        iters += model.num_steps

        if output_log and step % 100 == 0:
            print("after %d steps, perplexity is %.3f" %
                  (step, np.exp(total_costs / iters)))
        step += 1

    return step, np.exp(total_costs / iters)


# read_data()


# make_batches()


def main():
    initializer = tf.random_uniform_initializer(-0.05, 0.05)

    with tf.variable_scope("language_model",
                           reuse=None, initializer=initializer):
        train_model = PTBModel(True, TRAIN_BATCH_SIZE, TRAIN_NUM_STEP)

    with tf.variable_scope("language_model",
                           reuse=True, initializer=initializer):
        eval_model = PTBModel(False, EVAL_BATCH_SIZE, EVAL_NUM_STEP)

    with tf.Session() as sess:
        tf.global_variables_initializer().run()
        train_batches = make_batches(
            read_data(TRAIN_DATA), TRAIN_BATCH_SIZE, TRAIN_NUM_STEP)
        eval_batches = make_batches(
            read_data(EVAL_DATA), EVAL_BATCH_SIZE, EVAL_NUM_STEP)
        test_batches = make_batches(
            read_data(TEST_DATA), EVAL_NUM_STEP, EVAL_NUM_STEP)

        step = 0
        for i in range(NUM_EPOCH):
            print("in iteration: %d" % (i + 1))
            step, train_pplx = run_epoch(sess, train_model, train_batches,
                                         train_model.train_op, True, step)
            print("epoch: %d train perplexity: %.3f" % (i+1, train_pplx))
            _, eval_pplx = run_epoch(sess, eval_model, eval_batches,
                                     tf.no_op(), False, 0)
            print("epoch: %d eval perplexity: %.3f" % (i + 1, eval_pplx))
            _, test_pplx = run_epoch(sess, eval_model, test_batches,
                                     tf.no_op(), False, 0)
            print("test perplexity: %.3f" % test_pplx)


if __name__ == '__main__':
    main()
