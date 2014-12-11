#!/usr/bin/python

import numpy as np
from matplotlib import pyplot as plt

with open("time_diff.log") as f:
	data = f.read()

	data = data.strip().split('\n')

	x = range(10,100,1)
	s1 = [row.split(' ')[0] for row in data]
	s2 = [row.split(' ')[1] for row in data]

	t1 = [row.split(' ')[2] for row in data]
	t2 = [row.split(' ')[3] for row in data]

	fig = plt.figure(figsize=(15,6))

	ax1 = fig.add_subplot(121)
	ax1.set_title('optimal set')
	ax1.set_xlabel('input set')
	ax1.set_ylabel('output set')
	ax1.plot(x,s1, c='r', label='LS')
	ax1.plot(x,s2, c='g', label='Linear-time LS')

	ax2 = fig.add_subplot(122)
	ax2.set_title('time taken')
	ax2.set_xlabel('input set')
	ax2.set_ylabel('time taken (in sec)')
	ax2.plot(x,t1, c='r', label='LS')
	ax2.plot(x,t2, c='g', label='Linear-time LS')

	leg = ax1.legend()
	leg = ax2.legend()

	plt.tight_layout()
	plt.show()
