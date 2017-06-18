import h5py
import os
import numpy as np
import cv2
import matplotlib.pyplot as plt
import tensorflow as tf
import yolo.config as cfg
from utils.help import say, conv_weigths_flatten, save_model, plot_model_history
from yolo.datacenter.data import collect_enduser_trainset

import keras.backend as K
from yolo.net.RCNet_thdim_net import RCNet_THdim_model, RCNet_shortdense_THdim_model, RCNet_THdim_dropout_model
import argparse
import sys

parser = argparse.ArgumentParser()

args = parser.parse_args()

K.set_learning_phase(1) #set learning phase

if cfg.image_dim_order == 'th':
	K.set_image_dim_ordering('th')

# 새로 학습
pretrain_weight_path = cfg.pretrained_model

# 초벌구이 위에서 학습
# pretrain_weight_path = 'models/train/yolo-2class-voc2007-base-shortdense-cell14-steps24000.h5'
# pretrain_weight_path = 'models/train/yolo-2class-complete.h5'

# 추가 학습
# pretrain_weight_path = 'models/train/{}-complete.h5'.format(cfg.model_name)
# pretrain_weight_path = 'models/train/yolo-2class-mydata-tracking-cell14-steps20000.h5'

is_freeze = True
verbalise = True

freeze_layer_weights = None
trainable_layer_weights = None
show_trainable_state = False # 여기를 True 로 바꾸면, conv layer 와 dense layer 의 학습별 weigths 가 변하는지 안변하는지를 확인할 수 있다.
# trained_save_weights_prefix = 'models/train/{}-'.format(cfg.model_name)
if not os.path.exists(cfg.model_folder):
	os.mkdir(cfg.model_folder)
	print("Make New Model Folder on {}".format(cfg.model_folder))

print(cfg.dataset_abs_location)

sess = tf.Session()
K.set_session(sess)

model = RCNet_THdim_model()
# model = RCNet_THdim_dropout_model()
# model = RCNet_shortdense_THdim_model()
model.summary()

from yolo.training_v1 import darkeras_loss, _TRAINER
from yolo.datacenter.data import shuffle, test_shuffle

inp_x = model.input
net_out = model.output

say("Building {} loss function".format(cfg.model_name), verbalise=verbalise)
loss_ph, loss_op = darkeras_loss(net_out)
say("Building {} train optimizer".format(cfg.model_name), verbalise=verbalise)
optimizer = _TRAINER[cfg.trainer](cfg.lr)
gradients = optimizer.compute_gradients(loss_op, var_list=model.trainable_weights)
train_op = optimizer.apply_gradients(gradients)

sess.run(tf.global_variables_initializer())
	
model.load_weights(pretrain_weight_path, by_name=True)

# End User Custom Training Function
collect_enduser_trainset()

batches = shuffle()

train_histories = {}
train_histories['train_loss'] = []
train_histories['val_loss'] = []
record_step = 100

for i, (x_batch, datum) in enumerate(batches):
	train_feed_dict = {
	   loss_ph[key]:datum[key] for key in loss_ph 
	}
	train_feed_dict[inp_x] = x_batch
	# print("feed_dict.keys() : ", len(train_feed_dict.keys()), train_feed_dict.keys())
	fetches = [train_op, loss_op] 
	fetched = sess.run(fetches, feed_dict=train_feed_dict)
	
	loss_val = fetched[1]
	if i % record_step == 0:
		train_histories['train_loss'].append(loss_val)
		# 100 번마다 한번씩 test loss 를 구해본다.
		test_x_batch, test_datum = test_shuffle()
		test_feed_dict = {
			loss_ph[key]:test_datum[key] for key in loss_ph
		}
		test_feed_dict[inp_x] = test_x_batch
		fetches = [train_op, loss_op]
		test_fetched = sess.run(fetches, feed_dict=test_feed_dict)
		test_loss_val = test_fetched[1]
		train_histories['val_loss'].append(test_loss_val)
		# say("step {} - train loss {}, test loss {}".format(i, loss_val, test_loss_val), verbalise=True)
		sys.stdout.write('\r')
		sys.stdout.write("step {} - train loss {}, test loss {}".format(i, loss_val, test_loss_val))
		sys.stdout.flush()
	else:
		#say("step {} - train loss {}".format(i, loss_val), verbalise=True)
		sys.stdout.write('\r')
		sys.stdout.write("step {} - train loss {}".format(i, loss_val))
		sys.stdout.flush()

	if show_trainable_state:
		conv1 = model.layers[0]
		dense_last = model.layers[-1]
		if freeze_layer_weights is None:
			freeze_layer_weights = sess.run(conv1.weights[0])
			trainable_layer_weights = sess.run(dense_last.weights[0])
		else:
			freeze_layer_comp = (freeze_layer_weights == sess.run(conv1.weights[0]))
			if all(conv_weigths_flatten(freeze_layer_comp)):
				print(conv1.name, "\tFreeze!")
			else:
				print(conv1.name, "\tTraining~")

			last_layer_comp = (trainable_layer_weights == sess.run(dense_last.weights[0]))
			last_layer_flatten = [val for sublist in last_layer_comp for val in sublist]
			if all(last_layer_flatten):
				print(dense_last.name, "\tFreeze!")
			else:
				print(dense_last.name, "\tTraining~")
	
	# if i == 0:
	# 	model.save_weights(trained_save_weights_prefix + 'steps{}.h5'.format(i))
	# 	say("Save weights : ", trained_save_weights_prefix + 'steps{}.h5'.format(i), verbalise=verbalise)
   
	if i % 4000 == 0:
		# save_model(model, save_folder, file_name, steps, descriptions, save_type='weights'):
		save_model(model, cfg.model_folder, cfg.model_name, i, cfg.descriptions)
		plot_model_history(train_histories, cfg.model_folder, cfg.model_name, record_step, is_test=True)
		# model.save_weights(trained_save_weights_prefix + 'steps{}.h5'.format(i))
		# say("Save weights : ", trained_save_weights_prefix + 'steps{}.h5'.format(i), verbalise=verbalise)

say('Training All Done..', verbalise=verbalise)
# model.save_weights(trained_save_weights_prefix + 'complete.h5')
# say("Save weights : ", trained_save_weights_prefix + 'complete.h5', verbalise=verbalise)