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

def plot_model_history(model_history, save_path):
	fig, axs = plt.subplots(1,1,figsize=(10,5))
	axs.plot(range(1,len(model_history['train_loss'])*100+1, 100),model_history['train_loss'])
	# axs.plot(range(1,len(model_history['val_loss'])+1),model_history['val_loss'])
	axs.set_title('Model Loss')
	axs.set_ylabel('loss')
	axs.set_xlabel('Steps')
	axs.set_xticks(np.arange(1,len(model_history['train_loss'])+1),len(model_history['train_loss'])/10)
	axs.legend(['train_loss', 'val_loss'], loc='best')
	plt.show()
	# fig.savefig(os.path.join(save_path, 'test.png'))


if __name__ == '__main__':
	history = {}
	history['train_loss'] = [1,2,3,4,5,5,4,3,4,5]
	history['val_loss'] = [3,4,4,5,4,3,4,5,6,6]
	# print(len(history['acc']))
	# print(len(history['val_acc']))
	plot_model_history(history, '.')
