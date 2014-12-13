#!/usr/bin/python

import numpy as np
from matplotlib import pyplot as plt

with open("compare.log") as f:
	data = f.read()

	data = data.strip().split('\n')

	x = range(10,200,1)
	s1 = [row.split(' ')[0] for row in data]
	s2 = [row.split(' ')[1] for row in data]

	f1 = [row.split(' ')[2] for row in data]
	f2 = [row.split(' ')[3] for row in data]

	t1 = [row.split(' ')[4] for row in data]
	t2 = [row.split(' ')[5] for row in data]

	fig = plt.figure(figsize=(18,6))

	ax1 = fig.add_subplot(131)
	ax1.set_title('optimal set')
	ax1.set_xlabel('input set')
	ax1.set_ylabel('output set')
	ax1.set_xlim(10,200)
	ax1.set_ylim(10,200)
	ax1.plot(x,s1, c='r', label='LS')
	ax1.plot(x,s2, c='g', label='Linear-time LS')

	ax2 = fig.add_subplot(132)
	ax2.set_title('optimal value')
	ax2.set_xlabel('input set')
	ax2.set_ylabel(r'g(S)')
	ax2.plot(x,f1, c='r', label='LS')
	ax2.plot(x,f2, c='g', label='Linear-time LS')

	ax3 = fig.add_subplot(133)
	ax3.set_title('time taken')
	ax3.set_xlabel('input set')
	ax3.set_ylabel('time taken (in sec)')
	ax3.plot(x,t1, c='r', label='LS')
	ax3.plot(x,t2, c='g', label='Linear-time LS')

	leg = ax1.legend()
	leg = ax2.legend()
	leg = ax3.legend()

	plt.tight_layout()
	plt.show()
