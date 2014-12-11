#!/usr/bin/python

import numpy as np
from matplotlib import pyplot as plt

with open("10_to_100.log") as f:
	data = f.read()

	data = data.strip().split('\n')

	x = [row.split(' ')[0] for row in data]
	y = [row.split(' ')[6] for row in data]
	z = [row.split(' ')[-1] for row in data]
	t = [row.split(' ')[5] for row in data]

	e = [row.split(' ')[7] for row in data]

	fig = plt.figure(figsize=(15,6))

	ax1 = fig.add_subplot(131)
	ax1.set_title(r'$R^2$-statistic')
	ax1.set_xlabel('Number of features selected')
	ax1.set_ylabel(r'$R^2$')
	ax1.set_ylim([0.8,1.0])
	ax1.plot(x,y, c='r', label='objective function')
	ax1.plot(x,z, c='g', label='R-squared on test data')

	ax2 = fig.add_subplot(132)
	ax2.set_title('Sum-squared error')
	ax2.set_xlabel('Number of features selected')
	ax2.set_ylabel('Sum-squared error')
	ax2.set_ylim([0.01,0.015])
	ax2.plot(x,e, c='r', label='error on test data')

	ax3 = fig.add_subplot(133)
	ax3.set_title('Training time')
	ax3.set_xlabel('Number of features selected')
	ax3.set_ylabel('Training time (in sec)')
	ax3.plot(x,t, c='b', label='training time')

	leg = ax1.legend()
	leg = ax2.legend()
	leg = ax3.legend()

#	plt.title(r'$g(S)+\eta f(S)$ with $\eta=0.00001$, $\epsilon=0.002$, $\delta=0.1$')
	plt.tight_layout()
	plt.show()
