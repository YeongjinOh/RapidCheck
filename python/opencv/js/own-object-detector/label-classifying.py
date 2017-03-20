from sklearn.externals import joblib
from ast import literal_eval

def parse_labels(file_name, scale=False):
	if not scale:
		with open(file_name, 'r') as fp:
			while True:
				one_line = fp.readline()
				if not one_line:
					break
				one_line = one_line.replace('\n', '')
				test_num = one_line.split(':')[0]
				ans_tuple = one_line.split(':')[1]
				ans_tuple = ans_tuple.replace(' ', '')
				ans_tuple = literal_eval(ans_tuple)
				print(one_line)
				print(ans_tuple[0], ans_tuple[1])


if __name__ == '__main__':
	parse_labels('./dataset/CarData/trueLocations.txt')
