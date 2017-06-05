import matplotlib.pyplot as plt
import numpy as np

# http://parneetk.github.io/blog/cnn-cifar10/
# def plot_model_history(model_history):
#     fig, axs = plt.subplots(1,2,figsize=(15,5))
#     # summarize history for accuracy
#     axs[0].plot(range(1,len(model_history.history['acc'])+1),model_history.history['acc'])
#     axs[0].plot(range(1,len(model_history.history['val_acc'])+1),model_history.history['val_acc'])
#     axs[0].set_title('Model Accuracy')
#     axs[0].set_ylabel('Accuracy')
#     axs[0].set_xlabel('Epoch')
#     axs[0].set_xticks(np.arange(1,len(model_history.history['acc'])+1),len(model_history.history['acc'])/10)
#     axs[0].legend(['train', 'val'], loc='best')
#     # summarize history for loss
#     axs[1].plot(range(1,len(model_history.history['loss'])+1),model_history.history['loss'])
#     axs[1].plot(range(1,len(model_history.history['val_loss'])+1),model_history.history['val_loss'])
#     axs[1].set_title('Model Loss')
#     axs[1].set_ylabel('Loss')
#     axs[1].set_xlabel('Epoch')
#     axs[1].set_xticks(np.arange(1,len(model_history.history['loss'])+1),len(model_history.history['loss'])/10)
#     axs[1].legend(['train', 'val'], loc='best')
#     plt.show()

def plot_model_history(model_history):
	fig, axs = plt.subplots(1,1,figsize=(15,5))
	axs.plot(range(1,len(model_history['acc'])+1),model_history['acc'])
	axs.plot(range(1,len(model_history['val_acc'])+1),model_history['val_acc'])
	axs.set_title('Model Accuracy')
	axs.set_ylabel('Accuracy')
	axs.set_xlabel('Epoch')
	axs.set_xticks(np.arange(1,len(model_history['acc'])+1),len(model_history['acc'])/10)
	axs.legend(['train', 'val'], loc='best')
	plt.show()
	fig.savefig('tmp/test.png')


if __name__ == '__main__':
	history = {}
	history['acc'] = [1,2,3,4,5,5,4,3,4,5]
	history['val_acc'] = [3,4,4,5,4,3,4,5,6,6]
	# print(len(history['acc']))
	# print(len(history['val_acc']))
	plot_model_history(history)
