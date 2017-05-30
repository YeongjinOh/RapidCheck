from yolo.dataset.data import parse, shuffle

for x_batch, feeds in shuffle():
	print("x_batch.shape : ", x_batch.shape)