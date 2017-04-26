def say(*words, verbalise=False):
	if verbalise:
		print(list(words))

def convert_darkweights2keras(model, weigths_path, verbalise=False):
	data = np.fromfile(weights_path, np.float32)
	data = data[4:]
	say("weights shape : ", data.shape, verbalise=verbalise)
	idx = 0
	for i,layer in enumerate(model.layers):
		shape = [w.shape for w in layer.get_weights()]
		if shape != []:
			kshape,bshape = shape
			bia = data[idx:idx+np.prod(bshape)].reshape(bshape)
			idx += np.prod(bshape)
			ker = data[idx:idx+np.prod(kshape)].reshape(kshape)
			idx += np.prod(kshape)
			layer.set_weights([ker,bia])
	say("convert np weights file -> kears models", "Successful", verbalise=verbalise)
	