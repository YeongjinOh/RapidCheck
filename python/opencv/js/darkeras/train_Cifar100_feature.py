import numpy as np
import matplotlib.pyplot as plt
from numpy.random import permutation as perm

import keras.backend as K
from keras.datasets import cifar100 as c100
from keras.utils import np_utils
from keras.layers import Conv2D, MaxPooling2D, Dropout, Dense, Flatten
from keras.models import Sequential
import yolo.config as cfg
from yolo.process import preprocess

def plot_model_history(model_history):
    fig, axs = plt.subplots(1,2,figsize=(15,5))
    # summarize history for accuracy
    axs[0].plot(range(1,len(model_history.history['acc'])+1),model_history.history['acc'])
    axs[0].plot(range(1,len(model_history.history['val_acc'])+1),model_history.history['val_acc'])
    axs[0].set_title('Model Accuracy')
    axs[0].set_ylabel('Accuracy')
    axs[0].set_xlabel('Epoch')
    axs[0].set_xticks(np.arange(1,len(model_history.history['acc'])+1),len(model_history.history['acc'])/10)
    axs[0].legend(['train', 'val'], loc='best')
    # summarize history for loss
    axs[1].plot(range(1,len(model_history.history['loss'])+1),model_history.history['loss'])
    axs[1].plot(range(1,len(model_history.history['val_loss'])+1),model_history.history['val_loss'])
    axs[1].set_title('Model Loss')
    axs[1].set_ylabel('Loss')
    axs[1].set_xlabel('Epoch')
    axs[1].set_xticks(np.arange(1,len(model_history.history['loss'])+1),len(model_history.history['loss'])/10)
    axs[1].legend(['train', 'val'], loc='best')
    plt.show()

print(K.image_data_format())
print(K.image_dim_ordering())
inp_w, inp_h, inp_c = cfg.inp_size
batch_size = cfg.batch_size
epochs = cfg.epochs

def prepare_output_data(y_train, y_test):
    y_train = np_utils.to_categorical(y_train)
    y_test = np_utils.to_categorical(y_test)
    return y_train, y_test

def cifar_generator():
    """
    X_data : (data_size, 32, 32, 3)
    y_label : (data_size, 100)
    batch_size : batch size
    """
    (X_train, y_train), (X_test, y_test) = c100.load_data() # Load Data
    y_train, y_test = prepare_output_data(y_train, y_test) # prepare y_label 
    data_size = len(X_train)
    batch_iter_per_epoch = int(data_size/batch_size)
    while True:
        shuffle_idx = perm(np.arange(len(X_train)))
        print('\n')
        print("*"*30)
        print("Data Size : {}".format(data_size))
        print("Batch Size : {}".format(batch_size))
        print("Batch iterations per Epoch : {}".format(batch_iter_per_epoch))
        print("*"*30)
        print(shuffle_idx[0:10])
        
        for b in range(batch_iter_per_epoch):
            batch_features = np.zeros((batch_size, inp_w, inp_h, inp_c))
            batch_labels = np.zeros((batch_size, 100))

            for b_i, i in enumerate(range(b*batch_size, b*batch_size + batch_size)):
                # choose random index in features
                batch_features[b_i] = preprocess(X_train[shuffle_idx[i]], color_type='RGB')
                batch_labels[b_i] = y_train[shuffle_idx[i]]
            yield batch_features, batch_labels
    
    print("Done Generator")

def make_model(is_freeze=False, activation_type='relu'):
    model = Sequential()
    model.add(Conv2D(16, (3, 3), input_shape=(inp_w, inp_h, inp_c),padding='same', 
                            activation=activation_type, trainable=not is_freeze, name='conv1_1'))
    model.add(Conv2D(16, (3, 3), padding='same', 
                            activation=activation_type, trainable=not is_freeze, name='conv1_2'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    
    model.add(Conv2D(32,(3,3), padding='same', 
                            activation=activation_type, trainable=not is_freeze, name='conv2_1'))
    model.add(Conv2D(32,(3,3), padding='same', 
                            activation=activation_type, trainable=not is_freeze, name='conv2_2'))
    model.add(MaxPooling2D(pool_size=(2, 2),padding='valid'))

    model.add(Conv2D(64,(3,3), padding='same', 
                            activation=activation_type, trainable=not is_freeze, name='conv3_1'))
    model.add(Conv2D(64,(3,3), padding='same', 
                            activation=activation_type, trainable=not is_freeze, name='conv3_2'))
    model.add(MaxPooling2D(pool_size=(2, 2),padding='valid'))
#     model.add(Conv2D(128,(3,3), padding='same', 
#                             activation=activation_type, trainable=not is_freeze, name='conv4'))
#     model.add(MaxPooling2D(pool_size=(2, 2),padding='valid'))
#     model.add(Conv2D(256,(3,3), padding='same', 
#                             activation=activation_type, trainable=not is_freeze, name='conv5'))
#     model.add(MaxPooling2D(pool_size=(2, 2),padding='valid'))
#     model.add(Conv2D(512,(3,3), padding='same', 
#                             activation=activation_type, trainable=not is_freeze, name='conv6'))
#     model.add(MaxPooling2D(pool_size=(2, 2),padding='valid'))
#     model.add(Conv2D(1024,(3,3), padding='same', activation=activation_type, trainable=not is_freeze, name='conv7'))
#     model.add(Conv2D(1024,(3,3), padding='same', activation=activation_type, trainable=not is_freeze, name='conv8'))
#     model.add(Conv2D(1024,(3,3), padding='same', activation=activation_type, trainable=not is_freeze, name='conv9'))
    model.add(Flatten())
    model.add(Dense(256, name='dense1', activation=activation_type))
    model.add(Dense(512, name='dense2', activation=activation_type))
    model.add(Dense(100, name='softmax', activation='softmax'))
    
    return model

model = make_model(is_freeze=False)
model.summary()

model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])

history = model.fit_generator(cifar_generator(), 
                    steps_per_epoch=50000/batch_size, 
                    nb_epoch=50, 
                    verbose=1,
                    validation_data=None, 
                    class_weight=None,
                    workers=1)


plot_model_history(history)